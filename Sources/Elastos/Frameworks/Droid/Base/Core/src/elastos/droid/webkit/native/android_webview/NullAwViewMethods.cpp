
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

AwViewMethods::NullAwViewMethods(
    /* [in] */ AwContents* awContents,
    /* [in] */ InternalAccessDelegate* internalAccessAdapter,
    /* [in] */ IView* containerView)
    : mAwContents(awContents)
    , mInternalAccessAdapter(internalAccessAdapter)
    , mContainerView(containerView)
{
}

//@Override
void AwViewMethods::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    canvas->DrawColor(mAwContents->GetEffectiveBackgroundColor());
}

//@Override
void AwViewMethods::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    // When the containerView is using the NullAwViewMethods then it is not
    // attached to the AwContents. As such, we don't have any contents to measure
    // and using the last measured dimension is the best we can do.
    mInternalAccessAdapter->SetMeasuredDimension(
            mContainerView->GetMeasuredWidth(), mContainerView->GetMeasuredHeight());
}

//@Override
void AwViewMethods::RequestFocus()
{
    // Intentional no-op.
}

//@Override
void AwViewMethods::SetLayerType(
    /* [in] */ Int32 layerType,
    /* [in] */ IPaint* paint)
{
    // Intentional no-op.
}

//@Override
AutoPtr<IInputConnection> AwViewMethods::OnCreateInputConnection(
    /* [in] */ IEditorInfo* outAttrs)
{
    return NULL; // Intentional no-op.
}

//@Override
Boolean AwViewMethods::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return FALSE; // Intentional no-op.
}

//@Override
Boolean AwViewMethods::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    return FALSE; // Intentional no-op.
}

//@Override
Boolean AwViewMethods::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    return FALSE; // Intentional no-op.
}

//@Override
Boolean AwViewMethods::OnHoverEvent(
    /* [in] */ IMotionEvent* event)
{
    return FALSE; // Intentional no-op.
}

//@Override
Boolean AwViewMethods::OnGenericMotionEvent(
    /* [in] */ IMotionEvent* event)
{
    return FALSE; // Intentional no-op.
}

//@Override
void AwViewMethods::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    // Intentional no-op.
}

//@Override
void AwViewMethods::OnAttachedToWindow()
{
    // Intentional no-op.
}

//@Override
void AwViewMethods::OnDetachedFromWindow()
{
    // Intentional no-op.
}

//@Override
void AwViewMethods::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    // Intentional no-op.
}

//@Override
void AwViewMethods::OnFocusChanged(
    /* [in] */ Boolean focused,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    // Intentional no-op.
}

//@Override
void AwViewMethods::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 ow,
    /* [in] */ Int32 oh)
{
    // Intentional no-op.
}

//@Override
void AwViewMethods::OnVisibilityChanged(
    /* [in] */ IView* changedView,
    /* [in] */ Int32 visibility)
{
    // Intentional no-op.
}

//@Override
void AwViewMethods::OnWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    // Intentional no-op.
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
