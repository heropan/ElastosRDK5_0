
#include "CLoggingEventHandler.h"
#include "HttpLog.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

ECode CLoggingEventHandler::constructor()
{
    return NOERROR;
}

ECode CLoggingEventHandler::requestSent()
{
    HttpLog::V(String("LoggingEventHandler:requestSent()"));
    return NOERROR;
}

ECode CLoggingEventHandler::LocationChanged(
    /* [in] */ const String& newLocation,
    /* [in] */ Boolean permanent)
{
    if (HttpLog::LOGV) {
        HttpLog::V(String("LoggingEventHandler: locationChanged() ") + newLocation +
                  String(" permanent ") + StringUtils::BooleanToString(permanent));
    }
    return NOERROR;
}

ECode CLoggingEventHandler::Status(
    /* [in] */ Int32 major_version,
    /* [in] */ Int32 minor_version,
    /* [in] */ Int32 code,
    /* [in] */ const String& reason_phrase)
{
    if (HttpLog::LOGV) {
        HttpLog::V(String("LoggingEventHandler:status() major: ") + StringUtils::Int32ToString(major_version) +
              String(" minor: ") + StringUtils::Int32ToString(minor_version) +
              String(" code: ") + StringUtils::Int32ToString(code) +
              String(" reason: ") + reason_phrase);
    }
    return NOERROR;
}

ECode CLoggingEventHandler::Headers(
    /* [in] */ Elastos::Droid::Net::Http::IHeaders* headers)
{
    if (HttpLog::LOGV) {
        HttpLog::V(String("LoggingEventHandler:headers()"));
        // HttpLog::V(headers->ToString());
    }
    return NOERROR;
}

ECode CLoggingEventHandler::Data(
    /* [in] */ const ArrayOf<Byte> & data,
    /* [in] */ Int32 len)
{
    if (HttpLog::LOGV) {
        HttpLog::V(String("LoggingEventHandler: data() ") + StringUtils::Int32ToString(len) + String(" bytes"));
    }
    return NOERROR;
}

ECode CLoggingEventHandler::EndData()
{
    if (HttpLog::LOGV) {
        HttpLog::V(String("LoggingEventHandler: endData() called"));
    }
    return NOERROR;
}

ECode CLoggingEventHandler::Certificate(
    /* [in] */ Elastos::Droid::Net::Http::ISslCertificate* certificate)
{
    if (HttpLog::LOGV) {
         HttpLog::V(String("LoggingEventHandler: certificate(): ") /*+ certificate->ToString()*/);
     }
    return NOERROR;
}

ECode CLoggingEventHandler::Error(
    /* [in] */ Int32 id,
    /* [in] */ const String& description)
{
    if (HttpLog::LOGV) {
        HttpLog::V(String("LoggingEventHandler: error() called Id:") + StringUtils::Int32ToString(id) +
                  String(" description ") + description);
    }
    return NOERROR;
}

ECode CLoggingEventHandler::HandleSslErrorRequest(
    /* [in] */ Elastos::Droid::Net::Http::ISslError* error,
    /* [out] */ Boolean* result)
{
    if (HttpLog::LOGV) {
        HttpLog::V(String("LoggingEventHandler: handleSslErrorRequest():") /*+ error->ToString()*/);
    }
    // return false so that the caller thread won't wait forever
    *result = FALSE;
    return NOERROR;
}

}
}
}
}

