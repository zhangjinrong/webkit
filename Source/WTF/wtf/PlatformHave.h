/*
 * Copyright (C) 2006-2020 Apple Inc. All rights reserved.
 * Copyright (C) 2007-2009 Torch Mobile, Inc.
 * Copyright (C) 2010, 2011 Research In Motion Limited. All rights reserved.
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

#pragma once

#ifndef WTF_PLATFORM_GUARD_AGAINST_INDIRECT_INCLUSION
#error "Please #include <wtf/Platform.h> instead of this file directly."
#endif


/* HAVE() - specific system features (headers, functions or similar) that are present or not */
#define HAVE(WTF_FEATURE) (defined HAVE_##WTF_FEATURE && HAVE_##WTF_FEATURE)


#if defined(HAVE_FEATURES_H) && HAVE_FEATURES_H
/* If the included features.h is glibc's one, __GLIBC__ is defined. */
#include <features.h>
#endif


#if CPU(ARM_NEON)
/* All NEON intrinsics usage can be disabled by this macro. */
#define HAVE_ARM_NEON_INTRINSICS 1
#endif

/* FIXME: This should be renamed to WTF_CPU_ARM_IDIV_INSTRUCTIONS and moved to CPU.h */
#if defined(__ARM_ARCH_EXT_IDIV__) || CPU(APPLE_ARMV7S)
#define HAVE_ARM_IDIV_INSTRUCTIONS 1
#endif

#if PLATFORM(COCOA)
#define HAVE_OUT_OF_PROCESS_LAYER_HOSTING 1
#endif

#if PLATFORM(COCOA)
#define HAVE_REMAP_JIT 1
#endif

#if PLATFORM(MAC)
#define HAVE_RUNLOOP_TIMER 1
#endif

#if PLATFORM(MAC)
#define HAVE_SEC_KEYCHAIN 1
#endif

#if PLATFORM(MAC)
#define HAVE_HISERVICES 1
#endif

#if PLATFORM(MAC) || PLATFORM(IOS_FAMILY)
#define HAVE_NETWORK_EXTENSION 1
#endif

#if PLATFORM(IOS_FAMILY)
#define HAVE_READLINE 1
#endif

#if PLATFORM(IOS_FAMILY) && CPU(ARM_NEON)
#undef HAVE_ARM_NEON_INTRINSICS
#define HAVE_ARM_NEON_INTRINSICS 0
#endif

#if !defined(HAVE_PDFHOSTVIEWCONTROLLER_SNAPSHOTTING) && PLATFORM(IOS)
#define HAVE_PDFHOSTVIEWCONTROLLER_SNAPSHOTTING 1
#endif

#if !defined(HAVE_VISIBILITY_PROPAGATION_VIEW) && PLATFORM(IOS_FAMILY)
#define HAVE_VISIBILITY_PROPAGATION_VIEW 1
#endif

#if !defined(HAVE_UISCENE) && PLATFORM(IOS_FAMILY)
#define HAVE_UISCENE 1
#endif

#if !defined(HAVE_AVSTREAMSESSION) && PLATFORM(MAC)
#define HAVE_AVSTREAMSESSION 1
#endif

#if !defined(HAVE_PASSKIT_BOUND_INTERFACE_IDENTIFIER) && (PLATFORM(IOS) || (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101400))
#define HAVE_PASSKIT_BOUND_INTERFACE_IDENTIFIER 1
#endif

#if !defined(HAVE_PASSKIT_PAYMENT_METHOD_BILLING_ADDRESS)
#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500
#define HAVE_PASSKIT_PAYMENT_METHOD_BILLING_ADDRESS 1
#endif
#endif

#if !defined(USE_UIKIT_KEYBOARD_ADDITIONS) && (PLATFORM(IOS) || PLATFORM(MACCATALYST))
#define USE_UIKIT_KEYBOARD_ADDITIONS 1
#endif

#if !defined(HAVE_ACCESSIBILITY) && (PLATFORM(COCOA) || PLATFORM(WIN) || PLATFORM(GTK) || PLATFORM(WPE))
#define HAVE_ACCESSIBILITY 1
#endif

/* FIXME: Remove after CMake build enabled on Darwin */
#if OS(DARWIN)
#define HAVE_ERRNO_H 1
#endif

#if OS(DARWIN)
#define HAVE_LANGINFO_H 1
#endif

#if OS(DARWIN)
#define HAVE_LOCALTIME_R 1
#endif

#if OS(DARWIN)
#define HAVE_MMAP 1
#endif

#if OS(DARWIN)
#define HAVE_REGEX_H 1
#endif

#if OS(DARWIN)
#define HAVE_SIGNAL_H 1
#endif

#if OS(DARWIN)
#define HAVE_STAT_BIRTHTIME 1
#endif

#if OS(DARWIN)
#define HAVE_STRINGS_H 1
#endif

#if OS(DARWIN)
#define HAVE_STRNSTR 1
#endif

#if OS(DARWIN)
#define HAVE_SYS_PARAM_H 1
#endif

#if OS(DARWIN)
#define HAVE_SYS_TIME_H 1
#endif

#if OS(DARWIN)
#define HAVE_TM_GMTOFF 1
#endif

#if OS(DARWIN)
#define HAVE_TM_ZONE 1
#endif

#if OS(DARWIN)
#define HAVE_TIMEGM 1
#endif

#if OS(DARWIN)
#define HAVE_PTHREAD_MAIN_NP 1
#endif

#if OS(DARWIN) && (CPU(X86_64) || CPU(ARM64))
#define HAVE_INT128_T 1
#endif

#if OS(UNIX) && !OS(FUCHSIA)
#define HAVE_RESOURCE_H 1
#endif

#if OS(UNIX) && !OS(FUCHSIA)
#define HAVE_PTHREAD_SETSCHEDPARAM 1
#endif

#if OS(DARWIN)
#define HAVE_DISPATCH_H 1
#endif

#if OS(DARWIN)
#define HAVE_MADV_FREE 1
#endif

#if OS(DARWIN)
#define HAVE_MADV_FREE_REUSE 1
#endif

#if OS(DARWIN)
#define HAVE_MADV_DONTNEED 1
#endif

#if OS(DARWIN)
#define HAVE_PTHREAD_SETNAME_NP 1
#endif

#if OS(DARWIN)
#define HAVE_READLINE 1
#endif

#if OS(DARWIN)
#define HAVE_SYS_TIMEB_H 1
#endif

#if OS(DARWIN)
#define HAVE_AUDIT_TOKEN 1
#endif

#if OS(DARWIN) && __has_include(<mach/mach_exc.defs>) && !PLATFORM(GTK)
#define HAVE_MACH_EXCEPTIONS 1
#endif

#if OS(DARWIN) && !PLATFORM(IOS_FAMILY)
#define HAVE_HOSTED_CORE_ANIMATION 1
#endif

#if OS(DARWIN) || OS(FUCHSIA) || ((OS(FREEBSD) || defined(__GLIBC__) || defined(__BIONIC__)) && (CPU(X86) || CPU(X86_64) || CPU(ARM) || CPU(ARM64) || CPU(MIPS)))
#define HAVE_MACHINE_CONTEXT 1
#endif

#if OS(DARWIN) || (OS(LINUX) && defined(__GLIBC__) && !defined(__UCLIBC__) && !CPU(MIPS))
#define HAVE_BACKTRACE 1
#endif

#if (OS(DARWIN) || OS(LINUX)) && (PLATFORM(GTK) || PLATFORM(WPE)) && defined(__GLIBC__) && !defined(__UCLIBC__) && !CPU(MIPS)
#define HAVE_BACKTRACE_SYMBOLS 1
#endif

#if OS(DARWIN) || OS(LINUX)
#define HAVE_DLADDR 1
#endif

#if __has_include(<System/pthread_machdep.h>)
#define HAVE_FAST_TLS 1
#endif

#if COMPILER(GCC_COMPATIBLE)
#define HAVE_COMPUTED_GOTO 1
#endif

#if CPU(ARM64E) && OS(DARWIN)
#define HAVE_FJCVTZS_INSTRUCTION 1
#endif

#if PLATFORM(IOS) || (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500)
#define HAVE_APP_LINKS 1
#endif

