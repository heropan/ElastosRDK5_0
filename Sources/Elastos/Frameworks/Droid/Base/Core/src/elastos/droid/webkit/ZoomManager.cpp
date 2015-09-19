
#include "os/SystemClock.h"
#include "utility/FloatMath.h"
#include "view/CScaleGestureDetector.h"
#include "webkit/ZoomManager.h"
#include "webkit/CallbackProxy.h"
#include "webkit/CWebViewCore.h"
#include "webkit/CWebViewClassic.h"
#include "webkit/WebSettingsClassic.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Core::EIID_IRunnable;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Utility::FloatMath;
using Elastos::Droid::View::CScaleGestureDetector;
using Elastos::Droid::View::EIID_IOnScaleGestureListener;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//                    ZoomManager::FocusMovementQueue
//===============================================================
const Int32 ZoomManager::FocusMovementQueue::QUEUE_CAPACITY;

ZoomManager::FocusMovementQueue::FocusMovementQueue(
    /* [in] */ ZoomManager* owner)
    : mSum(0.0f)
    , mSize(0)
    , mIndex(0)
    , mOwner(owner)
{
    mQueue = ArrayOf<Float>::Alloc(QUEUE_CAPACITY);
}

void ZoomManager::FocusMovementQueue::Clear()
{
    mSize = 0;
    mSum = 0;
    mIndex = 0;
    for (Int32 i = 0; i < QUEUE_CAPACITY; ++i) {
        (*mQueue)[i] = 0;
    }
}

void ZoomManager::FocusMovementQueue::Add(
    /* [in] */ Float focusDelta)
{
    mSum += focusDelta;
    if (mSize < QUEUE_CAPACITY) {  // fill up the queue.
        mSize++;
    }
    else {  // circulate the queue.
        mSum -= (*mQueue)[mIndex];
    }
    (*mQueue)[mIndex] = focusDelta;
    mIndex = (mIndex + 1) % QUEUE_CAPACITY;
}

Float ZoomManager::FocusMovementQueue::GetSum()
{
    return mSum;
}

//===============================================================
//                 ZoomManager::ScaleDetectorListener
//===============================================================
ZoomManager::ScaleDetectorListener::ScaleDetectorListener(
    /* [in] */ ZoomManager* owner)
    : mAccumulatedSpan(0.0f)
    , mOwner(owner)
{}

CAR_INTERFACE_IMPL_LIGHT(ZoomManager::ScaleDetectorListener, IOnScaleGestureListener);

ECode ZoomManager::ScaleDetectorListener::OnScaleBegin(
    /* [in] */ IScaleGestureDetector* detector,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    mOwner->mInitialZoomOverview = FALSE;
    mOwner->DismissZoomPicker();
    mOwner->mFocusMovementQueue->Clear();
    detector->GetFocusX(&mOwner->mFocusX);
    detector->GetFocusY(&mOwner->mFocusY);
    mOwner->mWebView->mViewManager->StartZoom();
    mOwner->mWebView->OnPinchToZoomAnimationStart();
    mAccumulatedSpan = 0;
    *res = TRUE;
    return NOERROR;
}

// If the user moves the fingers but keeps the same distance between them,
// we should do panning only.
Boolean ZoomManager::ScaleDetectorListener::IsPanningOnly(
    /* [in] */ IScaleGestureDetector* detector)
{
    Float prevFocusX = mOwner->mFocusX;
    Float prevFocusY = mOwner->mFocusY;
    detector->GetFocusX(&mOwner->mFocusX);
    detector->GetFocusY(&mOwner->mFocusY);
    Float focusDelta = (prevFocusX == 0 && prevFocusY == 0) ? 0 :
            FloatMath::Sqrt((mOwner->mFocusX - prevFocusX) * (mOwner->mFocusX - prevFocusX)
                           + (mOwner->mFocusY - prevFocusY) * (mOwner->mFocusY - prevFocusY));
    mOwner->mFocusMovementQueue->Add(focusDelta);
    Float s1, s2;
    detector->GetCurrentSpan(&s1);
    detector->GetPreviousSpan(&s2);
    Float deltaSpan = s1 - s2 + mAccumulatedSpan;
    Boolean result = mOwner->mFocusMovementQueue->GetSum() > Elastos::Core::Math::Abs(deltaSpan);
    if (result) {
        mAccumulatedSpan += deltaSpan;
    }
    else {
        mAccumulatedSpan = 0;
    }
    return result;
}

Boolean ZoomManager::ScaleDetectorListener::HandleScale(
    /* [in] */ IScaleGestureDetector* detector)
{
    Float factor;
    detector->GetScaleFactor(&factor);
    Float scale = factor * mOwner->mActualScale;

    // if scale is limited by any reason, don't zoom but do ask
    // the detector to update the event.
    Boolean isScaleLimited =
            mOwner->IsScaleOverLimits(scale) || scale < mOwner->GetZoomOverviewScale();

    // Prevent scaling beyond overview scale.
    scale = Elastos::Core::Math::Max(mOwner->ComputeScaleWithLimits(scale), mOwner->GetZoomOverviewScale());

    if (mOwner->mPinchToZoomAnimating || mOwner->WillScaleTriggerZoom(scale)) {
        mOwner->mPinchToZoomAnimating = TRUE;
        // limit the scale change per step
        if (scale > mOwner->mActualScale) {
            scale = Elastos::Core::Math::Min(scale, mOwner->mActualScale * 1.25f);
        }
        else {
            scale = Elastos::Core::Math::Max(scale, mOwner->mActualScale * 0.8f);
        }
        scale = mOwner->ComputeScaleWithLimits(scale);
        // if the scale change is too small, regard it as jitter and skip it.
        if (Elastos::Core::Math::Abs(scale - mOwner->mActualScale) < MINIMUM_SCALE_WITHOUT_JITTER) {
            return isScaleLimited;
        }
        Float x, y;
        detector->GetFocusX(&x);
        detector->GetFocusY(&y);
        mOwner->SetZoomCenter(x, y);
        mOwner->SetZoomScale(scale, FALSE);
        mOwner->mWebView->Invalidate();
        return TRUE;
    }
    return isScaleLimited;
}

ECode ZoomManager::ScaleDetectorListener::OnScale(
    /* [in] */ IScaleGestureDetector* detector,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    if (IsPanningOnly(detector) || HandleScale(detector)) {
        mOwner->mFocusMovementQueue->Clear();
        *res = TRUE;
        return NOERROR;
    }
    *res = FALSE;
    return NOERROR;
}

ECode ZoomManager::ScaleDetectorListener::OnScaleEnd(
    /* [in] */ IScaleGestureDetector* detector)
{
    if (mOwner->mPinchToZoomAnimating) {
        mOwner->mPinchToZoomAnimating = FALSE;
        mOwner->mAnchorX = mOwner->mWebView->ViewToContentX((Int32) mOwner->mZoomCenterX + mOwner->mWebView->GetScrollX());
        mOwner->mAnchorY = mOwner->mWebView->ViewToContentY((Int32) mOwner->mZoomCenterY + mOwner->mWebView->GetScrollY());
        // don't reflow when zoom in; when zoom out, do reflow if the
        // new scale is almost minimum scale.
        Boolean reflowNow = !mOwner->CanZoomOut() || (mOwner->mActualScale <= 0.8 * mOwner->mTextWrapScale);
        // force zoom after mPreviewZoomOnly is set to false so that the
        // new view size will be passed to the WebKit
        AutoPtr<IWebSettings> settings;
        mOwner->mWebView->GetSettings((IWebSettings**)&settings);
        mOwner->RefreshZoomScale(reflowNow &&
            !((WebSettingsClassic*)(IWebSettingsClassic*)settings.Get())->GetUseFixedViewport());
        // call invalidate() to draw without zoom filter
        mOwner->mWebView->Invalidate();
    }

    mOwner->mWebView->mViewManager->EndZoom();
    mOwner->mWebView->OnPinchToZoomAnimationEnd(detector);
}


//===============================================================
//                     ZoomManager::PostScale
//===============================================================
ZoomManager::PostScale::PostScale(
    /* [in] */ Boolean updateTextWrap,
    /* [in] */ Boolean inZoomOverview,
    /* [in] */ Boolean inPortraitMode,
    /* [in] */ ZoomManager* owner)
    : mUpdateTextWrap(updateTextWrap)
    , mInZoomOverviewBeforeSizeChange(inZoomOverview)
    , mInPortraitMode(inPortraitMode)
    , mOwner(owner)
{}

ECode ZoomManager::PostScale::Run()
{
    AutoPtr<IWebViewCore> webCore;
    mOwner->mWebView->GetWebViewCore((IWebViewCore**)&webCore);
    if (webCore != NULL) {
        // we always force, in case our height changed, in which case we
        // still want to send the notification over to webkit.
        // Keep overview mode unchanged when rotating.
        Float newScale = mOwner->mActualScale;
        AutoPtr<IWebSettings> settings;
        mOwner->mWebView->GetSettings((IWebSettings**)&settings);
        Boolean result = FALSE;
        if ((settings->GetUseWideViewPort(&result), result) &&
            mInPortraitMode &&
            mInZoomOverviewBeforeSizeChange) {
            newScale = mOwner->GetZoomOverviewScale();
        }
        mOwner->SetZoomScale(newScale, mUpdateTextWrap, TRUE);
        // update the zoom buttons as the scale can be changed
        mOwner->UpdateZoomPicker();
    }
    return NOERROR;
}


//===============================================================
//                          ZoomManager
//===============================================================
const String ZoomManager::LOGTAG("webviewZoom");

const Float ZoomManager::DEFAULT_MAX_ZOOM_SCALE_FACTOR;
const Float ZoomManager::DEFAULT_MIN_ZOOM_SCALE_FACTOR;

Float ZoomManager::MIN_DOUBLE_TAP_SCALE_INCREMENT = 0.5f;
Float ZoomManager::MINIMUM_SCALE_INCREMENT = 0.007f;
Float ZoomManager::MINIMUM_SCALE_WITHOUT_JITTER = 0.007f;
const Int32 ZoomManager::ZOOM_ANIMATION_LENGTH;

ZoomManager::ZoomManager(
    /* [in] */ CWebViewClassic* webView,
    /* [in] */ CallbackProxy* callbackProxy)
    : mWebView(webView)
    , mCallbackProxy(callbackProxy)
    , mDefaultMaxZoomScale(0.0f)
    , mDefaultMinZoomScale(0.0f)
    , mMaxZoomScale(0.0f)
    , mMinZoomScale(0.0f)
    , mMinZoomScaleFixed(TRUE)
    , mInitialZoomOverview(FALSE)
    , mInZoomOverview(FALSE)
    , mZoomOverviewWidth(0)
    , mInvZoomOverviewWidth(0.0f)
    , mZoomCenterX(0.0f)
    , mZoomCenterY(0.0f)
    , mFocusX(0.0f)
    , mFocusY(0.0f)
    , mAnchorX(0)
    , mAnchorY(0)
    , mTextWrapScale(0.0f)
    , mDefaultScale(0.0f)
    , mInvDefaultScale(0.0f)
    , mDisplayDensity(0.0f)
    , mDoubleTapZoomFactor(1.0f)
    , mActualScale(0.0f)
    , mInvActualScale(0.0f)
    , mInitialScale(0.0f)
    , mZoomScale(0.0f)
    , mInvInitialZoomScale(0.0f)
    , mInvFinalZoomScale(0.0f)
    , mInitialScrollX(0)
    , mInitialScrollY(0)
    , mZoomStart(0)
    , mSupportMultiTouch(FALSE)
    , mAllowPanAndScale(FALSE)
    , mPinchToZoomAnimating(FALSE)
    , mHardwareAccelerated(FALSE)
    , mInHWAcceleratedZoom(FALSE)
{
    /*
     * Ideally mZoomOverviewWidth should be mContentWidth. But sites like
     * ESPN and Engadget always have wider mContentWidth no matter what the
     * viewport size is.
     */
    SetZoomOverviewWidth(CWebViewClassic::DEFAULT_VIEWPORT_WIDTH);

    mFocusMovementQueue = new FocusMovementQueue(this);
}

/**
 * Initialize both the default and actual zoom scale to the given density.
 *
 * @param density The logical density of the display. This is a scaling factor
 * for the Density Independent Pixel unit, where one DIP is one pixel on an
 * approximately 160 dpi screen (see android.util.DisplayMetrics.density).
 */
void ZoomManager::Init(
    /* [in] */ Float density)
{
    assert(density > 0);

    mDisplayDensity = density;
    SetDefaultZoomScale(density);
    mActualScale = density;
    mInvActualScale = 1 / density;
    mTextWrapScale = GetReadingLevelScale();
}

/**
 * Update the default zoom scale using the given density. It will also reset
 * the current min and max zoom scales to the default boundaries as well as
 * ensure that the actual scale falls within those boundaries.
 *
 * @param density The logical density of the display. This is a scaling factor
 * for the Density Independent Pixel unit, where one DIP is one pixel on an
 * approximately 160 dpi screen (see android.util.DisplayMetrics.density).
 */
void ZoomManager::UpdateDefaultZoomDensity(
    /* [in] */ Float density)
{
    assert(density > 0);

    if (Elastos::Core::Math::Abs(density - mDefaultScale) > MINIMUM_SCALE_INCREMENT) {
        // Remember the current zoom density before it gets changed.
        Float originalDefault = mDefaultScale;
        // set the new default density
        mDisplayDensity = density;
        SetDefaultZoomScale(density);
        Float scaleChange = (originalDefault > 0.0f) ? density / originalDefault : 1.0f;
        // adjust the scale if it falls outside the new zoom bounds
        SetZoomScale(mActualScale * scaleChange, TRUE);
    }
}

void ZoomManager::SetDefaultZoomScale(
    /* [in] */ Float defaultScale)
{
    Float originalDefault = mDefaultScale;
    mDefaultScale = defaultScale;
    mInvDefaultScale = 1 / defaultScale;
    mDefaultMaxZoomScale = defaultScale * DEFAULT_MAX_ZOOM_SCALE_FACTOR;
    mDefaultMinZoomScale = defaultScale * DEFAULT_MIN_ZOOM_SCALE_FACTOR;
    if (originalDefault > 0.0f && mMaxZoomScale > 0.0f) {
        // Keeps max zoom scale when zoom density changes.
        mMaxZoomScale = defaultScale / originalDefault * mMaxZoomScale;
    }
    else {
        mMaxZoomScale = mDefaultMaxZoomScale;
    }
    if (originalDefault > 0.0f && mMinZoomScale > 0.0f) {
        // Keeps min zoom scale when zoom density changes.
        mMinZoomScale = defaultScale / originalDefault * mMinZoomScale;
    }
    else {
        mMinZoomScale = mDefaultMinZoomScale;
    }
    if (!ExceedsMinScaleIncrement(mMinZoomScale, mMaxZoomScale)) {
        mMaxZoomScale = mMinZoomScale;
    }
    SanitizeMinMaxScales();
}

Float ZoomManager::GetScale()
{
    return mActualScale;
}

Float ZoomManager::GetInvScale()
{
    return mInvActualScale;
}

Float ZoomManager::GetTextWrapScale()
{
    return mTextWrapScale;
}

Float ZoomManager::GetMaxZoomScale()
{
    return mMaxZoomScale;
}

Float ZoomManager::GetMinZoomScale()
{
    return mMinZoomScale;
}

Float ZoomManager::GetDefaultScale()
{
    return mDefaultScale;
}

/**
 * Returns the zoom scale used for reading text on a double-tap.
 */
Float ZoomManager::GetReadingLevelScale()
{
    return ComputeScaleWithLimits(ComputeReadingLevelScale(GetZoomOverviewScale()));
}

/* package */
Float ZoomManager::ComputeReadingLevelScale(
    /* [in] */ Float scale)
{
    return Elastos::Core::Math::Max(mDisplayDensity * mDoubleTapZoomFactor,
                scale + MIN_DOUBLE_TAP_SCALE_INCREMENT);
}

Float ZoomManager::GetInvDefaultScale()
{
    return mInvDefaultScale;
}

Float ZoomManager::GetDefaultMaxZoomScale()
{
    return mDefaultMaxZoomScale;
}

Float ZoomManager::GetDefaultMinZoomScale()
{
    return mDefaultMinZoomScale;
}

Int32 ZoomManager::GetDocumentAnchorX()
{
    return mAnchorX;
}

Int32 ZoomManager::GetDocumentAnchorY()
{
    return mAnchorY;
}

void ZoomManager::ClearDocumentAnchor()
{
    mAnchorX = mAnchorY = 0;
}

void ZoomManager::SetZoomCenter(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    mZoomCenterX = x;
    mZoomCenterY = y;
}

void ZoomManager::SetInitialScaleInPercent(
    /* [in] */ Int32 scaleInPercent)
{
    mInitialScale = scaleInPercent * 0.01f;
}

Float ZoomManager::ComputeScaleWithLimits(
    /* [in] */ Float scale)
{
    if (scale < mMinZoomScale) {
        scale = mMinZoomScale;
    }
    else if (scale > mMaxZoomScale) {
        scale = mMaxZoomScale;
    }
    return scale;
}

Boolean ZoomManager::IsScaleOverLimits(
    /* [in] */ Float scale)
{
    return scale <= mMinZoomScale || scale >= mMaxZoomScale;
}

Boolean ZoomManager::IsZoomScaleFixed()
{
    return mMinZoomScale >= mMaxZoomScale;
}

Boolean ZoomManager::ExceedsMinScaleIncrement(
    /* [in] */ Float scaleA,
    /* [in] */ Float scaleB)
{
    return Elastos::Core::Math::Abs(scaleA - scaleB) >= MINIMUM_SCALE_INCREMENT;
}

Boolean ZoomManager::WillScaleTriggerZoom(
    /* [in] */ Float scale)
{
    return ExceedsMinScaleIncrement(scale, mActualScale);
}

Boolean ZoomManager::CanZoomIn()
{
    return mMaxZoomScale - mActualScale > MINIMUM_SCALE_INCREMENT;
}

Boolean ZoomManager::CanZoomOut()
{
    return mActualScale - mMinZoomScale > MINIMUM_SCALE_INCREMENT;
}

Boolean ZoomManager::ZoomIn()
{
    return Zoom(1.25f);
}

Boolean ZoomManager::ZoomOut()
{
    return Zoom(0.8f);
}

// returns TRUE if zoom out succeeds and FALSE if no zoom changes.
Boolean ZoomManager::Zoom(
    /* [in] */ Float zoomMultiplier)
{
    mInitialZoomOverview = FALSE;
    // TODO: alternatively we can disallow this during draw history mode
    mWebView->SwitchOutDrawHistory();
    // Center zooming to the center of the screen.
    mZoomCenterX = mWebView->GetViewWidth() * .5f;
    mZoomCenterY = mWebView->GetViewHeight() * .5f;
    mAnchorX = mWebView->ViewToContentX((Int32) mZoomCenterX + mWebView->GetScrollX());
    mAnchorY = mWebView->ViewToContentY((Int32) mZoomCenterY + mWebView->GetScrollY());
    AutoPtr<IWebSettings> settings;
    mWebView->GetSettings((IWebSettings**)&settings);
    return StartZoomAnimation(mActualScale * zoomMultiplier,
            !((WebSettingsClassic*)(IWebSettingsClassic*)settings.Get())->GetUseFixedViewport());
}

/**
 * Initiates an animated zoom of the WebView.
 *
 * @return true if the new scale triggered an animation and false otherwise.
 */
Boolean ZoomManager::StartZoomAnimation(
    /* [in] */ Float scale,
    /* [in] */ Boolean reflowText)
{
    mInitialZoomOverview = FALSE;
    Float oldScale = mActualScale;
    mInitialScrollX = mWebView->GetScrollX();
    mInitialScrollY = mWebView->GetScrollY();

    // snap to reading level scale if it is close
    if (!ExceedsMinScaleIncrement(scale, GetReadingLevelScale())) {
        scale = GetReadingLevelScale();
    }

    SetZoomScale(scale, reflowText);

    if (oldScale != mActualScale) {
        if (mHardwareAccelerated) {
            mInHWAcceleratedZoom = TRUE;
        }
        // use mZoomPickerScale to see zoom preview first
        mZoomStart = SystemClock::GetUptimeMillis();
        mInvInitialZoomScale = 1.0f / oldScale;
        mInvFinalZoomScale = 1.0f / mActualScale;
        mZoomScale = mActualScale;
        mWebView->OnFixedLengthZoomAnimationStart();
        mWebView->Invalidate();
        return TRUE;
    }
    else {
        return FALSE;
    }
}

/**
 * This method is called by the WebView's drawing code when a fixed length zoom
 * animation is occurring. Its purpose is to animate the zooming of the canvas
 * to the desired scale which was specified in startZoomAnimation(...).
 *
 * A fixed length animation begins when startZoomAnimation(...) is called and
 * continues until the ZOOM_ANIMATION_LENGTH time has elapsed. During that
 * interval each time the WebView draws it calls this function which is
 * responsible for generating the animation.
 *
 * Additionally, the WebView can check to see if such an animation is currently
 * in progress by calling isFixedLengthAnimationInProgress().
 */
void ZoomManager::AnimateZoom(
    /* [in] */ ICanvas* canvas)
{
    mInitialZoomOverview = FALSE;
    if (mZoomScale == 0) {
        Logger::W(LOGTAG, "A WebView is attempting to perform a fixed length zoom animation when no zoom is in progress");
        // Now that we've logged about it, go ahead and just recover
        mInHWAcceleratedZoom = FALSE;
        return;
    }

    Float zoomScale;
    Int32 interval = (Int32) (SystemClock::GetUptimeMillis() - mZoomStart);
    if (interval < ZOOM_ANIMATION_LENGTH) {
        Float ratio = (Float) interval / ZOOM_ANIMATION_LENGTH;
        zoomScale = 1.0f / (mInvInitialZoomScale
                + (mInvFinalZoomScale - mInvInitialZoomScale) * ratio);
        mWebView->Invalidate();
    }
    else {
        zoomScale = mZoomScale;
        // set mZoomScale to be 0 as we have finished animating
        mZoomScale = 0;
        mWebView->OnFixedLengthZoomAnimationEnd();
    }
    // calculate the intermediate scroll position. Since we need to use
    // zoomScale, we can't use the WebView's pinLocX/Y functions directly.
    Float scale = zoomScale * mInvInitialZoomScale;
    Int32 tx = Elastos::Core::Math::Round(scale * (mInitialScrollX + mZoomCenterX) - mZoomCenterX);
    Int32 contentWidth;
    mWebView->GetContentWidth(&contentWidth);
    tx = -CWebViewClassic::PinLoc(tx, mWebView->GetViewWidth(), Elastos::Core::Math::Round(contentWidth
            * zoomScale)) + mWebView->GetScrollX();
    Int32 titleHeight = mWebView->GetTitleHeight();
    Int32 ty = Elastos::Core::Math::Round(scale
            * (mInitialScrollY + mZoomCenterY - titleHeight)
            - (mZoomCenterY - titleHeight));
    Int32 contentHeight;
    ty = -(ty <= titleHeight ? Elastos::Core::Math::Max(ty, 0) : (mWebView->GetContentHeight(&contentHeight), CWebViewClassic::PinLoc(ty
            - titleHeight, mWebView->GetViewHeight(), Elastos::Core::Math::Round(contentHeight
            * zoomScale)) + titleHeight) + mWebView->GetScrollY());

    if (mHardwareAccelerated) {
        mWebView->UpdateScrollCoordinates(mWebView->GetScrollX() - tx, mWebView->GetScrollY() - ty);
        // By adding webView matrix, we need to offset the canvas a bit
        // to make the animation smooth.
        canvas->Translate(tx, ty);
        SetZoomScale(zoomScale, FALSE);

        if (mZoomScale == 0) {
            // We've reached the end of the zoom animation.
            mInHWAcceleratedZoom = FALSE;

            // Ensure that the zoom level is pushed to WebCore. This has not
            // yet occurred because we prevent it from happening while
            // mInHWAcceleratedZoom is true.
            mWebView->SendViewSizeZoom(FALSE);
        }
    }
    else {
        canvas->Translate(tx, ty);
        canvas->Scale(zoomScale, zoomScale);
    }
}

Boolean ZoomManager::IsZoomAnimating()
{
    return IsFixedLengthAnimationInProgress() || mPinchToZoomAnimating;
}

Boolean ZoomManager::IsFixedLengthAnimationInProgress()
{
    return mZoomScale != 0 || mInHWAcceleratedZoom;
}

void ZoomManager::UpdateDoubleTapZoom(
    /* [in] */ Int32 doubleTapZoom)
{
    Boolean zoomIn = (mTextWrapScale - mActualScale) < 0.1f;
    mDoubleTapZoomFactor = doubleTapZoom / 100.0f;
    mTextWrapScale = GetReadingLevelScale();
    Float newScale = zoomIn ? mTextWrapScale
            : Elastos::Core::Math::Min(mTextWrapScale, mActualScale);
    SetZoomScale(newScale, TRUE, TRUE);
}

void ZoomManager::RefreshZoomScale(
    /* [in] */ Boolean reflowText)
{
    SetZoomScale(mActualScale, reflowText, TRUE);
}

void ZoomManager::SetZoomScale(
    /* [in] */ Float scale,
    /* [in] */ Boolean reflowText)
{
    SetZoomScale(scale, reflowText, FALSE);
}

void ZoomManager::SetZoomScale(
    /* [in] */ Float scale,
    /* [in] */ Boolean reflowText,
    /* [in] */ Boolean force)
{
    Boolean isScaleLessThanMinZoom = scale < mMinZoomScale;
    scale = ComputeScaleWithLimits(scale);

    // determine whether or not we are in the zoom overview mode
    if (isScaleLessThanMinZoom && mMinZoomScale < mDefaultScale) {
        mInZoomOverview = TRUE;
    }
    else {
        mInZoomOverview = !ExceedsMinScaleIncrement(scale, GetZoomOverviewScale());
    }

    AutoPtr<IWebSettings> settings;
    mWebView->GetSettings((IWebSettings**)&settings);
    if (reflowText && !((WebSettingsClassic*)(IWebSettingsClassic*)settings.Get())->GetUseFixedViewport()) {
        mTextWrapScale = scale;
    }

    if (scale != mActualScale || force) {
        Float oldScale = mActualScale;
        Float oldInvScale = mInvActualScale;

        if (scale != mActualScale && !mPinchToZoomAnimating) {
            mCallbackProxy->OnScaleChanged(mActualScale, scale);
        }

        mActualScale = scale;
        mInvActualScale = 1 / scale;

        if (!mWebView->DrawHistory() && !mInHWAcceleratedZoom) {
            // If history Picture is drawn, don't update scroll. They will
            // be updated when we get out of that mode.
            // update our scroll so we don't appear to jump
            // i.e. keep the center of the doc in the center of the view
            // If this is part of a zoom on a HW accelerated canvas, we
            // have already updated the scroll so don't do it again.
            Int32 oldX = mWebView->GetScrollX();
            Int32 oldY = mWebView->GetScrollY();
            Float ratio = scale * oldInvScale;
            Float sx = ratio * oldX + (ratio - 1) * mZoomCenterX;
            Float sy = ratio * oldY + (ratio - 1)
                    * (mZoomCenterY - mWebView->GetTitleHeight());

            // Scale all the child views
            mWebView->mViewManager->ScaleAll();

            // as we don't have animation for scaling, don't do animation
            // for scrolling, as it causes weird intermediate state
            Int32 scrollX = mWebView->PinLocX(Elastos::Core::Math::Round(sx));
            Int32 scrollY = mWebView->PinLocY(Elastos::Core::Math::Round(sy));
            if (!mWebView->UpdateScrollCoordinates(scrollX, scrollY)) {
                // the scroll position is adjusted at the beginning of the
                // zoom animation. But we want to update the WebKit at the
                // end of the zoom animation. See comments in onScaleEnd().
                mWebView->SendOurVisibleRect();
            }
        }

        // if the we need to reflow the text then force the VIEW_SIZE_CHANGED
        // event to be sent to WebKit
        mWebView->SendViewSizeZoom(reflowText);
    }
}

Boolean ZoomManager::IsDoubleTapEnabled()
{
    AutoPtr<IWebSettings> settings;
    mWebView->GetSettings((IWebSettings**)&settings);
    Boolean result = FALSE;
    return settings != NULL && (settings->GetUseWideViewPort(&result), result);
}

/**
 * The double tap gesture can result in different behaviors depending on the
 * content that is tapped.
 *
 * (1) PLUGINS: If the taps occur on a plugin then we maximize the plugin on
 * the screen. If the plugin is already maximized then zoom the user into
 * overview mode.
 *
 * (2) HTML/OTHER: If the taps occur outside a plugin then the following
 * heuristic is used.
 *   A. If the current text wrap scale differs from newly calculated and the
 *      layout algorithm specifies the use of NARROW_COLUMNS, then fit to
 *      column by reflowing the text.
 *   B. If the page is not in overview mode then change to overview mode.
 *   C. If the page is in overmode then change to the default scale.
 */
void ZoomManager::HandleDoubleTap(
    /* [in] */ Float lastTouchX,
    /* [in] */ Float lastTouchY)
{
    // User takes action, set initial zoom overview to false.
    mInitialZoomOverview = FALSE;
    AutoPtr<IWebSettings> _settings;
    mWebView->GetSettings((IWebSettings**)&_settings);
    AutoPtr<WebSettingsClassic> settings = (WebSettingsClassic*)(IWebSettingsClassic*)_settings.Get();
    if (!IsDoubleTapEnabled()) {
        return;
    }

    SetZoomCenter(lastTouchX, lastTouchY);
    mAnchorX = mWebView->ViewToContentX((Int32) lastTouchX + mWebView->GetScrollX());
    mAnchorY = mWebView->ViewToContentY((Int32) lastTouchY + mWebView->GetScrollY());
    settings->SetDoubleTapToastCount(0);

    // remove the zoom control after double tap
    DismissZoomPicker();

    Float newTextWrapScale;
    if (settings->GetUseFixedViewport()) {
        newTextWrapScale = Elastos::Core::Math::Max(mActualScale, GetReadingLevelScale());
    }
    else {
        newTextWrapScale = mActualScale;
    }
    Boolean firstTimeReflow = !ExceedsMinScaleIncrement(mActualScale, mTextWrapScale);
    if (firstTimeReflow || mInZoomOverview) {
        // In case first time reflow or in zoom overview mode, let reflow and zoom
        // happen at the same time.
        mTextWrapScale = newTextWrapScale;
    }
    if (settings->IsNarrowColumnLayout()
            && ExceedsMinScaleIncrement(mTextWrapScale, newTextWrapScale)
            && !firstTimeReflow
            && !mInZoomOverview) {
        // Reflow only.
        mTextWrapScale = newTextWrapScale;
        RefreshZoomScale(TRUE);
    }
    else if (!mInZoomOverview && WillScaleTriggerZoom(GetZoomOverviewScale())) {
        // Reflow, if necessary.
        if (mTextWrapScale > GetReadingLevelScale()) {
            mTextWrapScale = GetReadingLevelScale();
            RefreshZoomScale(TRUE);
        }
        ZoomToOverview();
    }
    else {
        ZoomToReadingLevel();
    }
}

void ZoomManager::SetZoomOverviewWidth(
    /* [in] */ Int32 width)
{
    if (width == 0) {
        mZoomOverviewWidth = CWebViewClassic::DEFAULT_VIEWPORT_WIDTH;
    }
    else {
        mZoomOverviewWidth = width;
    }
    mInvZoomOverviewWidth = 1.0f / width;
}

/* package */
Float ZoomManager::GetZoomOverviewScale()
{
    return mWebView->GetViewWidth() * mInvZoomOverviewWidth;
}

Boolean ZoomManager::IsInZoomOverview()
{
    return mInZoomOverview;
}

