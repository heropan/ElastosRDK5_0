#include "elastos/droid/media/session/CMediaControllerTransportControls.h"
#include "elastos/droid/media/session/CMediaController.h"

namespace Elastos {
namespace Droid {
namespace Media {
namespace Session {

String CMediaControllerTransportControls::TAG("MediaControllerTransportControls");

CAR_INTERFACE_IMPL(CMediaControllerTransportControls, Object, IMediaControllerTransportControls)

CAR_OBJECT_IMPL(CMediaControllerTransportControls)

CMediaControllerTransportControls::CMediaControllerTransportControls()
{
}

CMediaControllerTransportControls::~CMediaControllerTransportControls()
{
}

ECode CMediaControllerTransportControls::constructor(
    /* [in] */ IMediaController * host)
{
    mHost = host;
    return NOERROR;
}

ECode CMediaControllerTransportControls::Play()
{
    // try {
    return ((CMediaController*)mHost.Get())->mSessionBinder->Play();
    // } catch (RemoteException e) {
    //     Log.wtf(TAG, "Error calling play.", e);
    // }
}

ECode CMediaControllerTransportControls::PlayFromMediaId(
    /* [in] */ const String& mediaId,
    /* [in] */ IBundle * extras)
{
    if (mediaId.IsEmpty()) {
        // throw new IllegalArgumentException(
        //         "You must specify a non-empty String for playFromMediaId.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // try {
    return ((CMediaController*)mHost.Get())->mSessionBinder->PlayFromMediaId(mediaId, extras);
    // } catch (RemoteException e) {
    //     Log.wtf(TAG, "Error calling play(" + mediaId + ").", e);
    // }
}

ECode CMediaControllerTransportControls::PlayFromSearch(
    /* [in] */ const String& query,
    /* [in] */ IBundle * extras)
{
    String str = query;
    if (str == NULL) {
        // This is to remain compatible with
        // INTENT_ACTION_MEDIA_PLAY_FROM_SEARCH
        str = "";
    }
    // try {
    return ((CMediaController*)mHost.Get())->mSessionBinder->PlayFromSearch(str, extras);
    // } catch (RemoteException e) {
    //     Log.wtf(TAG, "Error calling play(" + query + ").", e);
    // }
}

ECode CMediaControllerTransportControls::SkipToQueueItem(
    /* [in] */ Int64 id)
{
    // try {
    return ((CMediaController*)mHost.Get())->mSessionBinder->SkipToQueueItem(id);
    // } catch (RemoteException e) {
    //     Log.wtf(TAG, "Error calling skipToItem(" + id + ").", e);
    // }
}

ECode CMediaControllerTransportControls::Pause()
{
    // try {
    return ((CMediaController*)mHost.Get())->mSessionBinder->Pause();
    // } catch (RemoteException e) {
    //     Log.wtf(TAG, "Error calling pause.", e);
    // }
}

ECode CMediaControllerTransportControls::Stop()
{
    // try {
    return ((CMediaController*)mHost.Get())->mSessionBinder->Stop();
    // } catch (RemoteException e) {
    //     Log.wtf(TAG, "Error calling stop.", e);
    // }
}

ECode CMediaControllerTransportControls::SeekTo(
    /* [in] */ Int64 pos)
{
    // try {
    return ((CMediaController*)mHost.Get())->mSessionBinder->SeekTo(pos);
    // } catch (RemoteException e) {
    //     Log.wtf(TAG, "Error calling seekTo.", e);
    // }
}

ECode CMediaControllerTransportControls::FastForward()
{
    // try {
    return ((CMediaController*)mHost.Get())->mSessionBinder->FastForward();
    // } catch (RemoteException e) {
    //     Log.wtf(TAG, "Error calling fastForward.", e);
    // }
}

ECode CMediaControllerTransportControls::SkipToNext()
{
    // try {
    return ((CMediaController*)mHost.Get())->mSessionBinder->Next();
    // } catch (RemoteException e) {
    //     Log.wtf(TAG, "Error calling next.", e);
    // }
}

ECode CMediaControllerTransportControls::Rewind()
{
    // try {
    return ((CMediaController*)mHost.Get())->mSessionBinder->Rewind();
    // } catch (RemoteException e) {
    //     Log.wtf(TAG, "Error calling rewind.", e);
    // }
}

ECode CMediaControllerTransportControls::SkipToPrevious()
{
    // try {
    return ((CMediaController*)mHost.Get())->mSessionBinder->Previous();
    // } catch (RemoteException e) {
    //     Log.wtf(TAG, "Error calling previous.", e);
    // }
}

ECode CMediaControllerTransportControls::SetRating(
    /* [in] */ IRating * rating)
{
    // try {
    return ((CMediaController*)mHost.Get())->mSessionBinder->Rate(rating);
    // } catch (RemoteException e) {
    //     Log.wtf(TAG, "Error calling rate.", e);
    // }
}

ECode CMediaControllerTransportControls::SendCustomAction(
    /* [in] */ IPlaybackStateCustomAction * customAction,
    /* [in] */ IBundle * args)
{
    if (customAction == NULL) {
        // throw new IllegalArgumentException("CustomAction cannot be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String action;
    customAction->GetAction(&action);
    return SendCustomAction(action, args);
}

ECode CMediaControllerTransportControls::SendCustomAction(
    /* [in] */ const String& action,
    /* [in] */ IBundle * args)
{
    if (action.IsEmpty()) {
        // throw new IllegalArgumentException("CustomAction cannot be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // try {
    return ((CMediaController*)mHost.Get())->mSessionBinder->SendCustomAction(action, args);
    // } catch (RemoteException e) {
    //     Log.d(TAG, "Dead object in sendCustomAction.", e);
    // }
}


} // namespace Session
} // namespace Media
} // namepsace Droid
} // namespace Elastos
