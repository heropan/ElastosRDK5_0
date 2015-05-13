
#include "net/COpaqueUri.h"

namespace Elastos {
namespace Droid {
namespace Net {

ECode COpaqueUri::constructor()
{
    return NOERROR;
}

ECode COpaqueUri::constructor(
    /* [in] */ const String& scheme,
    /* [in] */ Handle32 ssp,
    /* [in] */ Handle32 fragment)
{
    return OpaqueUri::Init(scheme, ssp, fragment);
}

PInterface COpaqueUri::Probe(
    /* [in] */ REIID riid)
{
    return _COpaqueUri::Probe(riid);
}

ECode COpaqueUri::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return OpaqueUri::ReadFromParcel(source);
}

ECode COpaqueUri::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return OpaqueUri::WriteToParcel(dest);
}

ECode COpaqueUri::IsHierarchical(
    /* [out] */ Boolean* isHierarchical)
{
    return OpaqueUri::IsHierarchical(isHierarchical);
}

ECode COpaqueUri::IsOpaque(
    /* [out] */ Boolean* isOpaque)
{
    return OpaqueUri::IsOpaque(isOpaque);
}

ECode COpaqueUri::IsRelative(
    /* [out] */ Boolean* isRelative)
{
    return OpaqueUri::IsRelative(isRelative);
}

ECode COpaqueUri::IsAbsolute(
    /* [out] */ Boolean* isAbsolute)
{
    return OpaqueUri::IsAbsolute(isAbsolute);
}

ECode COpaqueUri::GetScheme(
    /* [out] */ String* scheme)
{
    return OpaqueUri::GetScheme(scheme);
}

ECode COpaqueUri::GetSchemeSpecificPart(
    /* [out] */ String* ssp)
{
    return OpaqueUri::GetSchemeSpecificPart(ssp);
}

ECode COpaqueUri::GetEncodedSchemeSpecificPart(
    /* [out] */ String* essp)
{
    return OpaqueUri::GetEncodedSchemeSpecificPart(essp);
}

ECode COpaqueUri::GetAuthority(
    /* [out] */ String* authority)
{
    return OpaqueUri::GetAuthority(authority);
}

ECode COpaqueUri::GetEncodedAuthority(
    /* [out] */ String* authority)
{
    return OpaqueUri::GetEncodedAuthority(authority);
}

ECode COpaqueUri::GetUserInfo(
    /* [out] */ String* userInfo)
{
    return OpaqueUri::GetUserInfo(userInfo);
}

ECode COpaqueUri::GetEncodedUserInfo(
    /* [out] */ String* userInfo)
{
    return OpaqueUri::GetEncodedUserInfo(userInfo);
}

ECode COpaqueUri::GetHost(
    /* [out] */ String* host)
{
    return OpaqueUri::GetHost(host);
}

ECode COpaqueUri::GetPort(
    /* [out] */ Int32* port)
{
    return OpaqueUri::GetPort(port);
}

ECode COpaqueUri::GetPath(
    /* [out] */ String* path)
{
    return OpaqueUri::GetPath(path);
}

ECode COpaqueUri::GetEncodedPath(
    /* [out] */ String* path)
{
    return OpaqueUri::GetEncodedPath(path);
}

ECode COpaqueUri::GetQuery(
    /* [out] */ String* query)
{
    return OpaqueUri::GetQuery(query);
}

ECode COpaqueUri::GetEncodedQuery(
    /* [out] */ String* query)
{
    return OpaqueUri::GetEncodedQuery(query);
}

ECode COpaqueUri::GetFragment(
    /* [out] */ String* fragment)
{
    return OpaqueUri::GetFragment(fragment);
}

ECode COpaqueUri::GetEncodedFragment(
    /* [out] */ String* fragment)
{
    return OpaqueUri::GetEncodedFragment(fragment);
}

ECode COpaqueUri::GetPathSegments(
    /* [out, callee] */ ArrayOf<String>** pathSegments)
{
    return OpaqueUri::GetPathSegments(pathSegments);
}

ECode COpaqueUri::GetLastPathSegment(
    /* [out] */ String* pathSegment)
{
    return OpaqueUri::GetLastPathSegment(pathSegment);
}

ECode COpaqueUri::Equals(
    /* [in]*/ IInterface* other,
    /* [out] */ Boolean* isEqual)
{
    return OpaqueUri::Equals(IUri::Probe(other), isEqual);
}

ECode COpaqueUri::EqualsEx(
    /* [in]*/ IUri* other,
    /* [out] */ Boolean* isEqual)
{
    return OpaqueUri::Equals(other, isEqual);
}

ECode COpaqueUri::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return OpaqueUri::GetHashCode(hashCode);
}

ECode COpaqueUri::CompareTo(
    /* [in] */ IUri* other,
    /* [out] */ Int32* result)
{
    return OpaqueUri::CompareTo(other, result);
}

ECode COpaqueUri::ToString(
    /* [out] */ String* result)
{
    return OpaqueUri::ToString(result);
}

ECode COpaqueUri::ToSafeString(
    /* [out] */ String* result)
{
    return OpaqueUri::ToSafeString(result);
}

ECode COpaqueUri::BuildUpon(
    /* [out] */ IUriBuilder** result)
{
    return OpaqueUri::BuildUpon(result);
}

ECode COpaqueUri::GetQueryParameterNames(
    /* [out, callee] */ ArrayOf<String>** aSet)
{
    return OpaqueUri::GetQueryParameterNames(aSet);
}

ECode COpaqueUri::GetQueryParameters(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<String>** aList)
{
    return OpaqueUri::GetQueryParameters(key, aList);
}

ECode COpaqueUri::GetQueryParameter(
    /* [in] */ const String& key,
    /* [out] */ String* param)
{
    return OpaqueUri::GetQueryParameter(key, param);
}

ECode COpaqueUri::GetBooleanQueryParameter(
    /* [in] */ const String& key,
    /* [in] */ Boolean defaultValue,
    /* [out] */ Boolean* result)
{
    return OpaqueUri::GetBooleanQueryParameter(key, defaultValue, result);
}

ECode COpaqueUri::NormalizeScheme(
    /* [out] */ IUri** uri)
{
    return OpaqueUri::NormalizeScheme(uri);
}

ECode COpaqueUri::GetCanonicalUri(
    /* [out] */ IUri** uri)
{
    return OpaqueUri::GetCanonicalUri(uri);
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
