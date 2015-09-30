
#include "VolumePreference.h"
#include "CVolumePreferenceSavedState.h"
#include "media/CRingtoneManagerHelper.h"
#include "os/CHandler.h"
#include "provider/Settings.h"
#include "R.h"

using Elastos::Droid::Media::IRingtone;
using Elastos::Droid::Media::IRingtoneManagerHelper;
using Elastos::Droid::Media::CRingtoneManagerHelper;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Provider::Settings;
using Elastos::Droid::View::IWindow;
using Elastos::Droid::View::EIID_IViewOnKeyListener;
using Elastos::Droid::Widget::EIID_ISeekBarOnSeekBarChangeListener;

namespace Elastos {
namespace Droid {
namespace Preference {

const String VolumePreference::TAG("VolumePreference");


/////////////////////////////////////////////////////
// VolumePreference::SeekBarVolumizer::MyContentObserver
/////////////////////////////////////////////////////

VolumePreference::SeekBarVolumizer::MyContentObserver::MyContentObserver(
    /* [in] */ IHandler* handler,
    /* [in] */ SeekBarVolumizer* host)
    : ContentObserver(handler)
    , mHost(host)
{}

ECode VolumePreference::SeekBarVolumizer::MyContentObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    ContentObserver::OnChange(selfChange);
    if (mHost->mSeekBar != NULL && mHost->mAudioManager != NULL) {
        Int32 volume;
        mHost->mAudioManager->GetStreamVolume(mHost->mStreamType, &volume);
        mHost->mSeekBar->SetProgress(volume);
    }
    return NOERROR;
}


/////////////////////////////////////////////////////
// VolumePreference::SeekBarVolumizer
/////////////////////////////////////////////////////

VolumePreference::SeekBarVolumizer::SeekBarVolumizer(
    /* [in] */ IContext* context,
    /* [in] */ ISeekBar* seekBar,
    /* [in] */ Int32 streamType,
    /* [in] */ VolumePreference* host)
    : mContext(context)
    , mStreamType(streamType)
    , mOriginalStreamVolume(0)
    , mLastProgress(-1)
    , mSeekBar(seekBar)
    , mVolumeBeforeMute(-1)
    , mHost(host)
{
    CHandler::New((IHandler**)&mHandler);
    mVolumeObserver = new MyContentObserver(mHandler, this);
    Init(context, seekBar, streamType, NULL);
}

VolumePreference::SeekBarVolumizer::SeekBarVolumizer(
    /* [in] */ IContext* context,
    /* [in] */ ISeekBar* seekBar,
    /* [in] */ Int32 streamType,
    /* [in] */ IUri* defaultUri,
    /* [in] */ VolumePreference* host)
    : mContext(context)
    , mStreamType(streamType)
    , mOriginalStreamVolume(0)
    , mLastProgress(-1)
    , mSeekBar(seekBar)
    , mVolumeBeforeMute(-1)
    , mHost(host)
{
    CHandler::New((IHandler**)&mHandler);
    mVolumeObserver = new MyContentObserver(mHandler, this);
    Init(context, seekBar, streamType, defaultUri);
}

PInterface VolumePreference::SeekBarVolumizer::Probe(
    /* [in] */REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(ISeekBarOnSeekBarChangeListener*)this;
    }
    else if (riid == EIID_ISeekBarOnSeekBarChangeListener) {
        return (ISeekBarOnSeekBarChangeListener*)this;
    }
    return Runnable::Probe(riid);
}

UInt32 VolumePreference::SeekBarVolumizer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 VolumePreference::SeekBarVolumizer::Release()
{
    return ElRefBase::Release();
}

ECode VolumePreference::SeekBarVolumizer::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface *)(ISeekBarOnSeekBarChangeListener*)this) {
        *pIID = EIID_ISeekBarOnSeekBarChangeListener;
        return NOERROR;
    }
    return Runnable::GetInterfaceID(pObject, pIID);
}

void VolumePreference::SeekBarVolumizer::Init(
    /* [in] */ IContext* context,
    /* [in] */ ISeekBar* seekBar,
    /* [in] */ Int32 streamType,
    /* [in] */ IUri* defaultUri)
{
    context->GetSystemService(IContext::AUDIO_SERVICE, (IInterface**)&mAudioManager);
    InitSeekBar(seekBar, defaultUri);
}

