
#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWVIEWMETHODS_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWVIEWMETHODS_H__

// import android.content.res.Configuration;
// import android.graphics.Canvas;
// import android.graphics.Paint;
// import android.graphics.Rect;
// import android.view.KeyEvent;
// import android.view.MotionEvent;
// import android.view.View;
// import android.view.inputmethod.EditorInfo;
// import android.view.inputmethod.InputConnection;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * An interface that defines a subset of the {@link View} functionality.
 *
 * <p>This interface allows us to hook up drawing and input related methods to the
 * {@link AwContents}'s consumer in embedded mode, and to the {@link FullScreenView}
 * in fullscreen mode.
 */
class AwViewMethods
{
public:
    /**
     * @see android.view.View#onDraw
     */
    virtual CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas) = 0;

    /**
     * @see android.view.View#onMeasure
     */
    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec) = 0;

    /**
     * @see android.view.View#requestFocus
     */
    virtual CARAPI_(void) RequestFocus() = 0;

    /**
     * @see android.view.View#setLayerType
     */
    virtual CARAPI_(void) SetLayerType(
        /* [in] */ Int32 layerType,
        /* [in] */ IPaint* paint) = 0;

    /**
     * @see android.view.View#onCreateInputConnection
     */
    virtual CARAPI_(AutoPtr<IInputConnection>) OnCreateInputConnection(
        /* [in] */ IEditorInfo* outAttrs) = 0;

    /**
     * @see android.view.View#onKeyUp
     */
    virtual CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event) = 0;

    /**
     * @see android.view.View#dispatchKeyEvent
     */
    virtual CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ IKeyEvent* event) = 0;

    /**
     * @see android.view.View#onTouchEvent
     */
    virtual CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event) = 0;

    /**
     * @see android.view.View#onHoverEvent
     */
    virtual CARAPI_(Boolean) OnHoverEvent(
        /* [in] */ IMotionEvent* event) = 0;

    /**
     * @see android.view.View#onGenericMotionEvent
     */
    virtual CARAPI_(Boolean) OnGenericMotionEvent(
        /* [in] */ IMotionEvent* event) = 0;

    /**
     * @see android.view.View#onConfigurationChanged
     */
    virtual CARAPI_(void) OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig) = 0;

    /**
     * @see android.view.View#onAttachedToWindow
     */
    virtual CARAPI_(void) OnAttachedToWindow() = 0;

    /**
     * @see android.view.View#onDetachedFromWindow
     */
    virtual CARAPI_(void) OnDetachedFromWindow() = 0;

    /**
     * @see android.view.View#onWindowFocusChanged
     */
    virtual CARAPI_(void) OnWindowFocusChanged(
        /* [in] */ Boolean hasWindowFocus) = 0;

    /**
     * @see android.view.View#onFocusChanged
     */
    virtual CARAPI_(void) OnFocusChanged(
        /* [in] */ Boolean focused,
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect) = 0;

    /**
     * @see android.view.View#onSizeChanged
     */
    virtual CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 ow,
        /* [in] */ Int32 oh) = 0;

    /**
     * @see android.view.View#onVisibilityChanged
     */
    virtual CARAPI_(void) OnVisibilityChanged(
        /* [in] */ IView* changedView,
        /* [in] */ Int32 visibility) = 0;

    /**
     * @see android.view.View#onWindowVisibilityChanged
     */
    virtual CARAPI_(void) OnWindowVisibilityChanged(
        /* [in] */ Int32 visibility) = 0;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWVIEWMETHODS_H__
