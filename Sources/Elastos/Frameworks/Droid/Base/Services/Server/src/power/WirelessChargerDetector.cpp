
#include "power/WirelessChargerDetector.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/Math.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Hardware::EIID_ISensorEventListener;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Power {

WirelessChargerDetector::MySensorEventListener::MySensorEventListener(
    /* [in] */ WirelessChargerDetector* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(WirelessChargerDetector::MySensorEventListener, ISensorEventListener)

ECode WirelessChargerDetector::MySensorEventListener::OnSensorChanged(
    /* [in] */ ISensorEvent* event)
{
    Int64 timestamp;
    event->GetTimestamp(&timestamp);
    AutoPtr< ArrayOf<Float> > values;
    event->GetValues((ArrayOf<Float>**)&values);
    mHost->ProcessSample(timestamp, (*values)[0], (*values)[1], (*values)[2]);
    return NOERROR;
}

ECode WirelessChargerDetector::MySensorEventListener::OnAccuracyChanged(
    /* [in] */ ISensor* sensor,
    /* [in] */ Int32 accuracy)
{
    return NOERROR;
}


const String WirelessChargerDetector::TAG("WirelessChargerDetector");
const Boolean WirelessChargerDetector::DEBUG;
const Int64 WirelessChargerDetector::NANOS_PER_MS;
const Int64 WirelessChargerDetector::SETTLE_TIME_NANOS;
const Int32 WirelessChargerDetector::MIN_SAMPLES;
const Int32 WirelessChargerDetector::WIRELESS_CHARGER_TURN_ON_BATTERY_LEVEL_LIMIT;
const Double WirelessChargerDetector::MOVEMENT_ANGLE_COS_THRESHOLD = Elastos::Core::Math::Cos(5 * 3.141592653589793/*Math::PI*/ / 180);
const Double WirelessChargerDetector::MIN_GRAVITY = ISensorManager::GRAVITY_EARTH - 1.0f;
const Double WirelessChargerDetector::MAX_GRAVITY = ISensorManager::GRAVITY_EARTH + 1.0f;

WirelessChargerDetector::WirelessChargerDetector(
    /* [in] */ ISensorManager* sensorManager,
    /* [in] */ ISuspendBlocker* suspendBlocker)
    : mSensorManager(sensorManager)
    , mSuspendBlocker(suspendBlocker)
    , mPoweredWirelessly(FALSE)
    , mAtRest(FALSE)
    , mRestX(0)
    , mRestY(0)
    , mRestZ(0)
    , mDetectionInProgress(FALSE)
    , mMustUpdateRestPosition(FALSE)
    , mTotalSamples(0)
    , mMovingSamples(0)
    , mFirstSampleTime(0)
    , mFirstSampleX(0)
    , mFirstSampleY(0)
    , mFirstSampleZ(0)
{
    sensorManager->GetDefaultSensor(ISensor::TYPE_GRAVITY, (ISensor**)&mGravitySensor);
}

// void dump(PrintWriter pw);

Boolean WirelessChargerDetector::Update(
    /* [in] */ Boolean isPowered,
    /* [in] */ Int32 plugType,
    /* [in] */ Int32 batteryLevel)
{
    AutoLock lock(mLock);

    Boolean wasPoweredWirelessly = mPoweredWirelessly;

    if (isPowered /*&& plugType == IBatteryManager::BATTERY_PLUGGED_WIRELESS*/) {
        // The device is receiving power from the wireless charger.
        // Update the rest position asynchronously.
        mPoweredWirelessly = TRUE;
        mMustUpdateRestPosition = TRUE;
        StartDetectionLocked();
    }
    else {
        // The device may or may not be on the wireless charger depending on whether
        // the unplug signal that we received was spurious.
        mPoweredWirelessly = FALSE;
        if (mAtRest) {
            if (plugType != 0 /*&& plugType != IBatteryManager::BATTERY_PLUGGED_WIRELESS*/) {
                // The device was plugged into a new non-wireless power source.
                // It's safe to assume that it is no longer on the wireless charger.
                mMustUpdateRestPosition = FALSE;
                ClearAtRestLocked();
            }
            else {
                // The device may still be on the wireless charger but we don't know.
                // Check whether the device has remained at rest on the charger
                // so that we will know to ignore the next wireless plug event
                // if needed.
                StartDetectionLocked();
            }
        }
    }

    // Report that the device has been docked only if the device just started
    // receiving power wirelessly, has a high enough battery level that we
    // can be assured that charging was not delayed due to the battery previously
    // having been full, and the device is not known to already be at rest
    // on the wireless charger from earlier.
    return mPoweredWirelessly && !wasPoweredWirelessly
            && batteryLevel < WIRELESS_CHARGER_TURN_ON_BATTERY_LEVEL_LIMIT
            && !mAtRest;
}

void WirelessChargerDetector::StartDetectionLocked()
{
    if (!mDetectionInProgress && mGravitySensor != NULL) {
        Boolean result;
        mSensorManager->RegisterListener(mListener, mGravitySensor,
                ISensorManager::SENSOR_DELAY_UI, &result);
        if (result) {
            mSuspendBlocker->Acquire();
            mDetectionInProgress = TRUE;
            mTotalSamples = 0;
            mMovingSamples = 0;
        }
    }
}

void WirelessChargerDetector::ProcessSample(
    /* [in] */ Int64 timeNanos,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float z)
{
    AutoLock lock(mLock);
    if (!mDetectionInProgress) {
        return;
    }

    mTotalSamples += 1;
    if (mTotalSamples == 1) {
        // Save information about the first sample collected.
        mFirstSampleTime = timeNanos;
        mFirstSampleX = x;
        mFirstSampleY = y;
        mFirstSampleZ = z;
    }
    else {
        // Determine whether movement has occurred relative to the first sample.
        if (HasMoved(mFirstSampleX, mFirstSampleY, mFirstSampleZ, x, y, z)) {
            mMovingSamples += 1;
        }
    }

    // Clear the at rest flag if movement has occurred relative to the rest sample.
    if (mAtRest && HasMoved(mRestX, mRestY, mRestZ, x, y, z)) {
        if (DEBUG) {
            Slogger::D(TAG, "No longer at rest: mRestX=%f, mRestY=%f, mRestZ=%f, x=%f, y=%f, z=%f"
                    , mRestX, mRestY, mRestZ, x, y, z);
        }
        ClearAtRestLocked();
    }

    // Save the result when done.
    if (timeNanos - mFirstSampleTime >= SETTLE_TIME_NANOS
            && mTotalSamples >= MIN_SAMPLES) {
        mSensorManager->UnregisterListener(mListener);
        if (mMustUpdateRestPosition) {
            if (mMovingSamples == 0) {
                mAtRest = TRUE;
                mRestX = x;
                mRestY = y;
                mRestZ = z;
            }
            else {
                ClearAtRestLocked();
            }
            mMustUpdateRestPosition = FALSE;
        }
        mDetectionInProgress = FALSE;
        mSuspendBlocker->Release();

        if (DEBUG) {
            Slogger::D(TAG, "New state: mAtRest=%f, mRestX=%f, mRestY=%f, mRestZ=%f, mTotalSamples=%d, mMovingSamples=%d"
                    , mAtRest, mRestX, mRestY, mRestZ, mTotalSamples, mMovingSamples);
        }
    }
}

void WirelessChargerDetector::ClearAtRestLocked()
{
    mAtRest = FALSE;
    mRestX = 0;
    mRestY = 0;
    mRestZ = 0;
}

Boolean WirelessChargerDetector::HasMoved(
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Float z1,
    /* [in] */ Float x2,
    /* [in] */ Float y2,
    /* [in] */ Float z2)
{
    Double dotProduct = (x1 * x2) + (y1 * y2) + (z1 * z2);
    Double mag1 = Elastos::Core::Math::Sqrt((x1 * x1) + (y1 * y1) + (z1 * z1));
    Double mag2 = Elastos::Core::Math::Sqrt((x2 * x2) + (y2 * y2) + (z2 * z2));
    if (mag1 < MIN_GRAVITY || mag1 > MAX_GRAVITY
            || mag2 < MIN_GRAVITY || mag2 > MAX_GRAVITY) {
        if (DEBUG) {
            Slogger::D(TAG, "Weird gravity vector: mag1=%d, mag2=%d", mag1, mag2);
        }
        return TRUE;
    }
    Boolean moved = (dotProduct < mag1 * mag2 * MOVEMENT_ANGLE_COS_THRESHOLD);
    if (DEBUG) {
        Slogger::D(TAG, "Check: moved=%d, x1=%f, y1=%f, z1=%f, x2=%f, y2=%f, z2=%f, angle=%d, dotProduct=%d, mag1=%d, mag2=%d"
                , moved, x1, y1, z1, x2, y2, z2, (Elastos::Core::Math::Acos(dotProduct / mag1 / mag2) * 180 / 3.1415926/*Math.PI*/)
                , dotProduct, mag1, mag2);
    }
    return moved;
}

} // namespace Power
} // namespace Server
} // namespace Droid
} // namespace Elastos
