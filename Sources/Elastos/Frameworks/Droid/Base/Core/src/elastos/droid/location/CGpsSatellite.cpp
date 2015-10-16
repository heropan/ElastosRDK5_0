
#include "elastos/droid/location/CGpsSatellite.h"

namespace Elastos {
namespace Droid {
namespace Location {

CGpsSatellite::CGpsSatellite()
    : mValid(FALSE)
    , mHasEphemeris(FALSE)
    , mHasAlmanac(FALSE)
    , mUsedInFix(FALSE)
    , mPrn(0)
    , mSnr(0.0f)
    , mElevation(0.0f)
    , mAzimuth(0.0f)
{
}

ECode CGpsSatellite::constructor(
    /* [in] */ Int32 prn)
{
    mPrn = prn;
    return NOERROR;
}

/**
 * Used by {@link LocationManager#getGpsStatus} to copy LocationManager's
 * cached GpsStatus instance to the client's copy.
 */
ECode CGpsSatellite::SetStatus(
    /* [in] */ IGpsSatellite* satellite)
{
    VALIDATE_NOT_NULL(satellite);

    PFL_EX("DEBUG")
//    Boolean valid;
//    satellite->IsValid(&valid);
//    mValid = valid;

    Boolean hasEphemeris;
    satellite->HasEphemeris(&hasEphemeris);
    mHasEphemeris = hasEphemeris;

    Boolean hasAlmanac;
    satellite->HasAlmanac(&hasAlmanac);
    mHasAlmanac = hasAlmanac;

    Boolean usedInFix;
    satellite->UsedInFix(&usedInFix);
    mUsedInFix = usedInFix;

    Float snr;
    satellite->GetSnr(&snr);
    mSnr = snr;

    Float elevation;
    satellite->GetElevation(&elevation);
    mElevation = elevation;

    Float azimuth;
    satellite->GetAzimuth(&azimuth);
    mAzimuth = azimuth;

    return NOERROR;
}

/**
 * Returns the PRN (pseudo-random number) for the satellite.
 *
 * @return PRN number
 */
ECode CGpsSatellite::GetPrn(
    /* [out] */ Int32* prn)
{
    VALIDATE_NOT_NULL(prn);
    *prn = mPrn;
    return NOERROR;
}

ECode CGpsSatellite::SetPrn(
    /* [in] */ Int32 prn)
{
    mPrn = prn;
    return NOERROR;
}
/**
 * Returns the signal to noise ratio for the satellite.
 *
 * @return the signal to noise ratio
 */
ECode CGpsSatellite::GetSnr(
    /* [out] */ Float* snr)
{
    VALIDATE_NOT_NULL(snr);
    *snr = mSnr;
    return NOERROR;
}

ECode CGpsSatellite::SetSnr(
    /* [in] */ Float snr)
{
    mSnr = snr;
    return NOERROR;
}

/**
 * Returns the elevation of the satellite in degrees.
 * The elevation can vary between 0 and 90.
 *
 * @return the elevation in degrees
 */
ECode CGpsSatellite::GetElevation(
    /* [out] */ Float* elevation)
{
    VALIDATE_NOT_NULL(elevation);
    *elevation = mElevation;
    return NOERROR;
}

ECode CGpsSatellite::SetElevation(
    /* [in] */ Float elevation)
{
    mElevation = elevation;
    return NOERROR;
}

/**
 * Returns the azimuth of the satellite in degrees.
 * The azimuth can vary between 0 and 360.
 *
 * @return the azimuth in degrees
 */
ECode CGpsSatellite::GetAzimuth(
    /* [out] */ Float* azimuth)
{
    VALIDATE_NOT_NULL(azimuth);
    *azimuth = mAzimuth;
    return NOERROR;
}

ECode CGpsSatellite::SetAzimuth(
    /* [in] */ Float azimuth)
{
    mAzimuth = azimuth;
    return NOERROR;
}

/**
 * Returns true if the GPS engine has ephemeris data for the satellite.
 *
 * @return true if the satellite has ephemeris data
 */
ECode CGpsSatellite::HasEphemeris(
    /* [out] */ Boolean* hasEphemeris)
{
    VALIDATE_NOT_NULL(hasEphemeris);
    *hasEphemeris = mHasEphemeris;
    return NOERROR;
}

ECode CGpsSatellite::SetHasEphemeris(
    /* [in] */ Boolean hasEphemeris)
{
    mHasEphemeris = hasEphemeris;
    return NOERROR;
}

/**
 * Returns true if the GPS engine has almanac data for the satellite.
 *
 * @return true if the satellite has almanac data
 */
ECode CGpsSatellite::HasAlmanac(
    /* [out] */ Boolean* hasAlmanac)
{
    VALIDATE_NOT_NULL(hasAlmanac);
    *hasAlmanac = mHasAlmanac;
    return NOERROR;
}


ECode CGpsSatellite::SetHasAlmanac(
    /* [in] */ Boolean hasAlmanac)
{
    mHasAlmanac = hasAlmanac;
    return NOERROR;
}

/**
 * Returns true if the satellite was used by the GPS engine when
 * calculating the most recent GPS fix.
 *
 * @return true if the satellite was used to compute the most recent fix.
 */
ECode CGpsSatellite::UsedInFix(
    /* [out] */ Boolean* usedInFix)
{
    VALIDATE_NOT_NULL(usedInFix);
    *usedInFix = mUsedInFix;
    return NOERROR;
}


ECode CGpsSatellite::SetUsedInFix(
    /* [in] */ Boolean usedInFix)
{
    mUsedInFix = usedInFix;
    return NOERROR;
}

ECode CGpsSatellite::IsValid(
    /* [out] */ Boolean* valid)
{
    VALIDATE_NOT_NULL(valid);
    *valid = mValid;
    return NOERROR;
}

ECode CGpsSatellite::SetValid(
    /* [in] */ Boolean valid)
{
    mValid = valid;
    return NOERROR;
}

}//namespace Location
}//namespace Droid
}//namespace Elastos
