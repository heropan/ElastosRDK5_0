
#include "content/CSyncStorageEngineDayStats.h"
#include <ext/frameworkext.h>

namespace Elastos {
namespace Droid {
namespace Content {

CSyncStorageEngineDayStats::CSyncStorageEngineDayStats()
    : mDay(0)
    , mSuccessCount(0)
    , mSuccessTime(0)
    , mFailureCount(0)
    , mFailureTime(0)
{}

CSyncStorageEngineDayStats::~CSyncStorageEngineDayStats()
{}

ECode CSyncStorageEngineDayStats::GetDay(
    /* [out] */ Int32* day)
{
    VALIDATE_NOT_NULL(day)
    *day = mDay;
    return NOERROR;
}

ECode CSyncStorageEngineDayStats::GetSuccessCount(
    /* [out] */ Int32* successCount)
{
    VALIDATE_NOT_NULL(successCount)
    *successCount = mSuccessCount;
    return NOERROR;
}

ECode CSyncStorageEngineDayStats::SetSuccessCount(
    /* [in] */ Int32 successCount)
{
    mSuccessCount = successCount;
    return NOERROR;
}

ECode CSyncStorageEngineDayStats::GetSuccessTime(
    /* [out] */ Int64* successTime)
{
    VALIDATE_NOT_NULL(successTime)
    *successTime = mSuccessTime;
    return NOERROR;
}

ECode CSyncStorageEngineDayStats::SetSuccessTime(
    /* [in] */ Int64 successTime)
{
    mSuccessTime = successTime;
    return NOERROR;
}

ECode CSyncStorageEngineDayStats::GetFailureCount(
    /* [out] */ Int32* failureCount)
{
    VALIDATE_NOT_NULL(failureCount)
    *failureCount = mFailureCount;
    return NOERROR;
}

ECode CSyncStorageEngineDayStats::SetFailureCount(
    /* [in] */ Int32 failureCount)
{
    mFailureCount = failureCount;
    return NOERROR;
}

ECode CSyncStorageEngineDayStats::GetFailureTime(
    /* [out] */ Int64* failureTime)
{
    VALIDATE_NOT_NULL(failureTime)
    *failureTime = mFailureTime;
    return NOERROR;
}

ECode CSyncStorageEngineDayStats::SetFailureTime(
    /* [in] */ Int64 failureTime)
{
    mFailureTime = failureTime;
    return NOERROR;
}

ECode CSyncStorageEngineDayStats::constructor(
    /* [in] */ Int32 day)
{
    mDay = day;
    return NOERROR;
}

}
}
}

