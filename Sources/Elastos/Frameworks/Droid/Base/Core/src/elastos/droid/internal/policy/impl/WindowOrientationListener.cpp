
#include "elastos/droid/internal/policy/impl/WindowOrientationListener.h"
#include "elastos/droid/os/SystemProperties.h"
#include "elastos/core/Math.h"

using Elastos::Droid::Hardware::EIID_ISensorEventListener;
using Elastos::Droid::Os::SystemProperties;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

//=====================================================================
//          WindowOrientationListener::SensorEventListenerImpl
//=====================================================================
CAR_INTERFACE_IMPL(WindowOrientationListener::SensorEventListenerImpl, Object, ISensorEventListener)

const Float WindowOrientationListener::SensorEventListenerImpl::RADIANS_TO_DEGREES = (Float) (180 / Elastos::Core::Math::PI);
const Int64 WindowOrientationListener::SensorEventListenerImpl::NANOS_PER_MS;
const Int32 WindowOrientationListener::SensorEventListenerImpl::ACCELEROMETER_DATA_X;
const Int32 WindowOrientationListener::SensorEventListenerImpl::ACCELEROMETER_DATA_Y;
const Int32 WindowOrientationListener::SensorEventListenerImpl::ACCELEROMETER_DATA_Z;
const Int64 WindowOrientationListener::SensorEventListenerImpl::PROPOSAL_SETTLE_TIME_NANOS;
const Int64 WindowOrientationListener::SensorEventListenerImpl::PROPOSAL_MIN_TIME_SINCE_FLAT_ENDED_NANOS;
const Int64 WindowOrientationListener::SensorEventListenerImpl::PROPOSAL_MIN_TIME_SINCE_SWING_ENDED_NANOS;
const Int64 WindowOrientationListener::SensorEventListenerImpl::PROPOSAL_MIN_TIME_SINCE_ACCELERATION_ENDED_NANOS;
const Float WindowOrientationListener::SensorEventListenerImpl::FLAT_ANGLE = 75;
const Int64 WindowOrientationListener::SensorEventListenerImpl::FLAT_TIME_NANOS;
const Float WindowOrientationListener::SensorEventListenerImpl::SWING_AWAY_ANGLE_DELTA = 20;
const Int64 WindowOrientationListener::SensorEventListenerImpl::SWING_TIME_NANOS;
const Int64 WindowOrientationListener::SensorEventListenerImpl::MAX_FILTER_DELTA_TIME_NANOS;
const Float WindowOrientationListener::SensorEventListenerImpl::FILTER_TIME_CONSTANT_MS = 200.0f;
const Float WindowOrientationListener::SensorEventListenerImpl::NEAR_ZERO_MAGNITUDE = 1;
const Float WindowOrientationListener::SensorEventListenerImpl::ACCELERATION_TOLERANCE = 4;
const Float WindowOrientationListener::SensorEventListenerImpl::MIN_ACCELERATION_MAGNITUDE = ISensorManager::STANDARD_GRAVITY - ACCELERATION_TOLERANCE;
const Float WindowOrientationListener::SensorEventListenerImpl::MAX_ACCELERATION_MAGNITUDE = ISensorManager::STANDARD_GRAVITY + ACCELERATION_TOLERANCE;
const Int32 WindowOrientationListener::SensorEventListenerImpl::MAX_TILT;
const Int32 WindowOrientationListener::SensorEventListenerImpl::ADJACENT_ORIENTATION_ANGLE_GAP;
const Int32 WindowOrientationListener::SensorEventListenerImpl::TILT_HISTORY_SIZE;

ECode WindowOrientationListener::SensorEventListenerImpl::GetProposedRotationLocked(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mProposedRotation;
    assert(0);
    return NOERROR;
}

