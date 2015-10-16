
#include "elastos/droid/net/CHierarchicalUri.h"

namespace Elastos {
namespace Droid {
namespace Net {

ECode CHierarchicalUri::constructor()
{
    return NOERROR;
}

ECode CHierarchicalUri::constructor(
    /* [in] */ const String& scheme,
    /* [in] */ Handle32 authority,
    /* [in] */ Handle32 path,
    /* [in] */ Handle32 query,
    /* [in] */ Handle32 fragment)
{
    return HierarchicalUri::Init(scheme, authority, path, query, fragment);
}

PInterface CHierarchicalUri::Probe(
    /* [in] */ REIID riid)
{
    return _CHierarchicalUri::Probe(riid);
}

ECode CHierarchicalUri::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return HierarchicalUri::ReadFromParcel(source);
}

ECode CHierarchicalUri::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return HierarchicalUri::WriteToParcel(dest);
}

ECode CHierarchicalUri::IsHierarchical(
    /* [out] */ Boolean* isHierarchical)
{
    return HierarchicalUri::IsHierarchical(isHierarchical);
}

ECode CHierarchicalUri::IsOpaque(
    /* [out] */ Boolean* isOpaque)
{
    return HierarchicalUri::IsOpaque(isOpaque);
}

ECode CHierarchicalUri::IsRelative(
    /* [out] */ Boolean* isRelative)
{
    return HierarchicalUri::IsRelative(isRelative);
}

ECode CHierarchicalUri::IsAbsolute(
    /* [out] */ Boolean* isAbsolute)
{
    return HierarchicalUri::IsAbsolute(isAbsolute);
}

ECode CHierarchicalUri::GetScheme(
    /* [out] */ String* scheme)
{
    return HierarchicalUri::GetScheme(scheme);
}

ECode CHierarchicalUri::GetSchemeSpecificPart(
    /* [out] */ String* ssp)
{
    return HierarchicalUri::GetSchemeSpecificPart(ssp);
}

ECode CHierarchicalUri::GetEncodedSchemeSpecificPart(
    /* [out] */ String* essp)
{
    return HierarchicalUri::GetEncodedSchemeSpecificPart(essp);
}

ECode CHierarchicalUri::GetAuthority(
    /* [out] */ String* authority)
{
    return HierarchicalUri::GetAuthority(authority);
}

ECode CHierarchicalUri::GetEncodedAuthority(
    /* [out] */ String* authority)
{
    return HierarchicalUri::GetEncodedAuthority(authority);
}

ECode CHierarchicalUri::GetUserInfo(
    /* [out] */ String* userInfo)
{
    return HierarchicalUri::GetUserInfo(userInfo);
}

ECode CHierarchicalUri::GetEncodedUserInfo(
    /* [out] */ String* userInfo)
{
    return HierarchicalUri::GetEncodedUserInfo(userInfo);
}

ECode CHierarchicalUri::GetHost(
    /* [out] */ String* host)
{
    return HierarchicalUri::GetHost(host);
}

ECode CHierarchicalUri::GetPort(
    /* [out] */ Int32* port)
{
    return HierarchicalUri::GetPort(port);
}

ECode CHierarchicalUri::GetPath(
    /* [out] */ String* path)
{
    return HierarchicalUri::GetPath(path);
}

ECode CHierarchicalUri::GetEncodedPath(
    /* [out] */ String* path)
{
    return HierarchicalUri::GetEncodedPath(path);
}

ECode CHierarchicalUri::GetQuery(
    /* [out] */ String* query)
{
    return HierarchicalUri::GetQuery(query);
}

ECode CHierarchicalUri::GetEncodedQuery(
    /* [out] */ String* query)
{
    return HierarchicalUri::GetEncodedQuery(query);
}

ECode CHierarchicalUri::GetFragment(
    /* [out] */ String* fragment)
{
    return HierarchicalUri::GetFragment(fragment);
}

ECode CHierarchicalUri::GetEncodedFragment(
    /* [out] */ String* fragment)
{
    return HierarchicalUri::GetEncodedFragment(fragment);
}

ECode CHierarchicalUri::GetPathSegments(
    /* [out, callee] */ ArrayOf<String>** pathSegments)
{
    return HierarchicalUri::GetPathSegments(pathSegments);
}

ECode CHierarchicalUri::GetLastPathSegment(
    /* [out] */ String* pathSegment)
{
    return HierarchicalUri::GetLastPathSegment(pathSegment);
}

ECode CHierarchicalUri::Equals(
    /* [in]*/ IInterface* other,
    /* [out] */ Boolean* isEqual)
{
    return HierarchicalUri::Equals(IUri::Probe(other), isEqual);
}

ECode CHierarchicalUri::Equals(
    /* [in]*/ IUri* other,
    /* [out] */ Boolean* isEqual)
{
    return HierarchicalUri::Equals(other, isEqual);
}

ECode CHierarchicalUri::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return HierarchicalUri::GetHashCode(hashCode);
}

ECode CHierarchicalUri::CompareTo(
    /* [in] */ IUri* other,
    /* [out] */ Int32* result)
{
    return HierarchicalUri::CompareTo(other, result);
}

ECode CHierarchicalUri::ToString(
    /* [out] */ String* result)
{
    return HierarchicalUri::ToString(result);
}

ECode CHierarchicalUri::ToSafeString(
    /* [out] */ String* result)
{
    return HierarchicalUri::ToSafeString(result);
}

ECode CHierarchicalUri::BuildUpon(
    /* [out] */ IUriBuilder** result)
{
    return HierarchicalUri::BuildUpon(result);
}

ECode CHierarchicalUri::GetQueryParameterNames(
    /* [out, callee] */ ArrayOf<String>** aSet)
{
    return HierarchicalUri::GetQueryParameterNames(aSet);
}

ECode CHierarchicalUri::GetQueryParameters(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<String>** aList)
{
    return HierarchicalUri::GetQueryParameters(key, aList);
}

ECode CHierarchicalUri::GetQueryParameter(
    /* [in] */ const String& key,
    /* [out] */ String* param)
{
    return HierarchicalUri::GetQueryParameter(key, param);
}

ECode CHierarchicalUri::GetBooleanQueryParameter(
    /* [in] */ const String& key,
    /* [in] */ Boolean defaultValue,
    /* [out] */ Boolean* result)
{
    return HierarchicalUri::GetBooleanQueryParameter(key, defaultValue, result);
}

ECode CHierarchicalUri::NormalizeScheme(
    /* [out] */ IUri** uri)
{
    return HierarchicalUri::NormalizeScheme(uri);
}

ECode CHierarchicalUri::GetCanonicalUri(
    /* [out] */ IUri** uri)
{
    return HierarchicalUri::GetCanonicalUri(uri);
}


} // namespace Net
} // namespace Droid
} // namespace Elastos
