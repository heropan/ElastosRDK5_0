
#include "wm/AppWindowAnimator.h"

 using Elastos::Droid::View::Animation::CTransformation;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

const String AppWindowAnimator::TAG("AppWindowAnimator");
const AutoPtr<IAnimation> AppWindowAnimator::sDummyAnimation
        = (IAnimation*)new DummyAnimation();

AppWindowAnimator::AppWindowAnimator(
    /* [in] */ AppWindowToken* atoken)
    : mService(atoken->mService)
    , mAnimator(atoken->mAnimator)
    , mAnimating(FALSE)
    , mAnimInitialized(FALSE)
    , mHasTransformation(FALSE)
    , mFreezingScreen(FALSE)
    , mAnimLayerAdjustment(0)
    , mAllDrawn(FALSE)
    , mThumbnailTransactionSeq(0)
    , mThumbnailX(0)
    , mThumbnailY(0)
    , mThumbnailLayer(0)
{
    AutoPtr<IWeakReferenceSource> wrs = (IWeakReferenceSource*)atoken->Probe(EIID_IWeakReferenceSource);
    assert(wrs != NULL);
    wrs->GetWeakReference((IWeakReference**)&mWeakAppToken);

    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mTransformation));
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mThumbnailTransformation));
}

AutoPtr<AppWindowToken> AppWindowAnimator::GetAppToken()
{
    AutoPtr<AppWindowToken> appWinToken;
    if (mWeakAppToken != NULL) {
        AutoPtr<IInterface> obj;
        mWeakAppToken->Resolve(EIID_IInterface, (IInterface**)&obj);
        appWinToken = (AppWindowToken*)obj.Get();
    }
    return appWinToken;
}

void AppWindowAnimator::SetAnimation(
    /* [in] */ IAnimation* anim,
    /* [in] */ Boolean initialized)
{
    // if (WindowManagerService.localLOGV) Slog.v(
    //     TAG, "Setting animation in " + mAppToken + ": " + anim);
    AutoPtr<AppWindowToken> appToken = GetAppToken();
    if (appToken == NULL) return;

    mAnimation = anim;
    mAnimating = FALSE;
    mAnimInitialized = initialized;
    anim->RestrictDuration(CWindowManagerService::MAX_ANIMATION_DURATION);
    anim->ScaleCurrentDuration(mService->mTransitionAnimationScale);
    Int32 zorder;
    anim->GetZAdjustment(&zorder);
    Int32 adj = 0;
    if (zorder == IAnimation::ZORDER_TOP) {
        adj = CWindowManagerService::TYPE_LAYER_OFFSET;
    }
    else if (zorder == IAnimation::ZORDER_BOTTOM) {
        adj = -CWindowManagerService::TYPE_LAYER_OFFSET;
    }

    if (mAnimLayerAdjustment != adj) {
        mAnimLayerAdjustment = adj;
        UpdateLayers();
    }
    // Start out animation gone if window is gone, or visible if window is visible.
    mTransformation->Clear();
    mTransformation->SetAlpha(appToken->mReportedVisible ? 1 : 0);
    mHasTransformation = TRUE;
}

void AppWindowAnimator::SetDummyAnimation()
{
    AutoPtr<AppWindowToken> appToken = GetAppToken();
    if (appToken == NULL) return;

    // if (WindowManagerService.localLOGV) Slog.v(TAG, "Setting dummy animation in " + mAppToken);
    mAnimation = sDummyAnimation;
    mAnimInitialized = FALSE;
    mHasTransformation = TRUE;
    mTransformation->Clear();
    mTransformation->SetAlpha(appToken->mReportedVisible ? 1 : 0);
}

void AppWindowAnimator::ClearAnimation()
{
    if (mAnimation != NULL) {
        mAnimation = NULL;
        mAnimating = TRUE;
        mAnimInitialized = FALSE;
    }
    ClearThumbnail();
}

void AppWindowAnimator::ClearThumbnail()
{
    if (mThumbnail != NULL) {
        mThumbnail->Destroy();
        mThumbnail = NULL;
    }
}

