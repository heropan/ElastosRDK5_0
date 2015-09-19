#include "ext/frameworkdef.h"
#include "text/style/LocaleSpan.h"

using Libcore::ICU::CLocale;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

LocaleSpan::LocaleSpan()
{}

LocaleSpan::LocaleSpan(
    /* [in] */ ILocale* locale)
{
    Init(locale);
}

LocaleSpan::LocaleSpan(
    /* [in] */ IParcel* src)
{
    Init(src);
}

void LocaleSpan::Init(
    /* [in] */ ILocale* locale)
{
    mLocale = locale;
}

void LocaleSpan::Init(
    /* [in] */ IParcel* src)
{
    ReadFromParcel(src);
}

Int32 LocaleSpan::GetSpanTypeId()
{
    return ITextUtils::LOCALE_SPAN;
}

Int32 LocaleSpan::DescribeContents()
{
    return 0;
}

ECode LocaleSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    String s1, s2, s3;
    FAIL_RETURN(source->ReadString(&s1));
    FAIL_RETURN(source->ReadString(&s2));
    FAIL_RETURN(source->ReadString(&s3));
    mLocale = NULL;
    return CLocale::New(s1, s2, s3, (ILocale**)&mLocale);
}

ECode LocaleSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    String strLanguage, strCountry, strVariant;
    FAIL_RETURN(dest->WriteString((mLocale->GetLanguage(&strLanguage), strLanguage)));
    FAIL_RETURN(dest->WriteString((mLocale->GetCountry(&strCountry), strCountry)));
    FAIL_RETURN(dest->WriteString((mLocale->GetVariant(&strVariant), strVariant)));
    return NOERROR;
}

AutoPtr<ILocale> LocaleSpan::GetLocale()
{
    return mLocale;
}

ECode LocaleSpan::UpdateDrawState(
    /* [in] */ ITextPaint* ds)
{
    Apply(ds, mLocale);
    return NOERROR;
}

ECode LocaleSpan::UpdateMeasureState(
    /* [in] */ ITextPaint* paint)
{
    Apply(paint, mLocale);
    return NOERROR;
}

void LocaleSpan::Apply(
    /* [in] */ IPaint* paint,
    /* [in] */ ILocale* locale)
{
    assert(paint != NULL);
    paint->SetTextLocale(locale);
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos