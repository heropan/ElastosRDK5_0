
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
