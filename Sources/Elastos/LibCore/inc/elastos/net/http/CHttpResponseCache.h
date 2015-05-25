
#ifndef __CHTTPRESPONSECACHE_H__
#define __CHTTPRESPONSECACHE_H__

#include "_CHttpResponseCache.h"
#include "cmdef.h"
#include "Elastos.CoreLibrary_server.h"
#include "elastos/Mutex.h"
#include "IoUtils.h"

using Elastos::IO::IFile;
using Elastos::IO::IOutputStream;
using Elastos::IO::IInputStream;
using Elastos::IO::IoUtils;
using Elastos::Utility::IMap;
using Elastos::Core::Mutex;
using Elastos::Security::IPrincipal;

namespace Elastos {
namespace Net {
namespace Http {

CarClass(CHttpResponseCache)
{
private:
    class CacheRequestImpl
        : public ElRefBase
        , public ICacheRequest
    {
    public:
        // CacheRequestImpl(
        //     /* [in] */ DiskLruCache.Editor editor) {
        //     this.editor = editor;
        //     this.cacheOut = editor.newOutputStream(ENTRY_BODY);
        //     this.body = new FilterOutputStream(cacheOut) {
        //         @Override public void close() throws IOException {
        //             synchronized (HttpResponseCache.this) {
        //                 if (done) {
        //                     return;
        //                 }
        //                 done = true;
        //                 writeSuccessCount++;
        //             }
        //             super.close();
        //             editor.commit();
        //         }

        //         @Override
        //         public void write(byte[] buffer, int offset, int length) throws IOException {
        //             // Since we don't override "write(int oneByte)", we can write directly to "out"
        //             // and avoid the inefficient implementation from the FilterOutputStream.
        //             out.write(buffer, offset, length);
        //         }
        //     };
        // }

        CAR_INTERFACE_DECL()

        CARAPI Abort() {
            {
                Mutex::Autolock lock(mLock);
                if (mDone) {
                    return NOERROR;
                }
                mDone = TRUE;
                // mWriteAbortCount++;
            }
            IoUtils::CloseQuietly(mCacheOut);
            assert(0 && "TODO");
            // try {
            //     editor.abort();
            // } catch (IOException ignored) {
            // }
            return NOERROR;
        }

        CARAPI GetBody(
            /* [out] */ IOutputStream** os) {
            VALIDATE_NOT_NULL(os)
            *os = mBody;
            INTERFACE_ADDREF(*os)

            return NOERROR;
        }

    private:
        // DiskLruCache.Editor editor;
        AutoPtr<IOutputStream> mCacheOut;
        Boolean mDone;
        AutoPtr<IOutputStream> mBody;
        Mutex mLock;
    };

    class Entry {
    /*
     * Reads an entry from an input stream. A typical entry looks like this:
     *   http://google.com/foo
     *   GET
     *   2
     *   Accept-Language: fr-CA
     *   Accept-Charset: UTF-8
     *   HTTP/1.1 200 OK
     *   3
     *   Content-Type: image/png
     *   Content-Length: 100
     *   Cache-Control: max-age=600
     *
     * A typical HTTPS file looks like this:
     *   https://google.com/foo
     *   GET
     *   2
     *   Accept-Language: fr-CA
     *   Accept-Charset: UTF-8
     *   HTTP/1.1 200 OK
     *   3
     *   Content-Type: image/png
     *   Content-Length: 100
     *   Cache-Control: max-age=600
     *
     *   AES_256_WITH_MD5
     *   2
     *   base64-encoded peerCertificate[0]
     *   base64-encoded peerCertificate[1]
     *   -1
     *
     * The file is newline separated. The first two lines are the URL and
     * the request method. Next is the number of HTTP Vary request header
     * lines, followed by those lines.
     *
     * Next is the response status line, followed by the number of HTTP
     * response header lines, followed by those lines.
     *
     * HTTPS responses also contain SSL session information. This begins
     * with a blank line, and then a line containing the cipher suite. Next
     * is the length of the peer certificate chain. These certificates are
     * base64-encoded and appear each on their own line. The next line
     * contains the length of the local certificate chain. These
     * certificates are also base64-encoded and appear each on their own
     * line. A length of -1 is used to encode a null array.
     */
    public:
        Entry(
            /* [in] */ IInputStream* in) {
            assert(0 && "TODO");
            // try {
            //     StrictLineReader reader = new StrictLineReader(in, Charsets.US_ASCII);
            //     uri = reader.readLine();
            //     requestMethod = reader.readLine();
            //     varyHeaders = new RawHeaders();
            //     int varyRequestHeaderLineCount = reader.readInt();
            //     for (int i = 0; i < varyRequestHeaderLineCount; i++) {
            //         varyHeaders.addLine(reader.readLine());
            //     }

            //     responseHeaders = new RawHeaders();
            //     responseHeaders.setStatusLine(reader.readLine());
            //     int responseHeaderLineCount = reader.readInt();
            //     for (int i = 0; i < responseHeaderLineCount; i++) {
            //         responseHeaders.addLine(reader.readLine());
            //     }

            //     if (isHttps()) {
            //         String blank = reader.readLine();
            //         if (!blank.isEmpty()) {
            //             throw new IOException("expected \"\" but was \"" + blank + "\"");
            //         }
            //         cipherSuite = reader.readLine();
            //         peerCertificates = readCertArray(reader);
            //         localCertificates = readCertArray(reader);
            //     } else {
            //         cipherSuite = null;
            //         peerCertificates = null;
            //         localCertificates = null;
            //     }
            // } finally {
            //     in.close();
            // }
        }

        Entry(
            /* [in] */ IURI* uri,
            /* [in] */ IRawHeaders* varyHeaders,
            /* [in] */ IHttpURLConnection* httpConnection) {
            uri->ToString(&mUri);
            mVaryHeaders = varyHeaders;
            httpConnection->GetRequestMethod(&mRequestMethod);
            assert(0 && "TODO");
            // mResponseHeaders = RawHeaders.fromMultimap(httpConnection.getHeaderFields());

            if (IsHttps()) {
                // HttpsURLConnection httpsConnection = (HttpsURLConnection) httpConnection;
                // cipherSuite = httpsConnection.getCipherSuite();
                // Certificate[] peerCertificatesNonFinal = null;
                // try {
                //     peerCertificatesNonFinal = httpsConnection.getServerCertificates();
                // } catch (SSLPeerUnverifiedException ignored) {
                // }
                // peerCertificates = peerCertificatesNonFinal;
                // localCertificates = httpsConnection.getLocalCertificates();
            } else {
                // cipherSuite = null;
                // peerCertificates = null;
                // localCertificates = null;
            }
        }

        // void WriteTo(
        //     /* [in] */ DiskLruCache.Editor editor) {
        //     OutputStream out = editor.newOutputStream(ENTRY_METADATA);
        //     Writer writer = new BufferedWriter(new OutputStreamWriter(out, Charsets.UTF_8));

        //     writer.write(uri + '\n');
        //     writer.write(requestMethod + '\n');
        //     writer.write(Integer.toString(varyHeaders.length()) + '\n');
        //     for (int i = 0; i < varyHeaders.length(); i++) {
        //         writer.write(varyHeaders.getFieldName(i) + ": "
        //                 + varyHeaders.getValue(i) + '\n');
        //     }

        //     writer.write(responseHeaders.getStatusLine() + '\n');
        //     writer.write(Integer.toString(responseHeaders.length()) + '\n');
        //     for (int i = 0; i < responseHeaders.length(); i++) {
        //         writer.write(responseHeaders.getFieldName(i) + ": "
        //                 + responseHeaders.getValue(i) + '\n');
        //     }

        //     if (isHttps()) {
        //         writer.write('\n');
        //         writer.write(cipherSuite + '\n');
        //         writeCertArray(writer, peerCertificates);
        //         writeCertArray(writer, localCertificates);
        //     }
        //     writer.close();
        // }

    private:
        Boolean IsHttps() {
            return mUri.StartWith("https://");
        }

        // Certificate[] readCertArray(StrictLineReader reader) {
        //     int length = reader.readInt();
        //     if (length == -1) {
        //         return null;
        //     }
        //     try {
        //         CertificateFactory certificateFactory = CertificateFactory.getInstance("X.509");
        //         Certificate[] result = new Certificate[length];
        //         for (int i = 0; i < result.length; i++) {
        //             String line = reader.readLine();
        //             byte[] bytes = Base64.decode(line.getBytes(Charsets.US_ASCII));
        //             result[i] = certificateFactory.generateCertificate(
        //                     new ByteArrayInputStream(bytes));
        //         }
        //         return result;
        //     } catch (CertificateException e) {
        //         throw new IOException(e);
        //     }
        // }

        // void writeCertArray(Writer writer, Certificate[] certificates) {
        //     if (certificates == null) {
        //         writer.write("-1\n");
        //         return;
        //     }
        //     try {
        //         writer.write(Integer.toString(certificates.length) + '\n');
        //         for (Certificate certificate : certificates) {
        //             byte[] bytes = certificate.getEncoded();
        //             String line = Base64.encode(bytes);
        //             writer.write(line + '\n');
        //         }
        //     } catch (CertificateEncodingException e) {
        //         throw new IOException(e);
        //     }
        // }

    public:
        Boolean Matches(
            /* [in] */ IURI* uri,
            /* [in] */ const String& requestMethod,
            /* [in] */ IMap* requestHeaders) {
            String outstr;
            assert(0 && "TODO");
            // return (mUri->ToString(&outstr), mUri.Equals(outstr))
            //         && mRequestMethod.Equals(requestMethod)
            //         && new ResponseHeaders(uri, responseHeaders)
            //                 .varyMatches(varyHeaders.toMultimap(), requestHeaders);
            return NOERROR;
        }

    public:
        String mUri;
        AutoPtr<IRawHeaders> mVaryHeaders;
        String mRequestMethod;
        AutoPtr<IRawHeaders> mResponseHeaders;
        String mCipherSuite;
        // Certificate[] peerCertificates;
        // Certificate[] localCertificates;
    };

protected:
    class EntryCacheResponse
        : public ElRefBase
        , public ICacheResponse
    {
    public:
        // EntryCacheResponse(
        //     /* [in] */ Entry* entry,
        //     /* [in] */ DiskLruCache.Snapshot snapshot) {
        //     this.entry = entry;
        //     this.snapshot = snapshot;
        //     this.in = newBodyInputStream(snapshot);
        // }

        CARAPI GetHeaders(
            /* [out] */ IMap** headers) {
            VALIDATE_NOT_NULL(headers)

            AutoPtr<IMap> obj;
            mEntry->mResponseHeaders->ToMultimap((IMap**)&obj);
            *headers = obj;
            INTERFACE_ADDREF(*headers)
            return NOERROR;
        }

        CARAPI GetBody(
            /* [out] */ IInputStream** is) {
            VALIDATE_NOT_NULL(is)

            *is = mIn;
            INTERFACE_ADDREF(*is)
            return NOERROR;
        }

    private:
        Entry* mEntry;
        // DiskLruCache.Snapshot snapshot;
        AutoPtr<IInputStream> mIn;
    };

    class EntrySecureCacheResponse
        : public ElRefBase
        , public ISecureCacheResponse
    {
    public:
        // EntrySecureCacheResponse(
        //     /* [in] */ Entry* entry,
        //     /* [in] */ DiskLruCache.Snapshot snapshot) {
        //     mEntry = entry;
        //     this.snapshot = snapshot;
        //     mIn = newBodyInputStream(snapshot);
        // }

        CARAPI GetHeaders(
            /* [out] */ IMap** headers) {
            VALIDATE_NOT_NULL(headers)

            AutoPtr<IMap> obj;
            mEntry->mResponseHeaders->ToMultimap((IMap**)&obj);
            *headers = obj;
            INTERFACE_ADDREF(*headers)
            return NOERROR;
        }

        CARAPI GetBody(
            /* [out] */ IInputStream** is) {
            VALIDATE_NOT_NULL(is)

            *is = mIn;
            INTERFACE_ADDREF(*is)
            return NOERROR;
        }

        CARAPI GetCipherSuite(
            /* [out] */ String* clipherSuite) {
            VALIDATE_NOT_NULL(clipherSuite)

            *clipherSuite = mEntry->mCipherSuite;
            return NOERROR;
        }

        CARAPI GetServerCertificateChain(
            /* [out] */ IObjectContainer** certificateChain) {
            assert(0 && "TODO");
            // if (entry.peerCertificates == null || entry.peerCertificates.length == 0) {
            //     throw new SSLPeerUnverifiedException(null);
            // }
            // return Arrays.asList(entry.peerCertificates.clone());
            return NOERROR;
        }

        CARAPI GetPeerPrincipal(
            /* [out] */ IPrincipal** principal) {
            assert(0 && "TODO");
            // if (entry.peerCertificates == null || entry.peerCertificates.length == 0) {
            //     throw new SSLPeerUnverifiedException(null);
            // }
            // return ((X509Certificate) entry.peerCertificates[0]).getSubjectX500Principal();
            return NOERROR;
        }

        CARAPI GetLocalCertificateChain(
            /* [out] */ IObjectContainer** certificateChain) {
            assert(0 && "TODO");
            // if (entry.localCertificates == null || entry.localCertificates.length == 0) {
            //     return null;
            // }
            // return Arrays.asList(entry.localCertificates.clone());
            return NOERROR;
        }

        CARAPI GetLocalPrincipal(
            /* [out] */ IPrincipal** principal) {
            assert(0 && "TODO");
            // if (entry.localCertificates == null || entry.localCertificates.length == 0) {
            //     return null;
            // }
            // return ((X509Certificate) entry.localCertificates[0]).getSubjectX500Principal();
            return NOERROR;
        }

    private:
        Entry* mEntry;
        // DiskLruCache.Snapshot snapshot;
        AutoPtr<IInputStream> mIn;
    };

public:
    CARAPI constructor(
        /* [in] */ IFile* directory,
        /* [in] */ Int64 maxSize);

    // public DiskLruCache getCache();

    CARAPI GetWriteAbortCount(
        /* [out] */ Int32* value);

    CARAPI GetWriteSuccessCount(
        /* [out] */ Int32* value);

    CARAPI GetNetworkCount(
        /* [out] */ Int32* value);

    CARAPI GetHitCount(
        /* [out] */ Int32* value);

    CARAPI GetRequestCount(
        /* [out] */ Int32* value);

    CARAPI Get(
        /* [in] */ IURI* uri,
        /* [in] */ const String& requestMethod,
        /* [in] */ IMap* requestHeaders,
        /* [out] */ ICacheResponse** cache);

    CARAPI Put(
        /* [in] */ IURI* uri,
        /* [in] */ IURLConnection* conn,
        /* [out] */ ICacheRequest** cache);

    CARAPI TrackResponse(
        /* [in] */ ResponseSource source);

    CARAPI TrackConditionalCacheHit();

    CARAPI Update(
        /* [in] */ ICacheResponse* conditionalCacheHit,
        /* [in] */ IHttpURLConnection* httpConnection);

private:
    CARAPI_(String) UriToKey(
        /* [in] */ IURI* uri);

    // void abortQuietly(DiskLruCache.Editor editor);

    CARAPI_(AutoPtr<IHttpEngine>) GetHttpEngine(
        /* [in] */ IHttpURLConnection* httpConnection);

private:
    // static InputStream newBodyInputStream(final DiskLruCache.Snapshot snapshot)

private:
    static const Int32 VERSION;
    static const Int32 ENTRY_METADATA;
    static const Int32 ENTRY_BODY;
    static const Int32 ENTRY_COUNT;

    // private final DiskLruCache cache;

    /* read and write statistics, all guarded by 'this' */
    Int32 mWriteSuccessCount;
    Int32 mWriteAbortCount;
    Int32 mNetworkCount;
    Int32 mHitCount;
    Int32 mRequestCount;

    Mutex mPlock;
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__CHTTPRESPONSECACHE_H__
