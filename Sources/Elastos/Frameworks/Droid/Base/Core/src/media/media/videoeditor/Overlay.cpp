#include "Overlay.h"
#include "MediaArtistNativeHelper.h"
#include "MediaItem.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Utility::CObjectStringMap;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

//838ad086-b887-4abb-8aab-a25b79a9611a
extern "C" const InterfaceID EIID_Overlay =
        { 0x838ad086, 0xb887, 0x4abb, { 0x8a, 0xab, 0xa2, 0x5b, 0x79, 0xa9, 0x61, 0x1a } };

Overlay::Overlay()
    : mUniqueId(NULL)
    , mStartTimeMs(0)
    , mDurationMs(0)
{
    Init(NULL, String(NULL), 0, 0);
}

Overlay::Overlay(
    /* [in] */ IMediaItem* mediaItem,
    /* [in] */ const String& overlayId,
    /* [in] */ Int64 startTimeMs,
    /* [in] */ Int64 durationMs)
    : mUniqueId(NULL)
    , mStartTimeMs(0)
    , mDurationMs(0)
{
    Init(mediaItem, overlayId, startTimeMs, durationMs);
}

ECode Overlay::Init(
    /* [in] */ IMediaItem* mediaItem,
    /* [in] */ const String& overlayId,
    /* [in] */ Int64 startTimeMs,
    /* [in] */ Int64 durationMs)
{
    if (mediaItem == NULL) {
        //throw new IllegalArgumentException("Media item cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if ((startTimeMs < 0) || (durationMs < 0) ) {
        //throw new IllegalArgumentException("Invalid start time and/OR duration");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int64 duration;
    mediaItem->GetDuration(&duration);
    if (startTimeMs + durationMs > duration) {
        //throw new IllegalArgumentException("Invalid start time and duration");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mMediaItem = mediaItem;
    mUniqueId = overlayId;
    mStartTimeMs = startTimeMs;
    mDurationMs = durationMs;

    CObjectStringMap::New((IObjectStringMap**)&mUserAttributes);
    return NOERROR;
}

ECode Overlay::GetId(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mUniqueId;
    return NOERROR;
}

ECode Overlay::GetDuration(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mDurationMs;
    return NOERROR;
}

ECode Overlay::SetDuration(
    /* [in] */ Int64 durationMs)
{
    if (durationMs < 0) {
        // throw new IllegalArgumentException("Invalid duration");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int64 duration;
    mMediaItem->GetDuration(&duration);
    if (mStartTimeMs + durationMs > duration) {
        // throw new IllegalArgumentException("Duration is too large");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IMediaItem> mediaItem;
    GetMediaItem((IMediaItem**)&mediaItem);
    AutoPtr<MediaArtistNativeHelper> helper;
    ((MediaItem*)mediaItem.Get())->GetNativeContext((MediaArtistNativeHelper**)&helper);
    helper->SetGeneratePreview(TRUE);

    Int64 oldDurationMs = mDurationMs;
    mDurationMs = durationMs;

    ((MediaItem*)mMediaItem.Get())->InvalidateTransitionsEx(mStartTimeMs, oldDurationMs, mStartTimeMs, mDurationMs);

    return NOERROR;
}

ECode Overlay::GetStartTime(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mStartTimeMs;
    return NOERROR;
}

ECode Overlay::SetStartTime(
    /* [in] */ Int64 startTimeMs)
{
    Int64 duration;
    mMediaItem->GetDuration(&duration);
    if (startTimeMs + mDurationMs > duration) {
        // throw new IllegalArgumentException("Start time is too large");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IMediaItem> mediaItem;
    GetMediaItem((IMediaItem**)&mediaItem);
    AutoPtr<MediaArtistNativeHelper> helper;
    ((MediaItem*)mediaItem.Get())->GetNativeContext((MediaArtistNativeHelper**)&helper);
    helper->SetGeneratePreview(TRUE);

    Int64 oldStartTimeMs = mStartTimeMs;
    mStartTimeMs = startTimeMs;

    ((MediaItem*)mMediaItem.Get())->InvalidateTransitionsEx(oldStartTimeMs, mDurationMs, mStartTimeMs, mDurationMs);

    return NOERROR;
}

ECode Overlay::SetStartTimeAndDuration(
    /* [in] */ Int64 startTimeMs,
    /* [in] */ Int64 durationMs)
{
    Int64 duration;
    mMediaItem->GetDuration(&duration);
    if (startTimeMs + durationMs > duration) {
        // throw new IllegalArgumentException("Invalid start time or duration");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IMediaItem> mediaItem;
    GetMediaItem((IMediaItem**)&mediaItem);
    AutoPtr<MediaArtistNativeHelper> helper;
    ((MediaItem*)mediaItem.Get())->GetNativeContext((MediaArtistNativeHelper**)&helper);
    helper->SetGeneratePreview(TRUE);

    Int64 oldStartTimeMs = mStartTimeMs;
    Int64 oldDurationMs = mDurationMs;

    mStartTimeMs = startTimeMs;
    mDurationMs = durationMs;

    ((MediaItem*)mMediaItem.Get())->InvalidateTransitionsEx(oldStartTimeMs, oldDurationMs, mStartTimeMs, mDurationMs);

    return NOERROR;
}

ECode Overlay::GetMediaItem(
    /* [out] */ IMediaItem** result)
{
    VALIDATE_NOT_NULL(result);

    *result = mMediaItem;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode Overlay::SetUserAttribute(
    /* [in] */ const String& name,
    /* [in] */ const String& value)
{
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(value, (ICharSequence**)&csq);
    return mUserAttributes->Put(name, (IInterface*)csq);
}

ECode Overlay::GetUserAttributes(
    /* [out] */ IObjectStringMap** result)
{
    VALIDATE_NOT_NULL(result);

    *result = mUserAttributes;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode Overlay::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (!IOverlay::Probe(object)) {
        *result = FALSE;
        return NOERROR;
    }
    *result = mUniqueId.Equals(((Overlay*)IOverlay::Probe(object))->mUniqueId);
    return NOERROR;
}

ECode Overlay::GetHashCode(
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