ECode WindowOrientationListener::SensorEventListenerImpl::DumpLocked(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ const String& prefix)
{
    VALIDATE_NOT_NULL(pw);
    // ==================before translated======================
    // pw.println(prefix + "mProposedRotation=" + mProposedRotation);
    // pw.println(prefix + "mPredictedRotation=" + mPredictedRotation);
    // pw.println(prefix + "mLastFilteredX=" + mLastFilteredX);
    // pw.println(prefix + "mLastFilteredY=" + mLastFilteredY);
    // pw.println(prefix + "mLastFilteredZ=" + mLastFilteredZ);
    // pw.println(prefix + "mTiltHistory={last: " + getLastTiltLocked() + "}");
    // pw.println(prefix + "mFlat=" + mFlat);
    // pw.println(prefix + "mSwinging=" + mSwinging);
    // pw.println(prefix + "mAccelerating=" + mAccelerating);
    // pw.println(prefix + "mOverhead=" + mOverhead);
    assert(0);
    return NOERROR;
}

ECode WindowOrientationListener::SensorEventListenerImpl::OnAccuracyChanged(
    /* [in] */ ISensor* sensor,
    /* [in] */ Int32 accuracy)
{
    VALIDATE_NOT_NULL(sensor);
    assert(0);
    return NOERROR;
}

