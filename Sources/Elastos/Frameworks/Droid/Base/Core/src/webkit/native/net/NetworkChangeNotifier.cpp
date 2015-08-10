
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

//===============================================================
// 			NetworkChangeNotifier::InnerObserver
//===============================================================
NetworkChangeNotifier::InnerObserver::InnerObserver(
	/* [in] */ NetworkChangeNotifier* owner)
	: mOwner(owner)
{
}

ECode NetworkChangeNotifier::InnerObserver::OnConnectionTypeChanged(
	/* [in] */ Int32 newConnectionType)
{
	assert(NULL == mOwner);
	mOwner->UpdateCurrentConnectionType(newConnectionType);
	return NOERROR;
}

//===============================================================
// 					NetworkChangeNotifier
//===============================================================
const Int32 NetworkChangeNotifier::CONNECTION_UNKNOWN;
const Int32 NetworkChangeNotifier::CONNECTION_ETHERNET;
const Int32 NetworkChangeNotifier::CONNECTION_WIFI;
const Int32 NetworkChangeNotifier::CONNECTION_2G;
const Int32 NetworkChangeNotifier::CONNECTION_3G;
const Int32 NetworkChangeNotifier::CONNECTION_4G;
const Int32 NetworkChangeNotifier::CONNECTION_NONE;
const Int32 NetworkChangeNotifier::CONNECTION_BLUETOOTH;
AutoPtr<NetworkChangeNotifier> NetworkChangeNotifier::sInstance;

//@CalledByNative
AutoPtr<NetworkChangeNotifier> NetworkChangeNotifier::Init(
    /* [in] */ IContext* context)
{
    if (sInstance == NULL) {
        sInstance = new NetworkChangeNotifier(context);
    }
    return sInstance;
}

Boolean NetworkChangeNotifier::IsInitialized()
{
    return Boolean(sInstance != NULL);
}

//@CalledByNative
Int32 NetworkChangeNotifier::GetCurrentConnectionType()
{
    return mCurrentConnectionType;
}

/**
 * Adds a native-side observer.
 */
//@CalledByNative
ECode NetworkChangeNotifier::AddNativeObserver(
    /* [in] */ Int64 nativeChangeNotifier)
{
    mNativeChangeNotifiers->Append(nativeChangeNotifier);
    return NOERROR;
}

/**
 * Removes a native-side observer.
 */
//@CalledByNative
ECode NetworkChangeNotifier::RemoveNativeObserver(
    /* [in] */ Int64 nativeChangeNotifier)
{
    mNativeChangeNotifiers->Remove(nativeChangeNotifier);
    return NOERROR;
}

/**
 * Returns the singleton instance.
 */
AutoPtr<NetworkChangeNotifier> NetworkChangeNotifier::GetInstance()
{
    assert (NULL == sInstance);
    return sInstance;
}

/**
 * Enables auto detection of the current network state based on notifications from the system.
 * Note that passing true here requires the embedding app have the platform ACCESS_NETWORK_STATE
 * permission.
 *
 * @param shouldAutoDetect true if the NetworkChangeNotifier should listen for system changes in
 *    network connectivity.
 */
ECode NetworkChangeNotifier::SetAutoDetectConnectivityState(
    /* [in] */ Boolean shouldAutoDetect)
{
    GetInstance()->SetAutoDetectConnectivityStateInternal(shouldAutoDetect);
    return NOERROR;
}

//@CalledByNative
ECode NetworkChangeNotifier::ForceConnectivityState(
    /* [in] */ Boolean networkAvailable)
{
    SetAutoDetectConnectivityState(FALSEx);
    GetInstance()->ForceConnectivityStateInternal(networkAvailable);
    return NOERROR;
}

ECode NetworkChangeNotifier::AddConnectionTypeObserver(
    /* [in] */ ConnectionTypeObserver* observer)
{
	VALIDATE_NOT_NULL(observer);
    GetInstance()->AddConnectionTypeObserverInternal(observer);
    return NOERROR;
}

ECode NetworkChangeNotifier::RemoveConnectionTypeObserver(
    /* [in] */ ConnectionTypeObserver* observer)
{
	VALIDATE_NOT_NULL(observer);
    GetInstance()->RemoveConnectionTypeObserverInternal(observer);
    return NOERROR;
}

// For testing only.
AutoPtr<NetworkChangeNotifierAutoDetect> NetworkChangeNotifier::GetAutoDetectorForTest()
{
    return GetInstance()->mAutoDetector;
}

/**
 * Checks if there currently is connectivity.
 */
Boolean NetworkChangeNotifier::IsOnline()
{
    Int32 connectionType = GetInstance()->GetCurrentConnectionType();
    return Boolean(connectionType != CONNECTION_UNKNOWN && connectionType != CONNECTION_NONE);
}

NetworkChangeNotifier::NetworkChangeNotifier(
    /* [in] */ IContext* context)
    : mCurrentConnectionType(CONNECTION_UNKNOWN)
{
    context->GetApplicationContext(&mContext);
    mNativeChangeNotifiers = ArrayOf<Int64>::Alloc(sizeof(Int64));;
    mConnectionTypeObservers = new ObserverList<ConnectionTypeObserver*>();
}

ECode NetworkChangeNotifier::ResetInstanceForTests(
    /* [in] */ IContext* context)
{
	VALIDATE_NOT_NULL(context);
    sInstance = new NetworkChangeNotifier(context);
    return NOERROR;
}

ECode NetworkChangeNotifier::DestroyAutoDetector()
{
    if (mAutoDetector != NULL) {
        mAutoDetector->Destroy();
        mAutoDetector = NULL;
    }
    return NOERROR;
}

ECode NetworkChangeNotifier::SetAutoDetectConnectivityStateInternal(
    /* [in] */ Boolean shouldAutoDetect)
{
    if (shouldAutoDetect) {

		// question: how to translate a function in funciton
        if (NULL == mAutoDetector) {
            mAutoDetector = new NetworkChangeNotifierAutoDetect(new InnerObserver(this), mContext);
            UpdateCurrentConnectionType(mAutoDetector->GetCurrentConnectionType());
        }
    }
    else {
        DestroyAutoDetector();
    }
    return NOERROR;
}

/**
 * Updates the perceived network state when not auto-detecting changes to connectivity.
 *
 * @param networkAvailable True if the NetworkChangeNotifier should perceive a "connected"
 *    state, false implies "disconnected".
 */
ECode NetworkChangeNotifier::ForceConnectivityStateInternal(
    /* [in] */ Boolean forceOnline)
{
    Boolean connectionCurrentlyExists = Boolean(mCurrentConnectionType != CONNECTION_NONE);
    if (connectionCurrentlyExists != forceOnline) {
        UpdateCurrentConnectionType(forceOnline ? CONNECTION_UNKNOWN : CONNECTION_NONE);
    }
    return NOERROR;
}

ECode NetworkChangeNotifier::UpdateCurrentConnectionType(
    /* [in] */ Int32 newConnectionType)
{
    mCurrentConnectionType = newConnectionType;
    NotifyObserversOfConnectionTypeChange(newConnectionType);
    return NOERROR;
}

/**
 * Alerts all observers of a connection change.
 */
ECode NetworkChangeNotifier::NotifyObserversOfConnectionTypeChange(
    /* [in] */ Int32 newConnectionType)
{
	Int64 nativeChangeNotifier;
	for (Int32 i=0; i<mNativeChangeNotifiers->GetLength(); ++i)
	{
		nativeChangeNotifier = (*mNativeChangeNotifiers)[i];
		NativeNotifyConnectionTypeChanged(nativeChangeNotifier, newConnectionType);
	}

	for (Int32 i=0; i<mConnectionTypeObservers->GetLength(); ++i)
	{
		AutoPtr<ConnectionTypeObserver> observer = (*mConnectionTypeObservers)[i];
		observer->OnConnectionTypeChanged(newConnectionType);
	}
	return NOERROR;
}

/**
 * Adds an observer for any connection type changes.
 */
ECode NetworkChangeNotifier::AddConnectionTypeObserverInternal(
    /* [in] */ ConnectionTypeObserver* observer)
{
	VALIDATE_NOT_NULL(observer);
    mConnectionTypeObservers->AddObserver(observer);
    return NOERROR;
}

/**
 * Removes an observer for any connection type changes.
 */
ECode NetworkChangeNotifier::RemoveConnectionTypeObserverInternal(
    /* [in] */ ConnectionTypeObserver* observer)
{
	VALIDATE_NOT_NULL(observer);
    mConnectionTypeObservers->RemoveObserver(observer);
    return NOERROR;
}

//@NativeClassQualifiedName("NetworkChangeNotifierDelegateAndroid")
/*
* native function
*/
//native void nativeNotifyConnectionTypeChanged(long nativePtr, int newConnectionType);
ECode NetworkChangeNotifier::NativeNotifyConnectionTypeChanged(
    /* [in] */ Int64 nativePtr,
    /* [in] */ Int32 newConnectionType)
{
	return NOERROR;
}

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
