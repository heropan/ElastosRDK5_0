
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//===============================================================
//             FullScreenView::InternalAccessAdapter
//===============================================================

FullScreenView::InternalAccessAdapter::InternalAccessAdapter(
    /* [in] */ FullScreenView* owner)
    : mOwner(owner)
{
}

//@Override
Boolean FullScreenView::InternalAccessAdapter::DrawChild(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IView* child,
    /* [in] */ Int64 drawingTime)
{
    // Intentional no-op
    return FALSE;
}

//@Override
Boolean FullScreenView::InternalAccessAdapter::Super_onKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return FullScreenView.super.onKeyUp(keyCode, event);
}

//@Override
Boolean FullScreenView::InternalAccessAdapter::Super_dispatchKeyEventPreIme(
    /* [in] */ IKeyEvent* event)
{
    return FullScreenView.super.dispatchKeyEventPreIme(event);
}

//@Override
Boolean FullScreenView::InternalAccessAdapter::Super_dispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    return FullScreenView.super.dispatchKeyEvent(event);
}

//@Override
Boolean FullScreenView::InternalAccessAdapter::Super_onGenericMotionEvent(
    /* [in] */ IMotionEvent* event)
{
    return FullScreenView.super.onGenericMotionEvent(event);
}

//@Override
void FullScreenView::InternalAccessAdapter::Super_onConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    // Intentional no-op
}

//@Override
Int32 FullScreenView::InternalAccessAdapter::Super_getScrollBarStyle()
{
    return FullScreenView.super.getScrollBarStyle();
}

//@Override
Boolean FullScreenView::InternalAccessAdapter::AwakenScrollBars()
{
    return FALSE;
}

//@Override
Boolean FullScreenView::InternalAccessAdapter::Super_awakenScrollBars(
    /* [in] */ Int32 startDelay,
    /* [in] */ Boolean invalidate)
{
    return FALSE;
}

//@Override
void FullScreenView::InternalAccessAdapter::OnScrollChanged(
    /* [in] */ Int32 lPix,
    /* [in] */ Int32 tPix,
    /* [in] */ Int32 oldlPix,
    /* [in] */ Int32 oldtPix)
{
    // Intentional no-op.
}

//@Override
void FullScreenView::InternalAccessAdapter::OverScrollBy(
    /* [in] */ Int32 deltaX,
    /* [in] */ Int32 deltaY,
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY,
    /* [in] */ Int32 scrollRangeX,
    /* [in] */ Int32 scrollRangeY,
    /* [in] */ Int32 maxOverScrollX,
    /* [in] */ Int32 maxOverScrollY,
    /* [in] */ Boolean isTouchEvent)
{
    // Intentional no-op.
}

//@Override
void FullScreenView::InternalAccessAdapter::Super_scrollTo(
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY)
{
    // Intentional no-op.
}

//@Override
void FullScreenView::InternalAccessAdapter::SetMeasuredDimension(
    /* [in] */ Int32 measuredWidth,
    /* [in] */ Int32 measuredHeight)
{
    mOwner->SetMeasuredDimension(measuredWidth, measuredHeight);
}

//===============================================================
//                      FullScreenView
//===============================================================

FullScreenView::FullScreenView(
    /* [in] */ IContext* context,
    /* [in] */ AwViewMethods* awViewMethods)
{
    super(context);
    SetAwViewMethods(awViewMethods);
    mInternalAccessAdapter = new InternalAccessAdapter();
}

AutoPtr<InternalAccessAdapter> FullScreenView::GetInternalAccessAdapter()
{
    return mInternalAccessAdapter;
}

void FullScreenView::SetAwViewMethods(
    /* [in] */ AwViewMethods* awViewMethods)
{
    mAwViewMethods = awViewMethods;
}

//@Override
void FullScreenView::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    mAwViewMethods->OnDraw(canvas);
}

//@Override
void FullScreenView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    mAwViewMethods->OnMeasure(widthMeasureSpec, heightMeasureSpec);
}

//@Override
Boolean FullScreenView::RequestFocus(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    mAwViewMethods->RequestFocus();
    return super.requestFocus(direction, previouslyFocusedRect);
}

//@Override
void FullScreenView::SetLayerType(
    /* [in] */ Int32 layerType,
    /* [in] */ IPaint* paint)
{
    super.setLayerType(layerType, paint);
    mAwViewMethods->SetLayerType(layerType, paint);
}

//@Override
AutoPtr<IInputConnection> FullScreenView::OnCreateInputConnection(
    /* [in] */ IEditorInfo* outAttrs)
{
    return mAwViewMethods->OnCreateInputConnection(outAttrs);
}

//@Override
Boolean FullScreenView::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return mAwViewMethods->OnKeyUp(keyCode, event);
}

//@Override
Boolean FullScreenView::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    return mAwViewMethods->DispatchKeyEvent(event);
}

//@Override
Boolean FullScreenView::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    return mAwViewMethods->OnTouchEvent(event);
}

//@Override
Boolean FullScreenView::OnHoverEvent(
    /* [in] */ IMotionEvent* event)
{
    return mAwViewMethods->OnHoverEvent(event);
}

//@Override
Boolean FullScreenView::OnGenericMotionEvent(
    /* [in] */ IMotionEvent* event)
{
    return mAwViewMethods->OnGenericMotionEvent(event);
}

//@Override
void FullScreenView::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    mAwViewMethods->OnConfigurationChanged(newConfig);
}

//@Override
void FullScreenView::OnAttachedToWindow()
{
    super.onAttachedToWindow();
    mAwViewMethods->OnAttachedToWindow();
}

//@Override
void FullScreenView::OnDetachedFromWindow()
{
    super.onDetachedFromWindow();
    mAwViewMethods->OnDetachedFromWindow();
}

//@Override
void FullScreenView::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    super.onWindowFocusChanged(hasWindowFocus);
    mAwViewMethods->OnWindowFocusChanged(hasWindowFocus);
}

//@Override
void FullScreenView::OnFocusChanged(
    /* [in] */ Boolean focused,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    super.onFocusChanged(focused, direction, previouslyFocusedRect);
    mAwViewMethods->OnFocusChanged(
            focused, direction, previouslyFocusedRect);
}

//@Override
void FullScreenView::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 ow,
    /* [in] */ Int32 oh)
{
    super.onSizeChanged(w, h, ow, oh);
    mAwViewMethods->OnSizeChanged(w, h, ow, oh);
}

//@Override
void FullScreenView::OnVisibilityChanged(
    /* [in] */ IView* changedView,
    /* [in] */ Int32 visibility)
{
    super.onVisibilityChanged(changedView, visibility);
    mAwViewMethods->OnVisibilityChanged(changedView, visibility);
}

//@Override
void FullScreenView::OnWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    super.onWindowVisibilityChanged(visibility);
    mAwViewMethods->OnWindowVisibilityChanged(visibility);
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
