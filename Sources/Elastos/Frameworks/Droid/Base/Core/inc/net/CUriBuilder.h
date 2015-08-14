
#ifndef __ELASTOS_DROID_NET_CURIBUILDER_H__
#define __ELASTOS_DROID_NET_CURIBUILDER_H__

#include "_CUriBuilder.h"
#include "net/Uri.h"

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * Helper class for building or manipulating URI references. Not safe for
 * concurrent use.
 *
 * <p>An absolute hierarchical URI reference follows the pattern:
 * {@code <scheme>://<authority><absolute path>?<query>#<fragment>}
 *
 * <p>Relative URI references (which are always hierarchical) follow one
 * of two patterns: {@code <relative or absolute path>?<query>#<fragment>}
 * or {@code //<authority><absolute path>?<query>#<fragment>}
 *
 * <p>An opaque URI follows this pattern:
 * {@code <scheme>:<opaque part>#<fragment>}
 *
 * <p>Use {@link Uri#buildUpon()} to obtain a builder representing an existing URI.
 */
CarClass(CUriBuilder)
{
public:
    CARAPI Scheme(
        /* [in] */ const String& scheme);

    CARAPI OpaquePart(
        /* [in] */ Handle32 opaquePart);

    CARAPI OpaquePart(
        /* [in] */ const String& opaquePart);

    CARAPI EncodedOpaquePart(
        /* [in] */ const String& opaquePart);

    CARAPI Authority(
        /* [in] */ Handle32 authority);

    CARAPI Authority(
        /* [in] */ const String& authority);

    CARAPI EncodedAuthority(
        /* [in] */ const String& authority);

    CARAPI Path(
        /* [in] */ Handle32 path);

    CARAPI Path(
        /* [in] */ const String& path);

    CARAPI EncodedPath(
        /* [in] */ const String& path);

    CARAPI AppendPath(
        /* [in] */ const String& newSegment);

    CARAPI AppendEncodedPath(
        /* [in] */ const String& newSegment);

    CARAPI Query(
        /* [in] */ Handle32 query);

    CARAPI Query(
        /* [in] */ const String& query);

    CARAPI EncodedQuery(
        /* [in] */ const String& query);

    CARAPI Fragment(
        /* [in] */ Handle32 fragment);

    CARAPI Fragment(
        /* [in] */ const String& fragment);

    CARAPI EncodedFragment(
        /* [in] */ const String& fragment);

    CARAPI AppendQueryParameter(
        /* [in] */ const String& key,
        /* [in] */ const String& value);

    CARAPI ClearQuery();

    CARAPI Build(
        /* [out] */ IUri** uri);

    CARAPI ToString(
       /* [out] */ String* info);

private:
    Boolean HasSchemeOrAuthority();

private:
    String mScheme;
    AutoPtr<Uri::Part> mOpaquePart;
    AutoPtr<Uri::Part> mAuthority;
    AutoPtr<Uri::PathPart> mPath;
    AutoPtr<Uri::Part> mQuery;
    AutoPtr<Uri::Part> mFragment;
};

} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CURIBUILDER_H__
