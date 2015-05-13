
#include "CListPreference.h"
#include "CListPreferenceSavedState.h"
#include "R.h"
#include "elastos/Logger.h"

using Elastos::Core::CStringWrapper;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnClickListener;
using Elastos::Droid::Content::Res::IResources;

namespace Elastos {
namespace Droid {
namespace Preference {

/////////////////////////////////////////////////////
// CListPreference::DialogInterfaceOnClickListener
/////////////////////////////////////////////////////

CListPreference::DialogInterfaceOnClickListener::DialogInterfaceOnClickListener(
    /* [in] */ CListPreference* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(CListPreference::DialogInterfaceOnClickListener, IDialogInterfaceOnClickListener)

ECode CListPreference::DialogInterfaceOnClickListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    mHost->mClickedDialogEntryIndex = which;

    /*
     * Clicking on an item simulates the positive button
     * click, and dismisses the dialog.
     */
    mHost->OnClick(dialog, IDialogInterface::BUTTON_POSITIVE);
    dialog->Dismiss();

    return NOERROR;
}


/////////////////////////////////////////////////////
// CListPreference
/////////////////////////////////////////////////////

CListPreference::CListPreference()
    : mClickedDialogEntryIndex(0)
{}

PInterface CListPreference::Probe(
    /* [in] */REIID riid)
{
    if (riid == EIID_IListPreference) {
        return (IListPreference*)this;
    }
    else if (riid == EIID_IDialogPreference) {
        return (IDialogPreference*)this;
    }

    return DialogPreference::Probe(riid);
}

UInt32 CListPreference::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CListPreference::Release()
{
    return ElRefBase::Release();
}

ECode CListPreference::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface *)(IListPreference*)this) {
        *pIID = EIID_IListPreference;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IDialogPreference*)this) {
        *pIID = EIID_IDialogPreference;
        return NOERROR;
    }
    return DialogPreference::GetInterfaceID(pObject, pIID);
}

ECode CListPreference::SetEntries(
    /* [in] */ ArrayOf<ICharSequence*>* entries)
{
    mEntries = entries;
    return NOERROR;
}

ECode CListPreference::SetEntriesEx(
    /* [in] */ Int32 entriesResId)
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    AutoPtr< ArrayOf<ICharSequence*> > entries;
    resources->GetTextArray(entriesResId, (ArrayOf<ICharSequence*>**)&entries);
    return SetEntries(entries);
}

ECode CListPreference::GetEntries(
    /* [out, callee] */ ArrayOf<ICharSequence*>** entries)
{
    VALIDATE_NOT_NULL(entries)
    *entries = mEntries;
    ARRAYOF_ADDREF(*entries)
    return NOERROR;
}

ECode CListPreference::SetEntryValues(
    /* [in] */ ArrayOf<ICharSequence*>* entryValues)
{
    mEntryValues = entryValues;
    return NOERROR;
}

ECode CListPreference::SetEntryValuesEx(
    /* [in] */ Int32 entryValuesResId)
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    AutoPtr< ArrayOf<ICharSequence*> > entries;
    resources->GetTextArray(entryValuesResId, (ArrayOf<ICharSequence*>**)&entries);
    return SetEntryValues(entries);
}

ECode CListPreference::GetEntryValues(
    /* [out, callee] */ ArrayOf<ICharSequence*>** entryValues)
{
    VALIDATE_NOT_NULL(entryValues)
    *entryValues = mEntryValues;
    ARRAYOF_ADDREF(*entryValues)
    return NOERROR;
}

ECode CListPreference::SetValue(
    /* [in] */ const String& value)
{
    mValue = value;
    Boolean isPersistString;
    return PersistString(value, &isPersistString);
}

ECode CListPreference::GetSummary(
    /* [out] */ ICharSequence** summary)
{
    VALIDATE_NOT_NULL(summary)

    AutoPtr<ICharSequence> entry;
    GetEntry((ICharSequence**)&entry);
    if (mSummary.IsNull() || entry == NULL) {
        return DialogPreference::GetSummary(summary);
    }

    String str;
    str.AppendFormat(mSummary.string(), entry.Get());
    return CStringWrapper::New(str, summary);
}

ECode CListPreference::SetSummary(
    /* [in] */ ICharSequence* summary)
{
    FAIL_RETURN(DialogPreference::SetSummary(summary))
    if (summary == NULL && !mSummary.IsNull()) {
        mSummary = String(NULL);
    }
    else if (summary != NULL) {
        String str;
        summary->ToString(&str);
        if (!str.Equals(mSummary)) {
            mSummary = str;
        }
    }

    return NOERROR;
}

ECode CListPreference::SetValueIndex(
    /* [in] */ Int32 index)
{
    if (mEntryValues != NULL) {
        AutoPtr<ICharSequence> cstr = (*mEntryValues)[index];
        String str;
        cstr->ToString(&str);
        SetValue(str);
    }

    return NOERROR;
}

