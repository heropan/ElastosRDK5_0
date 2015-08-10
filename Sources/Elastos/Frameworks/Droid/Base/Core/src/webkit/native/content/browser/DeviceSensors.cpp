
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//===============================================================
//            DeviceSensors::SensorManagerProxyImpl
//===============================================================

DeviceSensors::SensorManagerProxyImpl::SensorManagerProxyImpl(
    /* [in] */ ISensorManager* sensorManager)
    : mSensorManager(sensorManager)
{
}

//@Override
Boolean DeviceSensors::SensorManagerProxyImpl::RegisterListener(
    /* [in] */ ISensorEventListener* listener,
    /* [in] */ Int32 sensorType,
    /* [in] */ Int32 rate,
    /* [in] */ IHandler* handler)
{
    List<Sensor> sensors = mSensorManager.getSensorList(sensorType);
    if (sensors.isEmpty()) {
        return false;
    }
    return mSensorManager.registerListener(listener, sensors.get(0), rate, handler);
}

//@Override
void DeviceSensors::SensorManagerProxyImpl::UnregisterListener(
    /* [in] */ ISensorEventListener* listener,
    /* [in] */ Int32 sensorType)
{
    List<Sensor> sensors = mSensorManager.getSensorList(sensorType);
    if (!sensors.isEmpty()) {
        mSensorManager.unregisterListener(listener, sensors.get(0));
    }
}

//===============================================================
//                DeviceSensors::InnerCallable
//===============================================================

DeviceSensors::InnerCallable::InnerCallable(
    /* [in] */ DeviceSensors* owner)
    : mOwner(owner)
{
}

ECode DeviceSensors::InnerCallable::Call(
    /* [out] */ IInterface** result)
{
    return mOwner->mAppContext->GetSystemService(IContext::SENSOR_SERVICE, result);
}

//===============================================================
//                     DeviceSensors
//===============================================================

const String DeviceSensors::TAG("DeviceMotionAndOrientation");
// The only instance of that class and its associated lock.
AutoPtr<DeviceSensors> DeviceSensors::sSingleton;
Object DeviceSensors::sSingletonLock;

/**
 * constants for using in JNI calls, also see
 * content/browser/device_sensors/sensor_manager_android.cc
 */
const Int32 DeviceSensors::DEVICE_ORIENTATION;
const Int32 DeviceSensors::DEVICE_MOTION;

const Set<Integer> DeviceSensors::DEVICE_ORIENTATION_SENSORS = CollectionUtil.newHashSet(
        Sensor.TYPE_ROTATION_VECTOR);

const Set<Integer> DeviceSensors::DEVICE_MOTION_SENSORS = CollectionUtil.newHashSet(
        Sensor.TYPE_ACCELEROMETER,
        Sensor.TYPE_LINEAR_ACCELERATION,
        Sensor.TYPE_GYROSCOPE);

DeviceSensors::DeviceSensors(
    /* [in] */ IContext* context)
    : mNativePtr(0)
    , mDeviceMotionIsActive(FALSE)
    , mDeviceOrientationIsActive(FALSE)
{
    context->GetApplicationContext((IContext**)&mAppContext);
}

/**
 * Start listening for sensor events. If this object is already listening
 * for events, the old callback is unregistered first.
 *
 * @param nativePtr Value to pass to nativeGotOrientation() for each event.
 * @param rateInMilliseconds Requested callback rate in milliseconds. The
 *            actual rate may be higher. Unwanted events should be ignored.
 * @param eventType Type of event to listen to, can be either DEVICE_ORIENTATION or
 *                  DEVICE_MOTION.
 * @return True on success.
 */
//@CalledByNative
Boolean DeviceSensors::Start(
    /* [in] */ Int64 nativePtr,
    /* [in] */ Int32 eventType,
    /* [in] */ Int32 rateInMilliseconds)
{
    Boolean success = FALSE;

    {
        Object::Autolock lock(mNativePtrLock);
        switch (eventType) {
            case DEVICE_ORIENTATION:
                success = RegisterSensors(DEVICE_ORIENTATION_SENSORS, rateInMilliseconds,
                        TRUE);
                break;
            case DEVICE_MOTION:
                // note: device motion spec does not require all sensors to be available
                success = RegisterSensors(DEVICE_MOTION_SENSORS, rateInMilliseconds, FALSE);
                break;
            default:
//                Log.e(TAG, "Unknown event type: " + eventType);
                return FALSE;
        }

        if (success) {
            mNativePtr = nativePtr;
            SetEventTypeActive(eventType, TRUE);
        }

        return success;
    }
}

//@CalledByNative
Int32 DeviceSensors::GetNumberActiveDeviceMotionSensors()
{
    Set<Integer> deviceMotionSensors = new HashSet<Integer>(DEVICE_MOTION_SENSORS);
    deviceMotionSensors.removeAll(mActiveSensors);
    return DEVICE_MOTION_SENSORS.size() - deviceMotionSensors.size();
}

/**
 * Stop listening to sensors for a given event type. Ensures that sensors are not disabled
 * if they are still in use by a different event type.
 *
 * @param eventType Type of event to listen to, can be either DEVICE_ORIENTATION or
 *                  DEVICE_MOTION.
 * We strictly guarantee that the corresponding native*() methods will not be called
 * after this method returns.
 */
//@CalledByNative
void DeviceSensors::Stop(
    /* [in] */ Int32 eventType)
{
    Set<Integer> sensorsToRemainActive = new HashSet<Integer>();

    {
        Object::Autolock lock(mNativePtrLock);
        switch (eventType) {
            case DEVICE_ORIENTATION:
                if (mDeviceMotionIsActive) {
                    sensorsToRemainActive.addAll(DEVICE_MOTION_SENSORS);
                }
                break;
            case DEVICE_MOTION:
                if (mDeviceOrientationIsActive) {
                    sensorsToRemainActive.addAll(DEVICE_ORIENTATION_SENSORS);
                }
                break;
            default:
//                Log.e(TAG, "Unknown event type: " + eventType);
                return;
        }

        Set<Integer> sensorsToDeactivate = new HashSet<Integer>(mActiveSensors);
        sensorsToDeactivate.removeAll(sensorsToRemainActive);
        UnregisterSensors(sensorsToDeactivate);
        SetEventTypeActive(eventType, FALSE);
        if (mActiveSensors.isEmpty()) {
            mNativePtr = 0;
        }
    }
}

//@Override
void DeviceSensors::OnAccuracyChanged(
    /* [in] */ ISensor* sensor,
    /* [in] */ Int32 accuracy)
{
    // Nothing
}

//@Override
void DeviceSensors::OnSensorChanged(
    /* [in] */ ISensorEvent* event)
{
    AutoPtr<ISensor> sensor;
    event->GetSensor((ISensor**)&sensor);
    Int32 type;
    sensor->GetType(&type);
    AutoPtr< ArrayOf<Float> > values;
    event->GetValuse((ArrayOf<Float>**)&values);
    SensorChanged(type, values);
}

//@VisibleForTesting
void DeviceSensors::SensorChanged(
    /* [in] */ Int32 type,
    /* [in] */ Float[] values)
{
    switch (type) {
        case ISensor::TYPE_ACCELEROMETER:
            if (mDeviceMotionIsActive) {
                GotAccelerationIncludingGravity((*values)[0], (*values)[1], (*values)[2]);
            }
            break;
        case ISensor::TYPE_LINEAR_ACCELERATION:
            if (mDeviceMotionIsActive) {
                GotAcceleration((*values)[0], (*values)[1], (*values)[2]);
            }
            break;
        case ISensor::TYPE_GYROSCOPE:
            if (mDeviceMotionIsActive) {
                GotRotationRate((*values)[0], (*values)[1], (*values)[2]);
            }
            break;
        case ISensor::TYPE_ROTATION_VECTOR:
            if (mDeviceOrientationIsActive) {
                if (values->GetLength() > 4) {
                    // On some Samsung devices SensorManager.getRotationMatrixFromVector
                    // appears to throw an exception if rotation vector has length > 4.
                    // For the purposes of this class the first 4 values of the
                    // rotation vector are sufficient (see crbug.com/335298 for details).
                    if (mTruncatedRotationVector == NULL) {
                        mTruncatedRotationVector = ArrayOf<Float>::Alloc(4);
                    }
                    System.arraycopy(values, 0, mTruncatedRotationVector, 0, 4);
                    GetOrientationFromRotationVector(mTruncatedRotationVector);
                }
                else {
                    GetOrientationFromRotationVector(values);
                }
            }
            break;
        default:
            // Unexpected
            return;
    }
}

/**
 * Returns orientation angles from a rotation matrix, such that the angles are according
 * to spec {@link http://dev.w3.org/geo/api/spec-source-orientation.html}.
 * <p>
 * It is assumed the rotation matrix transforms a 3D column vector from device coordinate system
 * to the world's coordinate system, as e.g. computed by {@see SensorManager.getRotationMatrix}.
 * <p>
 * In particular we compute the decomposition of a given rotation matrix R such that <br>
 * R = Rz(alpha) * Rx(beta) * Ry(gamma), <br>
 * where Rz, Rx and Ry are rotation matrices around Z, X and Y axes in the world coordinate
 * reference frame respectively. The reference frame consists of three orthogonal axes X, Y, Z
 * where X points East, Y points north and Z points upwards perpendicular to the ground plane.
 * The computed angles alpha, beta and gamma are in radians and clockwise-positive when viewed
 * along the positive direction of the corresponding axis. Except for the special case when the
 * beta angle is +-pi/2 these angles uniquely define the orientation of a mobile device in 3D
 * space. The alpha-beta-gamma representation resembles the yaw-pitch-roll convention used in
 * vehicle dynamics, however it does not exactly match it. One of the differences is that the
 * 'pitch' angle beta is allowed to be within [-pi, pi). A mobile device with pitch angle
 * greater than pi/2 could correspond to a user lying down and looking upward at the screen.
 *
 * <p>
 * Upon return the array values is filled with the result,
 * <ul>
 * <li>values[0]: rotation around the Z axis, alpha in [0, 2*pi)</li>
 * <li>values[1]: rotation around the X axis, beta in [-pi, pi)</li>
 * <li>values[2]: rotation around the Y axis, gamma in [-pi/2, pi/2)</li>
 * </ul>
 * <p>
 *
 * @param R
 *        a 3x3 rotation matrix {@see SensorManager.getRotationMatrix}.
 *
 * @param values
 *        an array of 3 doubles to hold the result.
 *
 * @return the array values passed as argument.
 */
//@VisibleForTesting
AutoPtr< ArrayOf<Double> > DeviceSensors::ComputeDeviceOrientationFromRotationMatrix(
    /* [in] */ ArrayOf<Float>* R,
    /* [in] */ ArrayOf<Double>* values)
{
    /*
     * 3x3 (length=9) case:
     *   /  R[ 0]   R[ 1]   R[ 2]  \
     *   |  R[ 3]   R[ 4]   R[ 5]  |
     *   \  R[ 6]   R[ 7]   R[ 8]  /
     *
     */
    if (R->GetLength() != 9) {
        return values;
    }

    if ((*R)[8] > 0) {  // cos(beta) > 0
        (*values)[0] = Math::Atan2(-R[1], R[4]);
        (*values)[1] = Math::Asin(R[7]);           // beta (-pi/2, pi/2)
        (*values)[2] = Math::Atan2(-R[6], R[8]);   // gamma (-pi/2, pi/2)
    }
    else if ((*R)[8] < 0) {  // cos(beta) < 0
        (*values)[0] = Math::Atan2(R[1], -(*R)[4]);
        (*values)[1] = -Math::Asin((*R)[7]);
        (*values)[1] += ((*values)[1] >= 0) ? -Math::PI : Math::PI; // beta [-pi,-pi/2) U (pi/2,pi)
        (*values)[2] = Math::Atan2((*R)[6], -R[8]);   // gamma (-pi/2, pi/2)
    }
    else { // R[8] == 0
        if ((*R)[6] > 0) {  // cos(gamma) == 0, cos(beta) > 0
            (*values)[0] = Math::Atan2(-(*R)[1], (*R)[4]);
            (*values)[1] = Math::Asin((*R)[7]);       // beta [-pi/2, pi/2]
            (*values)[2] = -Math::PI / 2;          // gamma = -pi/2
        }
        else if ((*R)[6] < 0) { // cos(gamma) == 0, cos(beta) < 0
            (*values)[0] = Math::Atan2((*R)[1], -(*R)[4]);
            (*values)[1] = -Math::Asin((*R)[7]);
            (*values)[1] += ((*values)[1] >= 0) ? -Math::PI : Math::PI; // beta [-pi,-pi/2) U (pi/2,pi)
            (*values)[2] = -Math::PI / 2;          // gamma = -pi/2
        }
        else { // R[6] == 0, cos(beta) == 0
            // gimbal lock discontinuity
            (*values)[0] = Math::Atan2((*R)[3], (*R)[0]);
            (*values)[1] = ((*R)[7] > 0) ? Math::PI / 2 : -Math::PI / 2;  // beta = +-pi/2
            (*values)[2] = 0;                                        // gamma = 0
        }
    }

    // alpha is in [-pi, pi], make sure it is in [0, 2*pi).
    if ((*values)[0] < 0) {
        (*values)[0] += 2 * Math::PI; // alpha [0, 2*pi)
    }

    return (*values);
}

void DeviceSensors::GetOrientationFromRotationVector(
    /* [in] */ ArrayOf<Float>* rotationVector)
{
    AutoPtr< ArrayOf<Float> > deviceRotationMatrix = ArrayOf<Float>::Alloc(9);
    SensorManager::GetRotationMatrixFromVector(deviceRotationMatrix, rotationVector);

    AutoPtr< ArrayOf<Double> > rotationAngles = ArrayOf<Double>::Alloc(3);
    ComputeDeviceOrientationFromRotationMatrix(deviceRotationMatrix, rotationAngles);

    GotOrientation(Math::ToDegrees((*rotationAngles)[0]),
                   Math::ToDegrees((*rotationAngles)[1]),
                   Math::ToDegrees((*rotationAngles)[2]));
}

AutoPtr<SensorManagerProxy> DeviceSensors::GetSensorManagerProxy()
{
    if (mSensorManagerProxy != NULL) {
        return mSensorManagerProxy;
    }

    AutoPtr<ICallable> callable = new InnerCallable(this);
    AutoPtr<SensorManager> sensorManager = ThreadUtils::RunOnUiThreadBlockingNoException(callable);

    if (sensorManager != NULL) {
        mSensorManagerProxy = new SensorManagerProxyImpl(sensorManager);
    }

    return mSensorManagerProxy;
}

//@VisibleForTesting
void DeviceSensors::SetSensorManagerProxy(
    /* [in] */ SensorManagerProxy sensorManagerProxy)
{
    mSensorManagerProxy = sensorManagerProxy;
}

void DeviceSensors::SetEventTypeActive(
    /* [in] */ Int32 eventType,
    /* [in] */ Boolean value)
{
    switch (eventType) {
        case DEVICE_ORIENTATION:
            mDeviceOrientationIsActive = value;
            return;
        case DEVICE_MOTION:
            mDeviceMotionIsActive = value;
            return;
    }
}

/**
 * @param sensorTypes List of sensors to activate.
 * @param rateInMilliseconds Intended delay (in milliseconds) between sensor readings.
 * @param failOnMissingSensor If true the method returns true only if all sensors could be
 *                            activated. When false the method return true if at least one
 *                            sensor in sensorTypes could be activated.
 */
Boolean DeviceSensors::RegisterSensors(
    /* [in] */ Set<Integer> sensorTypes,
    /* [in] */ Int32 rateInMilliseconds,
    /* [in] */ Boolean failOnMissingSensor)
{
    Set<Integer> sensorsToActivate = new HashSet<Integer>(sensorTypes);
    sensorsToActivate.removeAll(mActiveSensors);
    Boolean success = FALSE;

    for (Integer sensorType : sensorsToActivate) {
        Boolean result = RegisterForSensorType(sensorType, rateInMilliseconds);
        if (!result && failOnMissingSensor) {
            // restore the previous state upon failure
            UnregisterSensors(sensorsToActivate);
            return FALSE;
        }

        if (result) {
            mActiveSensors.add(sensorType);
            success = TRUE;
        }
    }

    return success;
}

void DeviceSensors::UnregisterSensors(
    /* [in] */ Iterable<Integer> sensorTypes)
{
    for (Integer sensorType : sensorTypes) {
        if (mActiveSensors.contains(sensorType)) {
            GetSensorManagerProxy()->UnregisterListener(this, sensorType);
            mActiveSensors->Remove(sensorType);
        }
    }
}

