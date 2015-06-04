
#ifndef __CFILEURLCONNECTION_H__
#define __CFILEURLCONNECTION_H__

#include "_CFileURLConnection.h"
#include "FileURLConnection.h"

using Elastos::Net::IURLConnection;
using Elastos::Net::IURL;

namespace Libcore {
namespace Net {
namespace Url {

CarClass(CFileURLConnection), public FileURLConnection
{
public:
    CARAPI constructor(
        /* [in] */ IURL* url);

    CARAPI Connect();

    CARAPI GetAllowUserInteraction(
        /* [out] */ Boolean* value);

    CARAPI GetContent(
        /* [out] */ IInterface** content);

//  public Object getContent(Class[] types);

    CARAPI GetContentEncoding(
        /* [out] */ String* encoding);

    CARAPI GetContentLength(
        /* [out] */ Int32* length);

    CARAPI GetContentType(
        /* [out] */ String* type);

    CARAPI GetDate(
        /* [out] */ Int64* date);

    CARAPI GetDefaultUseCaches(
        /* [out] */ Boolean* value);

    CARAPI GetDoInput(
        /* [out] */ Boolean* allowed);

    CARAPI GetDoOutput(
        /* [out] */ Boolean* allowed);

    CARAPI GetExpiration(
        /* [out] */ Int64* expiration);

    CARAPI GetHeaderFieldByPosition(
        /* [in] */ Int32 pos,
        /* [out] */ String* value);

//  public Map<String, List<String>> getHeaderFields()

//  public Map<String, List<String>> getRequestProperties()

    CARAPI AddRequestProperty(
        /* [in] */ const String& field,
        /* [in] */ const String& newValue);

    CARAPI GetHeaderFieldByKey(
        /* [in] */ const String& key,
        /* [out] */ String* value);

    CARAPI GetHeaderFieldDate(
        /* [in] */ const String& field,
        /* [in] */ Int64 defaultValue,
        /* [out] */ Int64* value);

    CARAPI GetHeaderFieldInt32(
        /* [in] */ const String& field,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32* value);

    CARAPI GetHeaderFieldKey(
        /* [in] */ Int32 posn,
        /* [out] */ String* key);

    CARAPI GetIfModifiedSince(
        /* [out] */ Int64* time);

    CARAPI GetInputStream(
        /* [out] */ IInputStream** is);

    CARAPI GetLastModified(
        /* [out] */ Int64* value);

    CARAPI GetOutputStream(
        /* [out] */ IOutputStream** os);

//  public java.security.Permission getPermission();

    CARAPI GetRequestProperty(
        /* [in] */ const String& field,
        /* [out] */ String* property);

    CARAPI GetURL(
        /* [out] */ IURL** url);

    CARAPI GetUseCaches(
        /* [out] */ Boolean* allowed);

    CARAPI SetAllowUserInteraction(
        /* [in] */ Boolean newValue);

    CARAPI SetDefaultUseCaches(
        /* [in] */ Boolean newValue);

    CARAPI SetDoInput(
        /* [in] */ Boolean newValue);

    CARAPI SetDoOutput(
        /* [in] */ Boolean newValue);

    CARAPI SetIfModifiedSince(
        /* [in] */ Int64 newValue);

    CARAPI SetRequestProperty(
        /* [in] */ const String& field,
        /* [in] */ const String& newValue);

    CARAPI SetUseCaches(
        /* [in] */ Boolean newValue);

    CARAPI SetConnectTimeout(
        /* [in] */ Int32 timeout);

    CARAPI GetConnectTimeout(
        /* [out] */ Int32* timeout);

    CARAPI SetReadTimeout(
        /* [in] */ Int32 timeout);

    CARAPI GetReadTimeout(
        /* [out] */ Int32* timeout);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

} // namespace Url
} // namespace Net
} // namespace Libcore

#endif //__CFILEURLCONNECTION_H__
