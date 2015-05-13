
#ifndef __CBASEMOVEMENTMETHOD_H__
#define __CBASEMOVEMENTMETHOD_H__

#include "_CBaseMovementMethod.h"
#include "text/method/BaseMovementMethod.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CBaseMovementMethod)
    , public BaseMovementMethod
{
public:
    CARAPI Initialize(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* text);

    CARAPI OnKeyDown(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* text,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyUp(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* text,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyOther(
        /* [in] */ ITextView* view,
        /* [in] */ ISpannable* text,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnTakeFocus(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* text,
        /* [in] */ Int32 direction);

    CARAPI OnTrackballEvent(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* text,
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnTouchEvent(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* text,
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnGenericMotionEvent(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* text,
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    CARAPI CanSelectArbitrarily(
        /* [out] */ Boolean* result);
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CBASEMOVEMENTMETHOD_H__
