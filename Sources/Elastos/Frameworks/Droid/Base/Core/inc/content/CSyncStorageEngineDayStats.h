
#ifndef __CSYNCSTORAGEENGINEDAYSTATS_H__
#define __CSYNCSTORAGEENGINEDAYSTATS_H__

#include "_CSyncStorageEngineDayStats.h"

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CSyncStorageEngineDayStats)
{
public:
    CSyncStorageEngineDayStats();

    ~CSyncStorageEngineDayStats();

    CARAPI GetDay(
        /* [out] */ Int32* day);

    CARAPI GetSuccessCount(
        /* [out] */ Int32* successCount);

    CARAPI SetSuccessCount(
        /* [in] */ Int32 successCount);

    CARAPI GetSuccessTime(
        /* [out] */ Int64* successTime);

    CARAPI SetSuccessTime(
        /* [in] */ Int64 successTime);

    CARAPI GetFailureCount(
        /* [out] */ Int32* failureCount);

    CARAPI SetFailureCount(
        /* [in] */ Int32 failureCount);

    CARAPI GetFailureTime(
        /* [out] */ Int64* failureTime);

    CARAPI SetFailureTime(
        /* [in] */ Int64 failureTime);

    CARAPI constructor(
        /* [in] */ Int32 day);

private:
    Int32 mDay;
    Int32 mSuccessCount;
    Int64 mSuccessTime;
    Int32 mFailureCount;
    Int64 mFailureTime;

};

}
}
}

#endif // __CSYNCSTORAGEENGINEDAYSTATS_H__
