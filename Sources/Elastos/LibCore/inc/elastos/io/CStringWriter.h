#ifndef __CSTRINGWRITER_H__
#define __CSTRINGWRITER_H__

#include "_CStringWriter.h"
#include "StringWriter.h"

namespace Elastos {
namespace IO {

CarClass(CStringWriter)
    , public StringWriter
{
public:
    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 oneChar32);

    CARAPI WriteChars(
        /* [in] */ const ArrayOf<Char32> &buffer);

    CARAPI WriteCharsEx(
        /* [in] */ const ArrayOf<Char32>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI WriteString(
        /* [in] */ const String& str);

    CARAPI WriteStringEx(
        /* [in] */ const String& str,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI GetBuffer(
        /* [out] */ IStringBuffer** buf);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI AppendChar(
        /* [in] */ Char32 c);

    CARAPI AppendCharSequence(
        /* [in] */ ICharSequence *csq);

    CARAPI AppendCharSequenceEx(
        /* [in] */ ICharSequence *csq,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI CheckError(
        /* [out] */ Boolean *hasError);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 initialSize);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);
};

} // namespace IO
} // namespace Elastos

#endif // __CSTRINGWRITER_H__
