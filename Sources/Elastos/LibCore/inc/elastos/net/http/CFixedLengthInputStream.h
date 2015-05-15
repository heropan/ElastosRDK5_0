
#ifndef __CFIXEDLENGTHINPUTSTREAM_H__
#define __CFIXEDLENGTHINPUTSTREAM_H__

#include "_CFixedLengthInputStream.h"
#include "AbstractHttpInputStream.h"

namespace Elastos {
namespace Net {
namespace Http {

CarClass(CFixedLengthInputStream), public AbstractHttpInputStream
{
public:
    CARAPI constructor(
        /* [in] */ IInputStream* is,
        /* [in] */ ICacheRequest* cacheRequest,
        /* [in] */ IHttpEngine* httpEngine,
        /* [in] */ Int32 length);

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

    CARAPI ReadBytesEx(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* number);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 byteCount,
        /* [out] */ Int64* number);

    CARAPI Close();

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

private:
    Int32 mBytesRemaining;
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__CFIXEDLENGTHINPUTSTREAM_H__
