
#include "ext/frameworkdef.h"
#include "os/CLooperHelper.h"
#include "os/Looper.h"

namespace Elastos {
namespace Droid {
namespace Os {

ECode CLooperHelper::Prepare()
{
    return Looper::Prepare();
}

ECode CLooperHelper::MyLooper(
    /* [out] */ ILooper** looper)
{
    VALIDATE_NOT_NULL(looper);
    AutoPtr<ILooper> l = Looper::MyLooper();
    *looper = l;
    INTERFACE_ADDREF(*looper);
    return NOERROR;
}

ECode CLooperHelper::MyQueue(
    /* [out] */ IMessageQueue** queue)
{
    VALIDATE_NOT_NULL(queue);
    AutoPtr<IMessageQueue> q = Looper::MyQueue();
    *queue = q;
    INTERFACE_ADDREF(*queue);
    return NOERROR;
}

ECode CLooperHelper::GetMainLooper(
    /* [out] */ ILooper** looper)
{
    VALIDATE_NOT_NULL(looper);
    AutoPtr<ILooper> l = Looper::GetMainLooper();
    *looper = l;
    INTERFACE_ADDREF(*looper);
    return NOERROR;
}

ECode CLooperHelper::Loop()
{
    return Looper::Loop();
}

} // namespace Os
} // namespace Droid
} // namespace Elastos

