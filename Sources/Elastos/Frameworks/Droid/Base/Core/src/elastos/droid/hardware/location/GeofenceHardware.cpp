
#include "elastos/droid/hardware/location/GeofenceHardware.h"
#include "elastos/droid/hardware/location/GeofenceHardwareRequest.h"
#include "elastos/droid/hardware/location/GeofenceHardwareRequestParcelable.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/Build.h"
#include <elastos/core/AutoLock.h>

using Elastos::Droid::Os::Build;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Location {

CAR_INTERFACE_IMPL(GeofenceHardware::GeofenceHardwareMonitorCallbackWrapper,
        Object, IIGeofenceHardwareMonitorCallback)

GeofenceHardware::GeofenceHardwareMonitorCallbackWrapper::GeofenceHardwareMonitorCallbackWrapper(
/* [in] */ IGeofenceHardwareMonitorCallback* c)
{
    IWeakReferenceSource* wrs = IWeakReferenceSource::Probe(c);
    wrs->GetWeakReference((IWeakReference**)&mCallback);
}

ECode GeofenceHardware::GeofenceHardwareMonitorCallbackWrapper::OnMonitoringSystemChange(
    /* [in] */ IGeofenceHardwareMonitorEvent* event)
{
    AutoPtr<IGeofenceHardwareMonitorCallback> c;
    FAIL_RETURN(mCallback->Resolve(EIID_IGeofenceHardwareMonitorCallback, (IInterface**)&c))

    if (c == NULL) {
        return NOERROR;
    }

    // report the legacy event first, so older clients are not broken
    Int32 type;
    FAIL_RETURN(event->GetMonitoringType(&type))
    Int32 status;
    FAIL_RETURN(event->GetMonitoringStatus(&status))
    AutoPtr<ILocation> location;
    FAIL_RETURN(event->GetLocation((ILocation**)&location))

    FAIL_RETURN(c->OnMonitoringSystemChange(type,
            status == IGeofenceHardware::MONITOR_CURRENTLY_AVAILABLE, location))

    // and only call the updated callback on on L and above, this complies with the
    // documentation of GeofenceHardwareMonitorCallback
    if (Build::VERSION::SDK_INT >= Build::VERSION_CODES::LOLLIPOP) {
        FAIL_RETURN(c->OnMonitoringSystemChange(event))
    }
    return NOERROR;
}

CAR_INTERFACE_IMPL(GeofenceHardware::GeofenceHardwareCallbackWrapper,
        Object, IIGeofenceHardwareCallback)

GeofenceHardware::GeofenceHardwareCallbackWrapper::GeofenceHardwareCallbackWrapper(
    /* [in] */ IGeofenceHardwareCallback* c,
    /* [in] */ GeofenceHardware* host)
    : mHost(host)
{
    IWeakReferenceSource* wrs = IWeakReferenceSource::Probe(c);
    wrs->GetWeakReference((IWeakReference**)&mCallback);
}

ECode GeofenceHardware::GeofenceHardwareCallbackWrapper::OnGeofenceTransition(
    /* [in] */ Int32 geofenceId,
    /* [in] */ Int32 transition,
    /* [in] */ ILocation* location,
    /* [in] */ Int64 timestamp,
    /* [in] */ Int32 monitoringType)
{
    AutoPtr<IGeofenceHardwareCallback> c;
    FAIL_RETURN(mCallback->Resolve(EIID_IGeofenceHardwareCallback, (IInterface**)&c))

    if (c != NULL) {
        FAIL_RETURN(c->OnGeofenceTransition(geofenceId, transition, location, timestamp,
                monitoringType))
    }
    return NOERROR;
}

ECode GeofenceHardware::GeofenceHardwareCallbackWrapper::OnGeofenceAdd(
    /* [in] */ Int32 geofenceId,
    /* [in] */ Int32 status)
{
    AutoPtr<IGeofenceHardwareCallback> c;
    FAIL_RETURN(mCallback->Resolve(EIID_IGeofenceHardwareCallback, (IInterface**)&c))

    if (c != NULL) {
        FAIL_RETURN(c->OnGeofenceAdd(geofenceId, status))
    }
    return NOERROR;
}

ECode GeofenceHardware::GeofenceHardwareCallbackWrapper::OnGeofenceRemove(
    /* [in] */ Int32 geofenceId,
    /* [in] */ Int32 status)
{
    AutoPtr<IGeofenceHardwareCallback> c;
    FAIL_RETURN(mCallback->Resolve(EIID_IGeofenceHardwareCallback, (IInterface**)&c))

    if (c != NULL) {
        FAIL_RETURN(c->OnGeofenceRemove(geofenceId, status))
        mHost->RemoveCallback(c);
    }
    return NOERROR;
}

ECode GeofenceHardware::GeofenceHardwareCallbackWrapper::OnGeofencePause(
    /* [in] */ Int32 geofenceId,
    /* [in] */ Int32 status)
{
    AutoPtr<IGeofenceHardwareCallback> c;
    FAIL_RETURN(mCallback->Resolve(EIID_IGeofenceHardwareCallback, (IInterface**)&c))

    if (c != NULL) {
        FAIL_RETURN(c->OnGeofencePause(geofenceId, status))
    }
    return NOERROR;
}

ECode GeofenceHardware::GeofenceHardwareCallbackWrapper::OnGeofenceResume(
    /* [in] */ Int32 geofenceId,
    /* [in] */ Int32 status)
{
    AutoPtr<IGeofenceHardwareCallback> c;
    FAIL_RETURN(mCallback->Resolve(EIID_IGeofenceHardwareCallback, (IInterface**)&c))

    if (c != NULL) {
        FAIL_RETURN(c->OnGeofenceResume(geofenceId, status))
    }
    return NOERROR;
}

const Int32 GeofenceHardware::NUM_MONITORS = 2;

CAR_INTERFACE_IMPL(GeofenceHardware, Object, IGeofenceHardware)

GeofenceHardware::GeofenceHardware(
        /* [in] */ IIGeofenceHardware* service)
    : mService(service)
{
}

GeofenceHardware::~GeofenceHardware()
{
}

ECode GeofenceHardware::GetMonitoringTypes(
    /* [out, callee] */ ArrayOf<Int32>** types)
{
    VALIDATE_NOT_NULL(types);

    ECode ec = NOERROR;
    //try {
    ec = mService->GetMonitoringTypes(types);
    //} catch (RemoteException e) {
    if (FAILED(ec)) {
        *types = ArrayOf<Int32>::Alloc(0);
        REFCOUNT_ADD(*types);
    }
    //}

    return NOERROR;
}

ECode GeofenceHardware::GetStatusOfMonitoringType(
    /* [in] */ Int32 monitoringType,
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);

    ECode ec = NOERROR;
    //try {
    ec = mService->GetStatusOfMonitoringType(monitoringType, type);
    //} catch (RemoteException e) {
    if (FAILED(ec)) {
        *type = MONITOR_UNSUPPORTED;
    }
    //}
    return NOERROR;
}

