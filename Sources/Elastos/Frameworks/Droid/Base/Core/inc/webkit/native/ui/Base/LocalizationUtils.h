
// wuweizuo automatic build .h file from .java file.
// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_LOCALIZATIONUTILS_H_
#define _ELASTOS_DROID_WEBKIT_UI_LOCALIZATIONUTILS_H_

// package org.chromium.ui.base;
// import android.content.res.Configuration;
// import android.view.View;
// import org.chromium.base.ApiCompatibilityUtils;
// import org.chromium.base.ApplicationStatus;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import java.util.Locale;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

/**
 * This class provides the locale related methods for the native library.
 */
//@JNINamespace("l10n_util")
class LocalizationUtils
{
public:
    /**
     * @return the default locale, translating Android deprecated
     * language codes into the modern ones used by Chromium.
     */
    //@CalledByNative
    static CARAPI_(String) GetDefaultLocale();

    /**
     * Returns whether the Android layout direction is RTL.
     *
     * Note that the locale direction can be different from layout direction. Two known cases:
     * - RTL languages on Android 4.1, due to the lack of RTL layout support on 4.1.
     * - When user turned on force RTL layout option under developer options.
     *
     * Therefore, only this function should be used to query RTL for layout purposes.
     */
    //@CalledByNative
    static CARAPI_(Boolean) IsLayoutRtl();

    /**
     * Jni binding to base::i18n::GetFirstStrongCharacterDirection
     * @param string String to decide the direction.
     * @return One of the UNKNOWN_DIRECTION, RIGHT_TO_LEFT, and LEFT_TO_RIGHT.
     */
    static CARAPI_(Int32) GetFirstStrongCharacterDirection(
        /* in */ String string);

    /**
     * Jni binding to ui::TimeFormat::TimeRemaining. Converts milliseconds to
     * time remaining format : "3 mins left", "2 days left".
     * @param timeInMillis time in milliseconds
     * @return time remaining
     */
    static CARAPI_(String) GetDurationString(
        /* in */ Int64 timeInMillis);

private:
    LocalizationUtils();

    //@CalledByNative
    static CARAPI_(AutoPtr<ILocale>) GetJavaLocale(
        /* in */ String language,
        /* in */ String country,
        /* in */ String variant);

    //@CalledByNative
    static CARAPI_(String) GetDisplayNameForLocale(
        /* in */ ILocale* locale,
        /* in */ ILocale* displayLocale);

    static CARAPI_(Int32) NativeGetFirstStrongCharacterDirection(
        /* in */ String str);

    static CARAPI_(String) NativeGetDurationString(
        /* in */ Int64 timeInMillis);

public:
    // This is mirrored from base/i18n/rtl.h. Please keep in sync.
    static const Int32 UNKNOWN_DIRECTION = 0;
    static const Int32 RIGHT_TO_LEFT = 1;
    static const Int32 LEFT_TO_RIGHT = 2;

private:
    static Boolean sIsLayoutRtl;
};

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_LOCALIZATIONUTILS_H_

