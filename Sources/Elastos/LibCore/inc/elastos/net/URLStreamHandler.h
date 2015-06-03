
#ifndef __URLSTREAMHANDLER_H__
#define __URLSTREAMHANDLER_H__

#include "Elastos.CoreLibrary_server.h"

// extern "C" const InterfaceID EIID_URLStreamHandler;

namespace Elastos {
namespace Net {

class URLStreamHandler
{
public:
    virtual CARAPI OpenConnection(
        /* [in] */ IURL* u,
        /* [out] */ IURLConnection** urlConnection) = 0;

    virtual CARAPI OpenConnection(
        /* [in] */ IURL* u,
        /* [in] */ IProxy* proxy,
        /* [out] */ IURLConnection** urlConnection);

    virtual CARAPI ParseURL(
        /* [in] */ IURL* url,
        /* [in] */ const String& spec,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    virtual CARAPI SetURL(
        /* [in] */ IURL* u,
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& file,
        /* [in] */ const String& ref);

    virtual CARAPI SetURL(
        /* [in] */ IURL* u,
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& authority,
        /* [in] */ const String& userInfo,
        /* [in] */ const String& file,
        /* [in] */ const String& query,
        /* [in] */ const String& ref);

    virtual CARAPI ToExternalForm(
        /* [in] */ IURL* url,
        /* [in] */ Boolean escapeIllegalCharacters,
        /* [out] */ String* s);

    virtual CARAPI ToExternalForm(
        /* [in] */ IURL* url,
        /* [out] */ String* s);

    virtual CARAPI Equals(
        /* [in] */ IURL* url1,
        /* [in] */ IURL* url2,
        /* [out] */ Boolean* isEquals);

    virtual CARAPI GetDefaultPort(
        /* [out] */ Int32* portNum);

    virtual CARAPI GetHostAddress(
        /* [in] */ IURL* url,
        /* [out] */ IInetAddress** hostAddress);

    virtual CARAPI GetHashCode(
        /* [in] */ IURL* url,
        /* [out] */ Int32* code);

    virtual CARAPI HostsEqual(
        /* [in] */ IURL* a,
        /* [in] */ IURL* b,
        /* [out] */ Boolean* isEqual);

    virtual CARAPI SameFile(
        /* [in] */ IURL* url1,
        /* [in] */ IURL* url2,
        /* [out] */ Boolean* isSame);

private:
    static CARAPI_(String) GetHost(
        /* [in] */ IURL* url);

    static CARAPI_(String) RelativePath(
        /* [in] */ const String& base,
        /* [in] */ const String& path);
};

} // namespace Net
} // namespace Elastos

#endif //__URLSTREAMHANDLER_H__
