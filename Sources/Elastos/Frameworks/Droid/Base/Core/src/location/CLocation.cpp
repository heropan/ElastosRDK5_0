
#include "location/CLocation.h"
#include "os/CBundle.h"
#include <elastos/Math.h>
#include "elastos/StringBuffer.h"
#include <elastos/StringUtils.h>
#include <elastos/Logger.h>

using Elastos::Droid::Os::CBundle;
using Elastos::Core::StringBuffer;
using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Location {

struct ElaLocationCallback CLocation::sElaLocationCallback =
{
    &CLocation::GetLatitude,
    &CLocation::GetLongitude,
    &CLocation::HasAltitude,
    &CLocation::GetAltitude,
    &CLocation::HasAccuracy,
    &CLocation::GetAccuracy,
    &CLocation::HasBearing,
    &CLocation::GetBearing,
    &CLocation::HasSpeed,
    &CLocation::GetSpeed,
    &CLocation::GetTime
};

CLocation::CLocation()
   : mTime(0)
   , mElapsedRealtimeNanos(0)
   , mLatitude(0.0)
   , mLongitude(0.0)
   , mHasAltitude(FALSE)
   , mAltitude(0.0f)
   , mHasSpeed(FALSE)
   , mSpeed(0.0f)
   , mHasBearing(FALSE)
   , mBearing(0.0f)
   , mHasAccuracy(FALSE)
   , mAccuracy(0.0f)
   , mExtras(NULL)
   , mLat1(0.0)
   , mLon1(0.0)
   , mLat2(0.0)
   , mLon2(0.0)
   , mDistance(0.0f)
   , mInitialBearing(0.0f)
{
    // Scratchpad
    mResults = ArrayOf<Float>::Alloc(2);
}

ECode CLocation::constructor()
{
    return NOERROR;
}

/**
 * Construct a new Location with a named provider.
 *
 * <p>By default time, latitude and longitude are 0, and the location
 * has no bearing, altitude, speed, accuracy or extras.
 *
 * @param provider the name of the provider that generated this location
 */
ECode CLocation::constructor(
    /* [in] */ const String& provider)
{
    mProvider = provider;
    return NOERROR;
}

/**
 * Construct a new Location object that is copied from an existing one.
 */
ECode CLocation::constructor(
    /* [in] */ ILocation* l)
{
    Set(l);
    return NOERROR;
}

/**
 * Sets the contents of the location to the values from the given location.
 */
ECode CLocation::Set(
    /* [in] */ ILocation* l)
{
    AutoPtr<CLocation> location = (CLocation*)l;
    mProvider = location->mProvider;
    mTime = location->mTime;
    mElapsedRealtimeNanos = location->mElapsedRealtimeNanos;
    mLatitude = location->mLatitude;
    mLongitude = location->mLongitude;
    mHasAltitude = location->mHasAltitude;
    mAltitude = location->mAltitude;
    mHasSpeed = location->mHasSpeed;
    mSpeed = location->mSpeed;
    mHasBearing = location->mHasBearing;
    mBearing = location->mBearing;
    mHasAccuracy = location->mHasAccuracy;
    mAccuracy = location->mAccuracy;
    mExtras = NULL;
    if (location->mExtras != NULL) {
        ASSERT_SUCCEEDED(CBundle::New(location->mExtras, (IBundle**)&mExtras));
    }

    return NOERROR;
}

/**
 * Clears the contents of the location.
 */
ECode CLocation::Reset()
{
    mProvider = "";
    mTime = 0;
    mElapsedRealtimeNanos = 0;
    mLatitude = 0;
    mLongitude = 0;
    mHasAltitude = FALSE;
    mAltitude = 0;
    mHasSpeed = FALSE;
    mSpeed = 0;
    mHasBearing = FALSE;
    mBearing = 0;
    mHasAccuracy = FALSE;
    mAccuracy = 0;
    mExtras = NULL;

    return NOERROR;
}

/**
 * Converts a coordinate to a String representation. The outputType
 * may be one of FORMAT_DEGREES, FORMAT_MINUTES, or FORMAT_SECONDS.
 * The coordinate must be a valid double between -180.0 and 180.0.
 *
 * @throws IllegalArgumentException if coordinate is less than
 * -180.0, greater than 180.0, or is not a number.
 * @throws IllegalArgumentException if outputType is not one of
 * FORMAT_DEGREES, FORMAT_MINUTES, or FORMAT_SECONDS.
 */
ECode CLocation::Convert(
    /* [in] */ Double coordinate,
    /* [in] */ Int32 outputType,
    /* [out] */ String* representation)
{
    VALIDATE_NOT_NULL(representation);

    if (coordinate < -180.0 || coordinate > 180.0 || Elastos::Core::Math::IsNaN(coordinate)) {
        Logger::E("CLocation", "coordinate=%d", coordinate);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if ((outputType != ILocation::FORMAT_DEGREES) &&
        (outputType != ILocation::FORMAT_MINUTES) &&
        (outputType != ILocation::FORMAT_SECONDS)) {
        Logger::E("CLocation", "outputType=%d", outputType);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    StringBuffer sb;// = new StringBuilder();

    // Handle negative values
    if (coordinate < 0) {
        sb += "-";
        coordinate = -coordinate;
    }

//    DecimalFormat df = new DecimalFormat("###.#####");
    if (outputType == ILocation::FORMAT_MINUTES || outputType == ILocation::FORMAT_SECONDS) {
        Int32 degrees = (Int32) Elastos::Core::Math::Floor(coordinate);
        sb += degrees + ":";
//        sb.Append(':');
        coordinate -= degrees;
        coordinate *= 60.0;
        if (outputType == ILocation::FORMAT_SECONDS) {
            Int32 minutes = (Int32) Elastos::Core::Math::Floor(coordinate);
            sb += minutes + ":";
            coordinate -= minutes;
            coordinate *= 60.0;
        }
    }
//    sb.Append(df.format(coordinate));
    *representation = (String)sb.ToString();

    return NOERROR;

}

/**
 * Converts a String in one of the formats described by
 * FORMAT_DEGREES, FORMAT_MINUTES, or FORMAT_SECONDS into a
 * double.
 *
 * @throws NullPointerException if coordinate is null
 * @throws IllegalArgumentException if the coordinate is not
 * in one of the valid formats.
 */
ECode CLocation::Convert(
    /* [in] */ const String& coordinate,
    /* [out] */ Double* representation)
{
    VALIDATE_NOT_NULL(representation);

    // IllegalArgumentException if bad syntax
    if (coordinate == NULL) {
        Logger::E("CLocation", "coordinate");
        return E_NULL_POINTER_EXCEPTION;
    }

    Boolean negative = FALSE;
    if (coordinate.GetChar(0) == '-') {
        // coordinate = coordinate.Substring(1);
        negative = TRUE;
    }

    AutoPtr<ArrayOf<String> > values;
    StringUtils::Split(coordinate, String(":"), (ArrayOf<String>**)&values);
    if (values == NULL || values->GetLength() < 1) {
        Logger::E("CLocation", "coordinate=%s", coordinate.string());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

//    try {
    Int32 index = 0;
    Int32 tokens = values->GetLength();
    String degrees = (*values)[index++];
    Double val;
    if (tokens == 1) {
        val = StringUtils::ParseDouble(degrees);
        *representation = negative ? -val : val;
        return NOERROR;
    }

    String minutes = (*values)[index++];
    Int32 deg = StringUtils::ParseInt32(degrees);
    Double min;
    Double sec = 0.0;

    if (tokens > 2) {
        min = StringUtils::ParseInt32(minutes);
        String seconds = (*values)[index++];
        sec = StringUtils::ParseDouble(seconds);
    }
    else {
        min = StringUtils::ParseDouble(minutes);
    }

    Boolean isNegative180 = negative && (deg == 180) &&
        (min == 0) && (sec == 0);

    // deg must be in [0, 179] except for the case of -180 degrees
    if ((deg < 0.0) || (deg > 179 && !isNegative180)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("coordinate=" + coordinate);
    }
    if (min < 0 || min > 59) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("coordinate=" +
//                coordinate);
    }
    if (sec < 0 || sec > 59) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("coordinate=" +
//                coordinate);
    }

    val = deg * 3600.0 + min * 60.0 + sec;
    val /= 3600.0;
    *representation = negative ? -val : val;

    return NOERROR;
//    } catch (NumberFormatException nfe) {
//        throw new IllegalArgumentException("coordinate=" + coordinate);
//    }
}

/**
 * Computes the approximate distance in meters between two
 * locations, and optionally the initial and final bearings of the
 * shortest path between them.  Distance and bearing are defined using the
 * WGS84 ellipsoid.
 *
 * <p> The computed distance is stored in results[0].  If results has length
 * 2 or greater, the initial bearing is stored in results[1]. If results has
 * length 3 or greater, the final bearing is stored in results[2].
 *
 * @param startLatitude the starting latitude
 * @param startLongitude the starting longitude
 * @param endLatitude the ending latitude
 * @param endLongitude the ending longitude
 * @param results an array of floats to hold the results
 *
 * @throws IllegalArgumentException if results is null or has length < 1
 */
ECode CLocation::DistanceBetween(
    /* [in] */ Double startLatitude,
    /* [in] */ Double startLongitude,
    /* [in] */ Double endLatitude,
    /* [in] */ Double endLongitude,
    /* [in] */ ArrayOf<Float>* results)
{
    if (results == NULL || results->GetLength() < 1) {
        Logger::E("CLocation", "results is null or has length < 1");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    ComputeDistanceAndBearing(startLatitude, startLongitude,
        endLatitude, endLongitude, results);

    return NOERROR;
}

/**
 * Returns the approximate distance in meters between this
 * location and the given location.  Distance is defined using
 * the WGS84 ellipsoid.
 *
 * @param dest the destination location
 * @return the approximate distance in meters
 */
ECode CLocation::DistanceTo(
    /* [in] */ ILocation* dest,
    /* [out] */ Float* distance)
{
    VALIDATE_NOT_NULL(distance);

    // See if we already have the result
    Mutex::Autolock lock(mResultsLock);

    AutoPtr<CLocation> destLocation = (CLocation*)dest;
    if (mLatitude != mLat1 || mLongitude != mLon1 ||
        destLocation->mLatitude != mLat2 || destLocation->mLongitude != mLon2) {
        ComputeDistanceAndBearing(mLatitude, mLongitude,
            destLocation->mLatitude, destLocation->mLongitude, mResults);
        mLat1 = mLatitude;
        mLon1 = mLongitude;
        mLat2 = destLocation->mLatitude;
        mLon2 = destLocation->mLongitude;
        mDistance = (*mResults)[0];
        mInitialBearing = (*mResults)[1];
    }
    *distance = mDistance;

    return NOERROR;
}

/**
 * Returns the approximate initial bearing in degrees East of true
 * North when traveling along the shortest path between this
 * location and the given location.  The shortest path is defined
 * using the WGS84 ellipsoid.  Locations that are (nearly)
 * antipodal may produce meaningless results.
 *
 * @param dest the destination location
 * @return the initial bearing in degrees
 */
ECode CLocation::BearingTo(
    /* [in] */ ILocation* dest,
    /* [out] */ Float* initialBearing)
{
    VALIDATE_NOT_NULL(initialBearing);

    Mutex::Autolock lock(mResultsLock);

    // See if we already have the result
    AutoPtr<CLocation> destLocation = (CLocation*)dest;
    if (mLatitude != mLat1 || mLongitude != mLon1 ||
            destLocation->mLatitude != mLat2 || destLocation->mLongitude != mLon2) {
        ComputeDistanceAndBearing(mLatitude, mLongitude,
            destLocation->mLatitude, destLocation->mLongitude, mResults);
        mLat1 = mLatitude;
        mLon1 = mLongitude;
        mLat2 = destLocation->mLatitude;
        mLon2 = destLocation->mLongitude;
        mDistance = (*mResults)[0];
        mInitialBearing = (*mResults)[1];
    }
    *initialBearing = mInitialBearing;

    return NOERROR;
}

/**
 * Returns the name of the provider that generated this fix.
 *
 * @return the provider, or null if it has not been set
 */
ECode CLocation::GetProvider(
    /* [out] */ String* provider)
{
    VALIDATE_NOT_NULL(provider);
    *provider = mProvider;
    return NOERROR;
}

/**
 * Sets the name of the provider that generated this fix.
 */
ECode CLocation::SetProvider(
    /* [in] */ const String& provider)
{
    mProvider = provider;

    return NOERROR;
}

/**
 * Return the UTC time of this fix, in milliseconds since January 1, 1970.
 *
 * <p>Note that the UTC time on a device is not monotonic: it
 * can jump forwards or backwards unpredictably. So always use
 * {@link #getElapsedRealtimeNanos} when calculating time deltas.
 *
 * <p>On the other hand, {@link #getTime} is useful for presenting
 * a human readable time to the user, or for carefully comparing
 * location fixes across reboot or across devices.
 *
 * <p>All locations generated by the {@link LocationManager}
 * are guaranteed to have a valid UTC time, however remember that
 * the system time may have changed since the location was generated.
 *
 * @return time of fix, in milliseconds since January 1, 1970.
 */
ECode CLocation::GetTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = mTime;

    return NOERROR;
}

/**
 * Set the UTC time of this fix, in milliseconds since January 1,
 * 1970.
 *
 * @param time UTC time of this fix, in milliseconds since January 1, 1970
 */
ECode CLocation::SetTime(
    /* [in] */ Int64 time)
{
    mTime = time;

    return NOERROR;
}

/**
 * Return the time of this fix, in elapsed real-time since system boot.
 *
 * <p>This value can be reliably compared to
 * {@link android.os.SystemClock#elapsedRealtimeNanos},
 * to calculate the age of a fix and to compare Location fixes. This
 * is reliable because elapsed real-time is guaranteed monotonic for
 * each system boot and continues to increment even when the system
 * is in deep sleep (unlike {@link #getTime}.
 *
 * <p>All locations generated by the {@link LocationManager}
 * are guaranteed to have a valid elapsed real-time.
 *
 * @return elapsed real-time of fix, in nanoseconds since system boot.
 */
ECode CLocation::GetElapsedRealtimeNanos(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = mElapsedRealtimeNanos;

    return NOERROR;
}

/**
 * Set the time of this fix, in elapsed real-time since system boot.
 *
 * @param time elapsed real-time of fix, in nanoseconds since system boot.
 */
ECode CLocation::SetElapsedRealtimeNanos(
    /* [in] */ Int64 time)
{
    mElapsedRealtimeNanos = time;

    return NOERROR;
}

/**
 * Get the latitude, in degrees.
 *
 * <p>All locations generated by the {@link LocationManager}
 * will have a valid latitude.
 */
ECode CLocation::GetLatitude(
    /* [out] */ Double* latitude)
{
    VALIDATE_NOT_NULL(latitude);
    *latitude = mLatitude;

    return NOERROR;
}

/**
 * Set the latitude, in degrees.
 */
ECode CLocation::SetLatitude(
    /* [in] */ Double latitude)
{
    mLatitude = latitude;

    return NOERROR;
}

/**
 * Get the longitude, in degrees.
 *
 * <p>All locations generated by the {@link LocationManager}
 * will have a valid longitude.
 */
ECode CLocation::GetLongitude(
    /* [out] */ Double* longitude)
{
    VALIDATE_NOT_NULL(longitude);
    *longitude = mLongitude;

    return NOERROR;
}

/**
 * Set the longitude, in degrees.
 */
ECode CLocation::SetLongitude(
    /* [in] */ Double longitude)
{
    mLongitude = longitude;

    return NOERROR;
}

/**
 * True if this location has an altitude.
 */
ECode CLocation::HasAltitude(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mHasAltitude;

    return NOERROR;
}

/**
 * Get the altitude if available, in meters above sea level.
 *
 * <p>If this location does not have an altitude then 0.0 is returned.
 */
ECode CLocation::GetAltitude(
    /* [out] */ Double* altitude)
{
    VALIDATE_NOT_NULL(altitude);
    *altitude = mAltitude;

    return NOERROR;
}

/**
 * Set the altitude, in meters above sea level.
 *
 * <p>Following this call {@link #hasAltitude} will return true.
 */
ECode CLocation::SetAltitude(
    /* [in] */ Double altitude)
{
    mAltitude = altitude;
    mHasAltitude = TRUE;

    return NOERROR;
}

/**
 * Remove the altitude from this location.
 *
 * <p>Following this call {@link #hasAltitude} will return false,
 * and {@link #getAltitude} will return 0.0.
 */
ECode CLocation::RemoveAltitude()
{
    mAltitude = 0.0f;
    mHasAltitude = FALSE;

    return NOERROR;
}

/**
 * True if this location has a speed.
 */
ECode CLocation::HasSpeed(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mHasSpeed;

    return NOERROR;
}

/**
 * Get the speed if it is available, in meters/second over ground.
 *
 * <p>If this location does not have a speed then 0.0 is returned.
 */
ECode CLocation::GetSpeed(
    /* [out] */ Float* speed)
{
    VALIDATE_NOT_NULL(speed);
    *speed = mSpeed;

    return NOERROR;
}

/**
 * Set the speed, in meters/second over ground.
 *
 * <p>Following this call {@link #hasSpeed} will return true.
 */
ECode CLocation::SetSpeed(
    /* [in] */ Float speed)
{
    mSpeed = speed;
    mHasSpeed = TRUE;

    return NOERROR;
}

/**
 * Remove the speed from this location.
 *
 * <p>Following this call {@link #hasSpeed} will return false,
 * and {@link #getSpeed} will return 0.0.
 */
ECode CLocation::RemoveSpeed()
{
    mSpeed = 0.0f;
    mHasSpeed = FALSE;

    return NOERROR;
}

/**
 * True if this location has a bearing.
 */
ECode CLocation::HasBearing(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mHasBearing;

    return NOERROR;
}

/**
 * Get the bearing, in degrees.
 *
 * <p>Bearing is the horizontal direction of travel of this device,
 * and is not related to the device orientation. It is guaranteed to
 * be in the range (0.0, 360.0] if the device has a bearing.
 *
 * <p>If this location does not have a bearing then 0.0 is returned.
 */
ECode CLocation::GetBearing(
    /* [out] */ Float* bearing)
{
    VALIDATE_NOT_NULL(bearing);
    *bearing = mBearing;

    return NOERROR;
}

/**
 * Set the bearing, in degrees.
 *
 * <p>Bearing is the horizontal direction of travel of this device,
 * and is not related to the device orientation.
 *
 * <p>The input will be wrapped into the range (0.0, 360.0].
 */
ECode CLocation::SetBearing(
    /* [in] */ Float bearing)
{
    while (bearing < 0.0f) {
        bearing += 360.0f;
    }
    while (bearing >= 360.0f) {
        bearing -= 360.0f;
    }

    mBearing = bearing;
    mHasBearing = TRUE;

    return NOERROR;
}

/**
 * Remove the bearing from this location.
 *
 * <p>Following this call {@link #hasBearing} will return false,
 * and {@link #getBearing} will return 0.0.
 */
ECode CLocation::RemoveBearing()
{
    mBearing = 0.0f;
    mHasBearing = FALSE;

    return NOERROR;
}

/**
 * True if this location has an accuracy.
 *
 * <p>All locations generated by the {@link LocationManager} have an
 * accuracy.
 */
ECode CLocation::HasAccuracy(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mHasAccuracy;

    return NOERROR;
}

/**
 * Get the estimated accuracy of this location, in meters.
 *
 * <p>We define accuracy as the radius of 68% confidence. In other
 * words, if you draw a circle centered at this location's
 * latitude and longitude, and with a radius equal to the accuracy,
 * then there is a 68% probability that the true location is inside
 * the circle.
 *
 * <p>In statistical terms, it is assumed that location errors
 * are random with a normal distribution, so the 68% confidence circle
 * represents one standard deviation. Note that in practice, location
 * errors do not always follow such a simple distribution.
 *
 * <p>This accuracy estimation is only concerned with horizontal
 * accuracy, and does not indicate the accuracy of bearing,
 * velocity or altitude if those are included in this Location.
 *
 * <p>If this location does not have an accuracy, then 0.0 is returned.
 * All locations generated by the {@link LocationManager} include
 * an accuracy.
 */
ECode CLocation::GetAccuracy(
    /* [out] */ Float* accuracy)
{
    VALIDATE_NOT_NULL(accuracy);
    *accuracy = mAccuracy;

    return NOERROR;
}

/**
 * Set the estimated accuracy of this location, meters.
 *
 * <p>See {@link #getAccuracy} for the definition of accuracy.
 *
 * <p>Following this call {@link #hasAccuracy} will return true.
 */
ECode CLocation::SetAccuracy(
    /* [in] */ Float accuracy)
{
    mAccuracy = accuracy;
    mHasAccuracy = TRUE;

    return NOERROR;
}

/**
 * Remove the accuracy from this location.
 *
 * <p>Following this call {@link #hasAccuracy} will return false, and
 * {@link #getAccuracy} will return 0.0.
 */
ECode CLocation::RemoveAccuracy()
{
    mAccuracy = 0.0f;
    mHasAccuracy = FALSE;

    return NOERROR;
}

/**
 * Return true if this Location object is complete.
 *
 * <p>A location object is currently considered complete if it has
 * a valid provider, accuracy, wall-clock time and elapsed real-time.
 *
 * <p>All locations supplied by the {@link LocationManager} to
 * applications must be complete.
 *
 * @see #makeComplete
 * @hide
 */
ECode CLocation::IsComplete(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (mProvider == "") {
        *result = FALSE;
        return NOERROR;
    }

    if (!mHasAccuracy) {
        *result = FALSE;
        return NOERROR;
    }

    if (mTime == 0) {
        *result = FALSE;
        return NOERROR;
    }

    if (mElapsedRealtimeNanos == 0) {
        *result = FALSE;
        return NOERROR;
    }

    *result = TRUE;

    return NOERROR;
}

/**
 * Helper to fill incomplete fields.
 *
 * <p>Used to assist in backwards compatibility with
 * Location objects received from applications.
 *
 * @see #isComplete
 * @hide
 */
ECode CLocation::MakeComplete()
{
    if (mProvider == "") mProvider = "?";
    if (!mHasAccuracy) {
        mHasAccuracy = TRUE;
        mAccuracy = 100.0f;
    }
//    if (mTime == 0) mTime = System.currentTimeMillis();
//    if (mElapsedRealtimeNanos == 0) mElapsedRealtimeNanos = SystemClock.elapsedRealtimeNanos();

    return NOERROR;
}

/**
 * Returns additional provider-specific information about the
 * location fix as a Bundle.  The keys and values are determined
 * by the provider.  If no additional information is available,
 * null is returned.
 *
 * <p> A number of common key/value pairs are listed
 * below. Providers that use any of the keys on this list must
 * provide the corresponding value as described below.
 *
 * <ul>
 * <li> satellites - the number of satellites used to derive the fix
 * </ul>
 */
ECode CLocation::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras);
    *extras = mExtras;
    INTERFACE_ADDREF(*extras);

    return NOERROR;
}

/**
 * Sets the extra information associated with this fix to the
 * given Bundle.
 */
ECode CLocation::SetExtras(
    /* [in] */ IBundle* extras)
{
    mExtras = extras;

    return NOERROR;
}

//@Override
ECode CLocation::DescribeContents(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    return NOERROR;
}

ECode CLocation::ReadFromParcel(
    /* [in] */ IParcel* in)
{
    in->ReadString(&mProvider);
    in->ReadInt64(&mTime);
    in->ReadInt64(&mElapsedRealtimeNanos);
    in->ReadDouble(&mLatitude);
    in->ReadDouble(&mLongitude);

    Int32 tempInt32;

    in->ReadInt32(&tempInt32);
    mHasAltitude = tempInt32 != 0;
    in->ReadDouble(&mAltitude);
    in->ReadInt32(&tempInt32);
    mHasSpeed = tempInt32 != 0;
    in->ReadFloat(&mSpeed);
    in->ReadInt32(&tempInt32);
    mHasBearing = tempInt32 != 0;
    in->ReadFloat(&mBearing);
    in->ReadInt32(&tempInt32);
    mHasAccuracy = tempInt32 != 0;
    in->ReadFloat(&mAccuracy);
//    in->ReadBundle(&mExtras);

    return NOERROR;
}

//@Override
ECode CLocation::WriteToParcel(
    /* [in] */ IParcel* parcel)
{
    VALIDATE_NOT_NULL(parcel);

    parcel->WriteString(mProvider);
    parcel->WriteInt64(mTime);
    parcel->WriteInt64(mElapsedRealtimeNanos);
    parcel->WriteDouble(mLatitude);
    parcel->WriteDouble(mLongitude);
    parcel->WriteInt32(mHasAltitude ? 1 : 0);
    parcel->WriteDouble(mAltitude);
    parcel->WriteInt32(mHasSpeed ? 1 : 0);
    parcel->WriteFloat(mSpeed);
    parcel->WriteInt32(mHasBearing ? 1 : 0);
    parcel->WriteFloat(mBearing);
    parcel->WriteInt32(mHasAccuracy ? 1 : 0);
    parcel->WriteFloat(mAccuracy);
//    parcel->WriteBundle(mExtras);

    return NOERROR;
}

/**
 * Returns one of the optional extra {@link Location}s that can be attached
 * to this Location.
 *
 * @param key the key associated with the desired extra Location
 * @return the extra Location, or null if unavailable
 * @hide
 */
ECode CLocation::GetExtraLocation(
    /* [in] */ const String& key,
    /* [out] */ ILocation** location)
{
/*
    if (mExtras != null) {
        Parcelable value = mExtras.getParcelable(key);
        if (value instanceof Location) {
            return (Location) value;
        }
    }
    return null;
*/

    return NOERROR; //temp
}

/**
 * Attaches an extra {@link Location} to this Location.
 *
 * @param key the key associated with the Location extra
 * @param location the Location to attach
 * @hide
 */
ECode CLocation::SetExtraLocation(
    /* [in] */ const String& key,
    /* [in] */ ILocation* value)
{
/*
    if (mExtras == null) {
        mExtras = new Bundle();
    }
    mExtras.putParcelable(key, value);
*/

    return NOERROR; //temp
}

Double CLocation::GetLatitude(
    /* [in] */ IInterface* obj)
{
    AutoPtr<ILocation> location = (ILocation*)(obj);
    Double result;
    location->GetLatitude(&result);
    return result;
}

Double CLocation::GetLongitude(
    /* [in] */ IInterface* obj)
{
    AutoPtr<ILocation> location = (ILocation*)(obj);
    Double result;
    location->GetLongitude(&result);
    return result;
}

Boolean CLocation::HasAltitude(
    /* [in] */ IInterface* obj)
{
    AutoPtr<ILocation> location = (ILocation*)(obj);
    Boolean result;
    location->HasAltitude(&result);
    return result;
}

Double CLocation::GetAltitude(
    /* [in] */ IInterface* obj)
{
    AutoPtr<ILocation> location = (ILocation*)(obj);
    Double result;
    location->GetAltitude(&result);
    return result;
}

Boolean CLocation::HasAccuracy(
    /* [in] */ IInterface* obj)
{
    AutoPtr<ILocation> location = (ILocation*)(obj);
    Boolean result;
    location->HasAccuracy(&result);
    return result;
}

Float CLocation::GetAccuracy(
    /* [in] */ IInterface* obj)
{
    AutoPtr<ILocation> location = (ILocation*)(obj);
    Float result;
    location->GetAccuracy(&result);
    return result;
}

Boolean CLocation::HasBearing(
    /* [in] */ IInterface* obj)
{
    AutoPtr<ILocation> location = (ILocation*)(obj);
    Boolean result;
    location->HasBearing(&result);
    return result;
}

Float CLocation::GetBearing(
    /* [in] */ IInterface* obj)
{
    AutoPtr<ILocation> location = (ILocation*)(obj);
    Float result;
    location->GetBearing(&result);
    return result;
}

Boolean CLocation::HasSpeed(
    /* [in] */ IInterface* obj)
{
    AutoPtr<ILocation> location = (ILocation*)(obj);
    Boolean result;
    location->HasSpeed(&result);
    return result;
}

Float CLocation::GetSpeed(
    /* [in] */ IInterface* obj)
{
    AutoPtr<ILocation> location = (ILocation*)(obj);
    Float result;
    location->GetSpeed(&result);
    return result;
}

Int64 CLocation::GetTime(
    /* [in] */ IInterface* obj)
{
    AutoPtr<ILocation> location = (ILocation*)(obj);
    Int64 result;
    location->GetTime(&result);
    return result;
}

ECode CLocation::ComputeDistanceAndBearing(
    /* [in] */ Double lat1,
    /* [in] */ Double lon1,
    /* [in] */ Double lat2,
    /* [in] */ Double lon2,
    /* [in] */ ArrayOf<Float>* results)
{
    // Based on http://www.ngs.noaa.gov/PUBS_LIB/inverse.pdf
    // using the "Inverse Formula" (section 4)

    Int32 MAXITERS = 20;
    // Convert lat/long to radians
    lat1 *= Elastos::Core::Math::DOUBLE_PI / 180.0;
    lat2 *= Elastos::Core::Math::DOUBLE_PI / 180.0;
    lon1 *= Elastos::Core::Math::DOUBLE_PI / 180.0;
    lon2 *= Elastos::Core::Math::DOUBLE_PI / 180.0;

    Double a = 6378137.0; // WGS84 major axis
    Double b = 6356752.3142; // WGS84 semi-major axis
    Double f = (a - b) / a;
    Double aSqMinusBSqOverBSq = (a * a - b * b) / (b * b);

    Double L = lon2 - lon1;
    Double A = 0.0;
    Double U1 = Elastos::Core::Math::Atan((1.0 - f) * Elastos::Core::Math::Tan(lat1));
    Double U2 = Elastos::Core::Math::Atan((1.0 - f) * Elastos::Core::Math::Tan(lat2));

    Double cosU1 = Elastos::Core::Math::Cos(U1);
    Double cosU2 = Elastos::Core::Math::Cos(U2);
    Double sinU1 = Elastos::Core::Math::Sin(U1);
    Double sinU2 = Elastos::Core::Math::Sin(U2);
    Double cosU1cosU2 = cosU1 * cosU2;
    Double sinU1sinU2 = sinU1 * sinU2;

    Double sigma = 0.0;
    Double deltaSigma = 0.0;
    Double cosSqAlpha = 0.0;
    Double cos2SM = 0.0;
    Double cosSigma = 0.0;
    Double sinSigma = 0.0;
    Double cosLambda = 0.0;
    Double sinLambda = 0.0;

    Double lambda = L; // initial guess
    for (Int32 iter = 0; iter < MAXITERS; iter++) {
        Double lambdaOrig = lambda;
        cosLambda = Elastos::Core::Math::Cos(lambda);
        sinLambda = Elastos::Core::Math::Sin(lambda);
        Double t1 = cosU2 * sinLambda;
        Double t2 = cosU1 * sinU2 - sinU1 * cosU2 * cosLambda;
        Double sinSqSigma = t1 * t1 + t2 * t2; // (14)
        sinSigma = Elastos::Core::Math::Sqrt(sinSqSigma);
        cosSigma = sinU1sinU2 + cosU1cosU2 * cosLambda; // (15)
        sigma = Elastos::Core::Math::Atan2(sinSigma, cosSigma); // (16)
        Double sinAlpha = (sinSigma == 0) ? 0.0 :
            cosU1cosU2 * sinLambda / sinSigma; // (17)
        cosSqAlpha = 1.0 - sinAlpha * sinAlpha;
        cos2SM = (cosSqAlpha == 0) ? 0.0 :
            cosSigma - 2.0 * sinU1sinU2 / cosSqAlpha; // (18)

        Double uSquared = cosSqAlpha * aSqMinusBSqOverBSq; // defn
        A = 1 + (uSquared / 16384.0) * // (3)
            (4096.0 + uSquared *
             (-768 + uSquared * (320.0 - 175.0 * uSquared)));
        Double B = (uSquared / 1024.0) * // (4)
            (256.0 + uSquared *
             (-128.0 + uSquared * (74.0 - 47.0 * uSquared)));
        Double C = (f / 16.0) *
            cosSqAlpha *
            (4.0 + f * (4.0 - 3.0 * cosSqAlpha)); // (10)
        Double cos2SMSq = cos2SM * cos2SM;
        deltaSigma = B * sinSigma * // (6)
            (cos2SM + (B / 4.0) *
             (cosSigma * (-1.0 + 2.0 * cos2SMSq) -
              (B / 6.0) * cos2SM *
              (-3.0 + 4.0 * sinSigma * sinSigma) *
              (-3.0 + 4.0 * cos2SMSq)));

        lambda = L +
            (1.0 - C) * f * sinAlpha *
            (sigma + C * sinSigma *
             (cos2SM + C * cosSigma *
              (-1.0 + 2.0 * cos2SM * cos2SM))); // (11)

        Double delta = (lambda - lambdaOrig) / lambda;
        if (Elastos::Core::Math::Abs(delta) < 1.0e-12) {
            break;
        }
    }

    Float distance = (Float) (b * A * (sigma - deltaSigma));
    (*results)[0] = distance;
    if (results->GetLength() > 1) {
        Float initialBearing = (Float) Elastos::Core::Math::Atan2(cosU2 * sinLambda,
            cosU1 * sinU2 - sinU1 * cosU2 * cosLambda);
        initialBearing *= 180.0 / Elastos::Core::Math::DOUBLE_PI;
        (*results)[1] = initialBearing;
        if (results->GetLength() > 2) {
            Float finalBearing = (Float) Elastos::Core::Math::Atan2(cosU1 * sinLambda,
                -sinU1 * cosU2 + cosU1 * sinU2 * cosLambda);
            finalBearing *= 180.0 / Elastos::Core::Math::DOUBLE_PI;
            (*results)[2] = finalBearing;
        }
    }

    return NOERROR;
}

} // namespace Location
} // namepsace Droid
} // namespace Elastos
