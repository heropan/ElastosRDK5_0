
#include "text/method/CTimeKeyListener.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

IBaseKeyListener_METHODS_IMPL(CTimeKeyListener, TimeKeyListener, TimeKeyListener)
IKeyListener_METHODS_IMPL(CTimeKeyListener, TimeKeyListener, TimeKeyListener)
IInputFilter_METHODS_IMPL(CTimeKeyListener, TimeKeyListener, TimeKeyListener)

AutoPtr<ITimeKeyListener> CTimeKeyListener::sInstance;

AutoPtr<ITimeKeyListener> CTimeKeyListener::GetInstance()
{
    if (sInstance == NULL) {
        AutoPtr<CTimeKeyListener> tkl;
        CTimeKeyListener::NewByFriend((CTimeKeyListener**)&tkl);
        sInstance = (ITimeKeyListener*)(tkl.Get());
    }
    return sInstance;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos
