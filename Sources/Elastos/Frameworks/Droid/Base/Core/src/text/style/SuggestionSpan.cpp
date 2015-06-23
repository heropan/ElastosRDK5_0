#include "text/style/SuggestionSpan.h"
#include "text/CTextPaint.h"
#include "content/res/CConfiguration.h"
#include "os/SystemClock.h"
#include "R.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Res::CConfiguration;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Graphics::IColor;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

SuggestionSpan::SuggestionSpan()
{}

SuggestionSpan::SuggestionSpan(
    /* [in] */ IContext* context,
    /* [in] */ ArrayOf<String>* suggestions,
    /* [in] */ Int32 flags)
{
    Init(context, NULL, suggestions, flags, NULL);
}

SuggestionSpan::SuggestionSpan(
    /* [in] */ ILocale* locale,
    /* [in] */ ArrayOf<String>* suggestions,
    /* [in] */ Int32 flags)
{
    Init(NULL, locale, suggestions, flags, NULL);
}

SuggestionSpan::SuggestionSpan(
    /* [in] */ IContext* context,
    /* [in] */ ILocale* locale,
    /* [in] */ ArrayOf<String>* suggestions,
    /* [in] */ Int32 flags,
    /* [in] */ Handle32 notificationTargetClass)
{
    Init(context, locale, suggestions, flags, notificationTargetClass);
}

SuggestionSpan::SuggestionSpan(
    /* [in] */ IParcel* src)
{
    Init(src);
}

void SuggestionSpan::Init(
    /* [in] */ IContext* context,
    /* [in] */ ArrayOf<String>* suggestions,
    /* [in] */ Int32 flags)
{
    Init(context, NULL, suggestions, flags, NULL);
}

void SuggestionSpan::Init(
    /* [in] */ ILocale* locale,
    /* [in] */ ArrayOf<String>* suggestions,
    /* [in] */ Int32 flags)
{
    Init(NULL, locale, suggestions, flags, NULL);
}

void SuggestionSpan::Init(
    /* [in] */ IContext* context,
    /* [in] */ ILocale* locale,
    /* [in] */ ArrayOf<String>* suggestions,
    /* [in] */ Int32 flags,
    /* [in] */ Handle32 notificationTargetClass)
{
    /*const*/ Int32 N = Elastos::Core::Math::Min(ISuggestionSpan::SUGGESTIONS_MAX_SIZE, suggestions->GetLength());
    mSuggestions = ArrayOf<String>::Alloc(N);
    for(Int32 i=0; i<N; i++){
        (*mSuggestions)[i] = (*suggestions)[i];
    }
    mFlags = flags;
    if (locale != NULL) {
        locale->ToString(&mLocaleString);
    }
    else if (context != NULL) {
        AutoPtr<IResources> rs;
        context->GetResources((IResources**)&rs);
        AutoPtr<IConfiguration> cfg;
        rs->GetConfiguration((IConfiguration**)&cfg);
        AutoPtr<ILocale> localeCfg;
        cfg->GetLocale((ILocale**)&localeCfg);
        localeCfg->ToString(&mLocaleString);
    }
    else {
        Logger::E(String("SuggestionSpan"), String("No locale or context specified in SuggestionSpan constructor\n") );
        mLocaleString = "";
    }

    if (notificationTargetClass != NULL) {
//        notificationTargetClass->GetCanonicalName(&mNotificationTargetClassName);
    }
    else {
        mNotificationTargetClassName = "";
    }
    mHashCode = HashCodeInternal(mSuggestions, mLocaleString, mNotificationTargetClassName);

    InitStyle(context);
}

void SuggestionSpan::Init(
    /* [in] */ IParcel* src)
{
    ReadFromParcel(src);
}

void SuggestionSpan::InitStyle(
    /* [in] */ IContext* context)
{
    if (context == NULL) {
        mMisspelledUnderlineThickness = 0;
        mEasyCorrectUnderlineThickness = 0;
        mAutoCorrectionUnderlineThickness = 0;
        mMisspelledUnderlineColor = IColor::BLACK;
        mEasyCorrectUnderlineColor = IColor::BLACK;
        mAutoCorrectionUnderlineColor = IColor::BLACK;
        return;
    }

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::SuggestionSpan),
            ARRAY_SIZE(R::styleable::SuggestionSpan));

    Int32 defStyle = R::attr::textAppearanceMisspelledSuggestion;
    AutoPtr<ITypedArray> typedArray;
    context->ObtainStyledAttributes( NULL, attrIds, defStyle, 0, (ITypedArray**)&typedArray);
    typedArray->GetDimension( R::styleable::SuggestionSpan_textUnderlineThickness, 0, &mMisspelledUnderlineThickness);
    typedArray->GetColor( R::styleable::SuggestionSpan_textUnderlineColor, IColor::BLACK, &mMisspelledUnderlineColor);

    defStyle = R::attr::textAppearanceEasyCorrectSuggestion;
    typedArray = NULL;
    context->ObtainStyledAttributes( NULL, attrIds, defStyle, 0, (ITypedArray**)&typedArray);
    typedArray->GetDimension( R::styleable::SuggestionSpan_textUnderlineThickness, 0, &mEasyCorrectUnderlineThickness);
    typedArray->GetColor( R::styleable::SuggestionSpan_textUnderlineColor, IColor::BLACK, &mEasyCorrectUnderlineColor);

    defStyle = R::attr::textAppearanceAutoCorrectionSuggestion;
    typedArray = NULL;
    context->ObtainStyledAttributes( NULL, attrIds, defStyle, 0, (ITypedArray**)&typedArray);
    typedArray->GetDimension( R::styleable::SuggestionSpan_textUnderlineThickness, 0, &mAutoCorrectionUnderlineThickness);
    typedArray->GetColor( R::styleable::SuggestionSpan_textUnderlineColor, IColor::BLACK, &mAutoCorrectionUnderlineColor);
}

AutoPtr< ArrayOf<String> > SuggestionSpan::GetSuggestions()
{
    return mSuggestions;
}

String SuggestionSpan::GetLocale()
{
    return mLocaleString;
}

String SuggestionSpan::GetNotificationTargetClassName()
{
    return mNotificationTargetClassName;
}

Int32 SuggestionSpan::GetFlags()
{
    return mFlags;
}

void SuggestionSpan::SetFlags(
    /* [in] */ Int32 flags)
{
    mFlags = flags;
}

Int32 SuggestionSpan::DescribeContents()
{
    return 0;
}

ECode SuggestionSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    mSuggestions = NULL;
    FAIL_RETURN(source->ReadArrayOfString((ArrayOf<String>**)&mSuggestions));
    FAIL_RETURN(source->ReadInt32(&mFlags));
    FAIL_RETURN(source->ReadString(&mLocaleString));
    FAIL_RETURN(source->ReadString(&mNotificationTargetClassName));
    FAIL_RETURN(source->ReadInt32(&mHashCode));
    FAIL_RETURN(source->ReadInt32(&mEasyCorrectUnderlineColor));
    FAIL_RETURN(source->ReadFloat(&mEasyCorrectUnderlineThickness));
    FAIL_RETURN(source->ReadInt32(&mMisspelledUnderlineColor));
    FAIL_RETURN(source->ReadFloat(&mMisspelledUnderlineThickness));
    FAIL_RETURN(source->ReadInt32(&mAutoCorrectionUnderlineColor));
    FAIL_RETURN(source->ReadFloat(&mAutoCorrectionUnderlineThickness));
    return NOERROR;
}

