
#ifndef __ELASTOS_DROID_WEBKIT_HTML5VIDEOINLINE_H__
#define __ELASTOS_DROID_WEBKIT_HTML5VIDEOINLINE_H__

#include "elastos/droid/ext/frameworkext.h"

#include "elastos/droid/webkit/HTML5VideoView.h"

using Elastos::Droid::Graphics::ISurfaceTexture;
using Elastos::Droid::Graphics::IOnFrameAvailableListener;

namespace Elastos {
namespace Droid {
namespace Webkit {

class HTML5VideoViewProxy;

/**
 * @hide This is only used by the browser
 */
class HTML5VideoInline : public HTML5VideoView
{
public:
    HTML5VideoInline(
        /* [in] */ Int32 videoLayerId,
        /* [in] */ Int32 position,
        /* [in] */ Boolean skipPrepare);

    // Video control FUNCTIONS:
    CARAPI Start();

    CARAPI_(void) DecideDisplayMode();

    // Normally called immediately after setVideoURI. But for full screen,
    // this should be after surface holder created
    CARAPI_(void) PrepareDataAndDisplayMode(
        /* [in] */ HTML5VideoViewProxy* proxy);

    // Pause the play and update the play/pause button
    CARAPI_(void) PauseAndDispatch(
        /* [in] */ HTML5VideoViewProxy* proxy);

    // Inline Video specific FUNCTIONS:

    static CARAPI_(AutoPtr<ISurfaceTexture>) GetSurfaceTexture(
        /* [in] */ Int32 videoLayerId);

    static CARAPI_(Boolean) SurfaceTextureDeleted();

    CARAPI_(void) DeleteSurfaceTexture();

    static CARAPI_(void) CleanupSurfaceTexture();

    CARAPI_(Int32) GetTextureName();

    CARAPI ToString(
        /* [out] */ String* info);

private:
    CARAPI_(void) SetFrameAvailableListener(
        /* [in] */ IOnFrameAvailableListener* l);

private:

    // Due to the fact that the decoder consume a lot of memory, we make the
    // surface texture as singleton. But the GL texture (m_textureNames)
    // associated with the surface texture can be used for showing the screen
    // shot when paused, so they are not singleton.
    static AutoPtr<ISurfaceTexture> mSurfaceTexture;
    static AutoPtr< ArrayOf<Int32> > mTextureNames;

    // Every time when the VideoLayer Id change, we need to recreate the
    // SurfaceTexture in order to delete the old video's decoder memory.
    static Int32 mVideoLayerUsingSurfaceTexture;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_HTML5VIDEOINLINE_H__
