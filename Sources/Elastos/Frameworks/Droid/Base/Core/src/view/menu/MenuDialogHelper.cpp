
#include "view/menu/MenuDialogHelper.h"
#include "view/menu/MenuBuilderBase.h"
#include "view/menu/CListMenuPresenter.h"
#include "app/CAlertDialogBuilder.h"
#include "view/CWindowManagerLayoutParams.h"
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::App::IAlertDialogBuilder;
using Elastos::Droid::App::CAlertDialogBuilder;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnClickListener;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnKeyListener;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnDismissListener;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

MenuDialogHelper::MenuDialogHelper()
{}

MenuDialogHelper::MenuDialogHelper(
    /* [in] */ IMenuBuilder* menu)
    : mMenu(menu)
{}

ECode MenuDialogHelper::Init(
    /* [in] */ IMenuBuilder* menu)
{
    mMenu = menu;
    return NOERROR;
}

ECode MenuDialogHelper::Show(
    /* [in] */ IBinder* windowToken)
{
    // Many references to mMenu, create local reference
    AutoPtr<IMenuBuilder> menu = mMenu;

    // Get the builder for the dialog
    AutoPtr<IContext> context;
    menu->GetContext((IContext**)&context);
    AutoPtr<IAlertDialogBuilder> builder;

    CAlertDialogBuilder::New(context, (IAlertDialogBuilder**)&builder);

    AutoPtr<IContext> alctx;
    builder->GetContext((IContext**)&alctx);

    mPresenter = NULL;
    CListMenuPresenter::New(alctx, R::layout::list_menu_item_layout, (IListMenuPresenter**)&mPresenter);

    mPresenter->SetCallback((IMenuPresenterCallback*)(this->Probe(EIID_IMenuPresenterCallback)));
    mMenu->AddMenuPresenter(mPresenter);

    AutoPtr<IListAdapter> adapter;
    mPresenter->GetAdapter((IListAdapter**)&adapter);
    builder->SetAdapter(adapter, ((IDialogInterfaceOnClickListener*)(this->Probe(EIID_IDialogInterfaceOnClickListener))));

    // Set the title
    AutoPtr<IView> headerView;
    menu->GetHeaderView((IView**)&headerView);
    if (headerView != NULL) {
        // Menu's client has given a custom header view, use it
        builder->SetCustomTitle(headerView);
    }
    else {
        // Otherwise use the (text) title and icon
        AutoPtr<IDrawable> icon;
        menu->GetHeaderIcon((IDrawable**)&icon);
        builder->SetIconEx(icon);

        AutoPtr<ICharSequence> csq;
        menu->GetHeaderTitle((ICharSequence**)&csq);
        builder->SetTitleEx(csq);
    }

    // Set the key listener
    builder->SetOnKeyListener((IDialogInterfaceOnKeyListener*)(this->Probe(EIID_IDialogInterfaceOnKeyListener)));

    // Show the menu
    mDialog = NULL;
    builder->Create((IAlertDialog**)&mDialog);
    mDialog->SetOnDismissListener((IDialogInterfaceOnDismissListener*)(this->Probe(EIID_IDialogInterfaceOnDismissListener)));

    AutoPtr<IWindow> window;
    mDialog->GetWindow((IWindow**)&window);
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);
    lp->SetType(IWindowManagerLayoutParams::TYPE_APPLICATION_ATTACHED_DIALOG);
    if (windowToken != NULL) {
        lp->SetToken(windowToken);
    }
    Int32 flag;
    lp->GetFlags(&flag);
    lp->SetFlags(flag |= IWindowManagerLayoutParams::FLAG_ALT_FOCUSABLE_IM);

    mDialog->Show();
    return NOERROR;
}

ECode MenuDialogHelper::OnKey(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);

    if (keyCode == IKeyEvent::KEYCODE_MENU || keyCode == IKeyEvent::KEYCODE_BACK) {
        Int32 action, repeatCount;
        event->GetAction(&action);
        event->GetRepeatCount(&repeatCount);
        Boolean isCanceled;
        if (action == IKeyEvent::ACTION_DOWN
                && repeatCount == 0) {
            AutoPtr<IWindow> win;
            mDialog->GetWindow((IWindow**)&win);
            if (win != NULL) {
                AutoPtr<IView> decor;
                win->GetDecorView((IView**)&decor);
                if (decor != NULL) {
                    AutoPtr<IDispatcherState> dispatcher;
                    decor->GetKeyDispatcherState((IDispatcherState**)&dispatcher);
                    if (dispatcher != NULL) {
                        dispatcher->StartTracking(event, (IDialogInterfaceOnClickListener*)this);
                        *flag = TRUE;
                        return NOERROR;
                    }
                }
            }
        }
        else if (action == IKeyEvent::ACTION_UP && (event->IsCanceled(&isCanceled), !isCanceled)) {
            AutoPtr<IWindow> win;
            mDialog->GetWindow((IWindow**)&win);
            if (win != NULL) {
                AutoPtr<IView> decor;
                win->GetDecorView((IView**)&decor);
                if (decor != NULL) {
                    AutoPtr<IDispatcherState> dispatcher;
                    decor->GetKeyDispatcherState((IDispatcherState**)&dispatcher);
                    Boolean isTracking;
                    if (dispatcher != NULL && (dispatcher->IsTracking(event, &isTracking), isTracking)) {
                        mMenu->CloseEx(TRUE);
                        dialog->Dismiss();
                        *flag = TRUE;
                        return NOERROR;
                    }
                }
            }
        }
    }

    // Menu shortcut matching
    return mMenu->PerformShortcut(keyCode, event, 0, flag);
}

ECode MenuDialogHelper::SetPresenterCallback(
    /* [in] */ IMenuPresenterCallback* cb)
{
    mPresenterCallback = cb;
    return NOERROR;
}

ECode MenuDialogHelper::Dismiss()
{
    if (mDialog != NULL) {
        mDialog->Dismiss();
    }
    return NOERROR;
}

ECode MenuDialogHelper::OnDismiss(
    /* [in] */ IDialogInterface* dialog)
{
    return mPresenter->OnCloseMenu(mMenu, TRUE);
}

ECode MenuDialogHelper::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    if (allMenusAreClosing || menu == mMenu) {
        Dismiss();
    }
    if (mPresenterCallback != NULL) {
        return mPresenterCallback->OnCloseMenu(menu, allMenusAreClosing);
    }

    return NOERROR;
}

ECode MenuDialogHelper::OnOpenSubMenu(
    /* [in] */ IMenuBuilder* subMenu,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    if (mPresenterCallback != NULL) {
        return mPresenterCallback->OnOpenSubMenu(subMenu, result);
    }

    return NOERROR;
}

ECode MenuDialogHelper::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    AutoPtr<IMenuItem> item;
    AutoPtr<IListAdapter> adapter;
    mPresenter->GetAdapter((IListAdapter**)&adapter);
    assert(adapter != NULL);
    adapter->GetItem(which, (IInterface**)&item);

    Boolean state;
    return mMenu->PerformItemAction(item , 0, &state);
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
