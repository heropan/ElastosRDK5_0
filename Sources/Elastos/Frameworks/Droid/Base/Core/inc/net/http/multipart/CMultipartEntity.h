
#ifndef __ELASTOS_DROID_NET_INTERNAL_HTTP_MULTIPART_CMULTIPARTENTITY_H__
#define __ELASTOS_DROID_NET_INTERNAL_HTTP_MULTIPART_CMULTIPARTENTITY_H__

#include "_CMultipartEntity.h"
#include "elautoptr.h"

using Org::Apache::Commons::Logging::ILog;
using Org::Apache::Http::Params::IHttpParams;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Internal {
namespace Http {
namespace Multipart {

/**
 * Implements a request entity suitable for an HTTP multipart POST method.
 * <p>
 * The HTTP multipart POST method is defined in section 3.3 of
 * <a href="http://www.ietf.org/rfc/rfc1867.txt">RFC1867</a>:
 * <blockquote>
 * The media-type multipart/form-data follows the rules of all multipart
 * MIME data streams as outlined in RFC 1521. The multipart/form-data contains
 * a series of parts. Each part is expected to contain a content-disposition
 * header where the value is "form-data" and a name attribute specifies
 * the field name within the form, e.g., 'content-disposition: form-data;
 * name="xxxxx"', where xxxxx is the field name corresponding to that field.
 * Field names originally in non-ASCII character sets may be encoded using
 * the method outlined in RFC 1522.
 * </blockquote>
 * </p>
 * <p>This entity is designed to be used in conjunction with the
 * {@link org.apache.http.HttpRequest} to provide
 * multipart posts.  Example usage:</p>
 * <pre>
 *  File f = new File("/path/fileToUpload.txt");
 *  HttpRequest request = new HttpRequest("http://host/some_path");
 *  Part[] parts = {
 *      new StringPart("param_name", "value"),
 *      new FilePart(f.getName(), f)
 *  };
 *  filePost.setEntity(
 *      new MultipartRequestEntity(parts, filePost.getParams())
 *      );
 *  HttpClient client = new HttpClient();
 *  int status = client.executeMethod(filePost);
 * </pre>
 *
 * @since 3.0
 */
CarClass(CMultipartEntity)/*, public AbstractHttpEntity*/
{
public:
    CMultipartEntity();

    ~CMultipartEntity();

    /**
     * Creates a new multipart entity containing the given parts.
     * @param parts The parts to include.
     * @param params The params of the HttpMethod using this entity.
     */
    CARAPI constructor(
        /* [in] */ ArrayOf<IPart *>* parts,
        /* [in] */ Org::Apache::Http::Params::IHttpParams* params);

    CARAPI constructor(
        /* [in] */ ArrayOf<IPart *>* parts);

    /**
     * Returns <code>true</code> if all parts are repeatable, <code>false</code> otherwise.
     */
    CARAPI IsRepeatable(
        /* [out] */ Boolean* isRepeatable);

    /* (non-Javadoc)
     */
    CARAPI WriteTo(
        /* [in] */ Elastos::IO::IOutputStream* outStream);

    /* (non-Javadoc)
     * @see org.apache.commons.http.AbstractHttpEntity.#getContentType()
     */
    CARAPI GetContentType(
        /* [out] */ Org::Apache::Http::IHeader** header);

    /* (non-Javadoc)
     */
    CARAPI GetContentLength(
        /* [out] */ Int64* length);

    CARAPI GetContent(
        /* [out] */ Elastos::IO::IInputStream** stream);

    CARAPI IsStreaming(
        /* [out] */ Boolean* isStreaming);

    CARAPI GetContentEncoding(
        /* [out] */ Org::Apache::Http::IHeader** header);

    CARAPI IsChunked(
        /* [out] */ Boolean* isChunked);

    CARAPI SetContentType(
        /* [in] */ Org::Apache::Http::IHeader* contentType);

    CARAPI SetContentType(
        /* [in] */ const String& ctString);

    CARAPI SetContentEncoding(
        /* [in] */ Org::Apache::Http::IHeader* contentEncoding);

    CARAPI SetContentEncoding(
        /* [in] */ const String& ceString);

    CARAPI SetChunked(
        /* [in] */ Boolean chunked);

    CARAPI ConsumeContent();

protected:
    /**
     * Returns the MIME boundary string that is used to demarcate boundaries of
     * this part. The first call to this method will implicitly create a new
     * boundary string. To create a boundary string first the
     * HttpMethodParams.MULTIPART_BOUNDARY parameter is considered. Otherwise
     * a random one is generated.
     *
     * @return The boundary string of this entity in ASCII encoding.
     */
    CARAPI_(AutoPtr<ArrayOf<Byte> >) GetMultipartBoundary();

private:
    CARAPI_(void) InitStaticLOG();

    /**
     * Generates a random multipart boundary string.
    */
    static CARAPI_(AutoPtr<ArrayOf<Byte> >) GenerateMultipartBoundary();

protected:
    /** The MIME parts as set by the constructor */
    AutoPtr<ArrayOf<IPart*> > mParts;

private:
    /** Log object for this class. */
    static const AutoPtr<ILog> LOG;

    /** The Content-Type for multipart/form-data. */
    static const String MULTIPART_FORM_CONTENT_TYPE;

    /**
     * The pool of ASCII chars to be used for generating a multipart boundary.
     */
    static AutoPtr<ArrayOf<Byte> > MULTIPART_CHARS;

    AutoPtr<ArrayOf<Byte> > mMultipartBoundary;

    AutoPtr<IHttpParams> mParams;

    Boolean mContentConsumed;
};

}
}
}
}
}
}

#endif // __ELASTOS_DROID_NET_INTERNAL_HTTP_MULTIPART_CMULTIPARTENTITY_H__
