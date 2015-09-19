
#include "widget/CVideoViewEx.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CVideoViewEx, VideoView)
IDRAWABLECALLBACK_METHODS_IMPL(CVideoViewEx, VideoView)
IKEYEVENTCALLBACK_METHODS_IMPL(CVideoViewEx, VideoView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CVideoViewEx, VideoView)
ISURFACEVIEW_METHODS_IMPL(CVideoViewEx, VideoView)

ECode CVideoViewEx::constructor(
    /* [in] */ IContext* context)
{
    return VideoView::Init(context);
}

ECode CVideoViewEx::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return VideoView::Init(context, attrs);
}

ECode CVideoViewEx::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return VideoView::Init(context, attrs, defStyle);
}

PInterface CVideoViewEx::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CVideoViewEx::Probe(riid);
}

ECode CVideoViewEx::ResolveAdjustedSize(
    /* [in] */ Int32 desiredSize,
    /* [in] */ Int32 measureSpec,
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    *size = VideoView::ResolveAdjustedSize(desiredSize, measureSpec);
    return NOERROR;
}

ECode CVideoViewEx::SetVideoPath(
    /* [in] */ const String& path)
{
    return VideoView::SetVideoPath(path);
}

ECode CVideoViewEx::SetVideoURI(
    /* [in] */ IUri* uri)
{
    return VideoView::SetVideoURI(uri);
}

ECode CVideoViewEx::StopPlayback()
{
    return VideoView::StopPlayback();
}

ECode CVideoViewEx::SetMediaController(
    /* [in] */ IMediaController* controller)
{
    return VideoView::SetMediaController(controller);
}

ECode CVideoViewEx::SetOnPreparedListener(
    /* [in] */ IMediaPlayerOnPreparedListener* l)
{
    return VideoView::SetOnPreparedListener(l);
}

ECode CVideoViewEx::SetOnCompletionListener(
    /* [in] */ IMediaPlayerOnCompletionListener* l)
{
    return VideoView::SetOnCompletionListener(l);
}

ECode CVideoViewEx::SetOnErrorListener(
    /* [in] */ IMediaPlayerOnErrorListener* l)
{
    return VideoView::SetOnErrorListener(l);
}

ECode CVideoViewEx::SetOnInfoListener(
    /* [in] */ IMediaPlayerOnInfoListener* l)
{
    return VideoView::SetOnInfoListener(l);
}

ECode CVideoViewEx::Suspend()
{
    return VideoView::Suspend();
}

ECode CVideoViewEx::Resume()
{
    return VideoView::Resume();
}

// interface IMediaPlayerControl

ECode CVideoViewEx::Start()
{
    return VideoView::Start();
}

ECode CVideoViewEx::Pause()
{
    return VideoView::Pause();
}

ECode CVideoViewEx::GetDuration(
    /* [out] */ Int32* duration)
{
    VALIDATE_NOT_NULL(duration);
    *duration = VideoView::GetDuration();
    return NOERROR;
}

ECode CVideoViewEx::GetCurrentPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = VideoView::GetCurrentPosition();
    return NOERROR;
}

ECode CVideoViewEx::SeekTo(
    /* [in] */ Int32 position)
{
    return VideoView::SeekTo(position);
}

ECode CVideoViewEx::IsPlaying(
    /* [out] */ Boolean* isPlaying)
{
    VALIDATE_NOT_NULL(isPlaying);
    *isPlaying = VideoView::IsPlaying();
    return NOERROR;
}

ECode CVideoViewEx::GetBufferPercentage(
    /* [out] */ Int32* percentage)
{
    VALIDATE_NOT_NULL(percentage);
    *percentage = VideoView::GetBufferPercentage();
    return NOERROR;
}

ECode CVideoViewEx::CanPause(
    /* [out] */ Boolean* canPause)
{
    VALIDATE_NOT_NULL(canPause);
    *canPause = VideoView::CanPause();
    return NOERROR;
}

ECode CVideoViewEx::CanSeekBackward(
    /* [out] */ Boolean* canSeekBackward)
{
    VALIDATE_NOT_NULL(canSeekBackward);
    *canSeekBackward = VideoView::CanSeekBackward();
    return NOERROR;
}

ECode CVideoViewEx::CanSeekForward(
    /* [out] */ Boolean* canSeekForward)
{
    VALIDATE_NOT_NULL(canSeekForward);
    *canSeekForward = VideoView::CanSeekForward();
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
