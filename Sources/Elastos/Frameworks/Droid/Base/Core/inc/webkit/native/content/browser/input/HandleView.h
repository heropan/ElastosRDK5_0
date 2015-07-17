
#ifndef __HANDLEVIEW_H__
#define __HANDLEVIEW_H__

// import android.content.Context;
// import android.content.res.TypedArray;
// import android.graphics.Canvas;
// import android.graphics.Rect;
// import android.graphics.drawable.Drawable;
// import android.os.SystemClock;
// import android.util.TypedValue;
// import android.view.MotionEvent;
// import android.view.View;
// import android.view.ViewConfiguration;
// import android.view.ViewParent;
// import android.view.animation.AnimationUtils;
// import android.widget.PopupWindow;

// import org.chromium.content.browser.PositionObserver;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

/**
 * View that displays a selection or insertion handle for text editing.
 *
 * While a HandleView is logically a child of some other view, it does not exist in that View's
 * hierarchy.
 *
 */
class HandleView : public View
{
private:
    class InnerListener
        : public Object
        , public PositionObserver::Listener
    {
    public:
        InnerListener(
            /* [in] */ HandleView* owner);

        //@Override
        CARAPI_(void) OnPositionChanged(
            /* [in] */ Int32 x,
            /* [in] */ Int32 y);

    private:
        HandleView* mOwner;
    };

public:
    static const Int32 LEFT = 0;
    static const Int32 CENTER = 1;
    static const Int32 RIGHT = 2;

public:
    HandleView(CursorController controller, int pos, View parent,
            PositionObserver parentPositionObserver);

    CARAPI_(void) SetOrientation(int pos);

    CARAPI_(void) Show();

    CARAPI_(void) Hide();

    CARAPI_(Boolean) IsShowing();

    // x and y are in physical pixels.
    CARAPI_(void) MoveTo(int x, int y);

    //@Override
    CARAPI OnTouchEvent(MotionEvent ev,
        /* [out] */ Boolean* result);

    CARAPI_(boolean) IsDragging();

    /**
     * @return Returns the x position of the handle
     */
    CARAPI_(Int32) GetPositionX();

    /**
     * @return Returns the y position of the handle
     */
    CARAPI_(Int32) GetPositionY();

    // x and y are in physical pixels.
    CARAPI_(void) PositionAt(int x, int y);

    // Returns the x coordinate of the position that the handle appears to be pointing to relative
    // to the handles "parent" view.
    CARAPI_(Int32) GetAdjustedPositionX();

    // Returns the y coordinate of the position that the handle appears to be pointing to relative
    // to the handles "parent" view.
    CARAPI_(Int32) GetAdjustedPositionY();

    // Returns the x coordinate of the postion that the handle appears to be pointing to relative to
    // the root view of the application.
    CARAPI_(Int32) GetRootViewRelativePositionX();

    // Returns the y coordinate of the postion that the handle appears to be pointing to relative to
    // the root view of the application.
    CARAPI_(Int32) GetRootViewRelativePositionY();

    // Returns a suitable y coordinate for the text position corresponding to the handle.
    // As the handle points to a position on the base of the line of text, this method
    // returns a coordinate a small number of pixels higher (i.e. a slightly smaller number)
    // than getAdjustedPositionY.
    CARAPI_(Int32) GetLineAdjustedPositionY();

    CARAPI_(AutoPtr<IDrawable>) GetDrawable();

    /**
     * If the handle is not visible, sets its visibility to View.VISIBLE and begins fading it in.
     */
    CARAPI_(void) BeginFadeIn();

    CARAPI_(void) ShowPastePopupWindow();

protected:
    //@Override
    CARAPI OnMeasure(int widthMeasureSpec, int heightMeasureSpec);

    //@Override
    CARAPI OnDraw(Canvas c);

private:
    CARAPI_(Boolean) IsPositionVisible();

    CARAPI_(void) UpdatePosition(float rawX, float rawY);

    CARAPI_(void) UpdateAlpha();

    CARAPI_(void) UpdateParentPosition(int parentPositionX, int parentPositionY);

    CARAPI_(Int32) GetContainerPositionX();

    CARAPI_(Int32) GetContainerPositionY();

    CARAPI_(void) OnPositionChanged();

    CARAPI_(void) ShowContainer();

private:
    static const Float FADE_DURATION;

    AutoPtr<IDrawable> mDrawable;
    const AutoPtr<IPopupWindow> mContainer;

    // The position of the handle relative to the parent view.
    Int32 mPositionX;
    Int32 mPositionY;

    // The position of the parent relative to the application's root view.
    Int32 mParentPositionX;
    Int32 mParentPositionY;

    // The offset from this handles position to the "tip" of the handle.
    Float mHotspotX;
    Float mHotspotY;

    const AutoPtr<CursorController> mController;
    Boolean mIsDragging;
    Float mTouchToWindowOffsetX;
    Float mTouchToWindowOffsetY;

    const Int32 mLineOffsetY;
    Float mDownPositionX, mDownPositionY;
    Int64 mTouchTimer;
    Boolean mIsInsertionHandle;
    Float mAlpha;
    Int64 mFadeStartTime;

    const AutoPtr<IView> mParent;
    AutoPtr<InsertionHandleController::PastePopupMenu> mPastePopupWindow;

    const AutoPtr<IRect> mTempRect;

    const AutoPtr<PositionObserver> mParentPositionObserver;
    const AutoPtr<PositionObserver::Listener> mParentPositionListener;

    // Number of dips to subtract from the handle's y position to give a suitable
    // y coordinate for the corresponding text position. This is to compensate for the fact
    // that the handle position is at the base of the line of text.
    static const Float LINE_OFFSET_Y_DIP;

    static const AutoPtr< ArrayOf<Int32> > TEXT_VIEW_HANDLE_ATTRS;
};

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__HANDLEVIEW_H__
