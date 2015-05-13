#ifndef __CACCESSIBILITYSERVICEINFOHELPER_H__
#define __CACCESSIBILITYSERVICEINFOHELPER_H__

#include "_CAccessibilityServiceInfoHelper.h"

namespace Elastos {
namespace Droid {
namespace AccessibilityService {

CarClass(CAccessibilityServiceInfoHelper)
{
public:
    /**
     * Returns the string representation of a feedback type. For example,
     * {@link #FEEDBACK_SPOKEN} is represented by the string FEEDBACK_SPOKEN.
     *
     * @param feedbackType The feedback type.
     * @return The string representation.
     */
    CARAPI FeedbackTypeToString(
        /* [in] */ Int32 feedbackType,
        /* [out] */ String* representation);

    /**
     * Returns the string representation of a flag. For example,
     * {@link #DEFAULT} is represented by the string DEFAULT.
     *
     * @param flag The flag.
     * @return The string representation.
     */
    CARAPI FlagToString(
        /* [in] */ Int32 flag,
        /* [out] */ String* representation);
};

}
}
}

#endif // __CACCESSIBILITYSERVICEINFOHELPER_H__
