
#ifndef __ELASTOS_DROID_SYSTEMUI_STATUSBAR_POLICY_KEYBUTTONRIPPLE_H__
#define __ELASTOS_DROID_SYSTEMUI_STATUSBAR_POLICY_KEYBUTTONRIPPLE_H__

#include "_SystemUI.h"
#include "Elastos.CoreLibrary.Utility.h"
#include "Elastos.Droid.Graphics.h"
#include "Elastos.Droid.View.h"
#include <elastos/droid/animation/AnimatorListenerAdapter.h>
#include <elastos/droid/ext/frameworkext.h>
#include <elastos/droid/graphics/drawable/Drawable.h>
#include <elastos/core/Object.h>

using Elastos::Droid::Animation::AnimatorListenerAdapter;
using Elastos::Droid::Animation::IAnimator;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::ICanvasProperty;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Graphics::Drawable::Drawable;
using Elastos::Droid::View::IHardwareCanvas;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::Animation::IInterpolator;
using Elastos::Core::Object;
using Elastos::Utility::IArrayList;
using Elastos::Utility::IHashSet;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Policy {

class KeyButtonRipple
    : public Drawable
    , public IKeyButtonRipple
{
private:
    /**
     * Interpolator with a smooth log deceleration
     */
    class LogInterpolator
        : public Object
        , public IInterpolator
    {
    public:
        CAR_INTERFACE_DECL();

        // @Override
        CARAPI GetInterpolation(
            /* [in] */ Float input,
            /* [out] */ Float* result);
    };

    class AnimatorListener: public AnimatorListenerAdapter
    {
    public:
        AnimatorListener(
            /* [in] */ KeyButtonRipple* host);

        // @Override
        CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animation);

    private:
        KeyButtonRipple* mHost;
    };

public:
    CAR_INTERFACE_DECL();

    KeyButtonRipple(
        /* [in] */ IContext* ctx,
        /* [in] */ IView* targetView);

    // @Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    // @Override
    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    // @Override
    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    // @Override
    CARAPI GetOpacity(
        /* [out] */ Int32* opacity);

    CARAPI GetGlowAlpha(
        /* [out] */ Float* result);

    CARAPI SetGlowAlpha(
        /* [in] */ Float x);

    CARAPI GetGlowScale(
        /* [out] */ Float* result);

    CARAPI SetGlowScale(
        /* [in] */ Float x);

    // @Override
    CARAPI IsStateful(
        /* [out] */ Boolean* result);

    CARAPI SetPressed(
        /* [in] */ Boolean pressed);

protected:
    // @Override
    CARAPI_(Boolean) OnStateChange(
        /* [in] */ const ArrayOf<Int32>* state);

private:
    CARAPI_(AutoPtr<IPaint>) GetRipplePaint();

    CARAPI_(void) DrawSoftware(
        /* [in] */ ICanvas* canvas);

    CARAPI_(Boolean) IsHorizontal();

    CARAPI_(void) DrawHardware(
        /* [in] */ IHardwareCanvas* c);

    CARAPI_(void) CancelAnimations();

    CARAPI_(void) SetPressedSoftware(
        /* [in] */ Boolean pressed);

    CARAPI_(void) EnterSoftware();

    CARAPI_(void) ExitSoftware();

    CARAPI_(void) SetPressedHardware(
        /* [in] */ Boolean pressed);

    /**
     * Sets the left/top property for the round rect to {@code prop} depending on whether we are
     * horizontal or vertical mode.
     */
    CARAPI_(void) SetExtendStart(
        /* [in] */ ICanvasProperty/*<Float>*/* prop);

    CARAPI_(AutoPtr<ICanvasProperty/*<Float*/>) GetExtendStart();

    /**
     * Sets the right/bottom property for the round rect to {@code prop} depending on whether we are
     * horizontal or vertical mode.
     */
    CARAPI_(void) SetExtendEnd(
        /* [in] */ ICanvasProperty/*<Float>*/* prop);

    CARAPI_(AutoPtr<ICanvasProperty/*<Float*/>) GetExtendEnd();

    CARAPI_(Int32) GetExtendSize();

    CARAPI_(Int32) GetRippleSize();

    CARAPI_(void) EnterHardware();

    CARAPI_(void) ExitHardware();

private:
    static const Float GLOW_MAX_SCALE_FACTOR;
    static const Float GLOW_MAX_ALPHA;
    static const Int32 ANIMATION_DURATION_SCALE;
    static const Int32 ANIMATION_DURATION_FADE;

    AutoPtr<IPaint> mRipplePaint;
    AutoPtr<ICanvasProperty/*<Float*/> mLeftProp;
    AutoPtr<ICanvasProperty/*<Float*/> mTopProp;
    AutoPtr<ICanvasProperty/*<Float*/> mRightProp;
    AutoPtr<ICanvasProperty/*<Float*/> mBottomProp;
    AutoPtr<ICanvasProperty/*<Float*/> mRxProp;
    AutoPtr<ICanvasProperty/*<Float*/> mRyProp;
    AutoPtr<ICanvasProperty/*<Paint*/> mPaintProp;
    Float mGlowAlpha;
    Float mGlowScale;
    Boolean mPressed;
    Boolean mDrawingHardwareGlow;
    Int32 mMaxWidth;

    AutoPtr<IInterpolator> mInterpolator;
    AutoPtr<IInterpolator> mAlphaExitInterpolator;
    Boolean mSupportHardware;
    AutoPtr<IView> mTargetView;

    AutoPtr<IHashSet/*<Animator*/> mRunningAnimations;
    AutoPtr<IArrayList/*<Animator*/> mTmpArray;
    AutoPtr<AnimatorListenerAdapter> mAnimatorListener;
};

} // namespace Policy
} // namespace StatusBar
} // namespace SystemUI
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SYSTEMUI_STATUSBAR_POLICY_KEYBUTTONRIPPLE_H__
