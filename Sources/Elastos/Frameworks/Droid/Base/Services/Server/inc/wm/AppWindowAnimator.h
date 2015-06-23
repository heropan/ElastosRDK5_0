#ifndef __APPWINDOWANIMATOR_H__
#define __APPWINDOWANIMATOR_H__

#include "wm/CWindowManagerService.h"
#include "wm/WindowStateAnimator.h"
#include "wm/WindowAnimator.h"
#include "view/animation/Animation.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Droid::View::Animation::IAnimation;
using Elastos::Droid::View::Animation::ITransformation;
using Elastos::Droid::View::Animation::IAnimationListener;
using Elastos::Droid::View::Animation::IInterpolator;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

class AppWindowToken;
class WindowAnimator;
class WindowStateAnimator;

class AppWindowAnimator : public ElRefBase
{
public:
    // This is an animation that does nothing: it just immediately finishes
    // itself every time it is called.  It is used as a stub animation in cases
    // where we want to synchronize multiple things that may be animating.
    class DummyAnimation
        : public ElRefBase
        , public IAnimation
        , public Elastos::Droid::View::Animation::Animation
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI Reset();

        CARAPI Cancel();

        CARAPI Detach();

        CARAPI IsInitialized(
            /* [out] */ Boolean* isInitialized);

        CARAPI Initialize(
            /* [in] */ Int32 width,
            /* [in] */ Int32 height,
            /* [in] */ Int32 parentWidth,
            /* [in] */ Int32 parentHeight);

        CARAPI SetListenerHandler(
            /* [in] */ IHandler* handler);

        CARAPI SetInterpolator(
            /* [in] */ IContext* context,
            /* [in] */ Int32 resID);

        CARAPI SetInterpolator(
            /* [in] */ IInterpolator* i);

        CARAPI SetStartOffset(
            /* [in] */ Int64 startOffset);

        CARAPI SetDuration(
            /* [in] */ Int64 durationMillis);

        CARAPI RestrictDuration(
            /* [in] */ Int64 durationMillis);

        CARAPI ScaleCurrentDuration(
            /* [in] */ Float scale);

        CARAPI SetStartTime(
            /* [in] */ Int64 startTimeMillis);

        CARAPI Start();

        CARAPI StartNow();

        CARAPI SetRepeatMode(
            /* [in] */ Int32 repeatMode);

        CARAPI SetRepeatCount(
            /* [in] */ Int32 repeatCount);

        CARAPI IsFillEnabled(
            /* [out] */ Boolean* isFillEnabled);

        CARAPI SetFillEnabled(
            /* [in] */ Boolean fillEnabled);

        CARAPI SetFillBefore(
            /* [in] */ Boolean fillBefore);

        CARAPI SetFillAfter(
            /* [in] */ Boolean fillAfter);

        CARAPI SetZAdjustment(
            /* [in] */ Int32 zAdjustment);

        CARAPI SetBackgroundColor(
            /* [in] */ Int32 bg);

        CARAPI GetBackgroundColor(
            /* [out] */ Int32* bg);

        CARAPI SetDetachWallpaper(
            /* [in] */ Boolean detachWallpaper);

        CARAPI GetInterpolator(
            /* [out] */ IInterpolator** interpolator);

        CARAPI GetStartTime(
            /* [out] */ Int64* time);

        CARAPI GetDuration(
            /* [out] */ Int64* time);

        CARAPI GetStartOffset(
            /* [out] */ Int64* startOffset);

        CARAPI GetRepeatMode(
            /* [out] */ Int32* mode);

        CARAPI GetRepeatCount(
            /* [out] */ Int32* count);

        CARAPI GetFillBefore(
            /* [out] */ Boolean* result);

        CARAPI GetFillAfter(
            /* [out] */ Boolean* result);

        CARAPI GetZAdjustment(
            /* [out] */ Int32* zAdjustment);

        CARAPI GetDetachWallpaper(
            /* [out] */ Boolean* result);

        CARAPI WillChangeTransformationMatrix(
            /* [out] */ Boolean* result);

        CARAPI WillChangeBounds(
            /* [out] */ Boolean* result);

        CARAPI SetAnimationListener(
            /* [in] */ IAnimationListener* listener);

        CARAPI ComputeDurationHint(
            /* [out] */ Int64* hint);

        CARAPI GetTransformation(
            /* [in] */ Int64 currentTime,
            /* [in, out] */ ITransformation* outTransformation,
            /* [out] */ Boolean* result);

        CARAPI GetTransformation(
            /* [in] */ Int64 currentTime,
            /* [in, out] */ ITransformation* outTransformation,
            /* [in] */ Float scale,
            /* [out] */ Boolean* result);

        CARAPI HasStarted(
            /* [out] */ Boolean* result);

        CARAPI HasEnded(
            /* [out] */ Boolean* result);

        CARAPI HasAlpha(
            /* [out] */ Boolean* has);

        CARAPI  Clone(
            /* [out] */ IAnimation** animation);

        CARAPI GetInvalidateRegion(
            /* [in] */ Int32 left,
            /* [in] */ Int32 top,
            /* [in] */ Int32 right,
            /* [in] */ Int32 bottom,
            /* [in] */ IRectF* invalidate,
            /* [in] */ ITransformation* transformation);

        CARAPI InitializeInvalidateRegion(
            /* [in] */ Int32 left,
            /* [in] */ Int32 top,
            /* [in] */ Int32 right,
            /* [in] */ Int32 bottom);

    protected:
        CARAPI_(AutoPtr<IAnimation>) GetCloneInstance();
    };

public:
    AppWindowAnimator(
        /* [in] */ AppWindowToken* atoken);

    CARAPI Init();

    CARAPI_(void) SetAnimation(
        /* [in] */ IAnimation* anim,
        /* [in] */ Boolean initialized);

    CARAPI_(void) SetDummyAnimation();

    CARAPI_(void) ClearAnimation();

    CARAPI_(void) ClearThumbnail();

    CARAPI_(void) UpdateLayers();

    // This must be called while inside a transaction.
    CARAPI_(Boolean) StepAnimationLocked(
        /* [in] */ Int64 currentTime,
        /* [in] */ Int32 dw,
        /* [in] */ Int32 dh);

    CARAPI_(Boolean) ShowAllWindowsLocked();

    AutoPtr<AppWindowToken> GetAppToken();

private:
    CARAPI_(void) StepThumbnailAnimation(
        /* [in] */ Int64 currentTime);

    CARAPI_(Boolean) StepAnimation(
        /* [in] */ Int64 currentTime);

public:
    static const String TAG;

    AutoPtr<IWeakReference> mWeakAppToken; // AppWindowToken has AppWindowAnimator's ref
    AutoPtr<CWindowManagerService> mService;
    AutoPtr<WindowAnimator> mAnimator;

    Boolean mAnimating;
    AutoPtr<IAnimation> mAnimation;
    Boolean mAnimInitialized;
    Boolean mHasTransformation;
    AutoPtr<ITransformation> mTransformation;

    // Have we been asked to have this token keep the screen frozen?
    // Protect with mAnimator.
    Boolean mFreezingScreen;

    // Offset to the window of all layers in the token, for use by
    // AppWindowToken animations.
    Int32 mAnimLayerAdjustment;

    // Propagated from AppWindowToken.allDrawn, to determine when
    // the state changes.
    Boolean mAllDrawn;

    // Special surface for thumbnail animation.
    AutoPtr<ISurface> mThumbnail;
    Int32 mThumbnailTransactionSeq;
    Int32 mThumbnailX;
    Int32 mThumbnailY;
    Int32 mThumbnailLayer;
    AutoPtr<IAnimation> mThumbnailAnimation;
    AutoPtr<ITransformation> mThumbnailTransformation;

    /** WindowStateAnimator from mAppAnimator.allAppWindows as of last performLayout */
    List< AutoPtr<WindowStateAnimator> > mAllAppWinAnimators;

    static const AutoPtr<IAnimation> sDummyAnimation;

};

} // Wm
} // Server
} // Droid
} // Elastos

#endif //__APPWINDOWANIMATOR_H__
