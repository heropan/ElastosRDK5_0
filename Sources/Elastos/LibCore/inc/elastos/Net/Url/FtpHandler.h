
#ifndef __FTPHANDLER_H__
#define __FTPHANDLER_H__

#include "Elastos.Core_server.h"
#include "URLStreamHandler.h"

using Elastos::Net::IURLConnection;
using Elastos::Net::IURL;

namespace Elastos {
namespace Net {
namespace Url {

class FtpHandler : public URLStreamHandler
{
protected:
    CARAPI OpenConnection(
        /* [in] */ IURL* u,
        /* [out] */ IURLConnection** urlConnection);

    CARAPI OpenConnectionEx(
        /* [in] */ IURL* u,
        /* [in] */ IProxy* proxy,
        /* [out] */ IURLConnection** urlConnection);

    CARAPI GetDefaultPort(
        /* [out] */ Int32* portNum);
};

} // namespace Url
} // namespace Net
} // namespace Elastos

#endif //__FTPHANDLER_H__
