
#ifndef __COUTPUTSTREAMWRITER_H__
#define __COUTPUTSTREAMWRITER_H__

#include "_COutputStreamWriter.h"
#include "OutputStreamWriter.h"

namespace Elastos {
namespace IO {

CarClass(COutputStreamWriter)
    , public OutputStreamWriter
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

    CARAPI GetEncoding(
        /* [out] */ String *encoding);

    CARAPI constructor(
        /* [in] */ IOutputStream *os);

    CARAPI constructor(
        /* [in] */ IOutputStream *os,
        /* [in] */ const String &enc);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);
};

} // namespace IO
} // namespace Elastos

#endif // __COUTPUTSTREAMWRITER_H__
