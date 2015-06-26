
#ifndef __ELASTOS_NET_URLCONNECTION_H__
#define __ELASTOS_NET_URLCONNECTION_H__

#include "Object.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;
using Elastos::Security::IPermission;
using Elastos::Utility::IMap;
using Elastos::Utility::Etl::HashMap;

namespace Elastos {
namespace Net {

class URLConnection
    : public Object
    , public IURLConnection
{
public:
    class DefaultContentHandler
        : public Object
        , public IContentHandler
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI GetContent(
            /* [in] */ IURLConnection* uConn,
            /* [out] */ IInterface** obj);

        CARAPI GetContent(
            /* [in] */ IURLConnection* uConn,
            /* [in] */ const ArrayOf<InterfaceID>& types,
            /* [out] */ IInterface** obj);
    };

public:
    CAR_INTERFACE_DECL()

    virtual CARAPI Connect() = 0;

    virtual CARAPI GetAllowUserInteraction(
        /* [out] */ Boolean* value);

    virtual CARAPI GetContent(
        /* [out] */ IInterface** content);

    // Param is not generic in spec
//        public Object getContent(Class[] types);

    virtual CARAPI GetContentEncoding(
        /* [out] */ String* encoding);

    virtual CARAPI GetContentLength(
        /* [out] */ Int32* length);

    virtual CARAPI GetContentType(
        /* [out] */ String* type);

    virtual CARAPI GetDate(
        /* [out] */ Int64* date);

    static CARAPI_(Boolean) GetDefaultAllowUserInteraction();

    static CARAPI_(String) GetDefaultRequestProperty(
        /* [in] */ const String& field);

    virtual CARAPI GetDefaultUseCaches(
        /* [out] */ Boolean* value);

    virtual CARAPI GetDoInput(
        /* [out] */ Boolean* allowed);

    virtual CARAPI GetDoOutput(
        /* [out] */ Boolean* allowed);

    virtual CARAPI GetExpiration(
        /* [out] */ Int64* expiration);

    static CARAPI_(AutoPtr<IFileNameMap>) GetFileNameMap();

    virtual CARAPI GetHeaderField(
        /* [in] */ const String& key,
        /* [out] */ String* value);

    virtual CARAPI GetHeaderField(
        /* [in] */ Int32 pos,
        /* [out] */ String* value);

    virtual CARAPI GetHeaderFields(
        /* [out] */ IMap** headerFields);

    virtual CARAPI GetRequestProperties(
        /* [out] */ IMap** properties);

    virtual CARAPI AddRequestProperty(
        /* [in] */ const String& field,
        /* [in] */ const String& newValue);

    virtual CARAPI GetHeaderFieldByKey(
        /* [in] */ const String& key,
        /* [out] */ String* value);

    virtual CARAPI GetHeaderFieldDate(
        /* [in] */ const String& field,
        /* [in] */ Int64 defaultValue,
        /* [out] */ Int64* value);

    virtual CARAPI GetHeaderFieldInt32(
        /* [in] */ const String& field,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32* value);

    virtual CARAPI GetHeaderFieldKey(
        /* [in] */ Int32 posn,
        /* [out] */ String* key);

    virtual CARAPI GetIfModifiedSince(
        /* [out] */ Int64* time);

    virtual CARAPI GetInputStream(
        /* [out] */ IInputStream** is);

    virtual CARAPI GetLastModified(
        /* [out] */ Int64* value);

    virtual CARAPI GetOutputStream(
        /* [out] */ IOutputStream** os);

    virtual CARAPI GetPermission(
        /* [out] */ IPermission** perm);

    virtual CARAPI GetRequestProperty(
        /* [in] */ const String& field,
        /* [out] */ String* property);

    virtual CARAPI GetURL(
        /* [out] */ IURL** url);

    virtual CARAPI GetUseCaches(
        /* [out] */ Boolean* allowed);

    static CARAPI_(String) GuessContentTypeFromName(
        /* [in] */ const String& url);

    static CARAPI_(String) GuessContentTypeFromStream(
        /* [in] */ IInputStream* is);

    virtual CARAPI SetAllowUserInteraction(
        /* [in] */ Boolean newValue);

    static CARAPI SetContentHandlerFactory(
        /* [in] */ IContentHandlerFactory* contentFactory);

    static CARAPI_(void) SetDefaultAllowUserInteraction(
        /* [in] */ Boolean allows);

    static CARAPI_(void) SetDefaultRequestProperty(
        /* [in] */ const String& field,
        /* [in] */ const String& value);

    virtual CARAPI SetDefaultUseCaches(
        /* [in] */ Boolean newValue);

    virtual CARAPI SetDoInput(
        /* [in] */ Boolean newValue);

    virtual CARAPI SetDoOutput(
        /* [in] */ Boolean newValue);

    static CARAPI_(void) SetFileNameMap(
        /* [in] */ IFileNameMap* map);

    virtual CARAPI SetIfModifiedSince(
        /* [in] */ Int64 newValue);

    virtual CARAPI SetRequestProperty(
        /* [in] */ const String& field,
        /* [in] */ const String& newValue);

    virtual CARAPI SetUseCaches(
        /* [in] */ Boolean newValue);

    virtual CARAPI SetConnectTimeout(
        /* [in] */ Int32 timeout);

    virtual CARAPI GetConnectTimeout(
        /* [out] */ Int32* timeout);

    virtual CARAPI SetReadTimeout(
        /* [in] */ Int32 timeout);

    virtual CARAPI GetReadTimeout(
        /* [out] */ Int32* timeout);

protected:
    URLConnection();

    virtual ~URLConnection();

    CARAPI constructor(
        /* [in] */ IURL* url);

private:
    CARAPI GetContentHandler(
        /* [in] */ const String& type,
        /* [out] */ IContentHandler** contentHandler);

    CARAPI CheckNotConnected();

    CARAPI_(String) ParseTypeString(
        /* [in] */ const String& typeString);

public:
    AutoPtr<IContentHandler> mDefaultHandler;

    // static Hashtable<String, Object> contentHandlers = new Hashtable<String, Object>();
    static HashMap<String, AutoPtr<IContentHandler> > sContentHandlers;

protected:
    AutoPtr<IURL> mUrl;

    Int64 mIfModifiedSince;

    Boolean mUseCaches;// = defaultUseCaches;

    Boolean mConnected;

    Boolean mDoOutput;

    Boolean mDoInput;

    Boolean mAllowUserInteraction;

private:
    String mContentType;

    static Boolean sDefaultAllowUserInteraction;

    static Boolean sDefaultUseCaches;

    static Object sLock;

    Int64 mLastModified;

    static AutoPtr<IContentHandlerFactory> sContentHandlerFactory;

    Int32 mReadTimeout;

    Int32 mConnectTimeout;

    static AutoPtr<IFileNameMap> sFileNameMap;
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_URLCONNECTION_H__
