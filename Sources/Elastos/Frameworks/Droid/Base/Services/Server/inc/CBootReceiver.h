
#ifndef __CBOOTRECEIVER_H__
#define __CBOOTRECEIVER_H__

#include "_CBootReceiver.h"
#include "content/BroadcastReceiver.h"

using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;

namespace Elastos {
namespace Droid {
namespace Server {

class CBootReceiver : public BroadcastReceiver
{
public:
    CARAPI OnReceive(
        /* [in] */ IContext* context,
        /* [in] */ IIntent* intent);

    CARAPI ToString(
        /* [out] */ String* info);
};

} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__CBOOTRECEIVER_H__
