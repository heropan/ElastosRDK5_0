
#include "elastos/droid/systemui/statusbar/policy/BrightnessMirrorController.h"
#include "../../R.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.Droid.Widget.h"

using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::SystemUI::R;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::Widget::IFrameLayoutLayoutParams;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Policy {

BrightnessMirrorController::Runnable1::Runnable1(
    /* [in] */ BrightnessMirrorController* host)
    : mHost(host)
{}

ECode BrightnessMirrorController::Runnable1::Run()
{
    mHost->mBrightnessMirror->SetVisibility(IView::GONE);
    return NOERROR;
}

CAR_INTERFACE_IMPL(BrightnessMirrorController, Object, IBrightnessMirrorController);
BrightnessMirrorController::BrightnessMirrorController(
    /* [in] */ IStatusBarWindowView* statusBarWindow)
    : TRANSITION_DURATION_OUT(150)
    , TRANSITION_DURATION_IN(200)
{
    mInt2Cache = ArrayOf<Int32>::Alloc(2);
    AutoPtr<IView> view;
    IView::Probe(statusBarWindow)->FindViewById(R::id::scrim_behind, (IView**)&view);
    mScrimBehind = IScrimView::Probe(view);

    IView::Probe(statusBarWindow)->FindViewById(R::id::brightness_mirror, (IView**)&mBrightnessMirror);
    IView::Probe(statusBarWindow)->FindViewById(R::id::panel_holder, (IView**)&mPanelHolder);
}

ECode BrightnessMirrorController::ShowMirror()
{
    mBrightnessMirror->SetVisibility(IView::VISIBLE);
    assert(0 && "TODO");
    // mScrimBehind->AnimateViewAlpha(0.0f, TRANSITION_DURATION_OUT, PhoneStatusBar::ALPHA_OUT);
    AutoPtr<IViewPropertyAnimator> a;
    mPanelHolder->Animate((IViewPropertyAnimator**)&a);
    AutoPtr<IViewPropertyAnimator> o = OutAnimation(a);
    o->WithLayer();
    return NOERROR;
}

ECode BrightnessMirrorController::HideMirror()
{
    assert(0 && "TODO");
    // mScrimBehind->AnimateViewAlpha(1.0f, TRANSITION_DURATION_IN, PhoneStatusBar.ALPHA_IN);
    AutoPtr<IViewPropertyAnimator> a;
    mPanelHolder->Animate((IViewPropertyAnimator**)&a);
    AutoPtr<IViewPropertyAnimator> o = InAnimation(a);
    o->WithLayer();
    AutoPtr<Runnable1> run = new Runnable1(this);
    o->WithEndAction(run);
    return NOERROR;
}

AutoPtr<IViewPropertyAnimator> BrightnessMirrorController::OutAnimation(
    /* [in] */ IViewPropertyAnimator* a)
{
    a->Alpha(0.0f);
    a->SetDuration(TRANSITION_DURATION_OUT);
    assert(0 && "TODO");
    // a->SetInterpolator(PhoneStatusBar.ALPHA_OUT);
    return a;
}

AutoPtr<IViewPropertyAnimator> BrightnessMirrorController::InAnimation(
    /* [in] */ IViewPropertyAnimator* a)
{
    a->Alpha(1.0f);
    a->SetDuration(TRANSITION_DURATION_IN);
    assert(0 && "TODO");
    // a->SetInterpolator(PhoneStatusBar.ALPHA_IN);
    return a;
}

ECode BrightnessMirrorController::SetLocation(
    /* [in] */ IView* original)
{
    original->GetLocationInWindow(mInt2Cache);
    Int32 originalY = (*mInt2Cache)[1];
    mBrightnessMirror->GetLocationInWindow(mInt2Cache);
    Int32 mirrorY = (*mInt2Cache)[1];

    Float y = 0;
    mBrightnessMirror->GetTranslationY(&y);
    mBrightnessMirror->SetTranslationY(y + originalY - mirrorY);
    return NOERROR;
}

ECode BrightnessMirrorController::GetMirror(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = mBrightnessMirror;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode BrightnessMirrorController::UpdateResources()
{
    AutoPtr<IViewGroupLayoutParams> lp;
    mBrightnessMirror->GetLayoutParams((IViewGroupLayoutParams**)&lp);

    AutoPtr<IResources> res;
    mBrightnessMirror->GetResources((IResources**)&res);
    Int32 value = 0;
    res->GetDimensionPixelSize(R::dimen::notification_panel_width, &value);
    lp->SetWidth(value);
    res->GetInteger(R::integer::notification_panel_layout_gravity, &value);
    IFrameLayoutLayoutParams::Probe(lp)->SetGravity(value);
    mBrightnessMirror->SetLayoutParams(lp);

    Int32 padding = 0;
    res->GetDimensionPixelSize(R::dimen::notification_side_padding, &padding);

    mBrightnessMirror->GetPaddingTop(&value);
    Int32 b = 0;
    mBrightnessMirror->GetPaddingBottom(&b);
    mBrightnessMirror->SetPadding(padding, value, padding, b);
    return NOERROR;
}

} // namespace Policy
} // namespace StatusBar
} // namespace SystemUI
} // namespace Droid
} // namespace Elastos
