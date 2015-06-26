
#ifndef __FULLSCREENVIEW_H__
#define __FULLSCREENVIEW_H__

// import android.content.Context;
// import android.content.res.Configuration;
// import android.graphics.Canvas;
// import android.graphics.Paint;
// import android.graphics.Rect;
// import android.view.KeyEvent;
// import android.view.MotionEvent;
// import android.view.View;
// import android.view.inputmethod.EditorInfo;
// import android.view.inputmethod.InputConnection;
// import android.widget.AbsoluteLayout;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * A view that is used to render the web contents in fullscreen mode, ie.
 * html controls and subtitles, over the {@link ContentVideoView}.
 */
public class FullScreenView : public AbsoluteLayout
{
private:
    // AwContents.InternalAccessDelegate implementation --------------------------------------
    class InternalAccessAdapter : public AwContents::InternalAccessDelegate
    {
    public:
        InternalAccessAdapter(
            /* [in] */ FullScreenView* owner);

        //@Override
        CARAPI_(Boolean) DrawChild(
            /* [in] */ ICanvas* canvas,
            /* [in] */ IView* child,
            /* [in] */ Int64 drawingTime);

        //@Override
        CARAPI_(Boolean) Super_onKeyUp(
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event);

        //@Override
        CARAPI_(Boolean) Super_dispatchKeyEventPreIme(
            /* [in] */ IKeyEvent* event);

        //@Override
        CARAPI_(Boolean) Super_dispatchKeyEvent(
            /* [in] */ IKeyEvent* event);

        //@Override
        CARAPI_(Boolean) Super_onGenericMotionEvent(
            /* [in] */ IMotionEvent* event);

        //@Override
        CARAPI_(void) Super_onConfigurationChanged(
            /* [in] */ IConfiguration* newConfig);

        //@Override
        CARAPI_(Int32) Super_getScrollBarStyle();

        //@Override
        CARAPI_(Boolean) AwakenScrollBars();

        //@Override
        CARAPI_(Boolean) Super_awakenScrollBars(
            /* [in] */ Int32 startDelay,
            /* [in] */ Boolean invalidate);

        //@Override
        CARAPI_(void) OnScrollChanged(
            /* [in] */ Int32 lPix,
            /* [in] */ Int32 tPix,
            /* [in] */ Int32 oldlPix,
            /* [in] */ Int32 oldtPix);

        //@Override
        CARAPI_(void) OverScrollBy(
            /* [in] */ Int32 deltaX,
            /* [in] */ Int32 deltaY,
            /* [in] */ Int32 scrollX,
            /* [in] */ Int32 scrollY,
            /* [in] */ Int32 scrollRangeX,
            /* [in] */ Int32 scrollRangeY,
            /* [in] */ Int32 maxOverScrollX,
            /* [in] */ Int32 maxOverScrollY,
            /* [in] */ Boolean isTouchEvent);

        //@Override
        CARAPI_(void) Super_scrollTo(
            /* [in] */ Int32 scrollX,
            /* [in] */ Int32 scrollY);

        //@Override
        CARAPI_(void) SetMeasuredDimension(
            /* [in] */ Int32 measuredWidth,
            /* [in] */ Int32 measuredHeight);

    private:
        FullScreenView* mOwner;
    };

public:
    FullScreenView(
        /* [in] */ IContext* context,
        /* [in] */ AwViewMethods* awViewMethods);

    CARAPI_(AutoPtr<InternalAccessAdapter>) GetInternalAccessAdapter();

    CARAPI_(void) SetAwViewMethods(
        /* [in] */ AwViewMethods* awViewMethods);

    //@Override
    CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    //@Override
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    //@Override
    CARAPI_(Boolean) RequestFocus(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    //@Override
    CARAPI_(void) SetLayerType(
        /* [in] */ Int32 layerType,
        /* [in] */ IPaint* paint);

    //@Override
    CARAPI_(AutoPtr<IInputConnection>) OnCreateInputConnection(final EditorInfo outAttrs);

    //@Override
    CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    //@Override
    CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ IKeyEvent* event);

    //@Override
    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    //@Override
    CARAPI_(Boolean) OnHoverEvent(
        /* [in] */ IMotionEvent* event);

    //@Override
    CARAPI_(Boolean) OnGenericMotionEvent(
        /* [in] */ IMotionEvent* event);

    //@Override
    CARAPI_(void) OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    //@Override
    CARAPI_(void) OnDetachedFromWindow();

    //@Override
    CARAPI_(void) OnWindowFocusChanged(
        /* [in] */ Boolean hasWindowFocus);

    //@Override
    CARAPI_(void) OnFocusChanged(
        /* [in] */ Boolean focused,
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    //@Override
    CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 ow,
        /* [in] */ Int32 oh);

    //@Override
    CARAPI_(void) OnWindowVisibilityChanged(
        /* [in] */ Int32 visibility);

protected:
    //@Override
    CARAPI_(void) OnAttachedToWindow();

    //@Override
    CARAPI_(void) OnVisibilityChanged(
        /* [in] */ IView* changedView,
        /* [in] */ Int32 visibility);

private:
    AutoPtr<AwViewMethods> mAwViewMethods;
    AutoPtr<InternalAccessAdapter> mInternalAccessAdapter;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__FULLSCREENVIEW_H__
