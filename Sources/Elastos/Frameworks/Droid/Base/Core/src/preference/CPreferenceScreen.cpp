
#include "CPreferenceScreen.h"
#include "R.h"
#include "PreferenceGroupAdapter.h"
#include "CPreferenceManager.h"
#include "app/CDialog.h"
#include "text/TextUtils.h"
#include "widget/CListView.h"
#include "CPreferenceScreenSavedState.h"

using Elastos::Core::CStringWrapper;
using Elastos::Droid::App::CDialog;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnDismissListener;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Widget::CListView;
using Elastos::Droid::Widget::IAdapterViewOnItemClickListener;
using Elastos::Droid::Widget::EIID_IAdapterViewOnItemClickListener;

namespace Elastos {
namespace Droid {
namespace Preference {

PInterface CPreferenceScreen::Probe(
    /* [in] */REIID riid)
{
    if (riid == EIID_IPreferenceScreen) {
        return (IPreferenceScreen*)this;
    }
    else if (riid == EIID_IPreferenceGroup) {
        return (IPreferenceGroup*)this;
    }
    else if (riid == EIID_IAdapterViewOnItemClickListener) {
        return (IAdapterViewOnItemClickListener*)this;
    }
    else if (riid == EIID_IDialogInterfaceOnDismissListener) {
        return (IDialogInterfaceOnDismissListener*)this;
    }

    return PreferenceGroup::Probe(riid);
}

UInt32 CPreferenceScreen::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CPreferenceScreen::Release()
{
    return ElRefBase::Release();
}

ECode CPreferenceScreen::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface *)(IPreferenceScreen*)this) {
        *pIID = EIID_IPreferenceScreen;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IPreferenceGroup*)this) {
        *pIID = EIID_IPreferenceGroup;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IAdapterViewOnItemClickListener*)this) {
        *pIID = EIID_IAdapterViewOnItemClickListener;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IDialogInterfaceOnDismissListener*)this) {
        *pIID = EIID_IDialogInterfaceOnDismissListener;
        return NOERROR;
    }
    return PreferenceGroup::GetInterfaceID(pObject, pIID);
}

ECode CPreferenceScreen::GetRootAdapter(
    /* [out] */ IListAdapter** adapter)
{
    VALIDATE_NOT_NULL(adapter)
    if (mRootAdapter == NULL) {
        OnCreateRootAdapter((IListAdapter**)&mRootAdapter);
    }

    *adapter = mRootAdapter;
    REFCOUNT_ADD(*adapter)
    return NOERROR;
}

ECode CPreferenceScreen::OnCreateRootAdapter(
    /* [out] */ IListAdapter** adapter)
{
    VALIDATE_NOT_NULL(adapter)
    AutoPtr<PreferenceGroupAdapter> preferenceAdapter = new PreferenceGroupAdapter(THIS_PROBE(IPreferenceGroup));
    *adapter = (IListAdapter*)preferenceAdapter;
    REFCOUNT_ADD(*adapter)
    return NOERROR;
}

ECode CPreferenceScreen::Bind(
    /* [in] */ IListView* listView)
{
    listView->SetOnItemClickListener(THIS_PROBE(IAdapterViewOnItemClickListener));
    AutoPtr<IListAdapter> adapter;
    GetRootAdapter((IListAdapter**)&adapter);
    listView->SetAdapter(adapter);

    OnAttachedToActivity();
    return NOERROR;
}

ECode CPreferenceScreen::OnClick()
{
    AutoPtr<IIntent> intent;
    String fragment;
    Int32 count;
    if ((GetIntent((IIntent**)&intent), intent != NULL) || (GetFragment(&fragment), !fragment.IsNull())
            || (GetPreferenceCount(&count), count == 0)) {
        return NOERROR;
    }

    return ShowDialog(NULL);
}

ECode CPreferenceScreen::ShowDialog(
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
    mListView = NULL;
    childPrefScreen->FindViewById(R::id::list, (IView**)&mListView);
    Bind(mListView);

    // Set the title bar if title is available, else no title bar
    AutoPtr<ICharSequence> title;
    GetTitle((ICharSequence**)&title);
    Int32 resID;
    context->GetThemeResId((Int32*)&resID);
    AutoPtr<IDialog> dialog;
    CDialog::New(context, resID, (IDialog **)&dialog);
    mDialog = dialog;
    if (TextUtils::IsEmpty(title)) {
        AutoPtr<IWindow> window;
        dialog->GetWindow((IWindow**)&window);
        Boolean result;
        window->RequestFeature(IWindow::FEATURE_NO_TITLE, (Boolean*)&result);
    }
    else {
        dialog->SetTitle(title);
    }
    dialog->SetContentView(childPrefScreen);
    dialog->SetOnDismissListener(THIS_PROBE(IDialogInterfaceOnDismissListener));
    if (state != NULL) {
        dialog->OnRestoreInstanceState(state);
    }

    // Add the screen to the list of preferences screens opened as dialogs
    AutoPtr<IPreferenceManager> manager;
    GetPreferenceManager((IPreferenceManager**)&manager);
    manager->AddPreferencesScreen(dialog);

    return dialog->Show();
}

ECode CPreferenceScreen::OnDismiss(
    /* [in] */ IDialogInterface* dialog)
{
    mDialog = NULL;
    AutoPtr<IPreferenceManager> manager;
    GetPreferenceManager((IPreferenceManager**)&manager);
    manager->RemovePreferencesScreen(dialog);
    return NOERROR;
}

ECode CPreferenceScreen::GetDialog(
    /* [out] */ IDialog** dialog)
{
    VALIDATE_NOT_NULL(dialog)
    *dialog = mDialog;
    REFCOUNT_ADD(*dialog)
    return NOERROR;
}

ECode CPreferenceScreen::OnItemClick(
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

ECode CPreferenceScreen::IsOnSameScreenAsChildren(
    /* [out] */ Boolean* isOnSameScreenAsChildren)
{
    VALIDATE_NOT_NULL(isOnSameScreenAsChildren)
    *isOnSameScreenAsChildren = FALSE;
    return NOERROR;
}

ECode CPreferenceScreen::OnSaveInstanceState(
    /* [out] */ IParcelable** state)
{
    VALIDATE_NOT_NULL(state)
    AutoPtr<IParcelable> superState;
    PreferenceGroup::OnSaveInstanceState((IParcelable**)&superState);
    AutoPtr<IDialog> dialog = mDialog;
    Boolean isShow;
    if (dialog == NULL || (dialog->IsShowing(&isShow), !isShow)) {
        *state = superState;
        REFCOUNT_ADD(*state)
        return NOERROR;
    }

    AutoPtr<IPreferenceScreenSavedState> myState;
    CPreferenceScreenSavedState::New((IPreferenceScreenSavedState**)&myState);
    myState->SetDialogShowing(TRUE);
    AutoPtr<IBundle> b;
    dialog->OnSaveInstanceState((IBundle**)&b);
    myState->SetDialogBundle(b);

    *state = IParcelable::Probe(myState);
    REFCOUNT_ADD(*state)
    return NOERROR;
}

ECode CPreferenceScreen::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    if (state == NULL || IPreferenceScreenSavedState::Probe(state) == NULL) {
        // Didn't save state for us in onSaveInstanceState
        PreferenceGroup::OnRestoreInstanceState(state);
        return NOERROR;
    }

    AutoPtr<IPreferenceScreenSavedState> myState = IPreferenceScreenSavedState::Probe(state);
    AutoPtr<IParcelable> superState;
    myState->GetSuperState((IParcelable**)&superState);
    PreferenceGroup::OnRestoreInstanceState(superState);
    Boolean isDialogShowing;
    if (myState->IsDialogShowing(&isDialogShowing), isDialogShowing) {
        AutoPtr<IBundle> b;
        myState->GetDialogBundle((IBundle**)&b);
        ShowDialog(b);
    }
    return NOERROR;
}

ECode CPreferenceScreen::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    PreferenceGroup::Init(context, attrs, R::attr::preferenceScreenStyle);
    return NOERROR;
}

ECode CPreferenceScreen::SetOrderingAsAdded(
    /* [in] */ Boolean orderingAsAdded)
{
    return PreferenceGroup::SetOrderingAsAdded(orderingAsAdded);
}

ECode CPreferenceScreen::IsOrderingAsAdded(
    /* [out] */ Boolean* added)
{
    return PreferenceGroup::IsOrderingAsAdded(added);
}

ECode CPreferenceScreen::GetPreferenceCount(
    /* [out] */ Int32* count)
{
    return PreferenceGroup::GetPreferenceCount(count);
}

ECode CPreferenceScreen::GetPreference(
    /* [in] */ Int32 index,
    /* [out] */ IPreference** preference)
{
    return PreferenceGroup::GetPreference(index, preference);
}

ECode CPreferenceScreen::AddPreference(
    /* [in] */ IPreference* preference,
    /* [out] */ Boolean* result)
{
    return PreferenceGroup::AddPreference(preference, result);
}

ECode CPreferenceScreen::RemovePreference(
    /* [in] */ IPreference* preference,
    /* [out] */ Boolean* result)
{
    return PreferenceGroup::RemovePreference(preference, result);
}

ECode CPreferenceScreen::RemoveAll()
{
    return PreferenceGroup::RemoveAll();
}

ECode CPreferenceScreen::FindPreference(
    /* [in] */ ICharSequence* key,
    /* [out] */ IPreference** preference)
{
    return PreferenceGroup::FindPreference(key, preference);
}

ECode CPreferenceScreen::SortPreferences()
{
    return PreferenceGroup::SortPreferences();
}

ECode CPreferenceScreen::OnPrepareAddPreference(
    /* [in] */ IPreference* preference,
    /* [out] */ Boolean* result)
{
    return PreferenceGroup::OnPrepareAddPreference(preference, result);
}

}  // Preference
}  // Droid
}  // Elastos
