
#include "elastos/droid/server/am/LockTaskNotify.h"
#include "elastos/droid/R.h"
#include <elastos/core/CoreUtils.h>

using Elastos::Droid::Widget::CToastHelper;
using Elastos::Droid::Widget::IToastHelper;
using Elastos::Core::CoreUtils;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

const Int32 LockTaskNotify::H::SHOW_TOAST;

ECode LockTaskNotify::H::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    switch(what) {
        case SHOW_TOAST:
        {
            Int32 arg1;
            msg->GetArg1(&arg1);
            mHost->HandleShowToast(arg1 != 0);
            break;
        }
    }
    return NOERROR;
}

const String LockTaskNotify::TAG("LockTaskNotify");

LockTaskNotify::LockTaskNotify(
    /* [in] */ IContext* context)
{
    mContext = context;
    AutoPtr<IInterface> service;
    mContext->GetSystemService(IContext::ACCESSIBILITY_SERVICE, (IInterface**)&service);
    mAccessibilityManager = IAccessibilityManager::Probe(service);
    mHandler = new H(this);
}

void LockTaskNotify::ShowToast(
    /* [in] */ Boolean isLocked)
{
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(H::SHOW_TOAST, isLocked ? 1 : 0, 0 /* Not used */, (IMessage**)&msg);
    msg->SendToTarget();
}

void LockTaskNotify::HandleShowToast(
    /* [in] */ Boolean isLocked)
{
    String text;
    mContext->GetString(isLocked ? R::string::lock_to_app_toast_locked
        : R::string::lock_to_app_toast, &text);
    Boolean isEnabled;
    if (!isLocked && (mAccessibilityManager->IsEnabled(&isEnabled), isEnabled)) {
        mContext->GetString(R::string::lock_to_app_toast_accessible, &text);
    }
    if (mLastToast != NULL) {
        mLastToast->Cancel();
    }
    AutoPtr<IToastHelper> tHelper;
    CToastHelper::AcquireSingleton((IToastHelper**)&tHelper);
    mLastToast = NULL;
    tHelper->MakeText(mContext, CoreUtils::Convert(text), IToast::LENGTH_LONG, (IToast**)&mLastToast);
    mLastToast->Show();
}

void LockTaskNotify::Show(
    /* [in] */ Boolean starting)
{
    Int32 showString = R::string::lock_to_app_exit;
    if (starting) {
        showString = R::string::lock_to_app_start;
    }
    AutoPtr<IToastHelper> tHelper;
    CToastHelper::AcquireSingleton((IToastHelper**)&tHelper);
    String text;
    mContext->GetString(showString, &text);
    AutoPtr<IToast> toast;
    tHelper->MakeText(mContext, CoreUtils::Convert(text), IToast::LENGTH_LONG, (IToast**)&toast);
    toast->Show();
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
