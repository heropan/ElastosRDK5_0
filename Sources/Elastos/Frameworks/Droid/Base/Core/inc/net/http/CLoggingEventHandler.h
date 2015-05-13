
#ifndef __CLOGGINGEVENTHANDLER_H__
#define __CLOGGINGEVENTHANDLER_H__

#include "_CLoggingEventHandler.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CarClass(CLoggingEventHandler)
{
public:
    CARAPI constructor();

    CARAPI requestSent();

    CARAPI LocationChanged(
        /* [in] */ const String& newLocation,
        /* [in] */ Boolean permanent);

    CARAPI Status(
        /* [in] */ Int32 major_version,
        /* [in] */ Int32 minor_version,
        /* [in] */ Int32 code,
        /* [in] */ const String& reason_phrase);

    CARAPI Headers(
        /* [in] */ Elastos::Droid::Net::Http::IHeaders* headers);

    CARAPI Data(
        /* [in] */ const ArrayOf<Byte> & data,
        /* [in] */ Int32 len);

    CARAPI EndData();

    CARAPI Certificate(
        /* [in] */ Elastos::Droid::Net::Http::ISslCertificate* certificate);

    CARAPI Error(
        /* [in] */ Int32 id,
        /* [in] */ const String& description);

    CARAPI HandleSslErrorRequest(
        /* [in] */ Elastos::Droid::Net::Http::ISslError* error,
        /* [out] */ Boolean* result);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}

#endif // __CLOGGINGEVENTHANDLER_H__
