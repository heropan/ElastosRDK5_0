#include "Effect.h"
#include "MediaArtistNativeHelper.h"
#include "MediaItem.h"

using Elastos::Droid::Media::Videoeditor::MediaArtistNativeHelper;
using Elastos::Droid::Media::Videoeditor::MediaItem;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

//f39abff2-69c0-4b52-94e2-ce6375872719
extern "C" const InterfaceID EIID_Effect =
        { 0xf39abff2, 0x69c0, 0x4b52, { 0x94, 0xe2, 0xce, 0x63, 0x75, 0x87, 0x27, 0x19 } };

Effect::Effect()
    : mUniqueId(NULL)
    , mDurationMs(0)
    , mStartTimeMs(0)
{
}

Effect::Effect(
    /* [in] */ IMediaItem* mediaItem,
    /* [in] */ const String& effectId,
    /* [in] */ Int64 startTimeMs,
    /* [in] */ Int64 durationMs)
    : mUniqueId(effectId)
    , mDurationMs(durationMs)
    , mStartTimeMs(startTimeMs)
{
    Init(mediaItem, effectId, startTimeMs, durationMs);
}

ECode Effect::Init(
    /* [in] */ IMediaItem* mediaItem,
    /* [in] */ const String& effectId,
    /* [in] */ Int64 startTimeMs,
    /* [in] */ Int64 durationMs)
{
    if (mediaItem == NULL) {
    //    throw new IllegalArgumentException("Media item cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if ((startTimeMs < 0) || (durationMs < 0)) {
    //     throw new IllegalArgumentException("Invalid start time Or/And Duration");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int64 duration;
    mediaItem->GetDuration(&duration);
    if (startTimeMs + durationMs > duration) {
    //    throw new IllegalArgumentException("Invalid start time and duration");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mMediaItem = mediaItem;
    return NOERROR;
}

ECode Effect::GetId(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mUniqueId;
    return NOERROR;
}

ECode Effect::SetDuration(
    /* [in] */ Int64 durationMs)
{
    if (durationMs < 0) {
    //    throw new IllegalArgumentException("Invalid duration");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int64 duration;
    mMediaItem->GetDuration(&duration);
    if (mStartTimeMs + durationMs > duration) {
    //    throw new IllegalArgumentException("Duration is too large");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IMediaItem> item;
    GetMediaItem((IMediaItem**)&item);
    AutoPtr<MediaArtistNativeHelper> helper;
    (reinterpret_cast<MediaItem*>(item->Probe(EIID_MediaItem)))->GetNativeContext((MediaArtistNativeHelper**)&helper);
    ((MediaArtistNativeHelper*)helper)->SetGeneratePreview(TRUE);

    Int64 oldDurationMs = mDurationMs;
    mDurationMs = durationMs;

    (reinterpret_cast<MediaItem*>(mMediaItem->Probe(EIID_MediaItem)))->InvalidateTransitionsEx(mStartTimeMs, oldDurationMs, mStartTimeMs, mDurationMs);

    return NOERROR;
}

ECode Effect::GetDuration(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mDurationMs;
    return NOERROR;
}

ECode Effect::SetStartTime(
    /* [in] */ Int64 startTimeMs)
{
    Int64 duration;
    mMediaItem->GetDuration(&duration);
    if (startTimeMs + mDurationMs > duration) {
    //    throw new IllegalArgumentException("Start time is too large");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IMediaItem> item;
    GetMediaItem((IMediaItem**)&item);
    AutoPtr<MediaArtistNativeHelper> helper;
    (reinterpret_cast<MediaItem*>(item->Probe(EIID_MediaItem)))->GetNativeContext((MediaArtistNativeHelper**)&helper);
    ((MediaArtistNativeHelper*)helper)->SetGeneratePreview(TRUE);

    Int64 oldStartTimeMs = mStartTimeMs;
    mStartTimeMs = startTimeMs;

    (reinterpret_cast<MediaItem*>(mMediaItem->Probe(EIID_MediaItem)))->InvalidateTransitionsEx(oldStartTimeMs, mDurationMs, mStartTimeMs, mDurationMs);
    return NOERROR;
}

ECode Effect::GetStartTime(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mStartTimeMs;
    return NOERROR;
}

ECode Effect::SetStartTimeAndDuration(
    /* [in] */ Int64 startTimeMs,
    /* [in] */ Int64 durationMs)
{
    Int64 duration;
    mMediaItem->GetDuration(&duration);
    if (startTimeMs + durationMs > duration) {
    //    throw new IllegalArgumentException("Invalid start time or duration");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IMediaItem> item;
    GetMediaItem((IMediaItem**)&item);
    AutoPtr<MediaArtistNativeHelper> helper;
    (reinterpret_cast<MediaItem*>(item->Probe(EIID_MediaItem)))->GetNativeContext((MediaArtistNativeHelper**)&helper);
    ((MediaArtistNativeHelper*)helper)->SetGeneratePreview(TRUE);

    Int64 oldStartTimeMs = mStartTimeMs;
    Int64 oldDurationMs = mDurationMs;

    mStartTimeMs = startTimeMs;
    mDurationMs = durationMs;

    (reinterpret_cast<MediaItem*>(mMediaItem->Probe(EIID_MediaItem)))->InvalidateTransitionsEx(oldStartTimeMs, oldDurationMs, mStartTimeMs, mDurationMs);

    return NOERROR;
}

ECode Effect::GetMediaItem(
    /* [out] */ IMediaItem** result)
{
    VALIDATE_NOT_NULL(result);

    *result = mMediaItem;
    return NOERROR;
}

ECode Effect::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (!IEffect::Probe(object)) {
        *result = FALSE;
        return NOERROR;
    }
    *result = mUniqueId.Equals(((Effect*)IEffect::Probe(object))->mUniqueId);
    return NOERROR;
}

ECode Effect::GetHashCode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mUniqueId.GetHashCode();
    return NOERROR;
}

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
