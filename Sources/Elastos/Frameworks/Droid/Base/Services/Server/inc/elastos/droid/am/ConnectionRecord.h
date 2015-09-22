
#ifndef __ELASTOS_DROID_SERVER_AM_CONNECTIONRECORD_H__
#define __ELASTOS_DROID_SERVER_AM_CONNECTIONRECORD_H__

#include "ext/frameworkext.h"

using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::App::IIServiceConnection;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class AppBindRecord;
class ActivityRecord;

/**
 * Description of a single binding to a service.
 */
class ConnectionRecord : public ElRefBase
{
public:
    ConnectionRecord(
        /* [in] */ AppBindRecord* binding,
        /* [in] */ ActivityRecord* activity,
        /* [in] */ IIServiceConnection* conn,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 clientLabel,
        /* [in] */ IPendingIntent* clientIntent);

    // void dump(PrintWriter pw, String prefix);

    CARAPI_(String) ToString();

public:
    AutoPtr<AppBindRecord> mBinding;    // weak-ref The application/service binding.
    AutoPtr<ActivityRecord> mActivity;  // weak-ref If non-null, the owning activity.
    AutoPtr<IIServiceConnection> mConn;  // The client connection.
    Int32 mFlags;   // Binding options.
    Int32 mClientLabel;          // String resource labeling this client.
    AutoPtr<IPendingIntent> mClientIntent; // How to launch the client.
    String mStringName;     // Caching of toString.
    Boolean mServiceDead;            // Well is it?
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#define HASH_FUNC_FOR_AUTOPTR_CONNECTIONRECORD
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Server::Am::ConnectionRecord)
#endif

#endif //__ELASTOS_DROID_SERVER_AM_CONNECTIONRECORD_H__
