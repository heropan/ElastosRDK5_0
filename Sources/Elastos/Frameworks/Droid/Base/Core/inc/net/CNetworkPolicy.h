
#ifndef __ELASTOS_DROID_NET_CNETWORKPOLICY_H__
#define __ELASTOS_DROID_NET_CNETWORKPOLICY_H__

#include "_CNetworkPolicy.h"
#include <elastos/Mutex.h>


namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CNetworkPolicy)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ INetworkTemplate* temp,
        /* [in] */ Int32 cycleDay,
        /* [in] */ const String& cycleTimezone,
        /* [in] */ Int64 warningBytes,
        /* [in] */ Int64 limitBytes,
        /* [in] */ Boolean metered);

    CARAPI constructor(
        /* [in] */ INetworkTemplate* temp,
        /* [in] */ Int32 cycleDay,
        /* [in] */ const String& cycleTimezone,
        /* [in] */ Int64 warningBytes,
        /* [in] */ Int64 limitBytes,
        /* [in] */ Int64 lastWarningSnooze,
        /* [in] */ Int64 lastLimitSnooze,
        /* [in] */ Boolean metered,
        /* [in] */ Boolean inferred);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI IsOverWarning(
        /* [in] */ Int64 totalBytes,
        /* [out] */ Boolean* result);

    CARAPI IsOverLimit(
        /* [in] */ Int64 totalBytes,
        /* [out] */ Boolean* result);

    CARAPI ClearSnooze();

    CARAPI HasCycle(
        /* [out] */ Boolean* result);

    CARAPI CompareTo(
        /* [in] */ INetworkPolicy* another,
        /* [out] */ Int32* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* result);

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* result);

    CARAPI EqualsEx(
        /* [in] */ INetworkPolicy* obj,
        /* [out] */ Boolean* result);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI GetTemplate(
        /* [out] */ INetworkTemplate** result);

    CARAPI GetCycleDay(
        /* [out] */ Int32* cycleday);

    CARAPI SetCycleDay(
        /* [in] */ Int32 cycleday);

    CARAPI GetCycleTimezone(
        /* [out] */ String* cycleTimezone);

    CARAPI SetCycleTimezone(
        /* [in] */ const String& cycleTimezone);

    CARAPI GetWarningBytes(
        /* [out] */ Int64* warningbytes);

    CARAPI SetWarningBytes(
        /* [in] */ Int64 warningbytes);

    CARAPI GetLimitBytes(
        /* [out] */ Int64* limitbytes);

    CARAPI SetLimitBytes(
        /* [in] */ Int64 limitbytes);

    CARAPI GetLastWarningSnooze(
        /* [out] */ Int64* lastwarningsnooze);

    CARAPI SetLastWarningSnooze(
        /* [in] */ Int64 lastwarningsnooze);

    CARAPI GetLastLimitSnooze(
        /* [out] */ Int64* lastlimitsnooze);

    CARAPI SetLastLimitSnooze(
        /* [in] */ Int64 lastlimitsnooze);

    CARAPI GetMetered(
        /* [out] */ Boolean* metered);

    CARAPI SetMetered(
        /* [in] */ Boolean metered);

    CARAPI GetInferred(
        /* [out] */ Boolean* inferred);

    CARAPI SetInferred(
        /* [in] */ Boolean inferred);

public:

    AutoPtr<INetworkTemplate> mTemplate;
    Int32 mCycleDay;
    String mCycleTimezone;
    Int64 mWarningBytes;
    Int64 mLimitBytes;
    Int64 mLastWarningSnooze;
    Int64 mLastLimitSnooze;
    Boolean mMetered;
    Boolean mInferred;

private:
    static const Int64 DEFAULT_MTU = 1500;

    /** Cache for the hash code. */
    Int32 mHashCode;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CNETWORKPOLICY_H__

