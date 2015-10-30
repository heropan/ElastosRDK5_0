
#ifndef __ELASTOS_DROID_WEBKIT_CWEBRESOURCERESPONSE_H__
#define __ELASTOS_DROID_WEBKIT_CWEBRESOURCERESPONSE_H__

#include "_Elastos_Droid_Webkit_CWebResourceResponse.h"
#include "elastos/droid/webkit/WebResourceResponse.h"

using Elastos::IO::IInputStream;

namespace Elastos {
namespace Droid {
namespace Webkit {

struct ElaWebResourceResponseCallback
{
    String (*getMimeType)(IInterface* obj);
    String (*getEncoding)(IInterface* obj);
    AutoPtr<IInterface> (*getData)(IInterface* obj);
};

CarClass(CWebResourceResponse), public WebResourceResponse
{
public:
    CAR_OBJECT_DECL();

    CARAPI SetMimeType(
        /* [in] */ const String& mimeType);

    CARAPI GetMimeType(
        /* [out] */ String* mimeType);

    CARAPI SetEncoding(
        /* [in] */ const String& encoding);

    CARAPI GetEncoding(
        /* [out] */ String* encoding);

    CARAPI SetStatusCodeAndReasonPhrase(
        /* [in] */ Int32 statusCode,
        /* [in] */ const String& reasonPhrase);

    CARAPI GetStatusCode(
        /* [out] */ Int32* statusCode);

    CARAPI GetReasonPhrase(
        /* [out] */ String* reasonPhrase);

    CARAPI SetResponseHeaders(
        /* [in] */ IMap* headers);

    CARAPI GetResponseHeaders(
        /* [out] */ IMap** headers);

    CARAPI SetData(
        /* [in] */ IInputStream* data);

    CARAPI GetData(
        /* [out] */ IInputStream** inputStream);

    static String GetMimeType(
        /* [in] */ IInterface* obj);

    static String GetEncoding(
        /* [in] */ IInterface* obj);

    static AutoPtr<IInterface> GetData(
        /* [in] */ IInterface* obj);
public:
    static struct ElaWebResourceResponseCallback sWebResourceResponseCallback;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CWEBRESOURCERESPONSE_H__
