
#include "elastos/droid/hardware/location/ActivityRecognitionHardware.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/AutoLock.h>
#include "hardware/activity_recognition.h"

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Location {

CAR_INTERFACE_IMPL_2(ActivityRecognitionHardware, Object, IActivityRecognitionHardware, IIActivityRecognitionHardware)

const String ActivityRecognitionHardware::TAG = String("ActivityRecognitionHardware");

const String ActivityRecognitionHardware::HARDWARE_PERMISSION = Manifest.permission.LOCATION_HARDWARE;
const Int32 ActivityRecognitionHardware::INVALID_ACTIVITY_TYPE = -1;
const Int32 ActivityRecognitionHardware::NATIVE_SUCCESS_RESULT = 0;

AutoPtr<IActivityRecognitionHardware> ActivityRecognitionHardware::sSingletonInstance = NULL;
const Object ActivityRecognitionHardware::sSingletonInstanceLock;

// native bindings
static void ActivityRecognitionHardware::staInit = NativeClassInit();

ActivityRecognitionHardware::ActivityRecognitionHardware(
    /* [in] */ IContext* context)
    : mContext(context)
    , mSupportedActivities(FetchSupportedActivities())
{
    NativeInitialize();

    CRemoteCallbackList::New((IRemoteCallbackList**)&mSinks);
}

ActivityRecognitionHardware::~ActivityRecognitionHardware()
{
}

ECode ActivityRecognitionHardware::GetInstance(
    /* [in] */ IContext* context,
    /* [out] */ IActivityRecognitionHardware** instance)
{
    VALIDATE_NOT_NULL(instance);

    synchronized (sSingletonInstanceLock) {
        if (sSingletonInstance == NULL) {
            sSingletonInstance = new ActivityRecognitionHardware(context);
        }

        *instance = sSingletonInstance;
        REFCOUNT_ADD(*instance);
    }
    return NOERROR;
}

ECode ActivityRecognitionHardware::IsSupported(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = NativeIsSupported();
    return NOERROR;
}

ECode ActivityRecognitionHardware::GetSupportedActivities(
    /* [out, callee] */ ArrayOf<String>** activities)
{
    VALIDATE_NOT_NULL(activities);

    FAIL_RETURN(CheckPermissions())
    *activities = mSupportedActivities;
    REFCOUNT_ADD(*activities);
    return NOERROR;
}

ECode ActivityRecognitionHardware::IsActivitySupported(
    /* [in] */ const String& activity,
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    FAIL_RETURN(CheckPermissions())
    Int32 activityType = GetActivityType(activity);
    *supported = activityType != INVALID_ACTIVITY_TYPE;
    return NOERROR;
}

ECode ActivityRecognitionHardware::RegisterSink(
    /* [in] */ IIActivityRecognitionHardwareSink* sink,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(CheckPermissions())
    return mSinks->Register(sink, result);
}

ECode ActivityRecognitionHardware::UnregisterSink(
    /* [in] */ IIActivityRecognitionHardwareSink* sink,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(CheckPermissions())
    return mSinks->Unregister(sink, result);
}

ECode ActivityRecognitionHardware::EnableActivityEvent(
    /* [in] */ const String& activity,
    /* [in] */ Int32 eventType,
    /* [in] */ Int64 reportLatencyNs,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(CheckPermissions())

    Int32 activityType = GetActivityType(activity);
    if (activityType == INVALID_ACTIVITY_TYPE) {
        *result = FALSE;
        return NOERROR;
    }

    Int32 _result = NativeEnableActivityEvent(activityType, eventType, reportLatencyNs);
    *result =  _result == NATIVE_SUCCESS_RESULT;
    return NOERROR;
}

ECode ActivityRecognitionHardware::DisableActivityEvent(
    /* [in] */ const String& activityType,
    /* [in] */ Int32 eventType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(CheckPermissions())

    Int32 activityType = GetActivityType(activity);
    if (activityType == INVALID_ACTIVITY_TYPE) {
        *result = FALSE;
        return NOERROR;
    }

    Int32 _result = NativeDisableActivityEvent(activityType, eventType);
    *result = _result == NATIVE_SUCCESS_RESULT;
    return NOERROR;
}

ECode ActivityRecognitionHardware::Flush(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(CheckPermissions())
    Int32 _result = NativeFlush();
    *result = _result == NATIVE_SUCCESS_RESULT;
    return NOERROR;
}

void ActivityRecognitionHardware::OnActivityChanged(
    /* [in] */ ArrayOf<Event*>* events)
{
    if (events == NULL || events->GetLength() == 0) {
        SLogger::D(TAG, String("No events to broadcast for onActivityChanged."));
        return;
    }

    Int32 eventsLength = events->GetLength();
    AutoPtr<ArrayOf<IActivityRecognitionEvent*> > activityRecognitionEventArray
            = ArrayOf<IActivityRecognitionEvent*>::Alloc(eventsLength);

    for (Int32 i = 0; i < eventsLength; ++i) {
        AutoPtr<Event> event = (*events)[i];
        String activityName = GetActivityName(event->mActivity);
        AutoPtr<IActivityRecognitionEvent> tmp = new ActivityRecognitionEvent(activityName, event->mType, event->mTimestamp);
        activityRecognitionEventArray->Set(i, tmp);
    }
    AutoPtr<IActivityChangedEvent> activityChangedEvent =
            new ActivityChangedEvent(activityRecognitionEventArray);

    Int32 size = mSinks->BeginBroadcast();
    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IIActivityRecognitionHardwareSink> sink;
        mSinks->GetBroadcastItem(i, (IIActivityRecognitionHardwareSink**)&sink);
        ECode ec = NOERROR;
        //try {
            ec = sink->OnActivityChanged(activityChangedEvent);
        //} catch (RemoteException e) {
            if (FAILED(ec)) {
                Slogger::E(TAG, String("Error delivering activity changed event.")/*, e*/);
            }

        //}
    }
    mSinks->FinishBroadcast();
}

String ActivityRecognitionHardware::GetActivityName(
    /* [in] */ Int32 activityType)
{
    if (activityType < 0 || activityType >= mSupportedActivities->GetLength()) {
        StringBuilder sb;
        sb += "Invalid ActivityType: ";
        sb += activityType;
        sb += ", SupportedActivities: ";
        sb += mSupportedActivities->GetLength();
        String message;
        sb.ToString(&message);
        Slogger::E(TAG, message);
        return String(NULL);
    }

    return (*mSupportedActivities)[activityType];
}

Int32 ActivityRecognitionHardware::GetActivityType(
    /* [in] */ const String& activity)
{
    if (TextUtils::IsEmpty(activity)) {
        return INVALID_ACTIVITY_TYPE;
    }

    Int32 supportedActivitiesLength = mSupportedActivities->GetLength();
    for (Int32 i = 0; i < supportedActivitiesLength; ++i) {
        if (activity.Equals((*mSupportedActivities)[i])) {
            return i;
        }
    }

    return INVALID_ACTIVITY_TYPE;
}

ECode ActivityRecognitionHardware::CheckPermissions()
{
    StringBuilder sb;
    sb += "Permission \'";
    sb += HARDWARE_PERMISSION;
    sb += "\' not granted to access ActivityRecognitionHardware";
    String message;
    sb.ToString(&message);

    return mContext->EnforceCallingPermission(HARDWARE_PERMISSION, message);
}

ECode ActivityRecognitionHardware::FetchSupportedActivities(
    /* [out, callee] */ ArrayOf<String>** activities)
{
    AutoPtr<ArrayOf<String> > supportedActivities = NativeGetSupportedActivities();
    if (supportedActivities != NULL) {
        *activities = supportedActivities;
        return NOERROR;
    }

    *activities = ArrayOf<String>::Alloc(0);
    return NOERROR;
}

// keep base connection data from the HAL
static activity_recognition_module_t* sModule = NULL;
static activity_recognition_device_t* sDevice = NULL;

static jobject sCallbacksObject = NULL;
static jmethodID sOnActivityChanged = NULL;

void ActivityRecognitionHardware::NativeClassInit()
{
    // open the hardware module
    int error = hw_get_module(
            ACTIVITY_RECOGNITION_HARDWARE_MODULE_ID,
            (const hw_module_t**) &sModule);
    if (error != 0) {
        ALOGE("Error hw_get_module: %d", error);
        return;
    }

    error = activity_recognition_open(&sModule->common, &sDevice);
    if (error != 0) {
        ALOGE("Error opening device: %d", error);
        return;
    }

    // get references to the Java provided methods
    sOnActivityChanged = env->GetMethodID(
            clazz,
            "onActivityChanged",
            "([Landroid/hardware/location/ActivityRecognitionHardware$Event;)V");
    if (sOnActivityChanged == NULL) {
        ALOGE("Error obtaining ActivityChanged callback.");
        return;
    }

    // register callbacks
    sDevice->register_activity_callback(sDevice, &sCallbacks);
}

Boolean ActivityRecognitionHardware::NativeIsSupported()
{
    if (sModule != NULL && sDevice != NULL ) {
        return TRUE;
    }
    return FALSE;
}

void ActivityRecognitionHardware::NativeInitialize()
{
    if (sCallbacksObject == NULL) {
        sCallbacksObject = env->NewGlobalRef(obj);
    } else {
        ALOGD("Callbacks Object was already initialized.");
    }

    if (sDevice != NULL) {
        sDevice->register_activity_callback(sDevice, &sCallbacks);
    } else {
        ALOGD("ActivityRecognition device not found during initialization.");
    }
}

void ActivityRecognitionHardware::NativeRelease()
{
    if (sDevice == NULL) {
        return;
    }

    int error = activity_recognition_close(sDevice);
    if (error != 0) {
        ALOGE("Error closing device: %d", error);
        return;
    }
    return;
}

AutoPtr<ArrayOf<String> > ActivityRecognitionHardware::NativeGetSupportedActivities()
{
    if (sModule == NULL) {
        return NULL;
    }

    char const* const* list = NULL;
    int list_size = sModule->get_supported_activities_list(sModule, &list);
    if (list_size <= 0 || list == NULL) {
        return NULL;
    }

    // jclass string_class = env->FindClass("java/lang/String");
    // if (string_class == NULL) {
    //     ALOGE("Unable to find String class for supported activities.");
    //     return NULL;
    // }

    //jobjectArray string_array = env->NewObjectArray(list_size, string_class, NULL);
    AutoPtr<ArrayOf<String> > string_array = ArrayOf<String>::Alloc(list_size);
    if (string_array == NULL) {
        ALOGE("Unable to create string array for supported activities.");
        return NULL;
    }

    for (int i = 0; i < list_size; ++i) {
        const char* string_ptr = const_cast<const char*>(list[i]);
        //jstring string = env->NewStringUTF(string_ptr);
        String string = String(string_ptr);
        //env->SetObjectArrayElement(string_array, i, string);
        string_array->Set(i, string);
    }

    return string_array;
}

Int32 ActivityRecognitionHardware::NativeEnableActivityEvent(
    /* [in] */ Int32 activityType,
    /* [in] */ Int32 eventType,
    /* [in] */ Int64 reportLatenceNs)
{
    return sDevice->enable_activity_event(
            sDevice,
            (uint32_t) activity_handle,
            (uint32_t) event_type,
            report_latency_ns);
}

Int32 ActivityRecognitionHardware::NativeDisableActivityEvent(
    /* [in] */ Int32 activityType,
    /* [in] */ Int32 eventType)
{
    return sDevice->disable_activity_event(
            sDevice,
            (uint32_t) activity_handle,
            (uint32_t) event_type);
}

Int32 ActivityRecognitionHardware::NativeFlush()
{
    return sDevice->flush(sDevice);
}

} //Location
} //Hardware
} //Droid
} //Elastos