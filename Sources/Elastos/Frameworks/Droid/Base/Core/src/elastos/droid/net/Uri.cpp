
#include "elastos/droid/net/Uri.h"
#include "elastos/droid/net/CUriBuilder.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Os::IEnvironment;
using Elastos::Droid::Os::IStrictMode;
using Elastos::Droid::Utility::ILog;

using Elastos::Core::EIID_IComparable;
using Elastos::IO::Charset::IStandardCharsets;
using Elastos::Net::IURLEncoder;
using Elastos::Utility::EIID_IRandomAccess;
using Elastos::Utility::IArrayList;
using Elastos::Utility::ICollections;
using Elastos::Utility::ILinkedHashSet;
using Elastos::Utility::ILocale;
using Elastos::Utility::IRandomAccess;
using Elastos::Utility::Logging::Logger;
using Libcore::Net::IUriCodec;

namespace Elastos {
namespace Droid {
namespace Net {

//====================================================================
//                    StringUri
//====================================================================
StringUri::StringUri()
    : mCachedSsi(Uri::NOT_CALCULATED)
    , mCachedFsi(Uri::NOT_CALCULATED)
    , mScheme(Uri::NOT_CACHED)
{}

ECode StringUri::constructor(
    /* [in] */ const String& uriString)
{
    if (uriString.IsNull()) {
        // throw new NullPointerException("uriString");
        Logger::E("Uri", "uriString");
        return E_NULL_POINTER_EXCEPTION;
    }

    mUriString = uriString;
    return NOERROR;
}

ECode StringUri::ReadFrom(
    /* [in] */ IParcel* parcel,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    *uri = NULL;
    VALIDATE_NOT_NULL(parcel);

    String str;
    parcel->ReadString(&str);

    AutoPtr<StringUri> curi = new StringUri;

    *uri = IUri::Probe(curi);
    REFCOUNT_ADD(*uri);
    return NOERROR;
}

ECode StringUri::WriteToParcel(
    /* [in] */ IParcel* parcel)
{
    parcel->WriteInt32(TYPE_ID);
    parcel->WriteString(mUriString);
    return NOERROR;
}

ECode StringUri::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    Int32 id;
    parcel->ReadInt32(&id);
    parcel->ReadString(&mUriString);
    return NOERROR;
}

/** Finds the first ':'. Returns -1 if none found. */
Int32 StringUri::FindSchemeSeparator()
{
    return mCachedSsi == Uri::NOT_CALCULATED
            ? mCachedSsi = mUriString.IndexOf(':')
            : mCachedSsi;
}

/** Finds the first '#'. Returns -1 if none found. */
Int32 StringUri::FindFragmentSeparator()
{
    return mCachedFsi == Uri::NOT_CALCULATED
            ? mCachedFsi = mUriString.IndexOf('#', FindSchemeSeparator())
            : mCachedFsi;
}

ECode StringUri::IsHierarchical(
    /* [out] */ Boolean* isHierarchical)
{
    VALIDATE_NOT_NULL(isHierarchical);

    Int32 ssi = FindSchemeSeparator();
    if (ssi == Uri::NOT_FOUND) {
        // All relative URIs are hierarchical.
        *isHierarchical = TRUE;
        return NOERROR;
    }

    if (mUriString.GetLength() == (UInt32)(ssi + 1)) {
        // No ssp.
        *isHierarchical = FALSE;
        return NOERROR;
    }

    // If the ssp starts with a '/', this is hierarchical.
    *isHierarchical = mUriString.GetChar(ssi + 1) == '/';
    return NOERROR;
}

ECode StringUri::IsRelative(
    /* [out] */ Boolean* isRelative)
{
    VALIDATE_NOT_NULL(isRelative);

    // Note: We return true if the index is 0
    *isRelative = FindSchemeSeparator() == NOT_FOUND;
    return NOERROR;
}

ECode StringUri::GetScheme(
    /* [out] */ String* scheme)
{
    Boolean cached = (!mScheme.Equals(NOT_CACHED));
    *scheme = cached ? mScheme : (mScheme = ParseScheme());
    return NOERROR;
}

String StringUri::ParseScheme()
{
    Int32 ssi = FindSchemeSeparator();
    return ssi == NOT_FOUND ? String(NULL) : mUriString.Substring(0, ssi);
}

AutoPtr<Uri::Part> StringUri::GetSsp()
{
    if (mSsp == NULL) {
        mSsp = Part::FromEncoded(ParseSsp());
    }
    return mSsp;
}

ECode StringUri::GetEncodedSchemeSpecificPart(
    /* [out] */ String* essp)
{
    VALIDATE_NOT_NULL(essp);
    *essp = NULL;
    AutoPtr<Uri::Part> part = GetSsp();
    if (part) {
        *essp = part->GetEncoded();
    }
    return NOERROR;
}

ECode StringUri::GetSchemeSpecificPart(
    /* [out] */ String* ssp)
{
    VALIDATE_NOT_NULL(ssp);
    *ssp = NULL;
    AutoPtr<Uri::Part> part = GetSsp();
    if (part) {
        *ssp = part->GetDecoded();
    }
    return NOERROR;
}

String StringUri::ParseSsp()
{
    Int32 ssi = FindSchemeSeparator();
    Int32 fsi = FindFragmentSeparator();

    // Return everything between ssi and fsi.
    return fsi == NOT_FOUND
            ? mUriString.Substring(ssi + 1)
            : mUriString.Substring(ssi + 1, fsi);
}

AutoPtr<Uri::Part> StringUri::GetAuthorityPart()
{
    if (mAuthority == NULL) {
        String encodedAuthority = ParseAuthority(mUriString, FindSchemeSeparator());
        return mAuthority = Part::FromEncoded(encodedAuthority);
    }

    return mAuthority;
}

ECode StringUri::GetEncodedAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority);
    *authority = NULL;
    AutoPtr<Uri::Part> part = GetAuthorityPart();
    if (part) {
        *authority = part->GetEncoded();
    }
    return NOERROR;
}

ECode StringUri::GetAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority);
    AutoPtr<Uri::Part> part = GetAuthorityPart();
    assert(part != NULL);
    *authority = part->GetDecoded();
    return NOERROR;
}

AutoPtr<Uri::PathPart> StringUri::GetPathPart()
{
    if (mPath == NULL) {
        mPath = PathPart::FromEncoded(ParsePath());
    }
    return mPath;
}

ECode StringUri::GetPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = NULL;
    AutoPtr<Uri::PathPart> part = GetPathPart();
    if (part) {
        *path = part->GetDecoded();
    }
    return NOERROR;
}

ECode StringUri::GetEncodedPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = NULL;
    AutoPtr<Uri::PathPart> part = GetPathPart();
    if (part) {
        *path = part->GetEncoded();
    }
    return NOERROR;
}

ECode StringUri::GetPathSegments(
    /* [out] */ IList** pathSegments)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Need check.
    VALIDATE_NOT_NULL(pathSegments);
    *pathSegments = NULL;

    AutoPtr<Uri::PathPart> part = GetPathPart();
    AutoPtr<PathSegments> segments = part->GetPathSegments();

    if (segments != NULL) {
        Int32 size;
        segments->Size(&size);
        AutoPtr< ArrayOf<String> > array = ArrayOf<String>::Alloc(size);
        String str;
        for (Int32 i = 0; i < size; ++i) {
            FAIL_RETURN(segments->Get(i, &str));
            array->Set(i, str);
        }
        *pathSegments = array;
        REFCOUNT_ADD(*pathSegments);
    }

    return NOERROR;
#endif
}

String StringUri::ParsePath()
{
    String uriString = mUriString;
    Int32 ssi = FindSchemeSeparator();

    // If the URI is absolute.
    if (ssi > -1) {
        AutoPtr<ArrayOf<Char32> > charArray = uriString.GetChars();
        // Is there anything after the ':'?
        Boolean schemeOnly = (UInt32)(ssi + 1) == charArray->GetLength();
        if (schemeOnly) {
            // Opaque URI.
            return String(NULL);
        }

        // A '/' after the ':' means this is hierarchical.
        if ((*charArray)[ssi + 1] != '/') {
            // Opaque URI.
            return String(NULL);
        }
    }
    else {
        // All relative URIs are hierarchical.
    }

    return ParsePath(uriString, ssi);
}

