
#include "wm/ScreenRotationAnimation.h"
#include "R.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/Math.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Os::CSystemProperties;
using Elastos::Droid::View::ISurfaceHelper;
using Elastos::Droid::View::CSurfaceHelper;
using Elastos::Droid::View::CSurface;
using Elastos::Droid::View::Animation::CTransformation;
using Elastos::Droid::View::Animation::CAnimationUtils;
using Elastos::Droid::View::Animation::IAnimationUtils;
using Elastos::Droid::Graphics::IPixelFormat;
using Elastos::Droid::Graphics::CMatrix;
using Elastos::Droid::Graphics::CRect;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

const String ScreenRotationAnimation::TAG("ScreenRotationAnimation");
const Boolean ScreenRotationAnimation::DEBUG_STATE = FALSE;
const Boolean ScreenRotationAnimation::DEBUG_TRANSFORMS = FALSE;
const Boolean ScreenRotationAnimation::TWO_PHASE_ANIMATION = FALSE;
const Boolean ScreenRotationAnimation::USE_CUSTOM_BLACK_FRAME = FALSE;
const Int32 ScreenRotationAnimation::FREEZE_LAYER
        = CWindowManagerService::TYPE_LAYER_MULTIPLIER * 200;
Int32 ScreenRotationAnimation::mHwrotation = 0;

ScreenRotationAnimation::ScreenRotationAnimation(
    /* [in] */ IContext* context,
    /* [in] */ IDisplay* display,
    /* [in] */ ISurfaceSession* session,
    /* [in] */ Boolean inTransaction,
    /* [in] */ Int32 originalWidth,
    /* [in] */ Int32 originalHeight,
    /* [in] */ Int32 originalRotation,
    /* [in] */ Int32 exitAnim,
    /* [in] */ Int32 enterAnim)
    : mContext(context)
    , mDisplay(display)
    , mExitAnimId(exitAnim)
    , mEnterAnimId(enterAnim)
    , mOriginalRotation(originalRotation)
    , mOriginalWidth(originalWidth)
    , mOriginalHeight(originalHeight)
    , mCurRotation(0)
    , mStarted(FALSE)
    , mAnimRunning(FALSE)
    , mFinishAnimReady(FALSE)
    , mFinishAnimStartTime(0)
    , mHalfwayPoint(0)
    , mMoreRotateEnter(FALSE)
    , mMoreRotateExit(FALSE)
    , mMoreRotateFrame(FALSE)
    , mMoreFinishEnter(FALSE)
    , mMoreFinishExit(FALSE)
    , mMoreFinishFrame(FALSE)
    , mMoreStartEnter(FALSE)
    , mMoreStartExit(FALSE)
    , mMoreStartFrame(FALSE)
{
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mStartExitTransformation));
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mStartEnterTransformation));
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mStartFrameTransformation));

    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mFinishExitTransformation));
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mFinishEnterTransformation));
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mFinishFrameTransformation));

    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mRotateExitTransformation));
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mRotateEnterTransformation));
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mRotateFrameTransformation));

    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mLastRotateExitTransformation));
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mLastRotateEnterTransformation));
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mLastRotateFrameTransformation));

    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mExitTransformation));
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mEnterTransformation));
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mFrameTransformation));

    ASSERT_SUCCEEDED(CMatrix::New((IMatrix**)&mFrameInitialMatrix));
    ASSERT_SUCCEEDED(CMatrix::New((IMatrix**)&mSnapshotInitialMatrix));
    ASSERT_SUCCEEDED(CMatrix::New((IMatrix**)&mSnapshotFinalMatrix));
    ASSERT_SUCCEEDED(CMatrix::New((IMatrix**)&mExitFrameFinalMatrix));
    ASSERT_SUCCEEDED(CMatrix::New((IMatrix**)&mTmpMatrix));

    mTmpFloats = ArrayOf<Float>::Alloc(9);

    // Screenshot does NOT include rotation!
    if (originalRotation == ISurface::ROTATION_90
            || originalRotation == ISurface::ROTATION_270) {
        mWidth = originalHeight;
        mHeight = originalWidth;
    }
    else {
        mWidth = originalWidth;
        mHeight = originalHeight;
    }

    AutoPtr<ISurfaceHelper> surfaceHelper;
    CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&surfaceHelper);
    if (!inTransaction) {
        // if (WindowManagerService.SHOW_LIGHT_TRANSACTIONS) Slog.i(WindowManagerService.TAG,
        //         ">>> OPEN TRANSACTION ScreenRotationAnimation");
        surfaceHelper->OpenTransaction();
    }

    // try {
    //     try {
    // if (WindowManagerService.DEBUG_SURFACE_TRACE) {
    //     mSurface = new SurfaceTrace(session, "FreezeSurface",
    //             mWidth, mHeight,
    //             PixelFormat.OPAQUE, ISurface::FX_SURFACE_SCREENSHOT | ISurface::HIDDEN);
    // }
    // else {
    if (FAILED(CSurface::New(session, String("FreezeSurface"),
            mWidth, mHeight,
            IPixelFormat::OPAQUE, ISurface::FX_SURFACE_SCREENSHOT | ISurface::HIDDEN,
            (ISurface**)&mSurface))) {
        goto failed;
    }
    // }
    Boolean isValid;
    mSurface->IsValid(&isValid);
    if (!isValid) {
        // Screenshot failed, punt.
        mSurface = NULL;
        if (!inTransaction) {
            surfaceHelper->CloseTransaction();
            // if (WindowManagerService.SHOW_LIGHT_TRANSACTIONS) Slog.i(WindowManagerService.TAG,
            //         "<<< CLOSE TRANSACTION ScreenRotationAnimation");
        }
        return;
    }
    Int32 stack;
    mDisplay->GetLayerStack(&stack);
    mSurface->SetLayerStack(stack);
    mSurface->SetLayer(FREEZE_LAYER + 1);
    mSurface->SetAlpha(0);
    mSurface->Show();
        // } catch (ISurface::OutOfResourcesException e) {
        //     Slog.w(TAG, "Unable to allocate freeze surface", e);
        // }
failed:
    // if (WindowManagerService.SHOW_TRANSACTIONS ||
    //         WindowManagerService.SHOW_SURFACE_ALLOC) Slog.i(WindowManagerService.TAG,
    //                 "  FREEZE " + mSurface + ": CREATE");

    SetRotationInTransaction(originalRotation);
    // } finally {
    if (!inTransaction) {
        surfaceHelper->CloseTransaction();
        // if (WindowManagerService.SHOW_LIGHT_TRANSACTIONS) Slog.i(WindowManagerService.TAG,
        //         "<<< CLOSE TRANSACTION ScreenRotationAnimation");
    }
    // }

    // AutoPtr<ISystemProperties> sysProp;
    // CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
    // String hwrotation;
    // sysProp->Get(String("ro.sf.hwrotation"), String("0"), &hwrotation);
    // if(String("180").Equals(hwrotation)){
    //     mHwrotation = ISurface::ROTATION_180;
    // }
    // else{
    //     mHwrotation = ISurface::ROTATION_0;
    // }
}

Boolean ScreenRotationAnimation::HasScreenshot()
{
    return mSurface != NULL;
}

Int32 ScreenRotationAnimation::DeltaRotation(
    /* [in] */ Int32 oldRotation,
    /* [in] */ Int32 newRotation)
{
    Int32 delta = newRotation - oldRotation;
    if (delta < 0) delta += 4;
    return delta;
}

void ScreenRotationAnimation::SetSnapshotTransformInTransaction(
    /* [in] */ IMatrix* matrix,
    /* [in] */ Float alpha)
{
    if (mSurface != NULL) {
        matrix->GetValues(mTmpFloats.Get());
        mSurface->SetPosition((*mTmpFloats)[IMatrix::MTRANS_X],
                (*mTmpFloats)[IMatrix::MTRANS_Y]);
        mSurface->SetMatrix(
                (*mTmpFloats)[IMatrix::MSCALE_X], (*mTmpFloats)[IMatrix::MSKEW_Y],
                (*mTmpFloats)[IMatrix::MSKEW_X], (*mTmpFloats)[IMatrix::MSCALE_Y]);
        mSurface->SetAlpha(alpha);
        // if (DEBUG_TRANSFORMS) {
        //     float[] srcPnts = new float[] { 0, 0, mWidth, mHeight };
        //     float[] dstPnts = new float[4];
        //     matrix.mapPoints(dstPnts, srcPnts);
        //     Slog.i(TAG, "Original  : (" + srcPnts[0] + "," + srcPnts[1]
        //             + ")-(" + srcPnts[2] + "," + srcPnts[3] + ")");
        //     Slog.i(TAG, "Transformed: (" + dstPnts[0] + "," + dstPnts[1]
        //             + ")-(" + dstPnts[2] + "," + dstPnts[3] + ")");
        // }
    }
}

void ScreenRotationAnimation::CreateRotationMatrix(
    /* [in] */ Int32 rotation,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ IMatrix* outMatrix)
{
    assert(outMatrix != NULL);
    Boolean result;
    switch (rotation) {
        case ISurface::ROTATION_0:
            outMatrix->Reset();
            break;
        case ISurface::ROTATION_90:
            outMatrix->SetRotate(90, 0, 0);
            outMatrix->PostTranslate(height, 0, &result);
            break;
        case ISurface::ROTATION_180:
            outMatrix->SetRotate(180, 0, 0);
            outMatrix->PostTranslate(width, height, &result);
            break;
        case ISurface::ROTATION_270:
            outMatrix->SetRotate(270, 0, 0);
            outMatrix->PostTranslate(0, width, &result);
            break;
    }
}

void ScreenRotationAnimation::SetRotationInTransaction(
    /* [in] */ Int32 rotation)
{
    mCurRotation = rotation;

    // Compute the transformation matrix that must be applied
    // to the snapshot to make it stay in the same original position
    // with the current screen rotation.
    Int32 delta = DeltaRotation(rotation, ISurface::ROTATION_0);
    CreateRotationMatrix(delta, mWidth, mHeight, mSnapshotInitialMatrix);

    // if (DEBUG_STATE) Slogger::V(TAG, "**** ROTATION: " + delta);
    SetSnapshotTransformInTransaction(mSnapshotInitialMatrix, 1.0f);
}

Boolean ScreenRotationAnimation::SetRotationInTransaction(
    /* [in] */ Int32 rotation,
    /* [in] */ ISurfaceSession* session,
    /* [in] */ Int64 maxAnimationDuration,
    /* [in] */ Float animationScale,
    /* [in] */ Int32 finalWidth,
    /* [in] */ Int32 finalHeight)
{
    SetRotationInTransaction(rotation);
    if (TWO_PHASE_ANIMATION) {
        return StartAnimation(session, maxAnimationDuration, animationScale,
                finalWidth, finalHeight, FALSE);
    }

    // Don't start animation yet.
    return FALSE;
}

Boolean ScreenRotationAnimation::StartAnimation(
    /* [in] */ ISurfaceSession* session,
    /* [in] */ Int64 maxAnimationDuration,
    /* [in] */ Float animationScale,
    /* [in] */ Int32 finalWidth,
    /* [in] */ Int32 finalHeight,
    /* [in] */ Boolean dismissing)
{
    if (mSurface == NULL) {
        // Can't do animation.
        return FALSE;
    }
    if (mStarted) {
        return TRUE;
    }

    mStarted = TRUE;

    Boolean firstStart = FALSE;

    // Figure out how the screen has moved from the original rotation.
    Int32 delta = DeltaRotation(mCurRotation, mOriginalRotation);

    if (TWO_PHASE_ANIMATION && mFinishExitAnimation == NULL
            && (!dismissing || delta != ISurface::ROTATION_0)) {
        // if (DEBUG_STATE) Slogger::V(TAG, "Creating start and finish animations");
        firstStart = TRUE;
        AutoPtr<IAnimationUtils> animationUtils;
        CAnimationUtils::AcquireSingleton((IAnimationUtils**)&animationUtils);
        animationUtils->LoadAnimation(mContext, R::anim::screen_rotate_start_exit,
                (IAnimation**)&mStartExitAnimation);
        animationUtils->LoadAnimation(mContext, R::anim::screen_rotate_start_enter,
                (IAnimation**)&mStartEnterAnimation);
        if (USE_CUSTOM_BLACK_FRAME) {
            animationUtils->LoadAnimation(mContext, R::anim::screen_rotate_start_frame,
                    (IAnimation**)&mStartFrameAnimation);
        }
        animationUtils->LoadAnimation(mContext, R::anim::screen_rotate_finish_exit,
                (IAnimation**)&mFinishExitAnimation);
        animationUtils->LoadAnimation(mContext, R::anim::screen_rotate_finish_enter,
                (IAnimation**)&mFinishEnterAnimation);
        if (USE_CUSTOM_BLACK_FRAME) {
            animationUtils->LoadAnimation(mContext, R::anim::screen_rotate_finish_frame,
                    (IAnimation**)&mFinishFrameAnimation);
        }
    }

    // if (DEBUG_STATE) Slogger::V(TAG, "Rotation delta: " + delta + " finalWidth="
    //         + finalWidth + " finalHeight=" + finalHeight
    //         + " origWidth=" + mOriginalWidth + " origHeight=" + mOriginalHeight);

    mRotateExitAnimation = NULL;
    mRotateEnterAnimation = NULL;

    Boolean customAnim;
    if (mExitAnimId != 0 && mEnterAnimId != 0) {
        customAnim = TRUE;
        AutoPtr<IAnimationUtils> animationUtils;
        CAnimationUtils::AcquireSingleton((IAnimationUtils**)&animationUtils);
        animationUtils->LoadAnimation(mContext, mExitAnimId,
                (IAnimation**)&mRotateExitAnimation);
        animationUtils->LoadAnimation(mContext, mEnterAnimId,
                (IAnimation**)&mRotateEnterAnimation);
    }
    else {
        customAnim = FALSE;
        AutoPtr<IAnimationUtils> animationUtils;
        CAnimationUtils::AcquireSingleton((IAnimationUtils**)&animationUtils);
        if (USE_CUSTOM_BLACK_FRAME) {
            mRotateFrameAnimation = NULL;
        }
        switch (delta) {
            case ISurface::ROTATION_0:
                animationUtils->LoadAnimation(mContext, R::anim::screen_rotate_0_exit,
                        (IAnimation**)&mRotateExitAnimation);
                animationUtils->LoadAnimation(mContext, R::anim::screen_rotate_0_enter,
                        (IAnimation**)&mRotateEnterAnimation);
                if (USE_CUSTOM_BLACK_FRAME) {
                    animationUtils->LoadAnimation(mContext, R::anim::screen_rotate_0_frame,
                            (IAnimation**)&mRotateFrameAnimation);
                }
                break;
            case ISurface::ROTATION_90:
                animationUtils->LoadAnimation(mContext, R::anim::screen_rotate_plus_90_exit,
                        (IAnimation**)&mRotateExitAnimation);
                animationUtils->LoadAnimation(mContext, R::anim::screen_rotate_plus_90_enter,
                        (IAnimation**)&mRotateEnterAnimation);
                if (USE_CUSTOM_BLACK_FRAME) {
                    animationUtils->LoadAnimation(mContext, R::anim::screen_rotate_plus_90_frame,
                            (IAnimation**)&mRotateFrameAnimation);
                }
                break;
            case ISurface::ROTATION_180:
                animationUtils->LoadAnimation(mContext, R::anim::screen_rotate_180_exit,
                        (IAnimation**)&mRotateExitAnimation);
                animationUtils->LoadAnimation(mContext, R::anim::screen_rotate_180_enter,
                        (IAnimation**)&mRotateEnterAnimation);
                if (USE_CUSTOM_BLACK_FRAME) {
                    animationUtils->LoadAnimation(mContext, R::anim::screen_rotate_180_frame,
                            (IAnimation**)&mRotateFrameAnimation);
                }
                break;
            case ISurface::ROTATION_270:
                animationUtils->LoadAnimation(mContext, R::anim::screen_rotate_minus_90_exit,
                        (IAnimation**)&mRotateExitAnimation);
                animationUtils->LoadAnimation(mContext, R::anim::screen_rotate_minus_90_enter,
                        (IAnimation**)&mRotateEnterAnimation);
                if (USE_CUSTOM_BLACK_FRAME) {
                    animationUtils->LoadAnimation(mContext, R::anim::screen_rotate_minus_90_frame,
                            (IAnimation**)&mRotateFrameAnimation);
                }
                break;
            default:
                assert(0);
        }
    }

    // Initialize the animations.  This is a hack, redefining what "parent"
    // means to allow supplying the last and next size.  In this definition
    // "%p" is the original (let's call it "previous") size, and "%" is the
    // screen's current/new size.
    if (TWO_PHASE_ANIMATION && firstStart) {
        // Compute partial steps between original and final sizes.  These
        // are used for the dimensions of the exiting and entering elements,
        // so they are never stretched too significantly.
        Int32 halfWidth = (finalWidth + mOriginalWidth) / 2;
        Int32 halfHeight = (finalHeight + mOriginalHeight) / 2;

        // if (DEBUG_STATE) Slogger::V(TAG, "Initializing start and finish animations");
        mStartEnterAnimation->Initialize(finalWidth, finalHeight,
                halfWidth, halfHeight);
        mStartExitAnimation->Initialize(halfWidth, halfHeight,
                mOriginalWidth, mOriginalHeight);
        mFinishEnterAnimation->Initialize(finalWidth, finalHeight,
                halfWidth, halfHeight);
        mFinishExitAnimation->Initialize(halfWidth, halfHeight,
                mOriginalWidth, mOriginalHeight);
        if (USE_CUSTOM_BLACK_FRAME) {
            mStartFrameAnimation->Initialize(finalWidth, finalHeight,
                    mOriginalWidth, mOriginalHeight);
            mFinishFrameAnimation->Initialize(finalWidth, finalHeight,
                    mOriginalWidth, mOriginalHeight);
        }
    }
    mRotateEnterAnimation->Initialize(finalWidth, finalHeight, mOriginalWidth, mOriginalHeight);
    mRotateExitAnimation->Initialize(finalWidth, finalHeight, mOriginalWidth, mOriginalHeight);
    if (USE_CUSTOM_BLACK_FRAME) {
        mRotateFrameAnimation->Initialize(finalWidth, finalHeight, mOriginalWidth,
                mOriginalHeight);
    }
    mAnimRunning = FALSE;
    mFinishAnimReady = FALSE;
    mFinishAnimStartTime = -1;

    if (TWO_PHASE_ANIMATION && firstStart) {
        mStartExitAnimation->RestrictDuration(maxAnimationDuration);
        mStartExitAnimation->ScaleCurrentDuration(animationScale);
        mStartEnterAnimation->RestrictDuration(maxAnimationDuration);
        mStartEnterAnimation->ScaleCurrentDuration(animationScale);
        mFinishExitAnimation->RestrictDuration(maxAnimationDuration);
        mFinishExitAnimation->ScaleCurrentDuration(animationScale);
        mFinishEnterAnimation->RestrictDuration(maxAnimationDuration);
        mFinishEnterAnimation->ScaleCurrentDuration(animationScale);
        if (USE_CUSTOM_BLACK_FRAME) {
            mStartFrameAnimation->RestrictDuration(maxAnimationDuration);
            mStartFrameAnimation->ScaleCurrentDuration(animationScale);
            mFinishFrameAnimation->RestrictDuration(maxAnimationDuration);
            mFinishFrameAnimation->ScaleCurrentDuration(animationScale);
        }
    }
    mRotateExitAnimation->RestrictDuration(maxAnimationDuration);
    mRotateExitAnimation->ScaleCurrentDuration(animationScale);
    mRotateEnterAnimation->RestrictDuration(maxAnimationDuration);
    mRotateEnterAnimation->ScaleCurrentDuration(animationScale);
    if (USE_CUSTOM_BLACK_FRAME) {
        mRotateFrameAnimation->RestrictDuration(maxAnimationDuration);
        mRotateFrameAnimation->ScaleCurrentDuration(animationScale);
    }

    Int32 layerStack;
    mDisplay->GetLayerStack(&layerStack);
    AutoPtr<ISurfaceHelper> surfaceHelper;
    CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&surfaceHelper);
    if (USE_CUSTOM_BLACK_FRAME && mCustomBlackFrame == NULL) {
        // if (WindowManagerService.SHOW_LIGHT_TRANSACTIONS || DEBUG_STATE) Slog.i(
        //         WindowManagerService.TAG,
        //         ">>> OPEN TRANSACTION ScreenRotationAnimation.startAnimation");
        surfaceHelper->OpenTransaction();

        // Compute the transformation matrix that must be applied
        // the the black frame to make it stay in the initial position
        // before the new screen rotation.  This is different than the
        // snapshot transformation because the snapshot is always based
        // of the native orientation of the screen, not the orientation
        // we were last in.
        CreateRotationMatrix(delta, mOriginalWidth, mOriginalHeight, mFrameInitialMatrix);

        // try {
        AutoPtr<IRect> outer;
        CRect::New(-mOriginalWidth * 1, -mOriginalHeight * 1,
                mOriginalWidth * 2, mOriginalHeight * 2, (IRect**)&outer);
        AutoPtr<IRect> inner;
        CRect::New(0, 0, mOriginalWidth, mOriginalHeight, (IRect**)&inner);
        mCustomBlackFrame = new BlackFrame(session, outer, inner, FREEZE_LAYER + 3,
                layerStack);
        mCustomBlackFrame->SetMatrix(mFrameInitialMatrix);
        // } catch (Surface.OutOfResourcesException e) {
        //     Slog.w(TAG, "Unable to allocate black surface", e);
        // } finally {
        //     Surface.closeTransaction();
        //     if (WindowManagerService.SHOW_LIGHT_TRANSACTIONS || DEBUG_STATE) Slog.i(
        //             WindowManagerService.TAG,
        //             "<<< CLOSE TRANSACTION ScreenRotationAnimation.startAnimation");
        // }
        surfaceHelper->CloseTransaction();
    }

    if (!customAnim && mExitingBlackFrame == NULL) {
        // if (WindowManagerService.SHOW_LIGHT_TRANSACTIONS || DEBUG_STATE) Slog.i(
        //         WindowManagerService.TAG,
        //         ">>> OPEN TRANSACTION ScreenRotationAnimation.startAnimation");
        surfaceHelper->OpenTransaction();
        // try {
        // Compute the transformation matrix that must be applied
        // the the black frame to make it stay in the initial position
        // before the new screen rotation.  This is different than the
        // snapshot transformation because the snapshot is always based
        // of the native orientation of the screen, not the orientation
        // we were last in.
        CreateRotationMatrix(delta, mOriginalWidth, mOriginalHeight, mFrameInitialMatrix);

        AutoPtr<IRect> outer;
        CRect::New(-mOriginalWidth * 1, -mOriginalHeight * 1,
                mOriginalWidth * 2, mOriginalHeight * 2, (IRect**)&outer);
        AutoPtr<IRect> inner;
        CRect::New(0, 0, mOriginalWidth, mOriginalHeight, (IRect**)&inner);
        mExitingBlackFrame = new BlackFrame(session, outer, inner, FREEZE_LAYER + 2,
                layerStack);
        mExitingBlackFrame->SetMatrix(mFrameInitialMatrix);
        // } catch (ISurface::OutOfResourcesException e) {
        //     Slog.w(TAG, "Unable to allocate black surface", e);
        // } finally {
        //     ISurface::closeTransaction();
        //     if (WindowManagerService.SHOW_LIGHT_TRANSACTIONS || DEBUG_STATE) Slog.i(
        //             WindowManagerService.TAG,
        //             "<<< CLOSE TRANSACTION ScreenRotationAnimation.startAnimation");
        // }
        surfaceHelper->CloseTransaction();
    }

    if (customAnim && mEnteringBlackFrame == NULL) {
        // if (WindowManagerService.SHOW_LIGHT_TRANSACTIONS || DEBUG_STATE) Slog.i(
        //         WindowManagerService.TAG,
        //         ">>> OPEN TRANSACTION ScreenRotationAnimation.startAnimation");
        surfaceHelper->OpenTransaction();

        // try {
        AutoPtr<IRect> outer;
        CRect::New(-finalWidth * 1, -finalHeight * 1,
                finalWidth * 2, finalHeight * 2, (IRect**)&outer);
        AutoPtr<IRect> inner;
        CRect::New(0, 0, finalWidth, finalHeight, (IRect**)&inner);
        mEnteringBlackFrame = new BlackFrame(session, outer, inner, FREEZE_LAYER,
                layerStack);
        // } catch (ISurface::OutOfResourcesException e) {
        //     Slog.w(TAG, "Unable to allocate black surface", e);
        // } finally {
        //     ISurface::closeTransaction();
        //     if (WindowManagerService.SHOW_LIGHT_TRANSACTIONS || DEBUG_STATE) Slog.i(
        //             WindowManagerService.TAG,
        //             "<<< CLOSE TRANSACTION ScreenRotationAnimation.startAnimation");
        // }
        surfaceHelper->CloseTransaction();
    }

    return TRUE;
}

Boolean ScreenRotationAnimation::Dismiss(
    /* [in] */ ISurfaceSession* session,
    /* [in] */ Int64 maxAnimationDuration,
    /* [in] */ Float animationScale,
    /* [in] */ Int32 finalWidth,
    /* [in] */ Int32 finalHeight)
{
    // if (DEBUG_STATE) Slogger::V(TAG, "Dismiss!");
    if (mSurface == NULL) {
        // Can't do animation.
        return FALSE;
    }
    if (!mStarted) {
        // if (DynamicPowerPolicyChangeThread.mGlobleLauncherRotateState) {
        //     DynamicPowerPolicyChangeThread.mGlobleLauncherRotateState = FALSE;
        // }
        // else
        {
            StartAnimation(session, maxAnimationDuration, animationScale,
                finalWidth, finalHeight, TRUE);
        }
        //startAnimation(session, maxAnimationDuration, animationScale, finalWidth, finalHeight,
          //      true);
    }
    if (!mStarted) {
        return FALSE;
    }
    // if (DEBUG_STATE) Slogger::V(TAG, "Setting mFinishAnimReady = true");
    mFinishAnimReady = TRUE;
    return TRUE;
}

void ScreenRotationAnimation::Kill()
{
    // if (DEBUG_STATE) Slogger::V(TAG, "Kill!");
    if (mSurface != NULL) {
        if (CWindowManagerService::SHOW_TRANSACTIONS ||
                CWindowManagerService::SHOW_SURFACE_ALLOC) {
            Slogger::I(CWindowManagerService::TAG, "  FREEZE %p: DESTROY", mSurface.Get());
        }
        mSurface->Destroy();
        mSurface = NULL;
    }
    if (mCustomBlackFrame != NULL) {
        mCustomBlackFrame->Kill();
        mCustomBlackFrame = NULL;
    }
    if (mExitingBlackFrame != NULL) {
        mExitingBlackFrame->Kill();
        mExitingBlackFrame = NULL;
    }
    if (mEnteringBlackFrame != NULL) {
        mEnteringBlackFrame->Kill();
        mEnteringBlackFrame = NULL;
    }
    if (TWO_PHASE_ANIMATION) {
        if (mStartExitAnimation != NULL) {
            mStartExitAnimation->Cancel();
            mStartExitAnimation = NULL;
        }
        if (mStartEnterAnimation != NULL) {
            mStartEnterAnimation->Cancel();
            mStartEnterAnimation = NULL;
        }
        if (mFinishExitAnimation != NULL) {
            mFinishExitAnimation->Cancel();
            mFinishExitAnimation = NULL;
        }
        if (mFinishEnterAnimation != NULL) {
            mFinishEnterAnimation->Cancel();
            mFinishEnterAnimation = NULL;
        }
    }
    if (USE_CUSTOM_BLACK_FRAME) {
        if (mStartFrameAnimation != NULL) {
            mStartFrameAnimation->Cancel();
            mStartFrameAnimation = NULL;
        }
        if (mRotateFrameAnimation != NULL) {
            mRotateFrameAnimation->Cancel();
            mRotateFrameAnimation = NULL;
        }
        if (mFinishFrameAnimation != NULL) {
            mFinishFrameAnimation->Cancel();
            mFinishFrameAnimation = NULL;
        }
    }
    if (mRotateExitAnimation != NULL) {
        mRotateExitAnimation->Cancel();
        mRotateExitAnimation = NULL;
    }
    if (mRotateEnterAnimation != NULL) {
        mRotateEnterAnimation->Cancel();
        mRotateEnterAnimation = NULL;
    }
}

Boolean ScreenRotationAnimation::IsAnimating()
{
    return HasAnimations() || (TWO_PHASE_ANIMATION && mFinishAnimReady);
}

Boolean ScreenRotationAnimation::IsRotating()
{
    return mCurRotation != mOriginalRotation;
}

Boolean ScreenRotationAnimation::HasAnimations()
{
    return (TWO_PHASE_ANIMATION &&
                (mStartEnterAnimation != NULL || mStartExitAnimation != NULL
                || mFinishEnterAnimation != NULL || mFinishExitAnimation != NULL))
            || (USE_CUSTOM_BLACK_FRAME &&
                    (mStartFrameAnimation != NULL || mRotateFrameAnimation != NULL
                    || mFinishFrameAnimation != NULL))
            || mRotateEnterAnimation != NULL || mRotateExitAnimation != NULL;
}

Boolean ScreenRotationAnimation::StepAnimation(
    /* [in] */ Int64 now)
{
    if (now > mHalfwayPoint) {
        mHalfwayPoint = Elastos::Core::Math::INT64_MAX_VALUE;
    }
    if (mFinishAnimReady && mFinishAnimStartTime < 0) {
        if (DEBUG_STATE) Slogger::V(TAG, "Step: finish anim now ready");
        mFinishAnimStartTime = now;
    }

    if (TWO_PHASE_ANIMATION) {
        mMoreStartExit = FALSE;
        if (mStartExitAnimation != NULL) {
            mStartExitAnimation->GetTransformation(now, mStartExitTransformation,
                    &mMoreStartExit);
            if (DEBUG_TRANSFORMS)
                Slogger::V(TAG, "Stepped start exit: %p", mStartExitTransformation.Get());
        }

        mMoreStartEnter = FALSE;
        if (mStartEnterAnimation != NULL) {
            mStartEnterAnimation->GetTransformation(now, mStartEnterTransformation
                    , &mMoreStartEnter);
            if (DEBUG_TRANSFORMS)
                Slogger::V(TAG, "Stepped start enter: %p", mStartEnterTransformation.Get());
        }
    }
    if (USE_CUSTOM_BLACK_FRAME) {
        mMoreStartFrame = FALSE;
        if (mStartFrameAnimation != NULL) {
            mStartFrameAnimation->GetTransformation(now, mStartFrameTransformation,
                    &mMoreStartFrame);
            // if (DEBUG_TRANSFORMS) Slogger::V(TAG, "Stepped start frame: " + mStartFrameTransformation);
        }
    }

    Int64 finishNow = mFinishAnimReady ? (now - mFinishAnimStartTime) : 0;
    if (DEBUG_STATE) Slogger::V(TAG, "Step: finishNow=%d", finishNow);

    if (TWO_PHASE_ANIMATION) {
        mMoreFinishExit = FALSE;
        if (mFinishExitAnimation != NULL) {
            mFinishExitAnimation->GetTransformation(finishNow, mFinishExitTransformation,
                    &mMoreFinishExit);
            if (DEBUG_TRANSFORMS)
                Slogger::V(TAG, "Stepped finish exit: %p", mFinishExitTransformation.Get());
        }

        mMoreFinishEnter = FALSE;
        if (mFinishEnterAnimation != NULL) {
            mFinishEnterAnimation->GetTransformation(finishNow, mFinishEnterTransformation,
                    &mMoreFinishEnter);
            if (DEBUG_TRANSFORMS)
                Slogger::V(TAG, "Stepped finish enter: %p", mFinishEnterTransformation.Get());
        }
    }
    if (USE_CUSTOM_BLACK_FRAME) {
        mMoreFinishFrame = FALSE;
        if (mFinishFrameAnimation != NULL) {
            mFinishFrameAnimation->GetTransformation(finishNow, mFinishFrameTransformation,
                    &mMoreFinishFrame);
            if (DEBUG_TRANSFORMS)
                Slogger::V(TAG, "Stepped finish frame: %p", mFinishFrameTransformation.Get());
        }
    }

    mMoreRotateExit = FALSE;
    if (mRotateExitAnimation != NULL) {
        mRotateExitAnimation->GetTransformation(now, mRotateExitTransformation,
                &mMoreRotateExit);
        if (DEBUG_TRANSFORMS)
            Slogger::V(TAG, "Stepped rotate exit: %p", mRotateExitTransformation.Get());
    }

    mMoreRotateEnter = FALSE;
    if (mRotateEnterAnimation != NULL) {
        mRotateEnterAnimation->GetTransformation(now, mRotateEnterTransformation,
                &mMoreRotateEnter);
        if (DEBUG_TRANSFORMS)
            Slogger::V(TAG, "Stepped rotate enter: %p", mRotateEnterTransformation.Get());
    }

    if (USE_CUSTOM_BLACK_FRAME) {
        mMoreRotateFrame = FALSE;
        if (mRotateFrameAnimation != NULL) {
            mRotateFrameAnimation->GetTransformation(now, mRotateFrameTransformation,
                    &mMoreRotateFrame);
            if (DEBUG_TRANSFORMS)
                Slogger::V(TAG, "Stepped rotate frame: %p", mRotateFrameTransformation.Get());
        }
    }

    if (!mMoreRotateExit && (!TWO_PHASE_ANIMATION || (!mMoreStartExit && !mMoreFinishExit))) {
        if (TWO_PHASE_ANIMATION) {
            if (mStartExitAnimation != NULL) {
                if (DEBUG_STATE) Slogger::V(TAG, "Exit animations done, clearing start exit anim!");
                mStartExitAnimation->Cancel();
                mStartExitAnimation = NULL;
                mStartExitTransformation->Clear();
            }
            if (mFinishExitAnimation != NULL) {
                if (DEBUG_STATE) Slogger::V(TAG, "Exit animations done, clearing finish exit anim!");
                mFinishExitAnimation->Cancel();
                mFinishExitAnimation = NULL;
                mFinishExitTransformation->Clear();
            }
        }
        if (mRotateExitAnimation != NULL) {
            if (DEBUG_STATE) Slogger::V(TAG, "Exit animations done, clearing rotate exit anim!");
            mRotateExitAnimation->Cancel();
            mRotateExitAnimation = NULL;
            mRotateExitTransformation->Clear();
        }
    }

    if (!mMoreRotateEnter && (!TWO_PHASE_ANIMATION || (!mMoreStartEnter && !mMoreFinishEnter))) {
        if (TWO_PHASE_ANIMATION) {
            if (mStartEnterAnimation != NULL) {
                if (DEBUG_STATE) Slogger::V(TAG, "Enter animations done, clearing start enter anim!");
                mStartEnterAnimation->Cancel();
                mStartEnterAnimation = NULL;
                mStartEnterTransformation->Clear();
            }
            if (mFinishEnterAnimation != NULL) {
                if (DEBUG_STATE) Slogger::V(TAG, "Enter animations done, clearing finish enter anim!");
                mFinishEnterAnimation->Cancel();
                mFinishEnterAnimation = NULL;
                mFinishEnterTransformation->Clear();
            }
        }
        if (mRotateEnterAnimation != NULL) {
            if (DEBUG_STATE) Slogger::V(TAG, "Enter animations done, clearing rotate enter anim!");
            mRotateEnterAnimation->Cancel();
            mRotateEnterAnimation = NULL;
            mRotateEnterTransformation->Clear();
        }
    }

    if (USE_CUSTOM_BLACK_FRAME && !mMoreStartFrame && !mMoreRotateFrame && !mMoreFinishFrame) {
        if (mStartFrameAnimation != NULL) {
            if (DEBUG_STATE) Slogger::V(TAG, "Frame animations done, clearing start frame anim!");
            mStartFrameAnimation->Cancel();
            mStartFrameAnimation = NULL;
            mStartFrameTransformation->Clear();
        }
        if (mFinishFrameAnimation != NULL) {
            if (DEBUG_STATE) Slogger::V(TAG, "Frame animations done, clearing finish frame anim!");
            mFinishFrameAnimation->Cancel();
            mFinishFrameAnimation = NULL;
            mFinishFrameTransformation->Clear();
        }
        if (mRotateFrameAnimation != NULL) {
            if (DEBUG_STATE) Slogger::V(TAG, "Frame animations done, clearing rotate frame anim!");
            mRotateFrameAnimation->Cancel();
            mRotateFrameAnimation = NULL;
            mRotateFrameTransformation->Clear();
        }
    }

    mExitTransformation->Set(mRotateExitTransformation);
    mEnterTransformation->Set(mRotateEnterTransformation);
    if (TWO_PHASE_ANIMATION) {
        mExitTransformation->Compose(mStartExitTransformation);
        mExitTransformation->Compose(mFinishExitTransformation);

        mEnterTransformation->Compose(mStartEnterTransformation);
        mEnterTransformation->Compose(mFinishEnterTransformation);
    }

    if (DEBUG_TRANSFORMS) Slogger::V(TAG, "Final exit: %p", mExitTransformation.Get());
    if (DEBUG_TRANSFORMS) Slogger::V(TAG, "Final enter: %p", mEnterTransformation.Get());

    if (USE_CUSTOM_BLACK_FRAME) {
        mFrameTransformation->Set(mRotateFrameTransformation);
        mFrameTransformation->Compose(mStartFrameTransformation);
        mFrameTransformation->Compose(mFinishFrameTransformation);
        AutoPtr<IMatrix> m;
        mFrameTransformation->GetMatrix((IMatrix**)&m);
        Boolean result;
        m->PreConcat(mFrameInitialMatrix, &result);
        if (DEBUG_TRANSFORMS) Slogger::V(TAG, "Final frame: ", mFrameTransformation.Get());
    }

    Boolean more = (TWO_PHASE_ANIMATION
                && (mMoreStartEnter || mMoreStartExit || mMoreFinishEnter || mMoreFinishExit))
            || (USE_CUSTOM_BLACK_FRAME
                    && (mMoreStartFrame || mMoreRotateFrame || mMoreFinishFrame))
            || mMoreRotateEnter || mMoreRotateExit
            || !mFinishAnimReady;

    AutoPtr<IMatrix> m;
    mExitTransformation->GetMatrix((IMatrix**)&m);
    Boolean result;
    mSnapshotFinalMatrix->SetConcat(m, mSnapshotInitialMatrix, &result);

    if (DEBUG_STATE) Slogger::V(TAG, "Step: more=%d", more);

    return more;
}

void ScreenRotationAnimation::UpdateSurfacesInTransaction()
{
    if (!mStarted) {
        return;
    }

    if (mSurface != NULL) {
        if (!mMoreStartExit && !mMoreFinishExit && !mMoreRotateExit) {
            // if (DEBUG_STATE) Slogger::V(TAG, "Exit animations done, hiding screenshot surface");
            mSurface->Hide();
        }
    }

    if (mCustomBlackFrame != NULL) {
        if (!mMoreStartFrame && !mMoreFinishFrame && !mMoreRotateFrame) {
            // if (DEBUG_STATE) Slogger::V(TAG, "Frame animations done, hiding black frame");
            mCustomBlackFrame->Hide();
        }
        else {
            AutoPtr<IMatrix> m;
            mFrameTransformation->GetMatrix((IMatrix**)&m);
            mCustomBlackFrame->SetMatrix(m);
        }
    }

    if (mExitingBlackFrame != NULL) {
        if (!mMoreStartExit && !mMoreFinishExit && !mMoreRotateExit) {
            // if (DEBUG_STATE) Slogger::V(TAG, "Frame animations done, hiding exiting frame");
            mExitingBlackFrame->Hide();
        }
        else {
            AutoPtr<IMatrix> m;
            mExitTransformation->GetMatrix((IMatrix**)&m);
            Boolean result;
            mExitFrameFinalMatrix->SetConcat(m, mFrameInitialMatrix, &result);
            mExitingBlackFrame->SetMatrix(mExitFrameFinalMatrix);
        }
    }

    if (mEnteringBlackFrame != NULL) {
        if (!mMoreStartEnter && !mMoreFinishEnter && !mMoreRotateEnter) {
            // if (DEBUG_STATE) Slogger::V(TAG, "Frame animations done, hiding entering frame");
            mEnteringBlackFrame->Hide();
        }
        else {
            AutoPtr<IMatrix> m;
            mEnterTransformation->GetMatrix((IMatrix**)&m);
            mEnteringBlackFrame->SetMatrix(m);
        }
    }

    Float alpha;
    mExitTransformation->GetAlpha(&alpha);
    SetSnapshotTransformInTransaction(mSnapshotFinalMatrix, alpha);
}