void AppWindowAnimator::UpdateLayers()
{
    AutoPtr<AppWindowToken> appToken = GetAppToken();
    if (appToken == NULL) return;

    List<AutoPtr<WindowState> >::Iterator it = appToken->mAllAppWindows.Begin();
    Int32 adj = mAnimLayerAdjustment;
    mThumbnailLayer = -1;
    for (; it != appToken->mAllAppWindows.End(); ++it) {
        AutoPtr<WindowState> w = *it;
        AutoPtr<WindowStateAnimator> winAnimator = w->mWinAnimator;
        winAnimator->mAnimLayer = w->mLayer + adj;
        if (winAnimator->mAnimLayer > mThumbnailLayer) {
            mThumbnailLayer = winAnimator->mAnimLayer;
        }
        // if (WindowManagerService.DEBUG_LAYERS) Slog.v(TAG, "Updating layer " + w + ": "
        //         + winAnimator.mAnimLayer);
        if (w == mService->mInputMethodTarget && !mService->mInputMethodTargetWaitingAnim) {
            mService->SetInputMethodAnimLayerAdjustment(adj);
        }
        if (w == mAnimator->mWallpaperTarget && mAnimator->mLowerWallpaperTarget == NULL) {
            mService->SetWallpaperAnimLayerAdjustmentLocked(adj);
        }
    }
}

void AppWindowAnimator::StepThumbnailAnimation(
    /* [in] */ Int64 currentTime)
{
    mThumbnailTransformation->Clear();
    Boolean result;
    mThumbnailAnimation->GetTransformation(currentTime, mThumbnailTransformation, &result);
    AutoPtr<IMatrix> matrix;
    mThumbnailTransformation->GetMatrix((IMatrix**)&matrix);
    matrix->PreTranslate((Float)mThumbnailX, (Float)mThumbnailY, &result);

    AutoPtr<ScreenRotationAnimation> screenRotationAnimation =
            mAnimator->GetScreenRotationAnimationLocked(IDisplay::DEFAULT_DISPLAY);
    Boolean screenAnimation = screenRotationAnimation != NULL
            && screenRotationAnimation->IsAnimating();
    if (screenAnimation) {
        mThumbnailTransformation->PostCompose(
                screenRotationAnimation->GetEnterTransformation());
    }
    // cache often used attributes locally
    AutoPtr<ArrayOf<Float> > tmpFloats = mService->mTmpFloats;
    matrix->GetValues(tmpFloats.Get());
    // if (WindowManagerService.SHOW_TRANSACTIONS) WindowManagerService.logSurface(thumbnail,
    //         "thumbnail", "POS " + tmpFloats[Matrix.MTRANS_X]
    //         + ", " + tmpFloats[Matrix.MTRANS_Y], null);
    mThumbnail->SetPosition((*tmpFloats)[IMatrix::MTRANS_X],
            (*tmpFloats)[IMatrix::MTRANS_Y]);
    // if (WindowManagerService.SHOW_TRANSACTIONS) WindowManagerService.logSurface(thumbnail,
    //         "thumbnail", "alpha=" + thumbnailTransformation.getAlpha()
    //         + " layer=" + thumbnailLayer
    //         + " matrix=[" + tmpFloats[Matrix.MSCALE_X]
    //         + "," + tmpFloats[Matrix.MSKEW_Y]
    //         + "][" + tmpFloats[Matrix.MSKEW_X]
    //         + "," + tmpFloats[Matrix.MSCALE_Y] + "]", null);
    Float alpha;
    mThumbnailTransformation->GetAlpha(&alpha);
    mThumbnail->SetAlpha(alpha);
    // The thumbnail is layered below the window immediately above this
    // token's anim layer.
    mThumbnail->SetLayer(mThumbnailLayer + CWindowManagerService::WINDOW_LAYER_MULTIPLIER
            - CWindowManagerService::LAYER_OFFSET_THUMBNAIL);
    mThumbnail->SetMatrix((*tmpFloats)[IMatrix::MSCALE_X], (*tmpFloats)[IMatrix::MSKEW_Y],
            (*tmpFloats)[IMatrix::MSKEW_X], (*tmpFloats)[IMatrix::MSCALE_Y]);
}

Boolean AppWindowAnimator::StepAnimation(
    /* [in] */ Int64 currentTime)
{
    if (mAnimation == NULL) {
        return FALSE;
    }
    mTransformation->Clear();
    Boolean more;
    mAnimation->GetTransformation(currentTime, mTransformation, &more);
    // if (false && WindowManagerService.DEBUG_ANIM) Slog.v(
    //     TAG, "Stepped animation in " + mAppToken + ": more=" + more + ", xform=" + transformation);
    if (!more) {
        mAnimation = NULL;
        ClearThumbnail();
        // if (WindowManagerService.DEBUG_ANIM) Slog.v(
        //     TAG, "Finished animation in " + mAppToken + " @ " + currentTime);
    }
    mHasTransformation = more;
    return more;
}