ECode CListPreference::GetValue(
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)
    *value = mValue;
    return NOERROR;
}

ECode CListPreference::GetEntry(
    /* [out] */ ICharSequence** entry)
{
    Int32 index = GetValueIndex();
    if (index >= 0 && mEntries != NULL) {
        *entry = (*mEntries)[index];
        INTERFACE_ADDREF(*entry)
    }
    else {
        *entry = NULL;
    }

    return NOERROR;
}

ECode CListPreference::FindIndexOfValue(
    /* [in] */ const String& value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)

    if (!value.IsNull() && mEntryValues != NULL) {
        for (Int32 i = mEntryValues->GetLength() - 1; i >= 0; i--) {
            AutoPtr<ICharSequence> cstr = (*mEntryValues)[i];
            String str;
            cstr->ToString(&str);
            if (str.Equals(value)) {
                *index = i;
                return NOERROR;
            }
        }
    }
    *index = -1;
    return NOERROR;
}

Int32 CListPreference::GetValueIndex()
{
    Int32 index;
    FindIndexOfValue(mValue, &index);
    return index;
}

ECode CListPreference::OnPrepareDialogBuilder(
    /* [in] */ IAlertDialogBuilder* builder)
{
    FAIL_RETURN(DialogPreference::OnPrepareDialogBuilder(builder))

    if (mEntries == NULL || mEntryValues == NULL) {
        Logger::E("CListPreference", "ListPreference requires an entries array and an entryValues array.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    mClickedDialogEntryIndex = GetValueIndex();
    AutoPtr<IDialogInterfaceOnClickListener> listener = new DialogInterfaceOnClickListener(this);
    builder->SetSingleChoiceItemsEx2(mEntries, mClickedDialogEntryIndex, listener);

    /*
     * The typical interaction for list-based dialogs is to have
     * click-on-an-item dismiss the dialog instead of the user having to
     * press 'Ok'.
     */
    builder->SetPositiveButtonEx(NULL, NULL);

    return NOERROR;
}

ECode CListPreference::OnDialogClosed(
    /* [in] */ Boolean positiveResult)
{
    DialogPreference::OnDialogClosed(positiveResult);

    if (positiveResult && mClickedDialogEntryIndex >= 0 && mEntryValues != NULL) {
        AutoPtr<ICharSequence> cstr = (*mEntryValues)[mClickedDialogEntryIndex];
        Boolean isSuccess;
        if (CallChangeListener(cstr, &isSuccess), isSuccess) {
            String value;
            cstr->ToString(&value);
            SetValue(value);
        }
    }
    return NOERROR;
}

ECode CListPreference::OnGetDefaultValue(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 index,
    /* [out] */ IInterface** value)
{
    String str;
    a->GetString(index, &str);
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(str, (ICharSequence**)&cs);
    *value = (IInterface*)cs;
    INTERFACE_ADDREF(*value)
    return NOERROR;
}

ECode CListPreference::OnSetInitialValue(
    /* [in] */ Boolean restoreValue,
    /* [in] */ IInterface* defaultValue)
{
    String value;
    if (restoreValue) {
        GetPersistedString(mValue, &value);
    }
    else {
        ICharSequence::Probe(defaultValue)->ToString(&value);
    }

    return SetValue(value);
}

ECode CListPreference::OnSaveInstanceState(
    /* [out] */ IParcelable** parcel)
{
    VALIDATE_NOT_NULL(parcel)

    AutoPtr<IParcelable> superState;
    DialogPreference::OnSaveInstanceState((IParcelable**)&superState);
    Boolean isPersistent;
    IsPersistent(&isPersistent);
    if (isPersistent) {
        // No need to save instance state since it's persistent
        *parcel = superState;
        INTERFACE_ADDREF(*parcel)
        return NOERROR;
    }

    AutoPtr<IListPreferenceSavedState> myState;
    CListPreferenceSavedState::New(superState, (IListPreferenceSavedState**)&myState);
    String value;
    GetValue(&value);
    myState->SetValue(value);
    *parcel = IParcelable::Probe(myState);
    INTERFACE_ADDREF(*parcel)

    return NOERROR;
}

ECode CListPreference::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    if (state == NULL || IListPreferenceSavedState::Probe(state) == NULL) {
        // Didn't save state for us in onSaveInstanceState
        return DialogPreference::OnRestoreInstanceState(state);
    }

    AutoPtr<IListPreferenceSavedState> myState = IListPreferenceSavedState::Probe(state);
    AutoPtr<IParcelable> superState;
    myState->GetSuperState((IParcelable**)&superState);
    DialogPreference::OnRestoreInstanceState(superState);
    String value;
    myState->GetValue(&value);
    SetValue(value);
    return NOERROR;
}

ECode CListPreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    DialogPreference::Init(context, attrs);

    AutoPtr< ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::ListPreference),
            ARRAY_SIZE(R::styleable::ListPreference));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(attrs, attrIds, 0, 0, (ITypedArray**)&a);
    a->GetTextArray(R::styleable::ListPreference_entries, (ArrayOf<ICharSequence*>**)&mEntries);
    a->GetTextArray(R::styleable::ListPreference_entryValues, (ArrayOf<ICharSequence*>**)&mEntryValues);
    a->Recycle();

    /* Retrieve the Preference summary attribute since it's private
     * in the Preference class.
     */
    a = NULL;
    attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::Preference),
            ARRAY_SIZE(R::styleable::Preference));
    context->ObtainStyledAttributesEx3(attrs, attrIds, 0, 0, (ITypedArray**)&a);
    a->GetString(R::styleable::Preference_summary, &mSummary);
    a->Recycle();

    return NOERROR;
}

