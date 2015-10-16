
#include "elastos/droid/os/NativeMessageQueue.h"

namespace Elastos {
namespace Droid {
namespace Os {

NativeMessageQueue::NativeMessageQueue()
{
    mLooper = android::Looper::getForThread();
    if (mLooper == NULL) {
        mLooper = new android::Looper(false);
        android::Looper::setForThread(mLooper);
    }
}

NativeMessageQueue::~NativeMessageQueue()
{}

void NativeMessageQueue::PollOnce(
    /* [in] */ Int32 timeoutMillis)
{
    mLooper->pollOnce(timeoutMillis);
}

void NativeMessageQueue::Wake()
{
    mLooper->wake();
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
