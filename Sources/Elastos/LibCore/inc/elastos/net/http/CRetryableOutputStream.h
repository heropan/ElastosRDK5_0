
#ifndef __CRETRYABLEOUTPUTSTREAM_H__
#define __CRETRYABLEOUTPUTSTREAM_H__

#include "_CRetryableOutputStream.h"
#include "AbstractHttpOutputStream.h"

using Elastos::IO::IOutputStream;
using Elastos::IO::IByteArrayOutputStream;

namespace Elastos {
namespace Net {
namespace Http {

CarClass(CRetryableOutputStream), public AbstractHttpOutputStream
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 limi);

    CARAPI Write(
        /* [in] */ Int32 value);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI Close();

    CARAPI Flush();

    CARAPI ContentLength(
        /* [out] */ Int32* value);

    CARAPI WriteToSocket(
        /* [in] */ IOutputStream* socketOut);

Delete(
private:
    Int32 mLimit;
    AutoPtr<IByteArrayOutputStream> mContent;
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__CRETRYABLEOUTPUTSTREAM_H__
