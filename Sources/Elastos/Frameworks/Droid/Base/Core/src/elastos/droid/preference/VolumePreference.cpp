
#include "elastos/droid/preference/VolumePreference.h"
#include "elastos/droid/preference/CVolumePreferenceSavedState.h"
#include "elastos/droid/os/CHandler.h"
#include "elastos/droid/R.h"
#include "elastos/droid/preference/CSeekBarVolumizer.h"

using Elastos::Droid::Media::IRingtone;
using Elastos::Droid::Media::IRingtoneManagerHelper;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::View::IWindow;
using Elastos::Droid::View::EIID_IViewOnKeyListener;
using Elastos::Droid::Widget::EIID_ISeekBarOnSeekBarChangeListener;
using Elastos::Droid::R;
using Elastos::Droid::Preference::CSeekBarVolumizer;

namespace Elastos {
namespace Droid {
namespace Preference {

/////////////////////////VolumePreference::VolumeStore//////////////////////////////////////

CAR_INTERFACE_IMPL(VolumePreference::VolumeStore, Object, IVolumePreferenceVolumeStore)

VolumePreference::VolumeStore::VolumeStore()
    : volume(-1)
    , originalVolume(-1)
{
}

ECode VolumePreference::VolumeStore::constructor()
{
    return NOERROR;
}

ECode VolumePreference::VolumeStore::GetVolume(
    /* [out] */ Int32* volume)
{
    VALIDATE_NOT_NULL(volume)
    *volume = this->volume;
    return NOERROR;
}

ECode VolumePreference::VolumeStore::GetOriginalVolume(
    /* [out] */ Int32* volume)
{
    VALIDATE_NOT_NULL(volume)
    *volume = this->originalVolume;
    return NOERROR;
}

ECode VolumePreference::VolumeStore::SetVolume(
    /* [in] */ Int32 volume)
{
    this->volume = volume;
    return NOERROR;
}

ECode VolumePreference::VolumeStore::SetOriginalVolume(
    /* [in] */ Int32 volume)
{
    this->originalVolume = volume;
    return NOERROR;
}

/////////////////////////VolumePreference//////////////////////////////////////////////////////////

const String VolumePreference::TAG("VolumePreference");

CAR_INTERFACE_IMPL_4(VolumePreference, SeekBarDialogPreference,IVolumePreference, IPreferenceManagerOnActivityStopListener,IViewOnKeyListener,ISeekBarVolumizerCallback);

VolumePreference::VolumePreference()
    : mStreamType(0)
{
}

ECode VolumePreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    return Init(context, attrs, defStyleAttr, defStyleRes);
}

ECode VolumePreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
{
    return constructor(context, attrs, defStyleAttr, 0);
}

ECode VolumePreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, R::attr::dialogPreferenceStyle);
}

ECode VolumePreference::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    SeekBarDialogPreference::constructor(context, attrs, defStyleAttr, defStyleRes);
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::VolumePreference),
            ARRAY_SIZE(R::styleable::VolumePreference));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, defStyleAttr, defStyleRes, (ITypedArray**)&a);
    a->GetInt32(R::styleable::VolumePreference_streamType, 0, &mStreamType);
    a->Recycle();
    return NOERROR;
}

ECode VolumePreference::SetStreamType(
    /* [in] */ Int32 streamType)
{
    mStreamType = streamType;
    return NOERROR;
}

ECode VolumePreference::OnBindDialogView(
    /* [in] */ IView* view)
{
    FAIL_RETURN(SeekBarDialogPreference::OnBindDialogView(view))

    AutoPtr<ISeekBar> seekBar;
    view->FindViewById(R::id::seekbar, (IView**)&seekBar);
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    CSeekBarVolumizer::New(context, mStreamType, NULL,
            ISeekBarVolumizerCallback::Probe(this),(ISeekBarVolumizer**)&mSeekBarVolumizer);
    mSeekBarVolumizer->SetSeekBar(seekBar);

    AutoPtr<IPreferenceManager> manager;
    GetPreferenceManager((IPreferenceManager**)&manager);
    manager->RegisterOnActivityStopListener((IPreferenceManagerOnActivityStopListener*)this);

    // grab focus and key events so that pressing the volume buttons in the
    // dialog doesn't also show the normal volume adjust toast.
    view->SetOnKeyListener((IViewOnKeyListener*)this);
    view->SetFocusableInTouchMode(TRUE);
    Boolean result;
    view->RequestFocus(&result);
    return NOERROR;
}

