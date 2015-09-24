// wuweizuo automatic build .cpp file from .java file.

#include "ValidationMessageBubble.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Components {

//=====================================================================
//                       ValidationMessageBubble
//=====================================================================
ValidationMessageBubble::ValidationMessageBubble(
    /* [in] */ ContentViewCore* contentViewCore,
    /* [in] */ IRectF* anchor,
    /* [in] */ const String& mainText,
    /* [in] */ const String& subText)
{
    // ==================before translated======================
    // final ViewGroup root = (ViewGroup) View.inflate(contentViewCore.getContext(),
    //         R.layout.validation_message_bubble, null);
    // mPopup = new PopupWindow(root);
    // updateTextViews(root, mainText, subText);
    // measure(contentViewCore.getRenderCoordinates());
    // Point origin = adjustWindowPosition(
    //         contentViewCore, (int) (anchor.centerX() - getAnchorOffset()), (int) anchor.bottom);
    // mPopup.showAtLocation(
    //         contentViewCore.getContainerView(), Gravity.NO_GRAVITY, origin.x, origin.y);
}

AutoPtr<ValidationMessageBubble> ValidationMessageBubble::CreateAndShow(
    /* [in] */ ContentViewCore* contentViewCore,
    /* [in] */ Int32 anchorX,
    /* [in] */ Int32 anchorY,
    /* [in] */ Int32 anchorWidth,
    /* [in] */ Int32 anchorHeight,
    /* [in] */ const String& mainText,
    /* [in] */ const String& subText)
{
    // ==================before translated======================
    // final RectF anchorPixInScreen = makePixRectInScreen(
    //         contentViewCore, anchorX, anchorY, anchorWidth, anchorHeight);
    // return new ValidationMessageBubble(contentViewCore, anchorPixInScreen, mainText, subText);
    assert(0);
    AutoPtr<ValidationMessageBubble> empty;
    return empty;
}

ECode ValidationMessageBubble::Close()
{
    // ==================before translated======================
    // if (mPopup == null) return;
    // mPopup.dismiss();
    // mPopup = null;
    assert(0);
    return NOERROR;
}

ECode ValidationMessageBubble::SetPositionRelativeToAnchor(
    /* [in] */ ContentViewCore* contentViewCore,
    /* [in] */ Int32 anchorX,
    /* [in] */ Int32 anchorY,
    /* [in] */ Int32 anchorWidth,
    /* [in] */ Int32 anchorHeight)
{
    VALIDATE_NOT_NULL(contentViewCore);
    // ==================before translated======================
    // RectF anchor = makePixRectInScreen(
    //         contentViewCore, anchorX, anchorY, anchorWidth, anchorHeight);
    // Point origin = adjustWindowPosition(
    //         contentViewCore, (int) (anchor.centerX() - getAnchorOffset()), (int) anchor.bottom);
    // mPopup.update(origin.x, origin.y, mPopup.getWidth(), mPopup.getHeight());
    assert(0);
    return NOERROR;
}

AutoPtr<IRectF> ValidationMessageBubble::MakePixRectInScreen(
    /* [in] */ ContentViewCore* contentViewCore,
    /* [in] */ Int32 anchorX,
    /* [in] */ Int32 anchorY,
    /* [in] */ Int32 anchorWidth,
    /* [in] */ Int32 anchorHeight)
{
    // ==================before translated======================
    // final RenderCoordinates coordinates = contentViewCore.getRenderCoordinates();
    // final float yOffset = getWebViewOffsetYPixInScreen(contentViewCore);
    // return new RectF(
    //         coordinates.fromLocalCssToPix(anchorX),
    //         coordinates.fromLocalCssToPix(anchorY) + yOffset,
    //         coordinates.fromLocalCssToPix(anchorX + anchorWidth),
    //         coordinates.fromLocalCssToPix(anchorY + anchorHeight) + yOffset);
    assert(0);
    AutoPtr<IRectF> empty;
    return empty;
}

Float ValidationMessageBubble::GetWebViewOffsetYPixInScreen(
    /* [in] */ ContentViewCore* contentViewCore)
{
    // ==================before translated======================
    // int[] location = new int[2];
    // contentViewCore.getContainerView().getLocationOnScreen(location);
    // return location[1] + contentViewCore.getRenderCoordinates().getContentOffsetYPix();
    assert(0);
    return 0.0f;
}

ECode ValidationMessageBubble::UpdateTextViews(
    /* [in] */ IViewGroup* root,
    /* [in] */ const String& mainText,
    /* [in] */ const String& subText)
{
    VALIDATE_NOT_NULL(root);
    // ==================before translated======================
    // ((TextView) root.findViewById(R.id.main_text)).setText(mainText);
    // final TextView subTextView = (TextView) root.findViewById(R.id.sub_text);
    // if (!TextUtils.isEmpty(subText)) {
    //     subTextView.setText(subText);
    // } else {
    //     ((ViewGroup) subTextView.getParent()).removeView(subTextView);
    // }
    assert(0);
    return NOERROR;
}

ECode ValidationMessageBubble::Measure(
    /* [in] */ RenderCoordinates* coordinates)
{
    VALIDATE_NOT_NULL(coordinates);
    // ==================before translated======================
    // mPopup.setWindowLayoutMode(
    //         ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
    // mPopup.getContentView().setLayoutParams(
    //         new RelativeLayout.LayoutParams(
    //                 RelativeLayout.LayoutParams.WRAP_CONTENT,
    //                 RelativeLayout.LayoutParams.WRAP_CONTENT));
    // mPopup.getContentView().measure(
    //         View.MeasureSpec.makeMeasureSpec(coordinates.getLastFrameViewportWidthPixInt(),
    //                 View.MeasureSpec.AT_MOST),
    //         View.MeasureSpec.makeMeasureSpec(coordinates.getLastFrameViewportHeightPixInt(),
    //                 View.MeasureSpec.AT_MOST));
    assert(0);
    return NOERROR;
}

Float ValidationMessageBubble::GetAnchorOffset()
{
    // ==================before translated======================
    // final View root = mPopup.getContentView();
    // final int width = root.getMeasuredWidth();
    // final int arrowWidth = root.findViewById(R.id.arrow_image).getMeasuredWidth();
    // return ApiCompatibilityUtils.isLayoutRtl(root) ?
    //         (width * 3 / 4 - arrowWidth / 2) : (width / 4 + arrowWidth / 2);
    assert(0);
    return 0.0f;
}

AutoPtr<IPoint> ValidationMessageBubble::AdjustWindowPosition(
    /* [in] */ ContentViewCore* contentViewCore,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    // ==================before translated======================
    // final RenderCoordinates coordinates = contentViewCore.getRenderCoordinates();
    // final int viewWidth = coordinates.getLastFrameViewportWidthPixInt();
    // final int viewBottom = (int) getWebViewOffsetYPixInScreen(contentViewCore) +
    //         coordinates.getLastFrameViewportHeightPixInt();
    // final int width = mPopup.getContentView().getMeasuredWidth();
    // final int height = mPopup.getContentView().getMeasuredHeight();
    // if (x < 0) {
    //     x = 0;
    // } else if (x + width > viewWidth) {
    //     x = viewWidth - width;
    // }
    // if (y + height > viewBottom) {
    //     y = viewBottom - height;
    // }
    // return new Point(x, y);
    assert(0);
    AutoPtr<IPoint> empty;
    return empty;
}

} // namespace Components
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


