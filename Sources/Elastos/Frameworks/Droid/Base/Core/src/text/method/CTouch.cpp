
#include "text/method/CTouch.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CAR_INTERFACE_IMPL(CTouch, Object, ITouch)
CAR_OBJECT_IMPL(CTouch)

ECode CTouch::ScrollTo(
    /* [in] */ ITextView* widget,
    /* [in] */ ILayout* layout,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    Touch::ScrollTo(widget, layout, x, y);
    return NOERROR;
}

ECode CTouch::OnTouchEvent(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer,
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Touch::OnTouchEvent(widget, buffer, event);
    return NOERROR;
}

ECode CTouch::GetInitialScrollX(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Touch::GetInitialScrollX(widget, buffer);
    return NOERROR;
}

ECode CTouch::GetInitialScrollY(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Touch::GetInitialScrollY(widget, buffer);
    return NOERROR;
}

ECode CTouch::IsActivelySelecting(
    /* [in] */ ISpannable* buffer,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret)
    *ret = Touch::IsActivelySelecting(buffer);
    return NOERROR;
}

ECode CTouch::IsSelectionStarted(
    /* [in] */ ISpannable* buffer,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret)
    *ret = Touch::IsSelectionStarted(buffer);
    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos