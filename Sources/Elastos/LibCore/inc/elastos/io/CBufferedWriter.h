
#ifndef __CBUFFEREDWRITER_H__
#define __CBUFFEREDWRITER_H__

#include "_CBufferedWriter.h"
#include "BufferedWriter.h"

namespace Elastos {
namespace IO {

CarClass(CBufferedWriter)
    ,public BufferedWriter
{
public:
    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 oneChar32);

    CARAPI WriteChars(
        /* [in] */ const ArrayOf<Char32>& buffer);

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
        /* [in] */ ICharSequence* csq);

    CARAPI AppendCharSequenceEx(
        /* [in] */ ICharSequence* csq,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI NewLine();

    CARAPI constructor(
        /* [in] */ IWriter* wout);

    CARAPI constructor(
        /* [in] */ IWriter* wout,
        /* [in] */ Int32 size);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);
};

} // namespace IO
} // namespace Elastos

#endif // __CBUFFEREDWRITER_H__
