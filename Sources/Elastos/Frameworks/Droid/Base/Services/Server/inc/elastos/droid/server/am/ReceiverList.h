
#ifndef __ELASTOS_DROID_SERVER_AM_RECEIVERLIST_H__
#define __ELASTOS_DROID_SERVER_AM_RECEIVERLIST_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/server/am/BroadcastFilter.h"
#include <Elastos.Droid.Core.h>
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Droid::Content::IIntentReceiver;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class ProcessRecord;
class BroadcastRecord;
class CActivityManagerService;

class ReceiverList
    : public List< AutoPtr<BroadcastFilter> >
    , public IProxyDeathRecipient
{
public:
    CAR_INTERFACE_DECL()

    ReceiverList(
        /* [in] */ CActivityManagerService* owner,
        /* [in] */ ProcessRecord* callerApp,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 userId,
        /* [in] */ IIntentReceiver* receiver);

    virtual ~ReceiverList();

    // Want object identity, not the array identity we are inheriting.
    // public boolean equals(Object o)

    // public int hashCode()

    CARAPI ProxyDied();

    // void dumpLocal(PrintWriter pw, String prefix);

    // void dump(PrintWriter pw, String prefix);

    CARAPI_(String) ToString();

public:
    CActivityManagerService* mOwner;
    AutoPtr<IIntentReceiver> mReceiver;
    AutoPtr<ProcessRecord> mApp;
    Int32 mPid;
    Int32 mUid;
    Int32 mUserId;
    AutoPtr<BroadcastRecord> mCurBroadcast;
    Boolean mLinkedToDeath;

    String mStringName;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos


#define HASH_FUNC_FOR_AUTOPTR_RECEIVERLIST
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Server::Am::ReceiverList)
#endif



#endif //__ELASTOS_DROID_SERVER_AM_RECEIVERLIST_H__
