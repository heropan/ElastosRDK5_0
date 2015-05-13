
#ifndef __CQWERTYKEYLISTENER_H__
#define __CQWERTYKEYLISTENER_H__

#include "_CQwertyKeyListener.h"
#include "text/method/QwertyKeyListener.h"
#include "text/method/BaseKeyListenerMacro.h"
#include "text/method/KeyListenerMacro.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CQwertyKeyListener)
    , public QwertyKeyListener
{
public:
    IBaseKeyListener_METHODS_DECL()
    IKeyListener_METHODS_DECL()

    static CARAPI_(AutoPtr<IQwertyKeyListener>) GetInstance(
        /* [in] */ Boolean autotext,
        /* [in] */ Capitalize cap);

    /**
     * Gets an instance of the listener suitable for use with full keyboards.
     * Disables auto-capitalization, auto-text and long-press initiated on-screen
     * character pickers.
     */
    static CARAPI_(AutoPtr<IQwertyKeyListener>) GetInstanceForFullKeyboard();

    CARAPI constructor(
        /* [in] */ Capitalize cap,
        /* [in] */ Boolean autoText);

    CARAPI constructor(
        /* [in] */ Capitalize cap,
        /* [in] */ Boolean autotext,
        /* [in] */ Boolean fullKeyboard);

private:
    static const Int32 CAPITALIZELENGTH;// = 4;
    static AutoPtr<IQwertyKeyListener> sInstance[/*CAPITALIZELENGTH * 2*/8];
    static AutoPtr<IQwertyKeyListener> sFullKeyboardInstance;
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CQWERTYKEYLISTENER_H__