void ZoomManager::ZoomToOverview()
{
    // Force the titlebar fully reveal in overview mode
    Int32 scrollY = mWebView->GetScrollY();
    if (scrollY < mWebView->GetTitleHeight()) {
        mWebView->UpdateScrollCoordinates(mWebView->GetScrollX(), 0);
    }
    AutoPtr<IWebSettings> settings;
    mWebView->GetSettings((IWebSettings**)&settings);
    StartZoomAnimation(GetZoomOverviewScale(),
        !((WebSettingsClassic*)(IWebSettingsClassic*)settings.Get())->GetUseFixedViewport());
}

void ZoomManager::ZoomToReadingLevel()
{
    Float readingScale = GetReadingLevelScale();

    Int32 left = mWebView->GetBlockLeftEdge(mAnchorX, mAnchorY, readingScale);
    if (left != CWebViewClassic::NO_LEFTEDGE) {
        // add a 5pt padding to the left edge.
        Int32 viewLeft = mWebView->ContentToViewX(left < 5 ? 0 : (left - 5))
                - mWebView->GetScrollX();
        // Re-calculate the zoom center so that the new scroll x will be
        // on the left edge.
        if (viewLeft > 0) {
            mZoomCenterX = viewLeft * readingScale / (readingScale - mActualScale);
        }
        else {
            AutoPtr<IWebView> webView;
            mWebView->GetWebView((IWebView**)&webView);
            webView->ScrollBy(viewLeft, 0);
            mZoomCenterX = 0;
        }
    }
    AutoPtr<IWebSettings> settings;
    mWebView->GetSettings((IWebSettings**)&settings);
    StartZoomAnimation(readingScale,
        !((WebSettingsClassic*)(IWebSettingsClassic*)settings.Get())->GetUseFixedViewport());
}

void ZoomManager::UpdateMultiTouchSupport(
    /* [in] */ IContext* context)
{
    // check the preconditions
    AutoPtr<IWebSettings> settings;
    mWebView->GetSettings((IWebSettings**)&settings);
    assert(settings != NULL);

    AutoPtr<IPackageManager> pm;
    context->GetPackageManager((IPackageManager**)&pm);
    Boolean result = FALSE;
    mSupportMultiTouch =
            (pm->HasSystemFeature(IPackageManager::FEATURE_TOUCHSCREEN_MULTITOUCH, &result), result)
             || (pm->HasSystemFeature(IPackageManager::FEATURE_FAKETOUCH_MULTITOUCH_DISTINCT, &result), result)
            && (settings->SupportZoom(&result), result)
            && (settings->GetBuiltInZoomControls(&result), result);
    mAllowPanAndScale =
            (pm->HasSystemFeature(IPackageManager::FEATURE_TOUCHSCREEN_MULTITOUCH_DISTINCT, &result), result)
            || (pm->HasSystemFeature(IPackageManager::FEATURE_FAKETOUCH_MULTITOUCH_DISTINCT, &result), result);

    if (mSupportMultiTouch && (mScaleDetector == NULL)) {
        AutoPtr<IOnScaleGestureListener> listener = new ScaleDetectorListener(this);
        CScaleGestureDetector::New(context, listener, (IScaleGestureDetector**)&mScaleDetector);
    }
    else if (!mSupportMultiTouch && (mScaleDetector != NULL)) {
        mScaleDetector = NULL;
    }
}

Boolean ZoomManager::SupportsMultiTouchZoom()
{
    return mSupportMultiTouch;
}

Boolean ZoomManager::SupportsPanDuringZoom()
{
    return mAllowPanAndScale;
}

/**
 * Notifies the caller that the ZoomManager is requesting that scale related
 * updates should not be sent to webkit. This can occur in cases where the
 * ZoomManager is performing an animation and does not want webkit to update
 * until the animation is complete.
 *
 * @return true if scale related updates should not be sent to webkit and
 *         false otherwise.
 */
Boolean ZoomManager::IsPreventingWebkitUpdates()
{
    // currently only animating a multi-touch zoom and fixed length
    // animations prevent updates, but others can add their own conditions
    // to this method if necessary.
    return IsZoomAnimating();
}

AutoPtr<IScaleGestureDetector> ZoomManager::GetScaleGestureDetector()
{
    return mScaleDetector;
}

void ZoomManager::SanitizeMinMaxScales()
{
    if (mMinZoomScale > mMaxZoomScale) {
        Logger::W(LOGTAG, "mMinZoom > mMaxZoom!!! %f > %f", mMinZoomScale, mMaxZoomScale);
        mMaxZoomScale = mMinZoomScale;
    }
}

void ZoomManager::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 ow,
    /* [in] */ Int32 oh)
{
    // reset zoom and anchor to the top left corner of the screen
    // unless we are already zooming
    if (!IsFixedLengthAnimationInProgress()) {
        Int32 visibleTitleHeight;
        mWebView->GetVisibleTitleHeight(&visibleTitleHeight);
        mZoomCenterX = 0;
        mZoomCenterY = visibleTitleHeight;
        mAnchorX = mWebView->ViewToContentX(mWebView->GetScrollX());
        mAnchorY = mWebView->ViewToContentY(visibleTitleHeight + mWebView->GetScrollY());
    }

    // update mMinZoomScale if the minimum zoom scale is not fixed
    if (!mMinZoomScaleFixed) {
        // when change from narrow screen to wide screen, the new viewWidth
        // can be wider than the old content width. We limit the minimum
        // scale to 1.0f. The proper minimum scale will be calculated when
        // the new picture shows up.
        mMinZoomScale = Elastos::Core::Math::Min(1.0f, (Float) mWebView->GetViewWidth()
                / (mWebView->DrawHistory() ? mWebView->GetHistoryPictureWidth()
                        : mZoomOverviewWidth));
        // limit the minZoomScale to the initialScale if it is set
        if (mInitialScale > 0 && mInitialScale < mMinZoomScale) {
            mMinZoomScale = mInitialScale;
        }
        SanitizeMinMaxScales();
    }

    DismissZoomPicker();

    // onSizeChanged() is called during WebView layout. And any
    // requestLayout() is blocked during layout. As refreshZoomScale() will
    // cause its child View to reposition itself through ViewManager's
    // scaleAll(), we need to post a Runnable to ensure requestLayout().
    // Additionally, only update the text wrap scale if the width changed.
    AutoPtr<IWebSettings> settings;
    mWebView->GetSettings((IWebSettings**)&settings);
    AutoPtr<IRunnable> r = new PostScale(w != ow &&
            !((WebSettingsClassic*)(IWebSettingsClassic*)settings.Get())->GetUseFixedViewport(),
            mInZoomOverview, w < ow, this);
    AutoPtr<IWebView> webView;
    mWebView->GetWebView((IWebView**)&webView);
    Boolean result;
    webView->Post(r, &result);
}

