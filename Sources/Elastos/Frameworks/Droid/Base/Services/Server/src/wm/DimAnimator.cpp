
#include "wm/DimAnimator.h"
#include "wm/CWindowManagerService.h"
#include "R.h"
#include <elastos/core/Thread.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Graphics::IPixelFormat;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::Utility::CTypedValue;
using Elastos::Droid::View::CSurface;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

const String DimAnimator::TAG("DimAnimator");

DimAnimator::Parameters::Parameters(
    /* [in] */ WindowStateAnimator* dimWinAnimator,
    /* [in] */ Int32 dimWidth,
    /* [in] */ Int32 dimHeight,
    /* [in] */ Float dimTarget)
    : mDimWinAnimator(dimWinAnimator)
    , mDimWidth(dimWidth)
    , mDimHeight(dimHeight)
    , mDimTarget(dimTarget)
{}

DimAnimator::Parameters::Parameters(
    /* [in] */ Parameters* o)
    : mDimWinAnimator(o->mDimWinAnimator)
    , mDimWidth(o->mDimWidth)
    , mDimHeight(o->mDimHeight)
    , mDimTarget(o->mDimTarget)
{}

DimAnimator::DimAnimator(
    /* [in] */ ISurfaceSession* session,
    /* [in] */ Int32 layerStack)
    : mDimShown(FALSE)
    , mDimCurrentAlpha(0)
    , mDimTargetAlpha(0)
    , mDimDeltaPerMs(0)
    , mLastDimAnimTime(0)
    , mLastDimWidth(0)
    , mLastDimHeight(0)
{
    // try {
    // if (WindowManagerService.DEBUG_SURFACE_TRACE) {
    //     mDimSurface = new WindowStateAnimator.SurfaceTrace(session,
    //         "DimAnimator",
    //         16, 16, PixelFormat.OPAQUE,
    //         Surface.FX_SURFACE_DIM | Surface.HIDDEN);
    // } else {
    ASSERT_SUCCEEDED(CSurface::New(session, String("DimAnimator"),
            16, 16, IPixelFormat::OPAQUE,
            ISurface::FX_SURFACE_DIM | ISurface::HIDDEN, (ISurface**)&mDimSurface));
    // }
    if (CWindowManagerService::SHOW_TRANSACTIONS ||
            CWindowManagerService::SHOW_SURFACE_ALLOC) {
        Slogger::I(CWindowManagerService::TAG, "  DIM %p: CREATE", mDimSurface.Get());
    }
    mDimSurface->SetLayerStack(layerStack);
    mDimSurface->SetAlpha(0.0f);
    mDimSurface->Show();
    // } catch (Exception e) {
    //     Slog.e(WindowManagerService.TAG, "Exception creating Dim surface", e);
    // }
}

void DimAnimator::UpdateParameters(
    /* [in] */ IResources* res,
    /* [in] */ Parameters* params,
    /* [in] */ Int64 currentTime)
{
    if (mDimSurface == NULL) {
        Slogger::E(TAG, "updateParameters: no Surface");
        return;
    }

    // Multiply by 1.5 so that rotating a frozen surface that includes this does not expose a
    // corner.
    Int32 dw = (Int32)(params->mDimWidth * 1.5);
    Int32 dh = (Int32)(params->mDimHeight * 1.5);
    AutoPtr<WindowStateAnimator> winAnimator = params->mDimWinAnimator;
    Float target = params->mDimTarget;
    if (!mDimShown) {
        if (CWindowManagerService::SHOW_TRANSACTIONS) {
            Slogger::I(CWindowManagerService::TAG,
                    "  DIM %p: SHOW pos=(0,0) (%dx%d)", mDimSurface.Get(), dw, dh);
        }
        mDimShown = TRUE;
        // try {
        mLastDimWidth = dw;
        mLastDimHeight = dh;
        // back off position so mDimXXX/4 is before and mDimXXX/4 is after
        mDimSurface->SetPosition(-1 * dw / 6, -1 * dh /6);
        mDimSurface->SetSize(dw, dh);
        ECode ec = mDimSurface->Show();
        if (FAILED(ec)) {
            Slogger::W(CWindowManagerService::TAG, "Failure showing dim surface 0x%08x", ec);
        }
        // } catch (RuntimeException e) {
        //     Slog.w(WindowManagerService.TAG, "Failure showing dim surface", e);
        // }
    }
    else if (mLastDimWidth != dw || mLastDimHeight != dh) {
        mLastDimWidth = dw;
        mLastDimHeight = dh;
        mDimSurface->SetSize(dw, dh);
        // back off position so mDimXXX/4 is before and mDimXXX/4 is after
        mDimSurface->SetPosition(-1 * dw / 6, -1 * dh /6);
    }

    mDimSurface->SetLayer(winAnimator->mAnimLayer - CWindowManagerService::LAYER_OFFSET_DIM);

    if (CWindowManagerService::SHOW_TRANSACTIONS) {
        Slogger::I(CWindowManagerService::TAG, "  DIM %p: layer=%d target=%d",
                mDimSurface.Get(), winAnimator->mAnimLayer - 1, target);
    }
    if (mDimTargetAlpha != target) {
        // If the desired dim level has changed, then
        // start an animation to it.
        mLastDimAnimTime = currentTime;
        Int64 duration;
        if (winAnimator->mAnimating && winAnimator->mAnimation != NULL) {
            winAnimator->mAnimation->ComputeDurationHint(&duration);
        }
        else {
            duration = CWindowManagerService::DEFAULT_DIM_DURATION;
        }
        if (target > mDimTargetAlpha) {
            AutoPtr<ITypedValue> tv;
            CTypedValue::New((ITypedValue**)&tv);
            res->GetValue(R::fraction::config_dimBehindFadeDuration,
                    tv, TRUE);
            Int32 type;
            tv->GetType(&type);
            if (type == ITypedValue::TYPE_FRACTION) {
                Float fraction;
                tv->GetFraction(duration, duration, &fraction);
                duration = (Int64)fraction;
            }
            else if (type >= ITypedValue::TYPE_FIRST_INT
                    && type <= ITypedValue::TYPE_LAST_INT) {
                tv->GetData((Int32*)&duration);
            }
        }
        if (duration < 1) {
            // Don't divide by zero
            duration = 1;
        }
        mDimTargetAlpha = target;
        mDimDeltaPerMs = (mDimTargetAlpha - mDimCurrentAlpha) / duration;
    }
}

Boolean DimAnimator::UpdateSurface(
    /* [in] */ Boolean dimming,
    /* [in] */ Int64 currentTime,
    /* [in] */ Boolean displayFrozen)
{
    if (mDimSurface == NULL) {
        Slogger::E(TAG, "updateSurface: no Surface");
        return FALSE;
    }

    if (!dimming) {
        if (mDimTargetAlpha != 0) {
            mLastDimAnimTime = currentTime;
            mDimTargetAlpha = 0;
            mDimDeltaPerMs = (-mDimCurrentAlpha) / CWindowManagerService::DEFAULT_DIM_DURATION;
        }
    }

    Boolean animating = mLastDimAnimTime != 0;
    if (animating) {
        mDimCurrentAlpha += mDimDeltaPerMs
                * (currentTime-mLastDimAnimTime);
        if (displayFrozen) {
            // If the display is frozen, there is no reason to animate.
            animating = FALSE;
        }
        else if (mDimDeltaPerMs > 0) {
            if (mDimCurrentAlpha > mDimTargetAlpha) {
                animating = FALSE;
            }
        }
        else if (mDimDeltaPerMs < 0) {
            if (mDimCurrentAlpha < mDimTargetAlpha) {
                animating = FALSE;
            }
        }
        else {
            animating = FALSE;
        }

        // Do we need to continue animating?
        if (animating) {
            if (CWindowManagerService::SHOW_TRANSACTIONS) {
                Slogger::I(CWindowManagerService::TAG, "  DIM %p: alpha=%f",
                        mDimSurface.Get(), mDimCurrentAlpha);
            }
            mLastDimAnimTime = currentTime;
            mDimSurface->SetAlpha(mDimCurrentAlpha);
        }
        else {
            mDimCurrentAlpha = mDimTargetAlpha;
            mLastDimAnimTime = 0;
            if (CWindowManagerService::SHOW_TRANSACTIONS) {
                Slogger::I(CWindowManagerService::TAG, "  DIM %p: final alpha=%f",
                        mDimSurface.Get(), mDimCurrentAlpha);
            }
            mDimSurface->SetAlpha(mDimCurrentAlpha);
            if (!dimming) {
                if (CWindowManagerService::SHOW_TRANSACTIONS) {
                    Slogger::I(CWindowManagerService::TAG, "  DIM %p: HIDE", mDimSurface.Get());
                }
                // try {
                ECode ec = mDimSurface->Hide();
                if (FAILED(ec)) {
                    Slogger::W(CWindowManagerService::TAG,
                            "Illegal argument exception hiding dim surface");
                }
                // } catch (RuntimeException e) {
                //     Slog.w(WindowManagerService.TAG, "Illegal argument exception hiding dim surface");
                // }
                mDimShown = FALSE;
            }
        }
    }
    return animating;
}

void DimAnimator::Kill()
{
    if (mDimSurface != NULL) {
        mDimSurface->Destroy();
        mDimSurface = NULL;
    }
}


} // Wm
} // Server
} // Droid
} // Elastos