AutoPtr<Uri::Part> StringUri::GetQueryPart()
{
    if (mQuery == NULL) {
        mQuery = Part::FromEncoded(ParseQuery());
    }
    return mQuery;
}

ECode StringUri::GetEncodedQuery(
    /* [out] */ String* query)
{
    VALIDATE_NOT_NULL(query);
    *query = NULL;
    AutoPtr<Uri::Part> part = GetQueryPart();
    if (part) {
        *query = part->GetEncoded();
    }
    return NOERROR;
}

String StringUri::ParseQuery()
{
    // It doesn't make sense to cache this index. We only ever
    // calculate it once.
    Int32 qsi = mUriString.IndexOf('?', FindSchemeSeparator());
    if (qsi == NOT_FOUND) {
        return String(NULL);
    }

    Int32 fsi = FindFragmentSeparator();
    if (fsi == NOT_FOUND) {
        return mUriString.Substring(qsi + 1);
    }

    if (fsi < qsi) {
        // Invalid.
        return String(NULL);
    }

    return mUriString.Substring(qsi + 1, fsi);
}

ECode StringUri::GetQuery(
    /* [out] */ String* query)
{
    VALIDATE_NOT_NULL(query);
    *query = NULL;
    AutoPtr<Uri::Part> part = GetQueryPart();
    if (part) {
        *query = part->GetDecoded();
    }
    return NOERROR;
}

AutoPtr<Uri::Part> StringUri::GetFragmentPart()
{
    if (mFragment == NULL) {
        mFragment = Part::FromEncoded(ParseFragment());
    }
    return mFragment;
}

ECode StringUri::GetEncodedFragment(
    /* [out] */ String* fragment)
{
    VALIDATE_NOT_NULL(fragment);
    *fragment = NULL;
    AutoPtr<Uri::Part> part = GetFragmentPart();
    if (part) {
        *fragment = part->GetEncoded();
    }
    return NOERROR;
}

String StringUri::ParseFragment()
{
    Int32 fsi = FindFragmentSeparator();
    return fsi == Uri::NOT_FOUND ?
        String(NULL) : mUriString.Substring(fsi + 1);
}

ECode StringUri::GetFragment(
    /* [out] */ String* fragment)
{
    VALIDATE_NOT_NULL(fragment);
    *fragment = NULL;
    AutoPtr<Uri::Part> part = GetFragmentPart();
    if (part) {
        *fragment = part->GetDecoded();
    }
    return NOERROR;
}

ECode StringUri::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mUriString;
    return NOERROR;
}

String StringUri::ParseAuthority(
    /* [in] */ const String& uriString,
    /* [in] */ Int32 ssi)
{
    AutoPtr<ArrayOf<Char32> > charArray = uriString.GetChars();
    Int32 length = charArray->GetLength();

    // If "//" follows the scheme separator, we have an authority.
    if (length > ssi + 2
            && (*charArray)[ssi + 1] == '/'
            && (*charArray)[ssi + 2] == '/') {
        // We have an authority.

        // Look for the start of the path, query, or fragment, or the
        // end of the string.
        Int32 end = ssi + 3;
        while (end < length) {
            switch ((*charArray)[end]) {
                case '/': // Start of path
                case '?': // Start of query
                case '#': // Start of fragment
                    goto LOOP;
            }
            end++;
        }
LOOP:
        return uriString.Substring(ssi + 3, end);
    }
    else {
        return String(NULL);
    }
}

String StringUri::ParsePath(
    /* [in] */ const String& uriString,
    /* [in] */ Int32 ssi)
{
    AutoPtr<ArrayOf<Char32> > charArray = uriString.GetChars();
    Int32 length = charArray->GetLength();

    // Find start of path.
    Int32 pathStart, pathEnd;
    if (length > ssi + 2
            && (*charArray)[ssi + 1] == '/'
            && (*charArray)[ssi + 2] == '/') {
        // Skip over authority to path.
        pathStart = ssi + 3;
        while (pathStart < length) {
            switch ((*charArray)[pathStart]) {
                case '?': // Start of query
                case '#': // Start of fragment
                    return String(""); // Empty path.
                case '/': // Start of path!
                    goto LOOP1;
            }
            pathStart++;
        }
    }
    else {
        // Path starts immediately after scheme separator.
        pathStart = ssi + 1;
    }
LOOP1:

    // Find end of path.
    pathEnd = pathStart;
    while (pathEnd < length) {
        switch ((*charArray)[pathEnd]) {
            case '?': // Start of query
            case '#': // Start of fragment
                goto LOOP2;
        }
        pathEnd++;
    }
LOOP2:

    return uriString.Substring(pathStart, pathEnd);
}

ECode StringUri::BuildUpon(
    /* [out] */ IUriBuilder** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    String str;
    GetScheme(&str);

#ifdef DROID_CORE
    AutoPtr<CUriBuilder> builder;
    FAIL_RETURN(CUriBuilder::NewByFriend((CUriBuilder**)&builder));
#else
    AutoPtr<IUriBuilder> builder;
    FAIL_RETURN(CUriBuilder::New((IUriBuilder**)&builder));
#endif
    builder->Scheme(str);

    Boolean isHierarchical;
    IsHierarchical(&isHierarchical);

    if (isHierarchical) {
        AutoPtr<Uri::Part> p = GetAuthorityPart();
        builder->Authority(p.Get());
        AutoPtr<Uri::PathPart> pp = GetPathPart();
        builder->Path(pp.Get());
        p = GetQueryPart();
        builder->Query(p.Get());
        p = GetFragmentPart();
        builder->Fragment(p.Get());
    }
    else {
        AutoPtr<Uri::Part> p = GetSsp();
        builder->OpaquePart(p.Get());
        p = GetFragmentPart();
        builder->Fragment(p.Get());
    }

    *result = (IUriBuilder*)builder.Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

//====================================================================
//                    OpaqueUri
//====================================================================
ECode OpaqueUri::constructor(
    /* [in] */ const String& scheme,
    /* [in] */ Uri::Part* ssp,
    /* [in] */ Uri::Part* fragment)
{
    mScheme = scheme;
    mSsp = (Part*)ssp;
    mFragment = (Part*)fragment;

    if (mFragment == NULL) {
        mFragment = Part::sNULL;
    }

    return NOERROR;
}

ECode OpaqueUri::ReadFrom(
    /* [in] */ IParcel* parcel,
    /* [out] */ IUri** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;
    VALIDATE_NOT_NULL(parcel);

    String str;
    parcel->ReadString(&str);

    AutoPtr<Uri::Part> p1, p2;
    Uri::Part::ReadFrom(parcel, (Uri::Part**)&p1);
    Uri::Part::ReadFrom(parcel, (Uri::Part**)&p2);

    AutoPtr<OpaqueUri> curi = new OpaqueUri;
    curi->constructor(str, p1, p2);
    *result = IUri::Probe(curi);
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode OpaqueUri::WriteToParcel(
    /* [in] */ IParcel* parcel)
{
    VALIDATE_NOT_NULL(parcel);
    parcel->WriteInt32(OpaqueUri::TYPE_ID);
    parcel->WriteString(mScheme);

    assert(mSsp != NULL);
    mSsp->WriteTo(parcel);
    assert(mFragment != NULL);
    mFragment->WriteTo(parcel);
    return NOERROR;
}

ECode OpaqueUri::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    VALIDATE_NOT_NULL(parcel);
    Int32 id;
    parcel->ReadInt32(&id);
    parcel->ReadString(&mScheme);

    mSsp = NULL;
    Uri::Part::ReadFrom(parcel, (Uri::Part**)&mSsp);
    mFragment = NULL;
    Uri::Part::ReadFrom(parcel, (Uri::Part**)&mFragment);
    return NOERROR;
}

ECode OpaqueUri::IsHierarchical(
    /* [out] */ Boolean* isHierarchical)
{
    VALIDATE_NOT_NULL(isHierarchical);
    *isHierarchical = FALSE;
    return NOERROR;
}

ECode OpaqueUri::IsRelative(
    /* [out] */ Boolean* isRelative)
{
    VALIDATE_NOT_NULL(isRelative);
    *isRelative = mScheme.IsNull();
    return NOERROR;
}

ECode OpaqueUri::GetScheme(
    /* [out] */ String* scheme)
{
    VALIDATE_NOT_NULL(scheme);
    *scheme = mScheme;
    return NOERROR;
}

ECode OpaqueUri::GetEncodedSchemeSpecificPart(
    /* [out] */ String* essp)
{
    VALIDATE_NOT_NULL(essp);
    *essp = mSsp->GetEncoded();
    return NOERROR;
}

ECode OpaqueUri::GetSchemeSpecificPart(
    /* [out] */ String* ssp)
{
    VALIDATE_NOT_NULL(ssp);
    *ssp = mSsp->GetDecoded();
    return NOERROR;
}

ECode OpaqueUri::GetAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority);
    *authority = NULL;
    return NOERROR;
}

ECode OpaqueUri::GetEncodedAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority);
    *authority = NULL;
    return NOERROR;
}

ECode OpaqueUri::GetPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = NULL;
    return NOERROR;
}

ECode OpaqueUri::GetEncodedPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = NULL;
    return NOERROR;
}

ECode OpaqueUri::GetQuery(
    /* [out] */ String* query)
{
    VALIDATE_NOT_NULL(query);
    *query = NULL;
    return NOERROR;
}

ECode OpaqueUri::GetEncodedQuery(
    /* [out] */ String* query)
{
    VALIDATE_NOT_NULL(query);
    *query = NULL;
    return NOERROR;
}

ECode OpaqueUri::GetFragment(
    /* [out] */ String* fragment)
{
    VALIDATE_NOT_NULL(fragment);
    *fragment = mFragment->GetDecoded();
    return NOERROR;
}

ECode OpaqueUri::GetEncodedFragment(
    /* [out] */ String* fragment)
{
    VALIDATE_NOT_NULL(fragment);
    *fragment = mFragment->GetEncoded();
    return NOERROR;
}

ECode OpaqueUri::GetPathSegments(
    /* [out, callee] */ IList** pathSegments)
{
    VALIDATE_NOT_NULL(pathSegments);
    *pathSegments = NULL;
    return NOERROR;
}

ECode OpaqueUri::GetLastPathSegment(
    /* [out] */ String* pathSegment)
{
    VALIDATE_NOT_NULL(pathSegment);
    *pathSegment = NULL;
    return NOERROR;
}

ECode OpaqueUri::GetUserInfo(
    /* [out] */ String* userInfo)
{
    VALIDATE_NOT_NULL(userInfo);
    *userInfo = NULL;
    return NOERROR;
}

ECode OpaqueUri::GetEncodedUserInfo(
    /* [out] */ String* userInfo)
{
    VALIDATE_NOT_NULL(userInfo);
    *userInfo = NULL;
    return NOERROR;
}

ECode OpaqueUri::GetHost(
    /* [out] */ String* host)
{
    VALIDATE_NOT_NULL(host);
    *host = NULL;
    return NOERROR;
}

ECode OpaqueUri::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    *port = -1;
    return NOERROR;
}

ECode OpaqueUri::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    // @SuppressWarnings("StringEquality")
    Boolean cached = (!mCachedString.IsNull() && !mCachedString.Equals(Uri::NOT_CACHED));
    if (!cached) {
        StringBuilder sb(mScheme);
        sb += (":");

        String str;
        FAIL_RETURN(GetEncodedSchemeSpecificPart(&str));
        sb += str;

        Boolean isEmpty = mFragment->IsEmpty();
        if (!isEmpty) {
            sb += ("#");
            str = mFragment->GetEncoded();
            sb += str;
        }

        mCachedString = sb.ToString();
    }

    *result = mCachedString;
    return NOERROR;
}

ECode OpaqueUri::BuildUpon(
    /* [out] */ IUriBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Need check.
    VALIDATE_NOT_NULL(result);
    *result = NULL;

#ifdef DROID_CORE
    AutoPtr<CUriBuilder> builder;
    FAIL_RETURN(CUriBuilder::NewByFriend((CUriBuilder**)&builder));
#else
    AutoPtr<IUriBuilder> builder;
    FAIL_RETURN(CUriBuilder::New((IUriBuilder**)&builder));
#endif

    FAIL_RETURN(builder->Scheme(mScheme));
    FAIL_RETURN(builder->OpaquePart((Handle32)&mSsp));
    FAIL_RETURN(builder->Fragment((Handle32)&mFragment));
    *result = (IUriBuilder*)builder.Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
#endif
}

//====================================================================================
//              Uri::PathSegments
//====================================================================================
AutoPtr<Uri::PathSegments> Uri::PathSegments::sEMPTY = new Uri::PathSegments(ArrayOf<String>::Alloc(0), 0);

CAR_INTERFACE_IMPL(Uri::PathSegments, AbstractList, IRandomAccess)

Uri::PathSegments::PathSegments(
    /* [in] */ ArrayOf<String>* segments,
    /* [in] */ Int32 size)
    : mSegments(segments)
    , mSize(size)
{}

ECode Uri::PathSegments::Get(
    /* [in] */ Int32 index,
    /* [out] */ IInterface** encoded)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Need check.
    VALIDATE_NOT_NULL(encoded);

    if (index >= mSize) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    *encoded = (*mSegments)[index];
    return NOERROR;
#endif
}

ECode Uri::PathSegments::Size(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mSize;
    return NOERROR;
}

//====================================================================================
//              Uri::PathSegmentsBuilder
//====================================================================================

Uri::PathSegmentsBuilder::PathSegmentsBuilder()
    : mSize(0)
{
}

ECode Uri::PathSegmentsBuilder::Add(
    /* [in] */ const String& segment)
{
    mSegments.PushBack(segment);
    mSize++;

    return NOERROR;
}

AutoPtr<Uri::PathSegments> Uri::PathSegmentsBuilder::Build()
{
    if (mSegments.IsEmpty()) {
        return Uri::PathSegments::sEMPTY;
    }

    // try {
    AutoPtr<ArrayOf<String> > segments = ArrayOf<String>::Alloc(mSegments.GetSize());
    if (!segments) {
        return Uri::PathSegments::sEMPTY;
    }

    List<String>::Iterator itor;
    Int32 i = 0;
    for (itor = mSegments.Begin(); itor != mSegments.End(); ++itor, ++i) {
        (*segments)[i] = *itor;
    }

    AutoPtr<Uri::PathSegments> pathSegments = new Uri::PathSegments(segments, mSize);

    // } finally {
    // Makes sure this doesn't get reused.
    mSegments.Clear();
    mSize = 0;
    // }

    return pathSegments;
}

//====================================================================
//                    AbstractHierarchicalUri
//====================================================================
AbstractHierarchicalUri::AbstractHierarchicalUri()
    : mHost(NOT_CACHED)
    , mPort(NOT_CALCULATED)
{
}

ECode AbstractHierarchicalUri::GetLastPathSegment(
    /* [out] */ String* pathSegment)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Need check.
    VALIDATE_NOT_NULL(pathSegment);
    *pathSegment = NULL;

    AutoPtr<ArrayOf<String> > segments;
    GetPathSegments((ArrayOf<String>**)&segments);
    if (segments == NULL || segments->GetLength() == 0) {
        return NOERROR;
    }

    Int32 size = segments->GetLength();
    *pathSegment = (*segments)[size - 1];
    return NOERROR;
#endif
}

AutoPtr<Uri::Part> AbstractHierarchicalUri::GetUserInfoPart()
{
    if (mUserInfo == NULL) {
        String result = ParseUserInfo();
        mUserInfo = Uri::Part::FromEncoded(result);
    }

    return mUserInfo;
}

ECode AbstractHierarchicalUri::GetEncodedUserInfo(
    /* [out] */ String* userInfo)
{
    VALIDATE_NOT_NULL(userInfo);
    *userInfo = GetUserInfoPart()->GetEncoded();
    return NOERROR;
}

