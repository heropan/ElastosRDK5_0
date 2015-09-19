
#include "graphics/CSurfaceTexture.h"
#include "webkit/HTML5VideoInline.h"
#include "webkit/HTML5VideoViewProxy.h"
#include "view/CSurface.h"
#include "Manifest.h"
#include "opengl/CGLES20.h"

#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Graphics::CSurfaceTexture;
using Elastos::Droid::Manifest;
using Elastos::Droid::Opengl::CGLES20;
using Elastos::Droid::Opengl::IGLES20;
using Elastos::Droid::Os::IPowerManager;
using Elastos::Droid::View::CSurface;
using Elastos::Droid::View::ISurface;

namespace Elastos {
namespace Droid {
namespace Webkit {

AutoPtr<ISurfaceTexture> HTML5VideoInline::mSurfaceTexture;
AutoPtr< ArrayOf<Int32> > HTML5VideoInline::mTextureNames;
Int32 HTML5VideoInline::mVideoLayerUsingSurfaceTexture;

HTML5VideoInline::HTML5VideoInline(
    /* [in] */ Int32 videoLayerId,
    /* [in] */ Int32 position,
    /* [in] */ Boolean skipPrepare)
{
    Init(videoLayerId, position, skipPrepare);
}

// Video control FUNCTIONS:
ECode HTML5VideoInline::Start()
{
    if (!GetPauseDuringPreparing()) {
        HTML5VideoView::Start();
    }
    return NOERROR;
}

void HTML5VideoInline::DecideDisplayMode()
{
    AutoPtr<ISurfaceTexture> surfaceTexture = GetSurfaceTexture(GetVideoLayerId());
    AutoPtr<ISurface> surface;
    CSurface::New(surfaceTexture, (ISurface**)&surface);
    mPlayer->SetSurface(surface);
    surface->ReleaseSurface();
}

// Normally called immediately after setVideoURI. But for full screen,
// this should be after surface holder created
void HTML5VideoInline::PrepareDataAndDisplayMode(
    /* [in] */ HTML5VideoViewProxy* proxy)
{
    HTML5VideoView::PrepareDataAndDisplayMode(proxy);
    SetFrameAvailableListener(proxy);
    // TODO: This is a workaround, after b/5375681 fixed, we should switch
    // to the better way.
    Int32 pm;
    mProxy->GetContext()->CheckCallingOrSelfPermission(Manifest::Permission::WAKE_LOCK, &pm);
    if (pm == IPackageManager::PERMISSION_GRANTED) {
        mPlayer->SetWakeMode(proxy->GetContext(), IPowerManager::FULL_WAKE_LOCK);
    }
}

// Pause the play and update the play/pause button
void HTML5VideoInline::PauseAndDispatch(
    /* [in] */ HTML5VideoViewProxy* proxy)
{
    HTML5VideoView::PauseAndDispatch(proxy);
}

// Inline Video specific FUNCTIONS:

AutoPtr<ISurfaceTexture> HTML5VideoInline::GetSurfaceTexture(
    /* [in] */ Int32 videoLayerId)
{
    // Create the surface texture.
    if (videoLayerId != mVideoLayerUsingSurfaceTexture
        || mSurfaceTexture == NULL
        || mTextureNames == NULL) {
        // The GL texture will store in the VideoLayerManager at native side.
        // They will be clean up when requested.
        // The reason we recreated GL texture name is for screen shot support.
        mTextureNames = ArrayOf<Int32>::Alloc(1);
        AutoPtr<IGLES20> GLES20;
        CGLES20::AcquireSingleton((IGLES20**)&GLES20);
        GLES20->GlGenTextures(1, mTextureNames, 0);
        mSurfaceTexture = NULL;
        CSurfaceTexture::New((*mTextureNames)[0], (ISurfaceTexture**)&mSurfaceTexture);
    }
    mVideoLayerUsingSurfaceTexture = videoLayerId;
    return mSurfaceTexture;
}

Boolean HTML5VideoInline::SurfaceTextureDeleted()
{
    return (mSurfaceTexture == NULL);
}

void HTML5VideoInline::DeleteSurfaceTexture()
{
    CleanupSurfaceTexture();
    return;
}

void HTML5VideoInline::CleanupSurfaceTexture()
{
    mSurfaceTexture = NULL;
    mVideoLayerUsingSurfaceTexture = -1;
    return;
}

Int32 HTML5VideoInline::GetTextureName()
{
    if (mTextureNames != NULL) {
        return (*mTextureNames)[0];
    }
    else {
        return 0;
    }
}

void HTML5VideoInline::SetFrameAvailableListener(
    /* [in] */ IOnFrameAvailableListener* l)
{
    if (mSurfaceTexture != NULL) {
        mSurfaceTexture->SetOnFrameAvailableListener(l);
    }
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
