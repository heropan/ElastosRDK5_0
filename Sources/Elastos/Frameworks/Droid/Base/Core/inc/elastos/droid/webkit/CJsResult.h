
#ifndef __ELASTOS_DROID_WEBKIT_CJSRESULT_H__
#define __ELASTOS_DROID_WEBKIT_CJSRESULT_H__

#include "_Elastos_Droid_Webkit_CJsResult.h"
#include "elastos/droid/webkit/JsResult.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CJsResult), public JsResult
{
public:
    CAR_OBJECT_DECL();

    CARAPI Cancel();

    CARAPI Confirm();

    CARAPI GetResult(
        /* [out] */ Boolean* result);

protected:
    /* Notify the caller that the JsResult has completed */
    CARAPI_(void) WakeUp();
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CJSRESULT_H__
