// wuweizuo automatic build .cpp file from .java file.

#include "LocalizationUtils.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

//=====================================================================
//                          LocalizationUtils
//=====================================================================
const Int32 LocalizationUtils::UNKNOWN_DIRECTION;
const Int32 LocalizationUtils::RIGHT_TO_LEFT;
const Int32 LocalizationUtils::LEFT_TO_RIGHT;
AutoPtr<Boolean> LocalizationUtils::sIsLayoutRtl;

String LocalizationUtils::GetDefaultLocale()
{
    // ==================before translated======================
    // Locale locale = Locale.getDefault();
    // String language = locale.getLanguage();
    // String country = locale.getCountry();
    //
    // // Android uses deprecated lanuages codes for Hebrew and Indonesian but Chromium uses the
    // // updated codes. Also, Android uses "tl" while Chromium uses "fil" for Tagalog/Filipino.
    // // So apply a mapping.
    // // See http://developer.android.com/reference/java/util/Locale.html
    // if ("iw".equals(language)) {
    //     language = "he";
    // } else if ("in".equals(language)) {
    //     language = "id";
    // } else if ("tl".equals(language)) {
    //     language = "fil";
    // }
    // return country.isEmpty() ? language : language + "-" + country;
    assert(0);
    return String("");
}

Boolean LocalizationUtils::IsLayoutRtl()
{
    // ==================before translated======================
    // if (sIsLayoutRtl == null) {
    //     Configuration configuration =
    //             ApplicationStatus.getApplicationContext().getResources().getConfiguration();
    //     sIsLayoutRtl = Boolean.valueOf(
    //             ApiCompatibilityUtils.getLayoutDirection(configuration) ==
    //             View.LAYOUT_DIRECTION_RTL);
    // }
    //
    // return sIsLayoutRtl.booleanValue();
    assert(0);
    return FALSE;
}

Int32 LocalizationUtils::GetFirstStrongCharacterDirection(
    /* [in] */ const String& string)
{
    // ==================before translated======================
    // return nativeGetFirstStrongCharacterDirection(string);
    assert(0);
    return 0;
}

String LocalizationUtils::GetDurationString(
    /* [in] */ Int64 timeInMillis)
{
    // ==================before translated======================
    // return nativeGetDurationString(timeInMillis);
    assert(0);
    return String("");
}

LocalizationUtils::LocalizationUtils()
{
    // ==================before translated======================
    // /* cannot be instantiated */
}

AutoPtr<ILocale> LocalizationUtils::GetJavaLocale(
    /* [in] */ const String& language,
    /* [in] */ const String& country,
    /* [in] */ const String& variant)
{
    // ==================before translated======================
    // return new Locale(language, country, variant);
    assert(0);
    AutoPtr<ILocale> empty;
    return empty;
}

String LocalizationUtils::GetDisplayNameForLocale(
    /* [in] */ ILocale* locale,
    /* [in] */ ILocale* displayLocale)
{
    // ==================before translated======================
    // return locale.getDisplayName(displayLocale);
    assert(0);
    return String("");
}

Int32 LocalizationUtils::NativeGetFirstStrongCharacterDirection(
    /* [in] */ const String& string)
{
    assert(0);
    return 0;
}

String LocalizationUtils::NativeGetDurationString(
    /* [in] */ Int64 timeInMillis)
{
    assert(0);
    return String("");
}

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


