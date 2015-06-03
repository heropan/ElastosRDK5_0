
#ifndef __FTPURLINPUTSTREAM_H__
#define __FTPURLINPUTSTREAM_H__

#include "Elastos.CoreLibrary_server.h"
#include "InputStream.h"


using Elastos::IO::InputStream;
using Elastos::IO::IInputStream;
using Elastos::Net::ISocket;


namespace Elastos {
namespace Net {
namespace Url {

class FtpURLInputStream : public InputStream
{
public:
    CARAPI Init(
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
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* number);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 byteCount,
        /* [out] */ Int64* number);

private:
    AutoPtr<IInputStream> mIs; // Actual input stream

    AutoPtr<ISocket> mControlSocket;
};

} // namespace Url
} // namespace Net
} // namespace Elastos

#endif //__FTPURLINPUTSTREAM_H__
