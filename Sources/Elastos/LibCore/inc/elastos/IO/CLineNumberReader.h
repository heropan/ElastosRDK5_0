
#ifndef __CLINENUMBERREADER_H__
#define __CLINENUMBERREADER_H__

#include "_CLineNumberReader.h"
#include "LineNumberReader.h"

namespace Elastos {
namespace IO {

CarClass(CLineNumberReader)
    , public LineNumberReader
{
public:
    CARAPI Close();

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadChars(
        /* [out] */ ArrayOf<Char32>* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadCharsEx(
        /* [out] */ ArrayOf<Char32>* puffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [out] */ Int32* number);

    CARAPI IsReady(
        /* [out] */ Boolean* ready);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

    CARAPI ReadCharBuffer(
        /* [in] */ ICharBuffer* target,
        /* [out] */ Int32* number);

    CARAPI ReadLine(
        /* [out] */ String * contents);

    CARAPI GetLineNumber(
        /* [out] */ Int32* lineNumber);

    CARAPI SetLineNumber(
        /* [in] */ Int32 lineNumber);

    CARAPI constructor(
        /* [in] */ IReader* ir);

    CARAPI constructor(
        /* [in] */ IReader* ir,
        /* [in] */ Int32 size);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);
};

} // namespace IO
} // namespace Elastos

#endif // __CLINENUMBERREADER_H__