ECode WindowOrientationListener::SensorEventListenerImpl::OnSensorChanged(
    /* [in] */ ISensorEvent* event)
{
    VALIDATE_NOT_NULL(event);
    // ==================before translated======================
    // int proposedRotation;
    // int oldProposedRotation;
    //
    // synchronized (mLock) {
    //     // The vector given in the SensorEvent points straight up (towards the sky) under
    //     // ideal conditions (the phone is not accelerating).  I'll call this up vector
    //     // elsewhere.
    //     float x = event.values[ACCELEROMETER_DATA_X];
    //     float y = event.values[ACCELEROMETER_DATA_Y];
    //     float z = event.values[ACCELEROMETER_DATA_Z];
    //
    //     if (LOG) {
    //         Slog.v(TAG, "Raw acceleration vector: "
    //                 + "x=" + x + ", y=" + y + ", z=" + z
    //                 + ", magnitude=" + FloatMath.sqrt(x * x + y * y + z * z));
    //     }
    //
    //     // Apply a low-pass filter to the acceleration up vector in cartesian space.
    //     // Reset the orientation listener state if the samples are too far apart in time
    //     // or when we see values of (0, 0, 0) which indicates that we polled the
    //     // accelerometer too soon after turning it on and we don't have any data yet.
    //     final long now = event.timestamp;
    //     final long then = mLastFilteredTimestampNanos;
    //     final float timeDeltaMS = (now - then) * 0.000001f;
    //     final boolean skipSample;
    //     if (now < then
    //             || now > then + MAX_FILTER_DELTA_TIME_NANOS
    //             || (x == 0 && y == 0 && z == 0)) {
    //         if (LOG) {
    //             Slog.v(TAG, "Resetting orientation listener.");
    //         }
    //         resetLocked();
    //         skipSample = true;
    //     } else {
    //         final float alpha = timeDeltaMS / (FILTER_TIME_CONSTANT_MS + timeDeltaMS);
    //         x = alpha * (x - mLastFilteredX) + mLastFilteredX;
    //         y = alpha * (y - mLastFilteredY) + mLastFilteredY;
    //         z = alpha * (z - mLastFilteredZ) + mLastFilteredZ;
    //         if (LOG) {
    //             Slog.v(TAG, "Filtered acceleration vector: "
    //                     + "x=" + x + ", y=" + y + ", z=" + z
    //                     + ", magnitude=" + FloatMath.sqrt(x * x + y * y + z * z));
    //         }
    //         skipSample = false;
    //     }
    //     mLastFilteredTimestampNanos = now;
    //     mLastFilteredX = x;
    //     mLastFilteredY = y;
    //     mLastFilteredZ = z;
    //
    //     boolean isAccelerating = false;
    //     boolean isFlat = false;
    //     boolean isSwinging = false;
    //     if (!skipSample) {
    //         // Calculate the magnitude of the acceleration vector.
    //         final float magnitude = FloatMath.sqrt(x * x + y * y + z * z);
    //         if (magnitude < NEAR_ZERO_MAGNITUDE) {
    //             if (LOG) {
    //                 Slog.v(TAG, "Ignoring sensor data, magnitude too close to zero.");
    //             }
    //             clearPredictedRotationLocked();
    //         } else {
    //             // Determine whether the device appears to be undergoing external
    //             // acceleration.
    //             if (isAcceleratingLocked(magnitude)) {
    //                 isAccelerating = true;
    //                 mAccelerationTimestampNanos = now;
    //             }
    //
    //             // Calculate the tilt angle.
    //             // This is the angle between the up vector and the x-y plane (the plane of
    //             // the screen) in a range of [-90, 90] degrees.
    //             //   -90 degrees: screen horizontal and facing the ground (overhead)
    //             //     0 degrees: screen vertical
    //             //    90 degrees: screen horizontal and facing the sky (on table)
    //             final int tiltAngle = (int) Math.round(
    //                     Math.asin(z / magnitude) * RADIANS_TO_DEGREES);
    //             addTiltHistoryEntryLocked(now, tiltAngle);
    //
    //             // Determine whether the device appears to be flat or swinging.
    //             if (isFlatLocked(now)) {
    //                 isFlat = true;
    //                 mFlatTimestampNanos = now;
    //             }
    //             if (isSwingingLocked(now, tiltAngle)) {
    //                 isSwinging = true;
    //                 mSwingTimestampNanos = now;
    //             }
    //
    //             // If the tilt angle is too close to horizontal then we cannot determine
    //             // the orientation angle of the screen.
    //             if (tiltAngle <= TILT_OVERHEAD_ENTER) {
    //                 mOverhead = true;
    //             } else if (tiltAngle >= TILT_OVERHEAD_EXIT) {
    //                 mOverhead = false;
    //             }
    //             if (mOverhead) {
    //                 if (LOG) {
    //                     Slog.v(TAG, "Ignoring sensor data, device is overhead: "
    //                             + "tiltAngle=" + tiltAngle);
    //                 }
    //                 clearPredictedRotationLocked();
    //             } else if (Math.abs(tiltAngle) > MAX_TILT) {
    //                 if (LOG) {
    //                     Slog.v(TAG, "Ignoring sensor data, tilt angle too high: "
    //                             + "tiltAngle=" + tiltAngle);
    //                 }
    //                 clearPredictedRotationLocked();
    //             } else {
    //                 // Calculate the orientation angle.
    //                 // This is the angle between the x-y projection of the up vector onto
    //                 // the +y-axis, increasing clockwise in a range of [0, 360] degrees.
    //                 int orientationAngle = (int) Math.round(
    //                         -Math.atan2(-x, y) * RADIANS_TO_DEGREES);
    //                 if (orientationAngle < 0) {
    //                     // atan2 returns [-180, 180]; normalize to [0, 360]
    //                     orientationAngle += 360;
    //                 }
    //
    //                 // Find the nearest rotation.
    //                 int nearestRotation = (orientationAngle + 45) / 90;
    //                 if (nearestRotation == 4) {
    //                     nearestRotation = 0;
    //                 }
    //
    //                 // Determine the predicted orientation.
    //                 if (isTiltAngleAcceptableLocked(nearestRotation, tiltAngle)
    //                         && isOrientationAngleAcceptableLocked(nearestRotation,
    //                                 orientationAngle)) {
    //                     updatePredictedRotationLocked(now, nearestRotation);
    //                     if (LOG) {
    //                         Slog.v(TAG, "Predicted: "
    //                                 + "tiltAngle=" + tiltAngle
    //                                 + ", orientationAngle=" + orientationAngle
    //                                 + ", predictedRotation=" + mPredictedRotation
    //                                 + ", predictedRotationAgeMS="
    //                                         + ((now - mPredictedRotationTimestampNanos)
    //                                                 * 0.000001f));
    //                     }
    //                 } else {
    //                     if (LOG) {
    //                         Slog.v(TAG, "Ignoring sensor data, no predicted rotation: "
    //                                 + "tiltAngle=" + tiltAngle
    //                                 + ", orientationAngle=" + orientationAngle);
    //                     }
    //                     clearPredictedRotationLocked();
    //                 }
    //             }
    //         }
    //     }
    //     mFlat = isFlat;
    //     mSwinging = isSwinging;
    //     mAccelerating = isAccelerating;
    //
    //     // Determine new proposed rotation.
    //     oldProposedRotation = mProposedRotation;
    //     if (mPredictedRotation < 0 || isPredictedRotationAcceptableLocked(now)) {
    //         mProposedRotation = mPredictedRotation;
    //     }
    //     proposedRotation = mProposedRotation;
    //
    //     // Write final statistics about where we are in the orientation detection process.
    //     if (LOG) {
    //         Slog.v(TAG, "Result: currentRotation=" + mCurrentRotation
    //                 + ", proposedRotation=" + proposedRotation
    //                 + ", predictedRotation=" + mPredictedRotation
    //                 + ", timeDeltaMS=" + timeDeltaMS
    //                 + ", isAccelerating=" + isAccelerating
    //                 + ", isFlat=" + isFlat
    //                 + ", isSwinging=" + isSwinging
    //                 + ", isOverhead=" + mOverhead
    //                 + ", timeUntilSettledMS=" + remainingMS(now,
    //                         mPredictedRotationTimestampNanos + PROPOSAL_SETTLE_TIME_NANOS)
    //                 + ", timeUntilAccelerationDelayExpiredMS=" + remainingMS(now,
    //                         mAccelerationTimestampNanos + PROPOSAL_MIN_TIME_SINCE_ACCELERATION_ENDED_NANOS)
    //                 + ", timeUntilFlatDelayExpiredMS=" + remainingMS(now,
    //                         mFlatTimestampNanos + PROPOSAL_MIN_TIME_SINCE_FLAT_ENDED_NANOS)
    //                 + ", timeUntilSwingDelayExpiredMS=" + remainingMS(now,
    //                         mSwingTimestampNanos + PROPOSAL_MIN_TIME_SINCE_SWING_ENDED_NANOS));
    //     }
    // }
    //
    // // Tell the listener.
    // if (proposedRotation != oldProposedRotation && proposedRotation >= 0) {
    //     if (LOG) {
    //         Slog.v(TAG, "Proposed rotation changed!  proposedRotation=" + proposedRotation
    //                 + ", oldProposedRotation=" + oldProposedRotation);
    //     }
    //     onProposedRotationChanged(proposedRotation);
    // }
    assert(0);
    return NOERROR;
}

AutoPtr< ArrayOf< AutoPtr< ArrayOf<Int32> > > > WindowOrientationListener::SensorEventListenerImpl::MiddleInitTiltTolerance()
{
    // ==================before translated======================
    // int[][] result = new int[][] {
    //              /* ROTATION_0   */ { -25, 70 },
    //              /* ROTATION_90  */ { -25, 65 },
    //              /* ROTATION_180 */ { -25, 60 },
    //              /* ROTATION_270 */ { -25, 65 }
    //          };
    assert(0);
    AutoPtr< ArrayOf< AutoPtr< ArrayOf<Int32> > > > empty;
    return empty;
}

AutoPtr< ArrayOf<Float> > WindowOrientationListener::SensorEventListenerImpl::MiddleInitMtilthistory()
{
    // ==================before translated======================
    // float[] result = new float[TILT_HISTORY_SIZE];
    assert(0);
    AutoPtr< ArrayOf<Float> > empty;
    return empty;
}

AutoPtr< ArrayOf<Int64> > WindowOrientationListener::SensorEventListenerImpl::MiddleInitMtilthistorytimestampnanos()
{
    // ==================before translated======================
    // long[] result = new long[TILT_HISTORY_SIZE];
    assert(0);
    AutoPtr< ArrayOf<Int64> > empty;
    return empty;
}

Boolean WindowOrientationListener::SensorEventListenerImpl::IsTiltAngleAcceptableLocked(
    /* [in] */ Int32 rotation,
    /* [in] */ Int32 tiltAngle)
{
    // ==================before translated======================
    // return tiltAngle >= TILT_TOLERANCE[rotation][0]
    //         && tiltAngle <= TILT_TOLERANCE[rotation][1];
    assert(0);
    return FALSE;
}

