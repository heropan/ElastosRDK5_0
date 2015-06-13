
#include "CBasicEofSensorWatcher.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Conn {

CAR_INTERFACE_IMPL(CBasicEofSensorWatcher, Object, IEofSensorWatcher)

CAR_OBJECT_IMPL(CBasicEofSensorWatcher)

ECode CBasicEofSensorWatcher::EofDetected(
    /* [in] */ IInputStream* wrapped,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    // try {
    if (mAttemptReuse) {
        // there may be some cleanup required, such as
        // reading trailers after the response body:
        ICloseable::Probe(wrapped)->Close();
        mManagedConn->MarkReusable();
    }
    // } finally {
    //     managedConn.releaseConnection();
    // }
    mManagedConn->ReleaseConnection();
    *result = FALSE;
    return NOERROR;
}

ECode CBasicEofSensorWatcher::StreamClosed(
    /* [in] */ IInputStream* wrapped,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    // try {
    if (attemptReuse) {
        // this assumes that closing the stream will
        // consume the remainder of the response body:
        ICloseable::Probe(wrapped)->Close();
        mManagedConn->MarkReusable();
    }
    // } finally {
    //     managedConn.releaseConnection();
    // }
    mManagedConn->ReleaseConnection();
    *result = FALSE;
    return NOERROR;
}

ECode CBasicEofSensorWatcher::StreamAbort(
    /* [in] */ IInputStream* wrapped,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    mManagedConn->AbortConnection();
    *result = FALSE;
    return NOERROR;
}

ECode CBasicEofSensorWatcher::constructor(
    /* [in] */ IManagedClientConnection* conn,
    /* [in] */ Boolean reuse)
{
    if (conn == NULL) {
        Logger::E("CBasicEofSensorWatcher", "Connection may not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mManagedConn = conn;
    mAttemptReuse = reuse;
    return NOERROR;
}

} // namespace Conn
} // namespace Http
} // namespace Apache
} // namespace Org
