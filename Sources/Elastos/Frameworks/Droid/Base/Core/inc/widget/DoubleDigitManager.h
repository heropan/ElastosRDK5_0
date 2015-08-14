
#ifndef __ELASTOS_DROID_WIDGET_DOUBLEDIGITMANAGER_H__
#define __ELASTOS_DROID_WIDGET_DOUBLEDIGITMANAGER_H__

#include "ext/frameworkext.h"
#include "os/Runnable.h"

using Elastos::Core::IInteger32;
using Elastos::Droid::Os::Runnable;

namespace Elastos {
namespace Droid {
namespace Widget {

class DoubleDigitManager
{
private:
    class DoubleDigitManagerRunnable
        : public Runnable
    {
    public:
        DoubleDigitManagerRunnable(
            /* [in] */ DoubleDigitManager* host);

        CARAPI Run();

    private:
        DoubleDigitManager* mHost;
    };

public:
    DoubleDigitManager(
        /* [in] */ Int64 timeoutInMillis,
        /* [in] */ IDoubleDigitManagerCallBack* callBack);

    CARAPI ReportDigit(
            /* [in] */ Int32 digit);

protected:
    DoubleDigitManager();

    CARAPI Init(
        /* [in] */ Int64 timeoutInMillis,
        /* [in] */ IDoubleDigitManagerCallBack* callBack);

private:
    Int64 mTimeoutInMillins;

    AutoPtr<IDoubleDigitManagerCallBack> mCallBack;

    AutoPtr<IInteger32> mIntermediateDigit;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif
