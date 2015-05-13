
#ifndef __CBUFFEREDINPUTSTREAM_H__
#define __CBUFFEREDINPUTSTREAM_H__

#include "_CBufferedInputStream.h"
#include "BufferedInputStream.h"

namespace Elastos {
namespace IO {

CarClass(CBufferedInputStream)
    , public BufferedInputStream
{
public:
    CARAPI Available(
        /* [out] */ Int32* number);

    CARAPI Close();

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* value);

    CARAPI ReadBytesEx(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* value);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 length,
        /* [out] */ Int64* value);

    CARAPI constructor(
        /* [in] */ IInputStream* is);

    CARAPI constructor(
        /* [in] */ IInputStream* is,
        /* [in] */ Int32 size);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

} // namespace IO
} // namespace Elastos

#endif // __CBUFFEREDINPUTSTREAM_H__
