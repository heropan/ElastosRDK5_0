
#ifndef __LIBCORE_NET_URI_CFILEURLCONNECTION_H__
#define __LIBCORE_NET_URI_CFILEURLCONNECTION_H__

#include "_LIBCORE_NET_URI_CFileURLConnection.h"
#include "FileURLConnection.h"

using Elastos::Net::IURLConnection;
using Elastos::Net::IURL;

namespace Libcore {
namespace Net {
namespace Url {

CarClass(CFileURLConnection), public FileURLConnection
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Url
} // namespace Net
} // namespace Libcore

#endif //__LIBCORE_NET_URI_CFILEURLCONNECTION_H__
