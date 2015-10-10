
#include "elastos/droid/text/TextDirectionHeuristics.h"
#include "elastos/droid/text/TextUtils.h"

using Elastos::Utility::ILocaleHelper;
using Elastos::Utility::CLocaleHelper;

namespace Elastos {
namespace Droid {
namespace Text {

const Int32 TextDirectionHeuristics::STATE_TRUE = 0;
const Int32 TextDirectionHeuristics::STATE_FALSE = 1;
const Int32 TextDirectionHeuristics::STATE_UNKNOWN = 2;

AutoPtr<ITextDirectionHeuristic> TextDirectionHeuristics::LTR;
        // new TextDirectionHeuristics::TextDirectionHeuristicInternal(NULL /* no algorithm */, FALSE);

AutoPtr<ITextDirectionHeuristic> TextDirectionHeuristics::RTL =
        // new TextDirectionHeuristics::TextDirectionHeuristicInternal(NULL /* no algorithm */, TRUE);

AutoPtr<ITextDirectionHeuristic> TextDirectionHeuristics::FIRSTSTRONG_LTR =
       // new TextDirectionHeuristics::TextDirectionHeuristicInternal(
       //      TextDirectionHeuristics::FirstStrong::GetInstance(), FALSE);

AutoPtr<ITextDirectionHeuristic> TextDirectionHeuristics::FIRSTSTRONG_RTL =
       // new TextDirectionHeuristics::TextDirectionHeuristicInternal(
       //  TextDirectionHeuristics::FirstStrong::GetInstance(), TRUE);

AutoPtr<ITextDirectionHeuristic> TextDirectionHeuristics::ANYRTL_LTR =
       // new TextDirectionHeuristics::TextDirectionHeuristicInternal(
       //  TextDirectionHeuristics::AnyStrong::GetLTRInstance(), FALSE);

AutoPtr<ITextDirectionHeuristic> TextDirectionHeuristics::LOCALE;
       //  = TextDirectionHeuristics::TextDirectionHeuristicLocale::GetInstance();

AutoPtr<TextDirectionHeuristics::AnyStrong> TextDirectionHeuristics::AnyStrong::sRTLInstance = new TextDirectionHeuristics::AnyStrong(TRUE);
AutoPtr<TextDirectionHeuristics::AnyStrong> TextDirectionHeuristics::AnyStrong::sLTRInstance = new TextDirectionHeuristics::AnyStrong(FALSE);

AutoPtr<TextDirectionHeuristics::TextDirectionHeuristicLocale> TextDirectionHeuristics::TextDirectionHeuristicLocale::sInstance
        = new TextDirectionHeuristics::TextDirectionHeuristicLocale();

AutoPtr<TextDirectionHeuristics::FirstStrong> TextDirectionHeuristics::FirstStrong::sInstance
        = new TextDirectionHeuristics::FirstStrong();


AutoPtr<ITextDirectionHeuristic> TextDirectionHeuristics::GetLTR()
{
    return new TextDirectionHeuristics::TextDirectionHeuristicInternal(NULL /* no algorithm */, FALSE);
}

AutoPtr<ITextDirectionHeuristic> TextDirectionHeuristics::GetRTL()
{
    return new TextDirectionHeuristics::TextDirectionHeuristicInternal(NULL /* no algorithm */, TRUE);
}

AutoPtr<ITextDirectionHeuristic> TextDirectionHeuristics::GetFIRSTSTRONG_LTR()
{
    return new TextDirectionHeuristics::TextDirectionHeuristicInternal(
        TextDirectionHeuristics::TextDirectionHeuristicInternal::FirstStrong::GetINSTANCE(), FALSE);
}

AutoPtr<ITextDirectionHeuristic> TextDirectionHeuristics::GetFIRSTSTRONG_RTL()
{
    return new TextDirectionHeuristics::TextDirectionHeuristicInternal(
        TextDirectionHeuristics::TextDirectionHeuristicInternal::FirstStrong::GetINSTANCE(), TRUE);
}

AutoPtr<ITextDirectionHeuristic> TextDirectionHeuristics::GetANYRTL_LTR()
{
    return new TextDirectionHeuristics::TextDirectionHeuristicInternal(
        TextDirectionHeuristics::TextDirectionHeuristicInternal::AnyStrong::GetLTRInstance(), FALSE);
}

AutoPtr<ITextDirectionHeuristic> TextDirectionHeuristics::GetLOCALE()
{
    return TextDirectionHeuristics::TextDirectionHeuristicLocale::GetInstance();
}

Int32 TextDirectionHeuristics::IsRtlText(
    /* [in] */ Int32 directionality)
{
    switch (directionality) {
        case Character::DIRECTIONALITY_LEFT_TO_RIGHT:
            return STATE_FALSE;
        case Character::DIRECTIONALITY_RIGHT_TO_LEFT:
        case Character::DIRECTIONALITY_RIGHT_TO_LEFT_ARABIC:
            return STATE_TRUE;
        default:
            return STATE_UNKNOWN;
    }
}

Int32 TextDirectionHeuristics::IsRtlTextOrFormat(
    /* [in] */ Int32 directionality)
{
    switch (directionality) {
        case Character::DIRECTIONALITY_LEFT_TO_RIGHT:
        case Character::DIRECTIONALITY_LEFT_TO_RIGHT_EMBEDDING:
        case Character::DIRECTIONALITY_LEFT_TO_RIGHT_OVERRIDE:
            return STATE_FALSE;
        case Character::DIRECTIONALITY_RIGHT_TO_LEFT:
        case Character::DIRECTIONALITY_RIGHT_TO_LEFT_ARABIC:
        case Character::DIRECTIONALITY_RIGHT_TO_LEFT_EMBEDDING:
        case Character::DIRECTIONALITY_RIGHT_TO_LEFT_OVERRIDE:
            return STATE_TRUE;
        default:
            return STATE_UNKNOWN;
    }
}

//============================================================
// TextDirectionHeuristics::TextDirectionHeuristicImpl
//============================================================
CAR_INTERFACE_IMPL(TextDirectionHeuristics::TextDirectionHeuristicImpl, Object, ITextDirectionHeuristic)

ECode TextDirectionHeuristics::TextDirectionHeuristicImpl::IsRtl(
    /* [in] */ ArrayOf<Char32>* chars,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [out] */ Boolean* result)
{
    return IsRtl(CharBuffer.wrap(array), start, count);
}

ECode TextDirectionHeuristics::TextDirectionHeuristicImpl::IsRtl(
    /* [in] */ ICharSequence* cs,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [out] */ Boolean* result);
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    if (cs == NULL || start < 0 || count < 0 || chars->GetLength() - count < start) {
//                throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (mAlgorithm == NULL) {
        *result = DefaultIsRtl();
        return NOERROR;
    }

    *result = DoCheck(chars, start, count);

    return NOERROR;
}

Boolean TextDirectionHeuristics::DoCheck(
    /* [in] */ ArrayOf<Char32>* chars,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    TriState state = mAlgorithm->CheckRtl(chars, start, count);
    switch(state) {
        case TriState_TRUE:
            return TRUE;
        case TriState_FALSE:
            return FALSE;
        default:
            return DefaultIsRtl();
    }
}

//============================================================
// TextDirectionHeuristics::TextDirectionHeuristicLocale
//============================================================

Boolean TextDirectionHeuristics::TextDirectionHeuristicLocale::DefaultIsRtl()
{
    AutoPtr<ILocale> locale;
    AutoPtr<ILocaleHelper> helper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper);
    helper->GetDefault((ILocale**)&locale);
   Int32 dir = TextUtils::GetLayoutDirectionFromLocale(locale);
   return (dir == IView::LAYOUT_DIRECTION_RTL);
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
