#ifndef __ELASTOS_DROID_TEXT_METHOD_ArrowKeyMovementMethod_H__
#define __ELASTOS_DROID_TEXT_METHOD_ArrowKeyMovementMethod_H__

#include "Elastos.Droid.Core_server.h"
#include "elastos/droid/text/method/BaseMovementMethod.h"

using Elastos::Droid::Text::ISpannable;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::Widget::ITextView;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

/**
 * A movement method that provides cursor movement and selection.
 * Supports displaying the context menu on DPad Center.
 */
class ArrowKeyMovementMethod
    : public Object
    , public IArrowKeyMovementMethod
    , public IBaseMovementMethod
    , public IMovementMethod
{
public:
    CAR_INTERFACE_DECL()

    virtual ~ArrowKeyMovementMethod();

    //@Override
    CARAPI OnTouchEvent(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI CanSelectArbitrarily(
        /* [out] */ Boolean* result);

    //@Override
    CARAPI Initialize(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* text);

    //@Override
    CARAPI OnTakeFocus(
        /* [in] */ ITextView* view,
        /* [in] */ ISpannable* text,
        /* [in] */ Int32 dir);

    //public
    //static CARAPI_(AutoPtr<IMovementMethod>) GetInstance();

protected:
    //@Override
    CARAPI HandleMovementKey(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 movementMetaState,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI Left(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI Right(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI Up(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI Down(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI PageUp(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI PageDown(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI Top(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI Bottom(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI LineStart(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI LineEnd(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [out] */ Boolean* result);

    /** {@hide} */
    //@Override
    CARAPI LeftWord(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [out] */ Boolean* result);

    /** {@hide} */
    //@Override
    CARAPI RightWord(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI Home(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI End(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [out] */ Boolean* result);

private:
    static CARAPI_(Boolean) IsSelecting(
        /* [in] */ ISpannable* buffer);

    static CARAPI_(Int32) GetCurrentLineTop(
        /* [in] */ ISpannable* buffer,
        /* [in] */ ILayout* layout);

    static CARAPI_(Int32) GetPageHeight(
        /* [in] */ ITextView* widget);

    static CARAPI_(Boolean) IsTouchSelecting(
        /* [in] */ Boolean isMouse,
        /* [in] */ ISpannable* buffer);

private:
    static const AutoPtr<IInterface> LAST_TAP_DOWN;// = new ElObject();
    // static ArrowKeyMovementMethod sInstance;
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_ArrowKeyMovementMethod_H__