ECode SuggestionSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteArrayOfString(mSuggestions));
    FAIL_RETURN(dest->WriteInt32(mFlags));
    FAIL_RETURN(dest->WriteString(mLocaleString));
    FAIL_RETURN(dest->WriteString(mNotificationTargetClassName));
    FAIL_RETURN(dest->WriteInt32(mHashCode));
    FAIL_RETURN(dest->WriteInt32(mEasyCorrectUnderlineColor));
    FAIL_RETURN(dest->WriteFloat(mEasyCorrectUnderlineThickness));
    FAIL_RETURN(dest->WriteInt32(mMisspelledUnderlineColor));
    FAIL_RETURN(dest->WriteFloat(mMisspelledUnderlineThickness));
    FAIL_RETURN(dest->WriteInt32(mAutoCorrectionUnderlineColor));
    FAIL_RETURN(dest->WriteFloat(mAutoCorrectionUnderlineThickness));
    return NOERROR;
}

Int32 SuggestionSpan::GetSpanTypeId()
{
    return ITextUtils::SUGGESTION_SPAN;
}

Boolean SuggestionSpan::Equals(
    /* [in] */ IInterface* o)
{
    AutoPtr<ISuggestionSpan> ss = ISuggestionSpan::Probe(o);
    if (ss != NULL) {
        Int32 hashCode;
        return (ss->GetHashCode(&hashCode), hashCode) == mHashCode;
    }
    return FALSE;
}

Int32 SuggestionSpan::GetHashCode()
{
    return mHashCode;
}

Int32 SuggestionSpan::HashCodeInternal(
    /* [in] */ ArrayOf<String>* suggestions,
    /* [in] */ const String& locale,
    /* [in] */ const String& notificationTargetClassName)
{
    //Java:    return Arrays.hashCode(new Object[] {Long.valueOf(SystemClock.uptimeMillis()), suggestions, locale, notificationTargetClassName});
    return SystemClock::GetUptimeMillis();
}

ECode SuggestionSpan::UpdateDrawState(
    /* [in] */ ITextPaint* tp)
{
    VALIDATE_NOT_NULL(tp);
    /*const*/ Boolean misspelled = (mFlags & ISuggestionSpan::FLAG_MISSPELLED) != 0;
    /*const*/ Boolean easy = (mFlags & ISuggestionSpan::FLAG_EASY_CORRECT) != 0;
    /*const*/ Boolean autoCorrection = (mFlags & ISuggestionSpan::FLAG_AUTO_CORRECTION) != 0;
    if (easy) {
        Int32 underlineColor;
        if (!misspelled) {
//            tp->SetUnderlineText(mEasyCorrectUnderlineColor, mEasyCorrectUnderlineThickness);
            tp->SetUnderlineColor(mEasyCorrectUnderlineColor);
            tp->SetUnderlineThickness(mEasyCorrectUnderlineThickness);
        }
        else if ((tp->GetUnderlineColor(&underlineColor), underlineColor) == 0) {
            // Spans are rendered in an arbitrary order. Since misspelled is less prioritary
            // than just easy, do not apply misspelled if an easy (or a mispelled) has been set
//            tp->SetUnderlineText(mMisspelledUnderlineColor, mMisspelledUnderlineThickness);
            tp->SetUnderlineColor(mMisspelledUnderlineColor);
            tp->SetUnderlineThickness(mMisspelledUnderlineThickness);
        }
    } else if (autoCorrection) {
//        tp->SetUnderlineText(mAutoCorrectionUnderlineColor, mAutoCorrectionUnderlineThickness);
        tp->SetUnderlineColor(mAutoCorrectionUnderlineColor);
        tp->SetUnderlineThickness(mAutoCorrectionUnderlineThickness);
    }
    return NOERROR;
}

Int32 SuggestionSpan::GetUnderlineColor()
{
    /*const*/ Boolean misspelled = (mFlags & ISuggestionSpan::FLAG_MISSPELLED) != 0;
    /*const*/ Boolean easy = (mFlags & ISuggestionSpan::FLAG_EASY_CORRECT) != 0;
    /*const*/ Boolean autoCorrection = (mFlags & ISuggestionSpan::FLAG_AUTO_CORRECTION) != 0;
    if (easy) {
        if (!misspelled) {
                return mEasyCorrectUnderlineColor;
        }
        else {
                return mMisspelledUnderlineColor;
        }
    }
    else if (autoCorrection) {
        return mAutoCorrectionUnderlineColor;
    }
    return 0;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos
