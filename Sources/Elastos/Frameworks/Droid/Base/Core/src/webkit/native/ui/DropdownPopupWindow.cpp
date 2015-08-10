
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

//===============================================================
// 			DropdownPopupWindow::InnerLayoutChangeListener
//===============================================================
DropdownPopupWindow::InnerLayoutChangeListener::InnerLayoutChangeListener(
    /* in */ DropdownPopupWindow* window)
    : mWindow(window)
{
}

ECode DropdownPopupWindow::InnerLayoutChangeListener::OnLayoutChange(
    /* in */ IView* v,
    /* in */ Int32 left,
    /* in */ Int32 top,
    /* in */ Int32 right,
    /* in */ Int32 bottom,
    /* in */ Int32 oldLeft,
    /* in */ Int32 oldTop,
    /* in */ Int32 oldRight,
    /* in */ Int32 oldBottom)
{
	VALIDATE_NOT_NULL(v);
    if (v == mAnchorView)
    	mWindow->Show();
   	return NOERROR;
}

//===============================================================
// 				DropdownPopupWindow::InnerDismissListener
//===============================================================
DropdownPopupWindow::InnerDismissListener::InnerDismissListener(
    /* in */ DropdownPopupWindow* window)
    : mWindow(window)
{
}

ECode DropdownPopupWindow::InnerDismissListener::OnDismiss()
{
    if (NULL != mWindow->mOnDismissListener) {
        mWindow->mOnDismissListener->OnDismiss();
    }
    mWindow->mAnchorView->RemoveOnLayoutChangeListener(mWindow->mLayoutChangeListener);
    mWindow->mAnchorView->SetTag(NULL);
    mWindow->mViewAndroidDelegate->ReleaseAnchorView(mWindow->mAnchorView);
   	return NOERROR;
}

//===============================================================
// 						DropdownPopupWindow
//===============================================================
DropdownPopupWindow::DropdownPopupWindow(
    /* in */ IContext* context,
    /* in */ ViewAndroidDelegate* viewAndroidDelegate)
    : ListPopupWindow(context, NULL, 0, R::style::DropdownPopupWindow)
    , mContext(context)
    , mViewAndroidDelegate(viewAndroidDelegate)
    , mAnchorView(mViewAndroidDelegate->AcquireAnchorView())
    , mAnchorWidth(0.0f)
    , mAnchorHeight(0.0f)
    , mAnchorX(0.0f)
    , mAnchorY(0.0f)
{
    mAnchorView->SetId(R::id::dropdown_popup_window);
    mAnchorView->SetTag(this);

    mLayoutChangeListenerChange = new InnerLayoutChangeListener();
    mAnchorView->AddOnLayoutChangeListener(mLayoutChangeListener);

	AutoPtr<InnerDismissListener> dismissListener = new InnerDismissListener();
    SetOnDismissListener(dismissListener);
    SetAnchorView(mAnchorView);
}

ECode DropdownPopupWindow::SetAnchorRect(
    /* in */ Float x,
    /* in */ Float y,
    /* in */ Float width,
    /* in */ Float height)
{
    mAnchorWidth = width;
    mAnchorHeight = height;
    mAnchorX = x;
    mAnchorY = y;
    if (NULL != mAnchorView) {
        mViewAndroidDelegate->SetAnchorViewPosition(mAnchorView, mAnchorX, mAnchorY,mAnchorWidth, mAnchorHeight);
    }
    return NOERROR;
}

//@Override
ECode DropdownPopupWindow::SetAdapter(
    /* in */ IListAdapter* adapter)
{
	VALIDATE_NOT_NULL(adapter);
    mAdapter = adapter;
    SetAdapter(adapter);
    return NOERROR;
}

//@Override
ECode DropdownPopupWindow::Show()
{
    // An ugly hack to keep the popup from expanding on top of the keyboard.
    SetInputMethodMode(INPUT_METHOD_NEEDED);
    Int32 contentWidth = MeasureContentWidth();

    AutoPtr<IResources> resources;
	CResources::New((IResources**)&resources);
	mContext->GetResources((IResources**)&resources);

	AutoPtr<IDisplayMetrics> metrics;
	CDisplayMetrics::New((IDisplayMetrics**)&metrics);
	resources->GetDisplayMetrics((IDisplayMetrics**)&&metrics);
	Float density = 0.0f;
	metrics->Density(&density);
    Float contentWidthInDip = ContentWidth() / density;

    if (contentWidthInDip > mAnchorWidth) {
        SetContentWidth(contentWidth);
        AutoPtr<IRect> displayFrame;
        CRect::New((IRect**)&displayFrame);
        mAnchorView->GetWindowVisibleDisplayFrame((IRect**)&displayFrame);

        Float displayWidth = 0.0f;
        displayFrame->Width(&displayWidth);
        if (GetWidth() > displayWidth) {
            SetWidth(displayWidth);
        }
    }
    else {
        SetWidth(ViewGroup::LayoutParams::WRAP_CONTENT);
    }

    mViewAndroidDelegate->SetAnchorViewPosition(mAnchorView, mAnchorX, mAnchorY, mAnchorWidth, mAnchorHeight);
    ListPopupWindow::Show();

    AutoPtr<IListView> listView = GetListView();
    listView->SetDividerHeight(0);
    return NOERROR;
}

//@Override
ECode DropdownPopupWindow::SetOnDismissListener(
    /* in */ IPopupWindow::OnDismissListener* listener)
{
	VALIDATE_NOT_NULL(listener);
    mOnDismissListener = listener;
    return NOERROR;
}

/**
 * Measures the width of the list content.
 * @return The popup window width in pixels.
 */
Int32 DropdownPopupWindow::MeasureContentWidth()
{
    Int32 maxWidth = 0;
    if (NULL == mAdapter)
      return 0;

    Int32 widthMeasureSpec = 0;
    IMeasureSpec::MakeMeasureSpec(0, IMeasureSpec::UNSPECIFIED, &widthMeasureSpec);
    Int32 heightMeasureSpec = 0;
    IMeasureSpec::MakeMeasureSpec(0, IMeasureSpec::UNSPECIFIED, &heightMeasureSpec);

    Int32 count = 0;
    Int32 mearsureWidth = 0;
    mAdapter->GetCount(&count);
    for (Int32 i = 0; i < count; ++i) {
        AutoPtr<IView> itemView;
        mAdapter->GetView(i, &itemView, NULL);
        AutoPtr<ILinearLayout::ILayoutParams> params;
        CLinearLayout::LayoutParams::New(ILinearLayout::LayoutParams::WRAP_CONTENT, ILinearLayout::LayoutParams::WRAP_CONTENT, (ILinearLayout::ILayoutParams**)&params);
        itemView->SetLayoutParams(params);
        itemView->Measure(widthMeasureSpec, heightMeasureSpec);
        itemView->GetMeasuredWidth(&mearsureWidth);
        maxWidth = Math::Max(maxWidth, mearsureWidth);
    }
    return maxWidth;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
