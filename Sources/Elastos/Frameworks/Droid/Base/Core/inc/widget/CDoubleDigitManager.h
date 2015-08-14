#ifndef __ELASTOS_DROID_WIDGET_CDOUBLEDIGITMANAGER_H__
#define __ELASTOS_DROID_WIDGET_CDOUBLEDIGITMANAGER_H__

#include "_CDoubleDigitManager.h"
#include "widget/DoubleDigitManager.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CDoubleDigitManager), public DoubleDigitManager
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ Int64 timeoutInMillis,
        /* [in] */ IDoubleDigitManagerCallBack* callBack);

    CARAPI ReportDigit(
        /* [in] */ Int32 digit);

};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CDOUBLEDIGITMANAGER_H__
