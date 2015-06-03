
#include "AbstractHttpInputStream.h"
#include "CStreams.h"

using Elastos::IO::IStreams;
using Elastos::IO::CStreams;
using Elastos::IO::EIID_IInputStream;

namespace Elastos {
namespace Net {
namespace Http {

ECode AbstractHttpInputStream::Init(
    /* [in] */ IInputStream* in,
    /* [in] */ IHttpEngine* httpEngine,
    /* [in] */ ICacheRequest* cacheRequest)
{
    mIn = in;
    mHttpEngine = httpEngine;
    mClosed = FALSE;

    if (cacheRequest != NULL) {
        cacheRequest->GetBody((IOutputStream**)&mCacheBody);
    }
    else {
        mCacheBody = NULL;
    }

    // some apps return a null body; for compatibility we treat that like a null cache request
    if (mCacheBody == NULL) {
        mCacheRequest = NULL;
    }
    else {
        mCacheRequest = cacheRequest;
    }
    return NOERROR;
}

ECode AbstractHttpInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<IStreams> stream;
    CStreams::AcquireSingleton((IStreams**)&stream);

    return stream->ReadSingleByte((IInputStream*)this->Probe(EIID_IInputStream), value);
}

ECode AbstractHttpInputStream::CheckNotClosed()
{
    if (mClosed) {
        // throw new IOException("stream closed");
        return E_IO_EXCEPTION;
    }

    return NOERROR;
}

void AbstractHttpInputStream::CacheWrite(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    if (mCacheBody != NULL) {
        mCacheBody->WriteBytes(*buffer, offset, count);
    }
}

void AbstractHttpInputStream::EndOfInput(
    /* [in] */ Boolean reuseSocket)
{
    if (mCacheRequest != NULL) {
        mCacheBody->Close();
    }
    mHttpEngine->ToRelease(reuseSocket);
}

void AbstractHttpInputStream::UnexpectedEndOfInput()
{
    if (mCacheRequest != NULL) {
        mCacheRequest->Abort();
    }
    mHttpEngine->ToRelease(FALSE);
}


} // namespace Http
} // namespace Net
} // namespace Elastos
