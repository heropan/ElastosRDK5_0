
#ifndef __CJSPROMPTRESULT_H__
#define __CJSPROMPTRESULT_H__

#include "_CJsPromptResult.h"
#include "webkit/JsPromptResult.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CJsPromptResult), JsPromptResult
{
public:
    CARAPI constructor(
        /* [in] */ IJsResultReceiver* receiver);

    CARAPI Cancel();

    CARAPI Confirm();

    CARAPI GetResult(
        /* [out] */ Boolean* result);

    CARAPI ConfirmEx(
        /* [in] */ const String& result);

    CARAPI GetStringResult(
        /* [out] */ String* stringResult);

protected:
    CARAPI_(void) WakeUp();
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __CJSPROMPTRESULT_H__
