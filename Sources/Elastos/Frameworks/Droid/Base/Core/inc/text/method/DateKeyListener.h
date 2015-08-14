#ifndef __ELASTOS_DROID_TEXT_METHOD_DateKeyListener_H__
#define __ELASTOS_DROID_TEXT_METHOD_DateKeyListener_H__

#include "text/method/NumberKeyListener.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

/**
 * For entering dates in a text field.
 * <p></p>
 * As for all implementations of {@link KeyListener}, this class is only concerned
 * with hardware keyboards.  Software input methods have no obligation to trigger
 * the methods in this class.
 */
class DateKeyListener : public NumberKeyListener
{
public:
    CARAPI_(Int32) GetInputType();

    static CARAPI_(AutoPtr< ArrayOf<Char32> >) GetCHARACTERS();

    //static AutoPtr<IDateKeyListener> GetInstance();

protected:
    //@Override
    CARAPI_(AutoPtr< ArrayOf<Char32> >) GetAcceptedChars();

public:
    /**
     * The characters that are used.
     *
     * @see KeyEvent#getMatch
     * @see #getAcceptedChars
     */
    static const Char32 CHARACTERS[];// = new Char32[] { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '/', '-', '.' };

private:
    //static AutoPtr<IDateKeyListener> sInstance;
};


} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_DateKeyListener_H__
