/*
 * Copyright (C) 2012-2019 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "JITThunks.h"

#if ENABLE(JIT)

#include "JIT.h"
#include "JSCInlines.h"
#include "LLIntData.h"
#include "ThunkGenerators.h"
#include "VM.h"

namespace JSC {

JITThunks::JITThunks()
{
}

JITThunks::~JITThunks()
{
}

static inline NativeExecutable& getMayBeDyingNativeExecutable(const Weak<NativeExecutable>& weak)
{
    // This never gets Deleted / Empty slots.
    WeakImpl* impl = weak.unsafeImpl();
    ASSERT(impl);
    // We have a callback removing entry when finalizing. This means that we never hold Deallocated entry in HashSet.
    ASSERT(impl->state() != WeakImpl::State::Deallocated);
    // Never use jsCast here. This is possible that this value is "Dead" but not "Finalized" yet. In this case,
    // we can still access to non-JS data, as we are doing in a finalize callback.
    auto* executable = static_cast<NativeExecutable*>(impl->jsValue().asCell());
    ASSERT(executable);
    return *executable;
}

inline unsigned JITThunks::WeakNativeExecutableHash::hash(NativeExecutable* executable)
{
    return hash(executable->function(), executable->constructor(), executable->name());
}

inline unsigned JITThunks::WeakNativeExecutableHash::hash(const Weak<NativeExecutable>& key)
{
    return hash(&getMayBeDyingNativeExecutable(key));
}

inline bool JITThunks::WeakNativeExecutableHash::equal(NativeExecutable& a, NativeExecutable& b)
{
    if (&a == &b)
        return true;
    return a.function() == b.function() && a.constructor() == b.constructor() && a.name() == b.name();
}

inline bool JITThunks::WeakNativeExecutableHash::equal(const Weak<NativeExecutable>& a, const Weak<NativeExecutable>& b)
{
    return equal(getMayBeDyingNativeExecutable(a), getMayBeDyingNativeExecutable(b));
}

inline bool JITThunks::WeakNativeExecutableHash::equal(const Weak<NativeExecutable>& a, NativeExecutable* bExecutable)
{
    return equal(getMayBeDyingNativeExecutable(a), *bExecutable);
}

inline bool JITThunks::WeakNativeExecutableHash::equal(const Weak<NativeExecutable>& a, const HostFunctionKey& b)
{
    auto& aExecutable = getMayBeDyingNativeExecutable(a);
    return aExecutable.function() == std::get<0>(b) && aExecutable.constructor() == std::get<1>(b) && aExecutable.name() == std::get<2>(b);
}

MacroAssemblerCodePtr<JITThunkPtrTag> JITThunks::ctiNativeCall(VM& vm)
{
    ASSERT(VM::canUseJIT());
    return ctiStub(vm, nativeCallGenerator).code();
}

MacroAssemblerCodePtr<JITThunkPtrTag> JITThunks::ctiNativeConstruct(VM& vm)
{
    ASSERT(VM::canUseJIT());
    return ctiStub(vm, nativeConstructGenerator).code();
}

MacroAssemblerCodePtr<JITThunkPtrTag> JITThunks::ctiNativeTailCall(VM& vm)
{
    ASSERT(VM::canUseJIT());
    return ctiStub(vm, nativeTailCallGenerator).code();
}

MacroAssemblerCodePtr<JITThunkPtrTag> JITThunks::ctiNativeTailCallWithoutSavedTags(VM& vm)
{
    ASSERT(VM::canUseJIT());
    return ctiStub(vm, nativeTailCallWithoutSavedTagsGenerator).code();
}

MacroAssemblerCodePtr<JITThunkPtrTag> JITThunks::ctiInternalFunctionCall(VM& vm)
{
    ASSERT(VM::canUseJIT());
    return ctiStub(vm, internalFunctionCallGenerator).code();
}

MacroAssemblerCodePtr<JITThunkPtrTag> JITThunks::ctiInternalFunctionConstruct(VM& vm)
{
    ASSERT(VM::canUseJIT());
    return ctiStub(vm, internalFunctionConstructGenerator).code();
}

MacroAssemblerCodeRef<JITThunkPtrTag> JITThunks::ctiStub(VM& vm, ThunkGenerator generator)
{
    LockHolder locker(m_lock);
    CTIStubMap::AddResult entry = m_ctiStubMap.add(generator, MacroAssemblerCodeRef<JITThunkPtrTag>());
    if (entry.isNewEntry) {
        // Compilation thread can only retrieve existing entries.
        ASSERT(!isCompilationThread());
        entry.iterator->value = generator(vm);
    }
    return entry.iterator->value;
}

MacroAssemblerCodeRef<JITThunkPtrTag> JITThunks::existingCTIStub(ThunkGenerator generator)
{
    LockHolder locker(m_lock);
    CTIStubMap::iterator entry = m_ctiStubMap.find(generator);
    if (entry == m_ctiStubMap.end())
        return MacroAssemblerCodeRef<JITThunkPtrTag>();
    return entry->value;
}

struct JITThunks::HostKeySearcher {
    static unsigned hash(const HostFunctionKey& key) { return WeakNativeExecutableHash::hash(key); }
    static bool equal(const Weak<NativeExecutable>& a, const HostFunctionKey& b) { return WeakNativeExecutableHash::equal(a, b); }
};

struct JITThunks::NativeExecutableTranslator {
    static unsigned hash(NativeExecutable* key) { return WeakNativeExecutableHash::hash(key); }
    static bool equal(const Weak<NativeExecutable>& a, NativeExecutable* b) { return WeakNativeExecutableHash::equal(a, b); }
    static void translate(Weak<NativeExecutable>& location, NativeExecutable* executable, unsigned)
    {
        location = Weak<NativeExecutable>(executable, executable->vm().jitStubs.get());
    }
};

void JITThunks::finalize(Handle<Unknown> handle, void*)
{
    auto* nativeExecutable = static_cast<NativeExecutable*>(handle.get().asCell());
    auto hostFunctionKey = std::make_tuple(nativeExecutable->function(), nativeExecutable->constructor(), nativeExecutable->name());
    {
        DisallowGC disallowGC;
        auto iterator = m_nativeExecutableSet.find<HostKeySearcher>(hostFunctionKey);
        // Because this finalizer is called, this means that we still have dead Weak<> in m_nativeExecutableSet.
        ASSERT(iterator != m_nativeExecutableSet.end());
        ASSERT(iterator->unsafeImpl()->state() == WeakImpl::State::Finalized);
        m_nativeExecutableSet.remove(iterator);
    }
}

NativeExecutable* JITThunks::hostFunctionStub(VM& vm, TaggedNativeFunction function, TaggedNativeFunction constructor, const String& name)
{
    return hostFunctionStub(vm, function, constructor, nullptr, NoIntrinsic, nullptr, name);
}

NativeExecutable* JITThunks::hostFunctionStub(VM& vm, TaggedNativeFunction function, TaggedNativeFunction constructor, ThunkGenerator generator, Intrinsic intrinsic, const DOMJIT::Signature* signature, const String& name)
{
    ASSERT(!isCompilationThread());    
    ASSERT(VM::canUseJIT());

    auto hostFunctionKey = std::make_tuple(function, constructor, name);
    {
        DisallowGC disallowGC;
        auto iterator = m_nativeExecutableSet.find<HostKeySearcher>(hostFunctionKey);
        if (iterator != m_nativeExecutableSet.end()) {
            // It is possible that this returns Weak<> which is Dead, but not finalized.
            // We should not use this reference to store value created in the subsequent sequence, since allocating NativeExecutable can cause GC, which changes this Set.
            if (auto* executable = iterator->get())
                return executable;
        }
    }

    RefPtr<JITCode> forCall;
    if (generator) {
        MacroAssemblerCodeRef<JSEntryPtrTag> entry = generator(vm).retagged<JSEntryPtrTag>();
        forCall = adoptRef(new DirectJITCode(entry, entry.code(), JITType::HostCallThunk, intrinsic));
    } else if (signature)
        forCall = adoptRef(new NativeDOMJITCode(MacroAssemblerCodeRef<JSEntryPtrTag>::createSelfManagedCodeRef(ctiNativeCall(vm).retagged<JSEntryPtrTag>()), JITType::HostCallThunk, intrinsic, signature));
    else
        forCall = adoptRef(new NativeJITCode(MacroAssemblerCodeRef<JSEntryPtrTag>::createSelfManagedCodeRef(ctiNativeCall(vm).retagged<JSEntryPtrTag>()), JITType::HostCallThunk, intrinsic));
    
    Ref<JITCode> forConstruct = adoptRef(*new NativeJITCode(MacroAssemblerCodeRef<JSEntryPtrTag>::createSelfManagedCodeRef(ctiNativeConstruct(vm).retagged<JSEntryPtrTag>()), JITType::HostCallThunk, NoIntrinsic));
    
    NativeExecutable* nativeExecutable = NativeExecutable::create(vm, forCall.releaseNonNull(), function, WTFMove(forConstruct), constructor, name);
    {
        DisallowGC disallowGC;
        auto addResult = m_nativeExecutableSet.add<NativeExecutableTranslator>(nativeExecutable);
        if (!addResult.isNewEntry) {
            // Override the existing Weak<NativeExecutable> with the new one since it is dead.
            ASSERT(!*addResult.iterator);
            *addResult.iterator = Weak<NativeExecutable>(nativeExecutable, this);
            ASSERT(*addResult.iterator);
#if ASSERT_ENABLED
            auto iterator = m_nativeExecutableSet.find<HostKeySearcher>(hostFunctionKey);
            ASSERT(iterator != m_nativeExecutableSet.end());
            ASSERT(iterator->get() == nativeExecutable);
            ASSERT(iterator->unsafeImpl()->state() == WeakImpl::State::Live);
#endif
        }
    }
    return nativeExecutable;
}

NativeExecutable* JITThunks::hostFunctionStub(VM& vm, TaggedNativeFunction function, ThunkGenerator generator, Intrinsic intrinsic, const String& name)
{
    return hostFunctionStub(vm, function, callHostFunctionAsConstructor, generator, intrinsic, nullptr, name);
}

} // namespace JSC

#endif // ENABLE(JIT)