#if PLATFORM(IOS_FAMILY) && !PLATFORM(MACCATALYST)
#define HAVE_CELESTIAL 1
#endif

#if PLATFORM(IOS_FAMILY) && !PLATFORM(MACCATALYST)
#define HAVE_CORE_ANIMATION_RENDER_SERVER 1
#endif

#if PLATFORM(IOS_FAMILY) && !PLATFORM(MACCATALYST) && !PLATFORM(APPLETV)
#define HAVE_PARENTAL_CONTROLS_WITH_UNBLOCK_HANDLER 1
#endif

/* FIXME: Enable HAVE_PARENTAL_CONTROLS for watchOS Simulator once rdar://problem/54608386 is resolved */
#if PLATFORM(COCOA) && (!PLATFORM(APPLETV) && (!PLATFORM(WATCHOS) || !PLATFORM(IOS_FAMILY_SIMULATOR)))
#define HAVE_PARENTAL_CONTROLS 1
#endif

#if PLATFORM(COCOA) && !PLATFORM(APPLETV)
#define HAVE_AVKIT 1
#endif

#if PLATFORM(COCOA)
#define HAVE_CORE_VIDEO 1
#endif

#if PLATFORM(COCOA)
#define HAVE_MEDIA_PLAYER 1
#endif

#if PLATFORM(COCOA)
#define HAVE_AVFOUNDATION_LEGIBLE_OUTPUT_SUPPORT 1
#endif

#if PLATFORM(COCOA)
#define HAVE_MEDIA_ACCESSIBILITY_FRAMEWORK 1
#endif

#if PLATFORM(COCOA)
#define HAVE_AVFOUNDATION_LOADER_DELEGATE 1
#endif

#if PLATFORM(MAC) || PLATFORM(MACCATALYST)
#define HAVE_APPLE_GRAPHICS_CONTROL 1
#endif

#if PLATFORM(MAC) || PLATFORM(MACCATALYST)
#define HAVE_NSCURSOR 1
#endif

#if !defined(HAVE_QOS_CLASSES) && PLATFORM(COCOA)
#define HAVE_QOS_CLASSES 1
#endif

#if !defined(HAVE_VOUCHERS) && PLATFORM(COCOA)
#define HAVE_VOUCHERS 1
#endif

#if PLATFORM(COCOA)
#define HAVE_AVASSETREADER 1
#endif

#if PLATFORM(COCOA)
#define HAVE_IOSURFACE 1
#endif

#if PLATFORM(IOS_FAMILY) && !PLATFORM(IOS_FAMILY_SIMULATOR)
#define HAVE_IOSURFACE_COREIMAGE_SUPPORT 1
#endif

#if PLATFORM(IOS_FAMILY) && !PLATFORM(IOS_FAMILY_SIMULATOR) && !PLATFORM(MACCATALYST)
#define HAVE_IOSURFACE_ACCELERATOR 1
#endif

#if PLATFORM(MAC)
#define HAVE_NS_ACTIVITY 1
#endif

#if PLATFORM(COCOA)
#define HAVE_SEC_TRUST_SERIALIZATION 1
#endif

#if PLATFORM(MAC)
#define HAVE_TOUCH_BAR 1
#endif

#if PLATFORM(COCOA) && !(PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED < 101500) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define HAVE_HSTS_STORAGE_PATH 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101400) || PLATFORM(IOS) || PLATFORM(MACCATALYST)
#define HAVE_URL_FORMATTING 1
#endif

#if !OS(WINDOWS)
#define HAVE_STACK_BOUNDS_FOR_NEW_THREAD 1
#endif

#if PLATFORM(MAC) || PLATFORM(IOS) || PLATFORM(MACCATALYST)
#define HAVE_AVCONTENTKEYSESSION 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101400) || PLATFORM(IOS) || PLATFORM(MACCATALYST)
#define HAVE_SEC_KEY_PROXY 1
#endif

/* FIXME: Should this be enabled or IOS_FAMILY, not just IOS? */
#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101400) || PLATFORM(IOS)
#define HAVE_FOUNDATION_WITH_SAVE_COOKIES_WITH_COMPLETION_HANDLER 1
#endif

/* FIXME: Should this be enabled for IOS_FAMILY, not just IOS? */
#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101400) || PLATFORM(IOS)
#define HAVE_FOUNDATION_WITH_SAME_SITE_COOKIE_SUPPORT 1
#endif

#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MAX_ALLOWED < 101400
#define HAVE_NSHTTPCOOKIESTORAGE__INITWITHIDENTIFIER_WITH_INACCURATE_NULLABILITY 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101400) || PLATFORM(IOS) || PLATFORM(MACCATALYST)
#define HAVE_OS_DARK_MODE_SUPPORT 1
#endif

#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101400
#define HAVE_CG_FONT_RENDERING_GET_FONT_SMOOTHING_DISABLED 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500) || PLATFORM(IOS_FAMILY)
#define HAVE_CA_WHERE_ADDITIVE_TRANSFORMS_ARE_REVERSED 1
#endif

#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500
#define HAVE_READ_ONLY_SYSTEM_VOLUME 1
#endif

#ifdef __APPLE__
#define HAVE_FUNC_USLEEP 1
#endif

#if PLATFORM(MAC)
#define HAVE_WINDOW_SERVER_OCCLUSION_NOTIFICATIONS 1
#endif

#if PLATFORM(COCOA)
#define HAVE_SEC_ACCESS_CONTROL 1
#endif

#if PLATFORM(IOS)
/* FIXME: SafariServices.framework exists on macOS. It is only used by WebKit on iOS, so the behavior is correct, but the name is misleading. */
#define HAVE_SAFARI_SERVICES_FRAMEWORK 1
#endif

#if PLATFORM(MAC) || PLATFORM(IOS) || PLATFORM(WATCHOS) || PLATFORM(MACCATALYST)
#define HAVE_SAFE_BROWSING 1
#endif

#if PLATFORM(IOS)
#define HAVE_LINK_PREVIEW 1
#endif

#if (PLATFORM(IOS_FAMILY) || (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101400))
#define HAVE_ACCESSIBILITY_SUPPORT 1
#endif

#if PLATFORM(IOS_FAMILY) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 140000
#define HAVE_ACCESSIBILITY_BUNDLES_PATH 1
#endif

#if PLATFORM(IOS_FAMILY) || (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101400)
#define HAVE_AUTHORIZATION_STATUS_FOR_MEDIA_TYPE 1
#endif

#if PLATFORM(COCOA) && !(PLATFORM(MAC) && !(__MAC_OS_X_VERSION_MIN_REQUIRED >= 101400 && __MAC_OS_X_VERSION_MAX_ALLOWED >= 101404))
#define HAVE_CFNETWORK_OVERRIDE_SESSION_COOKIE_ACCEPT_POLICY 1
#endif

#if PLATFORM(COCOA) && !(PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED < 101500) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define HAVE_CFNETWORK_NSURLSESSION_STRICTRUSTEVALUATE 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500) || PLATFORM(IOS_FAMILY)
#define HAVE_CFNETWORK_NEGOTIATED_SSL_PROTOCOL_CIPHER 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500) || (PLATFORM(IOS_FAMILY) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 130000) || PLATFORM(MACCATALYST) || PLATFORM(WATCHOS) || PLATFORM(APPLETV)
#define HAVE_CFNETWORK_SAMESITE_COOKIE_API 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101600) || (PLATFORM(IOS_FAMILY) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 140000)
#define HAVE_CFNETWORK_METRICS_APIS_V4 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101600) || (PLATFORM(IOS_FAMILY) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 140000)
#define HAVE_CFNETWORK_ALTERNATIVE_SERVICE 1
#endif

#if (PLATFORM(IOS_FAMILY) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 140000) \
    || (PLATFORM(WATCHOS) && __WATCH_OS_VERSION_MIN_REQUIRED >= 70000) \
    || (PLATFORM(APPLETV) && __TV_OS_VERSION_MIN_REQUIRED >= 140000)
#define HAVE_RUNNINGBOARD_VISIBILITY_ASSERTIONS 1
#endif

