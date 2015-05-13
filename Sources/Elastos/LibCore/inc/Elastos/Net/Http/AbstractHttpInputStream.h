
#ifndef __ABSTRACTHTTPINPUTSTREAM_H__
#define __ABSTRACTHTTPINPUTSTREAM_H__

#include "Elastos.Core_server.h"
#include "InputStream.h"

using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;
using Elastos::IO::InputStream;
using Elastos::Net::ICacheRequest;
using Elastos::Net::Http::IHttpEngine;

namespace Elastos {
namespace Net {
namespace Http {

class AbstractHttpInputStream : public InputStream
{
public:
    CARAPI Init(
        /* [in] */ IInputStream* in,
        /* [in] */ IHttpEngine* httpEngine,
        /* [in] */ ICacheRequest* cacheRequest);
    /**
     * read() is implemented using read(byte[], int, int) so subclasses only
     * need to override the latter.
     */
    // @Override
    CARAPI Read(
        /* [out] */ Int32* value);

protected:
    CARAPI CheckNotClosed();

    CARAPI_(void) CacheWrite(
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    /**
     * Closes the cache entry and makes the socket available for reuse. This
     * should be invoked when the end of the body has been reached.
     */
    CARAPI_(void) EndOfInput(
        /* [in] */ Boolean reuseSocket);

    /**
     * Calls abort on the cache entry and disconnects the socket. This
     * should be invoked when the connection is closed unexpectedly to
     * invalidate the cache entry and to prevent the HTTP connection from
     * being reused. HTTP messages are sent in serial so whenever a message
     * cannot be read to completion, subsequent messages cannot be read
     * either and the connection must be discarded.
     *
     * <p>An earlier implementation skipped the remaining bytes, but this
     * requires that the entire transfer be completed. If the intention was
     * to cancel the transfer, closing the connection is the only solution.
     */
    CARAPI_(void) UnexpectedEndOfInput();

protected:
    AutoPtr<IInputStream> mIn;
    AutoPtr<IHttpEngine> mHttpEngine;
    Boolean mClosed;

private:
    AutoPtr<ICacheRequest> mCacheRequest;
    AutoPtr<IOutputStream> mCacheBody;
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__ABSTRACTHTTPINPUTSTREAM_H__
