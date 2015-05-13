
#ifndef __CFIXEDLENGTHOUTPUTSTREAM_H__
#define __CFIXEDLENGTHOUTPUTSTREAM_H__

#include "_CFixedLengthOutputStream.h"
#include "AbstractHttpOutputStream.h"

namespace Elastos {
namespace Net {
namespace Http {

CarClass(CFixedLengthOutputStream), public AbstractHttpOutputStream
{
public:
    CARAPI constructor(
        /* [in] */ IOutputStream* socketOut,
        /* [in] */ Int32 bytesRemaining);

    CARAPI Write(
        /* [in] */ Int32 oneByte);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI WriteBytesEx(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI Close();

    CARAPI Flush();

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

private:
    AutoPtr<IOutputStream> mSocketOut;
    Int32 mBytesRemaining;
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__CFIXEDLENGTHOUTPUTSTREAM_H__