void VolumePreference::SeekBarVolumizer::InitSeekBar(
    /* [in] */ ISeekBar* seekBar,
    /* [in] */ IUri* defaultUri)
{
    Int32 result;
    mAudioManager->GetStreamMaxVolume(mStreamType, &result);
    seekBar->SetMax(result);
    mAudioManager->GetStreamVolume(mStreamType, &mOriginalStreamVolume);
    seekBar->SetProgress(mOriginalStreamVolume);
    seekBar->SetOnSeekBarChangeListener(this);

    AutoPtr<IUri> uri;
    Settings::System::GetUriFor((*Settings::System::VOLUME_SETTINGS)[mStreamType], (IUri**)&uri);
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    cr->RegisterContentObserver(uri, FALSE, mVolumeObserver);

    if (defaultUri == NULL) {
        if (mStreamType == IAudioManager::STREAM_RING) {
            defaultUri = Settings::System::DEFAULT_RINGTONE_URI;
        }
        else if (mStreamType == IAudioManager::STREAM_NOTIFICATION) {
            defaultUri = Settings::System::DEFAULT_NOTIFICATION_URI;
        }
        else {
            defaultUri = Settings::System::DEFAULT_ALARM_ALERT_URI;
        }
    }

    AutoPtr<IRingtoneManagerHelper> helper;
    CRingtoneManagerHelper::AcquireSingleton((IRingtoneManagerHelper**)&helper);
    helper->GetRingtone(mContext, defaultUri, (IRingtone**)&mRingtone);

    if (mRingtone != NULL) {
        mRingtone->SetStreamType(mStreamType);
    }
}

void VolumePreference::SeekBarVolumizer::Stop()
{
    StopSample();
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    cr->UnregisterContentObserver(mVolumeObserver);
    mSeekBar->SetOnSeekBarChangeListener(NULL);
}

void VolumePreference::SeekBarVolumizer::RevertVolume()
{
    mAudioManager->SetStreamVolume(mStreamType, mOriginalStreamVolume, 0);
}

ECode VolumePreference::SeekBarVolumizer::OnProgressChanged(
    /* [in] */ ISeekBar* seekBar,
    /* [in] */ Int32 progress,
    /* [in] */ Boolean fromTouch)
{
    if (!fromTouch) {
        return NOERROR;
    }

    PostSetVolume(progress);
    return NOERROR;
}

void VolumePreference::SeekBarVolumizer::PostSetVolume(
    /* [in] */ Int32 progress)
{
    // Do the volume changing separately to give responsive UI
    mLastProgress = progress;
    mHandler->RemoveCallbacks(this);
    Boolean result;
    mHandler->Post(this, &result);
}

ECode VolumePreference::SeekBarVolumizer::OnStartTrackingTouch(
    /* [in] */ ISeekBar* seekBar)
{
    return NOERROR;
}

ECode VolumePreference::SeekBarVolumizer::OnStopTrackingTouch(
    /* [in] */ ISeekBar* seekBar)
{
    if (!IsSamplePlaying()) {
        StartSample();
    }
    return NOERROR;
}

ECode VolumePreference::SeekBarVolumizer::Run()
{
    mAudioManager->SetStreamVolume(mStreamType, mLastProgress, 0);
    return NOERROR;
}

Boolean VolumePreference::SeekBarVolumizer::IsSamplePlaying()
{
    Boolean isPlaying;
    return mRingtone != NULL && (mRingtone->IsPlaying(&isPlaying), isPlaying);
}

void VolumePreference::SeekBarVolumizer::StartSample()
{
    mHost->OnSampleStarting(this);
    if (mRingtone != NULL) {
        mRingtone->Play();
    }
}

void VolumePreference::SeekBarVolumizer::StopSample()
{
    if (mRingtone != NULL) {
        mRingtone->Stop();
    }
}

AutoPtr<ISeekBar> VolumePreference::SeekBarVolumizer::GetSeekBar()
{
    return mSeekBar;
}

