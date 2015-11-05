
#ifndef __ELASTOS_DROID_NET_HTTP_CHTTPRESPONSECACHEHELPER_H__
#define __ELASTOS_DROID_NET_HTTP_CHTTPRESPONSECACHEHELPER_H__

#include "_Elastos_Droid_Net_Http_CHttpResponseCacheHelper.h"
#include <elastos/core/Singleton.h>

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * Caches HTTP and HTTPS responses to the filesystem so they may be reused,
 * saving time and bandwidth. This class supports {@link HttpURLConnection} and
 * {@link HttpsURLConnection}; there is no platform-provided cache for {@link
 * DefaultHttpClient} or {@link AndroidHttpClient}.
 *
 * <h3>Installing an HTTP response cache</h3>
 * Enable caching of all of your application's HTTP requests by installing the
 * cache at application startup. For example, this code installs a 10 MiB cache
 * in the {@link Context#getCacheDir() application-specific cache directory} of
 * the filesystem}: <pre>   {@code
 *   protected void onCreate(Bundle savedInstanceState) {
 *       ...
 *
 *       try {
 *           File httpCacheDir = new File(context.getCacheDir(), "http");
 *           long httpCacheSize = 10 * 1024 * 1024; // 10 MiB
 *           HttpResponseCache.install(httpCacheDir, httpCacheSize);
 *       } catch (IOException e) {
 *           Log.i(TAG, "HTTP response cache installation failed:" + e);
 *       }
 *   }
 *
 *   protected void onStop() {
 *       ...
 *
 *       HttpResponseCache cache = HttpResponseCache.getInstalled();
 *       if (cache != null) {
 *           cache.flush();
 *       }
 *   }}</pre>
 * This cache will evict entries as necessary to keep its size from exceeding
 * 10 MiB. The best cache size is application specific and depends on the size
 * and frequency of the files being downloaded. Increasing the limit may improve
 * the hit rate, but it may also just waste filesystem space!
 *
 * <p>For some applications it may be preferable to create the cache in the
 * external storage directory. <strong>There are no access controls on the
 * external storage directory so it should not be used for caches that could
 * contain private data.</strong> Although it often has more free space,
 * external storage is optional and&#8212;even if available&#8212;can disappear
 * during use. Retrieve the external cache directory using {@link
 * Context#getExternalCacheDir()}. If this method returns null, your application
 * should fall back to either not caching or caching on non-external storage. If
 * the external storage is removed during use, the cache hit rate will drop to
 * zero and ongoing cache reads will fail.
 *
 * <p>Flushing the cache forces its data to the filesystem. This ensures that
 * all responses written to the cache will be readable the next time the
 * activity starts.
 *
 * <h3>Cache Optimization</h3>
 * To measure cache effectiveness, this class tracks three statistics:
 * <ul>
 *     <li><strong>{@link #getRequestCount() Request Count:}</strong> the number
 *         of HTTP requests issued since this cache was created.
 *     <li><strong>{@link #getNetworkCount() Network Count:}</strong> the
 *         number of those requests that required network use.
 *     <li><strong>{@link #getHitCount() Hit Count:}</strong> the number of
 *         those requests whose responses were served by the cache.
 * </ul>
 * Sometimes a request will result in a conditional cache hit. If the cache
 * contains a stale copy of the response, the client will issue a conditional
 * {@code GET}. The server will then send either the updated response if it has
 * changed, or a short 'not modified' response if the client's copy is still
 * valid. Such responses increment both the network count and hit count.
 *
 * <p>The best way to improve the cache hit rate is by configuring the web
 * server to return cacheable responses. Although this client honors all <a
 * href="http://www.ietf.org/rfc/rfc2616.txt">HTTP/1.1 (RFC 2068)</a> cache
 * headers, it doesn't cache partial responses.
 *
 * <h3>Force a Network Response</h3>
 * In some situations, such as after a user clicks a 'refresh' button, it may be
 * necessary to skip the cache, and fetch data directly from the server. To force
 * a full refresh, add the {@code no-cache} directive: <pre>   {@code
 *         connection.addRequestProperty("Cache-Control", "no-cache");
 * }</pre>
 * If it is only necessary to force a cached response to be validated by the
 * server, use the more efficient {@code max-age=0} instead: <pre>   {@code
 *         connection.addRequestProperty("Cache-Control", "max-age=0");
 * }</pre>
 *
 * <h3>Force a Cache Response</h3>
 * Sometimes you'll want to show resources if they are available immediately,
 * but not otherwise. This can be used so your application can show
 * <i>something</i> while waiting for the latest data to be downloaded. To
 * restrict a request to locally-cached resources, add the {@code
 * only-if-cached} directive: <pre>   {@code
 *     try {
 *         connection.addRequestProperty("Cache-Control", "only-if-cached");
 *         InputStream cached = connection.getInputStream();
 *         // the resource was cached! show it
 *     } catch (FileNotFoundException e) {
 *         // the resource was not cached
 *     }
 * }</pre>
 * This technique works even better in situations where a stale response is
 * better than no response. To permit stale cached responses, use the {@code
 * max-stale} directive with the maximum staleness in seconds: <pre>   {@code
 *         int maxStale = 60 * 60 * 24 * 28; // tolerate 4-weeks stale
 *         connection.addRequestProperty("Cache-Control", "max-stale=" + maxStale);
 * }</pre>
 *
 * <h3>Working With Earlier Releases</h3>
 * This class was added in Android 4.0 (Ice Cream Sandwich). Use reflection to
 * enable the response cache without impacting earlier releases: <pre>   {@code
 *       try {
 *           File httpCacheDir = new File(context.getCacheDir(), "http");
 *           long httpCacheSize = 10 * 1024 * 1024; // 10 MiB
 *           Class.forName("android.net.http.HttpResponseCache")
 *                   .getMethod("install", File.class, long.class)
 *                   .invoke(null, httpCacheDir, httpCacheSize);
 *       } catch (Exception httpResponseCacheNotAvailable) {
 *       }}</pre>
 */
CarClass(CHttpResponseCacheHelper)
    , public Singleton
    , public IHttpResponseCacheHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    /**
     * Returns the currently-installed {@code HttpResponseCache}, or null if
     * there is no cache installed or it is not a {@code HttpResponseCache}.
     */
    CARAPI GetInstalled(
        /* [out] */ IHttpResponseCache** result);

    /**
     * Creates a new HTTP response cache and {@link ResponseCache#setDefault
     * sets it} as the system default cache.
     *
     * @param directory the directory to hold cache data.
     * @param maxSize the maximum size of the cache in bytes.
     * @return the newly-installed cache
     * @throws IOException if {@code directory} cannot be used for this cache.
     *     Most applications should respond to this exception by logging a
     *     warning.
     */
    CARAPI Install(
        /* [in] */ IFile* directory,
        /* [in] */ Int64 maxSize,
        /* [out] */ IHttpResponseCache** result);
};

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif //  __ELASTOS_DROID_NET_HTTP_CHTTPRESPONSECACHEHELPER_H__
