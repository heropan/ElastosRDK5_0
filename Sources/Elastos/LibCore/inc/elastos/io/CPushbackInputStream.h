
#ifndef __ELASTOS_IO_CPUSHBACKINPUTSTREAM_H__
#define __ELASTOS_IO_CPUSHBACKINPUTSTREAM_H__

#include "_Elastos_IO_CPushbackInputStream.h"
#include "PushbackInputStream.h"

namespace Elastos {
namespace IO {

CarClass(CPushbackInputStream)
    , public PushbackInputStream
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
        /* [out] */ Int32* number);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* number);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

    CARAPI Unread(
        /* [in] */ Int32 oneByte);

    CARAPI UnreadBytes(
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI UnreadBytes(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

    CARAPI constructor(
        /* [in] */ IInputStream* is);

    CARAPI constructor(
        /* [in] */ IInputStream* is,
        /* [in] */ Int32 size);

};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_CPUSHBACKINPUTSTREAM_H__