String AbstractHierarchicalUri::ParseUserInfo()
{
    String authority;
    GetEncodedAuthority(&authority);
    if (authority.IsNull()) {
        return String(NULL);
    }

    Int32 end = authority.IndexOf('@');
    if (end == Uri::NOT_FOUND) {
        return String(NULL);
    }
    else {
        return authority.Substring(0, end);
    }
}

ECode AbstractHierarchicalUri::GetUserInfo(
    /* [out] */ String* userInfo)
{
    VALIDATE_NOT_NULL(userInfo);
    *userInfo = GetUserInfoPart()->GetDecoded();
    return NOERROR;
}

ECode AbstractHierarchicalUri::GetHost(
    /* [out] */ String* host)
{
    VALIDATE_NOT_NULL(host);
    Boolean cached = !mHost.Equals(Uri::NOT_CACHED);

    if (!cached) {
        mHost = ParseHost();
    }

    *host = mHost;
    return NOERROR;
}

String AbstractHierarchicalUri::ParseHost()
{
    String authority;
    GetEncodedAuthority(&authority);
    if (authority.IsNull()) {
        return String(NULL);

    }

    // Parse out user info and then port.
    Int32 userInfoSeparator = authority.IndexOf('@');
    Int32 portSeparator = authority.IndexOf(':', userInfoSeparator);

    String encodedHost = portSeparator == Uri::NOT_FOUND
        ? authority.Substring(userInfoSeparator + 1)
        : authority.Substring(userInfoSeparator + 1, portSeparator);

    String result;
    Decode(encodedHost, &result);
    return result;
}

ECode AbstractHierarchicalUri::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    if (mPort == Uri::NOT_CALCULATED) {
        mPort = ParsePort();
    }
    *port = mPort;
    return NOERROR;
}

Int32 AbstractHierarchicalUri::ParsePort()
{
    String authority;
    GetEncodedAuthority(&authority);
    if (authority.IsNull()) {
        return -1;
    }

    // Make sure we look for the port separtor *after* the user info
    // separator. We have URLs with a ':' in the user info.
    Int32 userInfoSeparator = authority.IndexOf('@');
    Int32 portSeparator = authority.IndexOf(':', userInfoSeparator);

    if (portSeparator == Uri::NOT_FOUND) {
        return -1;
    }

    String portString;
    Decode(authority.Substring(portSeparator + 1), &portString);

    Int32 result;
    result = StringUtils::ParseInt32(portString);
    return result;
}

//====================================================================
//                      HierarchicalUri
//====================================================================
HierarchicalUri::HierarchicalUri()
{
    mUriString = Uri::NOT_CACHED;
}

HierarchicalUri::HierarchicalUri(
    /* [in] */ const String& scheme,
    /* [in] */ Uri::Part* authority,
    /* [in] */ Uri::PathPart* path,
    /* [in] */ Uri::Part* query,
    /* [in] */ Uri::Part* fragment)
    : mScheme(scheme)
    , mAuthority(authority)
    , mPath(path)
    , mQuery(query)
    , mFragment(fragment)
{
}

ECode HierarchicalUri::constructor(
    /* [in] */ const String& scheme,
    /* [in] */ Uri::Part* authority,
    /* [in] */ Uri::PathPart* path,
    /* [in] */ Uri::Part* query,
    /* [in] */ Uri::Part* fragment)
{
    mScheme = scheme;
    mAuthority = (Uri::Part*)authority;
    mPath = (Uri::PathPart*)path;
    mQuery = (Uri::Part*)query;
    mFragment = (Uri::Part*)fragment;
    return NOERROR;
}

ECode HierarchicalUri::ReadFrom(
    /* [in] */ IParcel* parcel,
    /* [out] */ IUri** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;
    VALIDATE_NOT_NULL(parcel);

    String str;
    parcel->ReadString(&str);

    AutoPtr<Uri::Part> p1, p3, p4;
    AutoPtr<Uri::PathPart> p2;
    Uri::Part::ReadFrom(parcel, (Uri::Part**)&p1);
    Uri::PathPart::ReadFrom(parcel, (Uri::PathPart**)&p2);
    Uri::Part::ReadFrom(parcel, (Uri::Part**)&p3);
    Uri::Part::ReadFrom(parcel, (Uri::Part**)&p4);

    AutoPtr<HierarchicalUri> curi = new HierarchicalUri;
    curi->constructor(str, p1, p2, p3, p4);

    *result = IUri::Probe(curi);
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode HierarchicalUri::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    VALIDATE_NOT_NULL(parcel);
    Int32 id;
    parcel->ReadInt32(&id);
    parcel->ReadString(&mScheme);

    mAuthority = NULL;
    Uri::Part::ReadFrom(parcel, (Uri::Part**)&mAuthority);

    mPath = NULL;
    Uri::PathPart::ReadFrom(parcel, (Uri::PathPart**)&mPath);

    mQuery = NULL;
    Uri::Part::ReadFrom(parcel, (Uri::Part**)&mQuery);

    mFragment = NULL;
    Uri::Part::ReadFrom(parcel, (Uri::Part**)&mFragment);
    return NOERROR;
}

ECode HierarchicalUri::WriteToParcel(
    /* [in] */ IParcel* parcel)
{
    VALIDATE_NOT_NULL(parcel);
    parcel->WriteInt32(TYPE_ID);
    parcel->WriteString(mScheme);
    mAuthority->WriteTo(parcel);
    mPath->WriteTo(parcel);
    mQuery->WriteTo(parcel);
    mFragment->WriteTo(parcel);
    return NOERROR;
}

ECode HierarchicalUri::IsHierarchical(
    /* [out] */ Boolean* isHierarchical)
{
    VALIDATE_NOT_NULL(isHierarchical);
    return TRUE;
}

ECode HierarchicalUri::IsRelative(
    /* [out] */ Boolean* isRelative)
{
    VALIDATE_NOT_NULL(isRelative);
    return !mScheme.IsNull();
}

ECode HierarchicalUri::GetScheme(
    /* [out] */ String* scheme)
{
    VALIDATE_NOT_NULL(scheme);
    *scheme = mScheme;
    return NOERROR;
}

AutoPtr<Uri::Part> HierarchicalUri::GetSsp()
{
    if (mSsp == NULL) {
        mSsp = Uri::Part::FromEncoded(MakeSchemeSpecificPart());
    }
    return mSsp;
}

ECode HierarchicalUri::GetEncodedSchemeSpecificPart(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<Part> part = GetSsp();
    *result = part->GetEncoded();
    return NOERROR;
}

ECode HierarchicalUri::GetSchemeSpecificPart(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<Part> part = GetSsp();
    *result = part->GetDecoded();
    return NOERROR;
}

String HierarchicalUri::MakeSchemeSpecificPart()
{
    StringBuilder builder;
    AppendSspTo(builder);
    return builder.ToString();
}

ECode HierarchicalUri::AppendSspTo(
    /* [in] */ StringBuilder& builder)
{
    String encodedAuthority = mAuthority->GetEncoded();
    if (!encodedAuthority.IsNull()) {
        // Even if the authority is "", we still want to append "//".
        builder += ("//");
        builder += encodedAuthority;
    }

    String encodedPath = mPath->GetEncoded();
    if (!encodedPath.IsNull()) {
        builder += encodedPath;
    }

    if (!mQuery->IsEmpty()) {
        builder += ('?');
        builder += mQuery->GetEncoded();
    }
    return NOERROR;
}


ECode HierarchicalUri::GetAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority);
    *authority = mAuthority->GetDecoded();
    return NOERROR;
}

ECode HierarchicalUri::GetEncodedAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority);
    *authority = mAuthority->GetEncoded();
    return NOERROR;
}

ECode HierarchicalUri::GetEncodedPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = mPath->GetEncoded();
    return NOERROR;
}

ECode HierarchicalUri::GetPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = mPath->GetDecoded();
    return NOERROR;
}

ECode HierarchicalUri::GetQuery(
    /* [out] */ String* query)
{
    VALIDATE_NOT_NULL(query);
    *query = mPath->GetDecoded();
    return NOERROR;
}

ECode HierarchicalUri::GetEncodedQuery(
    /* [out] */ String* query)
{
    VALIDATE_NOT_NULL(query);
    *query = mPath->GetEncoded();
    return NOERROR;
}

ECode HierarchicalUri::GetFragment(
    /* [out] */ String* fragment)
{
    VALIDATE_NOT_NULL(fragment);
    *fragment = mPath->GetEncoded();
    return NOERROR;
}

ECode HierarchicalUri::GetEncodedFragment(
    /* [out] */ String* fragment)
{
    VALIDATE_NOT_NULL(fragment);
    *fragment = mPath->GetEncoded();
    return NOERROR;
}

// return a list of string
ECode HierarchicalUri::GetPathSegments(
    /* [out, callee] */ IList** segments)
{
    VALIDATE_NOT_NULL(segments);
    *segments = NULL;

    AutoPtr<Uri::PathSegments> pathSegments = mPath->GetPathSegments();
    if (pathSegments != NULL) {
        *segments = IList::Probe(pathSegments);
        REFCOUNT_ADD(*segments);
    }
    return NOERROR;
}

ECode HierarchicalUri::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    Boolean cached = !mUriString.Equals(Uri::NOT_CACHED);
    *info = cached ? mUriString
            : (mUriString = MakeUriString());
    return NOERROR;
}

String HierarchicalUri::MakeUriString()
{
    StringBuilder builder;

    if (!mScheme.IsNull()) {
        builder += mScheme;
        builder.AppendChar(':');
    }

    AppendSspTo(builder);

    if (!mFragment->IsEmpty()) {
        builder.AppendChar('#');
        builder += mFragment->GetEncoded();
    }

    return builder.ToString();
}

ECode HierarchicalUri::BuildUpon(
    /* [out] */ IUriBuilder** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

#ifdef DROID_CORE
    AutoPtr<CUriBuilder> builder;
    FAIL_RETURN(CUriBuilder::NewByFriend((CUriBuilder**)&builder));
#else
    AutoPtr<IUriBuilder> builder;
    FAIL_RETURN(CUriBuilder::New((IUriBuilder**)&builder));
#endif

    FAIL_RETURN(builder->Scheme(mScheme));
    FAIL_RETURN(builder->Authority(mAuthority));
    FAIL_RETURN(builder->Path(mPath));
    FAIL_RETURN(builder->Query(mQuery));
    FAIL_RETURN(builder->Fragment(mFragment));
    *result = IUriBuilder::Probe(builder);
    REFCOUNT_ADD(*result);
    return NOERROR;
}

//====================================================================
//                      UriBuilder
//====================================================================
CAR_INTERFACE_IMPL(UriBuilder, Object, IUriBuilder)

ECode UriBuilder::constructor()
{
    return NOERROR;
}

ECode UriBuilder::Scheme(
    /* [in] */ const String& scheme)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    mScheme = scheme;
    return NOERROR;
#endif
}

ECode UriBuilder::OpaquePart(
    /* [in] */ Uri::Part* opaquePart)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    mOpaquePart = (Uri::Part*)opaquePart;
    return NOERROR;
#endif
}

ECode UriBuilder::OpaquePart(
    /* [in] */ const String& opaquePart)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoPtr<Uri::Part> part = Uri::Part::FromDecoded(opaquePart);
    mOpaquePart = part;
    return NOERROR;
#endif
}

ECode UriBuilder::EncodedOpaquePart(
    /* [in] */ const String& opaquePart)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoPtr<Uri::Part> part = Uri::Part::FromDecoded(opaquePart);
    mOpaquePart = part;
    return NOERROR;
#endif
}

ECode UriBuilder::Authority(
    /* [in] */ Uri::Part* authority)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    mAuthority = authority;
    return NOERROR;
#endif
}

ECode UriBuilder::Authority(
    /* [in] */ const String& authority)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    AutoPtr<Uri::Part> part = Uri::Part::FromDecoded(authority);
    mAuthority = part;
    return NOERROR;
#endif
}

ECode UriBuilder::EncodedAuthority(
    /* [in] */ const String& authority)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    AutoPtr<Uri::Part> part = Uri::Part::FromDecoded(authority);
    mAuthority = part;
    return NOERROR;
#endif
}

ECode UriBuilder::Path(
    /* [in] */ Uri::PathPart* path)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    mPath = path;
    return NOERROR;
#endif
}

ECode UriBuilder::Path(
    /* [in] */ const String& path)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    AutoPtr<Uri::PathPart> part = Uri::PathPart::FromDecoded(path);
    mPath = part;
    return NOERROR;
#endif
}

ECode UriBuilder::EncodedPath(
    /* [in] */ const String& path)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    AutoPtr<Uri::PathPart> part = Uri::PathPart::FromDecoded(path);
    mPath = part;
    return NOERROR;
#endif
}

ECode UriBuilder::AppendPath(
    /* [in] */ const String& newSegment)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    AutoPtr<Uri::PathPart> part = Uri::PathPart::AppendDecodedSegment(mPath, newSegment);
    mPath = part;
    return NOERROR;
#endif
}

ECode UriBuilder::AppendEncodedPath(
    /* [in] */ const String& newSegment)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    AutoPtr<Uri::PathPart> part = Uri::PathPart::AppendEncodedSegment(mPath, newSegment);
    mPath = part;
    return NOERROR;
#endif
}

ECode UriBuilder::Query(
    /* [in] */ Uri::Part* query)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    mQuery = query;
    return NOERROR;
#endif
}

ECode UriBuilder::Query(
    /* [in] */ const String& query)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    AutoPtr<Uri::Part> part = Uri::Part::FromDecoded(query);
    mQuery = part;
    return NOERROR;
#endif
}

ECode UriBuilder::EncodedQuery(
    /* [in] */ const String& query)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    // This URI will be hierarchical.
    mOpaquePart = NULL;
    AutoPtr<Uri::Part> part = Uri::Part::FromDecoded(query);
    mQuery = part;
    return NOERROR;
#endif
}

ECode UriBuilder::Fragment(
    /* [in] */ Uri::Part* fragment)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    mFragment = fragment;
    return NOERROR;
#endif
}

ECode UriBuilder::Fragment(
    /* [in] */ const String& fragment)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoPtr<Uri::Part> part = Uri::Part::FromDecoded(fragment);
    mFragment = part;
    return NOERROR;
#endif
}

ECode UriBuilder::EncodedFragment(
    /* [in] */ const String& fragment)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoPtr<Uri::Part> part = Uri::Part::FromDecoded(fragment);
    mFragment = part;
    return NOERROR;
#endif
}

ECode UriBuilder::AppendQueryParameter(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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
#endif
}

ECode UriBuilder::ClearQuery()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    return Query((Handle32)NULL);
#endif
}

ECode UriBuilder::Build(
    /* [out] */ IUri** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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
#endif
}

ECode UriBuilder::HasSchemeOrAuthority(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    return !mScheme.IsNull()
            || (mAuthority != NULL && mAuthority != Uri::Part::sNULL);

#endif
}

ECode UriBuilder::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(str);
    *str = NULL;
    AutoPtr<IUri> uri;
    FAIL_RETURN(Build((IUri**)&uri));
    if (uri != NULL) {
        return uri->ToString(str);
    }
    return NOERROR;
#endif
}

//====================================================================================
//              Uri::AbstractPart
//====================================================================================
Uri::AbstractPart::AbstractPart(
    /* [in] */ const String& encoded,
    /* [in] */ const String& decoded)
    : mEncoded(encoded)
    , mDecoded(decoded)
{}

String Uri::AbstractPart::GetDecoded()
{
    // @SuppressWarnings("StringEquality")
    if (Uri::NOT_CACHED.Equals(mDecoded)) {
        Uri::Decode(mEncoded, &mDecoded);
    }
    return mDecoded;
}

ECode Uri::AbstractPart::WriteTo(
    /* [in] */ IParcel* parcel)
{
    // @SuppressWarnings("StringEquality")
    Boolean hasEncoded = !Uri::NOT_CACHED.Equals(mEncoded);

    // @SuppressWarnings("StringEquality")
    Boolean hasDecoded = !Uri::NOT_CACHED.Equals(mDecoded);

    if (hasEncoded && hasDecoded) {
        parcel->WriteInt32(Representation::BOTH);
        parcel->WriteString(mEncoded);
        parcel->WriteString(mDecoded);
    }
    else if (hasEncoded) {
        parcel->WriteInt32(Representation::ENCODED);
        parcel->WriteString(mEncoded);
    }
    else if (hasDecoded) {
        parcel->WriteInt32(Representation::DECODED);
        parcel->WriteString(mDecoded);
    }
    else {
        // throw new IllegalArgumentException("Neither encoded nor decoded");
        Logger::E("Uri", "Neither encoded nor decoded");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

//====================================================================================
//              Uri::EmptyPart
//====================================================================================
Uri::EmptyPart::EmptyPart(
    /* [in] */ const String& value)
    : Part(value, value)
{}

ECode Uri::EmptyPart::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    *result = TRUE;
    return NOERROR;
}

//====================================================================================
//              Uri::Part
//====================================================================================
const AutoPtr<Uri::Part> Uri::Part::sNULL = new EmptyPart(String(NULL));
const AutoPtr<Uri::Part> Uri::Part::sEMPTY = new EmptyPart(String(""));

Uri::Part::Part(
    /* [in] */ const String& encoded,
    /* [in] */ const String& decoded)
    : AbstractPart(encoded, decoded)
{}

Boolean Uri::Part::IsEmpty()
{
    return FALSE;
}

String Uri::Part::GetEncoded()
{
    // @SuppressWarnings("StringEquality")
    if (Uri::NOT_CACHED.Equals(mEncoded)) {
        Uri::Encode(mDecoded, &mEncoded);
    }
    return mEncoded;
}

ECode Uri::Part::ReadFrom(
    /* [in] */ IParcel* parcel,
    /* [out] */ Part** part)
{
    VALIDATE_NOT_NULL(part);
    *part = NULL;
    VALIDATE_NOT_NULL(parcel);

    AutoPtr<Part> result;
    Int32 representation;
    parcel->ReadInt32(&representation);
    String str1, str2;
    switch (representation) {
        case Representation::BOTH:
            parcel->ReadString(&str1);
            parcel->ReadString(&str2);
            result = From(str1, str2);
            break;

        case Representation::ENCODED:
            parcel->ReadString(&str1);
            result = FromEncoded(str1);
            break;

        case Representation::DECODED:
            parcel->ReadString(&str1);
            result = FromDecoded(str1);
            break;
        default:
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
            // throw new IllegalArgumentException("Unknown representation: "
            //         + representation);
    }

    *part = result;
    REFCOUNT_ADD(*part);
    return NOERROR;
}

AutoPtr<Uri::Part> Uri::Part::NonNull(
    /* [in] */ Part* part)
{
    return part == NULL ? sNULL.Get() : part;
}

/**
 * Creates a part from the encoded string.
 *
 * @param encoded part string
 */
AutoPtr<Uri::Part> Uri::Part::FromEncoded(
    /* [in] */ const String& encoded)
{
    return From(encoded, NOT_CACHED);
}

/**
 * Creates a part from the decoded string.
 *
 * @param decoded part string
 */
AutoPtr<Uri::Part> Uri::Part::FromDecoded(
    /* [in] */ const String& decoded)
{
    return From(NOT_CACHED, decoded);
}

AutoPtr<Uri::Part> Uri::Part::From(
    /* [in] */ const String& encoded,
    /* [in] */ const String& decoded)
{
    // We have to check both encoded and decoded in case one is
    // NOT_CACHED.

    if (encoded.IsNull()) {
        return sNULL;
    }
    if (encoded.IsEmpty()) {
        return sEMPTY;
    }

    if (decoded.IsNull()) {
        return sNULL;
    }
    if (decoded.IsEmpty()) {
        return sEMPTY;
    }

    AutoPtr<Part> p = new Part(encoded, decoded);
    return p;
}

//====================================================================================
//              Uri::PathPart
//====================================================================================
const AutoPtr<Uri::PathPart> Uri::PathPart::sNULL = new PathPart(String(NULL), String(NULL));
const AutoPtr<Uri::PathPart> Uri::PathPart::sEMPTY = new PathPart(String(""), String(""));

Uri::PathPart::PathPart(
    /* [in] */ const String& encoded,
    /* [in] */ const String& decoded)
    : AbstractPart(encoded, decoded)
{}

String Uri::PathPart::GetEncoded()
{
    // @SuppressWarnings("StringEquality")
    if (Uri::NOT_CACHED.Equals(mEncoded)) {
        // Uri::Encode(mDecoded, &mEncoded);
        Encode(mDecoded, String("/"), &mEncoded);
    }

    return mEncoded;
}

AutoPtr<Uri::PathSegments> Uri::PathPart::GetPathSegments()
{
    if (mPathSegments != NULL) {
        return mPathSegments;
    }

    String path = GetEncoded();
    if (path.IsNull()) {
        mPathSegments = PathSegments::sEMPTY;
        return mPathSegments;
    }

    AutoPtr<Uri::PathSegmentsBuilder> segmentBuilder = new Uri::PathSegmentsBuilder();

    Int32 previous = 0;
    Int32 current;
    while ((current = path.IndexOf('/', previous)) > -1) {
        // This check keeps us from adding a segment if the path starts
        // '/' and an empty segment for "//".

        if (previous < current) {
            String decodedSegment;
            Uri::Decode(path.Substring(previous, current), &decodedSegment);
            segmentBuilder->Add(decodedSegment);
        }
        previous = current + 1;
    }

    // Add in the final path segment.
    if (previous < (Int32)path.GetLength()) {
        String decoded;
        Uri::Decode(path.Substring(previous), &decoded);
        segmentBuilder->Add(decoded);
    }

    mPathSegments = segmentBuilder->Build();
    return mPathSegments;
}

AutoPtr<Uri::PathPart> Uri::PathPart::AppendEncodedSegment(
    /* [in] */ Uri::PathPart* oldPart,
    /* [in] */ const String& newSegment)
{
    if (oldPart == NULL) {
        // No old path.
        return FromEncoded(String("/") + newSegment);
    }

    String oldPath = oldPart->GetEncoded();
    if (oldPath.IsNull()) {
        oldPath = String("");
    }

    UInt32 oldPathLength = oldPath.GetLength();
    String newPath;
    if (oldPathLength == 0) {
        // No old path.
        newPath = String("/") + newSegment;
    } else if (oldPath[(oldPathLength - 1)] == '/') {
        newPath = oldPath + newSegment;
    } else {
        newPath = oldPath + String("/") + newSegment;
    }

    return FromEncoded(newPath);
}

AutoPtr<Uri::PathPart> Uri::PathPart::AppendDecodedSegment(
    /* [in] */ Uri::PathPart* oldPart,
    /* [in] */ const String& decoded)
{
    String encoded;
    Uri::Encode(decoded, &encoded);

    // TODO: Should we reuse old PathSegments? Probably not.
    return AppendEncodedSegment(oldPart, encoded);
}

ECode Uri::PathPart::ReadFrom(
    /* [in] */ IParcel* parcel,
    /* [out] */ Uri::PathPart** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    Int32 representation;
    parcel->ReadInt32(&representation);

    AutoPtr<PathPart> part;
    String str1, str2;
    switch (representation) {
        case AbstractPart::Representation::BOTH: {
            parcel->ReadString(&str1);
            parcel->ReadString(&str2);
            part = From(str1, str2);
            break;
        }
        case AbstractPart::Representation::ENCODED: {
            parcel->ReadString(&str1);
            part = FromEncoded(str1);
            break;
        }
        case AbstractPart::Representation::DECODED: {
            parcel->ReadString(&str1);
            part = FromDecoded(str1);
            break;
        }
        default: {
            // throw new AssertionError();
            return E_ASSERTION_ERROR;
        }
    }

    *result = part;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

AutoPtr<Uri::PathPart> Uri::PathPart::FromEncoded(
    /* [in] */ const String& encoded)
{
    return From(encoded, NOT_CACHED);
}

AutoPtr<Uri::PathPart> Uri::PathPart::FromDecoded(
    /* [in] */ const String& decoded)
{
    return From(NOT_CACHED, decoded);
}

AutoPtr<Uri::PathPart> Uri::PathPart::From(
    /* [in] */ const String& encoded,
    /* [in] */ const String& decoded)
{
    if (encoded.IsNull()) {
        return sNULL;
    }

    if (encoded.IsEmpty()) {
        return sEMPTY;
    }

    return new PathPart(encoded, decoded);
}

/**
 * Prepends path values with "/" if they're present, not empty, and
 * they don't already start with "/".
 */
AutoPtr<Uri::PathPart> Uri::PathPart::MakeAbsolute(
    /* [in] */ Uri::PathPart* oldPart)
{
    // @SuppressWarnings("StringEquality")
    Boolean encodedCached = !oldPart->mEncoded.Equals(Uri::NOT_CACHED);

    // We don't care which version we use, and we don't want to force
    // unneccessary encoding/decoding.
    String oldPath = encodedCached ? oldPart->mEncoded : oldPart->mDecoded;
    if (oldPath.IsNullOrEmpty() || oldPath.StartWith("/")) {
        return oldPart;
    }

    // Prepend encoded string if present.
    String newEncoded = encodedCached ? String("/") + oldPart->mEncoded : Uri::NOT_CACHED;

    // Prepend decoded string if present.
    // @SuppressWarnings("StringEquality")
    Boolean decodedCached = !oldPart->mDecoded.Equals(
        Uri::NOT_CACHED);
    String newDecoded = decodedCached ? String("/") + oldPart->mDecoded : Uri::NOT_CACHED;

    return new PathPart(newEncoded, newDecoded);
}

//====================================================================================
// Uri
//====================================================================================
CAR_INTERFACE_IMPL_3(Uri, Object, IParcelable, IComparable, IUri)

const String Uri::LOG("Uri");
const String Uri::NOT_CACHED("NOT CACHED");
const Char32 Uri::HEX_DIGITS[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
const Int32 Uri::NOT_FOUND = -1;
const Int32 Uri::NOT_CALCULATED = -2;
const String Uri::NOT_HIERARCHICAL = String("This isn't a hierarchical URI.");
const String Uri::DEFAULT_ENCODING = String("UTF-8");
const Int32 Uri::NULL_TYPE_ID = 0;

AutoPtr<IUri> Uri::CreateEmpty()
{
    AutoPtr<IUri> result;
#if 0 // TODO: Waiting for CHierarchicalUri
#ifdef DROID_CORE
    AutoPtr<CHierarchicalUri> obj;
    CHierarchicalUri::NewByFriend(
        String(NULL),
        (Handle32)Uri::Part::sNULL.Get(),
        (Handle32)Uri::PathPart::sEMPTY.Get(),
        (Handle32)Uri::Part::sNULL.Get(),
        (Handle32)Uri::Part::sNULL.Get(),
        (CHierarchicalUri**)&obj);
    result = (IUri*)obj->Probe(EIID_IUri);
#else
    CHierarchicalUri::New(
        String(NULL),
        (Handle32)Uri::Part::sNULL.Get(),
        (Handle32)Uri::PathPart::sEMPTY.Get(),
        (Handle32)Uri::Part::sNULL.Get(),
        (Handle32)Uri::Part::sNULL.Get(),
        (IUri**)&result);
#endif
#endif
    return result;
}
const AutoPtr<IUri> Uri::EMPTY = CreateEmpty();

ECode Uri::GetEmpty(
    /* [out] */ IUri** result)
{
    VALIDATE_NOT_NULL(*result)
    *result = EMPTY;
    REFCOUNT_ADD(*result)
    return NOERROR;
}


ECode Uri::IsOpaque(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    Boolean isHierachical;
    IsHierarchical(&isHierachical);
    *result = !isHierachical;
    return NOERROR;
}

ECode Uri::IsAbsolute(
    /* [out] */ Boolean* isAbsolute)
{
    VALIDATE_NOT_NULL(isAbsolute);

     Boolean isRelative;
     IsRelative(&isRelative);
     *isAbsolute = !isRelative;
    return NOERROR;
}

ECode Uri::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (!(o instanceof Uri)) {
            return false;
        }
        Uri other = (Uri) o;
        return toString().equals(other.toString());
#endif
}

ECode Uri::GetHashCode(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return toString().hashCode();

#endif
}

ECode Uri::CompareTo(
    /* [in] */ IInterface* other,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return toString().compareTo(other.toString());

#endif
}

ECode Uri::ToSafeString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String scheme = getScheme();
        String ssp = getSchemeSpecificPart();
        if (scheme != null) {
            if (scheme.equalsIgnoreCase("tel") || scheme.equalsIgnoreCase("sip")
                    || scheme.equalsIgnoreCase("sms") || scheme.equalsIgnoreCase("smsto")
                    || scheme.equalsIgnoreCase("mailto")) {
                StringBuilder builder = new StringBuilder(64);
                builder.append(scheme);
                builder.append(':');
                if (ssp != null) {
                    for (int i=0; i<ssp.length(); i++) {
                        char c = ssp.charAt(i);
                        if (c == '-' || c == '@' || c == '.') {
                            builder.append(c);
                        } else {
                            builder.append('x');
                        }
                    }
                }
                return builder.toString();
            }
        }
        // Not a sensitive scheme, but let's still be conservative about
        // the data we include -- only the ssp, not the query params or
        // fragment, because those can often have sensitive info.
        StringBuilder builder = new StringBuilder(64);
        if (scheme != null) {
            builder.append(scheme);
            builder.append(':');
        }
        if (ssp != null) {
            builder.append(ssp);
        }
        return builder.toString();

#endif
}

ECode Uri::Parse(
    /* [in] */ const String& uriString,
    /* [out] */ IUri** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return new StringUri(uriString);

#endif
}

ECode Uri::FromFile(
    /* [in] */ IFile* file,
    /* [out] */ IUri** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (file == null) {
            throw new NullPointerException("file");
        }
        PathPart path = PathPart.fromDecoded(file.getAbsolutePath());
        return new HierarchicalUri(
                "file", Part.EMPTY, path, Part.NULL, Part.NULL);

#endif
}

ECode Uri::FromParts(
    /* [in] */ const String& scheme,
    /* [in] */ const String& ssp,
    /* [in] */ const String& fragment,
    /* [out] */ IUri** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (scheme == null) {
            throw new NullPointerException("scheme");
        }
        if (ssp == null) {
            throw new NullPointerException("ssp");
        }
        return new OpaqueUri(scheme, Part.fromDecoded(ssp),
                Part.fromDecoded(fragment));

#endif
}

ECode Uri::GetQueryParameterNames(
    /* [out] */ ISet** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (isOpaque()) {
            throw new UnsupportedOperationException(NOT_HIERARCHICAL);
        }
        String query = getEncodedQuery();
        if (query == null) {
            return Collections.emptySet();
        }
        Set<String> names = new LinkedHashSet<String>();
        int start = 0;
        do {
            int next = query.indexOf('&', start);
            int end = (next == -1) ? query.length() : next;
            int separator = query.indexOf('=', start);
            if (separator > end || separator == -1) {
                separator = end;
            }
            String name = query.substring(start, separator);
            names.add(decode(name));
            // Move start to end of name.
            start = end + 1;
        } while (start < query.length());
        return Collections.unmodifiableSet(names);

#endif
}

ECode Uri::GetQueryParameters(
    /* [in] */ const String& key,
    /* [out] */ IList** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (isOpaque()) {
            throw new UnsupportedOperationException(NOT_HIERARCHICAL);
        }
        if (key == null) {
          throw new NullPointerException("key");
        }
        String query = getEncodedQuery();
        if (query == null) {
            return Collections.emptyList();
        }
        String encodedKey;
        try {
            encodedKey = URLEncoder.encode(key, DEFAULT_ENCODING);
        } catch (UnsupportedEncodingException e) {
            throw new AssertionError(e);
        }
        ArrayList<String> values = new ArrayList<String>();
        int start = 0;
        do {
            int nextAmpersand = query.indexOf('&', start);
            int end = nextAmpersand != -1 ? nextAmpersand : query.length();
            int separator = query.indexOf('=', start);
            if (separator > end || separator == -1) {
                separator = end;
            }
            if (separator - start == encodedKey.length()
                    && query.regionMatches(start, encodedKey, 0, encodedKey.length())) {
                if (separator == end) {
                  values.add("");
                } else {
                  values.add(decode(query.substring(separator + 1, end)));
                }
            }
            // Move start to end of name.
            if (nextAmpersand != -1) {
                start = nextAmpersand + 1;
            } else {
                break;
            }
        } while (true);
        return Collections.unmodifiableList(values);

#endif
}

ECode Uri::GetQueryParameter(
    /* [in] */ const String& key,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (isOpaque()) {
            throw new UnsupportedOperationException(NOT_HIERARCHICAL);
        }
        if (key == null) {
            throw new NullPointerException("key");
        }
        final String query = getEncodedQuery();
        if (query == null) {
            return null;
        }
        final String encodedKey = encode(key, null);
        final int length = query.length();
        int start = 0;
        do {
            int nextAmpersand = query.indexOf('&', start);
            int end = nextAmpersand != -1 ? nextAmpersand : length;
            int separator = query.indexOf('=', start);
            if (separator > end || separator == -1) {
                separator = end;
            }
            if (separator - start == encodedKey.length()
                    && query.regionMatches(start, encodedKey, 0, encodedKey.length())) {
                if (separator == end) {
                    return "";
                } else {
                    String encodedValue = query.substring(separator + 1, end);
                    return UriCodec.decode(encodedValue, true, StandardCharsets.UTF_8, false);
                }
            }
            // Move start to end of name.
            if (nextAmpersand != -1) {
                start = nextAmpersand + 1;
            } else {
                break;
            }
        } while (true);
        return null;

#endif
}

ECode Uri::GetBooleanQueryParameter(
    /* [in] */ const String& key,
    /* [in] */ Boolean defaultValue,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String flag = getQueryParameter(key);
        if (flag == null) {
            return defaultValue;
        }
        flag = flag.toLowerCase(Locale.ROOT);
        return (!"false".equals(flag) && !"0".equals(flag));

#endif
}

ECode Uri::NormalizeScheme(
    /* [out] */ IUri** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String scheme = getScheme();
        if (scheme == null) return this;  // give up
        String lowerScheme = scheme.toLowerCase(Locale.ROOT);
        if (scheme.equals(lowerScheme)) return this;  // no change
        return buildUpon().scheme(lowerScheme).build();

#endif
}

ECode Uri::WriteToParcel(
    /* [in] */ IParcel* out)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (uri == null) {
            out.writeInt(NULL_TYPE_ID);
        } else {
            uri.writeToParcel(out, 0);
        }

#endif
}

