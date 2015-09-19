
#ifndef __ELASTOS_DROID_OS_NATIVEMESSAGEQUEUE_H__
#define __ELASTOS_DROID_OS_NATIVEMESSAGEQUEUE_H__

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif
#include <utils/Looper.h>

namespace Elastos {
namespace Droid {
namespace Os {

class MessageQueue : public ElRefBase
{
public:
    /* Gets the message queue's looper. */
    inline CARAPI_(android::sp<android::Looper>) GetLooper() const
    {
        return mLooper;
    }

protected:
    MessageQueue() {}
    virtual ~MessageQueue() {}

protected:
    android::sp<android::Looper> mLooper;
};

class NativeMessageQueue : public MessageQueue
{
public:
    NativeMessageQueue();
    virtual ~NativeMessageQueue();

    CARAPI_(void) PollOnce(
        /* [in] */ Int32 timeoutMillis);

    CARAPI_(void) Wake();
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_NATIVEMESSAGEQUEUE_H__
