
#ifndef __CCHARARRAYREADER_H__
#define __CCHARARRAYREADER_H__

#include "_CCharArrayReader.h"
#include "CharArrayReader.h"

namespace Elastos {
namespace IO {

CarClass(CCharArrayReader)
    , public CharArrayReader
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
        /* [out] */ ArrayOf<Char32>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
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

    CARAPI constructor(
        /* [in] */ ArrayOf<Char32>* buf);

    CARAPI constructor(
        /* [in] */ ArrayOf<Char32>* buf,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);
};

} // namespace IO
} // namespace Elastos

#endif //__CCHARARRAYREADER_H__
