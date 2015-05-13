
#ifndef __CLOOPERHELPER_H__
#define __CLOOPERHELPER_H__

#include "_CLooperHelper.h"

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CLooperHelper)
{
public:
    CARAPI Prepare();

    CARAPI MyLooper(
        /* [out] */ ILooper** looper);

    CARAPI MyQueue(
        /* [out] */ IMessageQueue** queue);

    CARAPI GetMainLooper(
        /* [out] */ ILooper** looper);

    CARAPI Loop();
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__CLOOPERHELPER_H__