void VolumePreference::SeekBarVolumizer::ChangeVolumeBy(
    /* [in] */ Int32 amount)
{
    mSeekBar->IncrementProgressBy(amount);
    if (!IsSamplePlaying()) {
        StartSample();
    }
    Int32 value;
    mSeekBar->GetProgress(&value);
    PostSetVolume(value);
    mVolumeBeforeMute = -1;
}

void VolumePreference::SeekBarVolumizer::MuteVolume()
{
    if (mVolumeBeforeMute != -1) {
        mSeekBar->SetProgress(mVolumeBeforeMute);
        StartSample();
        PostSetVolume(mVolumeBeforeMute);
        mVolumeBeforeMute = -1;
    }
    else {
        mSeekBar->GetProgress(&mVolumeBeforeMute);
        mSeekBar->SetProgress(0);
        StopSample();
        PostSetVolume(0);
    }
}

void VolumePreference::SeekBarVolumizer::OnSaveInstanceState(
    /* [in] */ VolumeStore* volumeStore)
{
    if (mLastProgress >= 0) {
        volumeStore->mVolume = mLastProgress;
        volumeStore->mOriginalVolume = mOriginalStreamVolume;
    }
}

void VolumePreference::SeekBarVolumizer::OnRestoreInstanceState(
    /* [in] */ VolumeStore* volumeStore)
{
    if (volumeStore->mVolume != -1) {
        mOriginalStreamVolume = volumeStore->mOriginalVolume;
        mLastProgress = volumeStore->mVolume;
        PostSetVolume(mLastProgress);
    }
}


/////////////////////////////////////////////////////
// VolumePreference
/////////////////////////////////////////////////////

VolumePreference::VolumePreference(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : SeekBarDialogPreference(context, attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::VolumePreference),
            ARRAY_SIZE(R::styleable::VolumePreference));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, 0, 0, (ITypedArray**)&a);
    a->GetInt32(R::styleable::VolumePreference_streamType, 0, &mStreamType);
    a->Recycle();
}

PInterface VolumePreference::Probe(
    /* [in] */REIID riid)
{
    if (riid == EIID_IPreferenceManagerOnActivityStopListener) {
        return (IPreferenceManagerOnActivityStopListener*)this;
    }
    else if (riid == EIID_IViewOnKeyListener) {
        return (IViewOnKeyListener*)this;
    }

    return SeekBarDialogPreference::Probe(riid);
}

UInt32 VolumePreference::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 VolumePreference::Release()
{
    return ElRefBase::Release();
}

ECode VolumePreference::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface *)(IPreferenceManagerOnActivityStopListener*)this) {
        *pIID = EIID_IPreferenceManagerOnActivityStopListener;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IViewOnKeyListener*)this) {
        *pIID = EIID_IViewOnKeyListener;
        return NOERROR;
    }
    return SeekBarDialogPreference::GetInterfaceID(pObject, pIID);
}

void VolumePreference::SetStreamType(
    /* [in] */ Int32 streamType)
{
    mStreamType = streamType;
}

ECode VolumePreference::OnBindDialogView(
    /* [in] */ IView* view)
{
    FAIL_RETURN(SeekBarDialogPreference::OnBindDialogView(view))

    AutoPtr<ISeekBar> seekBar;
    view->FindViewById(R::id::seekbar, (IView**)&seekBar);
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    mSeekBarVolumizer = new SeekBarVolumizer(context, seekBar, mStreamType, this);

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
        mSeekBarVolumizer->StopSample();
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
    /* [in] */ SeekBarVolumizer* volumizer)
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
        AutoPtr<VolumeStore> store = ((CVolumePreferenceSavedState*)myState.Get())->GetVolumeStore();
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
    AutoPtr<IParcelable> superParcel;
    myState->GetSuperState((IParcelable**)&superParcel);
    SeekBarDialogPreference::OnRestoreInstanceState(superParcel);
    if (mSeekBarVolumizer != NULL) {
        AutoPtr<VolumeStore> store = ((CVolumePreferenceSavedState*)myState.Get())->GetVolumeStore();
        mSeekBarVolumizer->OnRestoreInstanceState(store);
    }

    return NOERROR;
}

}
}
}

