
#include "cmdef.h"
#include "CLocaleHelper.h"
#include "CLocale.h"

namespace Libcore {
namespace ICU {

ECode CLocaleHelper::GetAvailableLocales(
    /* [out] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);

    return CLocale::GetAvailableLocales(locales);
}

ECode CLocaleHelper::GetDefault(
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale);

    AutoPtr<ILocale> l = CLocale::GetDefault();
    *locale = l;
    INTERFACE_ADDREF(*locale);

    return NOERROR;
}

ECode CLocaleHelper::GetLocale(
    /* [in] */ const String& localeName,
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale);

    AutoPtr<ILocale> temp;
    if (localeName.Equals("CANADA")) {
        temp = CLocale::CANADA;
    }
    else if (localeName.Equals("CANADA_FRENCH")) {
        temp = CLocale::CANADA_FRENCH;
    }
    else if (localeName.Equals("CHINA")) {
        temp = CLocale::CHINA;
    }
    else if (localeName.Equals("CHINESE")) {
        temp = CLocale::CHINESE;
    }
    else if (localeName.Equals("ENGLISH")) {
        temp = CLocale::ENGLISH;
    }
    else if (localeName.Equals("FRANCE")) {
        temp = CLocale::FRANCE;
    }
    else if (localeName.Equals("GERMAN")) {
        temp = CLocale::GERMAN;
    }
    else if (localeName.Equals("ITALIAN")) {
        temp = CLocale::ITALIAN;
    }
    else if (localeName.Equals("ITALY")) {
        temp = CLocale::ITALY;
    }
    else if (localeName.Equals("JAPAN")) {
        temp = CLocale::JAPAN;
    }
    else if (localeName.Equals("JAPANESE")) {
        temp = CLocale::JAPANESE;
    }
    else if (localeName.Equals("KOREA")) {
        temp = CLocale::KOREA;
    }
    else if (localeName.Equals("KOREAN")) {
        temp = CLocale::KOREAN;
    }
    else if (localeName.Equals("PRC")) {
        temp = CLocale::PRC;
    }
    else if (localeName.Equals("SIMPLIFIED_CHINESE")) {
        temp = CLocale::SIMPLIFIED_CHINESE;
    }
    else if (localeName.Equals("TAIWAN")) {
        temp = CLocale::TAIWAN;
    }
    else if (localeName.Equals("TRADITIONAL_CHINESE")) {
        temp = CLocale::TRADITIONAL_CHINESE;
    }
    else if (localeName.Equals("UK")) {
        temp = CLocale::UK;
    }
    else if (localeName.Equals("US")) {
        temp = CLocale::US;
    }
    *locale = temp;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}

ECode CLocaleHelper::GetISOCountries(
    /* [out] */ ArrayOf<String>** codes)
{
    VALIDATE_NOT_NULL(codes);

    return CLocale::GetISOCountries(codes);
}

ECode CLocaleHelper::GetISOLanguages(
    /* [out] */ ArrayOf<String>** codes)
{
    VALIDATE_NOT_NULL(codes);

    return CLocale::GetISOLanguages(codes);
}

ECode CLocaleHelper::SetDefault(
    /* [in] */ ILocale* locale)
{
    return CLocale::SetDefault(locale);
}

/**
 * Locale constant for en_CA.
 */
ECode CLocaleHelper::GetCANADA(
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale);
    *locale = CLocale::CANADA;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}

/**
 * Locale constant for fr_CA.
 */
ECode CLocaleHelper::GetCANADA_FRENCH(
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale);
    *locale = CLocale::CANADA_FRENCH;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}
/**
 * Locale constant for zh_CN.
 */
ECode CLocaleHelper::GetCHINA(
    /* [out] */ ILocale** locale)
{
    *locale = CLocale::CHINA;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}
/**
 * Locale constant for zh.
 */
ECode CLocaleHelper::GetCHINESE(
    /* [out] */ ILocale** locale)
{
    *locale = CLocale::CHINESE;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}
/**
 * Locale constant for en.
 */
ECode CLocaleHelper::GetENGLISH(
    /* [out] */ ILocale** locale)
{
    *locale = CLocale::ENGLISH;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}
/**
 * Locale constant for fr_FR.
 */
ECode CLocaleHelper::GetFRANCE(
    /* [out] */ ILocale** locale)
{
    *locale = CLocale::FRANCE;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}
/**
 * Locale constant for fr.
 */
ECode CLocaleHelper::GetFRENCH(
    /* [out] */ ILocale** locale)
{
    *locale = CLocale::FRENCH;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}
/**
 * Locale constant for de.
 */
ECode CLocaleHelper::GetGERMAN(
    /* [out] */ ILocale** locale)
{
    *locale = CLocale::GERMAN;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}
/**
 * Locale constant for de_DE.
 */
ECode CLocaleHelper::GetGERMANY(
    /* [out] */ ILocale** locale)
{
    *locale = CLocale::GERMANY;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}
/**
 * Locale constant for it.
 */
ECode CLocaleHelper::GetITALIAN(
    /* [out] */ ILocale** locale)
{
    *locale = CLocale::ITALIAN;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}
/**
 * Locale constant for it_IT.
 */
ECode CLocaleHelper::GetITALY(
    /* [out] */ ILocale** locale)
{
    *locale = CLocale::ITALY;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}
/**
 * Locale constant for ja_JP.
 */
ECode CLocaleHelper::GetJAPAN(
    /* [out] */ ILocale** locale)
{
    *locale = CLocale::JAPAN;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}
/**
 * Locale constant for ja.
 */
ECode CLocaleHelper::GetJAPANESE(
    /* [out] */ ILocale** locale)
{
    *locale = CLocale::JAPANESE;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}
/**
 * Locale constant for ko_KR.
 */
ECode CLocaleHelper::GetKOREA(
    /* [out] */ ILocale** locale)
{
    *locale = CLocale::KOREA;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}
/**
 * Locale constant for ko.
 */
ECode CLocaleHelper::GetKOREAN(
    /* [out] */ ILocale** locale)
{
    *locale = CLocale::KOREAN;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}
/**
 * Locale constant for zh_CN.
 */
ECode CLocaleHelper::GetPRC(
    /* [out] */ ILocale** locale)
{
    *locale = CLocale::PRC;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}
/**
 * Locale constant for the root locale. The root locale has an empty language,
 * country, and variant.
 *
 * @since 1.6
 */
ECode CLocaleHelper::GetROOT(
    /* [out] */ ILocale** locale)
{
    *locale = CLocale::ROOT;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}
/**
 * Locale constant for zh_CN.
 */
ECode CLocaleHelper::GetSIMPLIFIED_CHINESE(
    /* [out] */ ILocale** locale)
{
    *locale = CLocale::SIMPLIFIED_CHINESE;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}
/**
 * Locale constant for zh_TW.
 */
ECode CLocaleHelper::GetTAIWAN(
    /* [out] */ ILocale** locale)
{
    *locale = CLocale::TAIWAN;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}
/**
 * Locale constant for zh_TW.
 */
ECode CLocaleHelper::GetTRADITIONAL_CHINESE(
    /* [out] */ ILocale** locale)
{
    *locale = CLocale::TRADITIONAL_CHINESE;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}
/**
 * Locale constant for en_GB.
 */
ECode CLocaleHelper::GetUK(
    /* [out] */ ILocale** locale)
{
    *locale = CLocale::UK;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}
/**
 * Locale constant for en_US.
 */
ECode CLocaleHelper::GetUS(
    /* [out] */ ILocale** locale)
{
    *locale = CLocale::US;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}

} // namespace ICU
} // namespace Libcore
