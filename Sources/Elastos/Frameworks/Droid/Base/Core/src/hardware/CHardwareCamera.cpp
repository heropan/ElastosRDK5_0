
#include "hardware/CHardwareCamera.h"
#include <elastos/StringBuilder.h>
#include "text/TextUtils.h"
#include "text/CSimpleStringSplitter.h"
#include <utils/String8.h>
#include <utils/Errors.h>
#include "graphics/CRect.h"
#include "graphics/CSurfaceTexture.h"
#include "graphics/CBitmapFactory.h"
#include <elastos/List.h>
#include "os/Looper.h"
#include "os/Process.h"
#include "os/ServiceManager.h"
#include <gui/BufferQueue.h>
#include <gui/SurfaceTexture.h>
#include <gui/Surface.h>
#include <elastos/StringUtils.h>
#include "privacy/CPrivacySettingsManager.h"


using Elastos::Core::StringUtils;
using Elastos::Utility::List;
using Elastos::Core::StringBuilder;
using Elastos::Core::CInteger32;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::IByteArrayOutputStream;
using Elastos::IO::CByteArrayOutputStream;
using Elastos::Core::CObjectContainer;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;
using Elastos::Droid::Graphics::BitmapCompressFormat_JPEG;
using Elastos::Droid::Privacy::CPrivacySettingsManager;
using Elastos::Droid::Privacy::IIPrivacySettingsManager;
using Elastos::Droid::Privacy::IPrivacySettings;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Graphics::CSurfaceTexture;
using Elastos::Droid::Graphics::IImageFormat;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::Looper;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Media::IIAudioService;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Text::ISimpleStringSplitter;
using Elastos::Droid::Text::CSimpleStringSplitter;


namespace Elastos {
namespace Droid {
namespace Hardware {

String CHardwareCamera::TAG = String("Camera");
const Int32 CHardwareCamera::CAMERA_MSG_ERROR            = 0x001;
const Int32 CHardwareCamera::CAMERA_MSG_SHUTTER          = 0x002;
const Int32 CHardwareCamera::CAMERA_MSG_FOCUS            = 0x004;
const Int32 CHardwareCamera::CAMERA_MSG_ZOOM             = 0x008;
const Int32 CHardwareCamera::CAMERA_MSG_PREVIEW_FRAME    = 0x010;
const Int32 CHardwareCamera::CAMERA_MSG_VIDEO_FRAME      = 0x020;
const Int32 CHardwareCamera::CAMERA_MSG_POSTVIEW_FRAME   = 0x040;
const Int32 CHardwareCamera::CAMERA_MSG_RAW_IMAGE        = 0x080;
const Int32 CHardwareCamera::CAMERA_MSG_COMPRESSED_IMAGE = 0x100;
const Int32 CHardwareCamera::CAMERA_MSG_RAW_IMAGE_NOTIFY = 0x200;
const Int32 CHardwareCamera::CAMERA_MSG_PREVIEW_METADATA = 0x400;
const Int32 CHardwareCamera::CAMERA_MSG_FOCUS_MOVE       = 0x800;
const Int32 CHardwareCamera::CAMERA_FACE_DETECTION_HW = 0;
const Int32 CHardwareCamera::CAMERA_FACE_DETECTION_SW = 1;

const Int32 CHardwareCamera::IS_ALLOWED = -1;
const Int32 CHardwareCamera::IS_NOT_ALLOWED = -2;
const Int32 CHardwareCamera::GOT_ERROR = -3;

const String CHardwareCamera::PRIVACY_TAG = String("PM,Camera");
AutoPtr<IContext> CHardwareCamera::mContext;
AutoPtr<IPrivacySettingsManager> CHardwareCamera::mPrivacySettingsManager;
Boolean CHardwareCamera::mPrivacyMode = FALSE;
AutoPtr<IIPackageManager> CHardwareCamera::mPackageManager;


String CHardwareCamera::Parameters::KEY_PREVIEW_SIZE = String("preview-size");
String CHardwareCamera::Parameters::KEY_PREVIEW_FORMAT = String("preview-format");
String CHardwareCamera::Parameters::KEY_PREVIEW_FRAME_RATE = String("preview-frame-rate");
String CHardwareCamera::Parameters::KEY_PREVIEW_FPS_RANGE = String("preview-fps-range");
String CHardwareCamera::Parameters::KEY_PICTURE_SIZE = String("picture-size");
String CHardwareCamera::Parameters::KEY_PICTURE_FORMAT = String("picture-format");
String CHardwareCamera::Parameters::KEY_JPEG_THUMBNAIL_SIZE = String("jpeg-thumbnail-size");
String CHardwareCamera::Parameters::KEY_JPEG_THUMBNAIL_WIDTH = String("jpeg-thumbnail-width");
String CHardwareCamera::Parameters::KEY_JPEG_THUMBNAIL_HEIGHT = String("jpeg-thumbnail-height");
String CHardwareCamera::Parameters::KEY_JPEG_THUMBNAIL_QUALITY = String("jpeg-thumbnail-quality");
String CHardwareCamera::Parameters::KEY_JPEG_QUALITY = String("jpeg-quality");
String CHardwareCamera::Parameters::KEY_ROTATION = String("rotation");
String CHardwareCamera::Parameters::KEY_GPS_LATITUDE = String("gps-latitude");
String CHardwareCamera::Parameters::KEY_GPS_LONGITUDE = String("gps-longitude");
String CHardwareCamera::Parameters::KEY_GPS_ALTITUDE = String("gps-altitude");
String CHardwareCamera::Parameters::KEY_GPS_TIMESTAMP = String("gps-timestamp");
String CHardwareCamera::Parameters::KEY_GPS_PROCESSING_METHOD = String("gps-processing-method");
String CHardwareCamera::Parameters::KEY_WHITE_BALANCE = String("whitebalance");
String CHardwareCamera::Parameters::KEY_EFFECT = String("effect");
String CHardwareCamera::Parameters::KEY_ANTIBANDING = String("antibanding");
String CHardwareCamera::Parameters::KEY_SCENE_MODE = String("scene-mode");
String CHardwareCamera::Parameters::KEY_FLASH_MODE = String("flash-mode");
String CHardwareCamera::Parameters::KEY_FOCUS_MODE = String("focus-mode");
String CHardwareCamera::Parameters::KEY_FOCUS_AREAS = String("focus-areas");
String CHardwareCamera::Parameters::KEY_MAX_NUM_FOCUS_AREAS = String("max-num-focus-areas");
String CHardwareCamera::Parameters::KEY_FOCAL_LENGTH = String("focal-length");
String CHardwareCamera::Parameters::KEY_HORIZONTAL_VIEW_ANGLE = String("horizontal-view-angle");
String CHardwareCamera::Parameters::KEY_VERTICAL_VIEW_ANGLE = String("vertical-view-angle");
String CHardwareCamera::Parameters::KEY_EXPOSURE_COMPENSATION = String("exposure-compensation");
String CHardwareCamera::Parameters::KEY_MAX_EXPOSURE_COMPENSATION = String("max-exposure-compensation");
String CHardwareCamera::Parameters::KEY_MIN_EXPOSURE_COMPENSATION = String("min-exposure-compensation");
String CHardwareCamera::Parameters::KEY_EXPOSURE_COMPENSATION_STEP = String("exposure-compensation-step");
String CHardwareCamera::Parameters::KEY_AUTO_EXPOSURE_LOCK = String("auto-exposure-lock");
String CHardwareCamera::Parameters::KEY_AUTO_EXPOSURE_LOCK_SUPPORTED = String("auto-exposure-lock-supported");
String CHardwareCamera::Parameters::KEY_AUTO_WHITEBALANCE_LOCK = String("auto-whitebalance-lock");
String CHardwareCamera::Parameters::KEY_AUTO_WHITEBALANCE_LOCK_SUPPORTED = String("auto-whitebalance-lock-supported");
String CHardwareCamera::Parameters::KEY_METERING_AREAS = String("metering-areas");
String CHardwareCamera::Parameters::KEY_MAX_NUM_METERING_AREAS = String("max-num-metering-areas");
String CHardwareCamera::Parameters::KEY_ZOOM = String("zoom");
String CHardwareCamera::Parameters::KEY_MAX_ZOOM = String("max-zoom");
String CHardwareCamera::Parameters::KEY_ZOOM_RATIOS = String("zoom-ratios");
String CHardwareCamera::Parameters::KEY_ZOOM_SUPPORTED = String("zoom-supported");
String CHardwareCamera::Parameters::KEY_SMOOTH_ZOOM_SUPPORTED = String("smooth-zoom-supported");
String CHardwareCamera::Parameters::KEY_FOCUS_DISTANCES = String("focus-distances");
String CHardwareCamera::Parameters::KEY_VIDEO_SIZE = String("video-size");
String CHardwareCamera::Parameters::KEY_PREFERRED_PREVIEW_SIZE_FOR_VIDEO = String("preferred-preview-size-for-video");
String CHardwareCamera::Parameters::KEY_MAX_NUM_DETECTED_FACES_HW = String("max-num-detected-faces-hw");
String CHardwareCamera::Parameters::KEY_MAX_NUM_DETECTED_FACES_SW = String("max-num-detected-faces-sw");
String CHardwareCamera::Parameters::KEY_RECORDING_HINT = String("recording-hint");
String CHardwareCamera::Parameters::KEY_VIDEO_SNAPSHOT_SUPPORTED = String("video-snapshot-supported");
String CHardwareCamera::Parameters::KEY_VIDEO_STABILIZATION = String("video-stabilization");
String CHardwareCamera::Parameters::KEY_VIDEO_STABILIZATION_SUPPORTED = String("video-stabilization-supported");
String CHardwareCamera::Parameters::SUPPORTED_VALUES_SUFFIX = String("-values");
String CHardwareCamera::Parameters::TRUESTR = String("TRUE");
String CHardwareCamera::Parameters::FALSESTR = String("FALSE");
String CHardwareCamera::Parameters::PIXEL_FORMAT_YUV422SP = String("yuv422sp");
String CHardwareCamera::Parameters::PIXEL_FORMAT_YUV420SP = String("yuv420sp");
String CHardwareCamera::Parameters::PIXEL_FORMAT_YUV422I = String("yuv422i-yuyv");
String CHardwareCamera::Parameters::PIXEL_FORMAT_YUV420P = String("yuv420p");
String CHardwareCamera::Parameters::PIXEL_FORMAT_RGB565 = String("rgb565");
String CHardwareCamera::Parameters::PIXEL_FORMAT_JPEG = String("jpeg");
String CHardwareCamera::Parameters::PIXEL_FORMAT_BAYER_RGGB = String("bayer-rggb");

static Mutex sLock;
android::sp<android::Camera> CHardwareCamera::get_native_camera(
    /* [in] */ CHardwareCamera* thiz,
    /* [in] */ JNICameraContext** pContext)
{
    android::sp<android::Camera> camera;
    Mutex::Autolock _l(sLock);
    JNICameraContext* context = reinterpret_cast<JNICameraContext*>(thiz->mNativeContext)/*(env->GetIntField(thiz, fields.context))*/;
    if (context != NULL) {
        camera = context->getCamera();
    }
    // ALOGV("get_native_camera: context=%p, camera=%p", context, camera.get());
    if (camera == 0) {
        assert(0);
        // jniThrowRuntimeException(env, "Method called after release()");
    }

    if (pContext != NULL) *pContext = context;
    return camera;
}

bool CHardwareCamera::JNICameraContext::isRawImageCallbackBufferAvailable() const
{
    return !mRawImageCallbackBuffers.isEmpty();
}

CHardwareCamera::JNICameraContext::JNICameraContext(
    /* [in] */ CHardwareCamera* weak_this,
    /* [in] */ /*jclass*/CHardwareCamera* clazz,
    /* [in] */ const android::sp<android::Camera>& camera)
{
    mCameraJObjectWeak = weak_this;
    // mCameraJClass = clazz/*(jclass)env->NewGlobalRef(clazz)*/;
    mCamera = camera;

    // jclass faceClazz = env->FindClass("android/hardware/Camera$Face");
    // mFaceClass = (jclass) env->NewGlobalRef(faceClazz);

    // jclass rectClazz = env->FindClass("android/graphics/Rect");
    // mRectClass = (jclass) env->NewGlobalRef(rectClazz);

    mManualBufferMode = false;
    mManualCameraCallbackSet = false;
}

CHardwareCamera::JNICameraContext::~JNICameraContext()
{
    release();
}

void CHardwareCamera::JNICameraContext::release()
{
    // ALOGV("release");
    Mutex::Autolock _l(mLock);
    // JNIEnv *env = AndroidRuntime::getJNIEnv();

    // if (mCameraJObjectWeak != NULL) {
    //     env->DeleteGlobalRef(mCameraJObjectWeak);
    //     mCameraJObjectWeak = NULL;
    // }
    // if (mCameraJClass != NULL) {
    //     env->DeleteGlobalRef(mCameraJClass);
    //     mCameraJClass = NULL;
    // }
    // if (mFaceClass != NULL) {
    //     env->DeleteGlobalRef(mFaceClass);
    //     mFaceClass = NULL;
    // }
    // if (mRectClass != NULL) {
    //     env->DeleteGlobalRef(mRectClass);
    //     mRectClass = NULL;
    // }
    clearCallbackBuffers_l();
    mCamera.clear();
}

void CHardwareCamera::JNICameraContext::notify(
    /* [in] */ int32_t msgType,
    /* [in] */ int32_t ext1,
    /* [in] */ int32_t ext2)
{
    // ALOGV("notify");

    // VM pointer will be NULL if object is released
    Mutex::Autolock _l(mLock);
    if (mCameraJObjectWeak == NULL) {
        // ALOGW("callback on dead camera object");
        return;
    }

    // JNIEnv *env = AndroidRuntime::getJNIEnv();

    /*
     * If the notification or msgType is CAMERA_MSG_RAW_IMAGE_NOTIFY, change it
     * to CAMERA_MSG_RAW_IMAGE since CAMERA_MSG_RAW_IMAGE_NOTIFY is not exposed
     * to the Java app.
     */
    if (msgType == CAMERA_MSG_RAW_IMAGE_NOTIFY) {
        msgType = CAMERA_MSG_RAW_IMAGE;
    }

    PostEventFromNative(mCameraJObjectWeak, msgType, ext1, ext2, NULL);

    // env->CallStaticVoidMethod(mCameraJClass, fields.post_event,
    //         mCameraJObjectWeak, msgType, ext1, ext2, NULL);
}

AutoPtr<ArrayOf<Byte> > CHardwareCamera::JNICameraContext::getCallbackBuffer(
    /* [in] */ android::Vector<ArrayOf<Byte>*>* buffers,
    /* [in] */ size_t bufferSize)
{
    AutoPtr<ArrayOf<Byte> > obj;

    // Vector access should be protected by lock in postData()
    if (!buffers->isEmpty()) {
        // ALOGV("Using callback buffer from queue of length %d", buffers->size());
        AutoPtr<ArrayOf<Byte> > globalBuffer = buffers->itemAt(0);
        buffers->removeAt(0);

        obj = globalBuffer/*(jbyteArray)env->NewLocalRef(globalBuffer)*/;
        // env->DeleteGlobalRef(globalBuffer);

        if (obj != NULL) {
            Int32 bufferLength = obj->GetLength();
            if ((int)bufferLength < (int)bufferSize) {
                // ALOGE("Callback buffer was too small! Expected %d bytes, but got %d bytes!",
                //     bufferSize, bufferLength);
                // env->DeleteLocalRef(obj);
                return NULL;
            }
        }
    }

    return obj;
}

void CHardwareCamera::JNICameraContext::copyAndPost(
    /* [in] */ const android::sp<android::IMemory>& dataPtr,
    /* [in] */ int msgType)
{
    AutoPtr<ArrayOf<Byte> > obj;

    // allocate Java byte array and copy data
    if (dataPtr != NULL) {
        ssize_t offset;
        size_t size;
        android::sp<android::IMemoryHeap> heap = dataPtr->getMemory(&offset, &size);
        // ALOGV("copyAndPost: off=%ld, size=%d", offset, size);
        uint8_t *heapBase = (uint8_t*)heap->base();

        if (heapBase != NULL) {
            const Byte* data = reinterpret_cast<const Byte*>(heapBase + offset);

            if (msgType == CAMERA_MSG_RAW_IMAGE) {
                obj = getCallbackBuffer(&mRawImageCallbackBuffers, size);
            } else if (msgType == CAMERA_MSG_PREVIEW_FRAME && mManualBufferMode) {
                obj = getCallbackBuffer(&mCallbackBuffers, size);

                if (mCallbackBuffers.isEmpty()) {
                    // ALOGV("Out of buffers, clearing callback!");
                    mCamera->setPreviewCallbackFlags(CAMERA_FRAME_CALLBACK_FLAG_NOOP);
                    mManualCameraCallbackSet = false;

                    if (obj == NULL) {
                        return;
                    }
                }
            } else {
                // ALOGV("Allocating callback buffer");
                // obj = env->NewByteArray(size);
                obj = ArrayOf<Byte>::Alloc(size);
            }

            if (obj == NULL) {
                // ALOGE("Couldn't allocate byte array for JPEG data");
                // env->ExceptionClear();
            } else {
                //TODO
                // env->SetByteArrayRegion(obj, 0, size, data);
                obj->Copy(data, size);
            }
        } else {
            // ALOGE("image heap is NULL");
        }
    }

    // post image data to Java
    // env->CallStaticVoidMethod(mCameraJClass, fields.post_event,
    //         mCameraJObjectWeak, msgType, 0, 0, obj);
    PostEventFromNative(mCameraJObjectWeak, msgType, 0, 0, obj);
    // if (obj) {
    //     env->DeleteLocalRef(obj);
    // }
}

void CHardwareCamera::JNICameraContext::postData(
    /* [in] */ int32_t msgType,
    /* [in] */ const android::sp<android::IMemory>& dataPtr,
    /* [in] */ camera_frame_metadata_t *metadata)
{
    // VM pointer will be NULL if object is released
    Mutex::Autolock _l(mLock);
    // JNIEnv *env = AndroidRuntime::getJNIEnv();
    if (mCameraJObjectWeak == NULL) {
        // ALOGW("callback on dead camera object");
        return;
    }

    int32_t dataMsgType = msgType & ~CAMERA_MSG_PREVIEW_METADATA;

    // return data based on callback type
    switch (dataMsgType) {
        case CAMERA_MSG_VIDEO_FRAME:
            // should never happen
            break;

        // For backward-compatibility purpose, if there is no callback
        // buffer for raw image, the callback returns NULL.
        case CAMERA_MSG_RAW_IMAGE:
            // ALOGV("rawCallback");
            if (mRawImageCallbackBuffers.isEmpty()) {
                // env->CallStaticVoidMethod(mCameraJClass, fields.post_event,
                //         mCameraJObjectWeak, dataMsgType, 0, 0, NULL);
                PostEventFromNative(mCameraJObjectWeak, dataMsgType, 0, 0, NULL);
            } else {
                copyAndPost(dataPtr, dataMsgType);
            }
            break;

        // There is no data.
        case 0:
            break;

        default:
            // ALOGV("dataCallback(%d, %p)", dataMsgType, dataPtr.get());
            copyAndPost(dataPtr, dataMsgType);
            break;
    }

    // post frame metadata to Java
    if (metadata && (msgType & CAMERA_MSG_PREVIEW_METADATA)) {
        postMetadata(CAMERA_MSG_PREVIEW_METADATA, metadata);
    }
}

void CHardwareCamera::JNICameraContext::postDataTimestamp(
    /* [in] */ nsecs_t timestamp,
    /* [in] */ int32_t msgType,
    /* [in] */ const android::sp<android::IMemory>& dataPtr)
{
    // TODO: plumb up to Java. For now, just drop the timestamp
    postData(msgType, dataPtr, NULL);
}

void CHardwareCamera::JNICameraContext::postMetadata(
    /* [in] */ int32_t msgType,
    /* [in] */ camera_frame_metadata_t *metadata)
{
    // jobjectArray obj = NULL;
    // obj = (jobjectArray) env->NewObjectArray(metadata->number_of_faces,
    //                                          mFaceClass, NULL);
    AutoPtr<ArrayOf<Face*> > obj = ArrayOf<Face*>::Alloc(metadata->number_of_faces);
    if (obj == NULL) {
        // ALOGE("Couldn't allocate face metadata array");
        return;
    }

    for (int i = 0; i < metadata->number_of_faces; i++) {
        // jobject face = env->NewObject(mFaceClass, fields.face_constructor);
        AutoPtr<Face> face = new Face();

        // env->SetObjectArrayElement(obj, i, face);
        (*obj)[i] = face;

        // jobject rect = env->NewObject(mRectClass, fields.rect_constructor);
        AutoPtr<IRect> rect;
        CRect::New((IRect**)&rect);
        assert(rect != NULL);

        // env->SetIntField(rect, fields.rect_left, metadata->faces[i].rect[0]);
        // env->SetIntField(rect, fields.rect_top, metadata->faces[i].rect[1]);
        // env->SetIntField(rect, fields.rect_right, metadata->faces[i].rect[2]);
        // env->SetIntField(rect, fields.rect_bottom, metadata->faces[i].rect[3]);
        rect->SetLeft(metadata->faces[i].rect[0]);
        rect->SetTop(metadata->faces[i].rect[1]);
        rect->SetRight(metadata->faces[i].rect[2]);
        rect->SetBottom(metadata->faces[i].rect[3]);


        // env->SetObjectField(face, fields.face_rect, rect);
        face->mRect = rect;

        // env->SetIntField(face, fields.face_score, metadata->faces[i].score);
        face->mScore = metadata->faces[i].score;

        // env->DeleteLocalRef(face);
        // env->DeleteLocalRef(rect);
    }

    // env->CallStaticVoidMethod(mCameraJClass, fields.post_event,
    //         mCameraJObjectWeak, msgType, 0, 0, obj);
    PostEventFromNative(mCameraJObjectWeak, msgType, 0, 0, obj);
    // env->DeleteLocalRef(obj);
}

void CHardwareCamera::JNICameraContext::setCallbackMode(
    /* [in] */ bool installed,
    /* [in] */ bool manualMode)
{
    Mutex::Autolock _l(mLock);
    mManualBufferMode = manualMode;
    mManualCameraCallbackSet = false;

    // In order to limit the over usage of binder threads, all non-manual buffer
    // callbacks use CAMERA_FRAME_CALLBACK_FLAG_BARCODE_SCANNER mode now.
    //
    // Continuous callbacks will have the callback re-registered from handleMessage.
    // Manual buffer mode will operate as fast as possible, relying on the finite supply
    // of buffers for throttling.

    if (!installed) {
        mCamera->setPreviewCallbackFlags(CAMERA_FRAME_CALLBACK_FLAG_NOOP);
        clearCallbackBuffers_l(&mCallbackBuffers);
    } else if (mManualBufferMode) {
        if (!mCallbackBuffers.isEmpty()) {
            mCamera->setPreviewCallbackFlags(CAMERA_FRAME_CALLBACK_FLAG_CAMERA);
            mManualCameraCallbackSet = TRUE;
        }
    } else {
        mCamera->setPreviewCallbackFlags(CAMERA_FRAME_CALLBACK_FLAG_BARCODE_SCANNER);
        clearCallbackBuffers_l(&mCallbackBuffers);
    }
}

android::sp<android::Camera> CHardwareCamera::JNICameraContext::getCamera()
{
    Mutex::Autolock _l(mLock);
    return mCamera;
}

void CHardwareCamera::JNICameraContext::addCallbackBuffer(
    /* [in] */ ArrayOf<Byte>* cbb,
    /* [in] */ int msgType)
{
    // ALOGV("addCallbackBuffer: 0x%x", msgType);
    if (cbb != NULL) {
        Mutex::Autolock _l(mLock);
        switch (msgType) {
            case CAMERA_MSG_PREVIEW_FRAME: {
                // jbyteArray callbackBuffer = (jbyteArray)env->NewGlobalRef(cbb);
                cbb->AddRef();
                mCallbackBuffers.push(cbb);

                // ALOGV("Adding callback buffer to queue, %d total",
                //         mCallbackBuffers.size());

                // We want to make sure the camera knows we're ready for the
                // next frame. This may have come unset had we not had a
                // callbackbuffer ready for it last time.
                if (mManualBufferMode && !mManualCameraCallbackSet) {
                    mCamera->setPreviewCallbackFlags(CAMERA_FRAME_CALLBACK_FLAG_CAMERA);
                    mManualCameraCallbackSet = TRUE;
                }
                break;
            }
            case CAMERA_MSG_RAW_IMAGE: {
                // jbyteArray callbackBuffer = (jbyteArray)env->NewGlobalRef(cbb);
                cbb->AddRef();
                mRawImageCallbackBuffers.push(cbb);
                break;
            }
            default: {
                assert(0);
                // jniThrowException(env,
                //         "java/lang/IllegalArgumentException",
                //         "Unsupported message type");
                return;
            }
        }
    } else {
       // ALOGE("Null byte array!");
    }
}

void CHardwareCamera::JNICameraContext::clearCallbackBuffers_l()
{
    // TODO : call Release() of each element in container.
    clearCallbackBuffers_l(&mCallbackBuffers);
    clearCallbackBuffers_l(&mRawImageCallbackBuffers);
}

void CHardwareCamera::JNICameraContext::clearCallbackBuffers_l(
    /* [in] */ android::Vector<ArrayOf<Byte>*>* buffers)
{
    // ALOGV("Clearing callback buffers, %d remained", buffers->size());
    while (!buffers->isEmpty()) {
        // env->DeleteGlobalRef(buffers->top());
        //TODO :need to release?
        //buffers->top()->Release();

        buffers->pop();
    }
}

CHardwareCamera::CameraInfo::CameraInfo()
    : mFacing(0)
    , mOrientation(0)
    , mCanDisableShutterSound(FALSE)
{
    CHardwareCamera::Initiate();
}

UInt32 CHardwareCamera::CameraInfo::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CHardwareCamera::CameraInfo::Release()
{
    return ElRefBase::Release();
}

PInterface CHardwareCamera::CameraInfo::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_ICameraInfo == riid) {
        return (ICameraInfo *)this;
    }

    return NULL;
}

ECode CHardwareCamera::CameraInfo::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(ICameraInfo*)this) {
        *pIID = EIID_ICameraInfo;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CHardwareCamera::CameraInfo::SetFacing(
    /* [in] */ Int32 facing)
{
    mFacing = facing;
    return NOERROR;
}

ECode CHardwareCamera::CameraInfo::GetFacing(
    /* [out] */ Int32* facing)
{
    assert(facing != NULL);
    (*facing) = mFacing;
    return NOERROR;
}

ECode CHardwareCamera::CameraInfo::SetOrientation(
    /* [in] */ Int32 orientation)
{
    mOrientation = orientation;
    return NOERROR;
}

ECode CHardwareCamera::CameraInfo::GetOrientation(
    /* [out] */ Int32* orientation)
{
    assert(orientation != NULL);
    *orientation = mOrientation;
    return NOERROR;
}

ECode CHardwareCamera::CameraInfo::SetCanDisableShutterSound(
    /* [in] */ Boolean can)
{
    mCanDisableShutterSound = can;
    return NOERROR;
}

ECode CHardwareCamera::CameraInfo::GetCanDisableShutterSound(
    /* [out] */ Boolean* can)
{
    assert(can != NULL);
    *can = mCanDisableShutterSound;
    return NOERROR;
}

CHardwareCamera::Face::Face()
    : mScore(0)
    , mId(-1)
{
}

UInt32 CHardwareCamera::Face::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CHardwareCamera::Face::Release()
{
    return ElRefBase::Release();
}

PInterface CHardwareCamera::Face::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IFace == riid) {
        return (IFace *)this;
    }

    return NULL;
}

ECode CHardwareCamera::Face::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IFace*)this) {
        *pIID = EIID_IFace;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

CHardwareCamera::Size::Size(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
    : mWidth(w)
    , mHeight(h)
{
}

UInt32 CHardwareCamera::Size::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CHardwareCamera::Size::Release()
{
    return ElRefBase::Release();
}

PInterface CHardwareCamera::Size::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_ICameraSize == riid) {
        return (ICameraSize *)this;
    }

    return NULL;
}

ECode CHardwareCamera::Size::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(ICameraSize*)this) {
        *pIID = EIID_ICameraSize;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CHardwareCamera::Size::EqualsEx(
    /* [in] */ ICameraSize* obj,
    /* [out] */ Boolean* e)
{
    VALIDATE_NOT_NULL(e);
    *e = FALSE;
    if (obj == NULL) {
        return NOERROR;
    }

    Size* s = (Size*) obj;
    *e = mWidth == s->mWidth && mHeight == s->mHeight;
    return NOERROR;
}

ECode CHardwareCamera::Size::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* e)
{
    VALIDATE_NOT_NULL(e);
    *e = FALSE;
    return EqualsEx(ICameraSize::Probe(obj), e);
}

ECode CHardwareCamera::Size::GetHashCode(
    /* [out] */ Int32* code)
{
    assert(code != NULL);
    *code = mWidth * 32713 + mHeight;
    return NOERROR;
}

CHardwareCamera::Area::Area(
    /* [in] */ IRect* rect,
    /* [in] */ Int32 weight)
    : mRect(rect)
    , mWeight(weight)
{
}

UInt32 CHardwareCamera::Area::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CHardwareCamera::Area::Release()
{
    return ElRefBase::Release();
}

PInterface CHardwareCamera::Area::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_ICameraArea == riid) {
        return (ICameraArea *)this;
    }

    return NULL;
}

ECode CHardwareCamera::Area::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(ICameraArea*)this) {
        *pIID = EIID_ICameraArea;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CHardwareCamera::Area::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* e)
{
    VALIDATE_NOT_NULL(e);
    *e = FALSE;
    if (ICameraArea::Probe(obj) == NULL) {
        return NOERROR;
    }

    Area* a = (Area*) obj;
    if (mRect == NULL) {
        if (a->mRect != NULL) return NOERROR;
    } else {
        Boolean tmp = FALSE;
        mRect->Equals(a->mRect, &tmp);
        if (!tmp) return NOERROR;
    }

    *e = mWeight == a->mWeight;
    return NOERROR;
}

ECode CHardwareCamera::Area::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    *hash = 0; // do not care.
    return NOERROR;
}

CHardwareCamera::Parameters::Parameters()
{
    // mMap = new HashMap<String, String>(64);
}

UInt32 CHardwareCamera::Parameters::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CHardwareCamera::Parameters::Release()
{
    return ElRefBase::Release();
}

PInterface CHardwareCamera::Parameters::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IParameters == riid) {
        return (IParameters *)this;
    }

    return NULL;
}

ECode CHardwareCamera::Parameters::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IParameters*)this) {
        *pIID = EIID_IParameters;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CHardwareCamera::Parameters::Flatten(
    /* [out] */ String* content)
{
    assert(content != NULL);
    AutoPtr<StringBuilder> flattened = new StringBuilder(128);
    HashMap<String, String>::Iterator ator = mMap.Begin();
    for (; ator != mMap.End(); ++ator) {
        String k = ator->mFirst;
        flattened->AppendString(k);
        flattened->AppendString(String("="));
        flattened->AppendString(ator->mSecond);
        flattened->AppendString(String(";"));
    }

    // chop off the extra semicolon at the end
    flattened->DeleteChar(flattened->GetLength() - 1);
    *content = flattened->ToString();
    return NOERROR;
}

ECode CHardwareCamera::Parameters::Unflatten(
    /* [in] */ const String& flattened)
{
    mMap.Clear();

    AutoPtr<ISimpleStringSplitter> splitter;
    CSimpleStringSplitter::New(';', (ISimpleStringSplitter**)&splitter);
    splitter->SetString(flattened);

    Boolean has = FALSE;
    while (splitter->HasNext(&has), has) {
        AutoPtr<ICharSequence> cs;
        splitter->Next((IInterface**)&cs);
        String kv;
        cs->ToString(&kv);

        Int32 pos = kv.IndexOf('=');
        if (pos == -1) {
            continue;
        }
        String k = kv.Substring(0, pos);
        String v = kv.Substring(pos + 1);
        mMap[k] = v;
    }

    return NOERROR;
}

ECode CHardwareCamera::Parameters::Remove(
    /* [in] */ const String& key)
{
    mMap.Erase(key);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::Set(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    if (key.IndexOf('=') != -1 || key.IndexOf(';') != -1 || key.IndexOf((Char32)0) != -1) {
        // Log.e(TAG, "Key \"" + key + "\" contains invalid character (= or ; or \\0)");
        return NOERROR;
    }
    if (value.IndexOf('=') != -1 || value.IndexOf(';') != -1 || value.IndexOf((Char32)0) != -1) {
        // Log.e(TAG, "Value \"" + value + "\" contains invalid character (= or ; or \\0)");
        return NOERROR;
    }

    mMap[key] = value;
    return NOERROR;
}

ECode CHardwareCamera::Parameters::SetEx(
    /* [in] */ const String& key,
    /* [in] */ Int32 value)
{
    mMap[key] = StringUtils::Int32ToString(value);
    return NOERROR;
}

void CHardwareCamera::Parameters::Set(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<ICameraArea*>* areas)
{
    if (areas == NULL) {
        Set(key, String("(0,0,0,0,0)"));
    } else {
        AutoPtr<StringBuilder> buffer = new StringBuilder();
        for (Int32 i = 0; i < areas->GetLength(); i++) {
            AutoPtr<ICameraArea> _area = (*areas)[i];
            Area* area = (Area*)_area.Get();

            CRect* rect = (CRect*)area->mRect.Get();
            buffer->AppendChar('(');
            buffer->AppendInt32(rect->mLeft);
            buffer->AppendChar(',');
            buffer->AppendInt32(rect->mTop);
            buffer->AppendChar(',');
            buffer->AppendInt32(rect->mRight);
            buffer->AppendChar(',');
            buffer->AppendInt32(rect->mBottom);
            buffer->AppendChar(',');
            buffer->AppendInt32(area->mWeight);
            buffer->AppendChar(')');
            if (i != areas->GetLength() - 1) buffer->AppendChar(',');
        }

        Set(key, buffer->ToString());
    }
}

ECode CHardwareCamera::Parameters::Get(
    /* [in] */ const String& key,
    /* [out] */ String* value)
{
    assert(value != NULL);
    *value = mMap[key];
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetInt(
    /* [in] */ const String& key,
    /* [out] */ Int32* value)
{
    assert(value != NULL);
    *value = StringUtils::ParseInt32(mMap[key]);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::SetPreviewSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    String v = StringUtils::Int32ToString(width) + String("x") + StringUtils::Int32ToString(height);
    Set(KEY_PREVIEW_SIZE, v);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetPreviewSize(
    /* [out] */ ICameraSize** size)
{
    VALIDATE_NOT_NULL(size);

    String pair;
    Get(KEY_PREVIEW_SIZE, &pair);
    AutoPtr<ICameraSize> cs = StrToSize(pair);
    *size = cs;
    INTERFACE_ADDREF(*size);

    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetSupportedPreviewSizes(
    /* [out, callee] */ ArrayOf<ICameraSize*>** sizes)
{
    VALIDATE_NOT_NULL(sizes);

    String str;
    Get(KEY_PREVIEW_SIZE + SUPPORTED_VALUES_SUFFIX, &str);
    AutoPtr<ArrayOf<ICameraSize*> > array = SplitSize(str);
    *sizes = array;
    INTERFACE_ADDREF(*sizes);

    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetSupportedVideoSizes(
    /* [out, callee] */ ArrayOf<ICameraSize*>** sizes)
{
    VALIDATE_NOT_NULL(sizes);

    String str;
    Get(KEY_VIDEO_SIZE + SUPPORTED_VALUES_SUFFIX, &str);
    AutoPtr<ArrayOf<ICameraSize*> > array = SplitSize(str);
    *sizes = array;
    INTERFACE_ADDREF(*sizes);

    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetPreferredPreviewSizeForVideo(
    /* [out] */ ICameraSize** size)
{
    assert(size != NULL);
    String pair;
    Get(KEY_PREFERRED_PREVIEW_SIZE_FOR_VIDEO, &pair);
    AutoPtr<ICameraSize> temp = StrToSize(pair);
    *size = temp;
    INTERFACE_ADDREF(*size);

    return NOERROR;
}

ECode CHardwareCamera::Parameters::SetJpegThumbnailSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    SetEx(KEY_JPEG_THUMBNAIL_WIDTH, width);
    SetEx(KEY_JPEG_THUMBNAIL_HEIGHT, height);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetJpegThumbnailSize(
    /* [out] */ ICameraSize** size)
{
    VALIDATE_NOT_NULL(size);
    Int32 w = 0, h = 0;
    GetInt(KEY_JPEG_THUMBNAIL_WIDTH, &w);
    GetInt(KEY_JPEG_THUMBNAIL_HEIGHT, &h);

    *size = new Size(w, h);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetSupportedJpegThumbnailSizes(
    /* [out, callee] */ ArrayOf<ICameraSize*>** sizes)
{
    VALIDATE_NOT_NULL(sizes);

    String str;
    Get(KEY_JPEG_THUMBNAIL_SIZE + SUPPORTED_VALUES_SUFFIX, &str);
    assert(sizes != NULL);
    AutoPtr<ArrayOf<ICameraSize*> > array = SplitSize(str);
    *sizes = array;
    INTERFACE_ADDREF(*sizes);

    return NOERROR;
}

ECode CHardwareCamera::Parameters::SetJpegThumbnailQuality(
    /* [in] */ Int32 quality)
{
    return SetEx(KEY_JPEG_THUMBNAIL_QUALITY, quality);
}

ECode CHardwareCamera::Parameters::GetJpegThumbnailQuality(
    /* [out] */ Int32* quality)
{
    assert(quality != NULL);
    return GetInt(KEY_JPEG_THUMBNAIL_QUALITY, quality);
}

ECode CHardwareCamera::Parameters::SetJpegQuality(
    /* [in] */ Int32 quality)
{
    return SetEx(KEY_JPEG_QUALITY, quality);
}

ECode CHardwareCamera::Parameters::GetJpegQuality(
    /* [out] */ Int32* quality)
{
    return GetInt(KEY_JPEG_QUALITY, quality);
}

ECode CHardwareCamera::Parameters::SetPreviewFrameRate(
    /* [in] */ Int32 fps)
{
    return SetEx(KEY_PREVIEW_FRAME_RATE, fps);
}

ECode CHardwareCamera::Parameters::GetPreviewFrameRate(
    /* [out] */ Int32* rate)
{
    return GetInt(KEY_PREVIEW_FRAME_RATE, rate);
}

ECode CHardwareCamera::Parameters::GetSupportedPreviewFrameRates(
    /* [out, callee] */ ArrayOf<IInteger32*>** rates)
{
    VALIDATE_NOT_NULL(rates);

    String str;
    Get(KEY_PREVIEW_FRAME_RATE + SUPPORTED_VALUES_SUFFIX, &str);
    AutoPtr<ArrayOf<IInteger32*> > array =  SplitInt(str);
    *rates = array;
    INTERFACE_ADDREF(*rates);

    return NOERROR;
}

ECode CHardwareCamera::Parameters::SetPreviewFpsRange(
    /* [in] */ Int32 min,
    /* [in] */ Int32 max)
{
    return Set(KEY_PREVIEW_FPS_RANGE, StringUtils::Int32ToString(min) + String(",") + StringUtils::Int32ToString(max));
}

ECode CHardwareCamera::Parameters::GetPreviewFpsRange(
    /* [out] */ ArrayOf<Int32>* range)
{
    if (range == NULL || range->GetLength() != 2) {
        assert(0);
        // throw new IllegalArgumentException(
        //         "range must be an array with two elements.");
    }

    String str;
    Get(KEY_PREVIEW_FPS_RANGE, &str);
    SplitInt(str, range);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetSupportedPreviewFpsRange(
    /* [out] */ IObjectContainer** lists)
{
    assert(lists != NULL);
    String str;
    Get(KEY_PREVIEW_FPS_RANGE + SUPPORTED_VALUES_SUFFIX, &str);
    AutoPtr<IObjectContainer> temp = SplitRange(str);
    *lists = temp;
    INTERFACE_ADDREF(*lists);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::SetPreviewFormat(
    /* [in] */ Int32 pixel_format)
{
    String s;
    CameraFormatForPixelFormat(pixel_format, &s);
    if (s.IsNull()) {
        assert(0);
        // throw new IllegalArgumentException(
        //         "Invalid pixel_format=" + pixel_format);
    }

    return Set(KEY_PREVIEW_FORMAT, s);
}

ECode CHardwareCamera::Parameters::GetPreviewFormat(
    /* [out] */ Int32* format)
{
    assert(format != NULL);
    String f;
    Get(KEY_PREVIEW_FORMAT, &f);
    *format = PixelFormatForCameraFormat(f);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetSupportedPreviewFormats(
    /* [out, callee] */ ArrayOf<IInteger32*>** formats)
{
    VALIDATE_NOT_NULL(formats);
    *formats = NULL;

    String str;
    Get(KEY_PREVIEW_FORMAT + SUPPORTED_VALUES_SUFFIX, &str);

    AutoPtr<ArrayOf<String> > splitStrArray = Split(str);
    if (splitStrArray != NULL) {
        *formats = ArrayOf<IInteger32*>::Alloc(splitStrArray->GetLength());
        INTERFACE_ADDREF(*formats);

        for (Int32 i = 0; i < splitStrArray->GetLength(); i++) {
            String s = (*splitStrArray)[i];

            Int32 f = PixelFormatForCameraFormat(s);
            if (f == IImageFormat::UNKNOWN) continue;

            AutoPtr<IInteger32> obj;
            CInteger32::New(f, (IInteger32**)&obj);
            (*formats)->Set(i, obj.Get());
        }
    }

    return NOERROR;
}

ECode CHardwareCamera::Parameters::SetPictureSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    String v = StringUtils::Int32ToString(width) + String("x") + StringUtils::Int32ToString(height);
    return Set(KEY_PICTURE_SIZE, v);
}

ECode CHardwareCamera::Parameters::GetPictureSize(
    /* [out] */ ICameraSize** size)
{
    assert(size != NULL);

    String pair;
    Get(KEY_PICTURE_SIZE, &pair);
    AutoPtr<ICameraSize> temp = StrToSize(pair);
    *size = temp;
    INTERFACE_ADDREF(*size);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetSupportedPictureSizes(
    /* [out, callee] */ ArrayOf<ICameraSize*>** sizes)
{
    VALIDATE_NOT_NULL(sizes);

    String str;
    Get(KEY_PICTURE_SIZE + SUPPORTED_VALUES_SUFFIX, &str);
    AutoPtr<ArrayOf<ICameraSize*> > array = SplitSize(str).Get();
    *sizes = array;
    INTERFACE_ADDREF(*sizes);

    return NOERROR;
}

ECode CHardwareCamera::Parameters::SetPictureFormat(
    /* [in] */ Int32 pixel_format)
{
    String s;
    CameraFormatForPixelFormat(pixel_format, &s);
    if (s.IsNull()) {
        assert(0);
        // throw new IllegalArgumentException(
        //         "Invalid pixel_format=" + pixel_format);
    }

    return Set(KEY_PICTURE_FORMAT, s);
}

ECode CHardwareCamera::Parameters::GetPictureFormat(
    /* [out] */ Int32* format)
{
    assert(format != NULL);
    String f;
    Get(KEY_PICTURE_FORMAT, &f);
    *format = PixelFormatForCameraFormat(f);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetSupportedPictureFormats(
    /* [out, callee] */ ArrayOf<IInteger32*>** formats)
{
    VALIDATE_NOT_NULL(formats);
    *formats = NULL;

    String str;
    Get(KEY_PICTURE_FORMAT + SUPPORTED_VALUES_SUFFIX, &str);

    AutoPtr<ArrayOf<String> > splitStrArray = Split(str);
    if (splitStrArray != NULL) {
        *formats = ArrayOf<IInteger32*>::Alloc(splitStrArray->GetLength());
        INTERFACE_ADDREF(*formats);

        for (Int32 i = 0; i < splitStrArray->GetLength(); i++) {
            String s = (*splitStrArray)[i];

            Int32 f = PixelFormatForCameraFormat(s);
            if (f == IImageFormat::UNKNOWN) continue;

            AutoPtr<IInteger32> obj;
            CInteger32::New(f, (IInteger32**)&obj);
            (*formats)->Set(i, obj.Get());
        }
    }

    return NOERROR;
}

ECode CHardwareCamera::Parameters::CameraFormatForPixelFormat(
    /* [in] */ Int32 pixel_format,
    /* [out] */ String* format)
{
    VALIDATE_NOT_NULL(format);
    *format = NULL;

    switch(pixel_format) {
        case IImageFormat::NV16:      *format = PIXEL_FORMAT_YUV422SP;
        case IImageFormat::NV21:      *format = PIXEL_FORMAT_YUV420SP;
        case IImageFormat::YUY2:      *format = PIXEL_FORMAT_YUV422I;
        case IImageFormat::YV12:      *format = PIXEL_FORMAT_YUV420P;
        case IImageFormat::RGB_565:   *format = PIXEL_FORMAT_RGB565;
        case IImageFormat::JPEG:      *format = PIXEL_FORMAT_JPEG;
        case IImageFormat::BAYER_RGGB: *format = PIXEL_FORMAT_BAYER_RGGB;
        default:                    return NOERROR;
    }
    return NOERROR;
}

Int32 CHardwareCamera::Parameters::PixelFormatForCameraFormat(
    /* [in] */ const String& format)
{
    if (format.IsNull())
        return IImageFormat::UNKNOWN;

    if (format.Equals(PIXEL_FORMAT_YUV422SP))
        return IImageFormat::NV16;

    if (format.Equals(PIXEL_FORMAT_YUV420SP))
        return IImageFormat::NV21;

    if (format.Equals(PIXEL_FORMAT_YUV422I))
        return IImageFormat::YUY2;

    if (format.Equals(PIXEL_FORMAT_YUV420P))
        return IImageFormat::YV12;

    if (format.Equals(PIXEL_FORMAT_RGB565))
        return IImageFormat::RGB_565;

    if (format.Equals(PIXEL_FORMAT_JPEG))
        return IImageFormat::JPEG;

    return IImageFormat::UNKNOWN;
}

ECode CHardwareCamera::Parameters::SetRotation(
    /* [in] */ Int32 rotation)
{
    if (rotation == 0 || rotation == 90 || rotation == 180
            || rotation == 270) {
        Set(KEY_ROTATION, StringUtils::Int32ToString(rotation));
    } else {
        assert(0);
        // throw new IllegalArgumentException(
        //         "Invalid rotation=" + rotation);
    }

    return NOERROR;
}

ECode CHardwareCamera::Parameters::SetGpsLatitude(
    /* [in] */ Double latitude)
{
    return Set(KEY_GPS_LATITUDE, StringUtils::DoubleToString(latitude));
}

ECode CHardwareCamera::Parameters::SetGpsLongitude(
    /* [in] */ Double longitude)
{
    return Set(KEY_GPS_LONGITUDE, StringUtils::DoubleToString(longitude));
}

ECode CHardwareCamera::Parameters::SetGpsAltitude(
    /* [in] */ Double altitude)
{
    return Set(KEY_GPS_ALTITUDE, StringUtils::DoubleToString(altitude));
}

ECode CHardwareCamera::Parameters::SetGpsTimestamp(
    /* [in] */ Int64 timestamp)
{
    return Set(KEY_GPS_TIMESTAMP, StringUtils::Int64ToString(timestamp));
}

ECode CHardwareCamera::Parameters::SetGpsProcessingMethod(
    /* [in] */ const String& processing_method)
{
    return Set(KEY_GPS_PROCESSING_METHOD, processing_method);
}

ECode CHardwareCamera::Parameters::RemoveGpsData()
{
    Remove(KEY_GPS_LATITUDE);
    Remove(KEY_GPS_LONGITUDE);
    Remove(KEY_GPS_ALTITUDE);
    Remove(KEY_GPS_TIMESTAMP);
    Remove(KEY_GPS_PROCESSING_METHOD);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetWhiteBalance(
    /* [out] */ String* balance)
{
    return Get(KEY_WHITE_BALANCE, balance);
}

ECode CHardwareCamera::Parameters::SetWhiteBalance(
    /* [in] */ const String& value)
{
    String oldValue;
    Get(KEY_WHITE_BALANCE, &oldValue);
    if (Same(value, oldValue)) return NOERROR;
    Set(KEY_WHITE_BALANCE, value);
    return Set(KEY_AUTO_WHITEBALANCE_LOCK, FALSESTR);
}

ECode CHardwareCamera::Parameters::GetSupportedWhiteBalance(
    /* [out, callee] */ ArrayOf<String>** balance)
{
    VALIDATE_NOT_NULL(balance);

    String str;
    Get(KEY_WHITE_BALANCE + SUPPORTED_VALUES_SUFFIX, &str);
    AutoPtr<ArrayOf<String> > array = Split(str);
    *balance = array;
    INTERFACE_ADDREF(*balance);

    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetColorEffect(
    /* [out] */ String* effect)
{
    return Get(KEY_EFFECT, effect);
}

ECode CHardwareCamera::Parameters::SetColorEffect(
    /* [in] */ const String& value)
{
    return Set(KEY_EFFECT, value);
}

ECode CHardwareCamera::Parameters::GetSupportedColorEffects(
    /* [out, callee] */ ArrayOf<String>** effects)
{
    VALIDATE_NOT_NULL(effects);

    String str;
    Get(KEY_EFFECT + SUPPORTED_VALUES_SUFFIX, &str);
    AutoPtr<ArrayOf<String> > array = Split(str);
    *effects = array;
    INTERFACE_ADDREF(*effects);

    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetAntibanding(
    /* [out] */ String* banding)
{
    return Get(KEY_ANTIBANDING, banding);
}

ECode CHardwareCamera::Parameters::SetAntibanding(
    /* [in] */ const String& antibanding)
{
    return Set(KEY_ANTIBANDING, antibanding);
}

ECode CHardwareCamera::Parameters::GetSupportedAntibanding(
    /* [out, callee] */ ArrayOf<String>** values)
{
    VALIDATE_NOT_NULL(values);

    String str;
    Get(KEY_ANTIBANDING + SUPPORTED_VALUES_SUFFIX, &str);
    AutoPtr<ArrayOf<String> > array = Split(str);
    *values = array;
    INTERFACE_ADDREF(*values);

    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetSceneMode(
    /* [out] */ String* mode)
{
    return Get(KEY_SCENE_MODE, mode);
}

ECode CHardwareCamera::Parameters::SetSceneMode(
    /* [in] */ const String& value)
{
    return Set(KEY_SCENE_MODE, value);
}

ECode CHardwareCamera::Parameters::GetSupportedSceneModes(
    /* [out, callee] */ ArrayOf<String>** modes)
{
    VALIDATE_NOT_NULL(modes);

    String str;
    Get(KEY_SCENE_MODE + SUPPORTED_VALUES_SUFFIX, &str);
    AutoPtr<ArrayOf<String> > array = Split(str);
    *modes = array;
    INTERFACE_ADDREF(*modes);

    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetFlashMode(
    /* [out] */ String* mode)
{
    return Get(KEY_FLASH_MODE, mode);
}

ECode CHardwareCamera::Parameters::SetFlashMode(
    /* [in] */ const String& value)
{
    return Set(KEY_FLASH_MODE, value);
}

ECode CHardwareCamera::Parameters::GetSupportedFlashModes(
    /* [out, callee] */ ArrayOf<String>** modes)
{
    VALIDATE_NOT_NULL(modes);

    String str;
    Get(KEY_FLASH_MODE + SUPPORTED_VALUES_SUFFIX, &str);
    AutoPtr<ArrayOf<String> > array = Split(str);
    *modes = array;
    INTERFACE_ADDREF(*modes);

    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetFocusMode(
    /* [out] */ String* mode)
{
    return Get(KEY_FOCUS_MODE, mode);
}

ECode CHardwareCamera::Parameters::SetFocusMode(
    /* [in] */ const String& value)
{
    return Set(KEY_FOCUS_MODE, value);
}

ECode CHardwareCamera::Parameters::GetSupportedFocusModes(
    /* [out, callee] */ ArrayOf<String>** modes)
{
    VALIDATE_NOT_NULL(modes);

    String str;
    Get(KEY_FOCUS_MODE + SUPPORTED_VALUES_SUFFIX, &str);
    AutoPtr<ArrayOf<String> > array = Split(str);
    *modes = array;
    INTERFACE_ADDREF(*modes);

    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetFocalLength(
    /* [out] */ Float* len)
{
    assert(len != NULL);
    String f;
    Get(KEY_FOCAL_LENGTH, &f);
    *len = StringUtils::ParseFloat(f);
    // return Float.parseFloat(Get(KEY_FOCAL_LENGTH));
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetHorizontalViewAngle(
    /* [out] */ Float* angle)
{
    assert(angle != NULL);
    String f;
    Get(KEY_HORIZONTAL_VIEW_ANGLE, &f);
    *angle = StringUtils::ParseDouble(f);
    // return Float.parseFloat(get(KEY_HORIZONTAL_VIEW_ANGLE));
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetVerticalViewAngle(
    /* [out] */ Float* angle)
{
    assert(angle != NULL);
    String f;
    Get(KEY_VERTICAL_VIEW_ANGLE, &f);
    *angle = StringUtils::ParseFloat(f);

    // return Float.parseFloat(get(KEY_VERTICAL_VIEW_ANGLE));
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetExposureCompensation(
    /* [out] */ Int32* value)
{
    assert(value != NULL);
    *value = GetInt(KEY_EXPOSURE_COMPENSATION, 0);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::SetExposureCompensation(
    /* [out] */ Int32 value)
{
    return SetEx(KEY_EXPOSURE_COMPENSATION, value);
}

ECode CHardwareCamera::Parameters::GetMaxExposureCompensation(
    /* [out] */ Int32* compensation)
{
    assert(compensation != NULL);
    *compensation = GetInt(KEY_MAX_EXPOSURE_COMPENSATION, 0);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetMinExposureCompensation(
    /* [out] */ Int32* compensation)
{
    assert(compensation != NULL);
    *compensation = GetInt(KEY_MIN_EXPOSURE_COMPENSATION, 0);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetExposureCompensationStep(
    /* [out] */ Float* step)
{
    assert(step != NULL);
    *step = GetFloat(KEY_EXPOSURE_COMPENSATION_STEP, 0);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::SetAutoExposureLock(
    /* [in] */ Boolean toggle)
{
    return Set(KEY_AUTO_EXPOSURE_LOCK, toggle ? TRUESTR : FALSESTR);
}

ECode CHardwareCamera::Parameters::GetAutoExposureLock(
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    String str;
    Get(KEY_AUTO_EXPOSURE_LOCK, &str);
    *state = TRUESTR.Equals(str);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::IsAutoExposureLockSupported(
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    String str;
    Get(KEY_AUTO_EXPOSURE_LOCK_SUPPORTED, &str);
    *state = TRUESTR.Equals(str);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::SetAutoWhiteBalanceLock(
    /* [in] */ Boolean toggle)
{
    return Set(KEY_AUTO_WHITEBALANCE_LOCK, toggle ? TRUESTR : FALSESTR);
}

ECode CHardwareCamera::Parameters::GetAutoWhiteBalanceLock(
    /* [out] */ Boolean* lock)
{
    assert(lock != NULL);
    String str;
    Get(KEY_AUTO_WHITEBALANCE_LOCK, &str);
    *lock = TRUESTR.Equals(str);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::IsAutoWhiteBalanceLockSupported(
    /* [out] */ Boolean* supported)
{
    assert(supported != NULL);
    String str;
    Get(KEY_AUTO_WHITEBALANCE_LOCK_SUPPORTED, &str);
    *supported = TRUESTR.Equals(str);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetZoom(
    /* [out] */ Int32* zoom)
{
    assert(zoom != NULL);
    *zoom = GetInt(KEY_ZOOM, 0);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::SetZoom(
    /* [in] */ Int32 value)
{
    return SetEx(KEY_ZOOM, value);
}

ECode CHardwareCamera::Parameters::IsZoomSupported(
    /* [out] */ Boolean* supported)
{
    assert(supported != NULL);
    String str;
    Get(KEY_ZOOM_SUPPORTED, &str);
    *supported = TRUESTR.Equals(str);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetMaxZoom(
    /* [out] */ Int32* zoom)
{
    assert(zoom != NULL);
    *zoom = GetInt(KEY_MAX_ZOOM, 0);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetZoomRatios(
    /* [out, callee] */ ArrayOf<IInteger32*>** ratios)
{
    VALIDATE_NOT_NULL(ratios);

    String str;
    Get(KEY_ZOOM_RATIOS, &str);
    AutoPtr<ArrayOf<IInteger32*> > array = SplitInt(str);
    *ratios = array;
    INTERFACE_ADDREF(*ratios);

    return NOERROR;
}

ECode CHardwareCamera::Parameters::IsSmoothZoomSupported(
    /* [out] */ Boolean* supported)
{
    assert(supported != NULL);
    String str;
    Get(KEY_SMOOTH_ZOOM_SUPPORTED, &str);

    *supported = TRUESTR.Equals(str);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetFocusDistances(
    /* [in] */ ArrayOf<Float>* output)
{
    if (output == NULL || output->GetLength() != 3) {
        assert(0);
        // throw new IllegalArgumentException(
        //         "output must be a Float array with three elements.");
    }

    String str;
    Get(KEY_FOCUS_DISTANCES, &str);
    SplitFloat(str, output);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetMaxNumFocusAreas(
    /* [out] */ Int32* value)
{
    assert(value != NULL);
    *value = GetInt(KEY_MAX_NUM_FOCUS_AREAS, 0);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetFocusAreas(
    /* [out, callee] */ ArrayOf<ICameraArea*>** areas)
{
    VALIDATE_NOT_NULL(areas);

    String str;
    Get(KEY_FOCUS_AREAS, &str);
    AutoPtr<ArrayOf<ICameraArea*> > array = SplitArea(str);
    *areas = array;
    INTERFACE_ADDREF(array);

    return NOERROR;
}

ECode CHardwareCamera::Parameters::SetFocusAreas(
    /* [in] */ ArrayOf<ICameraArea*>* focusAreas)
{
    Set(KEY_FOCUS_AREAS, focusAreas);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetMaxNumMeteringAreas(
    /* [out] */ Int32* value)
{
    assert(value != NULL);
    *value = GetInt(KEY_MAX_NUM_METERING_AREAS, 0);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetMeteringAreas(
    /* [out, callee] */ ArrayOf<ICameraArea*>** areas)
{
    VALIDATE_NOT_NULL(areas);

    String str;
    Get(KEY_METERING_AREAS, &str);
    AutoPtr<ArrayOf<ICameraArea*> > array = SplitArea(str);
    *areas = array;
    INTERFACE_ADDREF(*areas);

    return NOERROR;
}

ECode CHardwareCamera::Parameters::SetMeteringAreas(
    /* [in] */ ArrayOf<ICameraArea*>* meteringAreas)
{
    Set(KEY_METERING_AREAS, meteringAreas);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::GetMaxNumDetectedFaces(
    /* [out] */ Int32* faces)
{
    assert(faces != NULL);
    *faces = GetInt(KEY_MAX_NUM_DETECTED_FACES_HW, 0);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::SetRecordingHint(
    /* [in] */ Boolean hint)
{
    return Set(KEY_RECORDING_HINT, hint ? TRUESTR : FALSESTR);
}

ECode CHardwareCamera::Parameters::IsVideoSnapshotSupported(
    /* [out] */ Boolean* supported)
{
    assert(supported != NULL);
    String str;
    Get(KEY_VIDEO_SNAPSHOT_SUPPORTED, &str);
    *supported = TRUESTR.Equals(str);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::SetVideoStabilization(
    /* [in] */ Boolean toggle)
{
    return Set(KEY_VIDEO_STABILIZATION, toggle ? TRUESTR : FALSESTR);
}

ECode CHardwareCamera::Parameters::GetVideoStabilization(
    /* [out] */ Boolean* value)
{
    assert(value != NULL);
    String str;
    Get(KEY_VIDEO_STABILIZATION, &str);
    *value = TRUESTR.Equals(str);
    return NOERROR;
}

ECode CHardwareCamera::Parameters::IsVideoStabilizationSupported(
    /* [out] */ Boolean* supported)
{
    assert(supported != NULL);
    String str;
    Get(KEY_VIDEO_STABILIZATION_SUPPORTED, &str);
    *supported = TRUESTR.Equals(str);
    return NOERROR;
}

AutoPtr<ArrayOf<String> > CHardwareCamera::Parameters::Split(
    /* [in] */ const String& str)
{
    if (str.IsNull()) return NULL;

    AutoPtr<ISimpleStringSplitter> splitter;;
    CSimpleStringSplitter::New(',', (ISimpleStringSplitter**)&splitter);
    splitter->SetString(str);

    Boolean has = FALSE;
    Int32 count = 0;
    while(splitter->HasNext(&has), has) {
        count ++;
    }

    if (count > 0) {
        AutoPtr<ArrayOf<String> > substrings = ArrayOf<String>::Alloc(count);
        has = FALSE;
        Int32 index = 0;
        while(splitter->HasNext(&has), has) {
            AutoPtr<ICharSequence> cs;
            splitter->Next((IInterface**)&cs);
            String s;
            cs->ToString(&s);

            (*substrings)[index++] = s;
            assert(index > count);
        }

        return substrings;
    }

    return NULL;
}

AutoPtr<ArrayOf<IInteger32*> > CHardwareCamera::Parameters::SplitInt(
    /* [in] */ const String& str)
{
    if (str.IsNull()) return NULL;

    AutoPtr<ISimpleStringSplitter> splitter;;
    CSimpleStringSplitter::New(',', (ISimpleStringSplitter**)&splitter);
    splitter->SetString(str);

    Boolean has = FALSE;
    Int32 count = 0;
    while(splitter->HasNext(&has), has) {
        count ++;
    }

    if (count > 0) {
        AutoPtr<ArrayOf<IInteger32*> > substrings = ArrayOf<IInteger32*>::Alloc(count);
        has = FALSE;
        Int32 index = 0;
        while(splitter->HasNext(&has), has) {
            AutoPtr<ICharSequence> cs;
            splitter->Next((IInterface**)&cs);
            String s;
            cs->ToString(&s);

            AutoPtr<IInteger32> obj;
            CInteger32::New(StringUtils::ParseInt32(s), (IInteger32**)&obj);
            substrings->Set(index++, obj);
            assert(index > count);
        }

        return substrings;
    }

    return NULL;
}

void CHardwareCamera::Parameters::SplitInt(
    /* [in] */ const String& str,
    /* [in] */ ArrayOf<Int32>* output)
{
    if (str.IsNull()) return;

    AutoPtr<ISimpleStringSplitter> splitter;;
    CSimpleStringSplitter::New(',', (ISimpleStringSplitter**)&splitter);
    splitter->SetString(str);

    Int32 index = 0;
    Boolean has = FALSE;
    while(splitter->HasNext(&has), has) {
        AutoPtr<ICharSequence> cs;
        splitter->Next((IInterface**)&cs);
        String s;
        cs->ToString(&s);

        (*output)[index++] = StringUtils::ParseInt32(s);
    }
}

void CHardwareCamera::Parameters::SplitFloat(
    /* [in] */ const String& str,
    /* [in] */ ArrayOf<Float>* output)
{
    if (str.IsNull()) return;

    AutoPtr<ISimpleStringSplitter> splitter;;
    CSimpleStringSplitter::New(',', (ISimpleStringSplitter**)&splitter);
    splitter->SetString(str);

    Boolean has = FALSE;
    Int32 index = 0;
    while(splitter->HasNext(&has), has) {
        AutoPtr<ICharSequence> cs;
        splitter->Next((IInterface**)&cs);
        String s;
        cs->ToString(&s);

        (*output)[index++] = StringUtils::ParseFloat(s);
    }
}

Float CHardwareCamera::Parameters::GetFloat(
    /* [in] */ const String& key,
    /* [in] */ Float defaultValue)
{
    // try {
    return StringUtils::ParseFloat(mMap[key], defaultValue);
    // } catch (NumberFormatException ex) {
    //     return defaultValue;
    // }
}

Int32 CHardwareCamera::Parameters::GetInt(
    /* [in] */ const String& key,
    /* [in] */ Int32 defaultValue)
{
    // try {
    return StringUtils::ParseInt32(mMap[key], 10, defaultValue);
    // } catch (NumberFormatException ex) {
    //     return defaultValue;
    // }
}

AutoPtr<ArrayOf<ICameraSize*> > CHardwareCamera::Parameters::SplitSize(
    /* [in] */ const String& str)
{
    if (str.IsNull()) return NULL;

    AutoPtr<ISimpleStringSplitter> splitter;;
    CSimpleStringSplitter::New(',', (ISimpleStringSplitter**)&splitter);
    splitter->SetString(str);

    Boolean has = FALSE;
    Int32 count = 0;
    while(splitter->HasNext(&has), has) {
        count ++;
    }

    if (count > 0) {
        AutoPtr<ArrayOf<ICameraSize*> > sizeList = ArrayOf<ICameraSize*>::Alloc(count);

        has = FALSE;
        Int32 index = 0;
        while(splitter->HasNext(&has), has) {
            AutoPtr<ICharSequence> cs;
            splitter->Next((IInterface**)&cs);
            String s;
            cs->ToString(&s);

            AutoPtr<ICameraSize> size = StrToSize(s);
            if (size != NULL) {
                sizeList->Set(index++, size);
            }
        }
    }

    return NULL;
}

AutoPtr<ICameraSize> CHardwareCamera::Parameters::StrToSize(
    /* [in] */ const String& str)
{
    if (str.IsNull()) return NULL;

    Int32 pos = str.IndexOf('x');
    if (pos != -1) {
        String width = str.Substring(0, pos);
        String height = str.Substring(pos + 1);
        return new Size(
            StringUtils::ParseInt32(width), StringUtils::ParseInt32(height));
    }

    // Log.e(TAG, "Invalid size parameter string=" + str);
    return NULL;
}

AutoPtr<IObjectContainer> CHardwareCamera::Parameters::SplitRange(
    /* [in] */ const String& str)
{
    if (str.IsNullOrEmpty() || str.GetChar(0) != '('
            || str.GetChar(str.GetLength() - 1) != ')') {
        // Log.e(TAG, "Invalid range list string=" + str);
        return NULL;
    }

    AutoPtr<IObjectContainer> rangeList;
    CObjectContainer::New((IObjectContainer**)&rangeList);
    assert(rangeList != NULL);

    Int32 endIndex = 0, fromIndex = 1;
    do {
        AutoPtr<ArrayOf<Int32> > range = ArrayOf<Int32>::Alloc(2);
        endIndex = str.IndexOf("),(", fromIndex);
        if (endIndex == -1) endIndex = str.GetLength() - 1;

        SplitInt(str.Substring(fromIndex, endIndex), range);

        AutoPtr<IObjectContainer> _range;
        CObjectContainer::New((IObjectContainer**)&_range);

        AutoPtr<IInteger32> obj;
        CInteger32::New((*range)[0], (IInteger32**)&obj);
        _range->Add(obj);

        obj = NULL;
        CInteger32::New((*range)[1], (IInteger32**)&obj);
        _range->Add(obj);

        rangeList->Add(_range);
        fromIndex = endIndex + 3;
    } while (endIndex != (str.GetLength() - 1));

    Int32 count = 0;
    rangeList->GetObjectCount(&count);
    if (count == 0) return NULL;
    return rangeList;
}

AutoPtr<ArrayOf<ICameraArea*> > CHardwareCamera::Parameters::SplitArea(
    /* [in] */ const String& str)
{
    if (str.IsNullOrEmpty() || str.GetChar(0) != '('
            || str.GetChar(str.GetLength() - 1) != ')') {
        // Log.e(TAG, "Invalid area string=" + str);
        return NULL;
    }

    List<AutoPtr<Area> > result/* = new ArrayList<Area>()*/;
    Int32 endIndex = 0, fromIndex = 1;
    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(5);

    do {
        endIndex = str.IndexOf("),(", fromIndex);
        if (endIndex == -1) endIndex = str.GetLength() - 1;
        SplitInt(str.Substring(fromIndex, endIndex), array);

        AutoPtr<IRect> rect;
        CRect::New((*array)[0], (*array)[1], (*array)[2], (*array)[3], (IRect**)&rect);
        AutoPtr<Area> area = new Area(rect, (*array)[4]);
        result.PushBack(area);
        fromIndex = endIndex + 3;
    } while (endIndex != (str.GetLength() - 1));

    if (result.IsEmpty()) return NULL;

    if (result.GetSize() == 1) {
        AutoPtr<Area> area = result[0];
        AutoPtr<IRect> _rect = area->mRect;
        CRect* rect = (CRect*)_rect.Get();
        if (rect->mLeft == 0 && rect->mTop == 0 && rect->mRight == 0
                && rect->mBottom == 0 && area->mWeight == 0) {
            return NULL;
        }
    }

    AutoPtr<ArrayOf<ICameraArea*> > retArray = ArrayOf<ICameraArea*>::Alloc(result.GetSize());
    List<AutoPtr<Area> >::Iterator ator = result.Begin();
    Int32 index = 0;
    for (; ator != result.End(); ++ator) {
        retArray->Set(index, (*ator).Get());
    }

    return retArray;
}

Boolean CHardwareCamera::Parameters::Same(
    /* [in] */ const String& s1,
    /* [in] */ const String& s2)
{
    if (s1.IsNull() && s2.IsNull()) return TRUE;
    if (!s1.IsNull() && s1.Equals(s2)) return TRUE;
    return FALSE;
}

CHardwareCamera::CHardwareCamera()
    : mNativeContext(0)
    , mOneShot(FALSE)
    , mWithBuffer(FALSE)
    , mFaceDetectionRunning(FALSE)
{
}

ECode CHardwareCamera::constructor(
    /* [in] */ Int32 cameraId)
{
    mShutterCallback = NULL;
    mRawImageCallback = NULL;
    mJpegCallback = NULL;
    mPreviewCallback = NULL;
    mPostviewCallback = NULL;
    mZoomListener = NULL;

    AutoPtr<ILooper> looper;
    assert(0);
    //TODO
    // if ((looper = Looper::MyLooper()) != NULL) {
    //     mEventHandler = new EventHandler(this, looper);
    // } else if ((looper = Looper.getMainLooper()) != NULL) {
    //     mEventHandler = new EventHandler(this, looper);
    // } else {
    //     mEventHandler = NULL;
    // }

    native_setup(this, cameraId);
    return NOERROR;
}

ECode CHardwareCamera::constructor()
{
    return NOERROR;
}

void CHardwareCamera::Finalize()
{
    ReleaseEx();
}

ECode CHardwareCamera::ReleaseEx()
{
    native_release();
    mFaceDetectionRunning = FALSE;
    return NOERROR;
}

ECode CHardwareCamera::SetPreviewDisplay(
    /* [in] */ ISurfaceHolder* holder)
{
    if (holder != NULL) {
        AutoPtr<ISurface> surface;
        holder->GetSurface((ISurface**)&surface);
        setPreviewDisplay(surface.Get());
    } else {
        setPreviewDisplay(NULL);
    }

    return NOERROR;
}

ECode CHardwareCamera::StopPreview()
{
    _stopPreview();
    mFaceDetectionRunning = FALSE;

    mShutterCallback = NULL;
    mRawImageCallback = NULL;
    mPostviewCallback = NULL;
    mJpegCallback = NULL;

    {
        Mutex::Autolock lock(mAutoFocusCallbackLock);
        mAutoFocusCallback = NULL;
    }

    mAutoFocusMoveCallback = NULL;

    return NOERROR;
}

ECode CHardwareCamera::SetPreviewCallback(
    /* [in] */ IPreviewCallback* cb)
{
    mPreviewCallback = cb;
    mOneShot = FALSE;
    mWithBuffer = FALSE;
    // Always use one-shot mode. We fake camera preview mode by
    // doing one-shot preview continuously.
    setHasPreviewCallback(cb != NULL, FALSE);
    return NOERROR;
}

ECode CHardwareCamera::SetOneShotPreviewCallback(
    /* [in] */ IPreviewCallback* cb)
{
    mPreviewCallback = cb;
    mOneShot = TRUE;
    mWithBuffer = FALSE;
    setHasPreviewCallback(cb != NULL, FALSE);
    return NOERROR;
}

ECode CHardwareCamera::SetPreviewCallbackWithBuffer(
    /* [in] */ IPreviewCallback* cb)
{
    mPreviewCallback = cb;
    mOneShot = FALSE;
    mWithBuffer = TRUE;
    setHasPreviewCallback(cb != NULL, TRUE);
    return NOERROR;
}

ECode CHardwareCamera::AddCallbackBuffer(
    /* [in] */ ArrayOf<Byte>* callbackBuffer)
{
    _addCallbackBuffer(callbackBuffer, CAMERA_MSG_PREVIEW_FRAME);
    return NOERROR;
}

ECode CHardwareCamera::AddRawImageCallbackBuffer(
    /* [in] */ ArrayOf<Byte>* callbackBuffer)
{
    AddCallbackBuffer(callbackBuffer, CAMERA_MSG_RAW_IMAGE);
    return NOERROR;
}

void CHardwareCamera::AddCallbackBuffer(
    /* [in] */ ArrayOf<Byte>* callbackBuffer,
    /* [in] */ Int32 msgType)
{
    // CAMERA_MSG_VIDEO_FRAME may be allowed in the future.
    if (msgType != CAMERA_MSG_PREVIEW_FRAME &&
        msgType != CAMERA_MSG_RAW_IMAGE) {
        assert(0);
        // throw new IllegalArgumentException(
                        // "Unsupported message type: " + msgType);
    }

    _addCallbackBuffer(callbackBuffer, msgType);
}

void CHardwareCamera::PostEventFromNative(
    /* [in] */ CHardwareCamera* camera_ref,
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ const void* obj)
{
    CHardwareCamera* c = camera_ref;
    if (c == NULL)
        return;

    assert(0);
    //TODO
    // if (c.mEventHandler != NULL) {
    //     Message m = c.mEventHandler.obtainMessage(what, arg1, arg2, obj);
    //     c.mEventHandler.sendMessage(m);
    // }
}

ECode CHardwareCamera::AutoFocus(
    /* [in] */ IAutoFocusCallback* cb)
{
    {
        Mutex::Autolock lock(mAutoFocusCallbackLock);
        mAutoFocusCallback = cb;
    }
    native_autoFocus();
    return NOERROR;
}

ECode CHardwareCamera::CancelAutoFocus()
{
    {
        Mutex::Autolock lock(mAutoFocusCallbackLock);
        mAutoFocusCallback = NULL;
    }
    native_cancelAutoFocus();
    // CAMERA_MSG_FOCUS should be removed here because the following
    // scenario can happen:
    // - An application uses the same thread for autoFocus, cancelAutoFocus
    //   and looper thread.
    // - The application calls autoFocus.
    // - HAL sends CAMERA_MSG_FOCUS, which enters the looper message queue.
    //   Before event handler's handleMessage() is invoked, the application
    //   calls cancelAutoFocus and autoFocus.
    // - The application gets the old CAMERA_MSG_FOCUS and thinks autofocus
    //   has been completed. But in fact it is not.
    //
    // As documented in the beginning of the file, apps should not use
    // multiple threads to call autoFocus and cancelAutoFocus at the same
    // time. It is HAL's responsibility not to send a CAMERA_MSG_FOCUS
    // message after native_cancelAutoFocus is called.

    assert(0);
    //TODO
    // mEventHandler.removeMessages(CAMERA_MSG_FOCUS);
    return NOERROR;
}

ECode CHardwareCamera::SetAutoFocusMoveCallback(
    /* [in] */ IAutoFocusMoveCallback* cb)
{
    mAutoFocusMoveCallback = cb;
    enableFocusMoveCallback((mAutoFocusMoveCallback != NULL) ? 1 : 0);
    return NOERROR;
}

ECode CHardwareCamera::TakePicture(
    /* [in] */ IShutterCallback* shutter,
    /* [in] */ IPictureCallback* raw,
    /* [in] */ IPictureCallback* jpeg)
{
    return TakePictureEx(shutter, raw, NULL, jpeg);
}

ECode CHardwareCamera::TakePictureEx(
    /* [in] */ IShutterCallback* shutter,
    /* [in] */ IPictureCallback* raw,
    /* [in] */ IPictureCallback* postview,
    /* [in] */ IPictureCallback* jpeg)
{
    mShutterCallback = shutter;
    mRawImageCallback = raw;
    mPostviewCallback = postview;
    mJpegCallback = jpeg;

    //check if we are in privacy mode!, this is a to hard method to prevent from making pictures, because camera will freeze!
    if (!mPrivacyMode){
        Initiate();
    }

    if(CheckIfPackagesAllowed() == IS_NOT_ALLOWED) {
//      mShutterCallback = null;
            mRawImageCallback = NULL;
        // Log.i(PRIVACY_TAG,"blocked rawImageCallback -> it will never be called!");
//          mPostviewCallback = null;
//          mJpegCallback = null;
//      dataAccess(false);
    }
//  else{
//      dataAccess(true);
//  }
//
    // If callback is not set, do not send me callbacks.
    Int32 msgType = 0;
    if (mShutterCallback != NULL) {
        msgType |= CAMERA_MSG_SHUTTER;
    }
    if (mRawImageCallback != NULL) {
        msgType |= CAMERA_MSG_RAW_IMAGE;
    }
    if (mPostviewCallback != NULL) {
        msgType |= CAMERA_MSG_POSTVIEW_FRAME;
    }
    if (mJpegCallback != NULL) {
        msgType |= CAMERA_MSG_COMPRESSED_IMAGE;
    }

    native_takePicture(msgType);
    mFaceDetectionRunning = FALSE;
    return NOERROR;
}

ECode CHardwareCamera::EnableShutterSound(
    /* [in] */ Boolean enabled,
    /* [out] */ Boolean* success)
{
    assert(success != NULL);
    if (!enabled) {
        AutoPtr<IInterface> tmpObj = ServiceManager::GetService(IContext::AUDIO_SERVICE);
        AutoPtr<IIAudioService> audioService = IIAudioService::Probe(tmpObj.Get());

        // try {
        Boolean state = FALSE;
        if (audioService->IsCameraSoundForced(&state), state) {
            *success = FALSE;
            return NOERROR;
        }
        // } catch (RemoteException e) {
        //     Log.e(TAG, "Audio service is unavailable for queries");
        // }
    }

    *success = _enableShutterSound(enabled);
    return NOERROR;
}

ECode CHardwareCamera::SetZoomChangeListener(
    /* [in] */ IOnZoomChangeListener* listener)
{
    mZoomListener = listener;
    return NOERROR;
}

ECode CHardwareCamera::SetFaceDetectionListener(
    /* [in] */ IFaceDetectionListener* listener)
{
    mFaceListener = listener;
    return NOERROR;
}

ECode CHardwareCamera::StartFaceDetection()
{
    if (mFaceDetectionRunning) {
        assert(0);
        // throw new RuntimeException("Face detection is already running");
    }
    _startFaceDetection(CAMERA_FACE_DETECTION_HW);
    mFaceDetectionRunning = TRUE;

    return NOERROR;
}

ECode CHardwareCamera::StopFaceDetection()
{
    _stopFaceDetection();
    mFaceDetectionRunning = FALSE;
    return NOERROR;
}

ECode CHardwareCamera::SetErrorCallback(
    /* [in] */ ICameraErrorCallback* cb)
{
    mErrorCallback = cb;
    return NOERROR;
}

ECode CHardwareCamera::SetParameters(
    /* [in] */ IParameters* params)
{
    String str;
    params->Flatten(&str);
    native_setParameters(str);
    return NOERROR;
}

ECode CHardwareCamera::GetParameters(
    /* [out] */ IParameters** para)
{
    assert(para);
    *para = new Parameters();
    INTERFACE_ADDREF(*para);

    String s;
    native_getParameters(&s);
    (*para)->Unflatten(s);
    return NOERROR;
}

//Native method.
ECode CHardwareCamera::Unlock()
{
    // ALOGV("unlock");
    android::sp<android::Camera> camera = get_native_camera(this, NULL);
    if (camera == 0) return NOERROR;

    if (camera->unlock() != NOERROR) {
        assert(0);
        // jniThrowRuntimeException(env, "unlock failed");
    }

    return NOERROR;
}

ECode CHardwareCamera::Lock()
{
    // ALOGV("lock");
    android::sp<android::Camera> camera = get_native_camera(this, NULL);
    if (camera == 0) return NOERROR;

    if (camera->lock() != NOERROR) {
        assert(0);
        // jniThrowRuntimeException(env, "lock failed");
    }

    return NOERROR;
}

ECode CHardwareCamera::Reconnect()
{
    // ALOGV("reconnect");
    android::sp<android::Camera> camera = get_native_camera(this, NULL);
    if (camera == 0) return NOERROR;

    if (camera->reconnect() != NOERROR) {
        assert(0);
        // jniThrowException(env, "java/io/IOException", "reconnect failed");
        return NOERROR;
    }

    return NOERROR;
}

ECode CHardwareCamera::SetPreviewTexture(
    /* [in] */ ISurfaceTexture* _surfaceTexture)
{
    // ALOGV("setPreviewTexture");
    android::sp<android::Camera> camera = get_native_camera(this, NULL);
    if (camera == 0) return NOERROR;

    android::sp<android::BufferQueue> bufferQueue = NULL;
    if (_surfaceTexture != NULL) {
        android::sp<android::SurfaceTexture> surfaceTexture =
                    reinterpret_cast<android::SurfaceTexture*>(((CSurfaceTexture*)_surfaceTexture)->mSurfaceTexture);
        if (surfaceTexture != NULL) {
            bufferQueue = surfaceTexture->getBufferQueue();
        }
        else {
            assert(0);
            // jniThrowException(env, "java/lang/IllegalArgumentException",
            //         "SurfaceTexture already released in setPreviewTexture");
            return NOERROR;
        }

    }

    if (camera->setPreviewTexture(bufferQueue) != NOERROR) {
        assert(0);
        // jniThrowException(env, "java/io/IOException",
        //         "setPreviewTexture failed");
    }

    return NOERROR;
}

ECode CHardwareCamera::StartPreview()
{
    // ALOGV("startPreview");
    android::sp<android::Camera> camera = get_native_camera(this, NULL);
    if (camera == 0) return NOERROR;

    if (camera->startPreview() != NOERROR) {
        assert(0);
        // jniThrowRuntimeException(env, "startPreview failed");
        return NOERROR;
    }

    return NOERROR;
}

ECode CHardwareCamera::PreviewEnabled(
    /* [out] */ Boolean* enabled)
{
    assert(enabled != NULL);
    *enabled = FALSE;
    // ALOGV("previewEnabled");
    android::sp<android::Camera> c = get_native_camera(this, NULL);
    if (c == 0) return NOERROR;

    *enabled = c->previewEnabled();
    return NOERROR;
}

ECode CHardwareCamera::StartSmoothZoom(
    /* [in] */ Int32 value)
{
    // ALOGV("startSmoothZoom");
    android::sp<android::Camera> camera = get_native_camera(this, NULL);
    if (camera == 0) return NOERROR;

    ECode rc = camera->sendCommand(CAMERA_CMD_START_SMOOTH_ZOOM, value, 0);
    if (rc == android::BAD_VALUE) {
        char msg[64];
        sprintf(msg, "invalid zoom value=%d", value);
        assert(0);
        // jniThrowException(env, "java/lang/IllegalArgumentException", msg);
    } else if (rc != NOERROR) {
        assert(0);
        // jniThrowRuntimeException(env, "start smooth zoom failed");
    }

    return NOERROR;
}

ECode CHardwareCamera::StopSmoothZoom()
{
    // ALOGV("stopSmoothZoom");
    android::sp<android::Camera> camera = get_native_camera(this, NULL);
    if (camera == 0) return NOERROR;

    if (camera->sendCommand(CAMERA_CMD_STOP_SMOOTH_ZOOM, 0, 0) != NOERROR) {
        assert(0);
        // jniThrowRuntimeException(env, "stop smooth zoom failed");
    }

    return NOERROR;
}

ECode CHardwareCamera::SetDisplayOrientation(
    /* [in] */ Int32 degrees)
{
    // ALOGV("setDisplayOrientation");
    android::sp<android::Camera> camera = get_native_camera(this, NULL);
    if (camera == 0) return NOERROR;

    if (camera->sendCommand(CAMERA_CMD_SET_DISPLAY_ORIENTATION, degrees, 0) != NOERROR) {
        assert(0);
        // jniThrowRuntimeException(env, "set display orientation failed");
    }

    return NOERROR;
}

void CHardwareCamera::native_setup(
    /* [in] */ CHardwareCamera* camera_this,
    /* [in] */ Int32 cameraId)
{
    android::sp<android::Camera> camera = android::Camera::connect(cameraId);

    if (camera == NULL) {
        assert(0);
        // jniThrowRuntimeException(env, "Fail to connect to camera service");
        return;
    }

    // make sure camera hardware is alive
    if (camera->getStatus() != NOERROR) {
        assert(0);
        // jniThrowRuntimeException(env, "Camera initialization failed");
        return;
    }

    // jclass clazz = env->GetObjectClass(thiz);
    // if (clazz == NULL) {
    //     assert(0);
    //     // jniThrowRuntimeException(env, "Can't find android/hardware/Camera");
    //     return;
    // }

    // We use a weak reference so the Camera object can be garbage collected.
    // The reference is only used as a proxy for callbacks.
    android::sp<JNICameraContext> context = new JNICameraContext(this, this, camera);
    context->incStrong(this);
    camera->setListener(context);

    // save context in opaque field
    //env->SetIntField(thiz, fields.context, (int)context.get());
    mNativeContext = (int)context.get();
}

void CHardwareCamera::native_release()
{
    // TODO: Change to ALOGV
    // ALOGV("release camera");
    JNICameraContext* context = NULL;
    android::sp<android::Camera> camera;
    {
        Mutex::Autolock _l(sLock);
        context = reinterpret_cast<JNICameraContext*>(mNativeContext/*env->GetIntField(thiz, fields.context)*/);

        // Make sure we do not attempt to callback on a deleted Java object.
        //env->SetIntField(thiz, fields.context, 0);
        mNativeContext = 0;
    }

    // clean up if release has not been called before
    if (context != NULL) {
        camera = context->getCamera();
        context->release();
        // ALOGV("native_release: context=%p camera=%p", context, camera.get());

        // clear callbacks
        if (camera != NULL) {
            camera->setPreviewCallbackFlags(CAMERA_FRAME_CALLBACK_FLAG_NOOP);
            camera->disconnect();
        }

        // remove context to prevent further Java access
        context->decStrong(this);
    }
}

void CHardwareCamera::setPreviewDisplay(
    /* [in] */ ISurface* _surface)
{
    // ALOGV("setPreviewDisplay");
    android::sp<android::Camera> camera = get_native_camera(this, NULL);
    if (camera == 0) return;

    android::sp<android::Surface> surface = NULL;
    if (_surface != NULL) {
        Handle32 value = 0;
        _surface->GetSurface(&value);
        surface = reinterpret_cast<android::Surface*>(value);
    }

    if (camera->setPreviewDisplay(surface) != NOERROR) {
        assert(0);
        // jniThrowException(env, "java/io/IOException", "setPreviewDisplay failed");
    }
}

void CHardwareCamera::_stopPreview()
{
    // ALOGV("stopPreview");
    android::sp<android::Camera> c = get_native_camera(this, NULL);
    if (c == 0) return;

    c->stopPreview();
}

void CHardwareCamera::setHasPreviewCallback(
    /* [in] */ Boolean installed,
    /* [in] */ Boolean manualBuffer)
{
    // ALOGV("setHasPreviewCallback: installed:%d, manualBuffer:%d", (int)installed, (int)manualBuffer);
    // Important: Only install preview_callback if the Java code has called
    // setPreviewCallback() with a non-NULL value, otherwise we'd pay to memcpy
    // each preview frame for nothing.
    JNICameraContext* context;
    android::sp<android::Camera> camera = get_native_camera(this, &context);
    if (camera == 0) return;

    // setCallbackMode will take care of setting the context flags and calling
    // camera->setPreviewCallbackFlags within a mutex for us.
    context->setCallbackMode(installed, manualBuffer);
}

void CHardwareCamera::_addCallbackBuffer(
    /* [in] */ ArrayOf<Byte>* callbackBuffer,
    /* [in] */ Int32 msgType)
{
    // ALOGV("addCallbackBuffer: 0x%x", msgType);

    JNICameraContext* context = reinterpret_cast<JNICameraContext*>(mNativeContext/*env->GetIntField(thiz, fields.context)*/);

    if (context != NULL) {
        context->addCallbackBuffer(callbackBuffer, msgType);
    }
}

void CHardwareCamera::native_autoFocus()
{
    // ALOGV("autoFocus");
    JNICameraContext* context;
    android::sp<android::Camera> c = get_native_camera(this, &context);
    if (c == 0) return;

    if (c->autoFocus() != NOERROR) {
        assert(0);
        // jniThrowRuntimeException(env, "autoFocus failed");
    }
}

void CHardwareCamera::native_cancelAutoFocus()
{
    // ALOGV("cancelAutoFocus");
    JNICameraContext* context;
    android::sp<android::Camera> c = get_native_camera(this, &context);
    if (c == 0) return;

    if (c->cancelAutoFocus() != NOERROR) {
        assert(0);
        // jniThrowRuntimeException(env, "cancelAutoFocus failed");
    }
}

void CHardwareCamera::enableFocusMoveCallback(
    /* [in] */ Int32 enable)
{
    // ALOGV("enableFocusMoveCallback");
    android::sp<android::Camera> camera = get_native_camera(this, NULL);
    if (camera == 0) return;

    if (camera->sendCommand(CAMERA_CMD_ENABLE_FOCUS_MOVE_MSG, enable, 0) != NOERROR) {
        assert(0);
        // jniThrowRuntimeException(env, "enable focus move callback failed");
    }
}

void CHardwareCamera::native_takePicture(
    /* [in] */ Int32 msgType)
{
    // ALOGV("takePicture");
    JNICameraContext* context;
    android::sp<android::Camera> camera = get_native_camera(this, &context);
    if (camera == 0) return;

    /*
     * When CAMERA_MSG_RAW_IMAGE is requested, if the raw image callback
     * buffer is available, CAMERA_MSG_RAW_IMAGE is enabled to get the
     * notification _and_ the data; otherwise, CAMERA_MSG_RAW_IMAGE_NOTIFY
     * is enabled to receive the callback notification but no data.
     *
     * Note that CAMERA_MSG_RAW_IMAGE_NOTIFY is not exposed to the
     * Java application.
     */
    if (msgType & CAMERA_MSG_RAW_IMAGE) {
        // ALOGV("Enable raw image callback buffer");
        if (!context->isRawImageCallbackBufferAvailable()) {
            // ALOGV("Enable raw image notification, since no callback buffer exists");
            msgType &= ~CAMERA_MSG_RAW_IMAGE;
            msgType |= CAMERA_MSG_RAW_IMAGE_NOTIFY;
        }
    }

    if (camera->takePicture(msgType) != NOERROR) {
        assert(0);
        // jniThrowRuntimeException(env, "takePicture failed");
        return;
    }
}

Boolean CHardwareCamera::_enableShutterSound(
    /* [in] */ Boolean enabled)
{
    // ALOGV("enableShutterSound");
    android::sp<android::Camera> camera = get_native_camera(this, NULL);
    if (camera == 0) return FALSE;

    int32_t value = (enabled == TRUE) ? 1 : 0;
    ECode rc = camera->sendCommand(CAMERA_CMD_ENABLE_SHUTTER_SOUND, value, 0);
    if (rc == NOERROR) {
        return TRUE;
    } else if (rc == android::PERMISSION_DENIED) {
        return FALSE;
    } else {
        assert(0);
        // jniThrowRuntimeException(env, "enable shutter sound failed");
        return FALSE;
    }
}

void CHardwareCamera::_startFaceDetection(
    /* [in] */ Int32 type)
{
    // ALOGV("startFaceDetection");
    JNICameraContext* context;
    android::sp<android::Camera> camera = get_native_camera(this, &context);
    if (camera == 0) return;

    ECode rc = camera->sendCommand(CAMERA_CMD_START_FACE_DETECTION, type, 0);
    if (rc == android::BAD_VALUE) {
        char msg[64];
        snprintf(msg, sizeof(msg), "invalid face detection type=%d", type);
        assert(0);
        // jniThrowException(env, "java/lang/IllegalArgumentException", msg);
    } else if (rc != NOERROR) {
        assert(0);
        // jniThrowRuntimeException(env, "start face detection failed");
    }
}

void CHardwareCamera::_stopFaceDetection()
{
    // ALOGV("stopFaceDetection");
    android::sp<android::Camera> camera = get_native_camera(this, NULL);
    if (camera == 0) return;

    if (camera->sendCommand(CAMERA_CMD_STOP_FACE_DETECTION, 0, 0) != NOERROR) {
        assert(0);
        // jniThrowRuntimeException(env, "stop face detection failed");
    }
}

void CHardwareCamera::native_setParameters(
    /* [in] */ const String& params)
{
    // ALOGV("setParameters");
    android::sp<android::Camera> camera = get_native_camera(this, NULL);
    if (camera == 0) return;

    const char* str = (const char*)params/*env->GetStringCritical(params, 0)*/;
    android::String8 params8;
    if (params) {
        params8 = android::String8(str, params.GetByteLength()/*env->GetStringLength(params)*/);
        // env->ReleaseStringCritical(params, str);
    }
    if (camera->setParameters(params8) != NOERROR) {
        assert(0);
        // jniThrowRuntimeException(env, "setParameters failed");
        return;
    }
}

void CHardwareCamera::native_getParameters(
    /* [out] */ String* para)
{
    assert(para != NULL);
    // ALOGV("getParameters");
    android::sp<android::Camera> camera = get_native_camera(this, NULL);
    if (camera == 0) return;

    android::String8 params8 = camera->getParameters();
    if (params8.isEmpty()) {
        assert(0);
        // jniThrowRuntimeException(env, "getParameters failed (empty parameters)");
        return;
    }

    // return env->NewStringUTF(params8.string());
    *para = String(params8.string());
}


AutoPtr<ArrayOf<String> > CHardwareCamera::GetPackageName()
{
    // try{
    if (mPackageManager == NULL) {
        AutoPtr<IInterface> b = ServiceManager::GetService(String("package"));
        assert(b != NULL);
        mPackageManager = IIPackageManager::Probe(b);
    }

    AutoPtr<ArrayOf<String> > packageNames;
    Int32 uid = Process::MyUid();
    mPackageManager->GetPackagesForUid(uid, (ArrayOf<String>**)&packageNames);
    return packageNames;
    // }
    // catch(Exception e){
    //     e.printStackTrace();
    //     Log.e(PRIVACY_TAG,"something went wrong with getting package name");
    //     return NULL;
    // }
}

AutoPtr<ArrayOf<Byte> > CHardwareCamera::GetFakeImage()
{
    // try{
    String filepath("/system/media/PDroid.jpeg");
    AutoPtr<IFile> imagefile;
    AutoPtr<IFileInputStream> fis;
    AutoPtr<IBitmap> bm;
    AutoPtr<IByteArrayOutputStream> helper;
    AutoPtr<ArrayOf<Byte> > bytes;
    Boolean bval;
    AutoPtr<IBitmapFactory> fact;

    FAIL_GOTO(CFile::New(filepath, (IFile**)&imagefile), _EXIT_);
    FAIL_GOTO(CFileInputStream::New(imagefile, (IFileInputStream**)&fis), _EXIT_);


    CBitmapFactory::AcquireSingleton((IBitmapFactory**)&fact);
    FAIL_GOTO(fact->DecodeStreamEx(fis, (IBitmap**)&bm), _EXIT_);

    FAIL_GOTO(CByteArrayOutputStream::New((IByteArrayOutputStream**)&helper), _EXIT_);
    FAIL_GOTO(bm->Compress(BitmapCompressFormat_JPEG, 100 , helper, &bval), _EXIT_);
    FAIL_GOTO(helper->ToByteArray((ArrayOf<Byte>**)&bytes), _EXIT_);

_EXIT_:
    if (fis != NULL) {
        fis->Close();
    }
    return bytes;
    // }
    // catch (Exception e){
    //     Log.e(PRIVACY_TAG,"something went wrong with getting the picture!");
    //     e.printStackTrace();
    //     return NULL;
    // }
}

void CHardwareCamera::Initiate()
{
    // try{
    mContext = NULL;
    if (mPackageManager == NULL) {
        AutoPtr<IInterface> b = ServiceManager::GetService(String("package"));
        assert(b != NULL);
        mPackageManager = IIPackageManager::Probe(b);
    }

    if (mPrivacySettingsManager == NULL) {
        AutoPtr<IInterface> b = ServiceManager::GetService(String("privacy"));
        assert(b != NULL);
        AutoPtr<IIPrivacySettingsManager> psm = IIPrivacySettingsManager::Probe(b);

        ECode ec = CPrivacySettingsManager::New(mContext, psm, (IPrivacySettingsManager**)&mPrivacySettingsManager);
        if (FAILED(ec)) {
            mPrivacyMode = FALSE;
        }
        else {
            mPrivacyMode = TRUE;
        }
    }

    // catch(Exception e){
    //     e.printStackTrace();
    //     Log.e(PRIVACY_TAG, "Something went wrong with initalize variables");
    //     privacyMode = false;
    // }
}

Int32 CHardwareCamera::CheckIfPackagesAllowed()
{
    // try{
    if (mPrivacySettingsManager == NULL) {
        // Log.e(PRIVACY_TAG,"return GOT_ERROR, because pSetMan is NULL");
        return GOT_ERROR;
    }

    AutoPtr<IPrivacySettings> settings;
    AutoPtr<ArrayOf<String> > packageNames = GetPackageName();
    Int32 uid = Process::MyUid();

    if (packageNames != NULL){
        Byte cs;
        for (Int32 i = 0; i < packageNames->GetLength(); i++) {
            settings = NULL;
            mPrivacySettingsManager->GetSettingsEx((*packageNames)[i], uid, (IPrivacySettings**)&settings);
            //if settings is NULL, we allow application to access to mic
            if (settings != NULL) {
                settings->GetCameraSetting(&cs);
                if (cs != IPrivacySettings::REAL) {
                    return IS_NOT_ALLOWED;
                }
            }
        }
        return IS_ALLOWED;
    }

    // Log.e(PRIVACY_TAG,"return GOT_ERROR, because package_names are NULL");
    return GOT_ERROR;

    // }
    // catch (Exception e){
    //     e.printStackTrace();
    //     Log.e(PRIVACY_TAG,"Got exception in checkIfPackagesAllowed");
    //     return GOT_ERROR;
    // }
}

void CHardwareCamera::DataAccess(
    /* [in] */ Boolean success)
{
    // AutoPtr<ArrayOf<String> > packageNames = GetPackageName();
    // if (success && packageNames != NULL){
    //     for(Int32 i = 0; i < packageNames->GetLength(); i++)
    //         Log.i(PRIVACY_TAG,"Allowed Package: -" + package_names[i] + "- accessing camera.");
    // }
    // else if (packageNames != NULL){
    //     for(Int32 i = 0; i < packageNames->GetLength(); i++)
    //         Log.i(PRIVACY_TAG,"Blocked Package: -" + package_names[i] + "- accessing camera.");
    // }
}

} // namespace Hardware
} // namespace Droid
} // namespace Elastos
