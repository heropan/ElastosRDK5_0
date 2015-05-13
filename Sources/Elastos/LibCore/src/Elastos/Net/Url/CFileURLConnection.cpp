
#include "CFileURLConnection.h"

namespace Elastos {
namespace Net {
namespace Url {

PInterface CFileURLConnection::Probe(
    /* [in] */ REIID riid)
{
    return _CFileURLConnection::Probe(riid);
}

ECode CFileURLConnection::constructor(
    /* [in] */ IURL* url)
{
    return FileURLConnection::Init(url);
}

ECode CFileURLConnection::Connect()
{
    return FileURLConnection::Connect();
}

ECode CFileURLConnection::GetAllowUserInteraction(
    /* [out] */ Boolean* value)
{
    return FileURLConnection::GetAllowUserInteraction(value);
}

ECode CFileURLConnection::GetContent(
    /* [out] */ IInterface** content)
{
    return FileURLConnection::GetContent(content);
}

//  public Object getContent(Class[] types);

ECode CFileURLConnection::GetContentEncoding(
    /* [out] */ String* encoding)
{
    return FileURLConnection::GetContentEncoding(encoding);
}

ECode CFileURLConnection::GetContentLength(
    /* [out] */ Int32* length)
{
    return FileURLConnection::GetContentLength(length);
}

ECode CFileURLConnection::GetContentType(
    /* [out] */ String* type)
{
    return FileURLConnection::GetContentType(type);
}

ECode CFileURLConnection::GetDate(
    /* [out] */ Int64* date)
{
    return FileURLConnection::GetDate(date);
}

ECode CFileURLConnection::GetDefaultUseCaches(
    /* [out] */ Boolean* value)
{
    return FileURLConnection::GetDefaultUseCaches(value);
}

ECode CFileURLConnection::GetDoInput(
    /* [out] */ Boolean* allowed)
{
    return FileURLConnection::GetDoInput(allowed);
}

ECode CFileURLConnection::GetDoOutput(
    /* [out] */ Boolean* allowed)
{
    return FileURLConnection::GetDoOutput(allowed);
}

ECode CFileURLConnection::GetExpiration(
    /* [out] */ Int64* expiration)
{
    return FileURLConnection::GetExpiration(expiration);
}

ECode CFileURLConnection::GetHeaderFieldByPosition(
    /* [in] */ Int32 pos,
    /* [out] */ String* value)
{
    return FileURLConnection::GetHeaderFieldByPosition(pos, value);
}

//  public Map<String, List<String>> getHeaderFields()

//  public Map<String, List<String>> getRequestProperties()

ECode CFileURLConnection::AddRequestProperty(
    /* [in] */ const String& field,
    /* [in] */ const String& newValue)
{
    return FileURLConnection::AddRequestProperty(field, newValue);
}

ECode CFileURLConnection::GetHeaderFieldByKey(
    /* [in] */ const String& key,
    /* [out] */ String* value)
{
    return FileURLConnection::GetHeaderFieldByKey(key, value);
}

ECode CFileURLConnection::GetHeaderFieldDate(
    /* [in] */ const String& field,
    /* [in] */ Int64 defaultValue,
    /* [out] */ Int64* value)
{
    return FileURLConnection::GetHeaderFieldDate(field, defaultValue, value);
}

ECode CFileURLConnection::GetHeaderFieldInt32(
    /* [in] */ const String& field,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32* value)
{
    return FileURLConnection::GetHeaderFieldInt32(field, defaultValue, value);
}

ECode CFileURLConnection::GetHeaderFieldKey(
    /* [in] */ Int32 posn,
    /* [out] */ String* key)
{
    return FileURLConnection::GetHeaderFieldKey(posn, key);
}

ECode CFileURLConnection::GetIfModifiedSince(
    /* [out] */ Int64* time)
{
    return FileURLConnection::GetIfModifiedSince(time);
}

ECode CFileURLConnection::GetInputStream(
    /* [out] */ IInputStream** is)
{
    return FileURLConnection::GetInputStream(is);
}

ECode CFileURLConnection::GetLastModified(
    /* [out] */ Int64* value)
{
    return FileURLConnection::GetLastModified(value);
}

ECode CFileURLConnection::GetOutputStream(
    /* [out] */ IOutputStream** os)
{
    return FileURLConnection::GetOutputStream(os);
}

//  public java.security.Permission getPermission();

ECode CFileURLConnection::GetRequestProperty(
    /* [in] */ const String& field,
    /* [out] */ String* property)
{
    return FileURLConnection::GetRequestProperty(field, property);
}

ECode CFileURLConnection::GetURL(
    /* [out] */ IURL** url)
{
    return FileURLConnection::GetURL(url);
}

ECode CFileURLConnection::GetUseCaches(
    /* [out] */ Boolean* allowed)
{
    return FileURLConnection::GetUseCaches(allowed);
}

ECode CFileURLConnection::SetAllowUserInteraction(
    /* [in] */ Boolean newValue)
{
    return FileURLConnection::SetAllowUserInteraction(newValue);
}

ECode CFileURLConnection::SetDefaultUseCaches(
    /* [in] */ Boolean newValue)
{
    return FileURLConnection::SetDefaultUseCaches(newValue);
}

ECode CFileURLConnection::SetDoInput(
    /* [in] */ Boolean newValue)
{
    return FileURLConnection::SetDoInput(newValue);
}

ECode CFileURLConnection::SetDoOutput(
    /* [in] */ Boolean newValue)
{
    return FileURLConnection::SetDoOutput(newValue);
}

ECode CFileURLConnection::SetIfModifiedSince(
    /* [in] */ Int64 newValue)
{
    return FileURLConnection::SetIfModifiedSince(newValue);
}

ECode CFileURLConnection::SetRequestProperty(
    /* [in] */ const String& field,
    /* [in] */ const String& newValue)
{
    return FileURLConnection::SetRequestProperty(field, newValue);
}

ECode CFileURLConnection::SetUseCaches(
    /* [in] */ Boolean newValue)
{
    return FileURLConnection::SetUseCaches(newValue);
}

ECode CFileURLConnection::SetConnectTimeout(
    /* [in] */ Int32 timeout)
{
    return FileURLConnection::SetConnectTimeout(timeout);
}

ECode CFileURLConnection::GetConnectTimeout(
    /* [out] */ Int32* timeout)
{
    return FileURLConnection::GetConnectTimeout(timeout);
}

ECode CFileURLConnection::SetReadTimeout(
    /* [in] */ Int32 timeout)
{
    return FileURLConnection::SetReadTimeout(timeout);
}

ECode CFileURLConnection::GetReadTimeout(
    /* [out] */ Int32* timeout)
{
    return FileURLConnection::GetReadTimeout(timeout);
}

} // namespace Url
} // namespace Net
} // namespace Elastos