Boolean ScreenRotationAnimation::StepAnimationLocked(
    /* [in] */ Int64 now)
{
    if (!HasAnimations()) {
        // if (DEBUG_STATE) Slogger::V(TAG, "Step: no animations running");
        mFinishAnimReady = FALSE;
        return FALSE;
    }

    if (!mAnimRunning) {
        // if (DEBUG_STATE) Slogger::V(TAG, "Step: starting start, finish, rotate");
        if (TWO_PHASE_ANIMATION) {
            if (mStartEnterAnimation != NULL) {
                mStartEnterAnimation->SetStartTime(now);
            }
            if (mStartExitAnimation != NULL) {
                mStartExitAnimation->SetStartTime(now);
            }
            if (mFinishEnterAnimation != NULL) {
                mFinishEnterAnimation->SetStartTime(0);
            }
            if (mFinishExitAnimation != NULL) {
                mFinishExitAnimation->SetStartTime(0);
            }
        }
        if (USE_CUSTOM_BLACK_FRAME) {
            if (mStartFrameAnimation != NULL) {
                mStartFrameAnimation->SetStartTime(now);
            }
            if (mFinishFrameAnimation != NULL) {
                mFinishFrameAnimation->SetStartTime(0);
            }
            if (mRotateFrameAnimation != NULL) {
                mRotateFrameAnimation->SetStartTime(now);
            }
        }
        if (mRotateEnterAnimation != NULL) {
            mRotateEnterAnimation->SetStartTime(now);
        }
        if (mRotateExitAnimation != NULL) {
            mRotateExitAnimation->SetStartTime(now);
        }
        mAnimRunning = TRUE;
        Int64 duration;
        mRotateEnterAnimation->GetDuration(&duration);
        mHalfwayPoint = now + duration / 2;
    }

    return StepAnimation(now);
}

AutoPtr<ITransformation> ScreenRotationAnimation::GetEnterTransformation()
{
    return mEnterTransformation;
}


} // Wm
} // Server
} // Droid
} // Elastos
