
#ifndef __ELASTOS_DROID_NET_HTTP_LOGGINGEVENTHANDLER_H__
#define __ELASTOS_DROID_NET_HTTP_LOGGINGEVENTHANDLER_H__

#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * {@hide}
 */
class LoggingEventHandler
    : public Object
    , public ILoggingEventHandler
    , public IEventHandler
{
public:
    CAR_INTERFACE_DECL()

    CARAPI constructor();

    CARAPI RequestSent();

    CARAPI Status(
        /* [in] */ Int32 major_version,
        /* [in] */ Int32 minor_version,
        /* [in] */ Int32 code, /* Status-Code value */
        /* [in] */ const String& reason_phrase);

     CARAPI Headers(
        /* [in] */ IHeaders* headers);

    CARAPI LocationChanged(
        /* [in] */ const String& newLocation,
        /* [in] */ Boolean permanent);

    CARAPI Data(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 len);

    CARAPI EndData();

    CARAPI Certificate(
        /* [in] */ ISslCertificate* certificate);

    CARAPI Error(
        /* [in] */ Int32 id,
        /* [in] */ const String& description);

    CARAPI HandleSslErrorRequest(
        /* [in] */ ISslError* error,
        /* [out] */ Boolean* result);
};

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_HTTP_LOGGINGEVENTHANDLER_H__
