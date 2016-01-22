
#ifndef  __ELASTOS_DROID_PACKAGES_SYSTEMUI_STATUSBAR_ACTIVATABLENOTIFICATIONVIEW_H__
#define  __ELASTOS_DROID_PACKAGES_SYSTEMUI_STATUSBAR_ACTIVATABLENOTIFICATIONVIEW_H__

#include "elastos/droid/packages/systemui/statusbar/ExpandableOutlineView.h"

namespace Elastos {
namespace Droid {
namespace Packages {
namespace SystemUI {
namespace StatusBar {

/**
 * Base class for both {@link ExpandableNotificationRow} and {@link NotificationOverflowContainer}
 * to implement dimming/activating on Keyguard for the double-tap gesture
 */
class ActivatableNotificationView
    : public ExpandableOutlineView
    , public IActivatableNotificationView
{
public:
    CAR_INTERFACE_DECL();

    ActivatableNotificationView();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    // @Override
    protected CARAPI OnFinishInflate();

    private final Runnable mTapTimeoutRunnable = new Runnable() {
        @Override
        public void run() {
            makeInactive(TRUE /* animate */);
        }
    };

    // @Override
    CARAPI OnTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    // @Override
    CARAPI DrawableHotspotChanged(
        /* [in] */ Float x,
        /* [in] */ Float y);

    // @Override
    protected CARAPI DrawableStateChanged();

    private CARAPI_(Boolean) HandleTouchEventDimmed(
        /* [in] */ IMotionEvent* event);

    private CARAPI_(void) MakeActive();

    private CARAPI_(void) StartActivateAnimation(
        /* [in] */ Boolean reverse);

    /**
     * Cancels the hotspot and makes the notification inactive.
     */
    CARAPI MakeInactive(
        /* [in] */ Boolean animate);

    private CARAPI_(Boolean) IsWithinTouchSlop(
        /* [in] */ IMotionEvent* event);

    CARAPI SetDimmed(
        /* [in] */ Boolean dimmed,
        /* [in] */ Boolean fade);

    CARAPI SetDark(
        /* [in] */ Boolean dark,
        /* [in] */ Boolean fade);

    private static CARAPI_(AutoPtr<IPaint>) CreateDarkPaint();

    CARAPI SetShowingLegacyBackground(
        /* [in] */ Boolean showing);

    // @Override
    CARAPI SetBelowSpeedBump(
        /* [in] */ Boolean below);

    /**
     * Sets the tint color of the background
     */
    CARAPI SetTintColor(
        /* [in] */ Int32 color);

    private CARAPI_(void) UpdateBackgroundTint();

    private CARAPI_(void) FadeBackground();

    private CARAPI_(void) UpdateBackground();

    private CARAPI_(void) CancelFadeAnimations();

    // @Override
    protected CARAPI OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    // @Override
    CARAPI SetActualHeight(
        /* [in] */ Int32 actualHeight,
        /* [in] */ Boolean notifyListeners);

    // @Override
    CARAPI SetClipTopAmount(
        /* [in] */ Int32 clipTopAmount);

    // @Override
    CARAPI PerformRemoveAnimation(
        /* [in] */ Int64 duration,
        /* [in] */ Float translationDirection,
        /* [in] */ IRunnable* onFinishedRunnable);

    // @Override
    CARAPI PerformAddAnimation(
        /* [in] */ Int64 delay,
        /* [in] */ Int64 duration);

    private CARAPI_(void) StartAppearAnimation(
        /* [in] */ Boolean isAppearing,
        /* [in] */ Float translationDirection,
        /* [in] */ Int64 delay,
        /* [in] */ Int64 duration,
        /* [in] */ IRunnable* onFinishedRunnable);

    private CARAPI_(void) UpdateAppearRect();

    private CARAPI_(void) UpdateAppearAnimationAlpha();

    private CARAPI_(Int32) GetBackgroundColor();

    private CARAPI_(Int32) GetRippleColor();

    /**
     * When we draw the appear animation, we render the view in a bitmap and render this bitmap
     * as a shader of a rect. This call creates the Bitmap and switches the drawing mode,
     * such that the normal drawing of the views does not happen anymore.
     *
     * @param enable Should it be enabled.
     */
    private CARAPI_(void) EnableAppearDrawing(
        /* [in] */ Boolean enable);

    // @Override
    protected CARAPI_(void) DispatchDraw(
        /* [in] */ ICanvas* canvas);

    private CARAPI_(void) DrawAppearRect(
        /* [in] */ ICanvas* canvas);

    CARAPI SetOnActivatedListener(
        /* [in] */ IActivatableNotificationViewOnActivatedListener* onActivatedListener);

    CARAPI Reset();

private:
    private static final Int64 DOUBLETAP_TIMEOUT_MS = 1200;
    private static final Int32 BACKGROUND_ANIMATION_LENGTH_MS = 220;
    private static final Int32 ACTIVATE_ANIMATION_LENGTH = 220;

    /**
     * The amount of width, which is kept in the end when performing a disappear animation (also
     * the amount from which the horizontal appearing begins)
     */
    private static final Float HORIZONTAL_COLLAPSED_REST_PARTIAL = 0.05f;

    /**
     * At which point from [0,1] does the horizontal collapse animation end (or start when
     * expanding)? 1.0 meaning that it ends immediately and 0.0 that it is continuously animated.
     */
    private static final Float HORIZONTAL_ANIMATION_END = 0.2f;

    /**
     * At which point from [0,1] does the alpha animation end (or start when
     * expanding)? 1.0 meaning that it ends immediately and 0.0 that it is continuously animated.
     */
    private static final Float ALPHA_ANIMATION_END = 0.0f;

    /**
     * At which point from [0,1] does the horizontal collapse animation start (or start when
     * expanding)? 1.0 meaning that it starts immediately and 0.0 that it is animated at all.
     */
    private static final Float HORIZONTAL_ANIMATION_START = 1.0f;

    /**
     * At which point from [0,1] does the vertical collapse animation start (or end when
     * expanding) 1.0 meaning that it starts immediately and 0.0 that it is animated at all.
     */
    private static final Float VERTICAL_ANIMATION_START = 1.0f;

    private static final Interpolator ACTIVATE_INVERSE_INTERPOLATOR
            = new PathInterpolator(0.6f, 0, 0.5f, 1);
    private static final Interpolator ACTIVATE_INVERSE_ALPHA_INTERPOLATOR
            = new PathInterpolator(0, 0, 0.5f, 1);
    private final Int32 mTintedRippleColor;
    private final Int32 mLowPriorityRippleColor;
    private final Int32 mNormalRippleColor;

    private Boolean mDimmed;
    private Boolean mDark;
    private final Paint mDarkPaint = createDarkPaint();

    private Int32 mBgTint = 0;
    private final Int32 mRoundedRectCornerRadius;

    /**
     * Flag to indicate that the notification has been touched once and the second touch will
     * click it.
     */
    private Boolean mActivated;

    private Float mDownX;
    private Float mDownY;
    private final Float mTouchSlop;

    private OnActivatedListener mOnActivatedListener;

    private final Interpolator mLinearOutSlowInInterpolator;
    private final Interpolator mFastOutSlowInInterpolator;
    private final Interpolator mSlowOutFastInInterpolator;
    private final Interpolator mSlowOutLinearInInterpolator;
    private final Interpolator mLinearInterpolator;
    private Interpolator mCurrentAppearInterpolator;
    private Interpolator mCurrentAlphaInterpolator;

    private NotificationBackgroundView mBackgroundNormal;
    private NotificationBackgroundView mBackgroundDimmed;
    private ObjectAnimator mBackgroundAnimator;
    private RectF mAppearAnimationRect = new RectF();
    private PorterDuffColorFilter mAppearAnimationFilter;
    private Float mAnimationTranslationY;
    private Boolean mDrawingAppearAnimation;
    private Paint mAppearPaint = new Paint();
    private ValueAnimator mAppearAnimator;
    private Float mAppearAnimationFraction = -1.0f;
    private Float mAppearAnimationTranslation;
    private Boolean mShowingLegacyBackground;
    private final Int32 mLegacyColor;
    private final Int32 mNormalColor;
    private final Int32 mLowPriorityColor;
    private Boolean mIsBelowSpeedBump;
};

} // namespace StatusBar
} // namespace SystemUI
} // namespace Packages
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_PACKAGES_SYSTEMUI_STATUSBAR_ACTIVATABLENOTIFICATIONVIEW_H__
