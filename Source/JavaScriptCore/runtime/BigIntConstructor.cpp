/*
 * Copyright (C) 2017 Caio Lima <ticaiolima@gmail.com>
 * Copyright (C) 2017-2020 Apple Inc. All rights reserved.
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
#include "BigIntConstructor.h"

#include "BigIntPrototype.h"
#include "JSBigInt.h"
#include "JSCInlines.h"
#include "JSGlobalObjectFunctions.h"
#include "Lookup.h"
#include "ParseInt.h"
#include "StructureInlines.h"

namespace JSC {

static EncodedJSValue JSC_HOST_CALL bigIntConstructorFuncAsUintN(JSGlobalObject*, CallFrame*);
static EncodedJSValue JSC_HOST_CALL bigIntConstructorFuncAsIntN(JSGlobalObject*, CallFrame*);

} // namespace JSC

#include "BigIntConstructor.lut.h"

namespace JSC {

STATIC_ASSERT_IS_TRIVIALLY_DESTRUCTIBLE(BigIntConstructor);

const ClassInfo BigIntConstructor::s_info = { "Function", &Base::s_info, &bigIntConstructorTable, nullptr, CREATE_METHOD_TABLE(BigIntConstructor) };

/* Source for BigIntConstructor.lut.h
@begin bigIntConstructorTable
  asUintN   bigIntConstructorFuncAsUintN   DontEnum|Function 2
  asIntN    bigIntConstructorFuncAsIntN    DontEnum|Function 2
@end
*/

static EncodedJSValue JSC_HOST_CALL callBigIntConstructor(JSGlobalObject*, CallFrame*);

BigIntConstructor::BigIntConstructor(VM& vm, Structure* structure)
    : InternalFunction(vm, structure, callBigIntConstructor, nullptr)
{
}

void BigIntConstructor::finishCreation(VM& vm, BigIntPrototype* bigIntPrototype)
{
    Base::finishCreation(vm, "BigInt"_s, NameAdditionMode::WithoutStructureTransition);
    ASSERT(inherits(vm, info()));

    putDirectWithoutTransition(vm, vm.propertyNames->prototype, bigIntPrototype, PropertyAttribute::DontEnum | PropertyAttribute::DontDelete | PropertyAttribute::ReadOnly);
    putDirectWithoutTransition(vm, vm.propertyNames->length, jsNumber(1), PropertyAttribute::ReadOnly | PropertyAttribute::DontEnum);
}

// ------------------------------ Functions ---------------------------

static EncodedJSValue toBigInt(JSGlobalObject* globalObject, JSValue argument)
{
    ASSERT(argument.isPrimitive());
    VM& vm = globalObject->vm();
    
    if (argument.isBigInt())
        return JSValue::encode(argument);

    if (argument.isBoolean()) {
#if USE(BIGINT32)
        return JSValue::encode(JSValue(JSValue::JSBigInt32, argument.asBoolean()));
#else
        return JSValue::encode(JSBigInt::createFrom(vm, argument.asBoolean()));
#endif
    }

    if (argument.isString()) {
        return toStringView(globalObject, argument, [&] (StringView view) {
            return JSValue::encode(JSBigInt::parseInt(globalObject, view));
        });
    }

    ASSERT(argument.isUndefinedOrNull() || argument.isNumber() || argument.isSymbol());
    auto scope = DECLARE_THROW_SCOPE(vm);
    return throwVMTypeError(globalObject, scope, "Invalid argument type in ToBigInt operation"_s);
}

static EncodedJSValue JSC_HOST_CALL callBigIntConstructor(JSGlobalObject* globalObject, CallFrame* callFrame)
{
    VM& vm = globalObject->vm();
    auto scope = DECLARE_THROW_SCOPE(vm);
    
    JSValue value = callFrame->argument(0);
    JSValue primitive = value.toPrimitive(globalObject);
    RETURN_IF_EXCEPTION(scope, encodedJSValue());

    if (primitive.isInt32()) {
#if USE(BIGINT32)
        return JSValue::encode(JSValue(JSValue::JSBigInt32, primitive.asInt32()));
#else
        return JSValue::encode(JSBigInt::createFrom(vm, primitive.asInt32()));
#endif
    }

    if (primitive.isDouble()) {
        double number = primitive.asDouble();
        if (trunc(number) != number || std::abs(number) > maxSafeInteger())
            return throwVMError(globalObject, scope, createRangeError(globalObject, "Not a safe integer"_s));

        return JSValue::encode(JSBigInt::makeHeapBigIntOrBigInt32(vm, static_cast<int64_t>(primitive.asDouble())));
    }

    EncodedJSValue result = toBigInt(globalObject, primitive);
    RETURN_IF_EXCEPTION(scope, encodedJSValue());
    return result;
}

EncodedJSValue JSC_HOST_CALL bigIntConstructorFuncAsUintN(JSGlobalObject*, CallFrame*)
{
    // FIXME: [ESNext][BigInt] Implement BigInt.asIntN and BigInt.asUintN
    // https://bugs.webkit.org/show_bug.cgi?id=181144
    return encodedJSUndefined();
}

EncodedJSValue JSC_HOST_CALL bigIntConstructorFuncAsIntN(JSGlobalObject*, CallFrame*)
{
    // FIXME: [ESNext][BigInt] Implement BigInt.asIntN and BigInt.asUintN
    // https://bugs.webkit.org/show_bug.cgi?id=181144
    return encodedJSUndefined();
}

} // namespace JSC
