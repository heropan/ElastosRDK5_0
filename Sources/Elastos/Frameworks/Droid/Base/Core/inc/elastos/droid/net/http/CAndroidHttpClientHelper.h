
#ifndef __ELASTOS_DROID_NET_HTTP_CANDROIDHTTPCLIENTHELPER_H__
#define __ELASTOS_DROID_NET_HTTP_CANDROIDHTTPCLIENTHELPER_H__

#include "_Elastos_Droid_Net_Http_CAndroidHttpClientHelper.h"

using Org::Apache::Http::Entity::IAbstractHttpEntity;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CarClass(CAndroidHttpClientHelper)
{
public:
    /**
     * Create a new HttpClient with reasonable defaults (which you can update).
     *
     * @param userAgent to report in your HTTP requests
     * @param context to use for caching SSL sessions (may be null for no caching)
     * @return AndroidHttpClient for you to use for all your requests.
     */
    CARAPI NewInstance(
        /* [in] */ const String& userAgent,
        /* [in] */ Elastos::Droid::Content::IContext* context,
        /* [out] */ Elastos::Droid::Net::Http::IAndroidHttpClient** client);

    /**
     * Create a new HttpClient with reasonable defaults (which you can update).
     * @param userAgent to report in your HTTP requests.
     * @return AndroidHttpClient for you to use for all your requests.
     */
    CARAPI NewInstance(
        /* [in] */ const String& userAgent,
        /* [out] */ Elastos::Droid::Net::Http::IAndroidHttpClient** client);

    /**
     * Modifies a request to indicate to the server that we would like a
     * gzipped response.  (Uses the "Accept-Encoding" HTTP header.)
     * @param request the request to modify
     * @see #getUngzippedContent
     */
    CARAPI ModifyRequestToAcceptGzipResponse(
        /* [in, out] */ Org::Apache::Http::IHttpRequest* request);

    /**
     * Gets the input stream from a response entity.  If the entity is gzipped
     * then this will get a stream over the uncompressed data.
     *
     * @param entity the entity whose content should be read
     * @return the input stream to read from
     * @throws IOException
     */
    CARAPI GetUngzippedContent(
        /* [in] */ Org::Apache::Http::IHttpEntity* entity,
        /* [out] */ Elastos::IO::IInputStream** stream);

    /**
     * Compress data to send to server.
     * Creates a Http Entity holding the gzipped data.
     * The data will not be compressed if it is too short.
     * @param data The bytes to compress
     * @return Entity holding the data
     */
    CARAPI GetCompressedEntity(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Elastos::Droid::Content::IContentResolver* resolver,
        /* [out] */ Org::Apache::Http::Entity::IAbstractHttpEntity** entity);

    /**
     * Retrieves the minimum size for compressing data.
     * Shorter data will not be compressed.
     */
    CARAPI GetMinGzipSize(
        /* [in] */ Elastos::Droid::Content::IContentResolver* resolver,
        /* [out] */ Int64* size);

    /**
     * Returns the date of the given HTTP date string. This method can identify
     * and parse the date formats emitted by common HTTP servers, such as
     * <a href="http://www.ietf.org/rfc/rfc0822.txt">RFC 822</a>,
     * <a href="http://www.ietf.org/rfc/rfc0850.txt">RFC 850</a>,
     * <a href="http://www.ietf.org/rfc/rfc1036.txt">RFC 1036</a>,
     * <a href="http://www.ietf.org/rfc/rfc1123.txt">RFC 1123</a> and
     * <a href="http://www.opengroup.org/onlinepubs/007908799/xsh/asctime.html">ANSI
     * C's asctime()</a>.
     *
     * @return the number of milliseconds since Jan. 1, 1970, midnight GMT.
     * @throws IllegalArgumentException if {@code dateString} is not a date or
     *     of an unsupported format.
     */
    CARAPI ParseDate(
        /* [in] */ const String& dateString,
        /* [out] */ Int64* date);

public:
    // Gzip of data shorter than this probably won't be worthwhile
    static Int64 DEFAULT_SYNC_MIN_GZIP_BYTES;

};

}
}
}
}

#endif // __ELASTOS_DROID_NET_HTTP_CANDROIDHTTPCLIENTHELPER_H__
