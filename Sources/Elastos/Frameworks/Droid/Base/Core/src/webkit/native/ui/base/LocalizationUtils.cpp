// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

const Int32 LocalizationUtils::UNKNOWN_DIRECTION;
const Int32 LocalizationUtils::RIGHT_TO_LEFT;
const Int32 LocalizationUtils::LEFT_TO_RIGHT;
Boolean LocalizationUtils::sIsLayoutRtl = FALSE;

String LocalizationUtils::GetDefaultLocale()
{
    AutoPtr<ILocale> locale;
    CLocale::GetDefault((ILocale**)&locale);
    String language;
    String country;
    locale->GetLanguage(&language);
    locale->GetCountry(&country);

    // Android uses deprecated lanuages codes for Hebrew and Indonesian but Chromium uses the
    // updated codes. Also, Android uses "tl" while Chromium uses "fil" for Tagalog/Filipino.
    // So apply a mapping.
    // See http://developer.android.com/reference/java/util/Locale.html
    if (String("iw".) == language) {
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

//@CalledByNative
Boolean LocalizationUtils::IsLayoutRtl()
{
    if (FALSE == sIsLayoutRtl) {
        AutoPtr<IContext> context;
        ApplicationStatus::GetApplicationContext((IContext**)&context);

        AutoPtr<IResources> resources;
        context->GetResources((IResources**)&resources);

        AutoPtr<IConfiguration> configuration;
        resources->GetConfiguration((IConfiguration**)&configuration);

        Int32 layoutDirect;
        ApiCompatibilityUtils::GetLayoutDirection(configuration, &layoutDirect);

        sIsLayoutRtl = (Boolean)(layoutDirect == IView::LAYOUT_DIRECTION_RTL);
    }

    return sIsLayoutRtl;
}

Int32 LocalizationUtils::GetFirstStrongCharacterDirection(
    /* in */ String str)
{
    return NativeGetFirstStrongCharacterDirection(str);
}

String LocalizationUtils::GetDurationString(
    /* in */ Int64 timeInMillis)
{
    return NativeGetDurationString(timeInMillis);
}

LocalizationUtils::LocalizationUtils()
{
}

//@CalledByNative
AutoPtr<ILocale> LocalizationUtils::GetJavaLocale(
    /* in */ String language,
    /* in */ String country,
    /* in */ String variant)
{
    AutoPtr<ILocale> locale;
    CLocale::New(language, country, variant, (ILocale**)&locale);
    return locale;
}

//@CalledByNative
String LocalizationUtils::GetDisplayNameForLocale(
    /* in */ ILocale* locale,
    /* in */ ILocale* displayLocale)
{
    String result;
    locale->GetDisplayName(displayLocale, &String);
    return result;
}

Int32 LocalizationUtils::NativeGetFirstStrongCharacterDirection(
    /* in */ String str)
{
}

String LocalizationUtils::NativeGetDurationString(
    /* in */ Int64 timeInMillis)
{
}

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

