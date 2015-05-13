
#ifndef __ELASTOS_DROID_NET_URI_H__
#define __ELASTOS_DROID_NET_URI_H__

#include "ext/frameworkext.h"
#include <elastos/StringBuilder.h>
#include <elastos/List.h>

using Elastos::Core::StringBuilder;
using Elastos::Utility::List;
using Elastos::IO::IFile;

namespace Elastos {
namespace Droid {
namespace Net {

class Uri
{
public:
    /**
     * Wrapper for path segment array.
     */
    class PathSegments : public ElRefBase
    {
    public:
        PathSegments(
            /* [in] */ ArrayOf<String>* segments,
            /* [in] */ Int32 size);

        virtual CARAPI Get(
            /* [in] */ Int32 index,
            /* [out] */ String* value);

        virtual CARAPI_(Int32) Size();

    public:
        static AutoPtr<PathSegments> sEMPTY;

        AutoPtr< ArrayOf<String> > mSegments;
        Int32 mSize;
    };

    /**
     * Builds PathSegments.
     */
    class PathSegmentsBuilder : public ElRefBase
    {
    public:
        PathSegmentsBuilder();

        CARAPI Add(
            /* [in] */ const String& segment);

        CARAPI_(AutoPtr<PathSegments>) Build();
    private:
        List<String> mSegments;
        Int32 mSize;
    };

    /**
     * Support for part implementations.
     */
    class AbstractPart : public ElRefBase
    {
    public:
        /**
         * Enum which indicates which representation of a given part we have.
         */
        class Representation
        {
        public:
            static const Int32 BOTH = 0;
            static const Int32 ENCODED = 1;
            static const Int32 DECODED = 2;
        };

    public:
        AbstractPart(
            /* [in] */ const String& encoded,
            /* [in] */ const String& decoded);

        virtual CARAPI_(String) GetEncoded() = 0;

        CARAPI_(String) GetDecoded();

        CARAPI WriteTo(
            /* [in] */ IParcel* parcel);

    public:
        /* volatile */ String mEncoded;
        /* volatile */ String mDecoded;
    };

    /**
     * Immutable wrapper of encoded and decoded versions of a URI part. Lazily
     * creates the encoded or decoded version from the other.
     */
    class Part : public AbstractPart
    {
    protected:
        Part(
            /* [in] */ const String& encoded,
            /* [in] */ const String& decoded);

    public:
        virtual CARAPI_(Boolean) IsEmpty();

        virtual CARAPI_(String) GetEncoded();

        static CARAPI ReadFrom(
            /* [in] */ IParcel* parcel,
            /* [out] */ Part** part);

        /**
         * Returns given part or {@link #NULL} if the given part is null.
         */
        static CARAPI_(AutoPtr<Part>) NonNull(
            /* [in] */ Part* part);

        /**
         * Creates a part from the encoded string.
         *
         * @param encoded part string
         */
        static CARAPI_(AutoPtr<Part>) FromEncoded(
            /* [in] */ const String& encoded);

        /**
         * Creates a part from the decoded string.
         *
         * @param decoded part string
         */
        static CARAPI_(AutoPtr<Part>) FromDecoded(
            /* [in] */ const String& decoded);

        /**
         * Creates a part from the encoded and decoded strings.
         *
         * @param encoded part string
         * @param decoded part string
         */
        static CARAPI_(AutoPtr<Part>) From(
            /* [in] */ const String& encoded,
            /* [in] */ const String& decoded);

    public:
        /** A part with null values. */
        static const AutoPtr<Part> sNULL;

        /** A part with empty strings for values. */
        static const AutoPtr<Part> sEMPTY;
    };

    class EmptyPart : public Part
    {
    public:
        EmptyPart(
            /* [in] */ const String& value)
            : Part(value, value)
        {}

        // @Override
        virtual CARAPI_(Boolean) IsEmpty()
        {
            return TRUE;
        }
    };

