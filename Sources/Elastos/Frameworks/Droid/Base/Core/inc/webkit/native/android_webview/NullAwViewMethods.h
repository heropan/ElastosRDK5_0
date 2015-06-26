
#ifndef __NULLAWVIEWMETHODS_H__
#define __NULLAWVIEWMETHODS_H__

// import android.content.res.Configuration;
// import android.graphics.Canvas;
// import android.graphics.Paint;
// import android.graphics.Rect;
// import android.view.KeyEvent;
// import android.view.MotionEvent;
// import android.view.View;
// import android.view.inputmethod.EditorInfo;
// import android.view.inputmethod.InputConnection;

// import org.chromium.android_webview.AwContents.InternalAccessDelegate;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * No-op implementation of {@link AwViewMethods} that follows the null object pattern.
 * This {@link NullAwViewMethods} is hooked up to the WebView in fullscreen mode, and
 * to the {@link FullScreenView} in embedded mode, but not to both at the same time.
 */
class NullAwViewMethods : public AwViewMethods
{
public:
    NullAwViewMethods(
        /* [in] */ AwContents* awContents,
        /* [in] */ InternalAccessDelegate* internalAccessAdapter,
        /* [in] */ IView* containerView);

    //@Override
    CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    //@Override
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    //@Override
    CARAPI_(void) RequestFocus();

    //@Override
    CARAPI_(void) SetLayerType(
        /* [in] */ Int32 layerType,
        /* [in] */ IPaint* paint);

    //@Override
    CARAPI_(AutoPtr<IInputConnection>) OnCreateInputConnection(
        /* [in] */ IEditorInfo* outAttrs);

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
    CARAPI_(void) OnAttachedToWindow();

    //@Override
    CARAPI_(void) OnDetachedFromWindow();

    //@Override
    CARAPI_(void) OnWindowFocusChanged(
        /* [in] */ Boolean hasWindowFocus);

    //@Override
    CARAPI_(void) OnFocusChanged(
        /* [in] */ Boolean focused,
        /* [in] */ Int32 direction,
        /* [in] */ IRect previouslyFocusedRect);

    //@Override
    CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 ow,
        /* [in] */ Int32 oh);

    //@Override
    CARAPI_(void) OnVisibilityChanged(
        /* [in] */ IView* changedView,
        /* [in] */ Int32 visibility);

    //@Override
    CARAPI_(void) OnWindowVisibilityChanged(
        /* [in] */ Int32 visibility);

private:
    AutoPtr<AwContents> mAwContents;
    AutoPtr<InternalAccessDelegate> mInternalAccessAdapter;
    AutoPtr<IView> mContainerView;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__NULLAWVIEWMETHODS_H__
