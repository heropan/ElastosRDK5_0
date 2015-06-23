
#include "MultiCheckPreference.h"
#include "CMultiCheckPreferenceSavedState.h"
#include "R.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Core::IArrayOf;
using Elastos::Core::CArrayOf;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::Core::EIID_IBoolean;
using Elastos::Core::CStringWrapper;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnMultiChoiceClickListener;
using Elastos::Droid::Content::Res::IResources;

namespace Elastos {
namespace Droid {
namespace Preference {

/////////////////////////////////////////////////////
// MultiCheckPreference::MultiChoiceClickListener
/////////////////////////////////////////////////////

MultiCheckPreference::MultiChoiceClickListener::MultiChoiceClickListener(
    /* [in] */ MultiCheckPreference* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(MultiCheckPreference::MultiChoiceClickListener, IDialogInterfaceOnMultiChoiceClickListener)

ECode MultiCheckPreference::MultiChoiceClickListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which,
    /* [in] */ Boolean isChecked)
{
    (*mHost->mSetValues)[which] = isChecked;
    return NOERROR;
}


/////////////////////////////////////////////////////
// MultiCheckPreference
/////////////////////////////////////////////////////

MultiCheckPreference::MultiCheckPreference(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : DialogPreference(context, attrs)
{
    Init(context, attrs);
}

MultiCheckPreference::MultiCheckPreference(
    /* [in] */ IContext* context)
    : DialogPreference(context, NULL)
{
    Init(context, NULL);
}

void MultiCheckPreference::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::ListPreference),
            ARRAY_SIZE(R::styleable::ListPreference));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, 0, 0, (ITypedArray**)&a);
    mEntries = NULL;
    a->GetTextArray(R::styleable::ListPreference_entries, (ArrayOf<ICharSequence*>**)&mEntries);
    if (mEntries != NULL) {
        SetEntries(mEntries);
    }
    AutoPtr< ArrayOf<ICharSequence*> > textArray;
    a->GetTextArray(R::styleable::ListPreference_entryValues, (ArrayOf<ICharSequence*>**)&textArray);
    SetEntryValuesCS(textArray);
    a->Recycle();

    /* Retrieve the Preference summary attribute since it's private
     * in the Preference class.
     */
    a = NULL;
    attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::Preference),
            ARRAY_SIZE(R::styleable::Preference));
    context->ObtainStyledAttributes(attrs, attrIds, 0, 0, (ITypedArray**)&a);
    a->GetString(R::styleable::Preference_summary, &mSummary);
    a->Recycle();
}

PInterface MultiCheckPreference::Probe(
    /* [in] */REIID riid)
{
    if (riid == EIID_IDialogPreference) {
        return (IDialogPreference*)this;
    }

    return DialogPreference::Probe(riid);
}

UInt32 MultiCheckPreference::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MultiCheckPreference::Release()
{
    return ElRefBase::Release();
}

ECode MultiCheckPreference::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface *)(IDialogPreference*)this) {
        *pIID = EIID_IDialogPreference;
        return NOERROR;
    }
    return DialogPreference::GetInterfaceID(pObject, pIID);
}

void MultiCheckPreference::SetEntries(
    /* [in] */ ArrayOf<ICharSequence*>* entries)
{
    mEntries = entries;
    mSetValues = ArrayOf<Boolean>::Alloc(entries->GetLength());
    mOrigValues = ArrayOf<Boolean>::Alloc(entries->GetLength());
}

void MultiCheckPreference::SetEntries(
    /* [in] */ Int32 entriesResId)
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    AutoPtr< ArrayOf<ICharSequence*> > entries;
    resources->GetTextArray(entriesResId, (ArrayOf<ICharSequence*>**)&entries);
    SetEntries(entries);
}

AutoPtr< ArrayOf<ICharSequence*> > MultiCheckPreference::GetEntries()
{
    return mEntries;
}

void MultiCheckPreference::SetEntryValues(
    /* [in] */ ArrayOf<String>* entryValues)
{
    mEntryValues = entryValues;
    for (Int32 i = 0; i < mSetValues->GetLength(); ++i) {
        (*mSetValues)[i] = FALSE;
        (*mOrigValues)[i] = FALSE;
    }
}

void MultiCheckPreference::SetEntryValues(
    /* [in] */ Int32 entryValuesResId)
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    AutoPtr< ArrayOf<ICharSequence*> > entries;
    resources->GetTextArray(entryValuesResId, (ArrayOf<ICharSequence*>**)&entries);
    SetEntryValuesCS(entries);
}

void MultiCheckPreference::SetEntryValuesCS(
    /* [in] */ ArrayOf<ICharSequence*>* values)
{
    SetValues(NULL);
    if (values != NULL) {
        mEntryValues = ArrayOf<String>::Alloc(values->GetLength());
        for (Int32 i = 0; i < values->GetLength(); i++) {
            ((*values)[i])->ToString(&((*mEntryValues)[i]));
        }
    }
}

AutoPtr< ArrayOf<String> > MultiCheckPreference::GetEntryValues()
{
    return mEntryValues;
}

Boolean MultiCheckPreference::GetValue(
    /* [in] */ Int32 index)
{
    return (*mSetValues)[index];
}

void MultiCheckPreference::SetValue(
    /* [in] */ Int32 index,
    /* [in] */ Boolean state)
{
    (*mSetValues)[index] = state;
}

void MultiCheckPreference::SetValues(
    /* [in] */ ArrayOf<Boolean>* values)
{
    if (mSetValues != NULL) {
        for (Int32 i = 0; i < mSetValues->GetLength(); i++) {
            (*mSetValues)[i] = FALSE;
            (*mOrigValues)[i] = FALSE;
        }
        if (values != NULL) {
            mSetValues->Copy(values, values->GetLength() < mSetValues->GetLength() ? values->GetLength() : mSetValues->GetLength());
        }
    }
}

ECode MultiCheckPreference::GetSummary(
    /* [out] */ ICharSequence** summary)
{
    VALIDATE_NOT_NULL(summary)
    if (mSummary.IsNull()) {
        return DialogPreference::GetSummary(summary);
    }
    else {
        return CStringWrapper::New(mSummary, summary);
    }
}

ECode MultiCheckPreference::SetSummary(
    /* [in] */ ICharSequence* summary)
{
    FAIL_RETURN(DialogPreference::SetSummary(summary))
    if (summary == NULL && !mSummary.IsNull()) {
        mSummary = String(NULL);
    }
    else if (summary != NULL) {
        String str;
        summary->ToString(&str);
        if (!str.Equals(mSummary)){
            mSummary = str;
        }
    }

    return NOERROR;
}

AutoPtr< ArrayOf<Boolean> > MultiCheckPreference::GetValues()
{
    return mSetValues;
}

Int32 MultiCheckPreference::FindIndexOfValue(
    /* [in] */ const String& value)
{
    if (!value.IsNull() && mEntryValues != NULL) {
        for (Int32 i = mEntryValues->GetLength() - 1; i >= 0; i--) {
            if (((*mEntryValues)[i]).Equals(value)) {
                return i;
            }
        }
    }
    return -1;
}

ECode MultiCheckPreference::OnPrepareDialogBuilder(
    /* [in] */ IAlertDialogBuilder* builder)
{
    FAIL_RETURN(DialogPreference::OnPrepareDialogBuilder(builder))

    if (mEntries == NULL || mEntryValues == NULL) {
        Logger::E("MultiCheckPreference", "ListPreference requires an entries array and an entryValues array.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    mOrigValues->Copy(mSetValues);
    AutoPtr<IDialogInterfaceOnMultiChoiceClickListener> listener = new MultiChoiceClickListener(this);
    return builder->SetMultiChoiceItems(mEntries, mSetValues, listener);
}

ECode MultiCheckPreference::OnDialogClosed(
    /* [in] */ Boolean positiveResult)
{
    FAIL_RETURN(DialogPreference::OnDialogClosed(positiveResult))

    if (positiveResult) {
        AutoPtr< ArrayOf<Boolean> > values = GetValues();
        AutoPtr<IArrayOf> array;
        CArrayOf::New(EIID_IBoolean, values->GetLength(), (IArrayOf**)&array);
        for (Int32 i = 0; i < values->GetLength(); ++i) {
            AutoPtr<IBoolean> b;
            CBoolean::New((*values)[i], (IBoolean**)&b);
            array->Put(i, b);
        }
        Boolean result;
        if (CallChangeListener(array, &result), result) {
            return NOERROR;
        }
    }
    mSetValues->Copy(mOrigValues, mSetValues->GetLength());

    return NOERROR;
}

ECode MultiCheckPreference::OnGetDefaultValue(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 index,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value)
    String str;
    a->GetString(index, &str);
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(str, (ICharSequence**)&cs);
    *value = (IInterface*)cs;
    REFCOUNT_ADD(*value)
    return NOERROR;
}

ECode MultiCheckPreference::OnSetInitialValue(
    /* [in] */ Boolean restorePersistedValue,
    /* [in] */ IInterface* defaultValue)
{
    return NOERROR;
}

ECode MultiCheckPreference::OnSaveInstanceState(
    /* [out] */ IParcelable** state)
{
    VALIDATE_NOT_NULL(state)

    AutoPtr<IParcelable> superState;
    DialogPreference::OnSaveInstanceState((IParcelable**)&superState);
    Boolean isPersistent;
    if (IsPersistent(&isPersistent), isPersistent) {
        // No need to save instance state since it's persistent
        *state = superState;
        REFCOUNT_ADD(*state)
        return NOERROR;
    }

    AutoPtr<IMultiCheckPreferenceSavedState> myState;
    CMultiCheckPreferenceSavedState::New(superState, (IMultiCheckPreferenceSavedState**)&myState);
    AutoPtr< ArrayOf<Boolean> > values = GetValues();
    myState->SetValues(values);
    *state = IParcelable::Probe(myState);
    REFCOUNT_ADD(*state)
    return NOERROR;
}

ECode MultiCheckPreference::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    if (state == NULL || IMultiCheckPreferenceSavedState::Probe(state) == NULL) {
        // Didn't save state for us in onSaveInstanceState
        return DialogPreference::OnRestoreInstanceState(state);
    }

    AutoPtr<IMultiCheckPreferenceSavedState> myState = IMultiCheckPreferenceSavedState::Probe(state);
    AutoPtr<IParcelable> superParcel;
    myState->GetSuperState((IParcelable**)&superParcel);
    DialogPreference::OnRestoreInstanceState(superParcel);
    AutoPtr< ArrayOf<Boolean> > values;
    myState->GetValues((ArrayOf<Boolean>**)&values);
    SetValues(values);

    return NOERROR;
}

