
#ifndef __INTENTBINDRECORD_H__
#define __INTENTBINDRECORD_H__

#include "ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>
#include "am/AppBindRecord.h"

using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Content::IIntentFilterComparison;


#ifndef HASH_FUNC_FOR_AUTOPTR_USING_ADDR_PROCESSRECORD
#define HASH_FUNC_FOR_AUTOPTR_USING_ADDR_PROCESSRECORD
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Server::Am::ProcessRecord)
#endif


namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class ProcessRecord;
class CServiceRecord;


/**
 * A particular Intent that has been bound to a Service.
 */
class IntentBindRecord : public ElRefBase
{
public:
    IntentBindRecord(
        /* [in] */ CServiceRecord* service,
        /* [in] */ IIntentFilterComparison* intent);

    virtual ~IntentBindRecord();

    // void dump(PrintWriter pw, String prefix);

    // void dumpInService(PrintWriter pw, String prefix);

    CARAPI_(Int32) CollectFlags();

    CARAPI_(String) ToString();

public:
     /** The running service. */
    CServiceRecord* mService;
    /** The intent that is bound.*/
    AutoPtr<IIntentFilterComparison> mIntent; //
     /** All apps that have bound to this Intent. */
    HashMap< AutoPtr<ProcessRecord>, AutoPtr<AppBindRecord> > mApps;
    /** Binder published from service. */
    AutoPtr<IBinder> mBinder;
    /** Set when we have initiated a request for this binder. */
    Boolean mRequested;
    /** Set when we have received the requested binder. */
    Boolean mReceived;
    /** Set when we still need to tell the service all clients are unbound. */
    Boolean mHasBound;
    /** Set when the service's onUnbind() has asked to be told about new clients. */
    Boolean mDoRebind;

    String mStringName;      // caching of toString
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__INTENTBINDRECORD_H__
