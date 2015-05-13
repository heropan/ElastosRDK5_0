
#ifndef __CGPSSTATUSLISTENERTRANSPORT_H__
#define __CGPSSTATUSLISTENERTRANSPORT_H__

#include "ext/frameworkext.h"
#include "_CGpsStatusListenerTransport.h"
#include <elastos/List.h>
#include "os/HandlerBase.h"

using Elastos::Utility::List;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Location {

class LocationManager;

// This class is used to send GPS status events to the client's main thread.
CarClass(CGpsStatusListenerTransport)
{
private:

    class GpsHandler : public HandlerBase
    {
    public:
        GpsHandler(
            /* [in] */ CGpsStatusListenerTransport* host)
            : mHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);
    private:
        CGpsStatusListenerTransport* mHost;
    };

    class Nmea : public ElRefBase
    {
    public:
        Nmea(
            /* [in] */ Int64 timestamp,
            /* [in] */ const String& nmea);

    public:
        Int64 mTimestamp;
        String mNmea;
    };

public:
    CARAPI constructor(
        /* [in] */ Handle32 host,
        /* [in] */ IGpsStatusListener* listener);

    CARAPI constructor(
        /* [in] */ Handle32 host,
        /* [in] */ IGpsStatusNmeaListener* listener);

    CARAPI OnGpsStarted();

    CARAPI OnGpsStopped();

    CARAPI OnFirstFix(
        /* [in] */ Int32 ttff);

    CARAPI OnSvStatusChanged(
        /* [in] */ Int32 svCount,
        /* [in] */ ArrayOf<Int32>* prns,
        /* [in] */ ArrayOf<Float>* snrs,
        /* [in] */ ArrayOf<Float>* elevations,
        /* [in] */ ArrayOf<Float>* azimuths,
        /* [in] */ Int32 ephemerisMask,
        /* [in] */ Int32 almanacMask,
        /* [in] */ Int32 usedInFixMask);

    CARAPI OnNmeaReceived(
        /* [in] */ Int64 timestamp,
        /* [in] */ const String& nmea);

private:
    CARAPI HandleGpsChanged(
        /* [in] */ Int32 event);

    CARAPI HandleNmeaReceived();

private:
    // This must not equal any of the GpsStatus event IDs
    static const Int32 NMEA_RECEIVED;// = 1000;

    LocationManager* mHost;
    AutoPtr<IGpsStatusListener> mListener;
    AutoPtr<IGpsStatusNmeaListener> mNmeaListener;
    List< AutoPtr<Nmea> > mNmeaBuffer;
    Mutex mNmeaBufferLock;
    AutoPtr<IHandler> mGpsHandler;
};

} // namespace Location
} // namepsace Droid
} // namespace Elastos

#endif //__CGPSSTATUSLISTENERTRANSPORT_H__
