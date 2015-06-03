
#ifndef __CCHUNKEDINPUTSTREAM_H__
#define __CCHUNKEDINPUTSTREAM_H__

#include "_CChunkedInputStream.h"
#include "AbstractHttpInputStream.h"

namespace Elastos {
namespace Net {
namespace Http {

CarClass(CChunkedInputStream), public AbstractHttpInputStream
{
public:
    CARAPI constructor(
        /* [in] */ IInputStream* is,
        /* [in] */ ICacheRequest* cacheRequest,
        /* [in] */ IHttpEngine* httpEngine);

    CARAPI Available(
        /* [out] */ Int32* number);

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* number);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 byteCount,
        /* [out] */ Int64* number);

    CARAPI Close();

Delete(

private:
    CARAPI_(void) ReadChunkSize();

private:
    static const Int32 MIN_LAST_CHUNK_LENGTH;// = "\r\n0\r\n\r\n".length();
    static const Int32 NO_CHUNK_YET;// = -1;
    Int32 mBytesRemainingInChunk;// = NO_CHUNK_YET;
    Boolean mHasMoreChunks;// = true;
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__CCHUNKEDINPUTSTREAM_H__
