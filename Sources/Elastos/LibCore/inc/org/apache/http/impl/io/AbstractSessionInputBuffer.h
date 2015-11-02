
#ifndef __ORG_APACHE_HTTP_IMPL_IO_ABSTRACTSESSIONINPUTBUFFER_H__
#define __ORG_APACHE_HTTP_IMPL_IO_ABSTRACTSESSIONINPUTBUFFER_H__

#include "org/apache/http/impl/io/HttpTransportMetricsImpl.h"
#include "elastos/core/Object.h"

using Elastos::IO::IInputStream;
using Org::Apache::Http::IO::ISessionInputBuffer;
using Org::Apache::Http::IO::IHttpTransportMetrics;
using Org::Apache::Http::Params::IHttpParams;
using Org::Apache::Http::Utility::IByteArrayBuffer;
using Org::Apache::Http::Utility::ICharArrayBuffer;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace IO {

class AbstractSessionInputBuffer
    : public Object
    , public ISessionInputBuffer
{
public:
    AbstractSessionInputBuffer();

    CAR_INTERFACE_DECL()

    CARAPI Read(
        /* [out] */ Int32* count);

    CARAPI Read(
        /* [in] */ ArrayOf<Byte>* b,
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ Int32* count);

    CARAPI Read(
        /* [in] */ ArrayOf<Byte>* b,
        /* [out] */ Int32* count);

    CARAPI ReadLine(
        /* [in] */ ICharArrayBuffer* buffer,
        /* [out] */ Int32* count);

    CARAPI ReadLine(
        /* [out] */ String* line);

    CARAPI GetMetrics(
        /* [out] */ IHttpTransportMetrics** metrics);

protected:
    CARAPI Init(
        /* [in] */ IInputStream* instream,
        /* [in] */ Int32 buffersize,
        /* [in] */ IHttpParams* params);

    CARAPI_(Int32) FillBuffer();

    CARAPI_(Boolean) HasBufferedData();

    CARAPI_(Int32) LocateLF();

    CARAPI LineFromLineBuffer(
        /* [in] */ ICharArrayBuffer* charbuffer,
        /* [out] */ Int32* len);

    CARAPI LineFromReadBuffer(
        /* [in] */ ICharArrayBuffer* charbuffer,
        /* [in] */ Int32 pos,
        /* [out] */ Int32* len);

private:
    AutoPtr<IInputStream> mInstream;
    AutoPtr< ArrayOf<Byte> > mBuffer;
    Int32 mBufferpos;
    Int32 mBufferlen;
    AutoPtr<IByteArrayBuffer> mLinebuffer;
    String mCharset;
    Boolean mAscii;
    Int32 mMaxLineLen;
    AutoPtr<HttpTransportMetricsImpl> mMetrics;
};

} // namespace IO
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_IMPL_IO_ABSTRACTSESSIONINPUTBUFFER_H__
