
#ifndef __CBUFFEREDOUTPUTSTREAM_H__
#define __CBUFFEREDOUTPUTSTREAM_H__

#include "_CBufferedOutputStream.h"
#include "BufferedOutputStream.h"

namespace Elastos {
namespace IO {

CarClass(CBufferedOutputStream)
    , public BufferedOutputStream
{
public:
    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 oneByte);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte> & buffer);

    CARAPI WriteBytesEx(
        /* [in] */ const ArrayOf<Byte> & buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IOutputStream* out);

    CARAPI constructor(
        /* [in] */ IOutputStream* out,
        /* [in] */ Int32 size);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);
};

} // namespace IO
} // namespace Elastos

#endif // __CBUFFEREDOUTPUTSTREAM_H__
