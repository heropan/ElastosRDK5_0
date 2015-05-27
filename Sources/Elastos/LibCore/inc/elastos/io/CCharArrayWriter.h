
#ifndef __CCHARARRAYWRITER_H__
#define __CCHARARRAYWRITER_H__

#include "_CCharArrayWriter.h"
#include "CharArrayWriter.h"

namespace Elastos {
namespace IO {

CarClass(CCharArrayWriter)
    , public CharArrayWriter
{
public:
    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 oneChar32);

    CARAPI WriteChars(
        /* [in] */ const ArrayOf<Char32>& buffer);

    CARAPI WriteChars(
        /* [in] */ const ArrayOf<Char32>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI WriteString(
        /* [in] */ const String& str);

    CARAPI WriteString(
        /* [in] */ const String& str,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI AppendChar(
        /* [in] */ Char32 c);

    CARAPI AppendCharSequence(
        /* [in] */ ICharSequence* csq);

    CARAPI AppendCharSequence(
        /* [in] */ ICharSequence* csq,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI Reset();

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI ToCharArray(
        /* [out, callee] */ ArrayOf<Char32>** buffer);

    CARAPI WriteTo(
        /* [in] */ IWriter* ot);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 initialSize);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);
};

} // namespace IO
} // namespace Elastos

#endif // __CCHARARRAYWRITER_H__
