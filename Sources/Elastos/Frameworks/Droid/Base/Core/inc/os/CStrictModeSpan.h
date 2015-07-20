#ifndef __CSTRICTMODESPAN_H__
#define __CSTRICTMODESPAN_H__

#include "_CStrictModeSpan.h"
#include "ext/frameworkdef.h"
#include <elastos/utility/etl/List.h>
#include <elastos/Core/Object.h>
#include "os/CStrictMode.h"

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CStrictModeSpan)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Handle32 threadState);

    CARAPI Finish();

private:
    friend class CStrictMode;
    friend class CStrictModeViolationInfo;

    String mName;
    Int64 mCreateMillis;
    AutoPtr<CStrictModeSpan> mNext;
    AutoPtr<CStrictModeSpan> mPrev;  // not used when in freeList, only active
    AutoPtr<CStrictMode::ThreadSpanState> mContainerState;

    Object mContainerStateLock;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__CSTRICTMODESPAN_H__