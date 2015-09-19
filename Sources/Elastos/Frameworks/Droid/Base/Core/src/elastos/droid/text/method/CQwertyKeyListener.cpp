
#include "text/method/CQwertyKeyListener.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

const Int32 CQwertyKeyListener::CAPITALIZELENGTH = 4;
AutoPtr<IQwertyKeyListener> CQwertyKeyListener::sInstance[CQwertyKeyListener::CAPITALIZELENGTH * 2];
AutoPtr<IQwertyKeyListener> CQwertyKeyListener::sFullKeyboardInstance;

IBaseKeyListener_METHODS_IMPL(CQwertyKeyListener, QwertyKeyListener, QwertyKeyListener)
IKeyListener_METHODS_IMPL(CQwertyKeyListener, QwertyKeyListener, QwertyKeyListener)

/**
 * Returns a new or existing instance with the specified capitalization
 * and correction properties.
 */
AutoPtr<IQwertyKeyListener> CQwertyKeyListener::GetInstance(
    /* [in] */ Boolean autotext,
    /* [in] */ Capitalize cap)
{
    Int32 off = cap * 2 + (autotext ? 1 : 0);

    if (sInstance[off] == NULL) {
        AutoPtr<CQwertyKeyListener> listener;
        CQwertyKeyListener::NewByFriend(cap, autotext, (CQwertyKeyListener**)&listener);
        sInstance[off] = (IQwertyKeyListener*)(listener.Get());
    }

    return sInstance[off];
}

AutoPtr<IQwertyKeyListener> CQwertyKeyListener::GetInstanceForFullKeyboard()
{
    if (sFullKeyboardInstance == NULL) {
        AutoPtr<CQwertyKeyListener> qkl;
        CQwertyKeyListener::NewByFriend(Capitalize_NONE, FALSE, TRUE, (CQwertyKeyListener**)&qkl);
        sFullKeyboardInstance = (IQwertyKeyListener*)(qkl.Get());
    }
    return sFullKeyboardInstance;
}

ECode CQwertyKeyListener::constructor(
    /* [in] */ Capitalize cap,
    /* [in] */ Boolean autoText)
{
    Init(cap, autoText);
    return NOERROR;
}

ECode CQwertyKeyListener::constructor(
    /* [in] */ Capitalize cap,
    /* [in] */ Boolean autotext,
    /* [in] */ Boolean fullKeyboard)//:QwertyKeyListener(cap, autotext, fullKeyboard)
{
    Init(cap, autotext, fullKeyboard);
    return NOERROR;
}


} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos
