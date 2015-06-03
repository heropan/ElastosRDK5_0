
#ifndef __CURL_H__
#define __CURL_H__

#include "_CURL.h"

#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;

using Elastos::IO::IInputStream;

namespace Elastos {
namespace Net {

class URLStreamHandler;

CarClass(CURL)
{
    friend class URLStreamHandler;

public:
    CURL();

    CARAPI constructor(
        /* [in] */ const String& spec);

    CARAPI constructor(
        /* [in] */ IURL* context,
        /* [in] */ const String& spec);

    CARAPI constructor(
        /* [in] */ IURL* context,
        /* [in] */ const String& spec,
        /* [in] */ IURLStreamHandler* handler);

    CARAPI constructor(
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ const String& file);

    CARAPI constructor(
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& file);

    CARAPI constructor(
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& file,
        /* [in] */ IURLStreamHandler* handler);

    static CARAPI SetURLStreamHandlerFactory(
        /* [in] */ IURLStreamHandlerFactory* streamFactory);

    CARAPI SameFile(
        /* [in] */ IURL* otherURL,
        /* [out] */ Boolean* isSame);

    CARAPI_(Int32) GetHashCode();

    CARAPI GetContent(
        /* [out] */ IInterface** content);

//        public final Object getContent(Class[] types)

    CARAPI OpenStream(
        /* [out] */ IInputStream** is);

    CARAPI OpenConnection(
        /* [out] */ IURLConnection** connection);

    CARAPI OpenConnection(
        /* [in] */ IProxy* proxy,
        /* [out] */ IURLConnection** connection);

    CARAPI ToURI(
        /* [out] */ IURI** uri);

    CARAPI ToURILenient(
            /* [out] */ IURI** uri);

    CARAPI ToExternalForm(
        /* [out] */ String* url);

    CARAPI GetFile(
        /* [out] */ String* fileName);

    CARAPI GetHost(
        /* [out] */ String* hostName);

    CARAPI GetPort(
        /* [out] */ Int32* port);

    CARAPI GetEffectivePort(
        /* [out] */ Int32* port);

    CARAPI GetProtocol(
        /* [out] */ String* protocol);

    CARAPI GetRef(
        /* [out] */ String* ref);

    CARAPI GetQuery(
        /* [out] */ String* query);

    CARAPI GetPath(
        /* [out] */ String* path);

    CARAPI GetUserInfo(
        /* [out] */ String* userInfo);

    CARAPI GetAuthority(
        /* [out] */ String* authority);

    CARAPI GetDefaultPort(
        /* [out] */ Int32* port);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    // private void readObject(java.io.ObjectInputStream stream)

    // private void writeObject(ObjectOutputStream s)

    CARAPI_(void) FixURL(
        /* [in] */ Boolean fixHost);

    CARAPI_(void) SetupStreamHandler();

    CARAPI Set(
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& file,
        /* [in] */ const String& ref);

    CARAPI Set(
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& authority,
        /* [in] */ const String& userInfo,
        /* [in] */ const String& path,
        /* [in] */ const String& query,
        /* [in] */ const String& ref);

public:
    AutoPtr<IURLStreamHandler> mStreamHandler;

private:
//    static NetPermission specifyStreamHandlerPermission = new NetPermission(
//            "specifyStreamHandler");

    Int32 mHashCode;

    String mFile;

    String mProtocol;

    String mHost;

    Int32 mPort;

    String mAuthority;

    String mUserInfo;

    String mPath;

    String mQuery;

    String mRef;

    static HashMap<String, AutoPtr<IURLStreamHandler> > sStreamHandlers;

    static AutoPtr<IURLStreamHandlerFactory> sStreamHandlerFactory;

    static Mutex sLock;
};

} // namespace Net
} // namespace Elastos

#endif //__CURL_H__