ECode CListPreference::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode CListPreference::SetDialogTitle(
    /* [in] */ ICharSequence* dialogTitle)
{
    return DialogPreference::SetDialogTitle(dialogTitle);
}

ECode CListPreference::SetDialogTitleEx(
    /* [in] */ Int32 dialogTitleResId)
{
    return DialogPreference::SetDialogTitleEx(dialogTitleResId);
}

ECode CListPreference::GetDialogTitle(
    /* [out] */ ICharSequence** title)
{
    return DialogPreference::GetDialogTitle(title);
}

ECode CListPreference::SetDialogMessage(
    /* [in] */ ICharSequence* dialogMessage)
{
    return DialogPreference::SetDialogMessage(dialogMessage);
}

ECode CListPreference::SetDialogMessageEx(
    /* [in] */ Int32 dialogMessageResId)
{
    return DialogPreference::SetDialogMessageEx(dialogMessageResId);
}

ECode CListPreference::GetDialogMessage(
    /* [out] */ ICharSequence** message)
{
    return DialogPreference::GetDialogMessage(message);
}

ECode CListPreference::SetDialogIcon(
    /* [in] */ IDrawable* dialogIcon)
{
    return DialogPreference::SetDialogIcon(dialogIcon);
}

ECode CListPreference::SetDialogIconEx(
    /* [in] */ Int32 dialogIconRes)
{
    return DialogPreference::SetDialogIconEx(dialogIconRes);
}

ECode CListPreference::GetDialogIcon(
    /* [out] */ IDrawable** icon)
{
    return DialogPreference::GetDialogIcon(icon);
}

ECode CListPreference::SetPositiveButtonText(
    /* [in] */ ICharSequence* positiveButtonText)
{
    return DialogPreference::SetPositiveButtonText(positiveButtonText);
}

ECode CListPreference::SetPositiveButtonTextEx(
    /* [in] */ Int32 positiveButtonTextResId)
{
    return DialogPreference::SetPositiveButtonTextEx(positiveButtonTextResId);
}

ECode CListPreference::GetPositiveButtonText(
    /* [out] */ ICharSequence** text)
{
    return DialogPreference::GetPositiveButtonText(text);
}

ECode CListPreference::SetNegativeButtonText(
    /* [in] */ ICharSequence* negativeButtonText)
{
    return DialogPreference::SetNegativeButtonText(negativeButtonText);
}

ECode CListPreference::SetNegativeButtonTextEx(
    /* [in] */ Int32 negativeButtonTextResId)
{
    return DialogPreference::SetNegativeButtonTextEx(negativeButtonTextResId);
}

ECode CListPreference::GetNegativeButtonText(
    /* [out] */ ICharSequence** text)
{
    return DialogPreference::GetNegativeButtonText(text);
}

ECode CListPreference::SetDialogLayoutResource(
    /* [in] */ Int32 dialogLayoutResId)
{
    return DialogPreference::SetDialogLayoutResource(dialogLayoutResId);
}

ECode CListPreference::GetDialogLayoutResource(
    /* [out] */ Int32* layoutResId)
{
    return DialogPreference::GetDialogLayoutResource(layoutResId);
}

ECode CListPreference::ShowDialog(
    /* [in] */ IBundle* state)
{
    return DialogPreference::ShowDialog(state);
}

ECode CListPreference::NeedInputMethod(
    /* [out] */ Boolean* isNeed)
{
    return DialogPreference::NeedInputMethod(isNeed);
}

ECode CListPreference::OnCreateDialogView(
    /* [out] */ IView** view)
{
    return DialogPreference::OnCreateDialogView(view);
}

ECode CListPreference::OnBindDialogView(
    /* [in] */ IView* view)
{
    return DialogPreference::OnBindDialogView(view);
}

ECode CListPreference::GetDialog(
    /* [out] */ IDialog** dialog)
{
    return DialogPreference::GetDialog(dialog);
}

}
}
}
