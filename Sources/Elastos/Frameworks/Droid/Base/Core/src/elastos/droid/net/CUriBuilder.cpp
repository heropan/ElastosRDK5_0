
#include "elastos/droid/net/CHierarchicalUri.h"
#include "elastos/droid/net/COpaqueUri.h"
#include "elastos/droid/net/CUriBuilder.h"

namespace Elastos {
namespace Droid {
namespace Net {

ECode CUriBuilder::Scheme(
    /* [in] */ const String& scheme)
{
    mScheme = scheme;
    return NOERROR;
}

ECode CUriBuilder::OpaquePart(
    /* [in] */ Handle32 opaquePart)
{
    mOpaquePart = (Uri::Part*)opaquePart;
    return NOERROR;
}

ECode CUriBuilder::OpaquePart(
    /* [in] */ const String& opaquePart)
{
    AutoPtr<Uri::Part> part = Uri::Part::FromDecoded(opaquePart);
    mOpaquePart = part;
    return NOERROR;
}

ECode CUriBuilder::EncodedOpaquePart(
    /* [in] */ const String& opaquePart)
{
    AutoPtr<Uri::Part> part = Uri::Part::FromDecoded(opaquePart);
    mOpaquePart = part;
    return NOERROR;
}

ECode CUriBuilder::Authority(
    /* [in] */ Handle32 authority)
{
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    mAuthority = (Uri::Part*)authority;
    return NOERROR;
}

ECode CUriBuilder::Authority(
    /* [in] */ const String& authority)
{
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    AutoPtr<Uri::Part> part = Uri::Part::FromDecoded(authority);
    mAuthority = part;
    return NOERROR;
}

ECode CUriBuilder::EncodedAuthority(
    /* [in] */ const String& authority)
{
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    AutoPtr<Uri::Part> part = Uri::Part::FromDecoded(authority);
    mAuthority = part;
    return NOERROR;
}

ECode CUriBuilder::Path(
    /* [in] */ Handle32 path)
{
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    mPath = (Uri::PathPart*)path;
    return NOERROR;
}

ECode CUriBuilder::Path(
    /* [in] */ const String& path)
{
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    AutoPtr<Uri::PathPart> part = Uri::PathPart::FromDecoded(path);
    mPath = part;
    return NOERROR;
}

ECode CUriBuilder::EncodedPath(
    /* [in] */ const String& path)
{
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    AutoPtr<Uri::PathPart> part = Uri::PathPart::FromDecoded(path);
    mPath = part;
    return NOERROR;
}

ECode CUriBuilder::AppendPath(
    /* [in] */ const String& newSegment)
{
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    AutoPtr<Uri::PathPart> part = Uri::PathPart::AppendDecodedSegment(mPath, newSegment);
    mPath = part;
    return NOERROR;
}

ECode CUriBuilder::AppendEncodedPath(
    /* [in] */ const String& newSegment)
{
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    AutoPtr<Uri::PathPart> part = Uri::PathPart::AppendEncodedSegment(mPath, newSegment);
    mPath = part;
    return NOERROR;
}

ECode CUriBuilder::Query(
    /* [in] */ Handle32 query)
{
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    mQuery = (Uri::Part*)query;
    return NOERROR;
}

ECode CUriBuilder::Query(
    /* [in] */ const String& query)
{
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    AutoPtr<Uri::Part> part = Uri::Part::FromDecoded(query);
    mQuery = part;
    return NOERROR;
}

ECode CUriBuilder::EncodedQuery(
    /* [in] */ const String& query)
{
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    AutoPtr<Uri::Part> part = Uri::Part::FromDecoded(query);
    mQuery = part;
    return NOERROR;
}

ECode CUriBuilder::Fragment(
    /* [in] */ Handle32 fragment)
{
    mFragment = (Uri::Part*)fragment;
    return NOERROR;
}

ECode CUriBuilder::Fragment(
    /* [in] */ const String& fragment)
{
    AutoPtr<Uri::Part> part = Uri::Part::FromDecoded(fragment);
    mFragment = part;
    return NOERROR;
}

ECode CUriBuilder::EncodedFragment(
    /* [in] */ const String& fragment)
{
    AutoPtr<Uri::Part> part = Uri::Part::FromDecoded(fragment);
    mFragment = part;
    return NOERROR;
}

ECode CUriBuilder::AppendQueryParameter(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    // This URI will be hierarchical.
    mOpaquePart = NULL;

    String nullStr;
    String keyParam, valueParam;
    Uri::Encode(key, nullStr, &keyParam);
    Uri::Encode(value, nullStr, &valueParam);

    StringBuilder sb(keyParam);
    sb += "=";
    sb += valueParam;
    String encodedParameter = sb.ToString();

    if (mQuery == NULL) {
        mQuery = Uri::Part::FromEncoded(encodedParameter);
        return NOERROR;
    }

    String oldQuery = mQuery->GetEncoded();
    if (oldQuery.IsNullOrEmpty()) {
        mQuery = Uri::Part::FromEncoded(encodedParameter);
    } else {
        sb.Reset();
        sb += oldQuery;
        sb += "&";
        sb += encodedParameter;
        mQuery = Uri::Part::FromEncoded(sb.ToString());
    }

    return NOERROR;
}

ECode CUriBuilder::ClearQuery()
{
    return Query((Handle32)NULL);
}

ECode CUriBuilder::Build(
    /* [out] */ IUri** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    AutoPtr<IUri> uri;
    if (mOpaquePart != NULL) {
        if (mScheme.IsNull()) {
            // throw new UnsupportedOperationException("An opaque URI must have a scheme.");
            return E_UNSUPPORTED_OPERATION_EXCEPTION;
        }

        FAIL_RETURN(COpaqueUri::New(mScheme, (Handle32)mOpaquePart.Get(),
            (Handle32)mFragment.Get(), (IUri**)&uri));
    }
    else {
        // Hierarchical URIs should not return null for getPath().
        AutoPtr<Uri::PathPart> path = mPath;
        if (path == NULL || path == Uri::PathPart::sNULL) {
            path = Uri::PathPart::sEMPTY;
        }
        else {
            // If we have a scheme and/or authority, the path must
            // be absolute. Prepend it with a '/' if necessary.
            if (HasSchemeOrAuthority()) {
                path = Uri::PathPart::MakeAbsolute(path);
            }
        }

        FAIL_RETURN(CHierarchicalUri::New(mScheme,
            (Handle32)mAuthority.Get(), (Handle32)mPath.Get(),
            (Handle32)mQuery.Get(), (Handle32)mFragment.Get(),
            (IUri**)&uri));
    }

    *result = uri;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CUriBuilder::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = NULL;
    AutoPtr<IUri> uri;
    FAIL_RETURN(Build((IUri**)&uri));
    if (uri != NULL) {
        return uri->ToString(str);
    }
    return NOERROR;
}

Boolean CUriBuilder::HasSchemeOrAuthority() {
    return !mScheme.IsNull()
            || (mAuthority != NULL && mAuthority != Uri::Part::sNULL);

}

} // namespace Net
} // namespace Droid
} // namespace Elastos
