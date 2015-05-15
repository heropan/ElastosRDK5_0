
#ifndef __CFILTERINPUTSTREAM_H__
#define __CFILTERINPUTSTREAM_H__

#include "_CFilterInputStream.h"
#include "FilterInputStream.h"

namespace Elastos {
namespace IO {

CarClass(CFilterInputStream)
    , public FilterInputStream
{
public:
    CFilterInputStream();

    ~CFilterInputStream();

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
        /* [out] */ Int32* number);

    CARAPI ReadBytesEx(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* number);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 length,
        /* [out] */ Int64* value);

    CARAPI constructor(
        /* [in] */ IInputStream* is);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

} // namespace IO
} // namespace Elastos

#endif // __CFILTERINPUTSTREAM_H__
