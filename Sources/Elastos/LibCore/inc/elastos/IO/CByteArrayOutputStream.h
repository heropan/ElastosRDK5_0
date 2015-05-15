
#ifndef __CBYTEARRAYOUTPUTSTREAM_H__
#define __CBYTEARRAYOUTPUTSTREAM_H__

#include "_CByteArrayOutputStream.h"
#include "ByteArrayOutputStream.h"

namespace Elastos {
namespace IO {

CarClass(CByteArrayOutputStream)
    , public ByteArrayOutputStream
{
public:
    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 oneByte);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI WriteBytesEx(
        /* [in] */ const ArrayOf<Byte> & buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI Reset();

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI ToByteArray(
        /* [out, callee] */ ArrayOf<Byte>** bytes);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI ToStringEx(
        /* [in] */ const String& enc,
        /* [out] */ String* str);

    CARAPI WriteTo(
        /* [in] */ IOutputStream* os);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 size);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);
};

} // namespace IO
} // namespace Elastos

#endif // __CBYTEARRAYOUTPUTSTREAM_H__