Boolean AppWindowAnimator::StepAnimationLocked(
    /* [in] */ Int64 currentTime,
    /* [in] */ Int32 dw,
    /* [in] */ Int32 dh)
{
    AutoPtr<AppWindowToken> appToken = GetAppToken();
    if (appToken == NULL) return FALSE;

    if (mService->OkToDisplay()) {
        // We will run animations as long as the display isn't frozen.

        if (mAnimation == sDummyAnimation) {
            // This guy is going to animate, but not yet.  For now count
            // it as not animating for purposes of scheduling transactions;
            // when it is really time to animate, this will be set to
            // a real animation and the next call will execute normally.
            return FALSE;
        }

        if ((appToken->mAllDrawn || mAnimating || appToken->mStartingDisplayed)
                && mAnimation != NULL) {
            if (!mAnimating) {
                // if (WindowManagerService.DEBUG_ANIM) Slog.v(
                //     TAG, "Starting animation in " + mAppToken +
                //     " @ " + currentTime + ": dw=" + dw + " dh=" + dh
                //     + " scale=" + mService.mTransitionAnimationScale
                //     + " allDrawn=" + mAppToken.allDrawn + " animating=" + animating);
                if (!mAnimInitialized) {
                    mAnimation->Initialize(dw, dh, dw, dh);
                }
                mAnimation->SetStartTime(currentTime);
                mAnimating = TRUE;
                if (mThumbnail != NULL) {
                    mThumbnail->Show();
                    mThumbnailAnimation->SetStartTime(currentTime);
                }
            }
            if (StepAnimation(currentTime)) {
                // animation isn't over, step any thumbnail and that's
                // it for now.
                if (mThumbnail != NULL) {
                    StepThumbnailAnimation(currentTime);
                }
                return TRUE;
            }
        }
    }
    else if (mAnimation != NULL) {
        // If the display is frozen, and there is a pending animation,
        // clear it and make sure we run the cleanup code.
        mAnimating = TRUE;
        mAnimation = NULL;
    }

    mHasTransformation = FALSE;

    if (!mAnimating && mAnimation == NULL) {
        return FALSE;
    }

    mAnimator->SetAppLayoutChanges(this, IWindowManagerPolicy::FINISH_LAYOUT_REDO_ANIM,
            String("AppWindowToken"));

    ClearAnimation();
    mAnimating = FALSE;
    if (mAnimLayerAdjustment != 0) {
        mAnimLayerAdjustment = 0;
        UpdateLayers();
    }
    if (mService->mInputMethodTarget != NULL
            && mService->mInputMethodTarget->mAppToken == appToken) {
        mService->MoveInputMethodWindowsIfNeededLocked(TRUE);
    }

    // if (WindowManagerService.DEBUG_ANIM) Slog.v(
    //         TAG, "Animation done in " + mAppToken
    //         + ": reportedVisible=" + mAppToken.reportedVisible);

    mTransformation->Clear();

    List<AutoPtr<WindowStateAnimator> >::Iterator it = mAllAppWinAnimators.Begin();
    for (; it != mAllAppWinAnimators.End(); ++it) {
        (*it)->FinishExit();
    }
    appToken->UpdateReportedVisibilityLocked();

    return FALSE;
}

Boolean AppWindowAnimator::ShowAllWindowsLocked()
{
    Boolean isAnimating = FALSE;
    List<AutoPtr<WindowStateAnimator> >::Iterator it = mAllAppWinAnimators.Begin();
    for (; it != mAllAppWinAnimators.End(); ++it) {
        AutoPtr<WindowStateAnimator> winAnimator = *it;
        // if (WindowManagerService.DEBUG_VISIBILITY) Slog.v(TAG,
        //         "performing show on: " + winAnimator);
        winAnimator->PerformShowLocked();
        isAnimating |= winAnimator->IsAnimating();
    }
    return isAnimating;
}

PInterface AppWindowAnimator::DummyAnimation::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IAnimation*)this;
    }
    else if (riid == Elastos::Droid::View::Animation::EIID_IAnimation) {
        return (IAnimation*)this;
    }
    else if (riid == Elastos::Droid::View::Animation::EIID_Animation) {
        return reinterpret_cast<PInterface>((Animation*)this);
    }
    return NULL;
}

UInt32 AppWindowAnimator::DummyAnimation::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AppWindowAnimator::DummyAnimation::Release()
{
    return ElRefBase::Release();
}

ECode AppWindowAnimator::DummyAnimation::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode AppWindowAnimator::DummyAnimation::Reset()
{
    return Animation::Reset();
}

ECode AppWindowAnimator::DummyAnimation::Cancel()
{
    return Animation::Cancel();
}

ECode AppWindowAnimator::DummyAnimation::Detach()
{
    return Animation::Detach();
}

ECode AppWindowAnimator::DummyAnimation::IsInitialized(
    /* [out] */ Boolean* isInitialized)
{
     *isInitialized = Animation::IsInitialized();
     return NOERROR;
}

ECode AppWindowAnimator::DummyAnimation::Initialize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 parentWidth,
    /* [in] */ Int32 parentHeight)
{
    return Animation::Initialize(width, height, parentWidth, parentHeight);
}

ECode AppWindowAnimator::DummyAnimation::SetListenerHandler(
    /* [in] */ IHandler* handler)
{
    return Animation::SetListenerHandler(handler);
}

ECode AppWindowAnimator::DummyAnimation::SetInterpolator(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resID)
{
    return Animation::SetInterpolator(context, resID);
}

ECode AppWindowAnimator::DummyAnimation::SetInterpolator(
    /* [in] */ IInterpolator* i)
{
    return Animation::SetInterpolator(i);
}

ECode AppWindowAnimator::DummyAnimation::SetStartOffset(
    /* [in] */ Int64 startOffset)
{
    return Animation::SetStartOffset(startOffset);
}

ECode AppWindowAnimator::DummyAnimation::SetDuration(
    /* [in] */ Int64 durationMillis)
{
    return Animation::SetDuration(durationMillis);
}

ECode AppWindowAnimator::DummyAnimation::RestrictDuration(
    /* [in] */ Int64 durationMillis)
{
    return Animation::RestrictDuration(durationMillis);
}

ECode AppWindowAnimator::DummyAnimation::ScaleCurrentDuration(
    /* [in] */ Float scale)
{
    return Animation::ScaleCurrentDuration(scale);
}

ECode AppWindowAnimator::DummyAnimation::SetStartTime(
    /* [in] */ Int64 startTimeMillis)
{
    return Animation::SetStartTime(startTimeMillis);
}

ECode AppWindowAnimator::DummyAnimation::Start()
{
    return Animation::Start();
}

ECode AppWindowAnimator::DummyAnimation::StartNow()
{
    return Animation::StartNow();
}

ECode AppWindowAnimator::DummyAnimation::SetRepeatMode(
    /* [in] */ Int32 repeatMode)
{
    return Animation::SetRepeatMode(repeatMode);
}

ECode AppWindowAnimator::DummyAnimation::SetRepeatCount(
    /* [in] */ Int32 repeatCount)
{
    return Animation::SetRepeatCount(repeatCount);
}

ECode AppWindowAnimator::DummyAnimation::IsFillEnabled(
    /* [out] */ Boolean* isFillEnabled)
{
    VALIDATE_NOT_NULL(isFillEnabled);
    *isFillEnabled = Animation::IsFillEnabled();
    return NOERROR;
}

ECode AppWindowAnimator::DummyAnimation::SetFillEnabled(
    /* [in] */ Boolean fillEnabled)
{
    return Animation::SetFillEnabled(fillEnabled);
}

ECode AppWindowAnimator::DummyAnimation::SetFillBefore(
    /* [in] */ Boolean fillBefore)
{
    return Animation::SetFillBefore(fillBefore);
}

ECode AppWindowAnimator::DummyAnimation::SetFillAfter(
    /* [in] */ Boolean fillAfter)
{
    return Animation::SetFillAfter(fillAfter);
}

ECode AppWindowAnimator::DummyAnimation::SetZAdjustment(
    /* [in] */ Int32 zAdjustment)
{
    return Animation::SetZAdjustment(zAdjustment);
}

ECode AppWindowAnimator::DummyAnimation::SetBackgroundColor(
    /* [in] */ Int32 bg)
{
    return Animation::SetBackgroundColor(bg);
}

ECode AppWindowAnimator::DummyAnimation::GetBackgroundColor(
    /* [out] */ Int32* bg)
{
    VALIDATE_NOT_NULL(bg);
    return Animation::GetBackgroundColor(bg);
}

ECode AppWindowAnimator::DummyAnimation::SetDetachWallpaper(
    /* [in] */ Boolean detachWallpaper)
{
    return Animation::SetDetachWallpaper(detachWallpaper);
}

