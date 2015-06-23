
#ifndef __CSTRINGURI_H__
#define __CSTRINGURI_H__

#include "_CStringUri.h"
#include "net/Uri.h"

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CStringUri), public StringUri
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& uriString);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI IsHierarchical(
        /* [out] */ Boolean* isHierarchical);

    CARAPI IsOpaque(
        /* [out] */ Boolean* isOpaque);

    CARAPI IsRelative(
        /* [out] */ Boolean* isRelative);

    CARAPI IsAbsolute(
        /* [out] */ Boolean* isAbsolute);

    CARAPI GetScheme(
        /* [out] */ String* scheme);

    CARAPI GetSchemeSpecificPart(
        /* [out] */ String* ssp);

    CARAPI GetEncodedSchemeSpecificPart(
        /* [out] */ String* essp);

    CARAPI GetAuthority(
        /* [out] */ String* authority);

    CARAPI GetEncodedAuthority(
        /* [out] */ String* authority);

    CARAPI GetUserInfo(
        /* [out] */ String* userInfo);

    CARAPI GetEncodedUserInfo(
        /* [out] */ String* userInfo);

    CARAPI GetHost(
        /* [out] */ String* host);

    CARAPI GetPort(
        /* [out] */ Int32* port);

    CARAPI GetPath(
        /* [out] */ String* path);

    CARAPI GetEncodedPath(
        /* [out] */ String* path);

    CARAPI GetQuery(
        /* [out] */ String* query);

    CARAPI GetEncodedQuery(
        /* [out] */ String* query);

    CARAPI GetFragment(
        /* [out] */ String* fragment);

    CARAPI GetEncodedFragment(
        /* [out] */ String* fragment);

    CARAPI GetPathSegments(
        /* [out, callee] */ ArrayOf<String>** pathSegments);

    CARAPI GetLastPathSegment(
        /* [out] */ String* pathSegment);

    CARAPI Equals(
        /* [in]*/ IInterface* other,
        /* [out] */ Boolean* isEqual);

    CARAPI Equals(
        /* [in]*/ IUri* other,
        /* [out] */ Boolean* isEqual);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI CompareTo(
        /* [in] */ IUri* other,
        /* [out] */ Int32* result);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI ToSafeString(
        /* [out] */ String* result);

    CARAPI BuildUpon(
        /* [out] */ IUriBuilder** result);

    CARAPI GetQueryParameterNames(
        /* [out, callee] */ ArrayOf<String>** aSet);

    CARAPI GetQueryParameters(
        /* [in] */ const String& key,
        /* [out, callee] */ ArrayOf<String>** aList);

    CARAPI GetQueryParameter(
        /* [in] */ const String& key,
        /* [out] */ String* param);

    CARAPI GetBooleanQueryParameter(
        /* [in] */ const String& key,
        /* [in] */ Boolean defaultValue,
        /* [out] */ Boolean* result);

    CARAPI NormalizeScheme(
        /* [out] */ IUri** uri);

    CARAPI GetCanonicalUri(
        /* [out] */ IUri** uri);
};

} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif //__CSTRINGURI_H__
