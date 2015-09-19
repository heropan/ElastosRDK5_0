#ifndef __ELASTOS_DROID_TEXT_METHOD_DialerKeyListener_H__
#define __ELASTOS_DROID_TEXT_METHOD_DialerKeyListener_H__

#include "text/method/NumberKeyListener.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

/**
 * For dialing-only text entry
 * <p></p>
 * As for all implementations of {@link KeyListener}, this class is only concerned
 * with hardware keyboards.  Software input methods have no obligation to trigger
 * the methods in this class.
 */
class DialerKeyListener : public NumberKeyListener
{
public:
    //static AutoPtr<IDialerKeyListener> GetInstance();

    CARAPI_(Int32) GetInputType();

    static CARAPI_(AutoPtr< ArrayOf<Char32> >) GetCHARACTERS();

protected:
    //@Override
    CARAPI_(AutoPtr< ArrayOf<Char32> >) GetAcceptedChars();

    /**
     * Overrides the superclass's lookup method to prefer the number field
     * from the KeyEvent.
     */
    CARAPI_(Int32) Lookup(
        /* [in] */ IKeyEvent* event,
        /* [in] */ ISpannable* content);

public:
    /**
     * The characters that are used.
     *
     * @see KeyEvent#getMatch
     * @see #getAcceptedChars
     */
    static const Char32 CHARACTERS[];// = new char[] { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '#', '*', '+', '-', '(', ')', ',', '/', 'N', '.', ' ', ';' };
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_DialerKeyListener_H__
