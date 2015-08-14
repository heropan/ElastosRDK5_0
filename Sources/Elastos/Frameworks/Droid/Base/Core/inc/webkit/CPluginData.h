
#ifndef __ELASTOS_DROID_WEBKIT_CPLUGINDATA_H__
#define __ELASTOS_DROID_WEBKIT_CPLUGINDATA_H__

#include "_CPluginData.h"

using Elastos::IO::IInputStream;
using Elastos::Utility::IMap;

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CPluginData)
{
public:
    CARAPI constructor(
        /* [in] */ IInputStream* stream,
        /* [in] */ Int64 length,
        /* [in] */ IMap* headers,
        /* [in] */ Int32 code);

    CARAPI GetInputStream(
        /* [out] */ IInputStream** stream);

    CARAPI GetContentLength(
        /* [out] */ Int64* length);

    CARAPI GetHeaders(
        /* [out] */ IMap** headers);

    CARAPI GetStatusCode(
        /* [out] */ Int32* code);

private:
    /**
     * The content stream.
     */
    AutoPtr<IInputStream> mStream;

    /**
     * The content length.
     */
    Int64 mContentLength;

    /**
     * The associated HTTP response headers stored as a map of
     * lowercase header name to [ unmodified header name, header value].
     * TODO: This design was always a hack. Remove (involves updating
     * the Gears C++ side).
     */
    AutoPtr<IMap> mHeaders;

    /**
     * The associated HTTP response code.
     */
    Int32 mStatusCode;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CPLUGINDATA_H__
