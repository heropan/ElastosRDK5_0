
#include "elastos/droid/text/method/CDialerKeyListener.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

IBaseKeyListener_METHODS_IMPL(CDialerKeyListener, DialerKeyListener, DialerKeyListener)
IKeyListener_METHODS_IMPL(CDialerKeyListener, DialerKeyListener, DialerKeyListener)
IInputFilter_METHODS_IMPL(CDialerKeyListener, DialerKeyListener, DialerKeyListener)

AutoPtr<IDialerKeyListener> CDialerKeyListener::sInstance;

AutoPtr<IDialerKeyListener> CDialerKeyListener::GetInstance()
{
    if (sInstance == NULL) {
        AutoPtr<CDialerKeyListener> dkl;
        CDialerKeyListener::NewByFriend((CDialerKeyListener**)&dkl);
        sInstance = (IDialerKeyListener*)(dkl.Get());
    }
    return sInstance;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

