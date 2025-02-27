/*
 * Copyright (C) 2015 Andy VanWagoner (andy@vanwagoner.family)
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

#include "JSObject.h"
#include <unicode/unum.h>

namespace JSC {

class JSBoundFunction;

class IntlNumberFormat final : public JSNonFinalObject {
public:
    using Base = JSNonFinalObject;

    static constexpr bool needsDestruction = true;

    static void destroy(JSCell* cell)
    {
        static_cast<IntlNumberFormat*>(cell)->IntlNumberFormat::~IntlNumberFormat();
    }

    template<typename CellType, SubspaceAccess mode>
    static IsoSubspace* subspaceFor(VM& vm)
    {
        return vm.intlNumberFormatSpace<mode>();
    }

    static IntlNumberFormat* create(VM&, Structure*);
    static Structure* createStructure(VM&, JSGlobalObject*, JSValue);

    DECLARE_INFO;

    void initializeNumberFormat(JSGlobalObject*, JSValue locales, JSValue optionsValue);
    JSValue format(JSGlobalObject*, double);
    JSValue format(JSGlobalObject*, JSBigInt*);
    JSValue formatToParts(JSGlobalObject*, double value);
    JSObject* resolvedOptions(JSGlobalObject*);

    JSBoundFunction* boundFormat() const { return m_boundFormat.get(); }
    void setBoundFormat(VM&, JSBoundFunction*);

protected:
    IntlNumberFormat(VM&, Structure*);
    void finishCreation(VM&);
    static void visitChildren(JSCell*, SlotVisitor&);

private:
    enum class Style : uint8_t { Decimal, Percent, Currency };
    enum class CurrencyDisplay : uint8_t { Code, Symbol, Name };

    struct UNumberFormatDeleter {
        void operator()(UNumberFormat*) const;
    };

    static ASCIILiteral styleString(Style);
    static ASCIILiteral currencyDisplayString(CurrencyDisplay);

    String m_locale;
    String m_numberingSystem;
    String m_currency;
    std::unique_ptr<UNumberFormat, UNumberFormatDeleter> m_numberFormat;
    WriteBarrier<JSBoundFunction> m_boundFormat;
    unsigned m_minimumIntegerDigits { 1 };
    unsigned m_minimumFractionDigits { 0 };
    unsigned m_maximumFractionDigits { 3 };
    unsigned m_minimumSignificantDigits { 0 };
    unsigned m_maximumSignificantDigits { 0 };
    Style m_style { Style::Decimal };
    CurrencyDisplay m_currencyDisplay;
    bool m_useGrouping { true };
    bool m_initializedNumberFormat { false };

    struct UFieldPositionIteratorDeleter {
        void operator()(UFieldPositionIterator*) const;
    };

    struct IntlNumberFormatField {
        int32_t type;
        int32_t size;
        IntlNumberFormatField(int32_t type, int32_t size)
            : type(type)
            , size(size)
        { }
    };

    static ASCIILiteral partTypeString(UNumberFormatFields, double);
};

} // namespace JSC
