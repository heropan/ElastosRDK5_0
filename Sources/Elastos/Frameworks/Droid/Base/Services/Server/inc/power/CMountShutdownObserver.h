
#ifndef __CMOUNTSHUTDOWNOBSERVER_H__
#define __CMOUNTSHUTDOWNOBSERVER_H__

#include "_CMountShutdownObserver.h"
#include "power/ShutdownThread.h"
#include "elautoptr.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Power {

CarClass(CMountShutdownObserver)
{
public:
    CARAPI OnShutDownComplete(
        /* [in] */ Int32 statusCode);

    CARAPI constructor(
        /* [in] */ Handle32 host);

private:
    AutoPtr<ShutdownThread> mHost;
};

} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__CMOUNTSHUTDOWNOBSERVER_H__
