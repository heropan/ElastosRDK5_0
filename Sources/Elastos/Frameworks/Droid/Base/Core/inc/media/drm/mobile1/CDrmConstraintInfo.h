
#ifndef __ELASTOS_DROID_DRM_MOBILE1_CDRMCONSTRAINTINFO_H__
#define __ELASTOS_DROID_DRM_MOBILE1_CDRMCONSTRAINTINFO_H__

#include "_CDrmConstraintInfo.h"
#include <ext/frameworkext.h>

using Elastos::Utility::IDate;

namespace Elastos {
namespace Droid {
namespace Drm {
namespace Mobile1 {

/**
 * This class provides interfaces to access the DRM constraint.
 */
CarClass(CDrmConstraintInfo)
{
public:
    CDrmConstraintInfo();

    /**
     * Get the count constraint.
     *
     * @return the count or -1 if no limit.
     */
    CARAPI GetCount(
        /* [out] */ Int32* count);

    /**
     * Get the start date constraint.
     *
     * @return the start date or null if no limit.
     */
    CARAPI GetStartDate(
        /* [out] */ IDate** startDate);

    /**
     * Get the end date constraint.
     *
     * @return the end date or null if no limit.
     */
    CARAPI GetEndDate(
        /* [out] */ IDate** endDate);

    /**
     * Get the Interval constraint.
     *
     * @return the interval or -1 if no limit.
     */
    CARAPI GetInterval(
        /* [out] */ Int64* interval);

public:
    /**
     * The constraint of count.
     */
    Int32 mCount;

    /**
     * The constraint of start date.
     */
    Int64 mStartDate;

    /**
     * The constraint of end date.
     */
    Int64 mEndDate;

    /**
     * The constraint of interval.
     */
    Int64 mInterval;
};

} // namespace mobile1
} // namespace drm
} // namepsace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_DRM_MOBILE1_CDRMCONSTRAINTINFO_H__

