#include "CViewScaller.h"
#include "view/View.h"

using Elastos::Droid::View::IViewGroupLayoutParams;

namespace Elastos {
namespace Droid {
namespace SystemUI {

ECode CViewScaller::constructor()
{
    return NOERROR;
}

ECode CViewScaller::SetView(
    /* [in] */ IView* v)
{
    mView = v;
    return NOERROR;
}

ECode CViewScaller::SetHeight(
    /* [in] */ Float h)
{
    // if (DEBUG_SCALE) Slog.v(TAG, "SetHeight: setting to " + h);
    AutoPtr<IViewGroupLayoutParams> lp;
    mView->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    lp->SetHeight(h);
    mView->SetLayoutParams(lp);
    mView->RequestLayout();
    return NOERROR;
}

ECode CViewScaller::GetHeight(
    /* [out] */ Float* rst)
{
    Int32 height;
    AutoPtr<IViewGroupLayoutParams> lp;
    mView->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    lp->GetHeight(&height);
    if (height < 0) {
        mView->GetMeasuredHeight(&height);
    }
    *rst = height;
    return NOERROR;
}

ECode CViewScaller::GetNaturalHeight(
    /* [in] */ Int32 maximum,
    /* [out] */ Int32* naturalHeight)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    mView->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    // if (DEBUG_SCALE) Slog.v(TAG, "Inspecting a child of type: " +
    //         mView.getClass().getName());
    Int32 oldHeight;
    lp->GetHeight(&oldHeight);
    lp->SetHeight(IViewGroupLayoutParams::WRAP_CONTENT);
    mView->SetLayoutParams(lp);
    Int32 width;
    mView->GetMeasuredWidth(&width);
    mView->Measure(
            Elastos::Droid::View::View::MeasureSpec::MakeMeasureSpec(width,
                                             Elastos::Droid::View::View::MeasureSpec::EXACTLY),
            Elastos::Droid::View::View::MeasureSpec::MakeMeasureSpec(maximum,
                                             Elastos::Droid::View::View::MeasureSpec::AT_MOST));
    lp->SetHeight(oldHeight);
    mView->SetLayoutParams(lp);
    mView->GetMeasuredHeight(naturalHeight);
    return NOERROR;
}

}// namespace SystemUI
}// namespace Droid
}// namespace Elastos
