// wuweizuo automatic build .cpp file from .java file.

#include "DropdownPopupWindow.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

//=====================================================================
//           DropdownPopupWindow::InnerOnLayoutChangeListener
//=====================================================================
DropdownPopupWindow::InnerOnLayoutChangeListener::InnerOnLayoutChangeListener(
    /* [in] */ DropdownPopupWindow* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode DropdownPopupWindow::InnerOnLayoutChangeListener::OnLayoutChange(
    /* [in] */ IView* v,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [in] */ Int32 oldLeft,
    /* [in] */ Int32 oldTop,
    /* [in] */ Int32 oldRight,
    /* [in] */ Int32 oldBottom)
{
    VALIDATE_NOT_NULL(v);
    // ==================before translated======================
    // if (v == mAnchorView) DropdownPopupWindow.this.show();
    assert(0);
    return NOERROR;
}

//=====================================================================
//        DropdownPopupWindow::InnerPopupWindowOnDismissListener
//=====================================================================
DropdownPopupWindow::InnerPopupWindowOnDismissListener::InnerPopupWindowOnDismissListener(
    /* [in] */ DropdownPopupWindow* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode DropdownPopupWindow::InnerPopupWindowOnDismissListener::OnDismiss()
{
    // ==================before translated======================
    // if (mOnDismissListener != null) {
    //     mOnDismissListener.onDismiss();
    // }
    // mAnchorView.removeOnLayoutChangeListener(mLayoutChangeListener);
    // mAnchorView.setTag(null);
    // mViewAndroidDelegate.releaseAnchorView(mAnchorView);
    assert(0);
    return NOERROR;
}

//=====================================================================
//                         DropdownPopupWindow
//=====================================================================
DropdownPopupWindow::DropdownPopupWindow(
    /* [in] */ IContext* context,
    /* [in] */ ViewAndroidDelegate* viewAndroidDelegate)
{
    // ==================before translated======================
    // super(context, null, 0, R.style.DropdownPopupWindow);
    // mContext = context;
    // mViewAndroidDelegate = viewAndroidDelegate;
    //
    // mAnchorView = mViewAndroidDelegate.acquireAnchorView();
    // mAnchorView.setId(R.id.dropdown_popup_window);
    // mAnchorView.setTag(this);
    //
    // mLayoutChangeListener = new OnLayoutChangeListener() {
    //     @Override
    //     public void onLayoutChange(View v, int left, int top, int right, int bottom,
    //             int oldLeft, int oldTop, int oldRight, int oldBottom) {
    //         if (v == mAnchorView) DropdownPopupWindow.this.show();
    //     }
    // };
    // mAnchorView.addOnLayoutChangeListener(mLayoutChangeListener);
    //
    // super.setOnDismissListener(new PopupWindow.OnDismissListener() {
    //     @Override
    //     public void onDismiss() {
    //         if (mOnDismissListener != null) {
    //             mOnDismissListener.onDismiss();
    //         }
    //         mAnchorView.removeOnLayoutChangeListener(mLayoutChangeListener);
    //         mAnchorView.setTag(null);
    //         mViewAndroidDelegate.releaseAnchorView(mAnchorView);
    //     }
    // });
    //
    // setAnchorView(mAnchorView);
}

ECode DropdownPopupWindow::SetAnchorRect(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float width,
    /* [in] */ Float height)
{
    // ==================before translated======================
    // mAnchorWidth = width;
    // mAnchorHeight = height;
    // mAnchorX = x;
    // mAnchorY = y;
    // if (mAnchorView != null) {
    //     mViewAndroidDelegate.setAnchorViewPosition(mAnchorView, mAnchorX, mAnchorY,
    //             mAnchorWidth, mAnchorHeight);
    // }
    assert(0);
    return NOERROR;
}

ECode DropdownPopupWindow::SetAdapter(
    /* [in] */ IListAdapter* adapter)
{
    VALIDATE_NOT_NULL(adapter);
    // ==================before translated======================
    // mAdapter = adapter;
    // super.setAdapter(adapter);
    assert(0);
    return NOERROR;
}

ECode DropdownPopupWindow::Show()
{
    // ==================before translated======================
    // // An ugly hack to keep the popup from expanding on top of the keyboard.
    // setInputMethodMode(INPUT_METHOD_NEEDED);
    // int contentWidth = measureContentWidth();
    // float contentWidthInDip = contentWidth /
    //         mContext.getResources().getDisplayMetrics().density;
    // if (contentWidthInDip > mAnchorWidth) {
    //     setContentWidth(contentWidth);
    //     final Rect displayFrame = new Rect();
    //     mAnchorView.getWindowVisibleDisplayFrame(displayFrame);
    //     if (getWidth() > displayFrame.width()) {
    //         setWidth(displayFrame.width());
    //     }
    // } else {
    //     setWidth(ViewGroup.LayoutParams.WRAP_CONTENT);
    // }
    // mViewAndroidDelegate.setAnchorViewPosition(mAnchorView, mAnchorX, mAnchorY, mAnchorWidth,
    //         mAnchorHeight);
    // super.show();
    // getListView().setDividerHeight(0);
    assert(0);
    return NOERROR;
}

ECode DropdownPopupWindow::SetOnDismissListener(
    /* [in] */  PopupWindow)
{
    // ==================before translated======================
    // mOnDismissListener = listener;
    assert(0);
    return NOERROR;
}

Int32 DropdownPopupWindow::MeasureContentWidth()
{
    // ==================before translated======================
    // int maxWidth = 0;
    // View itemView = null;
    // if (mAdapter == null)
    //   return 0;
    // final int widthMeasureSpec = MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED);
    // final int heightMeasureSpec = MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED);
    // for (int i = 0; i < mAdapter.getCount(); i++) {
    //     itemView = mAdapter.getView(i, itemView, null);
    //     LinearLayout.LayoutParams params =
    //             new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT,
    //                     LinearLayout.LayoutParams.WRAP_CONTENT);
    //     itemView.setLayoutParams(params);
    //     itemView.measure(widthMeasureSpec, heightMeasureSpec);
    //     maxWidth = Math.max(maxWidth, itemView.getMeasuredWidth());
    // }
    // return maxWidth;
    assert(0);
    return 0;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


