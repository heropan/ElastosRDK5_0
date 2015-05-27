
#ifndef __CSTRINGBUFFERINPUTSTREAM_H__
#define __CSTRINGBUFFERINPUTSTREAM_H__

#include "_CStringBufferInputStream.h"
#include "StringBufferInputStream.h"

namespace Elastos {
namespace IO {

CarClass(CStringBufferInputStream)
    , public StringBufferInputStream
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

    CARAPI constructor(
        /* [in] */ const String& str);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

} // namespace IO
} // namespace Elastos

#endif // __CSTRINGBUFFERINPUTSTREAM_H__
