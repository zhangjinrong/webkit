/*
 * Copyright (C) 2020 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "AggregateErrorConstructor.h"

#include "AggregateError.h"
#include "ClassInfo.h"
#include "ExceptionScope.h"
#include "GCAssertions.h"
#include "RuntimeType.h"

namespace JSC {

STATIC_ASSERT_IS_TRIVIALLY_DESTRUCTIBLE(AggregateErrorConstructor);

const ClassInfo AggregateErrorConstructor::s_info = { "Function", &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(AggregateErrorConstructor) };

static EncodedJSValue JSC_HOST_CALL callAggregateErrorConstructor(JSGlobalObject*, CallFrame*);
static EncodedJSValue JSC_HOST_CALL constructAggregateErrorConstructor(JSGlobalObject*, CallFrame*);

AggregateErrorConstructor::AggregateErrorConstructor(VM& vm, Structure* structure)
    : Base(vm, structure, callAggregateErrorConstructor, constructAggregateErrorConstructor)
{
}

void AggregateErrorConstructor::finishCreation(VM& vm, AggregateErrorPrototype* prototype)
{
    Base::finishCreation(vm, errorTypeName(ErrorType::AggregateError), NameAdditionMode::WithoutStructureTransition);
    ASSERT(inherits(vm, info()));

    putDirectWithoutTransition(vm, vm.propertyNames->length, jsNumber(2), PropertyAttribute::DontEnum | PropertyAttribute::ReadOnly);
    putDirectWithoutTransition(vm, vm.propertyNames->prototype, prototype, PropertyAttribute::DontDelete | PropertyAttribute::ReadOnly | PropertyAttribute::DontEnum);
}

static EncodedJSValue JSC_HOST_CALL callAggregateErrorConstructor(JSGlobalObject* globalObject, CallFrame* callFrame)
{
    VM& vm = globalObject->vm();
    JSValue errors = callFrame->argument(0);
    JSValue message = callFrame->argument(1);
    Structure* errorStructure = jsCast<AggregateErrorConstructor*>(callFrame->jsCallee())->errorStructure(vm);
    return JSValue::encode(AggregateError::create(globalObject, vm, errorStructure, errors, message, nullptr, TypeNothing, false));
}

static EncodedJSValue JSC_HOST_CALL constructAggregateErrorConstructor(JSGlobalObject* globalObject, CallFrame* callFrame)
{
    VM& vm = globalObject->vm();
    auto scope = DECLARE_THROW_SCOPE(vm);
    JSValue errors = callFrame->argument(0);
    JSValue message = callFrame->argument(1);
    JSValue newTarget = callFrame->newTarget();
    ASSERT(newTarget.isObject());
    Structure* baseStructure = asObject(newTarget)->globalObject(vm)->errorStructure(ErrorType::AggregateError);
    Structure* errorStructure = InternalFunction::createSubclassStructure(globalObject, callFrame->jsCallee(), newTarget, baseStructure);
    RETURN_IF_EXCEPTION(scope, encodedJSValue());
    ASSERT(errorStructure);
    RELEASE_AND_RETURN(scope, JSValue::encode(AggregateError::create(globalObject, vm, errorStructure, errors, message, nullptr, TypeNothing, false)));
}

} // namespace JSC
