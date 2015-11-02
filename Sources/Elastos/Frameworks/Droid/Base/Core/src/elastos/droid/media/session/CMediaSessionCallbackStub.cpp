#include "elastos/droid/media/session/CMediaSessionCallbackStub.h"
#include "elastos/droid/media/session/CMediaSession.h"

namespace Elastos {
namespace Droid {
namespace Media {
namespace Session {

CAR_INTERFACE_IMPL(CMediaSessionCallbackStub, Object, IISessionCallback)

CAR_OBJECT_IMPL(CMediaSessionCallbackStub)

CMediaSessionCallbackStub::CMediaSessionCallbackStub()
{
}

CMediaSessionCallbackStub::~CMediaSessionCallbackStub()
{
}

ECode CMediaSessionCallbackStub::constructor(
    /* [in] */ IMediaSession * session)
{
    mMediaSession = session;
    return NOERROR;
}

ECode CMediaSessionCallbackStub::OnCommand(
    /* [in] */ const String& command,
    /* [in] */ IBundle * args,
    /* [in] */ IResultReceiver * cb)
{
    if (mMediaSession != NULL) {
        ((CMediaSession*)mMediaSession.Get())->PostCommand(command, args, cb);
    }
    return NOERROR;
}

ECode CMediaSessionCallbackStub::OnMediaButton(
    /* [in] */ IIntent * mediaButtonIntent,
    /* [in] */ Int32 sequenceNumber,
    /* [in] */ IResultReceiver * cb)
{
    // try {
        if (mMediaSession != NULL) {
            ((CMediaSession*)mMediaSession.Get())->DispatchMediaButton(mediaButtonIntent);
        }
    // } finally {
        if (cb != NULL) {
            cb->Send(sequenceNumber, NULL);
        }
    // }
    return NOERROR;
}

ECode CMediaSessionCallbackStub::OnPlay()
{
    if (mMediaSession != NULL) {
        ((CMediaSession*)mMediaSession.Get())->DispatchPlay();
    }
    return NOERROR;
}

ECode CMediaSessionCallbackStub::OnPlayFromMediaId(
    /* [in] */ const String& mediaId,
    /* [in] */ IBundle * extras)
{
    if (mMediaSession != NULL) {
        ((CMediaSession*)mMediaSession.Get())->DispatchPlayFromMediaId(mediaId, extras);
    }
    return NOERROR;
}

ECode CMediaSessionCallbackStub::OnPlayFromSearch(
    /* [in] */ const String& query,
    /* [in] */ IBundle * extras)
{
    if (mMediaSession != NULL) {
        ((CMediaSession*)mMediaSession.Get())->DispatchPlayFromSearch(query, extras);
    }
    return NOERROR;
}

ECode CMediaSessionCallbackStub::OnSkipToTrack(
    /* [in] */ Int64 id)
{
    if (mMediaSession != NULL) {
        ((CMediaSession*)mMediaSession.Get())->DispatchSkipToItem(id);
    }
    return NOERROR;
}

ECode CMediaSessionCallbackStub::OnPause()
{
    if (mMediaSession != NULL) {
        ((CMediaSession*)mMediaSession.Get())->DispatchPause();
    }
    return NOERROR;
}

ECode CMediaSessionCallbackStub::OnStop()
{
    if (mMediaSession != NULL) {
        ((CMediaSession*)mMediaSession.Get())->DispatchStop();
    }
    return NOERROR;
}

ECode CMediaSessionCallbackStub::OnNext()
{
    if (mMediaSession != NULL) {
        ((CMediaSession*)mMediaSession.Get())->DispatchNext();
    }
    return NOERROR;
}

ECode CMediaSessionCallbackStub::OnPrevious()
{
    if (mMediaSession != NULL) {
        ((CMediaSession*)mMediaSession.Get())->DispatchPrevious();
    }
    return NOERROR;
}

ECode CMediaSessionCallbackStub::OnFastForward()
{
    if (mMediaSession != NULL) {
        ((CMediaSession*)mMediaSession.Get())->DispatchFastForward();
    }
    return NOERROR;
}

ECode CMediaSessionCallbackStub::OnRewind()
{
    if (mMediaSession != NULL) {
        ((CMediaSession*)mMediaSession.Get())->DispatchRewind();
    }
    return NOERROR;
}

ECode CMediaSessionCallbackStub::OnSeekTo(
    /* [in] */ Int64 pos)
{
    if (mMediaSession != NULL) {
        ((CMediaSession*)mMediaSession.Get())->DispatchSeekTo(pos);
    }
    return NOERROR;
}

ECode CMediaSessionCallbackStub::OnRate(
    /* [in] */ IRating * rating)
{
    if (mMediaSession != NULL) {
        ((CMediaSession*)mMediaSession.Get())->DispatchRate(rating);
    }
    return NOERROR;
}

ECode CMediaSessionCallbackStub::OnCustomAction(
    /* [in] */ const String& action,
    /* [in] */ IBundle * args)
{
    if (mMediaSession != NULL) {
        ((CMediaSession*)mMediaSession.Get())->DispatchCustomAction(action, args);
    }
    return NOERROR;
}

ECode CMediaSessionCallbackStub::OnAdjustVolume(
    /* [in] */ Int32 direction)
{
    if (mMediaSession != NULL) {
        if (((CMediaSession*)mMediaSession.Get())->mVolumeProvider != NULL) {
            ((CMediaSession*)mMediaSession.Get())->mVolumeProvider->OnAdjustVolume(direction);
        }
    }
    return NOERROR;
}

ECode CMediaSessionCallbackStub::OnSetVolumeTo(
    /* [in] */ Int32 value)
{
    if (mMediaSession != NULL) {
        if (((CMediaSession*)mMediaSession.Get())->mVolumeProvider != NULL) {
            ((CMediaSession*)mMediaSession.Get())->mVolumeProvider->OnSetVolumeTo(value);
        }
    }
    return NOERROR;
}


} // namespace Session
} // namespace Media
} // namepsace Droid
} // namespace Elastos