ECode GeofenceHardware::AddGeofence(
    /* [in] */ Int32 geofenceId,
    /* [in] */ Int32 monitoringType,
    /* [in] */ IGeofenceHardwareRequest* geofenceRequest,
    /* [in] */ IGeofenceHardwareCallback* _callback,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    //try {
    Int32 type;
    FAIL_RETURN(geofenceRequest->GetType(&type))
    if (type == GeofenceHardwareRequest::GEOFENCE_TYPE_CIRCLE) {
        AutoPtr<IGeofenceHardwareRequestParcelable> parce = new GeofenceHardwareRequestParcelable();
        FAIL_RETURN(((GeofenceHardwareRequestParcelable*)parce.Get())->constructor(geofenceId, geofenceRequest))
        AutoPtr<GeofenceHardwareCallbackWrapper> wrapper;
        FAIL_RETURN(GetCallbackWrapper(_callback, (GeofenceHardwareCallbackWrapper**)&wrapper))
        FAIL_RETURN(mService->AddCircularFence(monitoringType, parce, wrapper, result))
    } else {
        //throw new IllegalArgumentException("Geofence Request type not supported");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    //} catch (RemoteException e) {
    //}
    *result = FALSE;
    return NOERROR;
}

ECode GeofenceHardware::RemoveGeofence(
    /* [in] */ Int32 geofenceId,
    /* [in] */ Int32 monitoringType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    ECode ec = NOERROR;
    //try {
    ec = mService->RemoveGeofence(geofenceId, monitoringType, result);
    //} catch (RemoteException e) {
    if (FAILED(ec)) {
        *result = FALSE;
    }
    //}
    return NOERROR;
}

ECode GeofenceHardware::PauseGeofence(
    /* [in] */ Int32 geofenceId,
    /* [in] */ Int32 monitoringType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    ECode ec = NOERROR;
    //try {
    ec = mService->PauseGeofence(geofenceId, monitoringType, result);
    //} catch (RemoteException e) {
    if (FAILED(ec)) {
        *result = FALSE;
    }
    //}
    return NOERROR;
}

ECode GeofenceHardware::ResumeGeofence(
    /* [in] */ Int32 geofenceId,
    /* [in] */ Int32 monitoringType,
    /* [in] */ Int32 monitorTransition,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    ECode ec = NOERROR;
    //try {
    ec = mService->ResumeGeofence(geofenceId, monitoringType, monitorTransition, result);
    //} catch (RemoteException e) {
    if (FAILED(ec)) {
        *result = FALSE;
    }
    //}
    return NOERROR;
}

ECode GeofenceHardware::RegisterForMonitorStateChangeCallback(
    /* [in] */ Int32 monitoringType,
    /* [in] */ IGeofenceHardwareMonitorCallback* _callback,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    ECode ec = NOERROR;
    //try {
    AutoPtr<GeofenceHardwareMonitorCallbackWrapper> wrapper;
    ec = GetMonitorCallbackWrapper(_callback, (GeofenceHardwareMonitorCallbackWrapper**)&wrapper);
    if (FAILED(ec)) {
        *result = FALSE;
        return NOERROR;
    }
    ec = mService->RegisterForMonitorStateChangeCallback(monitoringType, wrapper, result);
    //} catch (RemoteException e) {
    if (FAILED(ec)) {
        *result = FALSE;
    }
    //}
    return NOERROR;
}

ECode GeofenceHardware::UnregisterForMonitorStateChangeCallback(
    /* [in] */ Int32 monitoringType,
    /* [in] */ IGeofenceHardwareMonitorCallback* _callback,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Boolean _result = FALSE;
    //try {
    ECode ec = NOERROR;
    AutoPtr<GeofenceHardwareMonitorCallbackWrapper> wrapper;
    ec = GetMonitorCallbackWrapper(_callback, (GeofenceHardwareMonitorCallbackWrapper**)&wrapper);
    FAIL_GOTO(ec, EXIT);
    ec = mService->UnregisterForMonitorStateChangeCallback(monitoringType, wrapper, &_result);
    FAIL_GOTO(ec, EXIT);
    if (_result) {
        RemoveMonitorCallback(_callback);
    }

    //} catch (RemoteException e) {
    //}
EXIT:
    *result = _result;
    return NOERROR;
}

void GeofenceHardware::RemoveCallback(
    /* [in] */ IGeofenceHardwareCallback* _callback)
{
    synchronized (mCallbacksLock) {
        mCallbacks.Erase(_callback);
    }
}

ECode GeofenceHardware::GetCallbackWrapper(
    /* [in] */ IGeofenceHardwareCallback* _callback,
    /* [out] */ GeofenceHardwareCallbackWrapper** wrapper)
{
    VALIDATE_NOT_NULL(wrapper);

    synchronized (mCallbacksLock) {
        AutoPtr<GeofenceHardwareCallbackWrapper> _wrapper = mCallbacks[_callback];
        if (_wrapper == NULL) {
            _wrapper = new GeofenceHardwareCallbackWrapper(_callback, this);
            mCallbacks[_callback] = _wrapper;
        }
        *wrapper = _wrapper;
        REFCOUNT_ADD(*wrapper);
        return NOERROR;
    }
    return NOERROR;
}

void GeofenceHardware::RemoveMonitorCallback(
    /* [in] */ IGeofenceHardwareMonitorCallback* _callback)
{
    synchronized (mMonitorCallbacksLock) {
        mMonitorCallbacks.Erase(_callback);
    }
}

ECode GeofenceHardware::GetMonitorCallbackWrapper(
    /* [in] */ IGeofenceHardwareMonitorCallback* _callback,
    /* [out] */ GeofenceHardwareMonitorCallbackWrapper** wrapper)
{
    VALIDATE_NOT_NULL(wrapper);

    synchronized (mMonitorCallbacksLock) {
        AutoPtr<GeofenceHardwareMonitorCallbackWrapper> _wrapper = mMonitorCallbacks[_callback];
        if (_wrapper == NULL) {
            _wrapper = new GeofenceHardwareMonitorCallbackWrapper(_callback);
            mMonitorCallbacks[_callback] = _wrapper;
        }
        *wrapper = _wrapper;
        REFCOUNT_ADD(*wrapper);
        return NOERROR;
    }
    return NOERROR;
}

} //Location
} //Hardware
} //Droid
} //Elastos