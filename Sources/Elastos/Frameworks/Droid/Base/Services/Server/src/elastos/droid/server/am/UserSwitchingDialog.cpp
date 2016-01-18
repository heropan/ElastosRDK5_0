#include "Elastos.Droid.Widget.h"
#include "elastos/droid/server/am/UserSwitchingDialog.h"
#include "elastos/droid/R.h"

using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::ILayoutInflaterHelper;
//TODO using Elastos::Droid::View::CLayoutInflaterHelper;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IWindow;
using Elastos::Droid::View::IWindowManagerLayoutParams;
using Elastos::Droid::Widget::ITextView;

using Elastos::Core::CString;
using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

//=====================================================================
//                  UserSwitchingDialog::InnerHandler
//=====================================================================
UserSwitchingDialog::InnerHandler::InnerHandler(
    /* [in] */ UserSwitchingDialog* owner)
    : mOwner(owner)
{
    mOwner = owner;
}

ECode UserSwitchingDialog::InnerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case MSG_START_USER:
            mOwner->mService->StartUserInForeground(mOwner->mUserId, mOwner);
            break;
    }
    return NOERROR;
}

//=====================================================================
//                         UserSwitchingDialog
//=====================================================================
const String UserSwitchingDialog::TAG("ActivityManagerUserSwitchingDialog");
const Int32 UserSwitchingDialog::MSG_START_USER;

UserSwitchingDialog::UserSwitchingDialog(
    /* [in] */ CActivityManagerService* service,
    /* [in] */ IContext* _context,
    /* [in] */ Int32 userId,
    /* [in] */ String userName,
    /* [in] */ Boolean aboveSystem)
{
    AlertDialog::constructor(_context);
    mService = service;
    mUserId = userId;

    // Set up the dialog contents
    SetCancelable(FALSE);
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    // Custom view due to alignment and font size requirements
    AutoPtr<IView> view;
    AutoPtr<ILayoutInflaterHelper> lifHelper;
    //TODO CLayoutInflaterHelper::AcquireSingleton((ILayoutInflaterHelper**)&lifHelper);
    AutoPtr<ILayoutInflater> inflater;
    lifHelper->From(context, (ILayoutInflater**)&inflater);
    inflater->Inflate(R::layout::user_switching_dialog, NULL, (IView**)&view);

    AutoPtr<IView> tv;
    view->FindViewById(R::id::message, (IView**)&tv);
    ITextView* textView = ITextView::Probe(tv);
    String msg;
    AutoPtr<ICharSequence> userNameCS;
    CString::New(userName, (ICharSequence**)&userNameCS);
    //in java getString with param Object... formatArgs
    AutoPtr<ArrayOf<IInterface*> > args = ArrayOf<IInterface*>::Alloc(1);
    args->Set(0, TO_IINTERFACE(userNameCS));
    res->GetString(R::string::user_switching_message, args, &msg);
    AutoPtr<ICharSequence> msgCS;
    CString::New(msg, (ICharSequence**)&msgCS);
    textView->SetText(msgCS);
    SetView(view);

    AutoPtr<IWindow> window;
    GetWindow((IWindow**)&window);
    if (aboveSystem) {
        window->SetType(IWindowManagerLayoutParams::TYPE_SYSTEM_ERROR);
    }
    AutoPtr<IWindowManagerLayoutParams> attrs;
    window->GetAttributes((IWindowManagerLayoutParams**)&attrs);
    attrs->SetPrivateFlags(IWindowManagerLayoutParams::PRIVATE_FLAG_SYSTEM_ERROR |
            IWindowManagerLayoutParams::PRIVATE_FLAG_SHOW_FOR_ALL_USERS);
    window->SetAttributes(attrs);
}

ECode UserSwitchingDialog::Show()
{
    AlertDialog::Show();
    // TODO: Instead of just an arbitrary delay, wait for a signal that the window was fully
    // displayed by the window manager
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(MSG_START_USER, (IMessage**)&msg);
    Boolean res;
    mHandler->SendMessageDelayed(msg, 250, &res);
    return NOERROR;
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
