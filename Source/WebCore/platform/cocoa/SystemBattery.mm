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

#import "config.h"
#import "SystemBattery.h"

#import <pal/spi/cocoa/IOPSLibSPI.h>

namespace WebCore {

static Optional<bool> hasBattery;

void setSystemHasBattery(bool battery)
{
    hasBattery = battery;
}

bool systemHasBattery()
{
    if (!hasBattery.hasValue()) {
        hasBattery = [] {
            RetainPtr<CFTypeRef> powerSourcesInfo = adoptCF(IOPSCopyPowerSourcesInfo());
            if (!powerSourcesInfo)
                return false;
            RetainPtr<CFArrayRef> powerSourcesList = adoptCF(IOPSCopyPowerSourcesList(powerSourcesInfo.get()));
            if (!powerSourcesList)
                return false;
            for (CFIndex i = 0, count = CFArrayGetCount(powerSourcesList.get()); i < count; ++i) {
                CFDictionaryRef description = IOPSGetPowerSourceDescription(powerSourcesInfo.get(), CFArrayGetValueAtIndex(powerSourcesList.get(), i));
                CFTypeRef value = CFDictionaryGetValue(description, CFSTR(kIOPSTypeKey));
                if (!value || CFEqual(value, CFSTR(kIOPSInternalBatteryType)))
                    return true;
            }
            return false;
        }();
    }

    return *hasBattery;
}

}