Boolean WindowOrientationListener::SensorEventListenerImpl::IsOrientationAngleAcceptableLocked(
    /* [in] */ Int32 rotation,
    /* [in] */ Int32 orientationAngle)
{
    // ==================before translated======================
    // // If there is no current rotation, then there is no gap.
    // // The gap is used only to introduce hysteresis among advertised orientation
    // // changes to avoid flapping.
    // final int currentRotation = mCurrentRotation;
    // if (currentRotation >= 0) {
    //     // If the specified rotation is the same or is counter-clockwise adjacent
    //     // to the current rotation, then we set a lower bound on the orientation angle.
    //     // For example, if currentRotation is ROTATION_0 and proposed is ROTATION_90,
    //     // then we want to check orientationAngle > 45 + GAP / 2.
    //     if (rotation == currentRotation
    //             || rotation == (currentRotation + 1) % 4) {
    //         int lowerBound = rotation * 90 - 45
    //                 + ADJACENT_ORIENTATION_ANGLE_GAP / 2;
    //         if (rotation == 0) {
    //             if (orientationAngle >= 315 && orientationAngle < lowerBound + 360) {
    //                 return false;
    //             }
    //         } else {
    //             if (orientationAngle < lowerBound) {
    //                 return false;
    //             }
    //         }
    //     }
    //
    //     // If the specified rotation is the same or is clockwise adjacent,
    //     // then we set an upper bound on the orientation angle.
    //     // For example, if currentRotation is ROTATION_0 and rotation is ROTATION_270,
    //     // then we want to check orientationAngle < 315 - GAP / 2.
    //     if (rotation == currentRotation
    //             || rotation == (currentRotation + 3) % 4) {
    //         int upperBound = rotation * 90 + 45
    //                 - ADJACENT_ORIENTATION_ANGLE_GAP / 2;
    //         if (rotation == 0) {
    //             if (orientationAngle <= 45 && orientationAngle > upperBound) {
    //                 return false;
    //             }
    //         } else {
    //             if (orientationAngle > upperBound) {
    //                 return false;
    //             }
    //         }
    //     }
    // }
    // return true;
    assert(0);
    return FALSE;
}

Boolean WindowOrientationListener::SensorEventListenerImpl::IsPredictedRotationAcceptableLocked(
    /* [in] */ Int64 now)
{
    // ==================before translated======================
    // // The predicted rotation must have settled long enough.
    // if (now < mPredictedRotationTimestampNanos + PROPOSAL_SETTLE_TIME_NANOS) {
    //     return false;
    // }
    //
    // // The last flat state (time since picked up) must have been sufficiently long ago.
    // if (now < mFlatTimestampNanos + PROPOSAL_MIN_TIME_SINCE_FLAT_ENDED_NANOS) {
    //     return false;
    // }
    //
    // // The last swing state (time since last movement to put down) must have been
    // // sufficiently long ago.
    // if (now < mSwingTimestampNanos + PROPOSAL_MIN_TIME_SINCE_SWING_ENDED_NANOS) {
    //     return false;
    // }
    //
    // // The last acceleration state must have been sufficiently long ago.
    // if (now < mAccelerationTimestampNanos
    //         + PROPOSAL_MIN_TIME_SINCE_ACCELERATION_ENDED_NANOS) {
    //     return false;
    // }
    //
    // // Looks good!
    // return true;
    assert(0);
    return FALSE;
}

void WindowOrientationListener::SensorEventListenerImpl::ResetLocked()
{
    // ==================before translated======================
    // mLastFilteredTimestampNanos = Long.MIN_VALUE;
    // mProposedRotation = -1;
    // mFlatTimestampNanos = Long.MIN_VALUE;
    // mFlat = false;
    // mSwingTimestampNanos = Long.MIN_VALUE;
    // mSwinging = false;
    // mAccelerationTimestampNanos = Long.MIN_VALUE;
    // mAccelerating = false;
    // mOverhead = false;
    // clearPredictedRotationLocked();
    // clearTiltHistoryLocked();
    assert(0);
}

void WindowOrientationListener::SensorEventListenerImpl::ClearPredictedRotationLocked()
{
    // ==================before translated======================
    // mPredictedRotation = -1;
    // mPredictedRotationTimestampNanos = Long.MIN_VALUE;
    assert(0);
}

void WindowOrientationListener::SensorEventListenerImpl::UpdatePredictedRotationLocked(
    /* [in] */ Int64 now,
    /* [in] */ Int32 rotation)
{
    // ==================before translated======================
    // if (mPredictedRotation != rotation) {
    //     mPredictedRotation = rotation;
    //     mPredictedRotationTimestampNanos = now;
    // }
    assert(0);
}

Boolean WindowOrientationListener::SensorEventListenerImpl::IsAcceleratingLocked(
    /* [in] */ Float magnitude)
{
    // ==================before translated======================
    // return magnitude < MIN_ACCELERATION_MAGNITUDE
    //         || magnitude > MAX_ACCELERATION_MAGNITUDE;
    assert(0);
    return FALSE;
}

void WindowOrientationListener::SensorEventListenerImpl::ClearTiltHistoryLocked()
{
    // ==================before translated======================
    // mTiltHistoryTimestampNanos[0] = Long.MIN_VALUE;
    // mTiltHistoryIndex = 1;
    assert(0);
}

void WindowOrientationListener::SensorEventListenerImpl::AddTiltHistoryEntryLocked(
    /* [in] */ Int64 now,
    /* [in] */ Float tilt)
{
    // ==================before translated======================
    // mTiltHistory[mTiltHistoryIndex] = tilt;
    // mTiltHistoryTimestampNanos[mTiltHistoryIndex] = now;
    // mTiltHistoryIndex = (mTiltHistoryIndex + 1) % TILT_HISTORY_SIZE;
    // mTiltHistoryTimestampNanos[mTiltHistoryIndex] = Long.MIN_VALUE;
    assert(0);
}

Boolean WindowOrientationListener::SensorEventListenerImpl::IsFlatLocked(
    /* [in] */ Int64 now)
{
    // ==================before translated======================
    // for (int i = mTiltHistoryIndex; (i = nextTiltHistoryIndexLocked(i)) >= 0; ) {
    //     if (mTiltHistory[i] < FLAT_ANGLE) {
    //         break;
    //     }
    //     if (mTiltHistoryTimestampNanos[i] + FLAT_TIME_NANOS <= now) {
    //         // Tilt has remained greater than FLAT_TILT_ANGLE for FLAT_TIME_NANOS.
    //         return true;
    //     }
    // }
    // return false;
    assert(0);
    return FALSE;
}

Boolean WindowOrientationListener::SensorEventListenerImpl::IsSwingingLocked(
    /* [in] */ Int64 now,
    /* [in] */ Float tilt)
{
    // ==================before translated======================
    // for (int i = mTiltHistoryIndex; (i = nextTiltHistoryIndexLocked(i)) >= 0; ) {
    //     if (mTiltHistoryTimestampNanos[i] + SWING_TIME_NANOS < now) {
    //         break;
    //     }
    //     if (mTiltHistory[i] + SWING_AWAY_ANGLE_DELTA <= tilt) {
    //         // Tilted away by SWING_AWAY_ANGLE_DELTA within SWING_TIME_NANOS.
    //         return true;
    //     }
    // }
    // return false;
    assert(0);
    return FALSE;
}

Int32 WindowOrientationListener::SensorEventListenerImpl::NextTiltHistoryIndexLocked(
    /* [in] */ Int32 index)
{
    // ==================before translated======================
    // index = (index == 0 ? TILT_HISTORY_SIZE : index) - 1;
    // return mTiltHistoryTimestampNanos[index] != Long.MIN_VALUE ? index : -1;
    assert(0);
    return 0;
}

Float WindowOrientationListener::SensorEventListenerImpl::GetLastTiltLocked()
{
    // ==================before translated======================
    // int index = nextTiltHistoryIndexLocked(mTiltHistoryIndex);
    // return index >= 0 ? mTiltHistory[index] : Float.NaN;
    assert(0);
    return 0.0f;
}

Float WindowOrientationListener::SensorEventListenerImpl::RemainingMS(
    /* [in] */ Int64 now,
    /* [in] */ Int64 until)
{
    // ==================before translated======================
    // return now >= until ? 0 : (until - now) * 0.000001f;
    assert(0);
    return 0.0f;
}

