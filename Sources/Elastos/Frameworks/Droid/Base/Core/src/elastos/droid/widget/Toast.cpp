
#include "elastos/droid/widget/Toast.h"
#include "elastos/droid/widget/CToast.h"
#include "elastos/droid/R.h"
#include "elastos/droid/os/ServiceManager.h"
#include "elastos/droid/app/NotificationManager.h"
#include "elastos/droid/view/CGravity.h"
#include "elastos/droid/view/Gravity.h"
#include "elastos/droid/view/CWindowManagerLayoutParams.h"
#include "elastos/droid/view/accessibility/CAccessibilityManager.h"
#include "elastos/droid/view/accessibility/CAccessibilityEvent.h"
#include "elastos/droid/widget/CToastTransientNotification.h"

using Elastos::Core::CStringWrapper;
using Elastos::Droid::R;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Graphics::IPixelFormat;
using Elastos::Droid::App::EIID_ITransientNotification;
using Elastos::Droid::App::NotificationManager;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::Gravity;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::CWindowManagerLayoutParams;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::Accessibility::IAccessibilityManager;
using Elastos::Droid::View::Accessibility::CAccessibilityManager;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::Accessibility::CAccessibilityEvent;

namespace Elastos {
namespace Droid {
namespace Widget {

// 08c79d3a-93bf-4866-bc68-e8feb57f167d
extern "C" const InterfaceID EIID_Toast =
        { 0x08c79d3a, 0x93bf, 0x4866, { 0xbc, 0x68, 0xe8, 0xfe, 0xb5, 0x7f, 0x16, 0x7d } };

AutoPtr<IINotificationManager> Toast::sService;

Toast::Toast()
    : mDuration(0)
{}

/**
 * Construct an empty Toast object.  You must call {@link #setView} before you
 * can call {@link #show}.
 *
 * @param context  The context to use.  Usually your {@link android.app.Application}
 *                 or {@link android.app.Activity} object.
 */
Toast::Toast(
    /* [in] */ IContext* context)
    : mDuration(0)
{
     Init(context);
}

ECode Toast::Init(
    /* [in] */ IContext* context)
{
    mContext = context ;
    CToastTransientNotification::New(THIS_PROBE(IToast), (ITransientNotification**)&mTN);
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);

    Int32 y;
    res->GetDimensionPixelSize(R::dimen::toast_y_offset, &y);
    ((CToastTransientNotification*)mTN.Get())->SetY(y);
    return NOERROR;
}

/**
 * Show the view for the specified duration.
 */
ECode Toast::Show()
{
    if (mNextView == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    ((CToastTransientNotification*)mTN.Get())->SetNextView(mNextView);

    String pkgName;
    mContext->GetPackageName(&pkgName);

    AutoPtr<IINotificationManager> service = GetService();
    return service->EnqueueToast(pkgName, (ITransientNotification*)(mTN.Get()), mDuration);
}

/**
 * Close the view if it's showing, or don't show it if it isn't showing yet.
 * You do not normally have to call this.  Normally view will disappear on its own
 * after the appropriate duration.
 */
ECode Toast::Cancel()
{
    mTN->Hide();

    String pkgName;
    mContext->GetPackageName(&pkgName);
    AutoPtr<IINotificationManager> service = GetService();
    return service->CancelToast(pkgName, (ITransientNotification*)(mTN.Get()));
}

/**
 * Set the view to show.
 * @see #getView
 */
ECode Toast::SetView(
    /* [in] */ IView* view)
{
    mNextView = view;
    return NOERROR;
}

/**
 * Return the view.
 * @see #setView
 */
ECode Toast::GetView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = mNextView;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

/**
 * Set how long to show the view for.
 * @see #LENGTH_SHORT
 * @see #LENGTH_LONG
 */
ECode Toast::SetDuration(
    /* [in] */ Int32 duration)
{
    mDuration = duration;
    return NOERROR;
}

/**
 * Return the duration.
 * @see #setDuration
 */
ECode Toast::GetDuration(
    /* [out] */ Int32* duration)
{
    VALIDATE_NOT_NULL(duration);
    *duration = mDuration;
    return NOERROR;
}

/**
 * Set the margins of the view.
 *
 * @param horizontalMargin The horizontal margin, in percentage of the
 *        container width, between the container's edges and the
 *        notification
 * @param verticalMargin The vertical margin, in percentage of the
 *        container height, between the container's edges and the
 *        notification
 */
ECode Toast::SetMargin(
    /* [in] */ Float horizontalMargin,
    /* [in] */ Float verticalMargin)
{
    ((CToastTransientNotification*)mTN.Get())->SetHorizontalMargin(horizontalMargin);
    ((CToastTransientNotification*)mTN.Get())->SetVerticalMargin(verticalMargin);
    return NOERROR;
}

/**
 * Return the horizontal margin.
 */
ECode Toast::GetHorizontalMargin(
    /* [out] */ Float* horizontalMargin)
{
    VALIDATE_NOT_NULL(horizontalMargin);
    ((CToastTransientNotification*)mTN.Get())->GetHorizontalMargin(horizontalMargin);
    return NOERROR;
}

/**
 * Return the vertical margin.
 */
ECode Toast::GetVerticalMargin(
    /* [out] */ Float* verticalMargin)
{
    VALIDATE_NOT_NULL(verticalMargin);
    ((CToastTransientNotification*)mTN.Get())->GetVerticalMargin(verticalMargin);
    return NOERROR;
}

/**
 * Set the location at which the notification should appear on the screen.
 * @see android.view.Gravity
 * @see #getGravity
 */
ECode Toast::SetGravity(
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 xOffset,
    /* [in] */ Int32 yOffset)
{
    ((CToastTransientNotification*)mTN.Get())->SetGravity(gravity);
    ((CToastTransientNotification*)mTN.Get())->SetX(xOffset);
    ((CToastTransientNotification*)mTN.Get())->SetY(yOffset);
    return NOERROR;
}

/**
 * Get the location at which the notification should appear on the screen.
 * @see android.view.Gravity
 * @see #getGravity
 */
ECode Toast::GetGravity(
    /* [out] */ Int32* gravity)
{
    VALIDATE_NOT_NULL(gravity);
    ((CToastTransientNotification*)mTN.Get())->GetGravity(gravity);
    return NOERROR;
}

/**
 * Return the X offset in pixels to apply to the gravity's location.
 */
ECode Toast::GetXOffset(
    /* [out] */ Int32* xOffset)
{
    VALIDATE_NOT_NULL(xOffset);
    ((CToastTransientNotification*)mTN.Get())->GetX(xOffset);
    return NOERROR;
}

/**
 * Return the Y offset in pixels to apply to the gravity's location.
 */
ECode Toast::GetYOffset(
    /* [out] */ Int32* yOffset)
{
    VALIDATE_NOT_NULL(yOffset);
    ((CToastTransientNotification*)mTN.Get())->GetY(yOffset);
    return NOERROR;
}

ECode Toast::MakeText(
    /* [in] */ IContext* context,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 duration,
    /* [out] */ IToast** toast)
{
    VALIDATE_NOT_NULL(toast);

    CToast::New(context, (IToast**)toast);

    AutoPtr<ILayoutInflater> inflater;
    context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&inflater);

    AutoPtr<IView> v;
    inflater->Inflate(R::layout::transient_notification, NULL, (IView**)&v);
    assert(v != NULL);

    AutoPtr<IView> tv;
    v->FindViewById(R::id::message, (IView**)&tv);
    AutoPtr<ITextView> textView = ITextView::Probe(tv);
    textView->SetText(text);

    (*toast)->SetDuration(duration);

    Toast* t = reinterpret_cast<Toast*>((*toast)->Probe(EIID_Toast));
    t->mNextView = v;

    return NOERROR;
}

ECode Toast::MakeText(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resId,
    /* [in] */ Int32 duration,
    /* [out] */ IToast** toast)
{
    VALIDATE_NOT_NULL(toast);

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<ICharSequence> text;
    res->GetText(resId, (ICharSequence**)&text);
    return MakeText(context, text, duration, toast);
}

/**
 * Update the text in a Toast that was previously created using one of the makeText() methods.
 * @param resId The new text for the Toast.
 */
ECode Toast::SetText(
    /* [in] */ Int32 resId)
{
    AutoPtr<ICharSequence> text;
    mContext->GetText(resId, (ICharSequence**)&text);
    return SetText(text);
}

/**
 * Update the text in a Toast that was previously created using one of the makeText() methods.
 * @param s The new text for the Toast.
 */
ECode Toast::SetText(
    /* [in] */ ICharSequence* s)
{
    if (mNextView == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr<ITextView> tv;
    mNextView->FindViewById(R::id::message, (IView**)&tv);

    if (tv == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return tv->SetText(s);
}

AutoPtr<IINotificationManager> Toast::GetService()
{
    if (sService != NULL) {
        return sService;
    }

    String name("notification");
    AutoPtr<IInterface> obj = ServiceManager::GetService(name);
    sService = IINotificationManager::Probe(obj.Get());
    return sService;
}

ECode Toast::OnHide()
{
    return NOERROR;
}

ECode Toast::IsShowing(
        /* [out] */ Boolean* showing)
{
    VALIDATE_NOT_NULL(showing);
    CToastTransientNotification* ctn = (CToastTransientNotification*)mTN.Get();
    *showing = ctn->mView != NULL;
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos

