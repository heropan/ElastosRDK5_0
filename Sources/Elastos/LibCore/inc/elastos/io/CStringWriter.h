#ifndef __ELASTOS_IO_CSTRINGWRITER_H__
#define __ELASTOS_IO_CSTRINGWRITER_H__

#include "_Elastos_IO_CStringWriter.h"
#include "StringWriter.h"

namespace Elastos {
namespace IO {

CarClass(CStringWriter)
    , public StringWriter
    , public IStringWriter
    , public ICloseable
    , public IFlushable
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 oneChar32);

    CARAPI Write(
        /* [in] */ const ArrayOf<Char32> &buffer);

    CARAPI Write(
        /* [in] */ const ArrayOf<Char32>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI WriteString(
        /* [in] */ const String& str);

    CARAPI WriteString(
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

    CARAPI AppendCharSequence(
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

#endif // __ELASTOS_IO_CSTRINGWRITER_H__