//=====================================================================
//                      WindowOrientationListener
//=====================================================================
static Boolean InitDefaultLOG()
{
    Boolean result = FALSE;
    SystemProperties::GetBoolean(String("debug.orientation.log"), FALSE, &result);
    return result;
}

CAR_INTERFACE_IMPL(WindowOrientationListener, Object, IWindowOrientationListener)

const String WindowOrientationListener::TAG("WindowOrientationListener");
const Boolean WindowOrientationListener::LOG = InitDefaultLOG();
//SystemProperties.getBoolean("debug.orientation.log", false);
const Boolean WindowOrientationListener::USE_GRAVITY_SENSOR = false;

WindowOrientationListener::WindowOrientationListener()
{
}

ECode WindowOrientationListener::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IHandler* handler)
{
    // ==================before translated======================
    // this(context, handler, SensorManager.SENSOR_DELAY_UI);
    return NOERROR;
}

ECode WindowOrientationListener::Enable()
{
    // ==================before translated======================
    // synchronized (mLock) {
    //     if (mSensor == null) {
    //         Log.w(TAG, "Cannot detect sensors. Not enabled");
    //         return;
    //     }
    //     if (mEnabled == false) {
    //         if (LOG) {
    //             Log.d(TAG, "WindowOrientationListener enabled");
    //         }
    //         mSensorEventListener.resetLocked();
    //         mSensorManager.registerListener(mSensorEventListener, mSensor, mRate, mHandler);
    //         mEnabled = true;
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode WindowOrientationListener::Disable()
{
    // ==================before translated======================
    // synchronized (mLock) {
    //     if (mSensor == null) {
    //         Log.w(TAG, "Cannot detect sensors. Invalid disable");
    //         return;
    //     }
    //     if (mEnabled == true) {
    //         if (LOG) {
    //             Log.d(TAG, "WindowOrientationListener disabled");
    //         }
    //         mSensorManager.unregisterListener(mSensorEventListener);
    //         mEnabled = false;
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode WindowOrientationListener::SetCurrentRotation(
    /* [in] */ Int32 rotation)
{
    // ==================before translated======================
    // synchronized (mLock) {
    //     mCurrentRotation = rotation;
    // }
    assert(0);
    return NOERROR;
}

ECode WindowOrientationListener::GetProposedRotation(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // synchronized (mLock) {
    //     if (mEnabled) {
    //         return mSensorEventListener.getProposedRotationLocked();
    //     }
    //     return -1;
    // }
    assert(0);
    return NOERROR;
}

ECode WindowOrientationListener::CanDetectOrientation(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // synchronized (mLock) {
    //     return mSensor != null;
    // }
    assert(0);
    return NOERROR;
}

ECode WindowOrientationListener::Dump(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ const String& prefix)
{
    VALIDATE_NOT_NULL(pw);
    // ==================before translated======================
    // synchronized (mLock) {
    //     pw.println(prefix + TAG);
    //     prefix += "  ";
    //     pw.println(prefix + "mEnabled=" + mEnabled);
    //     pw.println(prefix + "mCurrentRotation=" + mCurrentRotation);
    //     pw.println(prefix + "mSensor=" + mSensor);
    //     pw.println(prefix + "mRate=" + mRate);
    //
    //     if (mSensorEventListener != null) {
    //         mSensorEventListener.dumpLocked(pw, prefix);
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode WindowOrientationListener::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IHandler* handler,
    /* [in] */ Int32 rate)
{
    // ==================before translated======================
    // mHandler = handler;
    // mSensorManager = (SensorManager)context.getSystemService(Context.SENSOR_SERVICE);
    // mRate = rate;
    // mSensor = mSensorManager.getDefaultSensor(USE_GRAVITY_SENSOR
    //         ? Sensor.TYPE_GRAVITY : Sensor.TYPE_ACCELEROMETER);
    // if (mSensor != null) {
    //     // Create listener only if sensors do exist
    //     mSensorEventListener = new SensorEventListenerImpl();
    // }
    return NOERROR;
}

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos


