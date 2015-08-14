
#ifndef __ELASTOS_DROID_WEBKIT_CWEBRESOURCERESPONSE_H__
#define __ELASTOS_DROID_WEBKIT_CWEBRESOURCERESPONSE_H__

#include "_CWebResourceResponse.h"
#include "webkit/WebResourceResponse.h"

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
    CARAPI constructor(
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding,
        /* [in] */ IInputStream* data);

    CARAPI SetMimeType(
        /* [in] */ const String& mimeType);

    CARAPI GetMimeType(
        /* [out] */ String* mimeType);

    CARAPI SetEncoding(
        /* [in] */ const String& encoding);

    CARAPI GetEncoding(
        /* [out] */ String* encoding);

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