    /**
     * Immutable wrapper of encoded and decoded versions of a path part. Lazily
     * creates the encoded or decoded version from the other.
     */
    class PathPart : public AbstractPart
    {
    protected:
        PathPart(
            /* [in] */ const String& encoded,
            /* [in] */ const String& decoded);

    public:
        virtual CARAPI_(String) GetEncoded();

        /**
         * Gets the individual path segments. Parses them if necessary.
         *
         * @return parsed path segments or null if this isn't a hierarchical
         *  URI
         */
        virtual CARAPI_(AutoPtr<PathSegments>) GetPathSegments();

        static CARAPI_(AutoPtr<PathPart>) AppendEncodedSegment(
            /* [in] */ PathPart* oldPart,
            /* [in] */ const String& newSegment);

        static CARAPI_(AutoPtr<PathPart>) AppendDecodedSegment(
            /* [in] */ PathPart* oldPart,
            /* [in] */ const String& decoded);

        static CARAPI ReadFrom(
            /* [in] */ IParcel* parcel,
            /* [out] */ PathPart** result);

        /**
         * Creates a path from the encoded string.
         *
         * @param encoded part string
         */
        static CARAPI_(AutoPtr<PathPart>) FromEncoded(
            /* [in] */ const String& encoded);

        /**
         * Creates a path from the decoded string.
         *
         * @param decoded part string
         */
        static CARAPI_(AutoPtr<PathPart>) FromDecoded(
            /* [in] */ const String& decoded);

        /**
         * Creates a path from the encoded and decoded strings.
         *
         * @param encoded part string
         * @param decoded part string
         */
        static CARAPI_(AutoPtr<PathPart>) From(
            /* [in] */ const String& encoded,
            /* [in] */ const String& decoded);

        /**
         * Prepends path values with "/" if they're present, not empty, and
         * they don't already start with "/".
         */
        static CARAPI_(AutoPtr<PathPart>) MakeAbsolute(
            /* [in] */ PathPart* oldPart);

        /** A part with null values. */
        static const AutoPtr<PathPart> sNULL;

        /** A part with empty strings for values. */
        static const AutoPtr<PathPart> sEMPTY;

    private:
        /**
         * Cached path segments. This doesn't need to be volatile--we don't
         * care if other threads see the result.
         */
        AutoPtr<PathSegments> mPathSegments;
    };

public:
    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    virtual CARAPI IsHierarchical(
        /* [out] */ Boolean* isHierarchical) = 0;

    virtual CARAPI IsOpaque(
        /* [out] */ Boolean* isOpaque);

    virtual CARAPI IsRelative(
        /* [out] */ Boolean* isRelative) = 0;

    CARAPI IsAbsolute(
        /* [out] */ Boolean* isAbsolute);

    virtual CARAPI GetScheme(
        /* [out] */ String* scheme) = 0;

    virtual CARAPI GetSchemeSpecificPart(
        /* [out] */ String* ssp) = 0;

    virtual CARAPI GetEncodedSchemeSpecificPart(
        /* [out] */ String* essp) = 0;

    virtual CARAPI GetAuthority(
        /* [out] */ String* authority) = 0;

    virtual CARAPI GetEncodedAuthority(
        /* [out] */ String* authority) = 0;

    virtual CARAPI GetUserInfo(
        /* [out] */ String* userInfo) = 0;

    virtual CARAPI GetEncodedUserInfo(
        /* [out] */ String* userInfo) = 0;

    virtual CARAPI GetHost(
        /* [out] */ String* host) = 0;

    virtual CARAPI GetPort(
        /* [out] */ Int32* port) = 0;

    virtual CARAPI GetPath(
        /* [out] */ String* path) = 0;

    virtual CARAPI GetEncodedPath(
        /* [out] */ String* path) = 0;

    virtual CARAPI GetQuery(
        /* [out] */ String* query) = 0;

