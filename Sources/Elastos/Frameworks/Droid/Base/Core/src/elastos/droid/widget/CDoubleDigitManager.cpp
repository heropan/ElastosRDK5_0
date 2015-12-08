#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/CDoubleDigitManager.h"

namespace Elastos {
namespace Droid {
namespace Widget {

PInterface CDoubleDigitManager::Probe(
    /* [in] */ REIID riid)
{
    return _CDoubleDigitManager::Probe(riid);
}

ECode CDoubleDigitManager::constructor(
    /* [in] */ Int64 timeoutInMillis,
    /* [in] */ IDoubleDigitManagerCallBack* callBack)
{
    return DoubleDigitManager::Init(timeoutInMillis, callBack);
}

ECode CDoubleDigitManager::ReportDigit(
    /* [in] */ Int32 digit)
{
    return DoubleDigitManager::ReportDigit(digit);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
