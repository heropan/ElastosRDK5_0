#include "widget/CToastTransientNotification.h"
#include "R.h"
#include "os/CHandler.h"
#include "view/CGravity.h"
#include "view/Gravity.h"
#include "view/CWindowManagerLayoutParams.h"
#include "view/accessibility/CAccessibilityManager.h"
#include "view/accessibility/CAccessibilityEvent.h"
#include "widget/Toast.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Droid::R;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Graphics::IPixelFormat;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::Gravity;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::IWindowManagerLayoutParams;
using Elastos::Droid::View::CWindowManagerLayoutParams;
using Elastos::Droid::View::Accessibility::IAccessibilityManager;
using Elastos::Droid::View::Accessibility::CAccessibilityManager;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::Accessibility::CAccessibilityEvent;

namespace Elastos{
namespace Droid{
namespace Widget{


//==============================================================================
//          CToastTransientNotification::ShowAction::ShowAction
//==============================================================================
CToastTransientNotification::ShowAction::ShowAction(
    /* [in] */ CToastTransientNotification* host)
    : mHost(host)
{
}

ECode CToastTransientNotification::ShowAction::Run()
{
    mHost->HandleShow();
    mHost->mNextView = NULL;

    return NOERROR;
}

//==============================================================================
//          CToastTransientNotification::HideAction
//==============================================================================
CToastTransientNotification::HideAction::HideAction(
    /* [in] */ CToastTransientNotification* host)
    : mHost(host)
{}

ECode CToastTransientNotification::HideAction::Run()
{
    mHost->HandleHide();
    mHost->Release();
    return NOERROR;
}

//==============================================================================
//          CToastTransientNotification
//==============================================================================
CToastTransientNotification::CToastTransientNotification()
    : mGravity(IGravity::CENTER_HORIZONTAL | IGravity::BOTTOM)
    , mX(0)
    , mY(0)
    , mHorizontalMargin(0)
    , mVerticalMargin(0)
{}

ECode CToastTransientNotification::constructor(
    /* [in] */ IToast* toast)
{
    AutoPtr<IWeakReferenceSource> source = IWeakReferenceSource::Probe(toast);
    if (source) {
        source->GetWeakReference((IWeakReference**)&mToast);
    }

    CHandler::New((IHandler**)&mHandler);
    /*
    // XXX This should be changed to use a Dialog, with a Theme.Toast
    // defined that sets up the layout params appropriately.
    final WindowManager.LayoutParams params = mParams;
    params.height = WindowManager.LayoutParams.WRAP_CONTENT;
    params.width = WindowManager.LayoutParams.WRAP_CONTENT;
    params.flags = WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE
            | WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE
            | WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON;
    params.format = PixelFormat.TRANSLUCENT;
    params.windowAnimations = com.android.internal.R.style.Animation_Toast;
    params.type = WindowManager.LayoutParams.TYPE_TOAST;
    params.setTitle("Toast");
    */
    mShow = (IRunnable*)new ShowAction(this);
    mHide = (IRunnable*)new HideAction(this);

    CWindowManagerLayoutParams::New(
        IViewGroupLayoutParams::WRAP_CONTENT,
        IViewGroupLayoutParams::WRAP_CONTENT,
        IWindowManagerLayoutParams::TYPE_TOAST,
        IWindowManagerLayoutParams::FLAG_NOT_FOCUSABLE
        | IWindowManagerLayoutParams::FLAG_NOT_TOUCHABLE
        | IWindowManagerLayoutParams::FLAG_KEEP_SCREEN_ON,
        IPixelFormat::TRANSLUCENT,
        (IWindowManagerLayoutParams**)&mParams);
    mParams->SetWindowAnimations(R::style::Animation_Toast);

    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(String("Toast"), (ICharSequence**)&cs);
    mParams->SetTitle(cs);
    return NOERROR;
}

ECode CToastTransientNotification::Show()
{
    Boolean result;
    return mHandler->Post(mShow, &result);
}

ECode CToastTransientNotification::Hide()
{
    AddRef();
    Boolean result;
    return mHandler->Post(mHide, &result);
}

ECode CToastTransientNotification::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = String("ToastTransientNotification");
    return NOERROR;
}

ECode CToastTransientNotification::HandleShow()
{
    if (mView != mNextView) {
        HandleHide();
        mView = mNextView;

        AutoPtr<IContext> context;
        mView->GetContext((IContext**)&context);
        if (mWM == NULL) {
            AutoPtr<IContext> appContext;
            context->GetApplicationContext((IContext**)&appContext);
            if (appContext == NULL)
                appContext = context;

            AutoPtr<IInterface> service;
            appContext->GetSystemService(IContext::WINDOW_SERVICE, (IInterface**)&service);
            mWM = IWindowManager::Probe(service);
        }

        // We can resolve the Gravity here by using the Locale for getting
        // the layout direction
        AutoPtr<IConfiguration> config;
        AutoPtr<IResources> resources;
        context->GetResources((IResources**)&resources);
        resources->GetConfiguration((IConfiguration**)&config);
        Int32 layoutDirection;
        config->GetLayoutDirection(&layoutDirection);

        Int32 gravity = Gravity::GetAbsoluteGravity(mGravity, layoutDirection);
        mParams->SetGravity(gravity);

        if ((gravity & IGravity::HORIZONTAL_GRAVITY_MASK) == IGravity::FILL_HORIZONTAL) {
            ((CWindowManagerLayoutParams*)(mParams.Get()))->mHorizontalWeight = 1.0f;
        }
        if ((gravity & IGravity::VERTICAL_GRAVITY_MASK) == IGravity::FILL_VERTICAL) {
            ((CWindowManagerLayoutParams*)(mParams.Get()))->mVerticalWeight = 1.0f;
        }
        mParams->SetX(mX);
        mParams->SetY(mY);
        mParams->SetVerticalMargin(mVerticalMargin);
        mParams->SetHorizontalMargin(mHorizontalMargin);

        AutoPtr<IViewParent> parent;
        mView->GetParent((IViewParent**)&parent);
        if (parent != NULL) {
            //if (localLOGV) Log.v(
            //        TAG, "REMOVE! " + mView + " in " + this);
            mWM->RemoveView(mView);
        }
        //if (localLOGV) Log.v(TAG, "ADD! " + mView + " in " + this);
        mWM->AddView(mView, mParams);

        TrySendAccessibilityEvent();
    }
    return NOERROR;
}

ECode CToastTransientNotification::TrySendAccessibilityEvent()
{
    Slogger::D("CToastTransientNotification", "TODO: need CAccessibilityManager");
    return E_NOT_IMPLEMENTED;

    AutoPtr<IContext> context;
    mView->GetContext((IContext**)&context);
    AutoPtr<IAccessibilityManager> accessibilityManager;
    CAccessibilityManager::GetInstance(context, (IAccessibilityManager**)&accessibilityManager);
    Boolean enabled;
    accessibilityManager->IsEnabled(&enabled);
    if (!enabled) {
        return NOERROR;
    }
    // treat toasts as notifications since they are used to
    // announce a transient piece of information to the user
    AutoPtr<IAccessibilityEvent> event;
    CAccessibilityEvent::Obtain(
        IAccessibilityEvent::TYPE_NOTIFICATION_STATE_CHANGED, (IAccessibilityEvent**)&event);

    AutoPtr<ICharSequence> clsName;
    CStringWrapper::New(String("Toast"), (ICharSequence**)&clsName);
    event->SetClassName(clsName);

    String strPkgName;
    context->GetPackageName(&strPkgName);
    AutoPtr<ICharSequence> pkgName;
    CStringWrapper::New(strPkgName, (ICharSequence**)&pkgName);
    event->SetPackageName(pkgName);

    Boolean result;
    mView->DispatchPopulateAccessibilityEvent(event, &result);
    accessibilityManager->SendAccessibilityEvent(event);
    return NOERROR;
}

ECode CToastTransientNotification::HandleHide()
{
    if (mView != NULL) {
        // note: checking parent() just to make sure the view has
        // been added...  i have seen cases where we get here when
        // the view isn't yet added, so let's try not to crash.
        AutoPtr<IViewParent> parent;
        mView->GetParent((IViewParent**)&parent);
        if (parent != NULL) {
            /* add by Gary. start {{----------------------------------- */
            /* 2011-10-25 */
            /* add a callback method when the toast hides and another method to check whether the toast is showing */
            if (mToast != NULL) {
                AutoPtr<IToast> obj;
                mToast->Resolve(EIID_IToast, (IInterface**)&obj);
                if (obj != NULL) {
                    Toast* toast = reinterpret_cast<Toast*>(obj->Probe(EIID_Toast));
                    if (toast != NULL) {
                        toast->OnHide();
                    }
                }
            }
            /* add by Gary. end   -----------------------------------}} */

            //if (localLOGV) Log.v(
            //        TAG, "REMOVE! " + mView + " in " + this);
            if (mWM) mWM->RemoveView(mView);
        }

        mView = NULL;
    }

    return NOERROR;
}

ECode CToastTransientNotification::SetNextView(
    /* [in] */ IView* nextView)
{
    mNextView = nextView;
    return NOERROR;
}

ECode CToastTransientNotification::SetGravity(
    /* [in] */ Int32 gravity)
{
    mGravity = gravity;
    return NOERROR;
}

ECode CToastTransientNotification::GetGravity(
    /* [out] */ Int32* gravity)
{
    VALIDATE_NOT_NULL(gravity);
    *gravity = mGravity;
    return NOERROR;
}

ECode CToastTransientNotification::SetX(
    /* [in] */ Int32 x)
{
    mX = x;
    return NOERROR;
}

ECode CToastTransientNotification::GetX(
    /* [out] */ Int32* x)
{
    VALIDATE_NOT_NULL(x);
    *x = mX;
    return NOERROR;
}


ECode CToastTransientNotification::SetY(
    /* [in] */ Int32 y)
{
    mY = y;
    return NOERROR;
}

ECode CToastTransientNotification::GetY(
    /* [out] */ Int32* y)
{
    VALIDATE_NOT_NULL(y);
    *y = mY;
    return NOERROR;
}

ECode CToastTransientNotification::SetHorizontalMargin(
    /* [in] */ Float horizontalMargin)
{
    mHorizontalMargin = horizontalMargin;
    return NOERROR;
}

ECode CToastTransientNotification::GetHorizontalMargin(
    /* [out] */ Float* horizontalMargin)
{
    VALIDATE_NOT_NULL(horizontalMargin);
    *horizontalMargin = mHorizontalMargin;
    return NOERROR;
}

ECode CToastTransientNotification::SetVerticalMargin(
    /* [in] */ Float verticalMargin)
{
    mVerticalMargin = verticalMargin;
    return NOERROR;
}

ECode CToastTransientNotification::GetVerticalMargin(
    /* [out] */ Float* verticalMargin)
{
    VALIDATE_NOT_NULL(verticalMargin);
    *verticalMargin = mVerticalMargin;
    return NOERROR;
}


} // namespace Widget
} // namespace Droid
} // namespace Elastos
