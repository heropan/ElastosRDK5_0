
#ifndef __LIBCORE_NET_URI_JARHANDLER_H__
#define __LIBCORE_NET_URI_JARHANDLER_H__

#include "URLStreamHandler.h"

using Elastos::Net::URLStreamHandler;
using Elastos::Net::IURLConnection;
using Elastos::Net::IURL;
using Elastos::Net::IInetAddress;

namespace Libcore {
namespace Net {
namespace Url {

extern "C"  const InterfaceID EIID_JarHandler;

class JarHandler
    : public URLStreamHandler
    , public IJarHandler
{
public:
    CAR_INTERFACE_DECL();

    CARAPI ToString(
        /* [out] */ String* result);

    virtual CARAPI OpenConnection(
        /* [in] */ IURL* u,
        /* [out] */ IURLConnection** urlConnection);

protected:
    CARAPI ParseURL(
        /* [in] */ IURL* url,
        /* [in] */ const String& spec,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI ToExternalForm(
        /* [in] */ IURL* url,
        /* [out] */ String* s);
};

} // namespace Url
} // namespace Net
} // namespace Libcore

#endif //__LIBCORE_NET_URI_JARHANDLER_H__
