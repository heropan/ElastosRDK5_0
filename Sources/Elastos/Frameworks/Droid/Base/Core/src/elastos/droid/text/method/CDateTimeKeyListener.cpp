
#include "elastos/droid/text/method/CDateTimeKeyListener.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

IBaseKeyListener_METHODS_IMPL(CDateTimeKeyListener, DateTimeKeyListener, DateTimeKeyListener)
IKeyListener_METHODS_IMPL(CDateTimeKeyListener, DateTimeKeyListener, DateTimeKeyListener)
IInputFilter_METHODS_IMPL(CDateTimeKeyListener, DateTimeKeyListener, DateTimeKeyListener)

AutoPtr<IDateTimeKeyListener> CDateTimeKeyListener::sInstance;

AutoPtr<IDateTimeKeyListener> CDateTimeKeyListener::GetInstance()
{
    if (sInstance == NULL) {
        AutoPtr<CDateTimeKeyListener> dtkl;
        CDateTimeKeyListener::NewByFriend((CDateTimeKeyListener**)&dtkl);
        sInstance = (IDateTimeKeyListener*)(dtkl.Get());
    }
    return sInstance;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

