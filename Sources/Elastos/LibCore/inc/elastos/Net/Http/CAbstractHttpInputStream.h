
#ifndef __CABSTRACTHTTPINPUTSTREAM_H__
#define __CABSTRACTHTTPINPUTSTREAM_H__

#include "_CAbstractHttpInputStream.h"
#include "AbstractHttpInputStream.h"

namespace Elastos {
namespace Net {
namespace Http {

CarClass(CAbstractHttpInputStream), public AbstractHttpInputStream
{
public:
    CARAPI constructor(
        /* [in] */ IInputStream* in,
        /* [in] */ IHttpEngine* httpEngine,
        /* [in] */ ICacheRequest* cacheRequest);

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
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__CABSTRACTHTTPINPUTSTREAM_H__
