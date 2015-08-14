
#ifndef __ELASTOS_DROID_WEBKIT_BASE_POWERSTATUSRECEIVER_H__
#define __ELASTOS_DROID_WEBKIT_BASE_POWERSTATUSRECEIVER_H__

// import android.content.BroadcastReceiver;
// import android.content.Context;
// import android.content.Intent;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

/**
 * A BroadcastReceiver that listens to changes in power status and notifies
 * PowerMonitor.
 * It's instantiated by the framework via the application intent-filter
 * declared in its manifest.
 */
class PowerStatusReceiver
    : public Object
    , public IBroadcastReceiver
{
public:
    //@Override
    CARAPI OnReceive(
        /* [in] */ IContext* context,
        /* [in] */ Intent intent);
};

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_BASE_POWERSTATUSRECEIVER_H__