    virtual CARAPI GetEncodedQuery(
        /* [out] */ String* query) = 0;

    virtual CARAPI GetFragment(
        /* [out] */ String* fragment) = 0;

    virtual CARAPI GetEncodedFragment(
        /* [out] */ String* fragment) = 0;

    //anthony check
    virtual CARAPI GetPathSegments(
        /* [out, callee] */ ArrayOf<String>** pathSegments) = 0;

    virtual CARAPI GetLastPathSegment(
        /* [out] */ String* pathSegment) = 0;

    /**
     * Compares this Uri to another object for equality. Returns true if the
     * encoded string representations of this Uri and the given Uri are
     * equal. Case counts. Paths are not normalized. If one Uri specifies a
     * default port explicitly and the other leaves it implicit, they will not
     * be considered equal.
     */
    CARAPI Equals(
        /* [in] */ IUri* other,
        /* [out] */ Boolean* isEqual);

    /**
     * Hashes the encoded string represention of this Uri consistently with
     * {@link #equals(Object)}.
     */
    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    /**
     * Compares the string representation of this Uri with that of
     * another.
     */
    CARAPI CompareTo(
        /* [in] */ IUri* other,
        /* [out] */ Int32* result);

    virtual CARAPI ToString(
        /* [out] */ String* result) = 0;

    /**
     * Return a string representation of the URI that is safe to print
     * to logs and other places where PII should be avoided.
     * @hide
     */
    CARAPI ToSafeString(
        /* [out] */ String* result);

    virtual CARAPI BuildUpon(
        /* [out] */ IUriBuilder** result) = 0;

    static CARAPI Parse(
        /* [in] */ const String& uriString,
        /* [out] */ IUri** uri);

    /**
     * Creates a Uri from a file. The URI has the form
     * "file://<absolute path>". Encodes path characters with the exception of
     * '/'.
     *
     * <p>Example: "file:///tmp/android.txt"
     *
     * @throws NullPointerException if file is null
     * @return a Uri for the given file
     */
    static CARAPI FromFile(
        /* [in] */ IFile* file,
        /* [out] */ IUri** uri);

    /**
     * Creates an opaque Uri from the given components. Encodes the ssp
     * which means this method cannot be used to create hierarchical URIs.
     *
     * @param scheme of the URI
     * @param ssp scheme-specific-part, everything between the
     *  scheme separator (':') and the fragment separator ('#'), which will
     *  get encoded
     * @param fragment fragment, everything after the '#', null if undefined,
     *  will get encoded
     *
     * @throws NullPointerException if scheme or ssp is null
     * @return Uri composed of the given scheme, ssp, and fragment
     *
     * @see Builder if you don't want the ssp and fragment to be encoded
     */
    static CARAPI FromParts(
        /* [in] */ const String& scheme,
        /* [in] */ const String& ssp,
        /* [in] */ const String& fragment,
        /* [out] */ IUri** uri);

    /**
     * Returns a set of the unique names of all query parameters. Iterating
     * over the set will return the names in order of their first occurrence.
     *
     * @throws UnsupportedOperationException if this isn't a hierarchical URI
     *
     * @return a set of decoded names
     */
    virtual CARAPI GetQueryParameterNames(
        /* [out, callee] */ ArrayOf<String>** names);

    /**
     * Searches the query string for parameter values with the given key.
     *
     * @param key which will be encoded
     *
     * @throws UnsupportedOperationException if this isn't a hierarchical URI
     * @throws NullPointerException if key is null
     * @return a list of decoded values
     */
    virtual CARAPI GetQueryParameters(
        /* [in] */ const String& key,
        /* [out, callee] */ ArrayOf<String>** values);

