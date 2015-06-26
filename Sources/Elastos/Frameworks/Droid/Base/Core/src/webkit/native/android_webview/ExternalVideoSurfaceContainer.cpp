
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//===============================================================
//    ExternalVideoSurfaceContainer::NoPunchingSurfaceView
//===============================================================

ExternalVideoSurfaceContainer::NoPunchingSurfaceView::NoPunchingSurfaceView(
    /* [in] */ IContext* context)
{
    super(context);
}
// SurfaceView.dispatchDraw implementation punches a hole in the view hierarchy.
// Disable this by making this a no-op.
//@Override
ECode ExternalVideoSurfaceContainer::NoPunchingSurfaceView::DispatchDraw(
    /* [in] */ ICanvas* canvas)
{
    retun NOERROR;
}

//===============================================================
//           ExternalVideoSurfaceContainer::Factory
//===============================================================

AutoPtr<ExternalVideoSurfaceContainer> ExternalVideoSurfaceContainer::Factory::Create(
    /* [in] */ Int64 nativeExternalVideoSurfaceContainer,
    /* [in] */ ContentViewCore* contentViewCore)
{
    AutoPtr<ExternalVideoSurfaceContainer> container = new ExternalVideoSurfaceContainer(
            nativeExternalVideoSurfaceContainer, contentViewCore);
    return container;
}

//===============================================================
//                ExternalVideoSurfaceContainer
//===============================================================

const Int32 ExternalVideoSurfaceContainer::INVALID_PLAYER_ID;
WeakReference<ExternalVideoSurfaceContainer> ExternalVideoSurfaceContainer::sActiveContainer;
AutoPtr<Factory> ExternalVideoSurfaceContainer::sFactory = new Factory();

ExternalVideoSurfaceContainer::ExternalVideoSurfaceContainer(
    /* [in] */ Int64 nativeExternalVideoSurfaceContainer,
    /* [in] */ ContentViewCore* contentViewCore)
    : mNativeExternalVideoSurfaceContainer(nativeExternalVideoSurfaceContainer)
    , mContentViewCore(contentViewCore)
    , mPlayerId(INVALID_PLAYER_ID)
    , mLeft(0.0f)
    , mTop(0.0f)
    , mRight(0.0f)
    , mBottom(0.0f)
    , mX(0)
    , mY(0)
    , mWidth(0)
    , mHeight(0)
{
    assert(contentViewCore != NULL);
    InitializeCurrentPositionOfSurfaceView();
}


//@VisibleForTesting
void ExternalVideoSurfaceContainer::SetFactory(
    /* [in] */ Factory* factory)
{
    sFactory = factory;
}

//@CalledByNative
AutoPtr<ExternalVideoSurfaceContainer> ExternalVideoSurfaceContainer::Create(
    /* [in] */ Int64 nativeExternalVideoSurfaceContainer,
    /* [in] */ ContentViewCore* contentViewCore)
{
    return sFactory::Create(nativeExternalVideoSurfaceContainer, contentViewCore);
}

/**
 * Called when a media player wants to request an external video surface.
 * @param playerId The ID of the media player.
 */
//@CalledByNative
void ExternalVideoSurfaceContainer::RequestExternalVideoSurface(
    /* [in] */ Int32 playerId)
{
    if (mPlayerId == playerId) return;

    if (mPlayerId == INVALID_PLAYER_ID) {
        SetActiveContainer(this);
    }

    mPlayerId = playerId;
    InitializeCurrentPositionOfSurfaceView();

    CreateSurfaceView();
}

/**
 * Called when a media player wants to release an external video surface.
 * @param playerId The ID of the media player.
 */
//@CalledByNative
void ExternalVideoSurfaceContainer::ReleaseExternalVideoSurface(
    /* [in] */ Int32 playerId)
{
    if (mPlayerId != playerId) return;

    ReleaseIfActiveContainer(this);

    mPlayerId = INVALID_PLAYER_ID;
}

//@CalledByNative
void ExternalVideoSurfaceContainer::Destroy()
{
    ReleaseExternalVideoSurface(mPlayerId);
}

void ExternalVideoSurfaceContainer::InitializeCurrentPositionOfSurfaceView()
{
    mX = Integer.MIN_VALUE;
    mY = Integer.MIN_VALUE;
    mWidth = 0;
    mHeight = 0;
}

void ExternalVideoSurfaceContainer::SetActiveContainer(
    /* [in] */ ExternalVideoSurfaceContainer container)
{
    AutoPtr<ExternalVideoSurfaceContainer> activeContainer = sActiveContainer.get();
    if (activeContainer != NULL) {
        activeContainer.removeSurfaceView();
    }
    sActiveContainer = new WeakReference<ExternalVideoSurfaceContainer>(container);
}

void ExternalVideoSurfaceContainer::ReleaseIfActiveContainer(
    /* [in] */ ExternalVideoSurfaceContainer* container)
{
    AutoPtr<ExternalVideoSurfaceContainer> activeContainer = sActiveContainer.get();
    if (activeContainer == container) {
        SetActiveContainer(NULL);
    }
}

void ExternalVideoSurfaceContainer::CreateSurfaceView()
{
    mSurfaceView = new NoPunchingSurfaceView(mContentViewCore->GetContext());
    AutoPtr<ISurfaceHolder> holder;
    mSurfaceView->GetHolder((ISurfaceHolder**)&holder);
    holder->AddCallback(this);
    // SurfaceHoder.surfaceCreated() will be called after the SurfaceView is attached to
    // the Window and becomes visible.
    mContentViewCore->GetContainerView()->AddView(mSurfaceView);
}

void ExternalVideoSurfaceContainer::RemoveSurfaceView()
{
    // SurfaceHoder.surfaceDestroyed() will be called in ViewGroup.removeView()
    // as soon as the SurfaceView is detached from the Window.
    mContentViewCore->GetContainerView()->RemoveView(mSurfaceView);
    mSurfaceView = NULL;
}

/**
 * Called when the position of the video element which uses the external
 * video surface is changed.
 * @param playerId The ID of the media player.
 * @param left The absolute CSS X coordinate of the left side of the video element.
 * @param top The absolute CSS Y coordinate of the top side of the video element.
 * @param right The absolute CSS X coordinate of the right side of the video element.
 * @param bottom The absolute CSS Y coordinate of the bottom side of the video element.
 */
//@CalledByNative
void ExternalVideoSurfaceContainer::OnExternalVideoSurfacePositionChanged(
    /* [in] */ Int32 playerId,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom)
{
    if (mPlayerId != playerId) return;

    mLeft = left;
    mTop = top;
    mRight = right;
    mBottom = bottom;

    LayOutSurfaceView();
}

/**
 * Called when the page that contains the video element is scrolled or zoomed.
 */
//@CalledByNative
void ExternalVideoSurfaceContainer::OnFrameInfoUpdated()
{
    if (mPlayerId == INVALID_PLAYER_ID) return;

    LayOutSurfaceView();
}

void ExternalVideoSurfaceContainer::LayOutSurfaceView()
{
    AutoPtr<RenderCoordinates> renderCoordinates = mContentViewCore->GetRenderCoordinates();
    RenderCoordinates::NormalizedPoint topLeft = renderCoordinates->CreateNormalizedPoint();
    RenderCoordinates::NormalizedPoint bottomRight = renderCoordinates->CreateNormalizedPoint();
    topLeft->SetAbsoluteCss(mLeft, mTop);
    bottomRight->SetAbsoluteCss(mRight, mBottom);
    Float top = topLeft->GetYPix();
    Float left = topLeft->GetXPix();
    Float bottom = bottomRight->GetYPix();
    Float right = bottomRight->GetXPix();

    Int32 x = Math::Round(left + renderCoordinates->GetScrollXPix());
    Int32 y = Math::Round(top + renderCoordinates->GetScrollYPix());
    Int32 width = Math::Round(right - left);
    Int32 height = Math::Round(bottom - top);
    if (mX == x && mY == y && mWidth == width && mHeight == height) return;
    mX = x;
    mY = y;
    mWidth = width;
    mHeight = height;

    mSurfaceView->SetX(x);
    mSurfaceView->SetY(y);
    AutoPtr<IViewGroupLayoutParams> layoutParams;
    mSurfaceView->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);
    layoutParams->SetWidth(width);
    layoutParams->SetHeight(height);
    mSurfaceView->RequestLayout();
}

// SurfaceHolder.Callback methods.
//@Override
ECode ExternalVideoSurfaceContainer::SurfaceChanged(
    /* [in] */ ISurfaceHolder* holder,
    /* [in] */ Int32 format,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return NOERROR;
}

//@Override
// surfaceCreated() callback can be called regardless of requestExternalVideoSurface,
// if the activity comes back from the background and becomes visible.
ECode ExternalVideoSurfaceContainer::SurfaceCreated(
    /* [in] */ ISurfaceHolder* holder)
{
    if (mPlayerId != INVALID_PLAYER_ID) {
        AutoPtr<ISurface> surface;
        holder->GetSurface((ISurface**)&surface);
        NativeSurfaceCreated(
                mNativeExternalVideoSurfaceContainer, mPlayerId, surface);
    }

    return NOERROR;
}

// surfaceDestroyed() callback can be called regardless of releaseExternalVideoSurface,
// if the activity moves to the backgound and becomes invisible.
//@Override
ECode ExternalVideoSurfaceContainer::SurfaceDestroyed(
    /* [in] */ ISurfaceHolder* holder)
{
    if (mPlayerId != INVALID_PLAYER_ID) {
        NativeSurfaceDestroyed(mNativeExternalVideoSurfaceContainer, mPlayerId);
    }

    return NOERROR;
}

void ExternalVideoSurfaceContainer::NativeSurfaceCreated(
    /* [in] */ Int64 nativeExternalVideoSurfaceContainerImpl,
    /* [in] */ Int32 playerId,
    /* [in] */ ISurface* surface)
{
}

void ExternalVideoSurfaceContainer::NativeSurfaceDestroyed(
    /* [in] */ Int64 nativeExternalVideoSurfaceContainerImpl,
    /* [in] */ Int32 playerId)
{
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos