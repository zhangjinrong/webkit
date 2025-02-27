/*
 * Copyright (C) 2015 Andy VanWagoner (andy@vanwagoner.family)
 * Copyright (C) 2019-2020 Apple Inc. All rights reserved.
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

#pragma once

#include "JSCJSValueInlines.h"
#include "JSObject.h"

namespace JSC {

class IntlObject final : public JSNonFinalObject {
public:
    using Base = JSNonFinalObject;
    static constexpr unsigned StructureFlags = Base::StructureFlags | HasStaticPropertyTable;

    template<typename CellType, SubspaceAccess>
    static IsoSubspace* subspaceFor(VM& vm)
    {
        STATIC_ASSERT_ISO_SUBSPACE_SHARABLE(IntlObject, Base);
        return &vm.plainObjectSpace;
    }

    static IntlObject* create(VM&, Structure*);
    static Structure* createStructure(VM&, JSGlobalObject*, JSValue);

    DECLARE_INFO;

private:
    IntlObject(VM&, Structure*);
};

String defaultLocale(JSGlobalObject*);
const HashSet<String>& intlCollatorAvailableLocales();
const HashSet<String>& intlDateTimeFormatAvailableLocales();
const HashSet<String>& intlNumberFormatAvailableLocales();
inline const HashSet<String>& intlPluralRulesAvailableLocales() { return intlNumberFormatAvailableLocales(); }

bool intlBooleanOption(JSGlobalObject*, JSValue options, PropertyName, bool& usesFallback);
String intlStringOption(JSGlobalObject*, JSValue options, PropertyName, std::initializer_list<const char*> values, const char* notFound, const char* fallback);
unsigned intlNumberOption(JSGlobalObject*, JSValue options, PropertyName, unsigned minimum, unsigned maximum, unsigned fallback);
unsigned intlDefaultNumberOption(JSGlobalObject*, JSValue, PropertyName, unsigned minimum, unsigned maximum, unsigned fallback);
Vector<String> canonicalizeLocaleList(JSGlobalObject*, JSValue locales);
HashMap<String, String> resolveLocale(JSGlobalObject*, const HashSet<String>& availableLocales, const Vector<String>& requestedLocales, const HashMap<String, String>& options, const char* const relevantExtensionKeys[], size_t relevantExtensionKeyCount, Vector<String> (*localeData)(const String&, size_t));
JSValue supportedLocales(JSGlobalObject*, const HashSet<String>& availableLocales, const Vector<String>& requestedLocales, JSValue options);
String removeUnicodeLocaleExtension(const String& locale);
String bestAvailableLocale(const HashSet<String>& availableLocales, const String& requestedLocale);
Vector<String> numberingSystemsForLocale(const String& locale);

bool isUnicodeLocaleIdentifierType(StringView);

} // namespace JSC