    /**
     * Searches the query string for the first value with the given key.
     *
     * <p><strong>Warning:</strong> Prior to Ice Cream Sandwich, this decoded
     * the '+' character as '+' rather than ' '.
     *
     * @param key which will be encoded
     * @throws UnsupportedOperationException if this isn't a hierarchical URI
     * @throws NullPointerException if key is null
     * @return the decoded value or null if no parameter is found
     */
    virtual CARAPI GetQueryParameter(
        /* [in] */ const String& key,
        /* [out] */ String* value);

    /**
     * Searches the query string for the first value with the given key and interprets it
     * as a boolean value. "false" and "0" are interpreted as <code>false</code>, everything
     * else is interpreted as <code>true</code>.
     *
     * @param key which will be decoded
     * @param defaultValue the default value to return if there is no query parameter for key
     * @return the boolean interpretation of the query parameter key
     */
    virtual CARAPI GetBooleanQueryParameter(
        /* [in] */ const String& key,
        /* [in] */ Boolean defaultValue,
        /* [out] */ Boolean* value);

    /**
     * Return an equivalent URI with a lowercase scheme component.
     * This aligns the Uri with Android best practices for
     * intent filtering.
     *
     * <p>For example, "HTTP://www.android.com" becomes
     * "http://www.android.com"
     *
     * <p>All URIs received from outside Android (such as user input,
     * or external sources like Bluetooth, NFC, or the Internet) should
     * be normalized before they are used to create an Intent.
     *
     * <p class="note">This method does <em>not</em> validate bad URI's,
     * or 'fix' poorly formatted URI's - so do not use it for input validation.
     * A Uri will always be returned, even if the Uri is badly formatted to
     * begin with and a scheme component cannot be found.
     *
     * @return normalized Uri (never null)
     * @see {@link android.content.Intent#setData}
     * @see {@link #setNormalizedData}
     */
    virtual CARAPI NormalizeScheme(
        /* [out] */ IUri** result);

    static CARAPI ReadFromParcel(
       /* [in] */ IParcel* parcel,
       /* [out] */ IUri** uri);

    static CARAPI WriteToParcel(
        /* [in] */ IParcel* out,
        /* [in] */ IUri* uri);

    static CARAPI Encode(
        /* [in] */ const String& s,
        /* [out] */ String* encoded);

    //anthony check
    //Modify it will change other code
    static CARAPI Encode(
        /* [in] */ const String& s,
        /* [in] */ const String& allow,
        /* [out] */ String* result);

    static CARAPI Decode(
        /* [in] */ const String& s,
        /* [out] */ String* decode);

    static CARAPI WithAppendedPath(
        /* [in] */ IUri* baseUri,
        /* [in] */ const String& pathSegment,
        /* [out] */ IUri** uri);

    /**
     * If this {@link Uri} is {@code file://}, then resolve and return its
     * canonical path. Also fixes legacy emulated storage paths so they are
     * usable across user boundaries. Should always be called from the app
     * process before sending elsewhere.
     *
     * @hide
     */
    virtual CARAPI GetCanonicalUri(
        /* [out] */ IUri** uri);

    static CARAPI_(AutoPtr<IUri>) GetEMPTY();

private:
    /**
     * Returns true if the given character is allowed.
     *
     * @param c character to check
     * @param allow characters to allow
     * @return true if the character is allowed or false if it should be
     *  encoded
     */
    static CARAPI_(Boolean) IsAllowed(
        /* [in] */ Char32 c,
        /* [in] */ const String& allow);

private:
    /**
     * The empty URI, equivalent to "".
     */
    static AutoPtr<IUri> EMPTY;// = new HierarchicalUri(null, Part.NULL,
//            PathPart.EMPTY, Part.NULL, Part.NULL);
public:

    /** Log tag. */
    static const String LOG;
    static const String TAG;

    /**
     * NOTE: EMPTY accesses this field during its own initialization, so this
     * field *must* be initialized first, or else EMPTY will see a null value!
     *
     * Placeholder for strings which haven't been cached. This enables us
     * to cache null. We intentionally create a new String instance so we can
     * compare its identity and there is no chance we will confuse it with
     * user data.
     */
    static const String NOT_CACHED;

    /** Index of a component which was not found. */
    static const Int32 NOT_FOUND = -1;

    /** Placeholder value for an index which hasn't been calculated yet. */
    static const Int32 NOT_CALCULATED = -2;

    /**
     * Error message presented when a user tries to treat an opaque URI as
     * hierarchical.
     */
    static const String NOT_HIERARCHICAL;

    /** Default encoding. */
    static const String DEFAULT_ENCODING;

    /** Identifies a null parcelled Uri. */
    static const Int32 NULL_TYPE_ID = 0;

    static const Char8 HEX_DIGITS[];// = "0123456789ABCDEF";
};

/**
 * Support for hierarchical URIs.
 */
class AbstractHierarchicalUri : public Uri
{
public:
    AbstractHierarchicalUri();

    virtual CARAPI GetLastPathSegment(
        /* [out] */ String* pathSegment);

    virtual CARAPI GetEncodedUserInfo(
        /* [out] */ String* userInfo);

    virtual CARAPI GetUserInfo(
        /* [out] */ String* userInfo);

    virtual CARAPI GetHost(
        /* [out] */ String* host);

    virtual CARAPI GetPort(
        /* [out] */ Int32* port);

private:
    CARAPI_(AutoPtr<Uri::Part>) GetUserInfoPart();

    CARAPI_(String) ParseUserInfo();

    CARAPI_(String) ParseHost();

    CARAPI_(Int32) ParsePort();

private:
    AutoPtr<Uri::Part> mUserInfo;
    String mHost;
    Int32 mPort;
};

class HierarchicalUri : public AbstractHierarchicalUri {
public:
    HierarchicalUri();

    HierarchicalUri(
        /* [in] */ const String& scheme,
        /* [in] */ Uri::Part* authority,
        /* [in] */ Uri::PathPart* path,
        /* [in] */ Uri::Part* query,
        /* [in] */ Uri::Part* fragment);

    CARAPI Init(
        /* [in] */ const String& scheme,
        /* [in] */ Handle32 authority,  //Part*
        /* [in] */ Handle32 path,       //PathPart*
        /* [in] */ Handle32 query,      //Part*
        /* [in] */ Handle32 fragment);  //Part*

    static CARAPI ReadFrom(
        /* [in] */ IParcel* parcel,
        /* [out] */ IUri** result);

    virtual CARAPI ReadFromParcel(
        /* [in] */ IParcel* parcel);

    virtual CARAPI WriteToParcel(
        /* [in] */ IParcel* parcel);

    virtual CARAPI IsHierarchical(
        /* [out] */ Boolean* isHierarchical);

    virtual CARAPI IsRelative(
        /* [out] */ Boolean* isRelative);

    virtual CARAPI GetScheme(
        /* [out] */ String* scheme);

    virtual CARAPI GetEncodedSchemeSpecificPart(
        /* [out] */ String* part);

    virtual CARAPI GetSchemeSpecificPart(
        /* [out] */ String* part);

    virtual CARAPI GetAuthority(
        /* [out] */ String* authority);

    virtual CARAPI GetEncodedAuthority(
        /* [out] */ String* authority);

    virtual CARAPI GetEncodedPath(
        /* [out] */ String* path);

    virtual CARAPI GetPath(
        /* [out] */ String* path);

    virtual CARAPI GetQuery(
        /* [out] */ String* query);

    virtual CARAPI GetEncodedQuery(
        /* [out] */ String* query);

    virtual CARAPI GetFragment(
        /* [out] */ String* fragment);

    virtual CARAPI GetEncodedFragment(
        /* [out] */ String* fragment);

    // return a list of string
    virtual CARAPI GetPathSegments(
        /* [out, callee] */ ArrayOf<String>** segments);

    virtual CARAPI ToString(
        /* [out] */ String* info);