#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500
#define HAVE_CSCHECKFIXDISABLE 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101400) || PLATFORM(IOS_FAMILY)
#define HAVE_SANDBOX_ISSUE_MACH_EXTENSION_TO_PROCESS_BY_AUDIT_TOKEN 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101400) || PLATFORM(IOS_FAMILY)
#define HAVE_SANDBOX_ISSUE_READ_EXTENSION_TO_PROCESS_BY_AUDIT_TOKEN 1
#endif

#if PLATFORM(COCOA) && !(PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED < 101500) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define HAVE_MDNS_FAST_REGISTRATION 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101400) || PLATFORM(IOS_FAMILY)
#define HAVE_DISALLOWABLE_USER_INSTALLED_FONTS 1
#endif

#if PLATFORM(COCOA) && !(PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED < 101500) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define HAVE_CTFONTCREATEFORCHARACTERSWITHLANGUAGEANDOPTION 1
#endif

#if PLATFORM(IOS) || PLATFORM(MACCATALYST)
#define HAVE_ARKIT_QUICK_LOOK_PREVIEW_ITEM 1
#endif

#if PLATFORM(IOS) || PLATFORM(MACCATALYST)
#define HAVE_UI_WK_DOCUMENT_CONTEXT 1
#endif

#if (PLATFORM(IOS) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 130400)
#define HAVE_UI_CURSOR_INTERACTION 1
#endif

#if (PLATFORM(IOS) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 130400) || PLATFORM(MACCATALYST)
#define HAVE_UIKIT_WITH_MOUSE_SUPPORT 1
#define HAVE_UI_PARALLAX_TRANSITION_GESTURE_RECOGNIZER 1
#endif

#if PLATFORM(MACCATALYST)
#define HAVE_LOOKUP_GESTURE_RECOGNIZER 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500) || PLATFORM(IOS_FAMILY)
#define HAVE_ALLOWS_SENSITIVE_LOGGING 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500) || PLATFORM(IOS_FAMILY)
#define HAVE_FAIRPLAYSTREAMING_CENC_INITDATA 1
#endif

#if PLATFORM(IOS_FAMILY) && !PLATFORM(MACCATALYST)
#define HAVE_UI_SCROLL_VIEW_INDICATOR_FLASHING_SPI 1
#endif

#if PLATFORM(IOS_FAMILY) && !PLATFORM(MACCATALYST)
#define HAVE_APP_LINKS_WITH_ISENABLED 1
#endif

#if PLATFORM(IOS)
#define HAVE_ROUTE_SHARING_POLICY_LONG_FORM_VIDEO 1
#endif

#if PLATFORM(IOS) && !PLATFORM(IOS_SIMULATOR)
#define HAVE_DEVICE_MANAGEMENT 1
#endif

#if PLATFORM(COCOA) && !(PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED < 101500)
#define HAVE_NSURLSESSION_WEBSOCKET 1
#endif

#if PLATFORM(COCOA) && !(PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED < 101500) && !PLATFORM(MACCATALYST)
#define HAVE_AVPLAYER_RESOURCE_CONSERVATION_LEVEL 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MAX_ALLOWED < 101404)
#define HAVE_VIDEO_PERFORMANCE_METRICS 1
#endif

#if PLATFORM(COCOA) && !(PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED < 101400) && !PLATFORM(MACCATALYST)
#define HAVE_CORETEXT_AUTO_OPTICAL_SIZING 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED < 101500)
#define HAVE_NSFONT_WITH_OPTICAL_SIZING_BUG 1
#endif

#if (PLATFORM(IOS) || (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500))
#define HAVE_APP_SSO 1
#endif

#if PLATFORM(COCOA) && !(PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED < 101500)
#define HAVE_TLS_PROTOCOL_VERSION_T 1
#endif

#if PLATFORM(IOS) || (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101400) || PLATFORM(WATCHOS) || PLATFORM(APPLETV) || PLATFORM(MACCATALYST)
#define HAVE_SEC_TRUST_EVALUATE_WITH_ERROR 1
#endif

#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500
#define HAVE_SUBVIEWS_IVAR_SPI 1
#endif

