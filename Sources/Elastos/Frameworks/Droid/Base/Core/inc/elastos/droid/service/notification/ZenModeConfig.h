#ifndef __ELASTOS_DROID_SERVICE_NOTIFICATION_ZENMODECONFIG_H__
#define __ELASTOS_DROID_SERVICE_NOTIFICATION_ZENMODECONFIG_H__

#include "Elastos.Droid.Content.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Net::IUri;
using Org::Xmlpull::V1::IXmlPullParser;
using Org::Xmlpull::V1::IXmlSerializer;

namespace Elastos {
namespace Droid {
namespace Service {
namespace Notification {

/**
 * Persisted configuration for zen mode.
 *
 * @hide
 */
class ZenModeConfig
    : public Object
    , public IZenModeConfig
    , public IParcelable
{
public:
    class DowntimeInfo
        : public Object
        , public IZenModeConfigDowntimeInfo
    {
    public:
        CAR_INTERFACE_DECL()

        DowntimeInfo();

        CARAPI constructor();

        // @Override
        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

        // @Override
        CARAPI Equals(
            /* [in] */ IInterface* o,
            /* [out] */ Boolean* result);

    public:
        Int32 mStartHour;
        Int32 mStartMinute;
        Int32 mEndHour;
        Int32 mEndMinute;
    };

public:
    CAR_INTERFACE_DECL()

    ZenModeConfig();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcelable* source);

    // @Override
    CARAPI ReadFromParcel(
        /* [in] */ IParcel* in);

    // @Override
    CARAPI WriteToParcel(
        /* [in] */ IParcel* out);

    // @Override
    CARAPI ToString(
        /* [out] */ String* result);

    static CARAPI_(String) SourceToString(
        /* [in] */ Int32 source);

    // @Override
    CARAPI Equals(
        /* [in] */ IInterface* o,
        /* [out] */ Boolean* result);

    // @Override
    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI IsValid(
        /* [out] */ Boolean* result);

    static CARAPI_(Boolean) IsValidSleepMode(
        /* [in] */ String sleepMode);

    static CARAPI TryParseDays(
        /* [in] */ String sleepMode,
        /* [out, callee] */ ArrayOf<Int32>** results);

    static CARAPI ReadXml(
        /* [in] */ IXmlPullParser* parser,
        /* [out] */ IZenModeConfig** result);

    CARAPI WriteXml(
        /* [in] */ IXmlSerializer* out);

    static CARAPI ReadConditionXml(
        /* [in] */ IXmlPullParser* parser,
        /* [out] */ ICondition** result);

    static CARAPI WriteConditionXml(
        /* [in] */ ICondition* c,
        /* [in] */ IXmlSerializer* out);

    static CARAPI_(Boolean) IsValidHour(
        /* [in] */ Int32 val);

    static CARAPI_(Boolean) IsValidMinute(
        /* [in] */ Int32 val);

    CARAPI Copy(
        /* [out] */ IZenModeConfig** copy);

    CARAPI ToDowntimeInfo(
        /* [out] */ IZenModeConfigDowntimeInfo** result);

    static CARAPI ToTimeCondition(
        /* [in] */ Int32 minutesFromNow,
        /* [out] */ ICondition** result);

    static CARAPI ToTimeCondition(
        /* [in] */ Int64 time,
        /* [in] */ Int32 minutes,
        /* [out] */ ICondition** result);

    static CARAPI ToCountdownConditionId(
        /* [in] */ Int64 time,
        /* [out] */ IUri** result);

    static CARAPI TryParseCountdownConditionId(
        /* [in] */ IUri* conditionId,
        /* [out] */ Int64* result);

    static CARAPI IsValidCountdownConditionId(
        /* [in] */ IUri* conditionId,
        /* [out] */ Boolean* result);

    static CARAPI ToDowntimeConditionId(
        /* [in] */ IZenModeConfigDowntimeInfo* downtime,
        /* [out] */ IUri** result);

    static CARAPI TryParseDowntimeConditionId(
        /* [in] */ IUri* conditionId,
        /* [out] */ IZenModeConfigDowntimeInfo** result);

    static CARAPI_(Boolean) IsValidDowntimeConditionId(
        /* [in] */ IUri* conditionId);

private:
    static CARAPI_(Int32) TryParseInt32(
        /* [in] */ String value,
        /* [in] */ Int32 defValue);

    static CARAPI_(Boolean) SafeBoolean(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ const String& att,
        /* [in] */ Boolean defValue);

    static CARAPI_(Int32) SafeInt(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ const String& att,
        /* [in] */ Int32 defValue);

    static CARAPI_(AutoPtr<IComponentName>) SafeComponentName(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ const String& att);

    static CARAPI_(AutoPtr<IUri>) SafeUri(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ const String& att);

    static CARAPI TryParseHourAndMinute(
        /* [in] */ String value,
        /* [out, callee] */ ArrayOf<Int32>** results);

    static CARAPI GetALL_DAYS(
        /* [out, callee] */ ArrayOf<Int32>** days);

    static CARAPI GetWEEKNIGHT_DAYS(
        /* [out, callee] */ ArrayOf<Int32>** days);

    static CARAPI GetMINUTE_BUCKETS(
        /* [out, callee] */ ArrayOf<Int32>** buckets);

public:
    const static AutoPtr<ArrayOf<Int32> > ALL_DAYS;
    const static AutoPtr<ArrayOf<Int32> > WEEKNIGHT_DAYS;
    const static AutoPtr<ArrayOf<Int32> > MINUTE_BUCKETS;

    Boolean mAllowCalls;
    Boolean mAllowMessages;
    Boolean mAllowEvents;
    Int32 mAllowFrom;

    String mSleepMode;
    Int32 mSleepStartHour;
    Int32 mSleepStartMinute;
    Int32 mSleepEndHour;
    Int32 mSleepEndMinute;
    AutoPtr<ArrayOf<IComponentName*> > mConditionComponents;
    AutoPtr<ArrayOf<IUri*> > mConditionIds;
    AutoPtr<ICondition> mExitCondition;
    AutoPtr<IComponentName> mExitConditionComponent;

private:
    const static String TAG;

    const static Int32 SECONDS_MS;
    const static Int32 MINUTES_MS;
    const static Int32 ZERO_VALUE_MS;

    const static Boolean DEFAULT_ALLOW_EVENTS;

    const static Int32 XML_VERSION;
    const static String ZEN_TAG;
    const static String ZEN_ATT_VERSION;
    const static String ALLOW_TAG;
    const static String ALLOW_ATT_CALLS;
    const static String ALLOW_ATT_MESSAGES;
    const static String ALLOW_ATT_FROM;
    const static String ALLOW_ATT_EVENTS;
    const static String SLEEP_TAG;
    const static String SLEEP_ATT_MODE;

    const static String SLEEP_ATT_START_HR;
    const static String SLEEP_ATT_START_MIN;
    const static String SLEEP_ATT_END_HR;
    const static String SLEEP_ATT_END_MIN;

    const static String CONDITION_TAG;
    const static String CONDITION_ATT_COMPONENT;
    const static String CONDITION_ATT_ID;
    const static String CONDITION_ATT_SUMMARY;
    const static String CONDITION_ATT_LINE1;
    const static String CONDITION_ATT_LINE2;
    const static String CONDITION_ATT_ICON;
    const static String CONDITION_ATT_STATE;
    const static String CONDITION_ATT_FLAGS;

    const static String EXIT_CONDITION_TAG;
    const static String EXIT_CONDITION_ATT_COMPONENT;

    // For built-in conditions
    const static String SYSTEM_AUTHORITY;

    // Built-in countdown conditions, e.g. condition://android/countdown/1399917958951
    const static String COUNTDOWN_PATH;

    // Built-in downtime conditions, e.g. condition://android/downtime?start=10.00&end=7.00
    const static String DOWNTIME_PATH;

};

} // namespace Notification
} // namespace Service
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVICE_NOTIFICATION_ZENMODECONFIG_H__
