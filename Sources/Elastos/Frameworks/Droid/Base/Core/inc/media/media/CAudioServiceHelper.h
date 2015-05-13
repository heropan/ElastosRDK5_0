
#ifndef __CAUDIOSERVICEHELPER_H__
#define __CAUDIOSERVICEHELPER_H__

#include "_CAudioServiceHelper.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CAudioServiceHelper)
{
public:
    CAudioServiceHelper();

    ~CAudioServiceHelper();

    /**
     * @see #setVibrateSetting(int, int);
     */
    CARAPI GetValueForVibrateSetting(
        /* [in] */ Int32 existingValue,
        /* [in] */ Int32 vibrateType,
        /* [in] */ Int32 vibrateSetting,
        /* [out] */ Int32* result);
};

} // namespace Media
} // namespace Droid
} // namespace Elastos

#endif // __CAUDIOSERVICEHELPER_H__
