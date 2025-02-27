/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"
#include "JSTestEnabledForContext.h"

#include "ActiveDOMObject.h"
#include "DOMIsoSubspaces.h"
#include "Document.h"
#include "JSDOMAttribute.h"
#include "JSDOMBinding.h"
#include "JSDOMConstructorNotConstructable.h"
#include "JSDOMExceptionHandling.h"
#include "JSDOMWrapperCache.h"
#include "JSTestSubObj.h"
#include "ScriptExecutionContext.h"
#include "Settings.h"
#include "WebCoreJSClientData.h"
#include <JavaScriptCore/FunctionPrototype.h>
#include <JavaScriptCore/HeapAnalyzer.h>
#include <JavaScriptCore/JSCInlines.h>
#include <JavaScriptCore/JSDestructibleObjectHeapCellType.h>
#include <JavaScriptCore/SubspaceInlines.h>
#include <wtf/GetPtr.h>
#include <wtf/PointerPreparations.h>
#include <wtf/URL.h>


namespace WebCore {
using namespace JSC;

// Attributes

JSC::EncodedJSValue jsTestEnabledForContextConstructor(JSC::JSGlobalObject*, JSC::EncodedJSValue, JSC::PropertyName);
bool setJSTestEnabledForContextConstructor(JSC::JSGlobalObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestEnabledForContextTestSubObjEnabledForContextConstructor(JSC::JSGlobalObject*, JSC::EncodedJSValue, JSC::PropertyName);
bool setJSTestEnabledForContextTestSubObjEnabledForContextConstructor(JSC::JSGlobalObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);

class JSTestEnabledForContextPrototype final : public JSC::JSNonFinalObject {
public:
    using Base = JSC::JSNonFinalObject;
    static JSTestEnabledForContextPrototype* create(JSC::VM& vm, JSDOMGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSTestEnabledForContextPrototype* ptr = new (NotNull, JSC::allocateCell<JSTestEnabledForContextPrototype>(vm.heap)) JSTestEnabledForContextPrototype(vm, globalObject, structure);
        ptr->finishCreation(vm);
        return ptr;
    }

    DECLARE_INFO;
    template<typename CellType, JSC::SubspaceAccess>
    static JSC::IsoSubspace* subspaceFor(JSC::VM& vm)
    {
        STATIC_ASSERT_ISO_SUBSPACE_SHARABLE(JSTestEnabledForContextPrototype, Base);
        return &vm.plainObjectSpace;
    }
    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

private:
    JSTestEnabledForContextPrototype(JSC::VM& vm, JSC::JSGlobalObject*, JSC::Structure* structure)
        : JSC::JSNonFinalObject(vm, structure)
    {
    }

    void finishCreation(JSC::VM&);
};
STATIC_ASSERT_ISO_SUBSPACE_SHARABLE(JSTestEnabledForContextPrototype, JSTestEnabledForContextPrototype::Base);

using JSTestEnabledForContextConstructor = JSDOMConstructorNotConstructable<JSTestEnabledForContext>;

template<> JSValue JSTestEnabledForContextConstructor::prototypeForStructure(JSC::VM& vm, const JSDOMGlobalObject& globalObject)
{
    UNUSED_PARAM(vm);
    return globalObject.functionPrototype();
}

template<> void JSTestEnabledForContextConstructor::initializeProperties(VM& vm, JSDOMGlobalObject& globalObject)
{
    putDirect(vm, vm.propertyNames->prototype, JSTestEnabledForContext::prototype(vm, globalObject), JSC::PropertyAttribute::DontDelete | JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum);
    putDirect(vm, vm.propertyNames->name, jsNontrivialString(vm, String("TestEnabledForContext"_s)), JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum);
    putDirect(vm, vm.propertyNames->length, jsNumber(0), JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum);
}

template<> const ClassInfo JSTestEnabledForContextConstructor::s_info = { "TestEnabledForContext", &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestEnabledForContextConstructor) };

/* Hash table for prototype */

static const HashTableValue JSTestEnabledForContextPrototypeTableValues[] =
{
    { "constructor", static_cast<unsigned>(JSC::PropertyAttribute::DontEnum), NoIntrinsic, { (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestEnabledForContextConstructor), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(setJSTestEnabledForContextConstructor) } },
};

const ClassInfo JSTestEnabledForContextPrototype::s_info = { "TestEnabledForContextPrototype", &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestEnabledForContextPrototype) };

void JSTestEnabledForContextPrototype::finishCreation(VM& vm)
{
    Base::finishCreation(vm);
    reifyStaticProperties(vm, JSTestEnabledForContext::info(), JSTestEnabledForContextPrototypeTableValues, *this);
}

const ClassInfo JSTestEnabledForContext::s_info = { "TestEnabledForContext", &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestEnabledForContext) };

JSTestEnabledForContext::JSTestEnabledForContext(Structure* structure, JSDOMGlobalObject& globalObject, Ref<TestEnabledForContext>&& impl)
    : JSDOMWrapper<TestEnabledForContext>(structure, globalObject, WTFMove(impl))
{
}

void JSTestEnabledForContext::finishCreation(VM& vm)
{
    Base::finishCreation(vm);
    ASSERT(inherits(vm, info()));

    static_assert(!std::is_base_of<ActiveDOMObject, TestEnabledForContext>::value, "Interface is not marked as [ActiveDOMObject] even though implementation class subclasses ActiveDOMObject.");

    if ((downcast<Document>(jsCast<JSDOMGlobalObject*>(globalObject())->scriptExecutionContext())->settings().testSettingEnabled() && TestSubObjEnabledForContext::enabledForContext(*jsCast<JSDOMGlobalObject*>(globalObject())->scriptExecutionContext())))
        putDirectCustomAccessor(vm, static_cast<JSVMClientData*>(vm.clientData)->builtinNames().TestSubObjEnabledForContextPublicName(), CustomGetterSetter::create(vm, jsTestEnabledForContextTestSubObjEnabledForContextConstructor, setJSTestEnabledForContextTestSubObjEnabledForContextConstructor), attributesForStructure(static_cast<unsigned>(JSC::PropertyAttribute::DontEnum)));
}

JSObject* JSTestEnabledForContext::createPrototype(VM& vm, JSDOMGlobalObject& globalObject)
{
    return JSTestEnabledForContextPrototype::create(vm, &globalObject, JSTestEnabledForContextPrototype::createStructure(vm, &globalObject, globalObject.objectPrototype()));
}

JSObject* JSTestEnabledForContext::prototype(VM& vm, JSDOMGlobalObject& globalObject)
{
    return getDOMPrototype<JSTestEnabledForContext>(vm, globalObject);
}

JSValue JSTestEnabledForContext::getConstructor(VM& vm, const JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestEnabledForContextConstructor>(vm, *jsCast<const JSDOMGlobalObject*>(globalObject));
}

void JSTestEnabledForContext::destroy(JSC::JSCell* cell)
{
    JSTestEnabledForContext* thisObject = static_cast<JSTestEnabledForContext*>(cell);
    thisObject->JSTestEnabledForContext::~JSTestEnabledForContext();
}

template<> inline JSTestEnabledForContext* IDLAttribute<JSTestEnabledForContext>::cast(JSGlobalObject& lexicalGlobalObject, EncodedJSValue thisValue)
{
    return jsDynamicCast<JSTestEnabledForContext*>(JSC::getVM(&lexicalGlobalObject), JSValue::decode(thisValue));
}