ECode Uri::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Uri::ReadFromParcel(
    /* [in] */ IParcel* parcel,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri)
    assert(0 && "TODO");
    return NOERROR;
}

ECode Uri::Encode(
    /* [in] */ const String& s,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return encode(s, null);

#endif
}

ECode Uri::Encode(
    /* [in] */ const String& s,
    /* [in] */ const String& allow,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (s == null) {
            return null;
        }
        // Lazily-initialized buffers.
        StringBuilder encoded = null;
        int oldLength = s.length();
        // This loop alternates between copying over allowed characters and
        // encoding in chunks. This results in fewer method calls and
        // allocations than encoding one character at a time.
        int current = 0;
        while (current < oldLength) {
            // Start in "copying" mode where we copy over allowed chars.
            // Find the next character which needs to be encoded.
            int nextToEncode = current;
            while (nextToEncode < oldLength
                    && isAllowed(s.charAt(nextToEncode), allow)) {
                nextToEncode++;
            }
            // If there's nothing more to encode...
            if (nextToEncode == oldLength) {
                if (current == 0) {
                    // We didn't need to encode anything!
                    return s;
                } else {
                    // Presumably, we've already done some encoding.
                    encoded.append(s, current, oldLength);
                    return encoded.toString();
                }
            }
            if (encoded == null) {
                encoded = new StringBuilder();
            }
            if (nextToEncode > current) {
                // Append allowed characters leading up to this point.
                encoded.append(s, current, nextToEncode);
            } else {
                // assert nextToEncode == current
            }
            // Switch to "encoding" mode.
            // Find the next allowed character.
            current = nextToEncode;
            int nextAllowed = current + 1;
            while (nextAllowed < oldLength
                    && !isAllowed(s.charAt(nextAllowed), allow)) {
                nextAllowed++;
            }
            // Convert the substring to bytes and encode the bytes as
            // '%'-escaped octets.
            String toEncode = s.substring(current, nextAllowed);
            try {
                byte[] bytes = toEncode.getBytes(DEFAULT_ENCODING);
                int bytesLength = bytes.length;
                for (int i = 0; i < bytesLength; i++) {
                    encoded.append('%');
                    encoded.append(HEX_DIGITS[(bytes[i] & 0xf0) >> 4]);
                    encoded.append(HEX_DIGITS[bytes[i] & 0xf]);
                }
            } catch (UnsupportedEncodingException e) {
                throw new AssertionError(e);
            }
            current = nextAllowed;
        }
        // Encoded could still be null at this point if s is empty.
        return encoded == null ? s : encoded.toString();

#endif
}

