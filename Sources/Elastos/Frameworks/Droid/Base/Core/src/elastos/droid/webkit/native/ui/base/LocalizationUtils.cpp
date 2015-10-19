
#include "elastos/droid/webkit/native/ui/base/LocalizationUtils.h"
#include "elastos/droid/webkit/native/base/ApplicationStatus.h"
#include "elastos/droid/webkit/native/base/ApiCompatibilityUtils.h"

using Elastos::Utility::CLocale;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Webkit::Base::ApplicationStatus;
using Elastos::Droid::Webkit::Base::ApiCompatibilityUtils;

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
Boolean LocalizationUtils::sIsLayoutRtl;

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

    AutoPtr<ILocale> locale;
    CLocale::New((ILocale**)&locale);
    String language;
    String country;
    locale->GetLanguage(&language);
    locale->GetCountry(&country);

    // Android uses deprecated lanuages codes for Hebrew and Indonesian but Chromium uses the
    // updated codes. Also, Android uses "tl" while Chromium uses "fil" for Tagalog/Filipino.
    // So apply a mapping.
    // See http://developer.android.com/reference/java/util/Locale.html
    if (String("iw") == language) {
        language = String("he");
    }
    else if (String("in") == language) {
        language = String("id");
    }
    else if (String("tl") == language) {
        language = String("fil");
    }
    return country.IsEmpty() ? language : language + String("-") + country;
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
    if (FALSE == sIsLayoutRtl) {
        AutoPtr<IContext> context = ApplicationStatus::GetApplicationContext();
        AutoPtr<IResources> resources;
        context->GetResources((IResources**)&resources);

        AutoPtr<IConfiguration> configuration;
        resources->GetConfiguration((IConfiguration**)&configuration);

        Int32 layoutDirect = ApiCompatibilityUtils::GetLayoutDirection(configuration);
        sIsLayoutRtl = (Boolean)(layoutDirect == IView::LAYOUT_DIRECTION_RTL);
    }

    return sIsLayoutRtl;
}

Int32 LocalizationUtils::GetFirstStrongCharacterDirection(
    /* [in] */ const String& string)
{
    // ==================before translated======================
    // return nativeGetFirstStrongCharacterDirection(string);

    return NativeGetFirstStrongCharacterDirection(string);
}

String LocalizationUtils::GetDurationString(
    /* [in] */ Int64 timeInMillis)
{
    // ==================before translated======================
    // return nativeGetDurationString(timeInMillis);

    return NativeGetDurationString(timeInMillis);
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

    AutoPtr<ILocale> locale;
    CLocale::New(language, country, variant, (ILocale**)&locale);
    return locale;
}

String LocalizationUtils::GetDisplayNameForLocale(
    /* [in] */ ILocale* locale,
    /* [in] */ ILocale* displayLocale)
{
    // ==================before translated======================
    // return locale.getDisplayName(displayLocale);

    String result;
    locale->GetDisplayName(displayLocale, &result);
    return result;
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


