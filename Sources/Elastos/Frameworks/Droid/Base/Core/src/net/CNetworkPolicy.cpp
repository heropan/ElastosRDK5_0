
#include "ext/frameworkext.h"
#include "net/CNetworkPolicy.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Net {

ECode CNetworkPolicy::constructor()
{
    return NOERROR;
}

ECode CNetworkPolicy::constructor(
    /* [in] */ INetworkTemplate* temp,
    /* [in] */ Int32 cycleDay,
    /* [in] */ const String& cycleTimezone,
    /* [in] */ Int64 warningBytes,
    /* [in] */ Int64 limitBytes,
    /* [in] */ Boolean metered)
{
    constructor(temp, cycleDay, cycleTimezone, warningBytes, limitBytes, SNOOZE_NEVER, SNOOZE_NEVER, metered, FALSE);
    return NOERROR;
}

ECode CNetworkPolicy::constructor(
    /* [in] */ INetworkTemplate* temp,
    /* [in] */ Int32 cycleDay,
    /* [in] */ const String& cycleTimezone,
    /* [in] */ Int64 warningBytes,
    /* [in] */ Int64 limitBytes,
    /* [in] */ Int64 lastWarningSnooze,
    /* [in] */ Int64 lastLimitSnooze,
    /* [in] */ Boolean metered,
    /* [in] */ Boolean inferred)
{
    mTemplate = temp; //checkNotNull(temp, "missing NetworkTemplate");
    mCycleDay = cycleDay;
    mCycleTimezone = cycleTimezone; //checkNotNull(cycleTimezone, "missing cycleTimezone");
    mWarningBytes = warningBytes;
    mLimitBytes = limitBytes;
    mLastWarningSnooze = lastWarningSnooze;
    mLastLimitSnooze = lastLimitSnooze;
    mMetered = metered;
    mInferred = inferred;
    return NOERROR;
}


/**
 * Test if given measurement is over {@link #warningBytes}.
 */
ECode CNetworkPolicy::IsOverWarning(
    /* [in] */ Int64 totalBytes,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = ((mWarningBytes != WARNING_DISABLED) && (totalBytes >= mWarningBytes));
    return NOERROR;
}

/**
 * Test if given measurement is near enough to {@link #limitBytes} to be
 * considered over-limit.
 */
ECode CNetworkPolicy::IsOverLimit(
    /* [in] */ Int64 totalBytes,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // over-estimate, since kernel will trigger limit once first packet
    // trips over limit.
    totalBytes += 2 * DEFAULT_MTU;
    *result = ((mLimitBytes != LIMIT_DISABLED) && (totalBytes >= mLimitBytes));
    return NOERROR;
}

/**
 * Clear any existing snooze values, setting to {@link #SNOOZE_NEVER}.
 */
ECode CNetworkPolicy::ClearSnooze()
{
    mLastWarningSnooze = SNOOZE_NEVER;
    mLastLimitSnooze = SNOOZE_NEVER;
    return NOERROR;
}

/**
 * Test if this policy has a cycle defined, after which usage should reset.
 */
ECode CNetworkPolicy::HasCycle(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (mCycleDay != CYCLE_NONE);
    return NOERROR;
}

//@Override
ECode CNetworkPolicy::CompareTo(
    /* [in] */ INetworkPolicy* another,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    Int64 limitbytes;
    another->GetLimitBytes(&limitbytes);
    if (another == NULL || limitbytes == LIMIT_DISABLED) {
        // other value is missing or disabled; we win
        *result = -1;
        return NOERROR;
    }
    if (mLimitBytes == LIMIT_DISABLED || limitbytes < mLimitBytes) {
        // we're disabled or other limit is smaller; they win
        *result = 1;
        return NOERROR;
    }
    *result = 0;
    return NOERROR;
}

//@Override
ECode CNetworkPolicy::GetHashCode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 hashCode = mHashCode;
    if (mHashCode != 0) {
        return NOERROR;
    }
    hashCode = 1;
    Int32 tempHashCode;
    if (mTemplate = NULL) tempHashCode = 0;
    else mTemplate->GetHashCode(&tempHashCode);

    hashCode = 31 * hashCode + tempHashCode;
    hashCode = 31 * hashCode + mCycleDay;
    hashCode = 31 * hashCode + mCycleTimezone.GetHashCode();
    hashCode = 31 * hashCode + (Int32) (mWarningBytes ^ (mWarningBytes >> 32));
    hashCode = 31 * hashCode + (Int32) (mLimitBytes ^ (mLimitBytes >> 32));
    hashCode = 31 * hashCode + (Int32) (mLastWarningSnooze ^ (mLastWarningSnooze >> 32));
    hashCode = 31 * hashCode + (Int32) (mLastLimitSnooze ^ (mLastLimitSnooze >> 32));
    hashCode = 31 * hashCode + (mMetered ? 1231 : 1237);
    hashCode = 31 * hashCode + (mInferred ? 1231 : 1237);
    *result = hashCode;
    return NOERROR;
}

//@Override
ECode CNetworkPolicy::Equals(
    /* [in] */ INetworkPolicy* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (INetworkPolicy::Probe(obj) != NULL) {
        AutoPtr<INetworkPolicy> other = obj;
        AutoPtr<INetworkTemplate> temp;
        other->GetTemplate((INetworkTemplate**)&temp);
        Int32 cycleDay;
        other->GetCycleDay(&cycleDay);
        String cycleTimezone;
        other->GetCycleTimezone(&cycleTimezone);
        Int64 warningBytes;
        other->GetWarningBytes(&warningBytes);
        Int64 limitBytes;
        other->GetLimitBytes(&limitBytes);
        Int64 lastWarningSnooze;
        other->GetLastWarningSnooze(&lastWarningSnooze);
        Int64 lastLimitSnooze;
        other->GetLastLimitSnooze(&lastLimitSnooze);
        Boolean metered;
        other->GetMetered(&metered);
        Boolean inferred;
        other->GetInferred(&inferred);

        Boolean bol;
        mTemplate->Equals(temp, &bol);

        *result = (mCycleDay == cycleDay) &&
                  (mWarningBytes == warningBytes) &&
                  (mLimitBytes == limitBytes) &&
                  (mLastWarningSnooze == lastWarningSnooze) &&
                  (mLastLimitSnooze == lastLimitSnooze) &&
                  (mMetered == metered) &&
                  (mInferred == inferred) &&
                  (mCycleTimezone.Equals(cycleTimezone)) &&
                  bol;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode CNetworkPolicy::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result )
{
    VALIDATE_NOT_NULL(result);
    return Equals(INetworkPolicy::Probe(obj), result);
}

//@Override
ECode CNetworkPolicy::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    StringBuilder builder("NetworkPolicy");
    builder += String("[");
    String temp;
    mTemplate->ToString(&temp);
    builder += temp;
    builder += String("]:");
    builder += String(" cycleDay=");
    builder += StringUtils::ToString(mCycleDay);
    builder += String(", cycleTimezone=");
    builder += mCycleTimezone;
    builder += String(", warningBytes=");
    builder += StringUtils::ToString(mWarningBytes);
    builder += String(", limitBytes=");
    builder += StringUtils::ToString(mLimitBytes);
    builder += String(", lastWarningSnooze=");
    builder += StringUtils::ToString(mLastWarningSnooze);
    builder += String(", lastLimitSnooze=");
    builder += StringUtils::ToString(mLastLimitSnooze);
    builder += String(", metered=");
    builder += StringUtils::BooleanToString(mMetered);
    builder += String(", inferred=");
    builder += StringUtils::BooleanToString(mInferred);
    *result = builder.ToString();
    return NOERROR;
}

//@Override
ECode CNetworkPolicy::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);
    dest->WriteInterfacePtr(mTemplate);
    dest->WriteInt32(mCycleDay);
    dest->WriteString(mCycleTimezone);
    dest->WriteInt64(mWarningBytes);
    dest->WriteInt64(mLimitBytes);
    dest->WriteInt64(mLastWarningSnooze);
    dest->WriteInt64(mLastLimitSnooze);
    dest->WriteBoolean(mMetered);
    dest->WriteBoolean(mInferred);
    return NOERROR;
}

