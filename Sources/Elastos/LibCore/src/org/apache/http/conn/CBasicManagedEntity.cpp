
#include "CBasicManagedEntity.h"
#include "CEofSensorInputStream.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Conn {

CAR_INTERFACE_IMPL_2(CBasicManagedEntity, HttpEntityWrapper, IConnectionReleaseTrigger, IEofSensorWatcher)

CAR_OBJECT_IMPL(CBasicManagedEntity)

ECode CBasicManagedEntity::IsRepeatable(
    /* [out] */ Boolean* isRepeatable)
{
    VALIDATE_NOT_NULL(isRepeatable)
    *isRepeatable = FALSE;
    return NOERROR;
}

ECode CBasicManagedEntity::GetContent(
    /* [out] */ IInputStream** inputStream)
{
    VALIDATE_NOT_NULL(inputStream)
    AutoPtr<IInputStream> inputStream;
    mWrappedEntity->GetContent((IInputStream**)&inputStream);
    AutoPtr<IEofSensorInputStream> eofInputStream;
    CEofSensorInputStream::New(inputStream, (IEofSensorWatcher*)this, (IEofSensorInputStream**)&eofInputStream);
    *inputStream = IInputStream::Probe(eofInputStream);
    REFCOUNT_ADD(*inputStream)
    return NOERROR;
}

ECode CBasicManagedEntity::ConsumeContent()
{
    if (mManagedConn == NULL) return NOERROR;

    // try {
    if (mAttemptReuse) {
        // this will not trigger a callback from EofSensorInputStream
        mWrappedEntity->ConsumeContent();
        mManagedConn->MarkReusable();
    }
    // } finally {
    //     releaseManagedConnection();
    // }
    ReleaseConnection();
    return NOERROR;
}

ECode CBasicManagedEntity::WriteTo(
    /* [in] */ IOutputStream* outstream)
{
    HttpEntityWrapper::WriteTo(outstream);
    return ConsumeContent();
}

ECode CBasicManagedEntity::ReleaseConnection()
{
    return ConsumeContent();
}

ECode CBasicManagedEntity::AbortConnection()
{
    if (managedConn != NULL) {
        // try {
        mManagedConn->AbortConnection();
        // } finally {
        //     managedConn = null;
        // }
        mManagedConn = NULL;
    }
    return NOERROR;
}

ECode CBasicManagedEntity::EofDetected(
    /* [in] */ IInputStream* wrapped,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    // try {
    if (mAttemptReuse && (mManagedConn != NULL)) {
        // there may be some cleanup required, such as
        // reading trailers after the response body:
        ICloseable::Probe(wrapped)->Close();
        mManagedConn->MarkReusable();
    }
    // } finally {
    //     releaseManagedConnection();
    // }
    ReleaseManagedConnection();
    *result = FALSE;
    return NOERROR;
}

ECode CBasicManagedEntity::StreamClosed(
    /* [in] */ IInputStream* wrapped,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    // try {
    if (mAttemptReuse && (mManagedConn != NULL)) {
        // this assumes that closing the stream will
        // consume the remainder of the response body:
        ICloseable::Probe(wrapped)->Close();
        mManagedConn->MarkReusable();
    }
    // } finally {
    //     releaseManagedConnection();
    // }
    ReleaseManagedConnection();
    *result = FALSE;
    return NOERROR;
}

ECode CBasicManagedEntity::StreamAbort(
    /* [in] */ IInputStream* wrapped,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (mManagedConn != NULL) {
        mManagedConn->AbortConnection();
    }
    *result = FALSE;
    return NOERROR;
}

ECode CBasicManagedEntity::ReleaseManagedConnection()
{
    if (mManagedConn != NULL) {
        // try {
        mManagedConn->ReleaseConnection();
        // } finally {
        //     managedConn = null;
        // }
        mManagedConn = NULL;
    }
    return NOERROR;
}

ECode CBasicManagedEntity::constructor(
    /* [in] */ IHttpEntity* entity,
    /* [in] */ IManagedClientConnection* conn,
    /* [in] */ Boolean reuse)
{
    Init(entity);

    if (conn == NULL) {
        Logger::E("CBasicManagedEntity", "Connection may not be null.");
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
