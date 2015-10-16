#ifndef __ELASTOS_DROID_WIDGET_CVIDEOVIEWEX_H__
#define __ELASTOS_DROID_WIDGET_CVIDEOVIEWEX_H__

#include "_Elastos_Droid_Widget_CVideoViewEx.h"


#include "elastos/droid/widget/VideoView.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CVideoViewEx), public VideoView
{
public:
    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    ISURFACEVIEW_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI ResolveAdjustedSize(
        /* [in] */ Int32 desiredSize,
        /* [in] */ Int32 measureSpec,
        /* [out] */ Int32* size);

    CARAPI SetVideoPath(
        /* [in] */ const String& path);

    CARAPI SetVideoURI(
        /* [in] */ IUri* uri);

    CARAPI StopPlayback();

    CARAPI SetMediaController(
        /* [in] */ IMediaController* controller);

    /**
     * Register a callback to be invoked when the media file
     * is loaded and ready to go.
     *
     * @param l The callback that will be run
     */
    CARAPI SetOnPreparedListener(
        /* [in] */ IMediaPlayerOnPreparedListener* l);

     /**
     * Register a callback to be invoked when the end of a media file
     * has been reached during playback.
     *
     * @param l The callback that will be run
     */
    CARAPI SetOnCompletionListener(
        /* [in] */ IMediaPlayerOnCompletionListener* l);

    /**
     * Register a callback to be invoked when an error occurs
     * during playback or setup.  If no listener is specified,
     * or if the listener returned false, VideoView will inform
     * the user of any errors.
     *
     * @param l The callback that will be run
     */
    CARAPI SetOnErrorListener(
        /* [in] */ IMediaPlayerOnErrorListener* l);

    /**
     * Register a callback to be invoked when an informational event
     * occurs during playback or setup.
     *
     * @param l The callback that will be run
     */
    CARAPI SetOnInfoListener(
        /* [in] */ IMediaPlayerOnInfoListener* l);

    CARAPI Suspend();

    CARAPI Resume();

    // interface IMediaPlayerControl

    CARAPI Start();

    CARAPI Pause();

    CARAPI GetDuration(
        /* [out] */ Int32* duration);

    CARAPI GetCurrentPosition(
        /* [out] */ Int32* position);

    CARAPI SeekTo(
        /* [in] */ Int32 position);

    CARAPI IsPlaying(
        /* [out] */ Boolean* isPlaying);

    CARAPI GetBufferPercentage(
        /* [out] */ Int32* percentage);

    CARAPI CanPause(
        /* [out] */ Boolean* canPause);

    CARAPI CanSeekBackward(
        /* [out] */ Boolean* canSeekBackward);

    CARAPI CanSeekForward(
        /* [out] */ Boolean* canSeekForward);

};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CVIDEOVIEWEX_H__