ECode Uri::IsAllowed(
    /* [in] */ Char32 c,
    /* [in] */ const String& allow,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return (c >= 'A' && c <= 'Z')
                || (c >= 'a' && c <= 'z')
                || (c >= '0' && c <= '9')
                || "_-!.~'()*".indexOf(c) != NOT_FOUND
                || (allow != null && allow.indexOf(c) != NOT_FOUND);

#endif
}

ECode Uri::Decode(
    /* [in] */ const String& s,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (s == null) {
            return null;
        }
        return UriCodec.decode(s, false, StandardCharsets.UTF_8, false);

#endif
}

ECode Uri::WithAppendedPath(
    /* [in] */ IUri* baseUri,
    /* [in] */ const String& pathSegment,
    /* [out] */ IUri** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Builder builder = baseUri.buildUpon();
        builder = builder.appendEncodedPath(pathSegment);
        return builder.build();

#endif
}

ECode Uri::GetCanonicalUri(
    /* [out] */ IUri** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if ("file".equals(getScheme())) {
            final String canonicalPath;
            try {
                canonicalPath = new File(getPath()).getCanonicalPath();
            } catch (IOException e) {
                return this;
            }
            if (Environment.isExternalStorageEmulated()) {
                final String legacyPath = Environment.getLegacyExternalStorageDirectory()
                        .toString();
                // Splice in user-specific path when legacy path is found
                if (canonicalPath.startsWith(legacyPath)) {
                    return Uri.fromFile(new File(
                            Environment.getExternalStorageDirectory().toString(),
                            canonicalPath.substring(legacyPath.length() + 1)));
                }
            }
            return Uri.fromFile(new File(canonicalPath));
        } else {
            return this;
        }

#endif
}

ECode Uri::CheckFileUriExposed(
    /* [in] */ const String& location)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if ("file".equals(getScheme())) {
            StrictMode.onFileUriExposed(location);
        }

#endif
}

ECode Uri::IsPathPrefixMatch(
    /* [in] */ IUri* prefix,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (!Objects.equals(getScheme(), prefix.getScheme())) return false;
        if (!Objects.equals(getAuthority(), prefix.getAuthority())) return false;
        List<String> seg = getPathSegments();
        List<String> prefixSeg = prefix.getPathSegments();
        final int prefixSize = prefixSeg.size();
        if (seg.size() < prefixSize) return false;
        for (int i = 0; i < prefixSize; i++) {
            if (!Objects.equals(seg.get(i), prefixSeg.get(i))) {
                return false;
            }
        }
        return true;

#endif
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