Boolean DeviceSensors::RegisterForSensorType(
    /* [in] */ Int32 type,
    /* [in] */ Int32 rateInMilliseconds)
{
    AutoPtr<SensorManagerProxy> sensorManager = GetSensorManagerProxy();
    if (sensorManager == NULL) {
        return FALSE;
    }

    const Int32 rateInMicroseconds = 1000 * rateInMilliseconds;

    return sensorManager->RegisterListener(this, type, rateInMicroseconds, GetHandler());
}

void DeviceSensors::GotOrientation(
    /* [in] */ Double alpha,
    /* [in] */ Double beta,
    /* [in] */ Double gamma)
{
    Object::Autolock lock(mNativePtrLock);
    if (mNativePtr != 0) {
        nativeGotOrientation(mNativePtr, alpha, beta, gamma);
    }
}

void DeviceSensors::GotAcceleration(
    /* [in] */ Double x,
    /* [in] */ Double y,
    /* [in] */ Double z)
{
    Object::Autolock lock(mNativePtrLock);

    if (mNativePtr != 0) {
        NativeGotAcceleration(mNativePtr, x, y, z);
    }
}

void DeviceSensors::GotAccelerationIncludingGravity(
    /* [in] */ Double x,
    /* [in] */ Double y,
    /* [in] */ Double z)
{
    Object::Autolock lock(mNativePtrLock);

    if (mNativePtr != 0) {
        NativeGotAccelerationIncludingGravity(mNativePtr, x, y, z);
    }
}

void DeviceSensors::GotRotationRate(
    /* [in] */ Double alpha,
    /* [in] */ Double beta,
    /* [in] */ Double gamma)
{
    Object::Autolock lock(mNativePtrLock);

    if (mNativePtr != 0) {
        NativeGotRotationRate(mNativePtr, alpha, beta, gamma);
    }
}

AutoPtr<IHandler> DeviceSensors::GetHandler()
{
    // TODO(timvolodine): Remove the mHandlerLock when sure that getHandler is not called
    // from multiple threads. This will be the case when device motion and device orientation
    // use the same polling thread (also see crbug/234282).
    Object::Autolock lock(mHandlerLock);
    if (mHandler == NULL) {
        AutoPtr<IHandlerThread> thread;
        CHandlerThread::New(String("DeviceMotionAndOrientation"), (IHandlerThread**)&thread);
        thread->Start();
        AutoPtr<ILooper> looper;
        thread->GetLooper((ILooper**)&looper);
        CHandler::New(looper, (IHandler**)&mHandler);  // blocks on thread start
    }
    return mHandler;
}

//@CalledByNative
AutoPtr<DeviceSensors> DeviceSensors::GetInstance(
    /* [in] */ IContext* appContext)
{
    Object::Autolock lock(sSingletonLock);

    if (sSingleton == NULL) {
        sSingleton = new DeviceSensors(appContext);
    }

    return sSingleton;
}

/**
 * Native JNI calls,
 * see content/browser/device_sensors/sensor_manager_android.cc
 */

/**
 * Orientation of the device with respect to its reference frame.
 */
void DeviceSensors::NativeGotOrientation(
    /* [in] */ Int64 nativeSensorManagerAndroid,
    /* [in] */ Double alpha,
    /* [in] */ Double beta,
    /* [in] */ Double gamma)
{
}

/**
 * Linear acceleration without gravity of the device with respect to its body frame.
 */
void DeviceSensors::NativeGotAcceleration(
    /* [in] */ Int64 nativeSensorManagerAndroid,
    /* [in] */ Double x,
    /* [in] */ Double y,
    /* [in] */ Double z)
{
}

/**
 * Acceleration including gravity of the device with respect to its body frame.
 */
void DeviceSensors::NativeGotAccelerationIncludingGravity(
    /* [in] */ Int64 nativeSensorManagerAndroid,
    /* [in] */ Double x,
    /* [in] */ Double y,
    /* [in] */ Double z)
{
}

/**
 * Rotation rate of the device with respect to its body frame.
 */
void DeviceSensors::NativeGotRotationRate(
    /* [in] */ Int64 nativeSensorManagerAndroid,
    /* [in] */ Double alpha,
    /* [in] */ Double beta,
    /* [in] */ Double gamma)
{
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
