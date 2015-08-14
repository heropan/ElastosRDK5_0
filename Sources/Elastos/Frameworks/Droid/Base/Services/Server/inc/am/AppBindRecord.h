
#ifndef __ELASTOS_DROID_SERVER_AM_APPBINDRECORD_H__
#define __ELASTOS_DROID_SERVER_AM_APPBINDRECORD_H__

#include "ext/frameworkext.h"
#include <elastos/utility/etl/HashSet.h>
#include "am/ConnectionRecord.h"

using Elastos::Utility::Etl::HashSet;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class CServiceRecord;
class IntentBindRecord;
class ProcessRecord;

/**
 * An association between a service and one of its client applications.
 */
class AppBindRecord : public ElRefBase
{
public:
    AppBindRecord(
        /* [in] */ CServiceRecord* service,
        /* [in] */ IntentBindRecord* intent,
        /* [in] */ ProcessRecord* client);

    // void dump(PrintWriter pw, String prefix);

    // void dumpInIntentBind(PrintWriter pw, String prefix);

    CARAPI_(String) ToString();

public:
    AutoPtr<CServiceRecord> mService;   // The running service.
    AutoPtr<IntentBindRecord> mIntent;  // The intent we are bound to.
    ProcessRecord* mClient;     // Who has started/bound the service.

    HashSet< AutoPtr<ConnectionRecord> > mConnections;// All ConnectionRecord for this client.
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_AM_APPBINDRECORD_H__
