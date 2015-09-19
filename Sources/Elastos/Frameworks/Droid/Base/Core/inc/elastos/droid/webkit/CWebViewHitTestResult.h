
#ifndef __ELASTOS_DROID_WEBKIT_CWEBVIEWHITTESTRESULT_H__
#define __ELASTOS_DROID_WEBKIT_CWEBVIEWHITTESTRESULT_H__

#include "_Elastos_Droid_Webkit_CWebViewHitTestResult.h"
#include "webkit/WebView.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CWebViewHitTestResult)
{
public:
    /**
     * @hide Only for use by WebViewProvider implementations
     */
    CWebViewHitTestResult();

    /**
     * @hide Only for use by WebViewProvider implementations
     */
    CARAPI SetType(
        /* [in] */ Int32 type);

    /**
     * @hide Only for use by WebViewProvider implementations
     */
    CARAPI SetExtra(
        /* [in] */ const String& extra);

    /**
     * Gets the type of the hit test result. See the XXX_TYPE constants
     * defined in this class.
     *
     * @return the type of the hit test result
     */
    CARAPI GetType(
        /* [out] */ Int32* type);

    /**
     * Gets additional type-dependant information about the result. See
     * {@link WebView#getHitTestResult()} for details. May either be null
     * or contain extra information about this result.
     *
     * @return additional type-dependant information about the result
     */
    CARAPI GetExtra(
        /* [out] */ String* extra);

private:
    Int32 mType;
    String mExtra;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CWEBVIEWHITTESTRESULT_H__
