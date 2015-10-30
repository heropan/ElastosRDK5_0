
#ifndef __ELASTOS_DROID_WEBKIT_CJSPROMPTRESULT_H__
#define __ELASTOS_DROID_WEBKIT_CJSPROMPTRESULT_H__

#include "_Elastos_Droid_Webkit_CJsPromptResult.h"
#include "elastos/droid/webkit/JsPromptResult.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CJsPromptResult)
    , public JsPromptResult
{
public:
    CARAPI Cancel();

    CARAPI Confirm();

    CARAPI GetResult(
        /* [out] */ Boolean* result);

    CARAPI Confirm(
        /* [in] */ const String& result);

    CARAPI GetStringResult(
        /* [out] */ String* stringResult);

protected:
    CARAPI_(void) WakeUp();
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CJSPROMPTRESULT_H__
