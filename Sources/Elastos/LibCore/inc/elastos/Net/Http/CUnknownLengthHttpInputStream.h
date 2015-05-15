
#ifndef __CUNKNOWNLENGTHHTTPINPUTSTREAM_H__
#define __CUNKNOWNLENGTHHTTPINPUTSTREAM_H__

#include "_CUnknownLengthHttpInputStream.h"
#include "AbstractHttpInputStream.h"

using Elastos::IO::IInputStream;
using Elastos::Net::ICacheRequest;
using Elastos::Net::Http::IHttpEngine;

namespace Elastos {
namespace Net {
namespace Http {

CarClass(CUnknownLengthHttpInputStream), public AbstractHttpInputStream
{
public:
    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

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

private:
    Boolean mInputExhausted;
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__CUNKNOWNLENGTHHTTPINPUTSTREAM_H__