ECode AppWindowAnimator::DummyAnimation::GetInterpolator(
    /* [out] */ IInterpolator** interpolator)
{
    VALIDATE_NOT_NULL(interpolator);
    AutoPtr<IInterpolator> temp = Animation::GetInterpolator();
    *interpolator = temp;
    INTERFACE_ADDREF(*interpolator);
    return NOERROR;
}

ECode AppWindowAnimator::DummyAnimation::GetStartTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = Animation::GetStartTime();
    return NOERROR;
}

ECode AppWindowAnimator::DummyAnimation::GetDuration(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = Animation::GetDuration();
    return NOERROR;
}

ECode AppWindowAnimator::DummyAnimation::GetStartOffset(
    /* [out] */ Int64* startOffset)
{
    VALIDATE_NOT_NULL(startOffset);
    *startOffset = Animation::GetStartOffset();
    return NOERROR;
}

ECode AppWindowAnimator::DummyAnimation::GetRepeatMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = Animation::GetRepeatMode();
    return NOERROR;
}

ECode AppWindowAnimator::DummyAnimation::GetRepeatCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = Animation::GetRepeatCount();
    return NOERROR;
}

ECode AppWindowAnimator::DummyAnimation::GetFillBefore(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Animation::GetFillBefore();
    return NOERROR;
}

ECode AppWindowAnimator::DummyAnimation::GetFillAfter(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Animation::GetFillAfter();
    return NOERROR;
}

ECode AppWindowAnimator::DummyAnimation::GetZAdjustment(
    /* [out] */ Int32* zAdjustment)
{
    VALIDATE_NOT_NULL(zAdjustment);
    *zAdjustment = Animation::GetZAdjustment();
    return NOERROR;
}

ECode AppWindowAnimator::DummyAnimation::GetDetachWallpaper(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Animation::GetDetachWallpaper();
    return NOERROR;
}

ECode AppWindowAnimator::DummyAnimation::WillChangeTransformationMatrix(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Animation::WillChangeTransformationMatrix();
    return NOERROR;
}

ECode AppWindowAnimator::DummyAnimation::WillChangeBounds(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Animation::WillChangeBounds();
    return NOERROR;
}

ECode AppWindowAnimator::DummyAnimation::SetAnimationListener(
    /* [in] */ IAnimationListener* listener)
{
    return Animation::SetAnimationListener(listener);
}

ECode AppWindowAnimator::DummyAnimation::ComputeDurationHint(
    /* [out] */ Int64* hint)
{
    VALIDATE_NOT_NULL(hint);
    *hint = Animation::ComputeDurationHint();
    return NOERROR;
}

ECode AppWindowAnimator::DummyAnimation::GetTransformation(
    /* [in] */ Int64 currentTime,
    /* [in, out] */ ITransformation* outTransformation,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode AppWindowAnimator::DummyAnimation::GetTransformation(
    /* [in] */ Int64 currentTime,
    /* [in, out] */ ITransformation* outTransformation,
    /* [in] */ Float scale,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Animation::GetTransformation(currentTime, outTransformation, scale);
    return NOERROR;
}

ECode AppWindowAnimator::DummyAnimation::HasStarted(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Animation::HasStarted();
    return NOERROR;
}

ECode AppWindowAnimator::DummyAnimation::HasEnded(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Animation::HasEnded();
    return NOERROR;
}

ECode AppWindowAnimator::DummyAnimation::HasAlpha(
    /* [out] */ Boolean* has)
{
     VALIDATE_NOT_NULL(has);
     *has = Animation::HasAlpha();
     return NOERROR;
}

ECode AppWindowAnimator::DummyAnimation::Clone(
    /* [out] */ IAnimation** animation)
{
    VALIDATE_NOT_NULL(animation);
    AutoPtr<IAnimation> temp = Animation::Clone();
    *animation = temp;
    INTERFACE_ADDREF(*animation);
    return NOERROR;
}

AutoPtr<IAnimation> AppWindowAnimator::DummyAnimation::GetCloneInstance()
{
    AutoPtr<IAnimation> temp = new AppWindowAnimator::DummyAnimation();
    return temp;
}

ECode AppWindowAnimator::DummyAnimation::GetInvalidateRegion(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [in] */ IRectF* invalidate,
    /* [in] */ ITransformation* transformation)
{
    return Animation::GetInvalidateRegion(left, top, right, bottom, invalidate, transformation);
}

ECode AppWindowAnimator::DummyAnimation::InitializeInvalidateRegion(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return Animation::InitializeInvalidateRegion(left, top, right, bottom);
}

} // Wm
} // Server
} // Droid
} // Elastos
