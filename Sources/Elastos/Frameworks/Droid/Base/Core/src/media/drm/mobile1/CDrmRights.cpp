
#include "ext/frameworkext.h"
#include "media/drm/mobile1/CDrmRights.h"
#include "media/drm/mobile1/CDrmConstraintInfo.h"
#include <objmng/svc_drm.h>

#define MS_PER_SECOND 1000                  /* Milliseconds per second */
#define MS_PER_MINUTE 60 * MS_PER_SECOND    /* Milliseconds per minute */
#define MS_PER_HOUR   60 * MS_PER_MINUTE    /* Milliseconds per hour */
#define MS_PER_DAY    24 * MS_PER_HOUR      /* Milliseconds per day */
#define SECONDS_PER_MINUTE 60                       /* Seconds per minute*/
#define SECONDS_PER_HOUR   60 * SECONDS_PER_MINUTE  /* Seconds per hour */
#define SECONDS_PER_DAY    24 * SECONDS_PER_HOUR    /* Seconds per day */
#define DAY_PER_MONTH 30                    /* Days per month */
#define DAY_PER_YEAR  365                   /* Days per year */
/** Nonzero if 'y' is a leap year, else zero. */
#define leap(y) (((y) % 4 == 0 && (y) % 100 != 0) || (y) % 400 == 0)
/** Number of leap years from 1970 to 'y' (not including 'y' itself). */
#define nleap(y) (((y) - 1969) / 4 - ((y) - 1901) / 100 + ((y) - 1601) / 400)
/** Accumulated number of days from 01-Jan up to start of current month. */
static const int32_t ydays[] = {
    0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
};
#define int64_const(s)          (s)
#define int64_add(dst, s1, s2)  ((void)((dst) = (s1) + (s2)))
#define int64_mul(dst, s1, s2)  ((void)((dst) = (int64_t)(s1) * (int64_t)(s2)))

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

namespace Elastos {
namespace Droid {
namespace Drm {
namespace Mobile1 {

const Int32 CDrmRights::JNI_DRM_SUCCESS;
const Int32 CDrmRights::JNI_DRM_FAILURE;

CDrmRights::CDrmRights()
    : mRoId("")
{}

ECode CDrmRights::GetConstraint(
    /* [in] */ Int32 permission,
    /* [out] */ IDrmConstraintInfo** constraint)
{
    VALIDATE_NOT_NULL(constraint);

    AutoPtr<CDrmConstraintInfo> c;
    FAIL_RETURN(CDrmConstraintInfo::NewByFriend((CDrmConstraintInfo**)&c));

    /* call native method to get latest constraint information */
    Int32 res = NativeGetConstraintInfo(permission, c);

    if (JNI_DRM_FAILURE == res){
        *constraint = NULL;
        return NOERROR;
    }

    *constraint = (IDrmConstraintInfo*)c.Get();
    INTERFACE_ADDREF(*constraint);

    return NOERROR;
}

ECode CDrmRights::ConsumeRights(
    /* [in] */ Int32 permission,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    /* call native method to consume and update rights */
    Int32 res = NativeConsumeRights(permission);

    if (JNI_DRM_FAILURE == res) {
        *result = FALSE;
        return NOERROR;
    }

    *result = TRUE;
    return NOERROR;
}

ECode CDrmRights::GetRoId(
    /* [out] */ String* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mRoId;
    return NOERROR;
}

static const T_DRM_Rights_Info_Node* SearchRightsObject(
    /* [in] */ const String& roId,
    /* [in] */ const T_DRM_Rights_Info_Node* rightsList)
{
    const T_DRM_Rights_Info_Node* tmp;

    if (roId.IsNull() || NULL == rightsList) {
        return NULL;
    }

    tmp = rightsList;

    while (NULL != tmp) {
        if(0 == strcmp((const char *)roId, (char *)tmp->roInfo.roId)) {
            break;
        }
        tmp = tmp->next;
    }

    return tmp;
}

/**
 * Returns the difference in seconds between the given GMT time
 * and 1970-01-01 00:00:00 GMT.
 *
 * \param year the year (since 1970)
 * \param month the month (1 - 12)
 * \param day the day (1 - 31)
 * \param hour the hour (0 - 23)
 * \param minute the minute (0 - 59)
 * \param second the second (0 - 59)
 *
 * \return the difference in seconds between the given GMT time
 *         and 1970-01-01 00:00:00 GMT.
 */
static int64_t Mkgmtime(
        /* [in] */ uint32_t year,
        /* [in] */ uint32_t month,
        /* [in] */ uint32_t day,
        /* [in] */ uint32_t hour,
        /* [in] */ uint32_t minute,
        /* [in] */ uint32_t second)
{
    Int64 result;

    /*
     * FIXME: It does not check whether the specified days
     *        is valid based on the specified months.
     */
    assert(year >= 1970
            && month > 0 && month <= 12
            && day > 0 && day <= 31
            && hour < 24 && minute < 60
            && second < 60);

    /* Set 'day' to the number of days into the year. */
    day += ydays[month - 1] + (month > 2 && leap (year)) - 1;

    /* Now calculate 'day' to the number of days since Jan 1, 1970. */
    day = day + 365 * (year - 1970) + nleap(year);

    int64_mul(result, int64_const(day), int64_const(SECONDS_PER_DAY));
    int64_add(result, result, int64_const(
        SECONDS_PER_HOUR * hour + SECONDS_PER_MINUTE * minute + second));

    return result;
}

/**
 * Compute the milliseconds by the specified <code>date</code>
 * and <code>time</code>.
 *
 * @param date - the specified date,
 *               <code>date = year * 10000 + month * 100 + day</code>
 * @param time - the specified time,
 *               <code>time = hour * 10000 + minute * 100 + second</code>
 *
 * @return the related milliseconds
 */
static int64_t ComputeTime(
    /* [in] */ int32_t date,
    /* [in] */ int32_t time)
{
    int32_t year, month, day, hour, minute, second;

    year = date / 10000;
    month = (date / 100) % 100;
    day = date % 100;
    hour = time / 10000;
    minute = (time / 100) % 100;
    second = time % 100;

    /* Adjust the invalid parameters. */
    if (year < 1970) year = 1970;
    if (month < 1) month = 1;
    if (month > 12) month = 12;
    if (day < 1) day = 1;
    if (day > 31) day = 31;
    if (hour < 0) hour = 0;
    if (hour > 23) hour = 23;
    if (minute < 0) minute = 0;
    if (minute > 59) minute = 59;
    if (second < 0) second = 0;
    if (second > 59) second = 59;

    return Mkgmtime(year, month, day, hour, minute, second) * 1000;
}

/**
 * Compute the milliseconds by the specified <code>date</code>
 * and <code>time</code>.
 * Note that here we always treat 1 year as 365 days and 1 month as 30 days
 * that is not precise. But it should not be a problem since OMA DRM 2.0
 * already restricts the interval representation to be day-based,
 * i.e. there will not be an interval with year or month any more in the
 * future.
 *
 * @param date - the specified date,
 *               <code>date = year * 10000 + month * 100 + day</code>
 * @param time - the specified time,
 *               <code>time = hour * 10000 + minute * 100 + second</code>
 *
 * @return the related milliseconds
 */
static int64_t ComputeInterval(
    /* [in] */ int32_t date,
    /* [in] */ int32_t time)
{
    int32_t year, month, day, hour, minute, second;
    int64_t milliseconds;

    year = date / 10000;
    month = (date / 100) % 100;
    day = date % 100;
    hour = time / 10000;
    minute = (time / 100) % 100;
    second = time % 100;

    /* milliseconds = ((((year * 365 + month * 30 + day) * 24
     *                + hour) * 60 + minute) * 60 + second) * 1000;
     */
    int64_mul(milliseconds,
        int64_const(year * DAY_PER_YEAR + month * DAY_PER_MONTH + day),
        int64_const(MS_PER_DAY));
    int64_add(milliseconds, milliseconds,
        int64_const(hour * MS_PER_HOUR + minute * MS_PER_MINUTE +
            second * MS_PER_SECOND));

    return milliseconds;
}

static Int32 SetConstraintFields(
    /* [in] */ CDrmConstraintInfo* _constraint,
    /* [in] */ T_DRM_Constraint_Info* constraint)
{
    /* if no this permission */
    if (constraint->indicator == (uint8_t)DRM_NO_RIGHTS) {
        _constraint->mCount = 0;
        return CDrmRights::JNI_DRM_SUCCESS;
    }

    /* set count field */
    if (constraint->indicator & DRM_COUNT_CONSTRAINT) {
        _constraint->mCount = constraint->count;
    }

    /* set start time field */
    if (constraint->indicator & DRM_START_TIME_CONSTRAINT) {
        Int64 startTime;

        startTime = ComputeTime(constraint->startDate, constraint->startTime);

        _constraint->mStartDate = startTime;
    }

    /* set end time field */
    if (constraint->indicator & DRM_END_TIME_CONSTRAINT) {
        Int64 endTime;

        endTime = ComputeTime(constraint->endDate, constraint->endTime);

        _constraint->mEndDate = endTime;
    }

    /* set interval field */
    if (constraint->indicator & DRM_INTERVAL_CONSTRAINT) {
        Int64 interval;

        interval = ComputeInterval(constraint->intervalDate, constraint->intervalTime);

        _constraint->mInterval = interval;
    }

    return CDrmRights::JNI_DRM_SUCCESS;
}

Int32 CDrmRights::NativeGetConstraintInfo(
    /* [in] */ Int32 permission,
    /* [in] */ CDrmConstraintInfo* _constraint)
{
    T_DRM_Rights_Info_Node* rightsList;
    T_DRM_Rights_Info_Node* curNode;
    T_DRM_Constraint_Info* constraint;

    if (mRoId.IsNull()) {
        return JNI_DRM_FAILURE;
    }

    /* this means forward-lock rights */
    if (mRoId.Equals("ForwardLock")) {
        return JNI_DRM_SUCCESS;
    }

    if (DRM_FAILURE == SVC_drm_viewAllRights(&rightsList)) {
        return JNI_DRM_FAILURE;
    }

    curNode = const_cast<T_DRM_Rights_Info_Node*>(SearchRightsObject(mRoId, rightsList));
    if (NULL == curNode) {
        SVC_drm_freeRightsInfoList(rightsList);
        return JNI_DRM_FAILURE;
    }

    switch (permission) {
    case IDrmRights::DRM_RIGHTS_PERMISSION_PLAY:
        constraint = &(curNode->roInfo.playRights);
        break;
    case IDrmRights::DRM_RIGHTS_PERMISSION_DISPLAY:
        constraint = &(curNode->roInfo.displayRights);
        break;
    case IDrmRights::DRM_RIGHTS_PERMISSION_EXECUTE:
        constraint = &(curNode->roInfo.executeRights);
        break;
    case IDrmRights::DRM_RIGHTS_PERMISSION_PRINT:
        constraint = &(curNode->roInfo.printRights);
        break;
    default:
        SVC_drm_freeRightsInfoList(rightsList);
        return JNI_DRM_FAILURE;
    }

    /* set constraint field */
    if (JNI_DRM_FAILURE == SetConstraintFields(_constraint, constraint)) {
        SVC_drm_freeRightsInfoList(rightsList);
        return JNI_DRM_FAILURE;
    }

    SVC_drm_freeRightsInfoList(rightsList);

    return JNI_DRM_SUCCESS;
}

Int32 CDrmRights::NativeConsumeRights(
    /* [in] */ Int32 permission)
{

    switch (permission) {
    case 1L:
        permission = IDrmRights::DRM_RIGHTS_PERMISSION_PLAY;
        break;
    case 2L:
        permission = IDrmRights::DRM_RIGHTS_PERMISSION_DISPLAY;
        break;
    case 3L:
        permission = IDrmRights::DRM_RIGHTS_PERMISSION_EXECUTE;
        break;
    case 4L:
        permission = IDrmRights::DRM_RIGHTS_PERMISSION_PRINT;
        break;
    default:
        return JNI_DRM_FAILURE;
    }

    if (mRoId.IsNull()) {
        return JNI_DRM_FAILURE;
    }
    if (mRoId.Equals("ForwardLock")) {
        return JNI_DRM_SUCCESS;
    }

    if (JNI_DRM_SUCCESS != SVC_drm_updateRights((uint8_t*)(mRoId.string()), permission)) {
       return JNI_DRM_FAILURE;
    }

    return JNI_DRM_SUCCESS;
}

} // namespace mobile1
} // namespace drm
} // namepsace Droid
} // namespace Elastos

