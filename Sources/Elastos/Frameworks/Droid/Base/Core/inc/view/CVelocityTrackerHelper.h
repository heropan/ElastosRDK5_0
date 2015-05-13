#ifndef __CVELOCITYTRACKERHELPER_H__
#define __CVELOCITYTRACKERHELPER_H__

#include "_CVelocityTrackerHelper.h"

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CVelocityTrackerHelper)
{
public:
    CARAPI Obtain(
        /* [out] */ IVelocityTracker** velocity);

    CARAPI ObtainEx(
        /* [in] */ const String& strategy,
        /* [out] */ IVelocityTracker** velocity);
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__CVELOCITYTRACKERHELPER_H__