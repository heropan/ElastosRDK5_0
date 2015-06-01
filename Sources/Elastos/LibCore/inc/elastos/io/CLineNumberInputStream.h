
#ifndef __ELASTOS_IO_CLINENUMBERINPUTSTREAM_H__
#define __ELASTOS_IO_CLINENUMBERINPUTSTREAM_H__

#include "_Elastos_IO_CLineNumberInputStream.h"
#include "LineNumberInputStream.h"

namespace Elastos {
namespace IO {

CarClass(CLineNumberInputStream), public LineNumberInputStream
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

    CARAPI GetLineNumber(
        /* [out] */ Int32* lineNumber);

    CARAPI SetLineNumber(
        /* [in] */ Int32 lineNumber);

    CARAPI constructor(
        /* [in] */ IInputStream* is);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_CLINENUMBERINPUTSTREAM_H__