ECode MultiCheckPreference::SetDialogTitle(
    /* [in] */ ICharSequence* dialogTitle)
{
    return DialogPreference::SetDialogTitle(dialogTitle);
}

ECode MultiCheckPreference::SetDialogTitle(
    /* [in] */ Int32 dialogTitleResId)
{
    return DialogPreference::SetDialogTitle(dialogTitleResId);
}

ECode MultiCheckPreference::GetDialogTitle(
    /* [out] */ ICharSequence** title)
{
    return DialogPreference::GetDialogTitle(title);
}

ECode MultiCheckPreference::SetDialogMessage(
    /* [in] */ ICharSequence* dialogMessage)
{
    return DialogPreference::SetDialogMessage(dialogMessage);
}

ECode MultiCheckPreference::SetDialogMessage(
    /* [in] */ Int32 dialogMessageResId)
{
    return DialogPreference::SetDialogMessage(dialogMessageResId);
}

ECode MultiCheckPreference::GetDialogMessage(
    /* [out] */ ICharSequence** message)
{
    return DialogPreference::GetDialogMessage(message);
}

ECode MultiCheckPreference::SetDialogIcon(
    /* [in] */ IDrawable* dialogIcon)
{
    return DialogPreference::SetDialogIcon(dialogIcon);
}

ECode MultiCheckPreference::SetDialogIcon(
    /* [in] */ Int32 dialogIconRes)
{
    return DialogPreference::SetDialogIcon(dialogIconRes);
}

ECode MultiCheckPreference::GetDialogIcon(
    /* [out] */ IDrawable** icon)
{
    return DialogPreference::GetDialogIcon(icon);
}

ECode MultiCheckPreference::SetPositiveButtonText(
    /* [in] */ ICharSequence* positiveButtonText)
{
    return DialogPreference::SetPositiveButtonText(positiveButtonText);
}

ECode MultiCheckPreference::SetPositiveButtonText(
    /* [in] */ Int32 positiveButtonTextResId)
{
    return DialogPreference::SetPositiveButtonText(positiveButtonTextResId);
}

ECode MultiCheckPreference::GetPositiveButtonText(
    /* [out] */ ICharSequence** text)
{
    return DialogPreference::GetPositiveButtonText(text);
}

ECode MultiCheckPreference::SetNegativeButtonText(
    /* [in] */ ICharSequence* negativeButtonText)
{
    return DialogPreference::SetNegativeButtonText(negativeButtonText);
}

ECode MultiCheckPreference::SetNegativeButtonText(
    /* [in] */ Int32 negativeButtonTextResId)
{
    return DialogPreference::SetNegativeButtonText(negativeButtonTextResId);
}

ECode MultiCheckPreference::GetNegativeButtonText(
    /* [out] */ ICharSequence** text)
{
    return DialogPreference::GetNegativeButtonText(text);
}

ECode MultiCheckPreference::SetDialogLayoutResource(
    /* [in] */ Int32 dialogLayoutResId)
{
    return DialogPreference::SetDialogLayoutResource(dialogLayoutResId);
}

ECode MultiCheckPreference::GetDialogLayoutResource(
    /* [out] */ Int32* layoutResId)
{
    return DialogPreference::GetDialogLayoutResource(layoutResId);
}

ECode MultiCheckPreference::ShowDialog(
    /* [in] */ IBundle* state)
{
    return DialogPreference::ShowDialog(state);
}

ECode MultiCheckPreference::NeedInputMethod(
    /* [out] */ Boolean* isNeed)
{
    return DialogPreference::NeedInputMethod(isNeed);
}

ECode MultiCheckPreference::OnCreateDialogView(
    /* [out] */ IView** view)
{
    return DialogPreference::OnCreateDialogView(view);
}

ECode MultiCheckPreference::GetDialog(
    /* [out] */ IDialog** dialog)
{
    return DialogPreference::GetDialog(dialog);
}

}
}
}

