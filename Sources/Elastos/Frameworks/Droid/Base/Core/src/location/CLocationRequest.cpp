
#include "location/CLocationRequest.h"
#include "location/LocationManager.h"
#include "os/SystemClock.h"
#include <elastos/core/Math.h>

using Elastos::Droid::Os::SystemClock;

namespace Elastos {
namespace Droid {
namespace Location {

const Double CLocationRequest::FASTEST_INTERVAL_FACTOR = 6.0;  // 6x

CLocationRequest::CLocationRequest()
    : mQuality(POWER_LOW)
    , mInterval(60 * 60 * 1000)   // 60 minutes
    , mFastestInterval((Int64)(mInterval / FASTEST_INTERVAL_FACTOR))
    , mExplicitFastestInterval(FALSE)
    , mExpireAt(Elastos::Core::Math::INT64_MAX_VALUE)
    , mNumUpdates(Elastos::Core::Math::INT32_MAX_VALUE)
    , mSmallestDisplacement(0.0f)
    , mProvider(ILocationManager::FUSED_PROVIDER)
{}

/**
 * Create a location request with default parameters.
 *
 * <p>Default parameters are for a low power, slowly updated location.
 * It can then be adjusted as required by the applications before passing
 * to the {@link LocationManager}
 *
 * @return a new location request
 */
AutoPtr<ILocationRequest> CLocationRequest::Create()
{
    AutoPtr<ILocationRequest> request;
    CLocationRequest::New((ILocationRequest**)&request);
    return request;
}

/** @hide */
AutoPtr<ILocationRequest> CLocationRequest::CreateFromDeprecatedProvider(
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ Boolean singleShot)
{
    if (minTime < 0) minTime = 0;
    if (minDistance < 0) minDistance = 0;

    Int32 quality;
    if (provider.Equals(ILocationManager::PASSIVE_PROVIDER)) {
        quality = POWER_NONE;
    } else if (provider.Equals(ILocationManager::GPS_PROVIDER)) {
        quality = ACCURACY_FINE;
    } else {
        quality = POWER_LOW;
    }

    AutoPtr<ILocationRequest> request;
    CLocationRequest::New((ILocationRequest**)&request);
    request->SetProvider(provider);
    request->SetQuality(quality);
    request->SetInterval(minTime);
    request->SetFastestInterval(minTime);
    request->SetSmallestDisplacement(minDistance);

    if (singleShot) request->SetNumUpdates(1);

    return request;
}

/** @hide */
AutoPtr<ILocationRequest> CLocationRequest::CreateFromDeprecatedCriteria(
    /* [in] */ ICriteria* criteria,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ Boolean singleShot)
{
    assert(criteria != NULL);

    if (minTime < 0) minTime = 0;
    if (minDistance < 0) minDistance = 0;

    Int32 quality;
    Int32 _quality;
    criteria->GetAccuracy(&_quality);
    switch (_quality) {
        case ICriteria::Criteria_ACCURACY_COARSE:
            quality = ACCURACY_BLOCK;
            break;
        case ICriteria::Criteria_ACCURACY_FINE:
            quality = ACCURACY_FINE;
            break;
        default: {
            criteria->GetPowerRequirement(&_quality);
            switch (_quality) {
                case ICriteria::Criteria_POWER_HIGH:
                    quality = POWER_HIGH;
                default:
                    quality = POWER_LOW;
            }
        }
    }

    AutoPtr<ILocationRequest> request;
    CLocationRequest::New((ILocationRequest**)&request);
    request->SetQuality(quality);
    request->SetInterval(minTime);
    request->SetFastestInterval(minTime);
    request->SetSmallestDisplacement(minDistance);
    if (singleShot) request->SetNumUpdates(1);
    return request;
}

/** @hide */
ECode CLocationRequest::constructor()
{
    return NOERROR;
}

/** @hide */
ECode CLocationRequest::constructor(
    /* [in] */ ILocationRequest* src)
{
    assert(src != NULL);
    CLocationRequest* _src = (CLocationRequest*)src;

    mQuality = _src->mQuality;
    mInterval = _src->mInterval;
    mFastestInterval = _src->mFastestInterval;
    mExplicitFastestInterval = _src->mExplicitFastestInterval;
    mExpireAt = _src->mExpireAt;
    mNumUpdates = _src->mNumUpdates;
    mSmallestDisplacement = _src->mSmallestDisplacement;
    mProvider = _src->mProvider;

    return NOERROR;
}

/**
 * Set the quality of the request.
 *
 * <p>Use with a accuracy constant such as {@link #ACCURACY_FINE}, or a power
 * constant such as {@link #POWER_LOW}. You cannot request both and accuracy and
 * power, only one or the other can be specified. The system will then
 * maximize accuracy or minimize power as appropriate.
 *
 * <p>The quality of the request is a strong hint to the system for which
 * location sources to use. For example, {@link #ACCURACY_FINE} is more likely
 * to use GPS, and {@link #POWER_LOW} is more likely to use WIFI & Cell tower
 * positioning, but it also depends on many other factors (such as which sources
 * are available) and is implementation dependent.
 *
 * <p>{@link #setQuality} and {@link #setInterval} are the most important parameters
 * on a location request.
 *
 * @param quality an accuracy or power constant
 * @throws InvalidArgumentException if the quality constant is not valid
 * @return the same object, so that setters can be chained
 */
ECode CLocationRequest::SetQuality(
    /* [in] */ Int32 quality)
{
    CheckQuality(quality);
    mQuality = quality;

    return NOERROR;
}

/**
 * Get the quality of the request.
 *
 * @return an accuracy or power constant
 */
ECode CLocationRequest::GetQuality(
    /* [out] */ Int32* quality)
{
    VALIDATE_NOT_NULL(quality);
    *quality = mQuality;
    return NOERROR;
}

/**
 * Set the desired interval for active location updates, in milliseconds.
 *
 * <p>The location manager will actively try to obtain location updates
 * for your application at this interval, so it has a
 * direct influence on the amount of power used by your application.
 * Choose your interval wisely.
 *
 * <p>This interval is inexact. You may not receive updates at all (if
 * no location sources are available), or you may receive them
 * slower than requested. You may also receive them faster than
 * requested (if other applications are requesting location at a
 * faster interval). The fastest rate that that you will receive
 * updates can be controlled with {@link #setFastestInterval}.
 *
 * <p>Applications with only the coarse location permission may have their
 * interval silently throttled.
 *
 * <p>An interval of 0 is allowed, but not recommended, since
 * location updates may be extremely fast on future implementations.
 *
 * <p>{@link #setQuality} and {@link #setInterval} are the most important parameters
 * on a location request.
 *
 * @param millis desired interval in millisecond, inexact
 * @throws InvalidArgumentException if the interval is less than zero
 * @return the same object, so that setters can be chained
 */
ECode CLocationRequest::SetInterval(
    /* [in] */ Int64 millis)
{
    CheckInterval(millis);
    mInterval = millis;
    if (!mExplicitFastestInterval) {
        mFastestInterval = (Int64)(mInterval / FASTEST_INTERVAL_FACTOR);
    }

    return NOERROR;
}

/**
 * Get the desired interval of this request, in milliseconds.
 *
 * @return desired interval in milliseconds, inexact
 */
ECode CLocationRequest::GetInterval(
    /* [out] */ Int64* interval)
{
    VALIDATE_NOT_NULL(interval);
    *interval = mInterval;

    return NOERROR;
}

/**
 * Explicitly set the fastest interval for location updates, in
 * milliseconds.
 *
 * <p>This controls the fastest rate at which your application will
 * receive location updates, which might be faster than
 * {@link #setInterval} in some situations (for example, if other
 * applications are triggering location updates).
 *
 * <p>This allows your application to passively acquire locations
 * at a rate faster than it actively acquires locations, saving power.
 *
 * <p>Unlike {@link #setInterval}, this parameter is exact. Your
 * application will never receive updates faster than this value.
 *
 * <p>If you don't call this method, a fastest interval
 * will be selected for you. It will be a value faster than your
 * active interval ({@link #setInterval}).
 *
 * <p>An interval of 0 is allowed, but not recommended, since
 * location updates may be extremely fast on future implementations.
 *
 * <p>If {@link #setFastestInterval} is set slower than {@link #setInterval},
 * then your effective fastest interval is {@link #setInterval}.
 *
 * @param millis fastest interval for updates in milliseconds, exact
 * @throws InvalidArgumentException if the interval is less than zero
 * @return the same object, so that setters can be chained
 */
ECode CLocationRequest::SetFastestInterval(
    /* [in] */ Int64 millis)
{
    CheckInterval(millis);
    mExplicitFastestInterval = TRUE;
    mFastestInterval = millis;

    return NOERROR;
}

/**
 * Get the fastest interval of this request, in milliseconds.
 *
 * <p>The system will never provide location updates faster
 * than the minimum of {@link #getFastestInterval} and
 * {@link #getInterval}.
 *
 * @return fastest interval in milliseconds, exact
 */
ECode CLocationRequest::GetFastestInterval(
    /* [out] */ Int64* fastestInterval)
{
    VALIDATE_NOT_NULL(fastestInterval);
    *fastestInterval = mFastestInterval;

    return NOERROR;
}

/**
 * Set the duration of this request, in milliseconds.
 *
 * <p>The duration begins immediately (and not when the request
 * is passed to the location manager), so call this method again
 * if the request is re-used at a later time.
 *
 * <p>The location manager will automatically stop updates after
 * the request expires.
 *
 * <p>The duration includes suspend time. Values less than 0
 * are allowed, but indicate that the request has already expired.
 *
 * @param millis duration of request in milliseconds
 * @return the same object, so that setters can be chained
 */
ECode CLocationRequest::SetExpireIn(
    /* [in] */ Int64 millis)
{
    Int64 elapsedRealtime = SystemClock::GetElapsedRealtime();

    // Check for > Long.MAX_VALUE overflow (elapsedRealtime > 0):
    if (millis > Elastos::Core::Math::INT64_MAX_VALUE - elapsedRealtime) {
      mExpireAt = Elastos::Core::Math::INT64_MAX_VALUE;
    } else {
      mExpireAt = millis + elapsedRealtime;
    }

    if (mExpireAt < 0) mExpireAt = 0;

    return NOERROR;
}

/**
 * Set the request expiration time, in millisecond since boot.
 *
 * <p>This expiration time uses the same time base as {@link SystemClock#elapsedRealtime}.
 *
 * <p>The location manager will automatically stop updates after
 * the request expires.
 *
 * <p>The duration includes suspend time. Values before {@link SystemClock#elapsedRealtime}
 * are allowed,  but indicate that the request has already expired.
 *
 * @param millis expiration time of request, in milliseconds since boot including suspend
 * @return the same object, so that setters can be chained
 */
ECode CLocationRequest::SetExpireAt(
    /* [in] */ Int64 millis)
{
    mExpireAt = millis;
    if (mExpireAt < 0) mExpireAt = 0;

    return NOERROR;
}

/**
 * Get the request expiration time, in milliseconds since boot.
 *
 * <p>This value can be compared to {@link SystemClock#elapsedRealtime} to determine
 * the time until expiration.
 *
 * @return expiration time of request, in milliseconds since boot including suspend
 */
ECode CLocationRequest::GetExpireAt(
    /* [out] */ Int64* expireAt)
{
    VALIDATE_NOT_NULL(expireAt);
    *expireAt = mExpireAt;

    return NOERROR;
}

/**
 * Set the number of location updates.
 *
 * <p>By default locations are continuously updated until the request is explicitly
 * removed, however you can optionally request a set number of updates.
 * For example, if your application only needs a single fresh location,
 * then call this method with a value of 1 before passing the request
 * to the location manager.
 *
 * @param numUpdates the number of location updates requested
 * @throws InvalidArgumentException if numUpdates is 0 or less
 * @return the same object, so that setters can be chained
 */
ECode CLocationRequest::SetNumUpdates(
    /* [in] */ Int32 numUpdates)
{
    if (numUpdates <= 0) {
    //    throw new IllegalArgumentException("invalid numUpdates: " + numUpdates);
        assert(0);
    }

    mNumUpdates = numUpdates;

    return NOERROR;
}

/**
 * Get the number of updates requested.
 *
 * <p>By default this is {@link Integer#MAX_VALUE}, which indicates that
 * locations are updated until the request is explicitly removed.
 * @return number of updates
 */
ECode CLocationRequest::GetNumUpdates(
    /* [out] */ Int32* numUpdates)
{
    VALIDATE_NOT_NULL(numUpdates);
    *numUpdates = mNumUpdates;

    return NOERROR;
}

/** @hide */
ECode CLocationRequest::DecrementNumUpdates()
{
    if (mNumUpdates != Elastos::Core::Math::INT32_MAX_VALUE) {
        mNumUpdates--;
    }
    if (mNumUpdates < 0) {
        mNumUpdates = 0;
    }

    return NOERROR;
}

/** @hide */
ECode CLocationRequest::SetProvider(
    /* [in] */ const String& provider)
{
    CheckProvider(provider);
    mProvider = provider;

    return NOERROR;
}

/** @hide */
ECode CLocationRequest::GetProvider(
    /* [out] */ String* provider)
{
    VALIDATE_NOT_NULL(provider);
    *provider = mProvider;

    return NOERROR;
}

/** @hide */
ECode CLocationRequest::SetSmallestDisplacement(
    /* [in] */ Float meters)
{
    CheckDisplacement(meters);
    mSmallestDisplacement = meters;

    return NOERROR;
}

/** @hide */
ECode CLocationRequest::GetSmallestDisplacement(
    /* [out] */ Float* smallestDisplacement)
{
    VALIDATE_NOT_NULL(smallestDisplacement);
    *smallestDisplacement = mSmallestDisplacement;

    return NOERROR;
}

//@Override
ECode CLocationRequest::DescribeContents(
    /* [out] */ Int32* results)
{
    VALIDATE_NOT_NULL(results);
    *results = 0;

    return NOERROR;
}

ECode CLocationRequest::ReadFromParcel(
    /* [in] */ IParcel* in)
{
    VALIDATE_NOT_NULL(in);

    Int32 tempInt32;
    in->ReadInt32(&tempInt32);
    SetQuality(tempInt32);
    Int64 tempInt64;
    in->ReadInt64(&tempInt64);
    SetFastestInterval(tempInt64);
    in->ReadInt64(&tempInt64);
    SetInterval(tempInt64);
    in->ReadInt64(&tempInt64);
    SetExpireAt(tempInt64);
    in->ReadInt32(&tempInt32);
    SetNumUpdates(tempInt32);
    Float smallestDisplacement;
    in->ReadFloat(&smallestDisplacement);
    SetSmallestDisplacement(smallestDisplacement);
    String provider;
    in->ReadString(&provider);
    if (!provider.IsNull()) {
        SetProvider(provider);
    }

    return NOERROR;
}

//@Override
ECode CLocationRequest::WriteToParcel(
    /* [in] */ IParcel* parcel)
{
    VALIDATE_NOT_NULL(parcel);

    parcel->WriteInt32(mQuality);
    parcel->WriteInt64(mFastestInterval);
    parcel->WriteInt64(mInterval);
    parcel->WriteInt64(mExpireAt);
    parcel->WriteInt32(mNumUpdates);
    parcel->WriteFloat(mSmallestDisplacement);
    parcel->WriteString(mProvider);

    return NOERROR;
}

/** @hide */
String CLocationRequest::QualityToString(
    /* [in] */ Int32 quality)
{
    String str;
    switch (quality) {
        case ACCURACY_FINE:
            {
                str = "ACCURACY_FINE";
                return str;
            }

        case ACCURACY_BLOCK:
            {
                str = "ACCURACY_BLOCK";
                return str;
            }

        case ACCURACY_CITY:
            {
                str = "ACCURACY_CITY";
                return str;
            }

        case POWER_NONE:
            {
                str = "POWER_NONE";
                return str;
            }

        case POWER_LOW:
            {
                str = "POWER_LOW";
                return str;
            }

        case POWER_HIGH:
            {
                str = "POWER_HIGH";
                return str;
            }

        default:
            {
                str = "???";
                return str;
            }
    }
}

void CLocationRequest::CheckInterval(
    /* [in] */ Int64 millis)
{
    if (millis < 0) {
    //    throw new IllegalArgumentException("invalid interval: " + millis);
        assert(0);
    }
}

void CLocationRequest::CheckQuality(
    /* [in] */ Int32 quality)
{
    switch (quality) {
        case ACCURACY_FINE:
        case ACCURACY_BLOCK:
        case ACCURACY_CITY:
        case POWER_NONE:
        case POWER_LOW:
        case POWER_HIGH:
            break;
        default:
            //throw new IllegalArgumentException("invalid quality: " + quality);
            assert(0);
    }
}

void CLocationRequest::CheckDisplacement(
    /* [in] */ Float meters)
{
    if (meters < 0.0f) {
    //    throw new IllegalArgumentException("invalid displacement: " + meters);
        assert(0);
    }
}

void CLocationRequest::CheckProvider(
    /* [in] */ const String& name)
{
    if (name.IsNullOrEmpty()) {
    //    throw new IllegalArgumentException("invalid provider: " + name);
        assert(0);
    }
}

}//namespace Location
}//namespace Droid
}//namespace Elastos
