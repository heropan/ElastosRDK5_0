// wuweizuo automatic build .cpp file from .java file.

#include "PopupZoomer.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//       PopupZoomer::InnerGestureDetectorSimpleOnGestureListener
//=====================================================================
PopupZoomer::InnerGestureDetectorSimpleOnGestureListener::InnerGestureDetectorSimpleOnGestureListener(
    /* [in] */ PopupZoomer* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

Boolean PopupZoomer::InnerGestureDetectorSimpleOnGestureListener::OnScroll(
    /* [in] */ IMotionEvent* e1,
    /* [in] */ IMotionEvent* e2,
    /* [in] */ Float distanceX,
    /* [in] */ Float distanceY)
{
    // ==================before translated======================
    // if (mAnimating) return true;
    //
    // if (isTouchOutsideArea(e1.getX(), e1.getY())) {
    //     hide(true);
    // } else {
    //     scroll(distanceX, distanceY);
    // }
    // return true;
    assert(0);
    return FALSE;
}

Boolean PopupZoomer::InnerGestureDetectorSimpleOnGestureListener::OnSingleTapUp(
    /* [in] */ IMotionEvent* e)
{
    // ==================before translated======================
    // return handleTapOrPress(e, false);
    assert(0);
    return FALSE;
}

ECode PopupZoomer::InnerGestureDetectorSimpleOnGestureListener::OnLongPress(
    /* [in] */ IMotionEvent* e)
{
    VALIDATE_NOT_NULL(e);
    // ==================before translated======================
    // handleTapOrPress(e, true);
    assert(0);
    return NOERROR;
}

Boolean PopupZoomer::InnerGestureDetectorSimpleOnGestureListener::HandleTapOrPress(
    /* [in] */ IMotionEvent* e,
    /* [in] */ Boolean isLongPress)
{
    // ==================before translated======================
    // if (mAnimating) return true;
    //
    // float x = e.getX();
    // float y = e.getY();
    // if (isTouchOutsideArea(x, y)) {
    //     // User clicked on area outside the popup.
    //     hide(true);
    // } else if (mOnTapListener != null) {
    //     PointF converted = convertTouchPoint(x, y);
    //     MotionEvent event = MotionEvent.obtainNoHistory(e);
    //     event.setLocation(converted.x, converted.y);
    //     if (isLongPress) {
    //         mOnTapListener.onLongPress(PopupZoomer.this, event);
    //     } else {
    //         mOnTapListener.onSingleTap(PopupZoomer.this, event);
    //     }
    //     hide(true);
    // }
    // return true;
    assert(0);
    return FALSE;
}

//=====================================================================
//                   PopupZoomer::ReverseInterpolator
//=====================================================================
PopupZoomer::ReverseInterpolator::ReverseInterpolator(
    /* [in] */ IInterpolator* i)
{
    // ==================before translated======================
    // mInterpolator = i;
}

Float PopupZoomer::ReverseInterpolator::GetInterpolation(
    /* [in] */ Float input)
{
    // ==================before translated======================
    // input = 1.0f - input;
    // if (mInterpolator == null) return input;
    // return mInterpolator.getInterpolation(input);
    assert(0);
    return 0.0f;
}

//=====================================================================
//                             PopupZoomer
//=====================================================================
const String PopupZoomer::LOGTAG("PopupZoomer");
const Int32 PopupZoomer::ZOOM_BOUNDS_MARGIN;
const Int64 PopupZoomer::ANIMATION_DURATION;
AutoPtr<IDrawable> PopupZoomer::sOverlayDrawable;
AutoPtr<IRect> PopupZoomer::sOverlayPadding;
Float PopupZoomer::sOverlayCornerRadius;

PopupZoomer::PopupZoomer(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // super(context);
    //
    // setVisibility(INVISIBLE);
    // setFocusable(true);
    // setFocusableInTouchMode(true);
    //
    // GestureDetector.SimpleOnGestureListener listener =
    //         new GestureDetector.SimpleOnGestureListener() {
    //             @Override
    //             public boolean onScroll(MotionEvent e1, MotionEvent e2,
    //                     float distanceX, float distanceY) {
    //                 if (mAnimating) return true;
    //
    //                 if (isTouchOutsideArea(e1.getX(), e1.getY())) {
    //                     hide(true);
    //                 } else {
    //                     scroll(distanceX, distanceY);
    //                 }
    //                 return true;
    //             }
    //
    //             @Override
    //             public boolean onSingleTapUp(MotionEvent e) {
    //                 return handleTapOrPress(e, false);
    //             }
    //
    //             @Override
    //             public void onLongPress(MotionEvent e) {
    //                 handleTapOrPress(e, true);
    //             }
    //
    //             private boolean handleTapOrPress(MotionEvent e, boolean isLongPress) {
    //                 if (mAnimating) return true;
    //
    //                 float x = e.getX();
    //                 float y = e.getY();
    //                 if (isTouchOutsideArea(x, y)) {
    //                     // User clicked on area outside the popup.
    //                     hide(true);
    //                 } else if (mOnTapListener != null) {
    //                     PointF converted = convertTouchPoint(x, y);
    //                     MotionEvent event = MotionEvent.obtainNoHistory(e);
    //                     event.setLocation(converted.x, converted.y);
    //                     if (isLongPress) {
    //                         mOnTapListener.onLongPress(PopupZoomer.this, event);
    //                     } else {
    //                         mOnTapListener.onSingleTap(PopupZoomer.this, event);
    //                     }
    //                     hide(true);
    //                 }
    //                 return true;
    //             }
    //         };
    // mGestureDetector = new GestureDetector(context, listener);
}

ECode PopupZoomer::SetOnTapListener(
    /* [in] */ OnTapListener* listener)
{
    VALIDATE_NOT_NULL(listener);
    // ==================before translated======================
    // mOnTapListener = listener;
    assert(0);
    return NOERROR;
}

ECode PopupZoomer::SetOnVisibilityChangedListener(
    /* [in] */ OnVisibilityChangedListener* listener)
{
    VALIDATE_NOT_NULL(listener);
    // ==================before translated======================
    // mOnVisibilityChangedListener = listener;
    assert(0);
    return NOERROR;
}

ECode PopupZoomer::SetBitmap(
    /* [in] */ IBitmap* bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    // ==================before translated======================
    // if (mZoomedBitmap != null) {
    //     mZoomedBitmap.recycle();
    //     mZoomedBitmap = null;
    // }
    // mZoomedBitmap = bitmap;
    //
    // // Round the corners of the bitmap so it doesn't stick out around the overlay.
    // Canvas canvas = new Canvas(mZoomedBitmap);
    // Path path = new Path();
    // RectF canvasRect = new RectF(0, 0, canvas.getWidth(), canvas.getHeight());
    // float overlayCornerRadius = getOverlayCornerRadius(getContext());
    // path.addRoundRect(canvasRect, overlayCornerRadius, overlayCornerRadius, Direction.CCW);
    // canvas.clipPath(path, Op.XOR);
    // Paint clearPaint = new Paint();
    // clearPaint.setXfermode(new PorterDuffXfermode(Mode.SRC));
    // clearPaint.setColor(Color.TRANSPARENT);
    // canvas.drawPaint(clearPaint);
    assert(0);
    return NOERROR;
}

Boolean PopupZoomer::IsShowing()
{
    // ==================before translated======================
    // return mShowing || mAnimating;
    assert(0);
    return FALSE;
}

ECode PopupZoomer::SetLastTouch(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    // ==================before translated======================
    // mTouch.x = x;
    // mTouch.y = y;
    assert(0);
    return NOERROR;
}

ECode PopupZoomer::Show(
    /* [in] */ IRect* rect)
{
    VALIDATE_NOT_NULL(rect);
    // ==================before translated======================
    // if (mShowing || mZoomedBitmap == null) return;
    //
    // setTargetBounds(rect);
    // startAnimation(true);
    assert(0);
    return NOERROR;
}

ECode PopupZoomer::Hide(
    /* [in] */ Boolean animation)
{
    // ==================before translated======================
    // if (!mShowing) return;
    //
    // if (animation) {
    //     startAnimation(false);
    // } else {
    //     hideImmediately();
    // }
    assert(0);
    return NOERROR;
}

Boolean PopupZoomer::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    // ==================before translated======================
    // mGestureDetector.onTouchEvent(event);
    // return true;
    assert(0);
    return FALSE;
}

Boolean PopupZoomer::AcceptZeroSizeView()
{
    // ==================before translated======================
    // return false;
    assert(0);
    return FALSE;
}

ECode PopupZoomer::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    VALIDATE_NOT_NULL(canvas);
    // ==================before translated======================
    // if (!isShowing() || mZoomedBitmap == null) return;
    // if (!acceptZeroSizeView() && (getWidth() == 0 || getHeight() == 0)) return;
    //
    // if (mNeedsToInitDimensions) {
    //     mNeedsToInitDimensions = false;
    //     initDimensions();
    // }
    //
    // canvas.save();
    // // Calculate the elapsed fraction of animation.
    // float time = (SystemClock.uptimeMillis() - mAnimationStartTime + mTimeLeft) /
    //         ((float) ANIMATION_DURATION);
    // time = constrain(time, 0, 1);
    // if (time >= 1) {
    //     mAnimating = false;
    //     if (!isShowing()) {
    //         hideImmediately();
    //         return;
    //     }
    // } else {
    //     invalidate();
    // }
    //
    // // Fraction of the animation to actally show.
    // float fractionAnimation;
    // if (mShowing) {
    //     fractionAnimation = mShowInterpolator.getInterpolation(time);
    // } else {
    //     fractionAnimation = mHideInterpolator.getInterpolation(time);
    // }
    //
    // // Draw a faded color over the entire view to fade out the original content, increasing
    // // the alpha value as fractionAnimation increases.
    // // TODO(nileshagrawal): We should use time here instead of fractionAnimation
    // // as fractionAnimaton is interpolated and can go over 1.
    // canvas.drawARGB((int) (80 * fractionAnimation), 0, 0, 0);
    // canvas.save();
    //
    // // Since we want the content to appear directly above its counterpart we need to make
    // // sure that it starts out at exactly the same size as it appears in the page,
    // // i.e. scale grows from 1/mScale to 1. Note that extrusion values are already zoomed
    // // with mScale.
    // float scale = fractionAnimation * (mScale - 1.0f) / mScale + 1.0f / mScale;
    //
    // // Since we want the content to appear directly above its counterpart on the
    // // page, we need to remove the mShiftX/Y effect at the beginning of the animation.
    // // The unshifting decreases with the animation.
    // float unshiftX = -mShiftX * (1.0f - fractionAnimation) / mScale;
    // float unshiftY = -mShiftY * (1.0f - fractionAnimation) / mScale;
    //
    // // Compute the rect to show.
    // RectF rect = new RectF();
    // rect.left = mTouch.x - mLeftExtrusion * scale + unshiftX;
    // rect.top = mTouch.y - mTopExtrusion * scale + unshiftY;
    // rect.right = mTouch.x + mRightExtrusion * scale + unshiftX;
    // rect.bottom = mTouch.y + mBottomExtrusion * scale + unshiftY;
    // canvas.clipRect(rect);
    //
    // // Since the canvas transform APIs all pre-concat the transformations, this is done in
    // // reverse order. The canvas is first scaled up, then shifted the appropriate amount of
    // // pixels.
    // canvas.scale(scale, scale, rect.left, rect.top);
    // canvas.translate(mPopupScrollX, mPopupScrollY);
    // canvas.drawBitmap(mZoomedBitmap, rect.left, rect.top, null);
    // canvas.restore();
    // Drawable overlayNineTile = getOverlayDrawable(getContext());
    // overlayNineTile.setBounds((int) rect.left - sOverlayPadding.left,
    //         (int) rect.top - sOverlayPadding.top,
    //         (int) rect.right + sOverlayPadding.right,
    //         (int) rect.bottom + sOverlayPadding.bottom);
    // // TODO(nileshagrawal): We should use time here instead of fractionAnimation
    // // as fractionAnimaton is interpolated and can go over 1.
    // int alpha = constrain((int) (fractionAnimation * 255), 0, 255);
    // overlayNineTile.setAlpha(alpha);
    // overlayNineTile.draw(canvas);
    // canvas.restore();
    assert(0);
    return NOERROR;
}

Float PopupZoomer::GetOverlayCornerRadius(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // if (sOverlayCornerRadius == 0) {
    //     try {
    //         sOverlayCornerRadius = context.getResources().getDimension(
    //                 R.dimen.link_preview_overlay_radius);
    //     } catch (Resources.NotFoundException e) {
    //         Log.w(LOGTAG, "No corner radius resource for PopupZoomer overlay found.");
    //         sOverlayCornerRadius = 1.0f;
    //     }
    // }
    // return sOverlayCornerRadius;
    assert(0);
    return 0.0f;
}

AutoPtr<IDrawable> PopupZoomer::GetOverlayDrawable(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // if (sOverlayDrawable == null) {
    //     try {
    //         sOverlayDrawable = context.getResources().getDrawable(
    //                 R.drawable.ondemand_overlay);
    //     } catch (Resources.NotFoundException e) {
    //         Log.w(LOGTAG, "No drawable resource for PopupZoomer overlay found.");
    //         sOverlayDrawable = new ColorDrawable();
    //     }
    //     sOverlayPadding = new Rect();
    //     sOverlayDrawable.getPadding(sOverlayPadding);
    // }
    // return sOverlayDrawable;
    assert(0);
    AutoPtr<IDrawable> empty;
    return empty;
}

Float PopupZoomer::Constrain(
    /* [in] */ Float amount,
    /* [in] */ Float low,
    /* [in] */ Float high)
{
    // ==================before translated======================
    // return amount < low ? low : (amount > high ? high : amount);
    assert(0);
    return 0.0f;
}

Int32 PopupZoomer::Constrain(
    /* [in] */ Int32 amount,
    /* [in] */ Int32 low,
    /* [in] */ Int32 high)
{
    // ==================before translated======================
    // return amount < low ? low : (amount > high ? high : amount);
    assert(0);
    return 0;
}

ECode PopupZoomer::Scroll(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    // ==================before translated======================
    // mPopupScrollX = constrain(mPopupScrollX - x, mMinScrollX, mMaxScrollX);
    // mPopupScrollY = constrain(mPopupScrollY - y, mMinScrollY, mMaxScrollY);
    // invalidate();
    assert(0);
    return NOERROR;
}

ECode PopupZoomer::StartAnimation(
    /* [in] */ Boolean show)
{
    // ==================before translated======================
    // mAnimating = true;
    // mShowing = show;
    // mTimeLeft = 0;
    // if (show) {
    //     setVisibility(VISIBLE);
    //     mNeedsToInitDimensions = true;
    //     if (mOnVisibilityChangedListener != null) {
    //         mOnVisibilityChangedListener.onPopupZoomerShown(this);
    //     }
    // } else {
    //     long endTime = mAnimationStartTime + ANIMATION_DURATION;
    //     mTimeLeft = endTime - SystemClock.uptimeMillis();
    //     if (mTimeLeft < 0) mTimeLeft = 0;
    // }
    // mAnimationStartTime = SystemClock.uptimeMillis();
    // invalidate();
    assert(0);
    return NOERROR;
}

ECode PopupZoomer::HideImmediately()
{
    // ==================before translated======================
    // mAnimating = false;
    // mShowing = false;
    // mTimeLeft = 0;
    // if (mOnVisibilityChangedListener != null) {
    //     mOnVisibilityChangedListener.onPopupZoomerHidden(this);
    // }
    // setVisibility(INVISIBLE);
    // mZoomedBitmap.recycle();
    // mZoomedBitmap = null;
    assert(0);
    return NOERROR;
}

ECode PopupZoomer::SetTargetBounds(
    /* [in] */ IRect* rect)
{
    VALIDATE_NOT_NULL(rect);
    // ==================before translated======================
    // mTargetBounds = rect;
    assert(0);
    return NOERROR;
}

