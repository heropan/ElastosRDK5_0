
#ifndef __FILEHANDLER_H__
#define __FILEHANDLER_H__

#include "Elastos.CoreLibrary_server.h"
#include "URLStreamHandler.h"

using Elastos::Net::IURLConnection;
using Elastos::Net::IURL;

namespace Elastos {
namespace Net {
namespace Url {

class FileHandler : public URLStreamHandler
{
public:
    CARAPI OpenConnection(
        /* [in] */ IURL* u,
        /* [out] */ IURLConnection** urlConnection);

    CARAPI OpenConnection(
        /* [in] */ IURL* u,
        /* [in] */ IProxy* proxy,
        /* [out] */ IURLConnection** urlConnection);

    CARAPI ParseURL(
        /* [in] */ IURL* url,
        /* [in] */ const String& spec,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);
};

} // namespace Url
} // namespace Net
} // namespace Elastos

#endif //__FILEHANDLER_H__