EncodedJSValue jsTestEnabledForContextConstructor(JSGlobalObject* lexicalGlobalObject, EncodedJSValue thisValue, PropertyName)
{
    VM& vm = JSC::getVM(lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    auto* prototype = jsDynamicCast<JSTestEnabledForContextPrototype*>(vm, JSValue::decode(thisValue));
    if (UNLIKELY(!prototype))
        return throwVMTypeError(lexicalGlobalObject, throwScope);
    return JSValue::encode(JSTestEnabledForContext::getConstructor(JSC::getVM(lexicalGlobalObject), prototype->globalObject()));
}

bool setJSTestEnabledForContextConstructor(JSGlobalObject* lexicalGlobalObject, EncodedJSValue thisValue, EncodedJSValue encodedValue)
{
    VM& vm = JSC::getVM(lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    auto* prototype = jsDynamicCast<JSTestEnabledForContextPrototype*>(vm, JSValue::decode(thisValue));
    if (UNLIKELY(!prototype)) {
        throwVMTypeError(lexicalGlobalObject, throwScope);
        return false;
    }
    // Shadowing a built-in constructor
    return prototype->putDirect(vm, vm.propertyNames->constructor, JSValue::decode(encodedValue));
}

static inline JSValue jsTestEnabledForContextTestSubObjEnabledForContextConstructorGetter(JSGlobalObject& lexicalGlobalObject, JSTestEnabledForContext& thisObject, ThrowScope& throwScope)
{
    UNUSED_PARAM(throwScope);
    UNUSED_PARAM(lexicalGlobalObject);
    return JSTestSubObj::getConstructor(JSC::getVM(&lexicalGlobalObject), thisObject.globalObject());
}

EncodedJSValue jsTestEnabledForContextTestSubObjEnabledForContextConstructor(JSGlobalObject* lexicalGlobalObject, EncodedJSValue thisValue, PropertyName)
{
    return IDLAttribute<JSTestEnabledForContext>::get<jsTestEnabledForContextTestSubObjEnabledForContextConstructorGetter>(*lexicalGlobalObject, thisValue, "TestSubObjEnabledForContext");
}

static inline bool setJSTestEnabledForContextTestSubObjEnabledForContextConstructorSetter(JSGlobalObject& lexicalGlobalObject, JSTestEnabledForContext& thisObject, JSValue value, ThrowScope& throwScope)
{
    UNUSED_PARAM(lexicalGlobalObject);
    VM& vm = throwScope.vm();
    // Shadowing a built-in constructor.
    return thisObject.putDirect(vm, Identifier::fromString(vm, reinterpret_cast<const LChar*>("TestSubObjEnabledForContext"), strlen("TestSubObjEnabledForContext")), value);
}

bool setJSTestEnabledForContextTestSubObjEnabledForContextConstructor(JSGlobalObject* lexicalGlobalObject, EncodedJSValue thisValue, EncodedJSValue encodedValue)
{
    return IDLAttribute<JSTestEnabledForContext>::set<setJSTestEnabledForContextTestSubObjEnabledForContextConstructorSetter>(*lexicalGlobalObject, thisValue, encodedValue, "TestSubObjEnabledForContext");
}

JSC::IsoSubspace* JSTestEnabledForContext::subspaceForImpl(JSC::VM& vm)
{
    auto& clientData = *static_cast<JSVMClientData*>(vm.clientData);
    auto& spaces = clientData.subspaces();
    if (auto* space = spaces.m_subspaceForTestEnabledForContext.get())
        return space;
    static_assert(std::is_base_of_v<JSC::JSDestructibleObject, JSTestEnabledForContext> || !JSTestEnabledForContext::needsDestruction);
    if constexpr (std::is_base_of_v<JSC::JSDestructibleObject, JSTestEnabledForContext>)
        spaces.m_subspaceForTestEnabledForContext = makeUnique<IsoSubspace> ISO_SUBSPACE_INIT(vm.heap, vm.destructibleObjectHeapCellType.get(), JSTestEnabledForContext);
    else
        spaces.m_subspaceForTestEnabledForContext = makeUnique<IsoSubspace> ISO_SUBSPACE_INIT(vm.heap, vm.cellHeapCellType.get(), JSTestEnabledForContext);
    auto* space = spaces.m_subspaceForTestEnabledForContext.get();
IGNORE_WARNINGS_BEGIN("unreachable-code")
IGNORE_WARNINGS_BEGIN("tautological-compare")
    if (&JSTestEnabledForContext::visitOutputConstraints != &JSC::JSCell::visitOutputConstraints)
        clientData.outputConstraintSpaces().append(space);
IGNORE_WARNINGS_END
IGNORE_WARNINGS_END
    return space;
}

void JSTestEnabledForContext::analyzeHeap(JSCell* cell, HeapAnalyzer& analyzer)
{
    auto* thisObject = jsCast<JSTestEnabledForContext*>(cell);
    analyzer.setWrappedObjectForCell(cell, &thisObject->wrapped());
    if (thisObject->scriptExecutionContext())
        analyzer.setLabelForCell(cell, "url " + thisObject->scriptExecutionContext()->url().string());
    Base::analyzeHeap(cell, analyzer);
}

bool JSTestEnabledForContextOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor, const char** reason)
{
    UNUSED_PARAM(handle);
    UNUSED_PARAM(visitor);
    UNUSED_PARAM(reason);
    return false;
}

void JSTestEnabledForContextOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    auto* jsTestEnabledForContext = static_cast<JSTestEnabledForContext*>(handle.slot()->asCell());
    auto& world = *static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, &jsTestEnabledForContext->wrapped(), jsTestEnabledForContext);
}

#if ENABLE(BINDING_INTEGRITY)
#if PLATFORM(WIN)
#pragma warning(disable: 4483)
extern "C" { extern void (*const __identifier("??_7TestEnabledForContext@WebCore@@6B@")[])(); }
#else
extern "C" { extern void* _ZTVN7WebCore21TestEnabledForContextE[]; }
#endif
#endif

JSC::JSValue toJSNewlyCreated(JSC::JSGlobalObject*, JSDOMGlobalObject* globalObject, Ref<TestEnabledForContext>&& impl)
{

#if ENABLE(BINDING_INTEGRITY)
    const void* actualVTablePointer = getVTablePointer(impl.ptr());
#if PLATFORM(WIN)
    void* expectedVTablePointer = __identifier("??_7TestEnabledForContext@WebCore@@6B@");
#else
    void* expectedVTablePointer = &_ZTVN7WebCore21TestEnabledForContextE[2];
#endif

    // If this fails TestEnabledForContext does not have a vtable, so you need to add the
    // ImplementationLacksVTable attribute to the interface definition
    static_assert(std::is_polymorphic<TestEnabledForContext>::value, "TestEnabledForContext is not polymorphic");

    // If you hit this assertion you either have a use after free bug, or
    // TestEnabledForContext has subclasses. If TestEnabledForContext has subclasses that get passed
    // to toJS() we currently require TestEnabledForContext you to opt out of binding hardening
    // by adding the SkipVTableValidation attribute to the interface IDL definition
    RELEASE_ASSERT(actualVTablePointer == expectedVTablePointer);
#endif
    return createWrapper<TestEnabledForContext>(globalObject, WTFMove(impl));
}

JSC::JSValue toJS(JSC::JSGlobalObject* lexicalGlobalObject, JSDOMGlobalObject* globalObject, TestEnabledForContext& impl)
{
    return wrap(lexicalGlobalObject, globalObject, impl);
}

TestEnabledForContext* JSTestEnabledForContext::toWrapped(JSC::VM& vm, JSC::JSValue value)
{
    if (auto* wrapper = jsDynamicCast<JSTestEnabledForContext*>(vm, value))
        return &wrapper->wrapped();
    return nullptr;
}

}
