#ifndef __CBRICKRECEIVER_H__
#define __CBRICKRECEIVER_H__

#include "_CBrickReceiver.h"
#include "ext/frameworkdef.h"
#include "content/BroadcastReceiver.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::BroadcastReceiver;

namespace Elastos {
namespace Droid {
namespace Server {

CarClass(CBrickReceiver), public BroadcastReceiver{
public:
    //@Override
    CARAPI OnReceive(
        /* [in] */ IContext *context,
        /* [in] */ IIntent *intent);

    CARAPI ToString(
        /* [out] */ String* info)
    {
        VALIDATE_NOT_NULL(info);
        *info = String("CBrickReceiver: ");
        (*info).AppendFormat("%p", this);
        return NOERROR;
    }
};

} // Server
} // Droid
} // Elastos

#endif //__CBRICKRECEIVER_H__
