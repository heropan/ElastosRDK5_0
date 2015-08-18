
#ifndef __ELASTOS_DROID_APP_CINSTRUMENTATIONACTIVITYRESULT_H__
#define __ELASTOS_DROID_APP_CINSTRUMENTATIONACTIVITYRESULT_H__

#include "_Elastos_Droid_App_CInstrumentationActivityResult.h"

using Elastos::Droid::Content::IIntent;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CInstrumentationActivityResult)
{
public:
    CInstrumentationActivityResult();

    /**
     * Create a new activity result.  See {@link Activity#setResult} for 
     * more information. 
     *  
     * @param resultCode The result code to propagate back to the
     * originating activity, often RESULT_CANCELED or RESULT_OK
     * @param resultData The data to propagate back to the originating
     * activity.
     */
    CARAPI constructor(
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* resultData);

    /**
     * Retrieve the result code contained in this result.
     */
    CARAPI GetResultCode(
        /* [out] */ Int32* resultCode);

    /**
     * Retrieve the data contained in this result.
     */
    CARAPI GetResultData(
        /* [out] */ IIntent** resultData);

private:
    Int32 mResultCode;
    AutoPtr<IIntent> mResultData;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_CINSTRUMENTATIONACTIVITYRESULT_H__
