
#include "SeekBarPreference.h"
#include "CSeekBarPreferenceSavedState.h"
#include "R.h"

using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Droid::Widget::EIID_ISeekBarOnSeekBarChangeListener;

namespace Elastos {
namespace Droid {
namespace Preference {

SeekBarPreference::SeekBarPreference(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : mProgress(0)
    , mMax(0)
    , mTrackingTouch(FALSE)
{
    Init(context, attrs, defStyle);
}

SeekBarPreference::SeekBarPreference(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : mProgress(0)
    , mMax(0)
    , mTrackingTouch(FALSE)
{
    Init(context, attrs, 0);
}

SeekBarPreference::SeekBarPreference(
    /* [in] */ IContext* context)
    : mProgress(0)
    , mMax(0)
    , mTrackingTouch(FALSE)
{
    Init(context, NULL, 0);
}

void SeekBarPreference::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    Preference::Init(context, attrs, defStyle);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::ProgressBar),
            ARRAY_SIZE(R::styleable::ProgressBar));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(attrs, attrIds, defStyle, 0, (ITypedArray**)&a);
    Int32 max;
    a->GetInt32(R::styleable::ProgressBar_max, mMax, &max);
    SetMax(max);
    a->Recycle();
    SetLayoutResource(R::layout::preference_widget_seekbar);
}

PInterface SeekBarPreference::Probe(
    /* [in] */REIID riid)
{
    if (riid == EIID_ISeekBarOnSeekBarChangeListener) {
        return (ISeekBarOnSeekBarChangeListener*)this;
    }

    return Preference::Probe(riid);
}

UInt32 SeekBarPreference::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 SeekBarPreference::Release()
{
    return ElRefBase::Release();
}

ECode SeekBarPreference::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface *)(ISeekBarOnSeekBarChangeListener*)this) {
        *pIID = EIID_ISeekBarOnSeekBarChangeListener;
        return NOERROR;
    }

    return Preference::GetInterfaceID(pObject, pIID);
}

ECode SeekBarPreference::OnBindView(
    /* [in] */ IView* view)
{
    FAIL_RETURN(Preference::OnBindView(view))
    AutoPtr<ISeekBar> seekBar;
    view->FindViewById(R::id::seekbar, (IView**)&seekBar);
    seekBar->SetOnSeekBarChangeListener(this);
    seekBar->SetMax(mMax);
    seekBar->SetProgress(mProgress);
    Boolean isEnabled;
    IsEnabled(&isEnabled);
    seekBar->SetEnabled(isEnabled);

    return NOERROR;
}

ECode SeekBarPreference::GetSummary(
    /* [out] */ ICharSequence** summary)
{
    VALIDATE_NOT_NULL(summary)
    *summary = NULL;
    return NOERROR;
}

ECode SeekBarPreference::OnSetInitialValue(
    /* [in] */ Boolean restoreValue,
    /* [in] */ IInterface* defaultValue)
{
    Int32 progress;
    if (restoreValue) {
        GetPersistedInt32(mProgress, &progress);
    }
    else {
        IInteger32::Probe(defaultValue)->GetValue(&progress);
    }
    SetProgress(progress);
    return NOERROR;
}

ECode SeekBarPreference::OnGetDefaultValue(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 index,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value)
    Int32 lvalue;
    a->GetInt32(index, 0, &lvalue);
    AutoPtr<IInteger32> integer;
    CInteger32::New(lvalue, (IInteger32**)&integer);
    *value = integer;
    INTERFACE_ADDREF(*value)
    return NOERROR;
}

ECode SeekBarPreference::OnKey(
    /* [in] */ IView* v,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    Int32 action;
    if (event->GetAction(&action), action != IKeyEvent::ACTION_UP) {
        if (keyCode == IKeyEvent::KEYCODE_PLUS || keyCode == IKeyEvent::KEYCODE_EQUALS) {
            SetProgress(GetProgress() + 1);
            *result = TRUE;
            return NOERROR;
        }
        if (keyCode == IKeyEvent::KEYCODE_MINUS) {
            SetProgress(GetProgress() - 1);
            *result = TRUE;
            return NOERROR;
        }
    }
    *result = FALSE;
    return NOERROR;
}

void SeekBarPreference::SetMax(
    /* [in] */ Int32 max)
{
    if (max != mMax) {
        mMax = max;
        NotifyChanged();
    }
}

void SeekBarPreference::SetProgress(
    /* [in] */ Int32 progress)
{
    SetProgress(progress, TRUE);
}

void SeekBarPreference::SetProgress(
    /* [in] */ Int32 progress,
    /* [in] */ Boolean notifyChanged)
{
    if (progress > mMax) {
        progress = mMax;
    }
    if (progress < 0) {
        progress = 0;
    }
    if (progress != mProgress) {
        mProgress = progress;
        Boolean result;
        PersistInt32(progress, &result);
        if (notifyChanged) {
            NotifyChanged();
        }
    }
}

Int32 SeekBarPreference::GetProgress()
{
    return mProgress;
}

void SeekBarPreference::SyncProgress(
    /* [in] */ ISeekBar* seekBar)
{
    Int32 progress;
    seekBar->GetProgress(&progress);
    if (progress != mProgress) {
        AutoPtr<IInteger32> iprogress;
        CInteger32::New(progress, (IInteger32**)&iprogress);
        Boolean result;
        if (CallChangeListener(iprogress, &result), result) {
            SetProgress(progress, FALSE);
        }
        else {
            seekBar->SetProgress(mProgress);
        }
    }
}

ECode SeekBarPreference::OnProgressChanged(
    /* [in] */ ISeekBar* seekBar,
    /* [in] */ Int32 progress,
    /* [in] */ Boolean fromUser)
{
    if (fromUser && !mTrackingTouch) {
        SyncProgress(seekBar);
    }
    return NOERROR;
}

ECode SeekBarPreference::OnStartTrackingTouch(
    /* [in] */ ISeekBar* seekBar)
{
    mTrackingTouch = TRUE;
    return NOERROR;
}

ECode SeekBarPreference::OnStopTrackingTouch(
    /* [in] */ ISeekBar* seekBar)
{
    mTrackingTouch = FALSE;
    Int32 progress;
    if (seekBar->GetProgress(&progress), progress != mProgress) {
        SyncProgress(seekBar);
    }
    return NOERROR;
}

ECode SeekBarPreference::OnSaveInstanceState(
    /* [out] */ IParcelable** state)
{
    VALIDATE_NOT_NULL(state)

    /*
     * Suppose a client uses this preference type without persisting. We
     * must save the instance state so it is able to, for example, survive
     * orientation changes.
     */

    AutoPtr<IParcelable> superState;
    Preference::OnSaveInstanceState(state);
    Boolean isPersistent;
    if (IsPersistent(&isPersistent), isPersistent) {
        // No need to save instance state since it's persistent
        *state = superState;
        INTERFACE_ADDREF(*state)
        return NOERROR;
    }

    // Save the instance state
    AutoPtr<ISeekBarPreferenceSavedState> myState;
    CSeekBarPreferenceSavedState::New(superState, (ISeekBarPreferenceSavedState**)&myState);
    myState->SetProgress(mProgress);
    myState->SetMax(mMax);
    *state = IParcelable::Probe(myState);
    INTERFACE_ADDREF(*state)
    return NOERROR;
}

ECode SeekBarPreference::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    if (ISeekBarPreferenceSavedState::Probe(state) == NULL) {
        // Didn't save state for us in onSaveInstanceState
        return OnRestoreInstanceState(state);
    }

    // Restore the instance state
    AutoPtr<ISeekBarPreferenceSavedState> myState = ISeekBarPreferenceSavedState::Probe(state);
    AutoPtr<IParcelable> superParcel;
    myState->GetSuperState((IParcelable**)&superParcel);
    Preference::OnRestoreInstanceState(superParcel);
    myState->GetProgress(&mProgress);
    myState->GetMax(&mMax);
    NotifyChanged();

    return NOERROR;
}

}
}
}

