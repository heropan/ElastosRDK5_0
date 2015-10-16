
#ifndef __ELASTOS_DROID_LOCATION_CLISTENERTRANSPORT_H__
#define __ELASTOS_DROID_LOCATION_CLISTENERTRANSPORT_H__

#include "_Elastos_Droid_Location_CListenerTransport.h"
#include "elastos/droid/os/HandlerBase.h"

using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Location {

class LocationManager;

CarClass(CListenerTransport)
{
private:
    class MyHandler : public HandlerBase
    {
    public:
        MyHandler(
            /* [in] */ CListenerTransport* host)
            : mHost(host)
        {}

        MyHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ CListenerTransport* host)
            : HandlerBase(looper)
            , mHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);
    private:
        CListenerTransport* mHost;
    };

public:
    CARAPI constructor(
        /* [in] */ Handle32 host,
        /* [in] */ ILocationListener* listener,
        /* [in] */ ILooper* looper);

    CARAPI OnLocationChanged(
        /* [in] */ ILocation* location);

    CARAPI OnStatusChanged(
        /* [in] */ const String& provider,
        /* [in] */ Int32 status,
        /* [in] */ IBundle* extras);

    CARAPI OnProviderEnabled(
        /* [in] */ const String& provider);

    CARAPI OnProviderDisabled(
        /* [in] */ const String& provider);

    CARAPI ToString(
        /* [out] */ String* info);

private:
    CARAPI HandleMessage(
        /* [in] */ IMessage* msg);
private:
    static const Int32 TYPE_LOCATION_CHANGED;// = 1;
    static const Int32 TYPE_STATUS_CHANGED;// = 2;
    static const Int32 TYPE_PROVIDER_ENABLED;// = 3;
    static const Int32 TYPE_PROVIDER_DISABLED;// = 4;

    LocationManager* mHost;
    AutoPtr<ILocationListener> mListener;
    AutoPtr<IHandler> mListenerHandler;
};

} // namespace Location
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_LOCATION_CLISTENERTRANSPORT_H__
