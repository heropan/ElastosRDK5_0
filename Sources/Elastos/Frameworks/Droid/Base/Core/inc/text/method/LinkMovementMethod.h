#ifndef __ELASTOS_DROID_TEXT_METHOD_LinkMovementMethod_H__
#define __ELASTOS_DROID_TEXT_METHOD_LinkMovementMethod_H__

#include "text/method/ScrollingMovementMethod.h"
//#include "os/ElObject.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

    #define Integer_MAX_VALUE 0x7fffffff
#endif
    #define Integer_MIN_VALUE 0x80000000
#endif

/**
 * A movement method that traverses links in the text buffer and scrolls if necessary.
 * Supports clicking on links with DPad Center or Enter.
 */
class LinkMovementMethod : public ScrollingMovementMethod
{
public:
    //@Override
    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [in] */ IMotionEvent* event);

    //@Override
    CARAPI_(void) Initialize(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* text);

    //@Override
    CARAPI_(void) OnTakeFocus(
        /* [in] */ ITextView* view,
        /* [in] */ ISpannable* text,
        /* [in] */ Int32 dir);

    //static AutoPtr<IMovementMethod> GetInstance();

protected:
    //@Override
    CARAPI_(Boolean) HandleMovementKey(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 movementMetaState,
        /* [in] */ IKeyEvent* event);

    //@Override
    CARAPI_(Boolean) Up(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    //@Override
    CARAPI_(Boolean) Down(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    //@Override
    CARAPI_(Boolean) Left(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    //@Override
    CARAPI_(Boolean) Right(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

private:
    CARAPI_(Boolean) Action(Int32 what,
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

private:
    static const Int32 CLICK;// = 1;
    static const Int32 UP;// = 2;
    static const Int32 DOWN;// = 3;

private:
    //static AutoPtr<ILinkMovementMethod> sInstance;

    static AutoPtr<IInterface> FROM_BELOW;// = new NoCopySpan.Concrete();
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_LinkMovementMethod_H__