    virtual CARAPI BuildUpon(
        /* [out] */ IUriBuilder** builder);

private:
    CARAPI_(AutoPtr<Part>) GetSsp();

    /**
     * Creates the encoded scheme-specific part from its sub parts.
     */
    CARAPI_(String) MakeSchemeSpecificPart();

    CARAPI AppendSspTo(StringBuilder& builder);

    CARAPI_(String) MakeUriString();

public:
    /** Used in parcelling. */
    static const Int32 TYPE_ID = 3;

private:
    String mScheme; // can be null
    AutoPtr<Uri::Part> mAuthority;
    AutoPtr<Uri::PathPart> mPath;
    AutoPtr<Uri::Part> mQuery;
    AutoPtr<Uri::Part> mFragment;

    AutoPtr<Uri::Part> mSsp;
    String mUriString;// = NOT_CACHED;;
};

/**
 * An implementation which wraps a String URI. This URI can be opaque or
 * hierarchical, but we extend AbstractHierarchicalUri in case we need
 * the hierarchical functionality.
 */
class StringUri : public AbstractHierarchicalUri
{
protected:
    StringUri();

    CARAPI Init(
        /* [in] */ const String& uriString);

public:
    static CARAPI ReadFrom(
        /* [in] */ IParcel* parcel,
        /* [out] */ IUri** uri);

    virtual CARAPI ReadFromParcel(
        /* [in] */ IParcel* parcel);

    virtual CARAPI WriteToParcel(
        /* [in] */ IParcel* parcel);

    virtual CARAPI IsHierarchical(
        /* [out] */ Boolean* isHierarchical);

    virtual CARAPI IsRelative(
        /* [out] */ Boolean* isRelative);

    virtual CARAPI GetScheme(
        /* [out] */ String* scheme);

    virtual CARAPI GetEncodedSchemeSpecificPart(
        /* [out] */ String* essp);

    virtual CARAPI GetSchemeSpecificPart(
        /* [out] */ String* ssp);

    virtual CARAPI GetEncodedAuthority(
        /* [out] */ String* authority);

    virtual CARAPI GetAuthority(
        /* [out] */ String* authority);

    virtual CARAPI GetPath(
        /* [out] */ String* path);

    virtual CARAPI GetEncodedPath(
        /* [out] */ String* path);

    virtual CARAPI GetPathSegments(
        /* [out, callee] */ ArrayOf<String>** pathSegments);

    virtual CARAPI GetEncodedQuery(
        /* [out] */ String* query);

    virtual CARAPI GetQuery(
        /* [out] */ String* query);

    virtual CARAPI GetEncodedFragment(
        /* [out] */ String* fragment);

    virtual CARAPI GetFragment(
        /* [out] */ String* fragment);

    virtual CARAPI ToString(
        /* [out] */ String* result);

    /**
     * Parses an authority out of the given URI string.
     *
     * @param uriString URI string
     * @param ssi scheme separator index, -1 for a relative URI
     *
     * @return the authority or null if none is found
     */
    static CARAPI_(String) ParseAuthority(
        /* [in] */ const String& uriString,
        /* [in] */ Int32 ssi);

    /**
     * Parses a path out of this given URI string.
     *
     * @param uriString URI string
     * @param ssi scheme separator index, -1 for a relative URI
     *
     * @return the path
     */
    static CARAPI_(String) ParsePath(
        /* [in] */ const String& uriString,
        /* [in] */ Int32 ssi);

    virtual CARAPI BuildUpon(
        /* [out] */ IUriBuilder** result);

private:
    /** Finds the first ':'. Returns -1 if none found. */
    CARAPI_(Int32) FindSchemeSeparator();

    /** Finds the first '#'. Returns -1 if none found. */
    CARAPI_(Int32) FindFragmentSeparator();

    CARAPI_(String) ParseScheme();

