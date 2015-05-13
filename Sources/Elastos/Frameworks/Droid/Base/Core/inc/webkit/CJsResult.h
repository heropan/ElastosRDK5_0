
#ifndef __CJSRESULT_H__
#define __CJSRESULT_H__

#include "_CJsResult.h"
#include "webkit/JsResult.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CJsResult), public JsResult
{
public:
    CARAPI constructor(
        /* [in] */ IJsResultReceiver* receiver);

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

#endif // __CJSRESULT_H__
