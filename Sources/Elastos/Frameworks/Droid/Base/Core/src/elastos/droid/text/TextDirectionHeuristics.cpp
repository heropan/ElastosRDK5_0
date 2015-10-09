
#include "elastos/droid/text/TextDirectionHeuristics.h"

namespace Elastos {
namespace Droid {
namespace Text {
#if 0
static AutoPtr<ITextDirectionHeuristic> LTRInit()
{
    return new TextDirectionHeuristics::TextDirectionHeuristicInternal(NULL /* no algorithm */, FALSE);
}

static AutoPtr<ITextDirectionHeuristic> RTLInit()
{
    return new TextDirectionHeuristics::TextDirectionHeuristicInternal(NULL /* no algorithm */, TRUE);
}

static AutoPtr<ITextDirectionHeuristic> FIRSTSTRONG_LTR_init()
{
    return new TextDirectionHeuristics::TextDirectionHeuristicInternal(
        TextDirectionHeuristics::TextDirectionHeuristicInternal::FirstStrong::GetINSTANCE(), FALSE);
}

static AutoPtr<ITextDirectionHeuristic> FIRSTSTRONG_RTL_init()
{
    return new TextDirectionHeuristics::TextDirectionHeuristicInternal(
        TextDirectionHeuristics::TextDirectionHeuristicInternal::FirstStrong::GetINSTANCE(), TRUE);
}

static AutoPtr<ITextDirectionHeuristic> ANYRTL_LTR_init()
{
    return new TextDirectionHeuristics::TextDirectionHeuristicInternal(
        TextDirectionHeuristics::TextDirectionHeuristicInternal::AnyStrong::GetINSTANCE_RTL(), FALSE);
}
#endif

const Int32 TextDirectionHeuristics::STATE_TRUE = 0;
const Int32 TextDirectionHeuristics::STATE_FALSE = 1;
const Int32 TextDirectionHeuristics::STATE_UNKNOWN = 2;

AutoPtr<ITextDirectionHeuristic> TextDirectionHeuristics::LTR =
        new TextDirectionHeuristics::TextDirectionHeuristicInternal(NULL /* no algorithm */, FALSE);//LTRInit();

AutoPtr<ITextDirectionHeuristic> TextDirectionHeuristics::RTL =
        new TextDirectionHeuristics::TextDirectionHeuristicInternal(NULL /* no algorithm */, TRUE);//RTLInit();

AutoPtr<ITextDirectionHeuristic> TextDirectionHeuristics::FIRSTSTRONG_LTR =
       new TextDirectionHeuristics::TextDirectionHeuristicInternal(
            TextDirectionHeuristics::FirstStrong::GetInstance(), FALSE);//FIRSTSTRONG_LTR_init();

AutoPtr<ITextDirectionHeuristic> TextDirectionHeuristics::FIRSTSTRONG_RTL =
       new TextDirectionHeuristics::TextDirectionHeuristicInternal(
        TextDirectionHeuristics::FirstStrong::GetInstance(), TRUE);//FIRSTSTRONG_RTL_init();

AutoPtr<ITextDirectionHeuristic> TextDirectionHeuristics::ANYRTL_LTR =
       new TextDirectionHeuristics::TextDirectionHeuristicInternal(
        TextDirectionHeuristics::AnyStrong::GetLTRInstance(), FALSE);//ANYRTL_LTR_init();

AutoPtr<ITextDirectionHeuristic> TextDirectionHeuristics::LOCALE = TextDirectionHeuristics::TextDirectionHeuristicLocale::GetInstance();

AutoPtr<TextDirectionHeuristics::AnyStrong> TextDirectionHeuristics::AnyStrong::sRTLInstance = new TextDirectionHeuristics::AnyStrong(TRUE);
AutoPtr<TextDirectionHeuristics::AnyStrong> TextDirectionHeuristics::AnyStrong::sLTRInstance = new TextDirectionHeuristics::AnyStrong(FALSE);

AutoPtr<TextDirectionHeuristics::TextDirectionHeuristicLocale> TextDirectionHeuristics::TextDirectionHeuristicLocale::sInstance
        = new TextDirectionHeuristics::TextDirectionHeuristicLocale();

AutoPtr<TextDirectionHeuristics::FirstStrong> TextDirectionHeuristics::FirstStrong::sInstance
        = new TextDirectionHeuristics::FirstStrong();


Int32 TextDirectionHeuristics::IsRtlText(
    /* [in] */ Int32 directionality)
{
    if (directionality == Character::DIRECTIONALITY_LEFT_TO_RIGHT) {
        return State_FALSE;
    } else if (directionality == Character::DIRECTIONALITY_RIGHT_TO_LEFT
            || directionality == Character::DIRECTIONALITY_RIGHT_TO_LEFT_ARABIC) {
        return State_TRUE;
    } else {
        return State_UNKNOWN;
    }
}

Int32 TextDirectionHeuristics::IsRtlTextOrFormat(
    /* [in] */ Int32 directionality)
{
    if (directionality == Character::DIRECTIONALITY_LEFT_TO_RIGHT
     || directionality == Character::DIRECTIONALITY_LEFT_TO_RIGHT_EMBEDDING
     || directionality == Character::DIRECTIONALITY_LEFT_TO_RIGHT_OVERRIDE) {
        return State_FALSE;
    } else if (directionality == Character::DIRECTIONALITY_RIGHT_TO_LEFT
            || directionality == Character::DIRECTIONALITY_RIGHT_TO_LEFT_ARABIC
            || directionality == Character::DIRECTIONALITY_RIGHT_TO_LEFT_EMBEDDING
            || directionality == Character::DIRECTIONALITY_RIGHT_TO_LEFT_OVERRIDE) {
        return State_TRUE;
    } else {
        return State_UNKNOWN;
    }
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