void ZoomManager::UpdateZoomRange(
    /* [in] */ CWebViewCoreViewState* viewState,
    /* [in] */ Int32 viewWidth,
    /* [in] */ Int32 minPrefWidth)
{
    if (viewState->mMinScale == 0) {
        if (viewState->mMobileSite) {
            if (minPrefWidth > Elastos::Core::Math::Max(0, viewWidth)) {
                mMinZoomScale = (Float) viewWidth / minPrefWidth;
                mMinZoomScaleFixed = FALSE;
            }
            else {
                mMinZoomScale = viewState->mDefaultScale;
                mMinZoomScaleFixed = TRUE;
            }
        }
        else {
            mMinZoomScale = mDefaultMinZoomScale;
            mMinZoomScaleFixed = FALSE;
        }
    }
    else {
        mMinZoomScale = viewState->mMinScale;
        mMinZoomScaleFixed = TRUE;
    }
    if (viewState->mMaxScale == 0) {
        mMaxZoomScale = mDefaultMaxZoomScale;
    }
    else {
        mMaxZoomScale = viewState->mMaxScale;
    }
    SanitizeMinMaxScales();
}

/**
 * Updates zoom values when Webkit produces a new picture. This method
 * should only be called from the UI thread's message handler.
 *
 * @return True if zoom value has changed
 */
Boolean ZoomManager::OnNewPicture(
    /* [in] */ CWebViewCoreDrawData* drawData)
{
    Int32 viewWidth = mWebView->GetViewWidth();
    Boolean zoomOverviewWidthChanged = SetupZoomOverviewWidth(drawData, viewWidth);
    Float newZoomOverviewScale = GetZoomOverviewScale();
    AutoPtr<IWebSettings> _settings;
    mWebView->GetSettings((IWebSettings**)&_settings);
    AutoPtr<WebSettingsClassic> settings = (WebSettingsClassic*)(IWebSettingsClassic*)_settings.Get();
    if (zoomOverviewWidthChanged &&
        settings->IsNarrowColumnLayout() &&
        settings->GetUseFixedViewport() &&
        (mInitialZoomOverview || mInZoomOverview)) {
        // Keep mobile site's text wrap scale unchanged.  For mobile sites,
        // the text wrap scale is the same as zoom overview scale.
        if (ExceedsMinScaleIncrement(mTextWrapScale, mDefaultScale) ||
                ExceedsMinScaleIncrement(newZoomOverviewScale, mDefaultScale)) {
            mTextWrapScale = GetReadingLevelScale();
        }
        else {
            mTextWrapScale = newZoomOverviewScale;
        }
    }

    if (!mMinZoomScaleFixed || settings->GetUseWideViewPort()) {
        mMinZoomScale = newZoomOverviewScale;
        mMaxZoomScale = Elastos::Core::Math::Max(mMaxZoomScale, mMinZoomScale);
        SanitizeMinMaxScales();
    }

    // fit the content width to the current view for the first new picture
    // after first layout.
    Boolean scaleHasDiff = ExceedsMinScaleIncrement(newZoomOverviewScale, mActualScale);
    // Make sure the actual scale is no less than zoom overview scale.
    Boolean scaleLessThanOverview =
            (newZoomOverviewScale - mActualScale) >= MINIMUM_SCALE_INCREMENT;
    // Make sure mobile sites are correctly handled since mobile site will
    // change content width after rotating.
    Boolean mobileSiteInOverview = mInZoomOverview &&
            !ExceedsMinScaleIncrement(newZoomOverviewScale, mDefaultScale);
    if (!mWebView->DrawHistory() &&
        ((scaleLessThanOverview && settings->GetUseWideViewPort())||
            ((mInitialZoomOverview || mobileSiteInOverview) &&
                scaleHasDiff && zoomOverviewWidthChanged))) {
        mInitialZoomOverview = FALSE;
        SetZoomScale(newZoomOverviewScale, !WillScaleTriggerZoom(mTextWrapScale) &&
            !settings->GetUseFixedViewport());
    }
    else {
        mInZoomOverview = !scaleHasDiff;
    }

    if (drawData->mFirstLayoutForNonStandardLoad && settings->GetLoadWithOverviewMode()) {
        // Set mInitialZoomOverview in case this is the first picture for non standard load,
        // so next new picture could be forced into overview mode if it's true.
        mInitialZoomOverview = mInZoomOverview;
    }

    return scaleHasDiff;
}

/**
 * Set up correct zoom overview width based on different settings.
 *
 * @param drawData webviewcore draw data
 * @param viewWidth current view width
 */
Boolean ZoomManager::SetupZoomOverviewWidth(
    /* [in] */ CWebViewCoreDrawData* drawData,
    /* [in] */ const Int32 viewWidth)
{
    AutoPtr<IWebSettings> settings;
    mWebView->GetSettings((IWebSettings**)&settings);
    Int32 newZoomOverviewWidth = mZoomOverviewWidth;
    Boolean result = FALSE;
    if (settings->GetUseWideViewPort(&result), result) {
        Int32 x;
        drawData->mContentSize->GetX(&x);
        if (x > 0) {
            // The webkitDraw for layers will not populate contentSize, and it'll be
            // ignored for zoom overview width update.
            newZoomOverviewWidth = Elastos::Core::Math::Min(CWebViewClassic::sMaxViewportWidth, x);
        }
    }
    else {
        // If not use wide viewport, use view width as the zoom overview width.
        newZoomOverviewWidth = Elastos::Core::Math::Round(viewWidth / mDefaultScale);
    }
    if (newZoomOverviewWidth != mZoomOverviewWidth) {
        SetZoomOverviewWidth(newZoomOverviewWidth);
        return TRUE;
    }
    return FALSE;
}

/**
 * Updates zoom values after Webkit completes the initial page layout. It
 * is called when visiting a page for the first time as well as when the
 * user navigates back to a page (in which case we may need to restore the
 * zoom levels to the state they were when you left the page). This method
 * should only be called from the UI thread's message handler.
 */