ECode PopupZoomer::InitDimensions()
{
    // ==================before translated======================
    // if (mTargetBounds == null || mTouch == null) return;
    //
    // // Compute the final zoom scale.
    // mScale = (float) mZoomedBitmap.getWidth() / mTargetBounds.width();
    //
    // float l = mTouch.x - mScale * (mTouch.x - mTargetBounds.left);
    // float t = mTouch.y - mScale * (mTouch.y - mTargetBounds.top);
    // float r = l + mZoomedBitmap.getWidth();
    // float b = t + mZoomedBitmap.getHeight();
    // mClipRect = new RectF(l, t, r, b);
    // int width = getWidth();
    // int height = getHeight();
    //
    // mViewClipRect = new RectF(ZOOM_BOUNDS_MARGIN,
    //         ZOOM_BOUNDS_MARGIN,
    //         width - ZOOM_BOUNDS_MARGIN,
    //         height - ZOOM_BOUNDS_MARGIN);
    //
    // // Ensure it stays inside the bounds of the view.  First shift it around to see if it
    // // can fully fit in the view, then clip it to the padding section of the view to
    // // ensure no overflow.
    // mShiftX = 0;
    // mShiftY = 0;
    //
    // // Right now this has the happy coincidence of showing the leftmost portion
    // // of a scaled up bitmap, which usually has the text in it.  When we want to support
    // // RTL languages, we can conditionally switch the order of this check to push it
    // // to the left instead of right.
    // if (mClipRect.left < ZOOM_BOUNDS_MARGIN) {
    //     mShiftX = ZOOM_BOUNDS_MARGIN - mClipRect.left;
    //     mClipRect.left += mShiftX;
    //     mClipRect.right += mShiftX;
    // } else if (mClipRect.right > width - ZOOM_BOUNDS_MARGIN) {
    //     mShiftX = (width - ZOOM_BOUNDS_MARGIN - mClipRect.right);
    //     mClipRect.right += mShiftX;
    //     mClipRect.left += mShiftX;
    // }
    // if (mClipRect.top < ZOOM_BOUNDS_MARGIN) {
    //     mShiftY = ZOOM_BOUNDS_MARGIN - mClipRect.top;
    //     mClipRect.top += mShiftY;
    //     mClipRect.bottom += mShiftY;
    // } else if (mClipRect.bottom > height - ZOOM_BOUNDS_MARGIN) {
    //     mShiftY = height - ZOOM_BOUNDS_MARGIN - mClipRect.bottom;
    //     mClipRect.bottom += mShiftY;
    //     mClipRect.top += mShiftY;
    // }
    //
    // // Allow enough scrolling to get to the entire bitmap that may be clipped inside the
    // // bounds of the view.
    // mMinScrollX = mMaxScrollX = mMinScrollY = mMaxScrollY = 0;
    // if (mViewClipRect.right + mShiftX < mClipRect.right) {
    //     mMinScrollX = mViewClipRect.right - mClipRect.right;
    // }
    // if (mViewClipRect.left + mShiftX > mClipRect.left) {
    //     mMaxScrollX = mViewClipRect.left - mClipRect.left;
    // }
    // if (mViewClipRect.top + mShiftY > mClipRect.top) {
    //     mMaxScrollY = mViewClipRect.top - mClipRect.top;
    // }
    // if (mViewClipRect.bottom + mShiftY < mClipRect.bottom) {
    //     mMinScrollY = mViewClipRect.bottom - mClipRect.bottom;
    // }
    // // Now that we know how much we need to scroll, we can intersect with mViewClipRect.
    // mClipRect.intersect(mViewClipRect);
    //
    // mLeftExtrusion = mTouch.x - mClipRect.left;
    // mRightExtrusion = mClipRect.right - mTouch.x;
    // mTopExtrusion = mTouch.y - mClipRect.top;
    // mBottomExtrusion = mClipRect.bottom - mTouch.y;
    //
    // // Set an initial scroll position to take touch point into account.
    // float percentX =
    //         (mTouch.x - mTargetBounds.centerX()) / (mTargetBounds.width() / 2.f) + .5f;
    // float percentY =
    //         (mTouch.y - mTargetBounds.centerY()) / (mTargetBounds.height() / 2.f) + .5f;
    //
    // float scrollWidth = mMaxScrollX - mMinScrollX;
    // float scrollHeight = mMaxScrollY - mMinScrollY;
    // mPopupScrollX = scrollWidth * percentX * -1f;
    // mPopupScrollY = scrollHeight * percentY * -1f;
    // // Constrain initial scroll position within allowed bounds.
    // mPopupScrollX = constrain(mPopupScrollX, mMinScrollX, mMaxScrollX);
    // mPopupScrollY = constrain(mPopupScrollY, mMinScrollY, mMaxScrollY);
    assert(0);
    return NOERROR;
}

AutoPtr<IPointF> PopupZoomer::ConvertTouchPoint(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    // ==================before translated======================
    // x -= mShiftX;
    // y -= mShiftY;
    // x = mTouch.x + (x - mTouch.x - mPopupScrollX) / mScale;
    // y = mTouch.y + (y - mTouch.y - mPopupScrollY) / mScale;
    // return new PointF(x, y);
    assert(0);
    AutoPtr<IPointF> empty;
    return empty;
}

Boolean PopupZoomer::IsTouchOutsideArea(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    // ==================before translated======================
    // return !mClipRect.contains(x, y);
    assert(0);
    return FALSE;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


