
#include "elastos/droid/net/http/LoggingEventHandler.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CAR_INTERFACE_IMPL_2(LoggingEventHandler, Object, ILoggingEventHandler, IEventHandler)

ECode LoggingEventHandler::constructor()
{
    return NOERROR;
}

ECode LoggingEventHandler::RequestSent()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    HttpLog::V(String("LoggingEventHandler:requestSent()"));
    return NOERROR;
#endif
}

ECode LoggingEventHandler::Status(
    /* [in] */ Int32 major_version,
    /* [in] */ Int32 minor_version,
    /* [in] */ Int32 code, /* Status-Code value */
    /* [in] */ const String& reason_phrase)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (HttpLog::LOGV) {
        HttpLog::V(String("LoggingEventHandler:status() major: ") + StringUtils::Int32ToString(major_version) +
              String(" minor: ") + StringUtils::Int32ToString(minor_version) +
              String(" code: ") + StringUtils::Int32ToString(code) +
              String(" reason: ") + reason_phrase);
    }
    return NOERROR;
#endif
}

ECode LoggingEventHandler::Headers(
    /* [in] */ IHeaders* headers)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (HttpLog::LOGV) {
        HttpLog::V(String("LoggingEventHandler:headers()"));
        // HttpLog::V(headers->ToString());
    }
    return NOERROR;
#endif
}

ECode LoggingEventHandler::LocationChanged(
    /* [in] */ const String& newLocation,
    /* [in] */ Boolean permanent)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (HttpLog::LOGV) {
        HttpLog::V(String("LoggingEventHandler: locationChanged() ") + newLocation +
                  String(" permanent ") + StringUtils::BooleanToString(permanent));
    }
    return NOERROR;
#endif
}

ECode LoggingEventHandler::Data(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 len)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (HttpLog::LOGV) {
        HttpLog::V(String("LoggingEventHandler: data() ") + StringUtils::Int32ToString(len) + String(" bytes"));
    }
    return NOERROR;
#endif
}

ECode LoggingEventHandler::EndData()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (HttpLog::LOGV) {
        HttpLog::V(String("LoggingEventHandler: endData() called"));
    }
    return NOERROR;
#endif
}

ECode LoggingEventHandler::Certificate(
    /* [in] */ ISslCertificate* certificate)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (HttpLog::LOGV) {
         HttpLog::V(String("LoggingEventHandler: certificate(): ") /*+ certificate->ToString()*/);
     }
    return NOERROR;
#endif
}

ECode LoggingEventHandler::Error(
    /* [in] */ Int32 id,
    /* [in] */ const String& description)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (HttpLog::LOGV) {
        HttpLog::V(String("LoggingEventHandler: error() called Id:") + StringUtils::Int32ToString(id) +
                  String(" description ") + description);
    }
    return NOERROR;
#endif
}

ECode LoggingEventHandler::HandleSslErrorRequest(
    /* [in] */ ISslError* error,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (HttpLog::LOGV) {
        HttpLog::V(String("LoggingEventHandler: handleSslErrorRequest():") /*+ error->ToString()*/);
    }
    // return false so that the caller thread won't wait forever
    *result = FALSE;
    return NOERROR;
#endif
}

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos
