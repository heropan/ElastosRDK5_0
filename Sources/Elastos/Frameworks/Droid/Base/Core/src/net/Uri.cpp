

#include "net/Uri.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/utility/etl/Set.h>
#include <elastos/utility/etl/List.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>
#include <Elastos.CoreLibrary.h>
#ifdef DROID_CORE
#include "os/CEnvironment.h"
#include "net/CStringUri.h"
#include "net/CHierarchicalUri.h"
#include "net/COpaqueUri.h"
#include "net/CUriBuilder.h"
#else
#include "Elastos.Droid.Core.h"
#endif

using Elastos::Core::StringUtils;
using Elastos::Utility::Set;
using Elastos::Core::StringBuilder;
using Elastos::Utility::Etl::List;
using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::IO::CFile;
using Elastos::IO::Charset::ICharset;
using Elastos::IO::Charset::ICharsets;
using Elastos::IO::Charset::CCharsets;
using Elastos::IO::Charset::ICharsetHelper;
using Elastos::IO::Charset::CCharsetHelper;
using Elastos::Net::IURLEncoder;
using Elastos::Net::CURLEncoder;
using Elastos::Net::IUriCodecHelper;
using Elastos::Net::CUriCodecHelper;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Net::Uri;
using Elastos::Droid::Net::IUriBuilder;
using Elastos::Droid::Os::IEnvironment;
using Elastos::Droid::Os::CEnvironment;

namespace Elastos {
namespace Droid {
namespace Net {

//====================================================================================
//              Uri::PathSegments
//====================================================================================
AutoPtr<Uri::PathSegments> Uri::PathSegments::sEMPTY = new Uri::PathSegments(ArrayOf<String>::Alloc(0), 0);

Uri::PathSegments::PathSegments(
    /* [in] */ ArrayOf<String>* segments,
    /* [in] */ Int32 size)
    : mSegments(segments)
    , mSize(size)
{}

ECode Uri::PathSegments::Get(
    /* [in] */ Int32 index,
    /* [out] */ String* encoded)
{
    VALIDATE_NOT_NULL(encoded);

    if (index >= mSize) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    *encoded = (*mSegments)[index];
    return NOERROR;
}

Int32 Uri::PathSegments::Size()
{
    return mSize;
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
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
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
//              Uri
//====================================================================================
const String Uri::TAG("Uri");
const Char8 Uri::HEX_DIGITS[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};


// const String LOG = String("Uri"); // Uri.class.getSimpleName();
const String Uri::NOT_CACHED= String("NOT CACHED");
const String Uri::NOT_HIERARCHICAL = String("This isn't a hierarchical URI.");
const String Uri::DEFAULT_ENCODING = String("UTF-8");

AutoPtr<IUri> Uri::EMPTY;

AutoPtr<IUri> Uri::GetEMPTY()
{
    if (EMPTY == NULL) {
#ifdef DROID_CORE
        AutoPtr<CHierarchicalUri> obj;
        CHierarchicalUri::NewByFriend(
            String(NULL),
            (Handle32)Uri::Part::sNULL.Get(),
            (Handle32)Uri::PathPart::sEMPTY.Get(),
            (Handle32)Uri::Part::sNULL.Get(),
            (Handle32)Uri::Part::sNULL.Get(),
            (CHierarchicalUri**)&obj);
        EMPTY = (IUri*)obj->Probe(EIID_IUri);
#else
        CHierarchicalUri::New(
            String(NULL),
            (Handle32)Uri::Part::sNULL.Get(),
            (Handle32)Uri::PathPart::sEMPTY.Get(),
            (Handle32)Uri::Part::sNULL.Get(),
            (Handle32)Uri::Part::sNULL.Get(),
            (IUri**)&EMPTY);
#endif
    }
    return EMPTY;
}

ECode Uri::IsOpaque(
    /* [out] */ Boolean* isOpaque)
{
    VALIDATE_NOT_NULL(isOpaque);

    Boolean isHierachical;
    IsHierarchical(&isHierachical);
    *isOpaque = !isHierachical;
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
    /* [in] */ IUri* other,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual);
    *isEqual = FALSE;

    if (IUri::Probe(other) == NULL) {
        return NOERROR;
    }

    String str1, str2;
    FAIL_RETURN(ToString(&str1));
    FAIL_RETURN(other->ToString(&str2));
    *isEqual = str1.Equals(str2);
    return NOERROR;
}

ECode Uri::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    *hashCode = 0;

    String str1;
    FAIL_RETURN(ToString(&str1));

    *hashCode = str1.GetHashCode();
    return NOERROR;
}

ECode Uri::CompareTo(
    /* [in] */ IUri* other,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 1;
    VALIDATE_NOT_NULL(other);

    String str1, str2;
    FAIL_RETURN(ToString(&str1));
    FAIL_RETURN(other->ToString(&str2));

    *result = str1.Compare(str2);
    return NOERROR;
}

ECode Uri::ToSafeString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    String scheme, ssp;
    GetScheme(&scheme);
    GetSchemeSpecificPart(&ssp);

    StringBuilder builder(64);
    if (!scheme.IsNull()){
        if (scheme.EqualsIgnoreCase("tel")
            || scheme.EqualsIgnoreCase("sip")
            || scheme.EqualsIgnoreCase("sms")
            || scheme.EqualsIgnoreCase("smsto")
            || scheme.EqualsIgnoreCase("mailto")) {

            builder += scheme;
            builder += ":";
            if (!ssp.IsNull()) {
                AutoPtr<ArrayOf<Char32> > charArray = ssp.GetChars();
                Char32 c;
                for (Int32 i = 0; i < charArray->GetLength(); i++) {
                    c = (*charArray)[i];
                    if (c == '-' || c == '@' || c == '.') {
                        builder += c;
                    } else {
                        builder += "x";
                    }
                }
            }
            builder.ToString(result);
            return NOERROR;
        }
    }

    // Not a sensitive scheme, but let's still be conservative about
    // the data we include -- only the ssp, not the query params or
    // fragment, because those can often have sensitive info.
    if (!scheme.IsNull()) {
        builder += scheme;
        builder += ":";
    }

    if (!ssp.IsNull()) {
        builder += ssp;
    }

    builder.ToString(result);
    return NOERROR;
}

ECode Uri::Parse(
    /* [in] */ const String& uriString,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    *uri = NULL;

#ifdef DROID_CORE
    AutoPtr<CStringUri> curi;
    FAIL_RETURN(CStringUri::NewByFriend(uriString, (CStringUri**)&curi));
#else
    AutoPtr<IUri> curi;
    FAIL_RETURN(CStringUri::New(uriString, (IUri**)&curi));
#endif

    *uri = (IUri*)curi.Get();
    REFCOUNT_ADD(*uri);
    return NOERROR;
}

ECode Uri::FromFile(
    /* [in] */ IFile* file,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    *uri = NULL;
    VALIDATE_NOT_NULL(file);

    String str;
    FAIL_RETURN(file->GetAbsolutePath(&str));

    AutoPtr<PathPart> path;
    path = PathPart::FromDecoded(str);

    String fileStr("file");

#ifdef DROID_CORE
    AutoPtr<CHierarchicalUri> curi;
    FAIL_RETURN(CHierarchicalUri::NewByFriend(fileStr,
        (Handle32)Part::sEMPTY.Get(),
        (Handle32)path.Get(),
        (Handle32)Part::sNULL.Get(),
        (Handle32)Part::sNULL.Get(),
        (CHierarchicalUri**)&curi));
#else
    AutoPtr<IUri> curi;
    FAIL_RETURN(CHierarchicalUri::New(fileStr,
        (Handle32)Part::sEMPTY.Get(),
        (Handle32)path.Get(),
        (Handle32)Part::sNULL.Get(),
        (Handle32)Part::sNULL.Get(),
        (IUri**)&curi));
#endif

    *uri = (IUri*)curi.Get();
    REFCOUNT_ADD(*uri);
    return NOERROR;
}

ECode Uri::FromParts(
    /* [in] */ const String& scheme,
    /* [in] */ const String& ssp,
    /* [in] */ const String& fragment,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    *uri = NULL;

    if (scheme.IsNull()) {
        Slogger::E(TAG, "scheme==NULL");
        return E_NULL_POINTER_EXCEPTION;
    }

    if (ssp.IsNull()) {
        Slogger::E(TAG, "ssp==NULL");
        return E_NULL_POINTER_EXCEPTION;
    }

    AutoPtr<Part> part1 = Part::FromDecoded(ssp);
    AutoPtr<Part> part2 = Part::FromDecoded(fragment);

#ifdef DROID_CORE
    AutoPtr<COpaqueUri> curi;
    FAIL_RETURN(COpaqueUri::NewByFriend(scheme, (Handle32)part1.Get(),
        (Handle32)part2.Get(), (COpaqueUri**)&curi));
#else
    AutoPtr<IUri> curi;
    FAIL_RETURN(COpaqueUri::New(scheme, (Handle32)part1.Get(),
        (Handle32)part2.Get(), (IUri**)&curi));
#endif

    *uri = (IUri*)curi.Get();
    REFCOUNT_ADD(*uri);
    return NOERROR;
}

//@return a set of decoded names
ECode Uri::GetQueryParameterNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names)
    *names = NULL;

    Boolean isOpaque;
    IsOpaque(&isOpaque);
    if (isOpaque) {
        Slogger::E(TAG, NOT_HIERARCHICAL.string());
        return E_RUNTIME_EXCEPTION;
    }

    String query;
    GetEncodedQuery(&query);
    if (query.IsNull()) {
        return NOERROR;
    }

    Set<String> sets;
    Int32 start = 0, end, next, separator;
    Int32 queryLength = query.GetLength();
    String name, decodeString;
    do {
        next = query.IndexOf('&', start);
        end = (next == -1) ? queryLength : next;

        separator = query.IndexOf('=', start);
        if (separator > end || separator == -1) {
            separator = end;
        }

        name = query.Substring(start, separator);
        Decode(name, &decodeString);
        sets.Insert(decodeString);

        // Move start to end of name.
        start = end + 1;

    } while (start < queryLength);


    Int32 count = sets.GetSize();
    AutoPtr<ArrayOf<String> > array = ArrayOf<String>::Alloc(count);

    Set<String>::Iterator it;
    for (start = 0, it = sets.Begin(); it!= sets.End(); ++it, ++start) {
        array->Set(start, *it);
    }

    *names = array;
    REFCOUNT_ADD(*names);
    return NOERROR;
}

//@return a list of decoded values
ECode Uri::GetQueryParameters(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<String>**  params)
{
    VALIDATE_NOT_NULL(params);
    *params = NULL;

    Boolean isOpaque;
    IsOpaque(&isOpaque);
    if (isOpaque) {
        Slogger::E(TAG, NOT_HIERARCHICAL);
        return E_RUNTIME_EXCEPTION;
    }

    if (key == NULL) {
        Slogger::E(TAG, "key");
        return E_RUNTIME_EXCEPTION;
    }

    String query;
    GetEncodedQuery(&query);
    if (query.IsNull()) {
        return NOERROR;
    }

    AutoPtr<IURLEncoder> urlEncoderhelper;
    CURLEncoder::AcquireSingleton((IURLEncoder**)&urlEncoderhelper);
    AutoPtr<ICharsetHelper> charsetHelper;
    CCharsetHelper::AcquireSingleton((ICharsetHelper**)&charsetHelper);

    AutoPtr<ICharset> charset;
    charsetHelper->ForName(DEFAULT_ENCODING, (ICharset**)&charset);

    String encodedKey;
    urlEncoderhelper->Encode(key, DEFAULT_ENCODING, &encodedKey);
    Int32 encodedKeyLength = encodedKey.GetLength();

    List<String> values;
    Int32 start = 0, end, nextAmpersand, separator;
    String emptyStr(""), decodeString;

    do {
        nextAmpersand = query.IndexOf('&', start);
        end = nextAmpersand != -1 ? nextAmpersand : query.GetLength();

        separator = query.IndexOf('=', start);
        if (separator > end || separator == -1) {
            separator = end;
        }

        if (separator - start == encodedKeyLength
                && query.RegionMatches(start, encodedKey, 0, encodedKeyLength)) {
            if (separator == end) {
                values.PushBack(emptyStr);
            }
            else {
              Decode(query.Substring(separator + 1, end), &decodeString);
              values.PushBack(decodeString);
            }
        }

        // Move start to end of name.
        if (nextAmpersand != -1) {
            start = nextAmpersand + 1;
        }
        else {
            break;
        }
    } while (TRUE);


    Int32 count = values.GetSize();
    AutoPtr<ArrayOf<String> > array = ArrayOf<String>::Alloc(count);
    List<String>::Iterator it;
    for (start = 0, it = values.Begin(); it != values.End(); ++it, ++start) {
        array->Set(start, *it);
    }

    *params = array;
    REFCOUNT_ADD(*params);
    return NOERROR;
}

ECode Uri::GetQueryParameter(
    /* [in] */ const String& key,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    Boolean isOpaque;
    IsOpaque(&isOpaque);

    if (key.IsNull()) {
        Slogger::E(TAG, "key");
        return E_RUNTIME_EXCEPTION;
    }

    String query;
    GetEncodedQuery(&query);
    if (query.IsNull()) {
        return NOERROR;
    }

    String encodedKey;
    Encode(key, String(), &encodedKey);
    Int32 encodedKeyLength = encodedKey.GetLength();

    Int32 length = query.GetLength();
    Int32 start = 0, end, nextAmpersand, separator;
    String encodedValue, decodedValue;

    AutoPtr<IUriCodecHelper> urlCodechelper;
    CUriCodecHelper::AcquireSingleton((IUriCodecHelper**)&urlCodechelper);
    AutoPtr<ICharset> charset;
    AutoPtr<ICharsets> charsetHelper;
    CCharsets::AcquireSingleton((ICharsets**)&charsetHelper );
    charsetHelper->GetUTF_8((ICharset**)&charset);

    do {
        nextAmpersand = query.IndexOf('&', start);
        end = nextAmpersand != -1 ? nextAmpersand : length;

        separator = query.IndexOf('=', start);
        if (separator > end || separator == -1) {
            separator = end;
        }

        if (separator - start == encodedKeyLength
                && query.RegionMatches(start, encodedKey, 0, encodedKeyLength)) {
            if (separator == end) {
                *result = String("");
                return E_NULL_POINTER_EXCEPTION;
            }
            else {
                encodedValue = query.Substring(separator + 1, end);
                return urlCodechelper->Decode(encodedValue, TRUE, charset, FALSE, result);
            }
        }

        // Move start to end of name.
        if (nextAmpersand != -1) {
            start = nextAmpersand + 1;
        }
        else {
            break;
        }
    } while (true);

    return NOERROR;
}

ECode Uri::GetBooleanQueryParameter(
    /* [in] */ const String& key,
    /* [in] */ Boolean defaultValue,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = defaultValue;

    String flag;
    GetQueryParameter(key, &flag);
    if (flag.IsNull()) {
        return NOERROR;
    }

    flag = flag.ToLowerCase();
    *value = !flag.Equals("false") && !flag.Equals("0");
    return NOERROR;
}

ECode Uri::NormalizeScheme(
    /* [out] */ IUri** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    String scheme;
    GetScheme(&scheme);
    if (scheme.IsNull()) {
        *result = THIS_PROBE(IUri);
        REFCOUNT_ADD(*result);
        return NOERROR;
    }

    String lowerScheme = scheme.ToLowerCase();

    if (scheme.Equals(lowerScheme)) {
        *result = THIS_PROBE(IUri);
        REFCOUNT_ADD(*result);
        return NOERROR;
    }

    AutoPtr<IUriBuilder> builder;
    BuildUpon((IUriBuilder**)&builder);
    FAIL_RETURN(builder->Scheme(lowerScheme));
    FAIL_RETURN(builder->Build(result));
    return NOERROR;
}

ECode Uri::ReadFromParcel(
   /* [in] */ IParcel* parcel,
   /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    *uri = NULL;

   Int32 type;
   parcel->ReadInt32(&type);

   switch (type) {
       case NULL_TYPE_ID: return NOERROR;
       case StringUri::TYPE_ID: return StringUri::ReadFrom(parcel, uri);
       case OpaqueUri::TYPE_ID: return OpaqueUri::ReadFrom(parcel, uri);
       case HierarchicalUri::TYPE_ID: return HierarchicalUri::ReadFrom(parcel, uri);
   }

    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode Uri::WriteToParcel(
    /* [in] */ IParcel* out,
    /* [in] */ IUri* uri)
{
    if (uri == NULL) {
        out->WriteInt32(NULL_TYPE_ID);
    }
    else {
        IParcelable* p = IParcelable::Probe(uri);
        assert(p != NULL);
        p->WriteToParcel(out);
    }

    return NOERROR;
}

ECode Uri::Encode(
    /* [in] */ const String& s,
    /* [out] */ String* encoded)
{
    return Encode(s, String(NULL), encoded);
}

ECode Uri::Encode(
    /* [in] */ const String& s,
    /* [in] */ const String& allow,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    if (s.IsNull()) {
        return NOERROR;
    }

    // Lazily-initialized buffers.
    StringBuilder encoded;
    Int32 oldLength = s.GetLength();
    String subStr, toEncode;

    // This loop alternates between copying over allowed characters and
    // encoding in chunks. This results in fewer method calls and
    // allocations than encoding one character at a time.
    Int32 current = 0, nextToEncode, end, nextAllowed;
    Char32 c;
    while (current < oldLength) {
        // Start in "copying" mode where we copy over allowed chars.

        // Find the next character which needs to be encoded.
        nextToEncode = current;
        while (nextToEncode < oldLength && IsAllowed(s.GetChar(nextToEncode), allow)) {
            nextToEncode++;
        }

        // If there's nothing more to encode...
        if (nextToEncode == oldLength) {
            if (current == 0) {
                // We didn't need to encode anything!
                *result = s;
                return NOERROR;
            } else {
                // Presumably, we've already done some encoding.
                end = (current + oldLength);
                if (end > oldLength) {
                    end = oldLength;
                }
                subStr = s.Substring(current, end);
                encoded += subStr;
                *result = encoded.ToString();
                return NOERROR;
            }
        }

        if (nextToEncode > current) {
            // Append allowed characters leading up to this point.
            end = (current + nextToEncode);
            if (end > oldLength) {
                end = oldLength;
            }
            subStr = s.Substring(current, end);
            encoded += subStr;
        } else {
            // assert nextToEncode == current
        }

        // Switch to "encoding" mode.

        // Find the next allowed character.
        current = nextToEncode;
        nextAllowed = current + 1;
        while (nextAllowed < oldLength && !IsAllowed(s.GetChar(nextAllowed), allow)) {
            nextAllowed++;
        }

        // Convert the substring to bytes and encode the bytes as
        // '%'-escaped octets.
        toEncode = s.Substring(current, nextAllowed);
        // try {
            // TODO: ALEX fix getBytes
            // byte[] bytes = toEncode.getBytes(DEFAULT_ENCODING);
            // int bytesLength = bytes.length;
            Int32 bytesLength = toEncode.GetLength();
            for (Int32 i = 0; i < bytesLength; i++) {
                encoded += "%";
                encoded.AppendChar(HEX_DIGITS[(toEncode[i] & 0xf0) >> 4]);
                encoded.AppendChar(HEX_DIGITS[toEncode[i] & 0xf]);
            }
        // } catch (UnsupportedEncodingException e) {
        //     throw new AssertionError(e);
//             }

        current = nextAllowed;
    }

    // Encoded could still be null at this point if s is empty.
    String encodedStr = encoded.ToString();
    *result = encodedStr.IsNull() ? s : encodedStr;
    return NOERROR;
}

Boolean Uri::IsAllowed(
    /* [in] */ Char32 c,
    /* [in] */ const String& allow)
{
    String pattern("_-!.~'()*");
    return (c >= 'A' && c <= 'Z')
            || (c >= 'a' && c <= 'z')
            || (c >= '0' && c <= '9')
            || pattern.IndexOf(c) != NOT_FOUND
            || (!allow.IsNull() && allow.IndexOf(c) != NOT_FOUND);
}

ECode Uri::Decode(
    /* [in] */ const String& s,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    if (s.IsNull()) {
        return NOERROR;
    }

    AutoPtr<ICharsets> charsetsHelper;
    CCharsets::AcquireSingleton((ICharsets**)&charsetsHelper);
    AutoPtr<ICharset> charset;
    charsetsHelper->GetUTF_8((ICharset**)&charset);

    AutoPtr<IUriCodecHelper> uriCodecHelper;
    CUriCodecHelper::AcquireSingleton((IUriCodecHelper**)&uriCodecHelper);
    return uriCodecHelper->Decode(s, FALSE, charset, FALSE, result);
}

ECode Uri::WithAppendedPath(
    /* [in] */ IUri* baseUri,
    /* [in] */ const String& pathSegment,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    *uri = NULL;
    assert(baseUri);

    AutoPtr<IUriBuilder> builder;
    baseUri->BuildUpon((IUriBuilder**)&builder);
    builder->AppendEncodedPath(pathSegment);
    return builder->Build(uri);
}

ECode Uri::GetCanonicalUri(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    *uri = NULL;

    String scheme;
    GetScheme(&scheme);
    if (!scheme.IsNull() && scheme.Equals("file")) {
        String path;
        GetPath(&path);

        String canonicalPath;
        AutoPtr<IFile> file, tmpFile;
        ECode ec = CFile::New(path, (IFile**)&file);
        if (FAILED(ec)) goto _Exit_;
        ec = file->GetCanonicalPath(&canonicalPath);
        if (FAILED(ec)) goto _Exit_;

        AutoPtr<IEnvironment> env;
        CEnvironment::AcquireSingleton((IEnvironment**)&env);
        Boolean isEmulated;
        if (env->IsExternalStorageEmulated(&isEmulated), isEmulated) {
            AutoPtr<IFile> dirFile;
            env->GetLegacyExternalStorageDirectory((IFile**)&dirFile);
            assert(dirFile != NULL);
            String legacyPath;
            dirFile->ToString(&legacyPath);

            // Splice in user-specific path when legacy path is found
            if (!canonicalPath.IsNull() && canonicalPath.StartWith(legacyPath)) {
                dirFile = NULL;
                env->GetExternalStorageDirectory((IFile**)&dirFile);
                assert(dirFile != NULL);
                String dirPath, mode;
                dirFile->ToString(&dirPath);
                mode = canonicalPath.Substring(legacyPath.GetLength() + 1);
                CFile::New(dirPath, mode, (IFile**)&tmpFile);
                return Uri::FromFile(tmpFile, uri);
            }
        }

        CFile::New(canonicalPath, (IFile**)&tmpFile);
        return Uri::FromFile(tmpFile, uri);
    }

_Exit_:
    *uri = THIS_PROBE(IUri);
    REFCOUNT_ADD(*uri)
    return NOERROR;
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

ECode HierarchicalUri::Init(
    /* [in] */ const String& scheme,
    /* [in] */ Handle32 authority,  //Part*
    /* [in] */ Handle32 path,       //PathPart*
    /* [in] */ Handle32 query,      //Part*
    /* [in] */ Handle32 fragment)   //Part*
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

#ifdef DROID_CORE
    AutoPtr<CHierarchicalUri> curi;
    FAIL_RETURN(CHierarchicalUri::NewByFriend(str,
        (Handle32)p1.Get(), (Handle32)p2.Get(),
        (Handle32)p3.Get(), (Handle32)p4.Get(),
        (CHierarchicalUri**)&curi));
#else
    AutoPtr<IUri> curi;
    FAIL_RETURN(CHierarchicalUri::New(str,
        (Handle32)p1.Get(), (Handle32)p2.Get(),
        (Handle32)p3.Get(), (Handle32)p4.Get(),
        (IUri**)&curi));
#endif

    *result = (IUri*)curi.Get();
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
    /* [out, callee] */ ArrayOf<String>** segments)
{
    VALIDATE_NOT_NULL(segments);
    *segments = NULL;

    AutoPtr<Uri::PathSegments> pathSegments = mPath->GetPathSegments();
    if (pathSegments != NULL) {
        *segments = pathSegments->mSegments;
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
    FAIL_RETURN(builder->Authority((Handle32)mAuthority.Get()));
    FAIL_RETURN(builder->Path((Handle32)mPath.Get()));
    FAIL_RETURN(builder->Query((Handle32)mQuery.Get()));
    FAIL_RETURN(builder->Fragment((Handle32)mFragment.Get()));
    *result = (IUriBuilder*)builder.Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

//====================================================================
//                    StringUri
//====================================================================
StringUri::StringUri()
    : mCachedSsi(Uri::NOT_CALCULATED)
    , mCachedFsi(Uri::NOT_CALCULATED)
    , mScheme(Uri::NOT_CACHED)
{}

ECode StringUri::Init(
    /* [in] */ const String& uriString)
{
    if (uriString.IsNull()) {
        // throw new NullPointerException("uriString");
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

#ifdef DROID_CORE
    AutoPtr<CStringUri> curi;
    FAIL_RETURN(CStringUri::NewByFriend(str, (CStringUri**)&curi));
#else
    AutoPtr<IUri> curi;
    FAIL_RETURN(CStringUri::New(str, (IUri**)&curi));
#endif

    *uri = (IUri*)curi.Get();
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
    /* [out, callee] */ ArrayOf<String>** pathSegments)
{
    VALIDATE_NOT_NULL(pathSegments);
    *pathSegments = NULL;

    AutoPtr<Uri::PathPart> part = GetPathPart();
    AutoPtr<PathSegments> segments = part->GetPathSegments();

    if (segments != NULL) {
        Int32 size = segments->Size();
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
    return fsi == NOT_FOUND ?
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
        builder->Authority((Handle32)p.Get());
        AutoPtr<Uri::PathPart> pp = GetPathPart();
        builder->Path((Handle32)pp.Get());
        p = GetQueryPart();
        builder->Query((Handle32)p.Get());
        p = GetFragmentPart();
        builder->Fragment((Handle32)p.Get());
    }
    else {
        AutoPtr<Uri::Part> p = GetSsp();
        builder->OpaquePart((Handle32)p.Get());
        p = GetFragmentPart();
        builder->Fragment((Handle32)p.Get());
    }

    *result = (IUriBuilder*)builder.Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

//====================================================================
//                    OpaqueUri
//====================================================================

OpaqueUri::OpaqueUri()
{
}

ECode OpaqueUri::Init(
    /* [in] */ const String& scheme,
    /* [in] */ Handle32 ssp,
    /* [in] */ Handle32 fragment)
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

#ifdef DROID_CORE
    AutoPtr<COpaqueUri> curi;
    FAIL_RETURN(COpaqueUri::NewByFriend(str, (Handle32)p1.Get(),
        (Handle32)p2.Get(), (COpaqueUri**)&curi));
#else
    AutoPtr<IUri> curi;
    FAIL_RETURN(COpaqueUri::New(str, (Handle32)p1.Get(),
        (Handle32)p2.Get(), (IUri**)&curi));
#endif

    *result = (IUri*)curi.Get();
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
    /* [out, callee] */ ArrayOf<String>** pathSegments)
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
    VALIDATE_NOT_NULL(result);
    *result = NULL;

#ifdef DROID_CORE
    AutoPtr<CUriBuilder> builder;
    FAIL_RETURN(CUriBuilder::NewByFriend((CUriBuilder**)&builder));
#else
    AutoPtr<IUriBuilder> builder;
    FAIL_RETURN(CUriBuilder::New((IUriBuilder**)&builder));
#endif

    FAIL_RETURN((*result)->Scheme(mScheme));
    FAIL_RETURN((*result)->OpaquePart((Handle32)&mSsp));
    FAIL_RETURN((*result)->Fragment((Handle32)&mFragment));
    *result = (IUriBuilder*)builder.Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}


} // namespace Net
} // namespace Droid
} // namespace Elastos
