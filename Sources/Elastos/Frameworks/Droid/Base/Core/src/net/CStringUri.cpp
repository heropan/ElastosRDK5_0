
#include "net/CStringUri.h"

namespace Elastos {
namespace Droid {
namespace Net {

ECode CStringUri::constructor()
{
    return NOERROR;
}

ECode CStringUri::constructor(
    /* [in] */ const String& uriString)
{
    return StringUri::Init(uriString);
}

PInterface CStringUri::Probe(
    /* [in] */ REIID riid)
{
    return _CStringUri::Probe(riid);
}

ECode CStringUri::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return StringUri::ReadFromParcel(source);
}

ECode CStringUri::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return StringUri::WriteToParcel(dest);
}

ECode CStringUri::IsHierarchical(
    /* [out] */ Boolean* isHierarchical)
{
    return StringUri::IsHierarchical(isHierarchical);
}

ECode CStringUri::IsOpaque(
    /* [out] */ Boolean* isOpaque)
{
    return StringUri::IsOpaque(isOpaque);
}

ECode CStringUri::IsRelative(
    /* [out] */ Boolean* isRelative)
{
    return StringUri::IsRelative(isRelative);
}

ECode CStringUri::IsAbsolute(
    /* [out] */ Boolean* isAbsolute)
{
    return StringUri::IsAbsolute(isAbsolute);
}

ECode CStringUri::GetScheme(
    /* [out] */ String* scheme)
{
    return StringUri::GetScheme(scheme);
}

ECode CStringUri::GetSchemeSpecificPart(
    /* [out] */ String* ssp)
{
    return StringUri::GetSchemeSpecificPart(ssp);
}

ECode CStringUri::GetEncodedSchemeSpecificPart(
    /* [out] */ String* essp)
{
    return StringUri::GetEncodedSchemeSpecificPart(essp);
}

ECode CStringUri::GetAuthority(
    /* [out] */ String* authority)
{
    return StringUri::GetAuthority(authority);
}

ECode CStringUri::GetEncodedAuthority(
    /* [out] */ String* authority)
{
    return StringUri::GetEncodedAuthority(authority);
}

ECode CStringUri::GetUserInfo(
    /* [out] */ String* userInfo)
{
    return StringUri::GetUserInfo(userInfo);
}

ECode CStringUri::GetEncodedUserInfo(
    /* [out] */ String* userInfo)
{
    return StringUri::GetEncodedUserInfo(userInfo);
}

ECode CStringUri::GetHost(
    /* [out] */ String* host)
{
    return StringUri::GetHost(host);
}

ECode CStringUri::GetPort(
    /* [out] */ Int32* port)
{
    return StringUri::GetPort(port);
}

ECode CStringUri::GetPath(
    /* [out] */ String* path)
{
    return StringUri::GetPath(path);
}

ECode CStringUri::GetEncodedPath(
    /* [out] */ String* path)
{
    return StringUri::GetEncodedPath(path);
}

ECode CStringUri::GetQuery(
    /* [out] */ String* query)
{
    return StringUri::GetQuery(query);
}

ECode CStringUri::GetEncodedQuery(
    /* [out] */ String* query)
{
    return StringUri::GetEncodedQuery(query);
}

ECode CStringUri::GetFragment(
    /* [out] */ String* fragment)
{
    return StringUri::GetFragment(fragment);
}

ECode CStringUri::GetEncodedFragment(
    /* [out] */ String* fragment)
{
    return StringUri::GetEncodedFragment(fragment);
}

ECode CStringUri::GetPathSegments(
    /* [out, callee] */ ArrayOf<String>** pathSegments)
{
    return StringUri::GetPathSegments(pathSegments);
}

ECode CStringUri::GetLastPathSegment(
    /* [out] */ String* pathSegment)
{
    return StringUri::GetLastPathSegment(pathSegment);
}

ECode CStringUri::Equals(
    /* [in]*/ IInterface* other,
    /* [out] */ Boolean* isEqual)
{
    return StringUri::Equals(IUri::Probe(other), isEqual);
}

ECode CStringUri::Equals(
    /* [in]*/ IUri* other,
    /* [out] */ Boolean* isEqual)
{
    return StringUri::Equals(other, isEqual);
}

ECode CStringUri::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return StringUri::GetHashCode(hashCode);
}

ECode CStringUri::CompareTo(
    /* [in] */ IUri* other,
    /* [out] */ Int32* result)
{
    return StringUri::CompareTo(other, result);
}

ECode CStringUri::ToString(
    /* [out] */ String* result)
{
    return StringUri::ToString(result);
}

ECode CStringUri::ToSafeString(
    /* [out] */ String* result)
{
    return StringUri::ToSafeString(result);
}

ECode CStringUri::BuildUpon(
    /* [out] */ IUriBuilder** result)
{
    return StringUri::BuildUpon(result);
}

ECode CStringUri::GetQueryParameterNames(
    /* [out, callee] */ ArrayOf<String>** aSet)
{
    return StringUri::GetQueryParameterNames(aSet);
}

ECode CStringUri::GetQueryParameters(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<String>** aList)
{
    return StringUri::GetQueryParameters(key, aList);
}

ECode CStringUri::GetQueryParameter(
    /* [in] */ const String& key,
    /* [out] */ String* param)
{
    return StringUri::GetQueryParameter(key, param);
}

ECode CStringUri::GetBooleanQueryParameter(
    /* [in] */ const String& key,
    /* [in] */ Boolean defaultValue,
    /* [out] */ Boolean* result)
{
    return StringUri::GetBooleanQueryParameter(key, defaultValue, result);
}

ECode CStringUri::NormalizeScheme(
    /* [out] */ IUri** uri)
{
    return StringUri::NormalizeScheme(uri);
}

ECode CStringUri::GetCanonicalUri(
    /* [out] */ IUri** uri)
{
    return StringUri::GetCanonicalUri(uri);
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