#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MAX_ALLOWED < 101500
#define HAVE_SUBVIEWS_IVAR_DECLARED_BY_SDK 1
#endif

#if PLATFORM(MAC) && __MAC_OS_X_VERSION_MAX_ALLOWED >= 101500
#define HAVE_AX_CLIENT_TYPE 1
#endif

#if PLATFORM(COCOA) && !(PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED < 101500) && !PLATFORM(MACCATALYST)
#define HAVE_DESIGN_SYSTEM_UI_FONTS 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101600) || (PLATFORM(IOS_FAMILY) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 140000)
#define HAVE_COOKIE_CHANGE_LISTENER_API 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500) || PLATFORM(IOS_FAMILY)
#define HAVE_DATA_PROTECTION_KEYCHAIN 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500) || (PLATFORM(IOS_FAMILY) && !PLATFORM(IOS_FAMILY_SIMULATOR))
#define HAVE_NEAR_FIELD 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101400) || PLATFORM(IOS_FAMILY)
#define HAVE_OS_SIGNPOST 1
#endif

#if PLATFORM(IOS_FAMILY) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define HAVE_SYSTEM_FONT_STYLE_TITLE_0 1
#define HAVE_SYSTEM_FONT_STYLE_TITLE_4 1
#endif

#if PLATFORM(COCOA) && !(PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED < 101400) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define HAVE_CG_PATH_UNEVEN_CORNERS_ROUNDEDRECT 1
#endif

#if PLATFORM(WATCHOS) || PLATFORM(APPLETV) || (PLATFORM(IOS_FAMILY) && !(defined __has_include && __has_include(<CoreFoundation/CFPriv.h>)))
#define HAVE_NSPROGRESS_PUBLISHING_SPI 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500) || (PLATFORM(IOS_FAMILY) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 130000)
#define HAVE_GCEXTENDEDGAMEPAD_BUTTONS_OPTIONS_MENU 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101400) || (PLATFORM(IOS_FAMILY))
#define HAVE_GCEXTENDEDGAMEPAD_BUTTONS_THUMBSTICK 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101600) || (PLATFORM(IOS_FAMILY) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 140000)
#define HAVE_WEBP 1
#define HAVE_AVSAMPLEBUFFERVIDEOOUTPUT 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101600)
#define HAVE_INCREMENTAL_PDF_APIS 1
#endif

#if (PLATFORM(IOS) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 140000)
#define HAVE_UICONTEXTMENU_LOCATION 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101600) || (PLATFORM(IOS_FAMILY) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 140000)
#define HAVE_SYSTEM_FEATURE_FLAGS 1
#endif

#if PLATFORM(IOS)
#define HAVE_AVOBSERVATIONCONTROLLER 1
#endif

#if PLATFORM(IOS_FAMILY)
#define HAVE_MENU_CONTROLLER_SHOW_HIDE_API 1
#endif

// FIXME: Should this be enabled on other iOS-family platforms?
#if PLATFORM(IOS) || PLATFORM(MACCATALYST)
#define HAVE_CANCEL_WEB_TOUCH_EVENTS_GESTURE 1
#endif

#if PLATFORM(IOS_FAMILY)
#define HAVE_UI_REMOTE_VIEW 1
#endif

#if PLATFORM(IOS_FAMILY)
#define HAVE_PDF_HOST_VIEW_CONTROLLER_WITH_BACKGROUND_COLOR 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101600)
#define HAVE_DD_HIGHLIGHT_CREATE_WITH_SCALE 1
#endif

#if PLATFORM(IOS) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 130400
#define HAVE_UISCENE_BASED_VIEW_SERVICE_STATE_NOTIFICATIONS 1
#endif

#if PLATFORM(IOS_FAMILY) && !PLATFORM(IOS_FAMILY_SIMULATOR) && !PLATFORM(MACCATALYST)
#define HAVE_IOS_JIT_RESTRICTIONS 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101600)
#define HAVE_AVAUDIO_ROUTING_ARBITER 1
#endif

#if (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101600) || (PLATFORM(MACCATALYST) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 140000)
#define HAVE_MEDIA_USAGE_FRAMEWORK 1
#endif
