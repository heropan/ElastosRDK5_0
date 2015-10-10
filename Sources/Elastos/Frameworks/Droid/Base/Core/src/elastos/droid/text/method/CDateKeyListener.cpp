
#include "elastos/droid/text/method/CDateKeyListener.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

IBaseKeyListener_METHODS_IMPL(CDateKeyListener, DateKeyListener, DateKeyListener)
IKeyListener_METHODS_IMPL(CDateKeyListener, DateKeyListener, DateKeyListener)
IInputFilter_METHODS_IMPL(CDateKeyListener, DateKeyListener, DateKeyListener)

AutoPtr<IDateKeyListener> CDateKeyListener::sInstance;

AutoPtr<IDateKeyListener> CDateKeyListener::GetInstance()
{
    if (sInstance == NULL) {
        AutoPtr<CDateKeyListener> dkl;
        CDateKeyListener::NewByFriend((CDateKeyListener**)&dkl);
        sInstance = (IDateKeyListener*)(dkl.Get());
    }
    return sInstance;
}




} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

