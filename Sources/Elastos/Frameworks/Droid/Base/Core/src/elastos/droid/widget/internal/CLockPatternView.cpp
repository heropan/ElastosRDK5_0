#include "elastos/droid/widget/internal/CLockPatternView.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

IVIEW_METHODS_IMPL(CLockPatternView, LockPatternView)
IDRAWABLECALLBACK_METHODS_IMPL(CLockPatternView, LockPatternView)
IKEYEVENTCALLBACK_METHODS_IMPL(CLockPatternView, LockPatternView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CLockPatternView, LockPatternView)

PInterface CLockPatternView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CLockPatternView::Probe(riid);
}

CARAPI CLockPatternView::constructor(
    /* [in] */ IContext* context)
{
    return LockPatternView::Init(context);
}

CARAPI CLockPatternView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return LockPatternView::Init(context, attrs);
}

CARAPI CLockPatternView::IsInStealthMode(
    /* [out] */ Boolean* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = LockPatternView::IsInStealthMode();
    return NOERROR;
}

CARAPI CLockPatternView::IsTactileFeedbackEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = LockPatternView::IsTactileFeedbackEnabled();
    return NOERROR;
}

CARAPI CLockPatternView::SetInStealthMode(
   /* [in] */ Boolean inStealthMode)
{
    return LockPatternView::SetInStealthMode(inStealthMode);
}

CARAPI CLockPatternView::SetTactileFeedbackEnabled(
   /* [in] */ Boolean tactileFeedbackEnabled)
{
    return LockPatternView::SetTactileFeedbackEnabled(tactileFeedbackEnabled);
}

CARAPI CLockPatternView::SetOnPatternListener(
   /* [in] */ IOnPatternListener* onPatternListener)
{
    return LockPatternView::SetOnPatternListener(onPatternListener);
}

CARAPI CLockPatternView::SetPattern(
   /* [in] */ DisplayMode displayMode,
   /* [in] */ IObjectContainer* pattern)
{
    return LockPatternView::SetPattern(displayMode, pattern);
}

CARAPI CLockPatternView::SetDisplayMode(
   /* [in] */ DisplayMode displayMode)
{
    return LockPatternView::SetDisplayMode(displayMode);
}

CARAPI CLockPatternView::ClearPattern()
{
    return LockPatternView::ClearPattern();
}

CARAPI CLockPatternView::DisableInput()
{
    return LockPatternView::DisableInput();
}

CARAPI CLockPatternView::EnableInput()
{
    return LockPatternView::EnableInput();
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos