
#ifndef __CFTPURLINPUTSTREAM_H__
#define __CFTPURLINPUTSTREAM_H__

#include "_CFtpURLInputStream.h"
#include "FtpURLInputStream.h"

namespace Elastos {
namespace Net {
namespace Url {

CarClass(CFtpURLInputStream), public FtpURLInputStream
{
public:
    CARAPI constructor(
        /* [in] */ IInputStream* is,
        /* [in] */ ISocket* controlSocket);

    CARAPI Available(
        /* [out] */ Int32* number);

    CARAPI Close();

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* number);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 byteCount,
        /* [out] */ Int64* number);

Delete(
};

} // namespace Url
} // namespace Net
} // namespace Elastos

#endif //__CFTPURLINPUTSTREAM_H__
