
#include "PreferenceScreen.h"
#include "R.h"
#include "CPreferenceGroupAdapter.h"
#include "app/CDialog.h"
#include "text/TextUtils.h"
#include "widget/CListView.h"
#include "CPreferenceScreenSavedState.h"

using Elastos::Core::CStringWrapper;
using Elastos::Droid::App::CDialog;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Widget::CListView;
using Elastos::Droid::Widget::IAdapterViewOnItemClickListener;
using Elastos::Droid::Widget::EIID_IAdapterViewOnItemClickListener;

namespace Elastos {
namespace Droid {
namespace Preference {

ECode PreferenceScreen::OnClick()
{
    String fragment;
    GetFragment(&fragment);

    AutoPtr<IIntent> intent;
    GetIntent((IIntent**)&intent);

    Int32 count;
    GetPreferenceCount(&count);

    if (intent != NULL || fragment != NULL || count == 0) {
        return NOERROR;
    }

    return ShowDialog(NULL);
}

ECode PreferenceScreen::OnSaveInstanceState(
    /* [out] */ IParcelable** state)
{
    VALIDATE_NOT_NULL(state);
    AutoPtr<IParcelable> superState;
    PreferenceGroup::OnSaveInstanceState((IParcelable**)&superState);
    AutoPtr<IDialog> dialog;
    dialog = mDialog;

    Boolean isShow;
    dialog->IsShowing((Boolean*)&isShow);
    if (dialog == NULL || !isShow) {
        *state = superState;
        REFCOUNT_ADD(*state);
        return NOERROR;
    }

    AutoPtr<CPreferenceScreenSavedState> myState;
    CPreferenceScreenSavedState::NewByFriend((CPreferenceScreenSavedState**)&myState);

    myState->mIsDialogShowing = TRUE;
    dialog->OnSaveInstanceState((IBundle**)&myState->mDialogBundle);

    *state = (IParcelable*)myState->Probe(EIID_IParcelable);
    REFCOUNT_ADD(*state);
    return NOERROR;
}

ECode PreferenceScreen::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    if (state == NULL || IPreferenceScreenSavedState::Probe(state) == NULL) {
        // Didn't save state for us in onSaveInstanceState
        PreferenceGroup::OnRestoreInstanceState(state);
        return NOERROR;
    }

    AutoPtr<IPreferenceScreenSavedState> myState = IPreferenceScreenSavedState::Probe(state);
    assert(myState != NULL);
    AutoPtr<IParcelable> superState;
    myState->GetSuperState((IParcelable**)&superState);
    PreferenceGroup::OnRestoreInstanceState(superState);
    if (((CPreferenceScreenSavedState*)myState.Get())->mIsDialogShowing) {
        ShowDialog(((CPreferenceScreenSavedState*)myState.Get())->mDialogBundle);
    }
    return NOERROR;
}

ECode PreferenceScreen::IsOnSameScreenAsChildren(
    /* [out] */ Boolean* isOnSameScreenAsChildren)
{
    VALIDATE_NOT_NULL(isOnSameScreenAsChildren);
    *isOnSameScreenAsChildren = FALSE;
    return NOERROR;
}

ECode PreferenceScreen::GetRootAdapter(
    /* [out] */ IListAdapter** adapter)
{
    VALIDATE_NOT_NULL(adapter);
    if (mRootAdapter == NULL) {
        OnCreateRootAdapter((IListAdapter**)&mRootAdapter);
    }

    *adapter = mRootAdapter;
    REFCOUNT_ADD(*adapter);
    return NOERROR;
}

ECode PreferenceScreen::OnCreateRootAdapter(
    /* [out] */ IListAdapter** adapter)
{
    AutoPtr<IPreferenceGroupAdapter> preferenceAdapter;
    CPreferenceGroupAdapter::New(THIS_PROBE(IPreferenceGroup), (IPreferenceGroupAdapter **)&preferenceAdapter);
    *adapter = preferenceAdapter;
    REFCOUNT_ADD(*adapter);
    return NOERROR;
}

ECode PreferenceScreen::Bind(
    /* [in] */ IListView* listView)
{
    listView->SetOnItemClickListener(THIS_PROBE(IAdapterViewOnItemClickListener));
    AutoPtr<IListAdapter> adapter;
    GetRootAdapter((IListAdapter**)&adapter);
    listView->SetAdapter(adapter);

    OnAttachedToActivity();
    return NOERROR;
}

ECode PreferenceScreen::OnDismiss(
    /* [in] */ IDialogInterface* dialog)
{
    mDialog = NULL;
    AutoPtr<IPreferenceManager> manager;

    GetPreferenceManager((IPreferenceManager**)&manager);
    manager->RemovePreferencesScreen(dialog);
    return NOERROR;
}

ECode PreferenceScreen::GetDialog(
    /* [out] */ IDialog** dialog)
{
    VALIDATE_NOT_NULL(dialog);
    *dialog = mDialog;
    REFCOUNT_ADD(*dialog);
    return NOERROR;
}

ECode PreferenceScreen::ShowDialog(
    /* [in] */IBundle* state)
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);

    if (mListView != NULL) {
        mListView->SetAdapter(NULL);
    }

    AutoPtr<ILayoutInflater> inflater;
    context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&inflater);

    AutoPtr<IView> childPrefScreen;
    inflater->Inflate(R::layout::preference_list_fragment, NULL, (IView**)&childPrefScreen);
    childPrefScreen->FindViewById(R::id::list, (IView**)&mListView);
    Bind(mListView);

    // Set the title bar if title is available, else no title bar
    AutoPtr<ICharSequence> title;
    GetTitle((ICharSequence**)&title);

    Int32 resID;
    context->GetThemeResId((Int32*)&resID);

    CDialog::New(context, resID, (IDialog **)&mDialog);

    AutoPtr<IDialog> dialog;
    dialog = mDialog;

    if (TextUtils::IsEmpty(title)) {
        AutoPtr<IWindow> window;
        dialog->GetWindow((IWindow**)&window);

        Boolean result;
        window->RequestFeature(IWindow::FEATURE_NO_TITLE, (Boolean *)&result);
    }
    else {
        dialog->SetTitle(title);
    }
    dialog->SetContentView(childPrefScreen);
    dialog->SetOnDismissListener((IDialogInterfaceOnDismissListener*)this);
    if (state != NULL) {
        dialog->OnRestoreInstanceState(state);
    }

    // Add the screen to the list of preferences screens opened as dialogs
    AutoPtr<IPreferenceManager> manager;
    GetPreferenceManager((IPreferenceManager**)&manager);
    manager->AddPreferencesScreen(dialog);

    return dialog->Show();
}

ECode PreferenceScreen::OnItemClick(
        /* [in] */IAdapterView* parent,
        /* [in] */IView* view,
        /* [in] */Int32 position,
        /* [in] */Int64 id)
{
    // If the list has headers, subtract them from the index.
    if (IListView::Probe(parent) != NULL) {
        position -= ((CListView*)IListView::Probe(parent))->GetHeaderViewsCount();
    }

    AutoPtr<IListAdapter> adapter;
    GetRootAdapter((IListAdapter**)&adapter);
    AutoPtr<IInterface> item;
    adapter->GetItem(position, (IInterface**)&item);
    if (IPreference::Probe(item) == NULL) {
        return NOERROR;
    }
    return reinterpret_cast<Preference*>(item->Probe(EIID_Preference))->PerformClick(THIS_PROBE(IPreferenceScreen));
}

}  // Preference
}  // Droid
}  // Elastos
