
#ifndef __CHTTPSHANDLER_H__
#define __CHTTPSHANDLER_H__

#include "_CHttpsHandler.h"
#include "URLStreamHandler.h"

namespace Elastos {
namespace Net {
namespace Http {

CarClass(CHttpsHandler), public URLStreamHandler
{
public:
    CARAPI constructor();

    CARAPI OpenConnection(
        /* [in] */ IURL* u,
        /* [out] */ IURLConnection** urlConnection);

    CARAPI OpenConnectionEx(
        /* [in] */ IURL* u,
        /* [in] */ IProxy* proxy,
        /* [out] */ IURLConnection** urlConnection);

    CARAPI ParseURL(
        /* [in] */ IURL* url,
        /* [in] */ const String& spec,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SetURL(
        /* [in] */ IURL* u,
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& file,
        /* [in] */ const String& ref);

    CARAPI SetURLEx(
        /* [in] */ IURL* u,
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& authority,
        /* [in] */ const String& userInfo,
        /* [in] */ const String& file,
        /* [in] */ const String& query,
        /* [in] */ const String& ref);

    CARAPI ToExternalFormEx(
        /* [in] */ IURL* url,
        /* [in] */ Boolean escapeIllegalCharacters,
        /* [out] */ String* s);

    CARAPI ToExternalForm(
        /* [in] */ IURL* url,
        /* [out] */ String* s);

    CARAPI EqualsEx(
        /* [in] */ IURL* url1,
        /* [in] */ IURL* url2,
        /* [out] */ Boolean* isEquals);

    CARAPI GetDefaultPort(
        /* [out] */ Int32* portNum);

    CARAPI GetHostAddress(
        /* [in] */ IURL* url,
        /* [out] */ IInetAddress** hostAddress);

    CARAPI GetHashCode(
        /* [in] */ IURL* url,
        /* [out] */ Int32* code);

    CARAPI HostsEqual(
        /* [in] */ IURL* a,
        /* [in] */ IURL* b,
        /* [out] */ Boolean* isEqual);

    CARAPI SameFile(
        /* [in] */ IURL* url1,
        /* [in] */ IURL* url2,
        /* [out] */ Boolean* isSame);
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__CHTTPSHANDLER_H__