ECode VolumePreference::OnKey(
    /* [in] */ IView* v,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    // If key arrives immediately after the activity has been cleaned up.
    if (mSeekBarVolumizer == NULL) {
        *result = TRUE;
        return NOERROR;
    }
    Int32 action;
    event->GetAction(&action);
    Boolean isdown = (action == IKeyEvent::ACTION_DOWN);
    switch (keyCode) {
        case IKeyEvent::KEYCODE_VOLUME_DOWN: {
            if (isdown) {
                mSeekBarVolumizer->ChangeVolumeBy(-1);
            }
            *result = TRUE;
        }
        case IKeyEvent::KEYCODE_VOLUME_UP: {
            if (isdown) {
                mSeekBarVolumizer->ChangeVolumeBy(1);
            }
            *result = TRUE;
        }
        case IKeyEvent::KEYCODE_VOLUME_MUTE: {
            if (isdown) {
                mSeekBarVolumizer->MuteVolume();
            }
            *result = TRUE;
        }
        default: {
            *result = FALSE;
        }
    }

    return NOERROR;
}

ECode VolumePreference::OnDialogClosed(
    /* [in] */ Boolean positiveResult)
{
    FAIL_RETURN(SeekBarDialogPreference::OnDialogClosed(positiveResult))
    if (!positiveResult && mSeekBarVolumizer != NULL) {
        mSeekBarVolumizer->RevertVolume();
    }
    Cleanup();
    return NOERROR;
}

ECode VolumePreference::OnActivityStop()
{
    if (mSeekBarVolumizer != NULL) {
        mSeekBarVolumizer->PostStopSample();
    }
    return NOERROR;
}

void VolumePreference::Cleanup()
{
    AutoPtr<IPreferenceManager> manager;
    GetPreferenceManager((IPreferenceManager**)&manager);
    manager->UnregisterOnActivityStopListener((IPreferenceManagerOnActivityStopListener*)this);

    if (mSeekBarVolumizer != NULL) {
        AutoPtr<IDialog> dialog;
        GetDialog((IDialog**)&dialog);
        Boolean isShowing;
        if (dialog != NULL && (dialog->IsShowing(&isShowing), isShowing)) {
            AutoPtr<IWindow> window;
            dialog->GetWindow((IWindow**)&window);
            AutoPtr<IView> decorView;
            window->GetDecorView((IView**)&decorView);
            AutoPtr<IView> view;
            decorView->FindViewById(R::id::seekbar, (IView**)&view);
            if (view != NULL) {
                view->SetOnKeyListener(NULL);
            }
            // Stopped while dialog was showing, revert changes
            mSeekBarVolumizer->RevertVolume();
        }
        mSeekBarVolumizer->Stop();
        mSeekBarVolumizer = NULL;
    }
}

ECode VolumePreference::OnSampleStarting(
    /* [in] */ ISeekBarVolumizer* volumizer)
{
    if (mSeekBarVolumizer != NULL && volumizer != mSeekBarVolumizer.Get()) {
        mSeekBarVolumizer->StopSample();
    }
    return NOERROR;
}

ECode VolumePreference::OnSaveInstanceState(
    /* [out] */ IParcelable** state)
{
    VALIDATE_NOT_NULL(state)

    AutoPtr<IParcelable> superState;
    SeekBarDialogPreference::OnSaveInstanceState((IParcelable**)&superState);
    Boolean isPersistent;
    if (IsPersistent(&isPersistent), isPersistent) {
        // No need to save instance state since it's persistent
        *state = superState;
        REFCOUNT_ADD(*state)
        return NOERROR;
    }

    AutoPtr<IVolumePreferenceSavedState> myState;
    CVolumePreferenceSavedState::New(superState, (IVolumePreferenceSavedState**)&myState);
    if (mSeekBarVolumizer != NULL) {
        AutoPtr<IVolumePreferenceVolumeStore> store;
        myState->GetVolumeStore((IVolumePreferenceVolumeStore**)&store);
        mSeekBarVolumizer->OnSaveInstanceState(store);
    }
    *state = IParcelable::Probe(myState);
    REFCOUNT_ADD(*state)
    return NOERROR;
}

ECode VolumePreference::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    if (state == NULL || IVolumePreferenceSavedState::Probe(state) == NULL) {
        // Didn't save state for us in onSaveInstanceState
        return SeekBarDialogPreference::OnRestoreInstanceState(state);
    }

    AutoPtr<IVolumePreferenceSavedState> myState = IVolumePreferenceSavedState::Probe(state);
    AutoPtr<IPreferenceBaseSavedState> pbss = IPreferenceBaseSavedState::Probe(myState);
    AutoPtr<IParcelable> superParcel;
    pbss->GetSuperState((IParcelable**)&superParcel);
    SeekBarDialogPreference::OnRestoreInstanceState(superParcel);
    if (mSeekBarVolumizer != NULL) {
        AutoPtr<IVolumePreferenceVolumeStore> store;
        myState->GetVolumeStore((IVolumePreferenceVolumeStore**)&store);
        mSeekBarVolumizer->OnRestoreInstanceState(store);
    }

    return NOERROR;
}

}
}
}