void ZoomManager::OnFirstLayout(
    /* [in] */ CWebViewCoreDrawData* drawData)
{
    // precondition check
    assert(drawData != NULL);
    assert(drawData->mViewState != NULL);
    AutoPtr<IWebSettings> _settings;
    mWebView->GetSettings((IWebSettings**)&_settings);
    assert(_settings != NULL);

    AutoPtr<CWebViewCoreViewState> viewState = drawData->mViewState;
    AutoPtr<IPoint> viewSize = drawData->mViewSize;
    Int32 x;
    viewSize->GetX(&x);
    UpdateZoomRange(viewState, x, drawData->mMinPrefWidth);
    SetupZoomOverviewWidth(drawData, mWebView->GetViewWidth());
    Float overviewScale = GetZoomOverviewScale();
    AutoPtr<WebSettingsClassic> settings = (WebSettingsClassic*)(IWebSettingsClassic*)_settings.Get();
    if (!mMinZoomScaleFixed || settings->GetUseWideViewPort()) {
        mMinZoomScale = (mInitialScale > 0) ?
                Elastos::Core::Math::Min(mInitialScale, overviewScale) : overviewScale;
        mMaxZoomScale = Elastos::Core::Math::Max(mMaxZoomScale, mMinZoomScale);
        SanitizeMinMaxScales();
    }

    if (!mWebView->DrawHistory()) {
        Float scale;
        if (mInitialScale > 0) {
            scale = mInitialScale;
        }
        else if (viewState->mIsRestored || viewState->mViewScale > 0) {
            scale = (viewState->mViewScale > 0)
                ? viewState->mViewScale : overviewScale;
            mTextWrapScale = (viewState->mTextWrapScale > 0)
                ? viewState->mTextWrapScale : GetReadingLevelScale();
        }
        else {
            scale = overviewScale;
            if (!settings->GetUseWideViewPort()
                || !settings->GetLoadWithOverviewMode()) {
                scale = Elastos::Core::Math::Max(mDefaultScale, scale);
            }
            if (settings->IsNarrowColumnLayout() &&
                settings->GetUseFixedViewport()) {
                // When first layout, reflow using the reading level scale to avoid
                // reflow when double tapped.
                mTextWrapScale = GetReadingLevelScale();
            }
        }
        Boolean reflowText = FALSE;
        if (!viewState->mIsRestored) {
            if (settings->GetUseFixedViewport()) {
                // Override the scale only in case of fixed viewport.
                scale = Elastos::Core::Math::Max(scale, overviewScale);
                mTextWrapScale = Elastos::Core::Math::Max(mTextWrapScale, overviewScale);
            }
            reflowText = ExceedsMinScaleIncrement(mTextWrapScale, scale);
        }
        mInitialZoomOverview = settings->GetLoadWithOverviewMode() &&
                !ExceedsMinScaleIncrement(scale, overviewScale);
        SetZoomScale(scale, reflowText);

        // update the zoom buttons as the scale can be changed
        UpdateZoomPicker();
    }
}

void ZoomManager::SaveZoomState(
    /* [in] */ IBundle* b)
{
    b->PutFloat(String("scale"), mActualScale);
    b->PutFloat(String("textwrapScale"), mTextWrapScale);
    b->PutBoolean(String("overview"), mInZoomOverview);
}

void ZoomManager::RestoreZoomState(
    /* [in] */ IBundle* b)
{
    // as getWidth() / getHeight() of the view are not available yet, set up
    // mActualScale, so that when onSizeChanged() is called, the rest will
    // be set correctly
    b->GetFloat(String("scale"), 1.0f, &mActualScale);
    mInvActualScale = 1 / mActualScale;
    b->GetFloat(String("textwrapScale"), mActualScale, &mTextWrapScale);
    b->GetBoolean(String("overview"), &mInZoomOverview);
}

AutoPtr<IZoomControlBase> ZoomManager::GetCurrentZoomControl()
{
    AutoPtr<IWebSettings> settings;
    mWebView->GetSettings((IWebSettings**)&settings);
    Boolean result;
    if (settings != NULL && (settings->SupportZoom(&result), result)) {
        settings->GetBuiltInZoomControls(&result);
        if (result) {
            if ((mEmbeddedZoomControl == NULL)
                    && (settings->GetDisplayZoomControls(&result), result)) {
                mEmbeddedZoomControl = new ZoomControlEmbedded(this, mWebView);
            }
            return (IZoomControlBase*)mEmbeddedZoomControl.Get();
        }
        else {
            if (mExternalZoomControl == NULL) {
                mExternalZoomControl = new ZoomControlExternal(mWebView);
            }
            return (IZoomControlBase*)mExternalZoomControl.Get();
        }
    }
    return NULL;
}

void ZoomManager::InvokeZoomPicker()
{
    AutoPtr<IZoomControlBase> control = GetCurrentZoomControl();
    if (control != NULL) {
        control->Show();
    }
}

void ZoomManager::DismissZoomPicker()
{
    AutoPtr<IZoomControlBase> control = GetCurrentZoomControl();
    if (control != NULL) {
        control->Hide();
    }
}

Boolean ZoomManager::IsZoomPickerVisible()
{
    AutoPtr<IZoomControlBase> control = GetCurrentZoomControl();
    Boolean isVisible = FALSE;
    return (control != NULL) ? (control->IsVisible(&isVisible), isVisible) : FALSE;
}

void ZoomManager::UpdateZoomPicker()
{
    AutoPtr<IZoomControlBase> control = GetCurrentZoomControl();
    if (control != NULL) {
        control->Update();
    }
}

/**
 * The embedded zoom control intercepts touch events and automatically stays
 * visible. The external control needs to constantly refresh its internal
 * timer to stay visible.
 */
void ZoomManager::KeepZoomPickerVisible()
{
    AutoPtr<IZoomControlBase> control = GetCurrentZoomControl();
    if (control != NULL && control == (IZoomControlBase*)mExternalZoomControl.Get()) {
        control->Show();
    }
}

AutoPtr<IView> ZoomManager::GetExternalZoomPicker()
{
    AutoPtr<IZoomControlBase> control = GetCurrentZoomControl();
    if (control != NULL && control == (IZoomControlBase*)mExternalZoomControl.Get()) {
        return (IView*)(mExternalZoomControl->GetControls().Get());
    }
    else {
        return NULL;
    }
}

void ZoomManager::SetHardwareAccelerated()
{
    mHardwareAccelerated = TRUE;
}

/**
 * OnPageFinished called by webview when a page is fully loaded.
 */
/* package*/
void ZoomManager::OnPageFinished(
    /* [in] */ const String& url)
{
    // Turn off initial zoom overview flag when a page is fully loaded.
    mInitialZoomOverview = FALSE;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
