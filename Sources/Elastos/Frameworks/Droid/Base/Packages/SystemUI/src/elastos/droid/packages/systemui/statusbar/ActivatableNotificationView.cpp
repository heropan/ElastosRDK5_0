
#include "elastos/droid/packages/systemui/statusbar/ActivatableNotificationView.h"

namespace Elastos {
namespace Droid {
namespace Packages {
namespace SystemUI {
namespace StatusBar {

CAR_INTERFACE_IMPL(ActivatableNotificationView, ExpandableOutlineView, IActivatableNotificationView);
ActivatableNotificationView();

CARAPI constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    ExpandableOutlineView::constructor(context, attrs);
    mTouchSlop = ViewConfiguration.get(context).getScaledTouchSlop();
    mFastOutSlowInInterpolator =
            AnimationUtils.loadInterpolator(context, android.R.interpolator.fast_out_slow_in);
    mSlowOutFastInInterpolator = new PathInterpolator(0.8f, 0.0f, 0.6f, 1.0f);
    mLinearOutSlowInInterpolator =
            AnimationUtils.loadInterpolator(context, android.R.interpolator.linear_out_slow_in);
    mSlowOutLinearInInterpolator = new PathInterpolator(0.8f, 0.0f, 1.0f, 1.0f);
    mLinearInterpolator = new LinearInterpolator();
    setClipChildren(FALSE);
    setClipToPadding(FALSE);
    mAppearAnimationFilter = new PorterDuffColorFilter(0, PorterDuff.Mode.SRC_ATOP);
    mRoundedRectCornerRadius = getResources().getDimensionPixelSize(
            R.dimen.notification_material_rounded_rect_radius);
    mLegacyColor = getResources().getColor(R.color.notification_legacy_background_color);
    mNormalColor = getResources().getColor(R.color.notification_material_background_color);
    mLowPriorityColor = getResources().getColor(
            R.color.notification_material_background_low_priority_color);
    mTintedRippleColor = context.getResources().getColor(
            R.color.notification_ripple_tinted_color);
    mLowPriorityRippleColor = context.getResources().getColor(
            R.color.notification_ripple_color_low_priority);
    mNormalRippleColor = context.getResources().getColor(
            R.color.notification_ripple_untinted_color);
}

// @Override
protected CARAPI OnFinishInflate()
{
    super.onFinishInflate();
    mBackgroundNormal = (NotificationBackgroundView) findViewById(R.id.backgroundNormal);
    mBackgroundDimmed = (NotificationBackgroundView) findViewById(R.id.backgroundDimmed);
    mBackgroundNormal.setCustomBackground(R.drawable.notification_material_bg);
    mBackgroundDimmed.setCustomBackground(R.drawable.notification_material_bg_dim);
    updateBackground();
    updateBackgroundTint();
}

private final Runnable mTapTimeoutRunnable = new Runnable() {
    @Override
    public void run() {
        makeInactive(TRUE /* animate */);
    }
};

// @Override
CARAPI OnTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (mDimmed) {
        *result = HandleTouchEventDimmed(event);
        return NOERROR;
    }

    return super::OnTouchEvent(event, result);
}

// @Override
CARAPI DrawableHotspotChanged(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    if (!mDimmed){
        mBackgroundNormal.drawableHotspotChanged(x, y);
    }
    return NOERROR;
}

// @Override
protected CARAPI DrawableStateChanged()
{
    super.drawableStateChanged();
    if (mDimmed) {
        mBackgroundDimmed->SetState(getDrawableState());
    }
    else {
        mBackgroundNormal->SetState(getDrawableState());
    }
    return NOERROR;
}

private CARAPI_(Boolean) HandleTouchEventDimmed(
    /* [in] */ IMotionEvent* event)
{
    Int32 action = event.getActionMasked();
    switch (action) {
        case MotionEvent.ACTION_DOWN:
            mDownX = event.getX();
            mDownY = event.getY();
            if (mDownY > GetActualHeight()) {
                return FALSE;
            }
            break;
        case MotionEvent.ACTION_MOVE:
            if (!isWithinTouchSlop(event)) {
                makeInactive(TRUE /* animate */);
                return FALSE;
            }
            break;
        case MotionEvent.ACTION_UP:
            if (isWithinTouchSlop(event)) {
                if (!mActivated) {
                    makeActive();
                    postDelayed(mTapTimeoutRunnable, DOUBLETAP_TIMEOUT_MS);
                } else {
                    Boolean performed = performClick();
                    if (performed) {
                        removeCallbacks(mTapTimeoutRunnable);
                    }
                }
            } else {
                makeInactive(TRUE /* animate */);
            }
            break;
        case MotionEvent.ACTION_CANCEL:
            makeInactive(TRUE /* animate */);
            break;
        default:
            break;
    }
    return TRUE;
}

private CARAPI_(void) MakeActive()
{
    startActivateAnimation(FALSE /* reverse */);
    mActivated = TRUE;
    if (mOnActivatedListener != NULL) {
        mOnActivatedListener.onActivated(this);
    }
}

private CARAPI_(void) StartActivateAnimation(
    /* [in] */ Boolean reverse)
{
    if (!isAttachedToWindow()) {
        return;
    }
    Int32 widthHalf = mBackgroundNormal.GetWidth()/2;
    Int32 heightHalf = mBackgroundNormal.GetActualHeight()/2;
    Float radius = (Float) Math.sqrt(widthHalf*widthHalf + heightHalf*heightHalf);
    Animator animator;
    if (reverse) {
        animator = ViewAnimationUtils.createCircularReveal(mBackgroundNormal,
                widthHalf, heightHalf, radius, 0);
    } else {
        animator = ViewAnimationUtils.createCircularReveal(mBackgroundNormal,
                widthHalf, heightHalf, 0, radius);
    }
    mBackgroundNormal.setVisibility(View.VISIBLE);
    Interpolator interpolator;
    Interpolator alphaInterpolator;
    if (!reverse) {
        interpolator = mLinearOutSlowInInterpolator;
        alphaInterpolator = mLinearOutSlowInInterpolator;
    } else {
        interpolator = ACTIVATE_INVERSE_INTERPOLATOR;
        alphaInterpolator = ACTIVATE_INVERSE_ALPHA_INTERPOLATOR;
    }
    animator.setInterpolator(interpolator);
    animator.setDuration(ACTIVATE_ANIMATION_LENGTH);
    if (reverse) {
        mBackgroundNormal.setAlpha(1f);
        animator.addListener(new AnimatorListenerAdapter() {
            @Override
            public void onAnimationEnd(Animator animation) {
                if (mDimmed) {
                    mBackgroundNormal.setVisibility(View.INVISIBLE);
                }
            }
        });
        animator.start();
    } else {
        mBackgroundNormal.setAlpha(0.4f);
        animator.start();
    }
    mBackgroundNormal.animate()
            .alpha(reverse ? 0f : 1f)
            .setInterpolator(alphaInterpolator)
            .setDuration(ACTIVATE_ANIMATION_LENGTH);
}

/**
 * Cancels the hotspot and makes the notification inactive.
 */
CARAPI MakeInactive(
    /* [in] */ Boolean animate)
{
    if (mActivated) {
        if (mDimmed) {
            if (animate) {
                startActivateAnimation(TRUE /* reverse */);
            }
            else {
                mBackgroundNormal.setVisibility(View.INVISIBLE);
            }
        }
        mActivated = FALSE;
    }
    if (mOnActivatedListener != NULL) {
        mOnActivatedListener.onActivationReset(this);
    }
    removeCallbacks(mTapTimeoutRunnable);
}

private CARAPI_(Boolean) IsWithinTouchSlop(
    /* [in] */ IMotionEvent* event)
{
    return Math.abs(event.getX() - mDownX) < mTouchSlop
            && Math.abs(event.getY() - mDownY) < mTouchSlop;
}

CARAPI SetDimmed(
    /* [in] */ Boolean dimmed,
    /* [in] */ Boolean fade)
{
    if (mDimmed != dimmed) {
        mDimmed = dimmed;
        if (fade) {
            fadeBackground();
        }
        else {
            updateBackground();
        }
    }
}

CARAPI SetDark(
    /* [in] */ Boolean dark,
    /* [in] */ Boolean fade)
{
    // TODO implement fade
    if (mDark != dark) {
        mDark = dark;
        if (mDark) {
            setLayerType(View.LAYER_TYPE_HARDWARE, mDarkPaint);
        }
        else {
            setLayerType(View.LAYER_TYPE_NONE, NULL);
        }
    }
}

private static CARAPI_(AutoPtr<IPaint>) CreateDarkPaint()
{
    final Paint p = new Paint();
    final Float[] invert = {
        -1f,  0f,  0f, 1f, 1f,
         0f, -1f,  0f, 1f, 1f,
         0f,  0f, -1f, 1f, 1f,
         0f,  0f,  0f, 1f, 0f
    };
    final ColorMatrix m = new ColorMatrix(invert);
    final ColorMatrix grayscale = new ColorMatrix();
    grayscale.setSaturation(0);
    m.preConcat(grayscale);
    p.setColorFilter(new ColorMatrixColorFilter(m));
    return p;
}

CARAPI SetShowingLegacyBackground(
    /* [in] */ Boolean showing)
{
    mShowingLegacyBackground = showing;
    UpdateBackgroundTint();
    return NOERROR;
}

// @Override
CARAPI SetBelowSpeedBump(
    /* [in] */ Boolean below)
{
    super.setBelowSpeedBump(below);
    if (below != mIsBelowSpeedBump) {
        mIsBelowSpeedBump = below;
        UpdateBackgroundTint();
    }
    return NOERROR;
}

/**
 * Sets the tint color of the background
 */
CARAPI SetTintColor(
    /* [in] */ Int32 color)
{
    mBgTint = color;
    UpdateBackgroundTint();
    return NOERROR;
}

private CARAPI_(void) UpdateBackgroundTint()
{
    Int32 color = getBackgroundColor();
    Int32 rippleColor = getRippleColor();
    if (color == mNormalColor) {
        // We don't need to tint a normal notification
        color = 0;
    }
    mBackgroundDimmed.setTint(color);
    mBackgroundNormal.setTint(color);
    mBackgroundDimmed.setRippleColor(rippleColor);
    mBackgroundNormal.setRippleColor(rippleColor);
}

private CARAPI_(void) FadeBackground()
{
    mBackgroundNormal.animate().cancel();
    if (mDimmed) {
        mBackgroundDimmed.setVisibility(View.VISIBLE);
    } else {
        mBackgroundNormal.setVisibility(View.VISIBLE);
    }
    Float startAlpha = mDimmed ? 1f : 0;
    Float endAlpha = mDimmed ? 0 : 1f;
    Int32 duration = BACKGROUND_ANIMATION_LENGTH_MS;
    // Check whether there is already a background animation running.
    if (mBackgroundAnimator != NULL) {
        startAlpha = (Float) mBackgroundAnimator.getAnimatedValue();
        duration = (Int32) mBackgroundAnimator.getCurrentPlayTime();
        mBackgroundAnimator.removeAllListeners();
        mBackgroundAnimator.cancel();
        if (duration <= 0) {
            updateBackground();
            return;
        }
    }
    mBackgroundNormal.setAlpha(startAlpha);
    mBackgroundAnimator =
            ObjectAnimator.ofFloat(mBackgroundNormal, View.ALPHA, startAlpha, endAlpha);
    mBackgroundAnimator.setInterpolator(mFastOutSlowInInterpolator);
    mBackgroundAnimator.setDuration(duration);
    mBackgroundAnimator.addListener(new AnimatorListenerAdapter() {
        @Override
        public void onAnimationEnd(Animator animation) {
            if (mDimmed) {
                mBackgroundNormal.setVisibility(View.INVISIBLE);
            } else {
                mBackgroundDimmed.setVisibility(View.INVISIBLE);
            }
            mBackgroundAnimator = NULL;
        }
    });
    mBackgroundAnimator.start();
}

private CARAPI_(void) UpdateBackground()
{
    if (mDimmed) {
        mBackgroundDimmed.setVisibility(View.VISIBLE);
        mBackgroundNormal.setVisibility(View.INVISIBLE);
    } else {
        cancelFadeAnimations();
        mBackgroundDimmed.setVisibility(View.INVISIBLE);
        mBackgroundNormal.setVisibility(View.VISIBLE);
        mBackgroundNormal.setAlpha(1f);
        removeCallbacks(mTapTimeoutRunnable);
    }
}

private CARAPI_(void) CancelFadeAnimations()
{
    if (mBackgroundAnimator != NULL) {
        mBackgroundAnimator.cancel();
    }
    mBackgroundNormal.animate().cancel();
}

// @Override
protected CARAPI OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    super.onLayout(changed, left, top, right, bottom);
    SetPivotX(GetWidth() / 2);
    return NOERROR;
}

// @Override
CARAPI SetActualHeight(
    /* [in] */ Int32 actualHeight,
    /* [in] */ Boolean notifyListeners)
{
    super.setActualHeight(actualHeight, notifyListeners);
    setPivotY(actualHeight / 2);
    mBackgroundNormal->SetActualHeight(actualHeight);
    mBackgroundDimmed->SetActualHeight(actualHeight);
    return NOERROR;
}

// @Override
CARAPI SetClipTopAmount(
    /* [in] */ Int32 clipTopAmount)
{
    super.setClipTopAmount(clipTopAmount);
    mBackgroundNormal.setClipTopAmount(clipTopAmount);
    mBackgroundDimmed.setClipTopAmount(clipTopAmount);
    return NOERROR;
}

// @Override
CARAPI PerformRemoveAnimation(
    /* [in] */ Int64 duration,
    /* [in] */ Float translationDirection,
    /* [in] */ IRunnable* onFinishedRunnable)
{
    EnableAppearDrawing(TRUE);
    if (mDrawingAppearAnimation) {
        StartAppearAnimation(FALSE /* isAppearing */, translationDirection,
                0, duration, onFinishedRunnable);
    }
    else if (onFinishedRunnable != NULL) {
        onFinishedRunnable.run();
    }
    return NOERROR;
}

// @Override
CARAPI PerformAddAnimation(
    /* [in] */ Int64 delay,
    /* [in] */ Int64 duration)
{
    EnableAppearDrawing(TRUE);
    if (mDrawingAppearAnimation) {
        StartAppearAnimation(TRUE /* isAppearing */, -1.0f, delay, duration, NULL);
    }
    return NOERROR;
}

private CARAPI_(void) StartAppearAnimation(
    /* [in] */ Boolean isAppearing,
    /* [in] */ Float translationDirection,
    /* [in] */ Int64 delay,
    /* [in] */ Int64 duration,
    /* [in] */ IRunnable* onFinishedRunnable)
{
    if (mAppearAnimator != NULL) {
        mAppearAnimator.cancel();
    }
    mAnimationTranslationY = translationDirection * mActualHeight;
    if (mAppearAnimationFraction == -1.0f) {
        // not initialized yet, we start anew
        if (isAppearing) {
            mAppearAnimationFraction = 0.0f;
            mAppearAnimationTranslation = mAnimationTranslationY;
        } else {
            mAppearAnimationFraction = 1.0f;
            mAppearAnimationTranslation = 0;
        }
    }

    Float targetValue;
    if (isAppearing) {
        mCurrentAppearInterpolator = mSlowOutFastInInterpolator;
        mCurrentAlphaInterpolator = mLinearOutSlowInInterpolator;
        targetValue = 1.0f;
    } else {
        mCurrentAppearInterpolator = mFastOutSlowInInterpolator;
        mCurrentAlphaInterpolator = mSlowOutLinearInInterpolator;
        targetValue = 0.0f;
    }
    mAppearAnimator = ValueAnimator.ofFloat(mAppearAnimationFraction,
            targetValue);
    mAppearAnimator.setInterpolator(mLinearInterpolator);
    mAppearAnimator.setDuration(
            (Int64) (duration * Math.abs(mAppearAnimationFraction - targetValue)));
    mAppearAnimator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
        @Override
        public void onAnimationUpdate(ValueAnimator animation) {
            mAppearAnimationFraction = (Float) animation.getAnimatedValue();
            updateAppearAnimationAlpha();
            updateAppearRect();
            invalidate();
        }
    });
    if (delay > 0) {
        // we need to apply the initial state already to avoid drawn frames in the wrong state
        updateAppearAnimationAlpha();
        updateAppearRect();
        mAppearAnimator.setStartDelay(delay);
    }
    mAppearAnimator.addListener(new AnimatorListenerAdapter() {
        private Boolean mWasCancelled;

        @Override
        public void onAnimationEnd(Animator animation) {
            if (onFinishedRunnable != NULL) {
                onFinishedRunnable.run();
            }
            if (!mWasCancelled) {
                mAppearAnimationFraction = -1;
                setOutlineRect(NULL);
                EnableAppearDrawing(FALSE);
            }
        }

        @Override
        public void onAnimationStart(Animator animation) {
            mWasCancelled = FALSE;
        }

        @Override
        public void onAnimationCancel(Animator animation) {
            mWasCancelled = TRUE;
        }
    });
    mAppearAnimator.start();
}

private CARAPI_(void) UpdateAppearRect()
{
    Float inverseFraction = (1.0f - mAppearAnimationFraction);
    Float translationFraction = mCurrentAppearInterpolator.getInterpolation(inverseFraction);
    Float translateYTotalAmount = translationFraction * mAnimationTranslationY;
    mAppearAnimationTranslation = translateYTotalAmount;

    // handle width animation
    Float widthFraction = (inverseFraction - (1.0f - HORIZONTAL_ANIMATION_START))
            / (HORIZONTAL_ANIMATION_START - HORIZONTAL_ANIMATION_END);
    widthFraction = Math.min(1.0f, Math.max(0.0f, widthFraction));
    widthFraction = mCurrentAppearInterpolator.getInterpolation(widthFraction);
    Float left = (GetWidth() * (0.5f - HORIZONTAL_COLLAPSED_REST_PARTIAL / 2.0f) *
            widthFraction);
    Float right = GetWidth() - left;

    // handle top animation
    Float heightFraction = (inverseFraction - (1.0f - VERTICAL_ANIMATION_START)) /
            VERTICAL_ANIMATION_START;
    heightFraction = Math.max(0.0f, heightFraction);
    heightFraction = mCurrentAppearInterpolator.getInterpolation(heightFraction);

    Float top;
    Float bottom;
    if (mAnimationTranslationY > 0.0f) {
        bottom = mActualHeight - heightFraction * mAnimationTranslationY * 0.1f
                - translateYTotalAmount;
        top = bottom * heightFraction;
    } else {
        top = heightFraction * (mActualHeight + mAnimationTranslationY) * 0.1f -
                translateYTotalAmount;
        bottom = mActualHeight * (1 - heightFraction) + top * heightFraction;
    }
    mAppearAnimationRect.set(left, top, right, bottom);
    setOutlineRect(left, top + mAppearAnimationTranslation, right,
            bottom + mAppearAnimationTranslation);
}

private CARAPI_(void) UpdateAppearAnimationAlpha()
{
    Int32 backgroundColor = getBackgroundColor();
    if (backgroundColor != -1) {
        Float contentAlphaProgress = mAppearAnimationFraction;
        contentAlphaProgress = contentAlphaProgress / (1.0f - ALPHA_ANIMATION_END);
        contentAlphaProgress = Math.min(1.0f, contentAlphaProgress);
        contentAlphaProgress = mCurrentAlphaInterpolator.getInterpolation(contentAlphaProgress);
        Int32 sourceColor = Color.argb((Int32) (255 * (1.0f - contentAlphaProgress)),
                Color.red(backgroundColor), Color.green(backgroundColor),
                Color.blue(backgroundColor));
        mAppearAnimationFilter.setColor(sourceColor);
        mAppearPaint.setColorFilter(mAppearAnimationFilter);
    }
}

private CARAPI_(Int32) GetBackgroundColor()
{
    if (mBgTint != 0) {
        return mBgTint;
    }
    else if (mShowingLegacyBackground) {
        return mLegacyColor;
    }
    else if (mIsBelowSpeedBump) {
        return mLowPriorityColor;
    }

    return mNormalColor;
}

private CARAPI_(Int32) GetRippleColor()
{
    if (mBgTint != 0) {
        return mTintedRippleColor;
    }
    else if (mShowingLegacyBackground) {
        return mTintedRippleColor;
    }
    else if (mIsBelowSpeedBump) {
        return mLowPriorityRippleColor;
    }
    return mNormalRippleColor;
}

/**
 * When we draw the appear animation, we render the view in a bitmap and render this bitmap
 * as a shader of a rect. This call creates the Bitmap and switches the drawing mode,
 * such that the normal drawing of the views does not happen anymore.
 *
 * @param enable Should it be enabled.
 */
private CARAPI_(void) EnableAppearDrawing(
    /* [in] */ Boolean enable)
{
    if (enable != mDrawingAppearAnimation) {
        if (enable) {
            if (GetWidth() == 0 || GetActualHeight() == 0) {
                // TODO: This should not happen, but it can during expansion. Needs
                // investigation
                return;
            }
            Bitmap bitmap = Bitmap.createBitmap(GetWidth(), GetActualHeight(),
                    Bitmap.Config.ARGB_8888);
            Canvas canvas = new Canvas(bitmap);
            draw(canvas);
            mAppearPaint.setShader(new BitmapShader(bitmap, Shader.TileMode.CLAMP,
                    Shader.TileMode.CLAMP));
        } else {
            mAppearPaint.setShader(NULL);
        }
        mDrawingAppearAnimation = enable;
        invalidate();
    }
}

// @Override
protected CARAPI_(void) DispatchDraw(
    /* [in] */ ICanvas* canvas)
{
    if (!mDrawingAppearAnimation) {
        super.dispatchDraw(canvas);
    }
    else {
        drawAppearRect(canvas);
    }
}

private CARAPI_(void) DrawAppearRect(
    /* [in] */ ICanvas* canvas)
{
    canvas.save();
    canvas.translate(0, mAppearAnimationTranslation);
    canvas.drawRoundRect(mAppearAnimationRect, mRoundedRectCornerRadius,
            mRoundedRectCornerRadius, mAppearPaint);
    canvas.restore();
}

CARAPI SetOnActivatedListener(
    /* [in] */ IActivatableNotificationViewOnActivatedListener* onActivatedListener)
{
    mOnActivatedListener = onActivatedListener;
}

CARAPI Reset()
{
    SetTintColor(0);
    SetShowingLegacyBackground(FALSE);
    SetBelowSpeedBump(FALSE);
    return NOERROR;
}

} // namespace StatusBar
} // namespace SystemUI
} // namespace Packages
} // namespace Droid
} // namespace Elastos