    CARAPI_(AutoPtr<Uri::Part>) GetSsp();

    CARAPI_(String) ParseSsp();

    CARAPI_(AutoPtr<Uri::Part>) GetAuthorityPart();

    CARAPI_(AutoPtr<Uri::PathPart>) GetPathPart();

    CARAPI_(String) ParsePath();

    CARAPI_(AutoPtr<Uri::Part>) GetQueryPart();

    CARAPI_(String) ParseQuery();

    CARAPI_(AutoPtr<Uri::Part>) GetFragmentPart();

    CARAPI_(String) ParseFragment();

public:
    /** Used in parcelling. */
    static const Int32 TYPE_ID = 1;

protected:
    /** URI string representation. */
    String mUriString;

    /** Cached scheme separator index. */
    volatile Int32 mCachedSsi;// = NOT_CALCULATED;

    /** Cached fragment separator index. */
    volatile Int32 mCachedFsi;// = NOT_CALCULATED;

    /* volatile */ String mScheme;
    AutoPtr<Uri::Part> mSsp;
    AutoPtr<Uri::Part> mAuthority;
    AutoPtr<Uri::PathPart> mPath;
    AutoPtr<Uri::Part> mQuery;
    AutoPtr<Uri::Part> mFragment;
};

class OpaqueUri : public Uri
{
protected:
    OpaqueUri();

    CARAPI Init(
        /* [in] */ const String& scheme,
        /* [in] */ Handle32 ssp,
        /* [in] */ Handle32 fragment);

public:
    static CARAPI ReadFrom(
        /* [in] */ IParcel* parcel,
        /* [out] */ IUri** uri);

    virtual CARAPI ReadFromParcel(
        /* [in] */ IParcel* parcel);

    virtual CARAPI WriteToParcel(
        /* [in] */ IParcel* parcel);

    virtual CARAPI IsHierarchical(
        /* [out] */ Boolean* isHierarchical);

    virtual CARAPI IsRelative(
        /* [out] */ Boolean* isRelative);

    virtual CARAPI GetScheme(
        /* [out] */ String* scheme);

    virtual CARAPI GetEncodedSchemeSpecificPart(
        /* [out] */ String* essp);

    virtual CARAPI GetSchemeSpecificPart(
        /* [out] */ String* ssp);

    virtual CARAPI GetEncodedAuthority(
        /* [out] */ String* authority);

    virtual CARAPI GetAuthority(
        /* [out] */ String* authority);

    virtual CARAPI GetPath(
        /* [out] */ String* path);

    virtual CARAPI GetEncodedPath(
        /* [out] */ String* path);

    virtual CARAPI GetQuery(
        /* [out] */ String* query);

    virtual CARAPI GetEncodedQuery(
        /* [out] */ String* query);

    virtual CARAPI GetFragment(
        /* [out] */ String* fragment);

    virtual CARAPI GetEncodedFragment(
        /* [out] */ String* fragment);

    virtual CARAPI GetPathSegments(
        /* [out, callee] */ ArrayOf<String>** pathSegments);

    virtual CARAPI GetLastPathSegment(
        /* [out] */ String* fragment);

    virtual CARAPI GetUserInfo(
        /* [out] */ String* fragment);

    virtual CARAPI GetEncodedUserInfo(
        /* [out] */ String* fragment);

    virtual CARAPI GetHost(
        /* [out] */ String* fragment);

    virtual CARAPI GetPort(
        /* [out] */ Int32* fragment);

    virtual CARAPI ToString(
        /* [out] */ String* result);

    virtual CARAPI BuildUpon(
        /* [out] */ IUriBuilder** result);

public:
    /** Used in parcelling. */
    static const Int32 TYPE_ID = 2;

private:
    String mScheme;
    AutoPtr<Uri::Part> mSsp;
    AutoPtr<Uri::Part> mFragment;
    String mCachedString;
};


} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_NET_URI_H__
