#ifndef __ELASTOS_DROID_TEXT_METHOD_NumberKeyListener_H__
#define __ELASTOS_DROID_TEXT_METHOD_NumberKeyListener_H__

#include "elastos/droid/text/method/BaseKeyListener.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

/**
 * For numeric text entry
 * <p></p>
 * As for all implementations of {@link KeyListener}, this class is only concerned
 * with hardware keyboards.  Software input methods have no obligation to trigger
 * the methods in this class.
 */
//public abstract
class NumberKeyListener : public BaseKeyListener /*implements InputFilter*/
{
public:
    CARAPI_(AutoPtr<ICharSequence>) Filter(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ISpanned* dest,
        /* [in] */ Int32 dstart,
        /* [in] */ Int32 dend);

    //@Override
    CARAPI_(Boolean) OnKeyDown(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

protected:
    /**
     * You can say which characters you can accept.
     */
    virtual CARAPI_(AutoPtr< ArrayOf<Char32> >) GetAcceptedChars() = 0;

    CARAPI_(Int32) Lookup(
        /* [in] */ IKeyEvent* event,
        /* [in] */ ISpannable* content);

    static CARAPI_(Boolean) Ok(
        /* [in] */ ArrayOf<Char32>* accept,
        /* [in] */ Char32 c);
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_NumberKeyListener_H__