ECode CNetworkPolicy::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);
    source->ReadInterfacePtr((Handle32*)&mTemplate);
    source->ReadInt32(&mCycleDay);
    source->ReadString(&mCycleTimezone);
    source->ReadInt64(&mWarningBytes);
    source->ReadInt64(&mLimitBytes);
    source->ReadInt64(&mLastWarningSnooze);
    source->ReadInt64(&mLastLimitSnooze);
    source->ReadBoolean(&mMetered);
    source->ReadBoolean(&mInferred);
    return NOERROR;
}

ECode CNetworkPolicy::GetTemplate(
    /* [out] */ INetworkTemplate** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mTemplate;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CNetworkPolicy::GetCycleDay(
    /* [out] */ Int32* cycleday)
{
    VALIDATE_NOT_NULL(cycleday);
    *cycleday = mCycleDay;
    return NOERROR;
}

ECode CNetworkPolicy::SetCycleDay(
    /* [in] */ Int32 cycleday)
{
    mCycleDay = cycleday;
    return NOERROR;
}

ECode CNetworkPolicy::GetCycleTimezone(
    /* [out] */ String* cycleTimezone)
{
    VALIDATE_NOT_NULL(cycleTimezone);
    *cycleTimezone = mCycleTimezone;
    return NOERROR;
}

ECode CNetworkPolicy::SetCycleTimezone(
    /* [in] */ const String& cycleTimezone)
{
    mCycleTimezone = cycleTimezone;
    return NOERROR;
}

ECode CNetworkPolicy::GetWarningBytes(
    /* [out] */ Int64* warningbytes)
{
    VALIDATE_NOT_NULL(warningbytes);
    *warningbytes = mWarningBytes;
    return NOERROR;
}

ECode CNetworkPolicy::SetWarningBytes(
    /* [in] */ Int64 warningbytes)
{
    mWarningBytes = warningbytes;
    return NOERROR;
}

ECode CNetworkPolicy::GetLimitBytes(
    /* [out] */ Int64* limitbytes)
{
    VALIDATE_NOT_NULL(limitbytes);
    *limitbytes = mLimitBytes;
    return NOERROR;
}

ECode CNetworkPolicy::SetLimitBytes(
    /* [in] */ Int64 limitbytes)
{
    mLimitBytes = limitbytes;
    return NOERROR;
}

ECode CNetworkPolicy::GetLastWarningSnooze(
    /* [out] */ Int64* lastwarningsnooze)
{
    VALIDATE_NOT_NULL(lastwarningsnooze);
    *lastwarningsnooze = mLastWarningSnooze;
    return NOERROR;
}

ECode CNetworkPolicy::SetLastWarningSnooze(
    /* [in] */ Int64 lastwarningsnooze)
{
    mLastWarningSnooze = lastwarningsnooze;
    return NOERROR;
}

ECode CNetworkPolicy::GetLastLimitSnooze(
    /* [out] */ Int64* lastlimitsnooze)
{
    VALIDATE_NOT_NULL(lastlimitsnooze);
    *lastlimitsnooze = mLastLimitSnooze;
    return NOERROR;
}

ECode CNetworkPolicy::SetLastLimitSnooze(
    /* [in] */ Int64 lastlimitsnooze)
{
    mLastLimitSnooze = lastlimitsnooze;
    return NOERROR;
}

ECode CNetworkPolicy::GetMetered(
    /* [out] */ Boolean* metered)
{
    VALIDATE_NOT_NULL(metered);
    *metered = mMetered;
    return NOERROR;
}

ECode CNetworkPolicy::SetMetered(
    /* [in] */ Boolean metered)
{
    mMetered = metered;
    return NOERROR;
}

ECode CNetworkPolicy::GetInferred(
    /* [out] */ Boolean* inferred)
{
    VALIDATE_NOT_NULL(inferred);
    *inferred = mInferred;
    return NOERROR;
}

ECode CNetworkPolicy::SetInferred(
    /* [in] */ Boolean inferred)
{
    mInferred = inferred;
    return NOERROR;
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
