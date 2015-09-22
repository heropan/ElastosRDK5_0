#ifndef __ELASTOS_DROID_TEXT_METHOD_ArrowKeyMovementMethod_H__
#define __ELASTOS_DROID_TEXT_METHOD_ArrowKeyMovementMethod_H__

#include "Elastos.Droid.Core_server.h"
#include "text/method/BaseMovementMethod.h"

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
    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [in] */ IMotionEvent* event);

    //@Override
    CARAPI_(Boolean) CanSelectArbitrarily();

    //@Override
    CARAPI_(void) Initialize(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* text);

    //@Override
    CARAPI_(void) OnTakeFocus(
        /* [in] */ ITextView* view,
        /* [in] */ ISpannable* text,
        /* [in] */ Int32 dir);

    //public
    //static CARAPI_(AutoPtr<IMovementMethod>) GetInstance();

protected:
    //@Override
    CARAPI_(Boolean) HandleMovementKey(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 movementMetaState,
        /* [in] */ IKeyEvent* event);

    //@Override
    CARAPI_(Boolean) Left(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    //@Override
    CARAPI_(Boolean) Right(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    //@Override
    CARAPI_(Boolean) Up(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    //@Override
    CARAPI_(Boolean) Down(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    //@Override
    CARAPI_(Boolean) PageUp(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    //@Override
    CARAPI_(Boolean) PageDown(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    //@Override
    CARAPI_(Boolean) Top(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    //@Override
    CARAPI_(Boolean) Bottom(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    //@Override
    CARAPI_(Boolean) LineStart(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    //@Override
    CARAPI_(Boolean) LineEnd(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    /** {@hide} */
    //@Override
    CARAPI_(Boolean) LeftWord(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    /** {@hide} */
    //@Override
    CARAPI_(Boolean) RightWord(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    //@Override
    CARAPI_(Boolean) Home(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    //@Override
    CARAPI_(Boolean) End(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

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
