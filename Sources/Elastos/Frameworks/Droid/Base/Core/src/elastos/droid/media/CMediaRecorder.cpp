
#include "elastos/droid/media/CMediaRecorder.h"
#include "elastos/droid/media/CCamcorderProfile.h"
#include "elastos/droid/os/Looper.h"
#include "elastos/droid/os/Process.h"
#include "elastos/droid/os/Environment.h"
#include "elastos/droid/os/ServiceManager.h"
#include "elastos/droid/hardware/CHardwareCamera.h"
#include "elastos/droid/privacy/CPrivacySettingsManager.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringBuilder.h>
#include <gui/Surface.h>
#include <hardware/camera.h>
#include <camera/Camera.h>
#include <camera/ICameraService.h>
#include <media/mediarecorder.h>


using Elastos::Droid::Hardware::CHardwareCamera;
using Elastos::Droid::Os::Looper;
using Elastos::Droid::Os::ILooperHelper;
using Elastos::Droid::Os::Environment;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Privacy::IPrivacySettings;
using Elastos::Droid::Privacy::CPrivacySettingsManager;
using Elastos::Droid::Privacy::IIPrivacySettingsManager;
using Elastos::Utility::Logging::Logger;
using Elastos::IO::IFlushable;
using Elastos::IO::ICloseable;
using Elastos::IO::IFileWriter;
using Elastos::IO::CFileWriter;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::Core::IRandom;
using Elastos::Utility::CRandom;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Media {

const String CMediaRecorder::TAG("CMediaRecorder");
const String CMediaRecorder::PRIVACY_PATH_DEF("/data/data/");
const String CMediaRecorder::PRIVACY_TAG("PM,MediaRecorder");

const Int32 CMediaRecorder::STATE_RECORD_AUDIO = 0;
const Int32 CMediaRecorder::STATE_RECORD_BOTH = 1;
const Int32 CMediaRecorder::MODE_RECORD_AUDIO = 2;
const Int32 CMediaRecorder::MODE_RECORD_BOTH = 3;
const Int32 CMediaRecorder::IS_ALLOWED = -1;
const Int32 CMediaRecorder::IS_NOT_ALLOWED = -2;
const Int32 CMediaRecorder::GOT_ERROR = -3;

const Int32 CMediaRecorder::MIC_DATA_ACCESS = 10;
const Int32 CMediaRecorder::BOTH_DATA_ACCESS = 11;

static Object sLock;

// ----------------------------------------------------------------------------
// ref-counted object for callbacks
class JNIMediaRecorderListener: public android::MediaRecorderListener
{
public:
    JNIMediaRecorderListener(IWeakReference* weak_thiz);
    ~JNIMediaRecorderListener();
    void notify(int msg, int ext1, int ext2);

private:
    JNIMediaRecorderListener();
    AutoPtr<IWeakReference> mWeakObject;    // Weak ref to MediaRecorder Java object to call on
};

JNIMediaRecorderListener::JNIMediaRecorderListener(IWeakReference* weak_thiz)
{
    // We use a weak reference so the MediaRecorder object can be garbage collected.
    // The reference is only used as a proxy for callbacks.
    mWeakObject = weak_thiz;
}

JNIMediaRecorderListener::~JNIMediaRecorderListener()
{}

void JNIMediaRecorderListener::notify(int msg, int ext1, int ext2)
{
    CMediaRecorder::PostEventFromNative(mWeakObject, msg, ext1, ext2, NULL);
}

// Returns true if it throws an exception.
static ECode process_media_recorder_call(
    /* [in] */ android::status_t opStatus,
    /* [in] */ ECode exception,
    /* [in] */ const char* msg = NULL)
{
    //ALOGV("process_media_recorder_call");
    if (opStatus == (android::status_t)android::INVALID_OPERATION) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    else if (opStatus != android::OK) {
        if (msg != NULL) Logger::E(CMediaRecorder::TAG, msg);
        return exception;
    }
    return NOERROR;
}

static android::sp<android::MediaRecorder> getMediaRecorder(CMediaRecorder* thiz)
{
    AutoLock l(sLock);
    android::MediaRecorder* const p = (android::MediaRecorder*)thiz->mNativeContext;
    return android::sp<android::MediaRecorder>(p);
}

static android::sp<android::MediaRecorder> setMediaRecorder(
    CMediaRecorder* thiz, const android::sp<android::MediaRecorder>& recorder)
{
    AutoLock l(sLock);
    android::sp<android::MediaRecorder> old = (android::MediaRecorder*)thiz->mNativeContext;
    if (recorder.get()) {
        recorder->incStrong(thiz);
    }
    if (old != 0) {
        old->decStrong(thiz);
    }
    thiz->mNativeContext = (Int32)recorder.get();
    return old;
}

static android::sp<android::Surface> get_surface(ISurface* clazz)
{
    //ALOGV("get_surface");
    Handle32 surface;
    clazz->GetSurface(&surface);
    android::Surface* const p = (android::Surface*)surface;
    return android::sp<android::Surface>(p);
}

static android::sp<android::Camera> get_native_camera(
    /* [in] */ IHardwareCamera* thiz,
    /* [in] */ CHardwareCamera::JNICameraContext* obj)
{
   android::sp<android::Camera> camera;
   assert(0 && "TODO: android_hardware_Camera.cpp");
   return camera;
}

//================================================================================
//              CMediaRecorder::EventHandler
//================================================================================

const Int32 CMediaRecorder::EventHandler::MEDIA_RECORDER_EVENT_LIST_START =  1;
const Int32 CMediaRecorder::EventHandler::MEDIA_RECORDER_EVENT_ERROR =  1;
const Int32 CMediaRecorder::EventHandler::MEDIA_RECORDER_EVENT_INFO =  2;
const Int32 CMediaRecorder::EventHandler::MEDIA_RECORDER_EVENT_LIST_END   =  99;

const Int32 CMediaRecorder::EventHandler::MEDIA_RECORDER_TRACK_EVENT_LIST_START =  100;
const Int32 CMediaRecorder::EventHandler::MEDIA_RECORDER_TRACK_EVENT_ERROR      =  100;
const Int32 CMediaRecorder::EventHandler::MEDIA_RECORDER_TRACK_EVENT_INFO       =  101;
const Int32 CMediaRecorder::EventHandler::MEDIA_RECORDER_TRACK_EVENT_LIST_END   =  1000;

CMediaRecorder::EventHandler::EventHandler(
    /* [in] */ CMediaRecorder* host,
    /* [in] */ ILooper* looper)
    : HandlerBase(looper)
    , mMediaRecorder(host)
{
}

ECode CMediaRecorder::EventHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);

    if (mMediaRecorder->mNativeContext == 0) {
        Logger::W(CMediaRecorder::TAG, "mediarecorder went away with unhandled events");
        return NOERROR;
    }

    Int32 what, arg1, arg2;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    msg->GetArg2(&arg2);

    switch(what) {
        case MEDIA_RECORDER_EVENT_ERROR:
        case MEDIA_RECORDER_TRACK_EVENT_ERROR:
            if (mMediaRecorder->mOnErrorListener != NULL) {
                return mMediaRecorder->mOnErrorListener->OnError(mMediaRecorder, arg1, arg2);
            }
            return NOERROR;

        case MEDIA_RECORDER_EVENT_INFO:
        case MEDIA_RECORDER_TRACK_EVENT_INFO:
            if (mMediaRecorder->mOnInfoListener != NULL) {
                return mMediaRecorder->mOnInfoListener->OnInfo(mMediaRecorder, arg1, arg2);
            }
            return NOERROR;

        default:
            Logger::E(CMediaRecorder::TAG, "Unknown message type %d", what);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

//================================================================================
//              CMediaRecorder::PrivacyRunner
//================================================================================

CMediaRecorder::PrivacyRunner::PrivacyRunner(
    /* [in] */ CMediaRecorder* owner)
    : mMediaRecorder(owner)
{
}

ECode CMediaRecorder::PrivacyRunner::Run()
{
    // try{
        Thread::Sleep(mDelay);
        //now we're going to stop stream
        mMediaRecorder->PrivacyStop();
        if (!mMediaRecorder->mPath.IsNull()){
            AutoPtr<IFile> tmp;
            CFile::New(mMediaRecorder->mPath, (IFile**)&tmp);
            Boolean bval;
            tmp->Delete(&bval);
            if (bval) mMediaRecorder->mDeletedFile = TRUE;
        }
        else if (mMediaRecorder->mFileDescriptor != NULL
            && !mMediaRecorder->mFileDescriptorPath.IsNull()) {
            AutoPtr<IFile> tmp;
            CFile::New(mMediaRecorder->mFileDescriptorPath, (IFile**)&tmp);
            Boolean bval;
            tmp->Delete(&bval);
            if (bval) mMediaRecorder->mDeletedFile = TRUE;
        }
        else {
            Logger::E(CMediaRecorder::PRIVACY_TAG, "Can't delete temporary File, because all is null?! It could be that we only want to record audio?!");
            mMediaRecorder->mDeletedFile = FALSE;
        }
    // }
    // catch(Exception e){
    //     Log.e(PRIVACY_TAG,"Something went wrong while waiting for cancel the stream!");
    //     e.printStackTrace();
    // }
    // finally{
        mMediaRecorder->PrivacyStop();
    // }
    return NOERROR;
}

//================================================================================
//              CMediaRecorder
//================================================================================
CMediaRecorder::CMediaRecorder()
    : mNativeContext(0)
    , mDeletedFile(FALSE)
    , mPrivacyMode(FALSE)
    , mStoppedStream(FALSE)
    , ACTUAL_STATE(STATE_RECORD_AUDIO)
{}

CMediaRecorder::~CMediaRecorder()
{
    if (!mDeletedFile){
        if(mPath != NULL){
            AutoPtr< IFile > tmp;
            CFile::New(mPath,(IFile**)&tmp);
            Boolean t;
            if(tmp->Delete(&t))
                mDeletedFile = TRUE;
        } else if(mFileDescriptor != NULL && mFileDescriptorPath != NULL){
            AutoPtr< IFile > tmp;
            CFile::New(mFileDescriptorPath,(IFile**)&tmp);
            Boolean t;
            if(tmp->Delete(&t))
                mDeletedFile = TRUE;
        } else{
            // Log.e(PRIVACY_TAG,"Can't delete temporary File, because all is NULL?! It could be that we only want to record audio?!");
            mDeletedFile = FALSE;
        }
    }

    NativeFinalize();
}

ECode CMediaRecorder::constructor()
{
    AutoPtr<ILooper> looper = Looper::GetMyLooper();
    if (looper != NULL) {
        mEventHandler = new EventHandler(this, looper);
    }
    else if ((looper = Looper::GetMainLooper()) != NULL) {
        mEventHandler = new EventHandler(this, looper);
    }
    else {
        mEventHandler = NULL;
    }

    ////////////////////////////////////////////////////////////////////////
    //BEGIN PRIVACY

    FAIL_RETURN(Initiate());

    //END PRIVACY
    ///////////////////////////////////////////////////////////////////////

    /* Native setup requires a weak reference to our object.
     * It's easier to create it here than in C++.
     */
    return NativeSetup();
}

ECode CMediaRecorder::SetCamera(
    /* [in] */ IHardwareCamera* camera)
{
    // we should not pass a NULL camera to get_native_camera() call.
    if (camera == NULL) {
        Logger::E(TAG, "camera object is a NULL pointer");
        return E_NULL_POINTER_EXCEPTION;
    }

    android::sp<android::Camera> c = get_native_camera(camera, NULL);
    android::sp<android::MediaRecorder> mr = getMediaRecorder(this);

    return process_media_recorder_call(
        mr->setCamera(c->remote(), c->getRecordingProxy()),
        E_RUNTIME_EXCEPTION, "setCamera failed.");
}

ECode CMediaRecorder::SetPreviewDisplay(
    /* [in] */ ISurface* sv)
{
    //////////////////////////////////////////////////////////
    //BEGIN PRIVACY
    ACTUAL_STATE = STATE_RECORD_BOTH;
    //END PRIVACY
    //////////////////////////////////////////////////////////

    mSurface = sv;
    return NOERROR;
}

//////////////////////////////////////////////////////////
//BEGIN PRIVACY

ECode CMediaRecorder::PrivacyStop()
{
    // try{
    ECode ec = Stop();
    if (FAILED(ec)) {
        Logger::E(PRIVACY_TAG, "Got exception while trying to call privacyStop()");
    }
    return ec;
}

ECode CMediaRecorder::MakePrivacyPath(
    /* [in] */ const String& path)
{
    // try{
    AutoPtr<IFileWriter> fWriter;
    AutoPtr<IFile> deleteMe;
    Boolean isDeleted;

    ECode ec = CFileWriter::New(path, (IFileWriter**)&fWriter);
    FAIL_GOTO(ec, _EXIT_)

    ec = fWriter->WriteString( String("test") );
    FAIL_GOTO(ec, _EXIT_)

    ec = IFlushable::Probe(fWriter.Get())->Flush();
    FAIL_GOTO(ec, _EXIT_)
    ec = ICloseable::Probe(fWriter.Get())->Close();
    FAIL_GOTO(ec, _EXIT_)

    ec = CFile::New(path, (IFile**)&deleteMe);
    FAIL_GOTO(ec, _EXIT_)

    ec = deleteMe->Delete(&isDeleted);
    FAIL_GOTO(ec, _EXIT_)

    Logger::I(PRIVACY_TAG, "found our package with internal path. File: %s", path.string());

_EXIT_:
    fWriter = NULL;
    deleteMe = NULL;
    return ec;
    // } catch(Exception e){
        //we're not allowed to write in this directory -> this is not our package!
    // } finally{
        // fWriter = NULL;
        // deleteMe = NULL;
        // System.gc();
    // }
}

String CMediaRecorder::GetPrivacyPath()
{
    AutoPtr< ArrayOf<String> > packages = GetPackageName();
    AutoPtr <IRandom> value;
    CRandom::New((IRandom**)&value);
    String currentPath, currentPackage, data_name;
    Int64 temp;
    value->NextInt64(&temp);

    StringBuilder sb(32);
    sb.Append(temp);
    sb.Append(".tmp");
    sb.ToString(&data_name);

    ECode ec = NOERROR;
    Boolean succeeded;
    String path;

    for (Int32 i = 0; i < packages->GetLength(); i++){
        currentPackage = (*packages)[i];

        //first check if cache folder exist
        StringBuilder sb1(32);
        sb1.AppendString(PRIVACY_PATH_DEF);
        sb1.AppendString(currentPackage);
        sb1.AppendCStr("/cache/");
        sb1.ToString(&path);

        AutoPtr<IFile> folder;
        CFile::New(path,(IFile**)&folder);
        ec = folder->Mkdirs(&succeeded);
        if (SUCCEEDED(ec) && succeeded) {
            sb1.AppendString(data_name);
            sb1.ToString(&path);

            ec = MakePrivacyPath(path);
            if (SUCCEEDED(ec)) {
                //all is fine, break now and save our current package name!
                currentPath = path;
                break;
            }
        }
    }

    if (!currentPath.IsNull()){
        Logger::I(PRIVACY_TAG, "returned file: %s for package: %s with internal path. Path: %s/",
            data_name.string(), currentPackage.string(), currentPath.string());
        return currentPath;
    }
    else { //last chance, try to write to SD-Card
        String sdPath;
        AutoPtr<IFile> t = Environment::GetExternalStorageDirectory();
        t->GetAbsolutePath(&sdPath);

        StringBuilder sb(32);
        sb.Append(sdPath);
        sb.Append("/");
        sb.Append(data_name);
        sb.ToString(&currentPath);

        ec = MakePrivacyPath(currentPath);
        if (SUCCEEDED(ec)) {
            Logger::I(PRIVACY_TAG, "Return filePath:  %s. It is on SDCard!", currentPath.string());

            return currentPath;
        }
    }

    return String(NULL);
}

AutoPtr< IFileDescriptor > CMediaRecorder::GetPrivacyFileDescriptor()
{
    AutoPtr< ArrayOf<String> > packages = GetPackageName();
    AutoPtr <IRandom> value;
    CRandom::New((IRandom**)&value);
    String currentPath, currentPackage, data_name;
    Int64 temp;
    StringBuilder sb(32);
    value->NextInt64(&temp);
    sb.Append(temp);
    sb.Append( ".tmp" );
    sb.ToString(&data_name);

    ECode ec = NOERROR;
    Boolean succeeded;
    String path;

    for (Int32 i = 0 ; i < packages->GetLength() ; i++){
        currentPackage = (*packages)[i];

        //first check if cache folder exist
        StringBuilder sb1(32);
        sb1.AppendString(PRIVACY_PATH_DEF);
        sb1.AppendString(currentPackage);
        sb1.AppendCStr("/cache/");
        sb1.ToString(&path);

        AutoPtr<IFile> folder;
        CFile::New(path, (IFile**)&folder);
        ec = folder->Mkdirs(&succeeded);
        if (SUCCEEDED(ec) && succeeded) {
            sb1.AppendString(data_name);
            sb1.ToString(&path);

            ec = MakePrivacyPath(path);
            if (SUCCEEDED(ec)) {
                //all is fine, break now and save our current package name!
                currentPath = path;
                break;
            }
        }
    }

    Boolean isSDCard = FALSE;
    if (currentPath.IsNull()) {
        //last chance, try to write to SD-Card
        String sdPath;
        AutoPtr<IFile> t = Environment::GetExternalStorageDirectory();
        t->GetAbsolutePath(&sdPath);

        StringBuilder sb(32);
        sb.Append(sdPath);
        sb.Append("/");
        sb.Append(data_name);
        sb.ToString(&path);

        ec = MakePrivacyPath(path);
        if (SUCCEEDED(ec)) {
            isSDCard = TRUE;
            Logger::I(PRIVACY_TAG, "Return filePath:  %s. It is on SDCard!", path.string());
            currentPath = path;
        }
    }

    if (!currentPath.IsNull()){
        AutoPtr<IFileOutputStream> fos;
        CFileOutputStream::New(currentPath, (IFileOutputStream**)&fos);

        AutoPtr<IFileDescriptor> fD;
        ec = fos->GetFD((IFileDescriptor**)&fD);
        if (FAILED(ec)) {
            Logger::E(PRIVACY_TAG, "Got exception while creating fileDescriptor -> return NULL");
            return NULL;
        }

        if (!isSDCard) {
            Logger::I(PRIVACY_TAG, "returned fileDescriptor for package: %s with internal path. Path: %s",
                currentPackage.string(), currentPath.string());
        }
        else {
            Logger::I(PRIVACY_TAG, "returned fileDescriptor. Path:  Path: %s",
                currentPath.string());
        }

        mFileDescriptorPath = currentPath;
        return fD;
    }

    return NULL;
}

AutoPtr< ArrayOf<String> > CMediaRecorder::GetPackageName()
{
    // try{
    if (mPackageManager == NULL) {
        AutoPtr<IInterface> tmpObj = ServiceManager::GetService(String("package"));
        mPackageManager = IIPackageManager::Probe(tmpObj.Get());
    }

    if (mPackageManager != NULL) {
        Int32 uid = Process::MyUid();
        AutoPtr< ArrayOf<String> > package_names;
        mPackageManager->GetPackagesForUid(uid,(ArrayOf<String>**)&package_names);
        return package_names;
    }
    else {
        Logger::E(PRIVACY_TAG, "something went wrong with getting package name");
        return NULL;
    }
    // }
    // catch(Exception e){
        // e.printStackTrace();
        // Log.e(PRIVACY_TAG,"something went wrong with getting package name");
        // return NULL;
    // }
}

Int32 CMediaRecorder::CheckIfPackagesAllowed(
    /* [in] */ Int32 privacySetting)
{
    // try{
    //boolean isAllowed = false;
    if (mPrivacySettingsManager != NULL) {
        AutoPtr< IPrivacySettings > pSet;
        AutoPtr< ArrayOf<String> > package_names = GetPackageName();
        Int32 uid = Process::MyUid();
        if (package_names != NULL){
            switch(privacySetting) {
                case MODE_RECORD_AUDIO:
                    for (Int32 i = 0; i < package_names->GetLength(); i++) {
                        pSet = NULL;
                        mPrivacySettingsManager->GetSettings(
                            (*package_names)[i], uid,(IPrivacySettings**)&pSet);
                        //if pSet is NULL, we allow application to access to mic
                        if (pSet != NULL) {
                            Byte t;
                            pSet->GetRecordAudioSetting(&t);
                            if (t != IPrivacySettings::REAL) {
                                return IS_NOT_ALLOWED;
                            }
                        }
                        pSet = NULL;
                    }
                    return IS_ALLOWED;

                case MODE_RECORD_BOTH: {
                    for (Int32 i = 0; i < package_names->GetLength(); i++) {
                        pSet = NULL;
                        mPrivacySettingsManager->GetSettings(
                            (*package_names)[i], uid,(IPrivacySettings**)&pSet);
                        //if pSet is NULL, we allow application to access to mic
                        if (pSet != NULL) {
                            Byte t;
                            pSet->GetRecordAudioSetting(&t);
                            if (t != IPrivacySettings::REAL) {
                                return IS_NOT_ALLOWED;
                            }
                        }
                        pSet = NULL;
                    }
                    return IS_ALLOWED;
                }

                default: return GOT_ERROR;
            }
        }
        else{
            Logger::E(PRIVACY_TAG, "return GOT_ERROR, because package_names are NULL");
            return GOT_ERROR;
        }
    }
    else{
        Logger::E(PRIVACY_TAG, "return GOT_ERROR, because mPrivacySettingsManager is NULL");
        return GOT_ERROR;
    }
    // }
    // catch (Exception e){
        // e.printStackTrace();
        // Log.e(PRIVACY_TAG,"Got exception in checkIfPackagesAllowed");
        // return GOT_ERROR;
    // }
    return GOT_ERROR;
}

ECode CMediaRecorder::Initiate()
{
        context = NULL;
        AutoPtr<IIPrivacySettingsManager> ps = (IIPrivacySettingsManager*)ServiceManager::GetService(String("privacy")).Get();
        CPrivacySettingsManager::New(context, ps, (IPrivacySettingsManager**)&mPrivacySettingsManager);
        mPackageManager = (IIPackageManager*)ServiceManager::GetService(String("package")).Get();
        mPrivacyMode = TRUE;
    // }
    // catch(Exception e){
    //     e.printStackTrace();
    //     Log.e(PRIVACY_TAG, "Something went wrong with initalize variables");
    //     privacyMode = false;
    // }

    return NOERROR;
}

void CMediaRecorder::DataAccess(
    /* [in] */ Boolean success,
    /* [in] */ Int32 micOrBoth)
{
    AutoPtr< ArrayOf<String> > package_names = GetPackageName();
    if (success && package_names != NULL){
        switch(micOrBoth){
        case MIC_DATA_ACCESS:
            for (Int32 i = 0; i < package_names->GetLength(); i++)
                Logger::I(PRIVACY_TAG, "Allowed Package: -%s- accessing microphone.", (*package_names)[i].string());
            break;
        case BOTH_DATA_ACCESS:
            for (Int32 i=0; i < package_names->GetLength(); i++)
                Logger::I(PRIVACY_TAG, "Allowed Package: -%s- accessing microphone and camera.", (*package_names)[i].string());
            break;
        }
    }
    else if (package_names != NULL){
        switch(micOrBoth){
        case MIC_DATA_ACCESS:
            for (Int32 i = 0; i < package_names->GetLength(); i++)
                Logger::I(PRIVACY_TAG, "Blocked Package: -%s- accessing microphone.", (*package_names)[i].string());
            break;

        case BOTH_DATA_ACCESS:
            for (Int32 i = 0; i < package_names->GetLength(); i++)
                Logger::I(PRIVACY_TAG, "Blocked Package: -%s- accessing microphone and camera.", (*package_names)[i].string());
            break;
        }
    }
}
//END PRIVACY
//////////////////////////////////////////////////////////

ECode CMediaRecorder::SetAudioSource(
    /* [in] */ Int32 audio_source)
{
    //ALOGV("setAudioSource(%d)", as);
    if (audio_source < AUDIO_SOURCE_DEFAULT
        || audio_source >= AUDIO_SOURCE_CNT) {
        Logger::E(TAG, "Invalid audio source");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    android::sp<android::MediaRecorder> mr = getMediaRecorder(this);
    return process_media_recorder_call(mr->setAudioSource(audio_source),
        E_RUNTIME_EXCEPTION, "setAudioSource failed.");
}

ECode CMediaRecorder::GetAudioSourceMax(
    /* [out] */ Int32 * result)
{
    VALIDATE_NOT_NULL(result);

    // FIXME disable selection of the remote submxi source selection once test code
    //       doesn't rely on it
    *result = IAudioSource::REMOTE_SUBMIX_SOURCE;
    //return IAudioSource::VOICE_COMMUNICATION;
    return NOERROR;
}

ECode CMediaRecorder::SetVideoSource(
    /* [in] */ Int32 video_source)
{
    //ALOGV("setVideoSource(%d)", vs);
    if (video_source < android::VIDEO_SOURCE_DEFAULT
        || video_source >= android::VIDEO_SOURCE_LIST_END) {
        Logger::E(TAG, "Invalid video source");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    android::sp<android::MediaRecorder> mr = getMediaRecorder(this);
    return process_media_recorder_call(mr->setVideoSource(video_source),
        E_RUNTIME_EXCEPTION, "setAudioSource failed.");
}

ECode CMediaRecorder::SetProfile(
    /* [in] */ ICamcorderProfile* profile)
{
    Int32 fileFormat;
    Int32 videoFrameRate, videoFrameWidth, videoFrameHeight;
    Int32 videoBitRate, videoCodec;

    //////////////////////////////////////////////////////////
    //BEGIN PRIVACY
    ACTUAL_STATE = STATE_RECORD_BOTH;
    //END PRIVACY
    //////////////////////////////////////////////////////////

    profile->GetFileFormat(&fileFormat);
    profile->GetVideoFrameRate(&videoFrameRate);
    profile->GetVideoFrameWidth(&videoFrameWidth);
    profile->GetVideoFrameHeight(&videoFrameHeight);
    profile->GetVideoBitRate(&videoBitRate);
    profile->GetVideoCodec(&videoCodec);

    SetOutputFormat(fileFormat);
    SetVideoFrameRate(videoFrameRate);
    SetVideoSize(videoFrameWidth, videoFrameHeight);
    SetVideoEncodingBitRate(videoBitRate);
    SetVideoEncoder(videoCodec);

    Int32 quality;

    profile->GetQuality(&quality);
    if (quality >= ICamcorderProfile::QUALITY_TIME_LAPSE_LOW &&
        quality <= ICamcorderProfile::QUALITY_TIME_LAPSE_QVGA) {
        // Nothing needs to be done. Call to setCaptureRate() enables
        // time lapse video recording.
    }
    else {
        Int32 audioBitRate, audioChannels, audioSampleRate, audioCodec;

        profile->GetAudioBitRate(&audioBitRate);
        profile->GetAudioChannels(&audioChannels);
        profile->GetAudioSampleRate(&audioSampleRate);
        profile->GetAudioCodec(&audioCodec);
        SetAudioEncodingBitRate(audioBitRate);
        SetAudioChannels(audioChannels);
        SetAudioSamplingRate(audioSampleRate);
        SetAudioEncoder(audioCodec);
    }
    return NOERROR;
}

ECode CMediaRecorder::SetCaptureRate(
    /* [in] */ Double fps)
{
    //////////////////////////////////////////////////////////
    //BEGIN PRIVACY
    ACTUAL_STATE = STATE_RECORD_BOTH;
    //END PRIVACY
    //////////////////////////////////////////////////////////

    // Make sure that time lapse is enabled when this method is called.
    SetParameter("time-lapse-enable=1");

    Double timeBetweenFrameCapture = 1 / fps;
    Int32 timeBetweenFrameCaptureMs = (Int32) (1000 * timeBetweenFrameCapture);
    StringBuilder sb("time-between-time-lapse-frame-capture=");
    sb.Append(timeBetweenFrameCaptureMs);
    return SetParameter(sb.ToString().string());
}

ECode CMediaRecorder::SetOrientationHint(
    /* [in] */ Int32 degrees)
{
    if (degrees != 0 && degrees != 90 && degrees != 180 && degrees != 270) {
        Logger::E(TAG, "Unsupported angle: %d", degrees);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    StringBuilder sb("video-param-rotation-angle-degrees=");
    sb.Append(degrees);
    return SetParameter(sb.ToString().string());
}

ECode CMediaRecorder::SetLocation(
    /* [in] */ Float latitude,
    /* [in] */ Float longitude)
{
    Int32 latitudex10000  = (Int32) (latitude * 10000 + 0.5);
    Int32 longitudex10000 = (Int32) (longitude * 10000 + 0.5);

    if (latitudex10000 > 900000 || latitudex10000 < -900000) {
        Logger::E(TAG, "Latitude: %f out of range.", latitude);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (longitudex10000 > 1800000 || longitudex10000 < -1800000) {
        Logger::E(TAG, "Longitude: %f out of range.", longitude);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    SetParameter(String("param-geotag-latitude=" + latitudex10000));
    SetParameter(String("param-geotag-longitude=" + longitudex10000));
    return NOERROR;
}

ECode CMediaRecorder::SetOutputFormat(
    /* [in] */ Int32 output_format)
{
    //ALOGV("setOutputFormat(%d)", of);
    if (output_format < android::OUTPUT_FORMAT_DEFAULT
        || output_format >= android::OUTPUT_FORMAT_LIST_END) {
        Logger::E(TAG, "Invalid output format");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    android::sp<android::MediaRecorder> mr = getMediaRecorder(this);
    return process_media_recorder_call(mr->setOutputFormat(output_format),
        E_RUNTIME_EXCEPTION, "setOutputFormat failed.");
}

/*JNI Method */ //throws IllegalStateException;
ECode CMediaRecorder::SetVideoSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    //ALOGV("setVideoSize(%d, %d)", width, height);
    android::sp<android::MediaRecorder> mr = getMediaRecorder(this);

    if (width <= 0 || height <= 0) {
        Logger::E(TAG, "invalid video size");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return process_media_recorder_call(mr->setVideoSize(width, height),
        E_RUNTIME_EXCEPTION, "setVideoSize failed.");
}

ECode CMediaRecorder::SetVideoFrameRate(
    /* [in] */ Int32 rate)
{
    //ALOGV("setVideoFrameRate(%d)", rate);
    if (rate <= 0) {
        Logger::E(TAG, "invalid frame rate");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    android::sp<android::MediaRecorder> mr = getMediaRecorder(this);
    return process_media_recorder_call(mr->setVideoFrameRate(rate),
        E_RUNTIME_EXCEPTION, "SetVideoFrameRate failed.");
}

ECode CMediaRecorder::SetMaxDuration(
    /* [in] */ Int32 max_duration_ms)
{
    //ALOGV("setMaxDuration(%d)", maxDurationMs);
    android::sp<android::MediaRecorder> mr = getMediaRecorder(this);

    char params[64];
    sprintf(params, "max-duration=%d", max_duration_ms);

    return process_media_recorder_call(mr->setParameters(android::String8(params)),
        E_RUNTIME_EXCEPTION, "SetMaxDuration failed.");
}

ECode CMediaRecorder::SetMaxFileSize(
    /* [in] */ Int64 max_filesize_bytes)
{
    //ALOGV("setMaxFileSize(%lld)", maxFilesizeBytes);
    android::sp<android::MediaRecorder> mr = getMediaRecorder(this);

    char params[64];
    sprintf(params, "max-filesize=%lld", max_filesize_bytes);

    return process_media_recorder_call(mr->setParameters(android::String8(params)),
        E_RUNTIME_EXCEPTION, "SetMaxFileSize failed.");
}

ECode CMediaRecorder::SetAudioEncoder(
    /* [in] */ Int32 audio_encoder)
{
    //ALOGV("setAudioEncoder(%d)", ae);
    if (audio_encoder < android::AUDIO_ENCODER_DEFAULT
        || audio_encoder >= android::AUDIO_ENCODER_LIST_END) {
        Logger::E(TAG, "Invalid audio encoder");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    android::sp<android::MediaRecorder> mr = getMediaRecorder(this);
    return process_media_recorder_call(mr->setAudioEncoder(audio_encoder),
        E_RUNTIME_EXCEPTION, "SetAudioEncoder failed.");
}

ECode CMediaRecorder::SetVideoEncoder(
    /* [in] */ Int32 video_encoder)
{
    //ALOGV("setVideoEncoder(%d)", ve);
    if (video_encoder < android::VIDEO_ENCODER_DEFAULT
        || video_encoder >= android::VIDEO_ENCODER_LIST_END) {
        Logger::E(TAG, "Invalid video encoder");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    android::sp<android::MediaRecorder> mr = getMediaRecorder(this);
    return process_media_recorder_call(mr->setVideoEncoder(video_encoder),
        E_RUNTIME_EXCEPTION, "SetAudioEncoder failed.");
}

ECode CMediaRecorder::SetAudioSamplingRate(
    /* [in] */ Int32 samplingRate)
{
    if (samplingRate <= 0) {
        Logger::E(TAG, "Audio sampling rate is not positive");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    StringBuilder sb("audio-param-sampling-rate=");
    sb.Append(samplingRate);
    return SetParameter(sb.ToString().string());
}

ECode CMediaRecorder::SetAudioChannels(
    /* [in] */ Int32 numChannels)
{
    if (numChannels <= 0) {
        Logger::E(TAG, "Number of channels is not positive");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    StringBuilder sb("audio-param-number-of-channels=");
    sb.Append(numChannels);
    return SetParameter(sb.ToString().string());
}

ECode CMediaRecorder::SetAudioEncodingBitRate(
    /* [in] */ Int32 bitRate)
{
    if (bitRate <= 0) {
        Logger::E(TAG, "Audio encoding bit rate is not positive");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    StringBuilder sb("audio-param-encoding-bitrate=");
    sb.Append(bitRate);
    return SetParameter(sb.ToString().string());
}

ECode CMediaRecorder::SetVideoEncodingBitRate(
    /* [in] */ Int32 bitRate)
{
    //////////////////////////////////////////////////////////
    //BEGIN PRIVACY
    ACTUAL_STATE = STATE_RECORD_BOTH;
    //END PRIVACY
    //////////////////////////////////////////////////////////

    if (bitRate <= 0) {
        Logger::E(TAG, "Video encoding bit rate is not positive");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    StringBuilder sb("video-param-encoding-bitrate=");
    sb.Append(bitRate);
    return SetParameter(sb.ToString().string());
}

ECode CMediaRecorder::SetAuxiliaryOutputFile(
    /* [in] */ IFileDescriptor* fd)
{
    Logger::W(TAG, "setAuxiliaryOutputFile(FileDescriptor) is no longer supported.");
    return NOERROR;
}

ECode CMediaRecorder::SetAuxiliaryOutputFile(
    /* [in] */ const String& path)
{
    Logger::W(TAG, "setAuxiliaryOutputFile(String) is no longer supported.");
    return NOERROR;
}

ECode CMediaRecorder::SetOutputFile(
    /* [in] */ IFileDescriptor* fd)
{
    mPath = NULL;
    mFileDescriptor = fd;
    mDeletedFile = FALSE;
    return NOERROR;
}

ECode CMediaRecorder::SetOutputFile(
    /* [in] */ const String& path)
{
    mFileDescriptor = NULL;
    mPath = path;
    mDeletedFile = FALSE;
    return NOERROR;
}

ECode CMediaRecorder::NativeSetOutputFile(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 length)
{
    //ALOGV("setOutputFile");
    if (fd == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 rawFd;
    fd->GetDescriptor(&rawFd);
    android::sp<android::MediaRecorder> mr = getMediaRecorder(this);
    android::status_t opStatus = mr->setOutputFile(rawFd, offset, length);
    return process_media_recorder_call(opStatus,
        E_IO_EXCEPTION, "setOutputFile failed.");
}

ECode CMediaRecorder::NativePrepare()
{
    //ALOGV("prepare");
    android::sp<android::MediaRecorder> mr = getMediaRecorder(this);

    if (mSurface != NULL) {
        const android::sp<android::Surface> native_surface = get_surface(mSurface);

        // The application may misbehave and
        // the preview surface becomes unavailable
        if (native_surface.get() == 0) {
            Logger::E(TAG, "Application lost the surface: invalid preview surface");
            return E_IO_EXCEPTION;
        }

        Logger::I(TAG, "prepare: surface=%p (identity=%d)", native_surface.get(), native_surface->getIdentity());
        ECode ec = process_media_recorder_call(mr->setPreviewSurface(native_surface),
            E_RUNTIME_EXCEPTION, "setPreviewSurface failed.");
        if (SUCCEEDED(ec)) return NOERROR;
    }

    return process_media_recorder_call(mr->prepare(),
        E_IO_EXCEPTION, "prepare failed.");
}

ECode CMediaRecorder::Prepare()
{
    ECode ec = NOERROR;

    //////////////////////////////////////////////////////////
    //BEGIN PRIVACY
    if(!mPrivacyMode){
        Initiate();
    }

    mDeletedFile = FALSE;
    Boolean skip = FALSE;
    String nullStr;

    switch (ACTUAL_STATE) {
        case STATE_RECORD_AUDIO:
            if (CheckIfPackagesAllowed(MODE_RECORD_AUDIO) == IS_NOT_ALLOWED
            /* || checkIfPackagesAllowed(MODE_RECORD_BOTH) == IS_NOT_ALLOWED*/) {
                AutoPtr<ArrayOf<String> > x = GetPackageName();
                if(x != NULL && x->GetLength() > 0)
                    mPrivacySettingsManager->Notification(
                        (*x)[0], 0, IPrivacySettings::EMPTY,
                        IPrivacySettings::DATA_RECORD_AUDIO, nullStr, NULL);

                mRunner = new PrivacyRunner(this);
                //here wo do not need to exchange the path or filedescriptor, because we can interrupt very quick!
                mRunner->SetDelay(50); // try very low value
                mRunner->Start();
                skip = TRUE;
//              if(x != NULL) Log.i(PRIVACY_TAG,"now throw exception in prepare method for package: " + x[0]);
//              else Log.i(PRIVACY_TAG,"now throw exception in prepare method");
//              if(ACTUAL_STATE == STATE_RECORD_BOTH){
//                  dataAccess(false, BOTH_DATA_ACCESS);
//                  if(x != NULL)
//                      mPrivacySettingsManager.notification(x[0], 0, PrivacySettings.EMPTY, PrivacySettings.DATA_CAMERA, NULL, mPrivacySettingsManager.getSettings(x[0], Process.myUid()));
//              }
//              else{
//                  dataAccess(false, MIC_DATA_ACCESS);
//                  if(x != NULL)
//                      mPrivacySettingsManager.notification(x[0], 0, PrivacySettings.EMPTY, PrivacySettings.DATA_RECORD_AUDIO, NULL, mPrivacySettingsManager.getSettings(x[0], Process.myUid()));
//                  //now test something, because a lot of applications crashes if we throw illegalstateException. We intercept now when applications wants to record audio!
//                  //skip = true;
//                  //break;
//              }
//              throw new IllegalStateException(); //now throw exception to prevent recording
            }
            break;

        case STATE_RECORD_BOTH:
            if (CheckIfPackagesAllowed(MODE_RECORD_BOTH) == IS_NOT_ALLOWED){
                AutoPtr<ArrayOf<String> > x = GetPackageName();
                if(x != NULL && x->GetLength() > 0)
                    mPrivacySettingsManager->Notification(
                        (*x)[0], 0, IPrivacySettings::EMPTY,
                        IPrivacySettings::DATA_CAMERA, nullStr, NULL);

                if (mPath != NULL){
                    //now overwrite path
                    mPath = GetPrivacyPath();
                }
                else if (mFileDescriptor != NULL){
                    //now overwrite fileDescriptor
                    mFileDescriptor = GetPrivacyFileDescriptor();
                }
                else {
                    //no chance to get it, throw exception
                    Logger::E(TAG, "No valid output file");
                    return E_IO_EXCEPTION;
                }

                mRunner = new PrivacyRunner(this);
                //we use default time for video record
                mRunner->Start();
                skip = TRUE;

            }
            break;
     }
    //END PRIVACY
    //////////////////////////////////////////////////////////
    AutoPtr<ArrayOf<String> > packageName = GetPackageName();
    if(!skip){
        if (ACTUAL_STATE == STATE_RECORD_BOTH && packageName != NULL && packageName->GetLength() > 0) {
            mPrivacySettingsManager->Notification(
                (*packageName)[0], 0, IPrivacySettings::REAL,
                IPrivacySettings::DATA_CAMERA, nullStr, NULL);
        }
        else if (packageName != NULL && packageName->GetLength() > 0) {
            mPrivacySettingsManager->Notification(
                (*packageName)[0], 0, IPrivacySettings::REAL,
                IPrivacySettings::DATA_RECORD_AUDIO, nullStr, NULL);
        }
        mDeletedFile = true;
    }


    if (mPath != NULL) {
        AutoPtr<IFileOutputStream> fos;
        CFileOutputStream::New(mPath, (IFileOutputStream**)&fos);
        //try {
            AutoPtr<IFileDescriptor> fileDescriptor;
            ec = fos->GetFD((IFileDescriptor**)&fileDescriptor);
            if (FAILED(ec)) {
                fos->Close();
                goto Last;
            }
            NativeSetOutputFile(fileDescriptor, 0, 0);
        //} finally {
            fos->Close();
        //}

    }
    else if (mFileDescriptor != NULL) {
        NativeSetOutputFile(mFileDescriptor, 0, 0);
    }
    else {
        //throw new IOException("No valid output file");
        return E_IO_EXCEPTION;
    }

Last:
    NativePrepare();
    return ec;
}

ECode CMediaRecorder::Start()
{
    //ALOGV("start");
    android::sp<android::MediaRecorder> mr = getMediaRecorder(this);
    return process_media_recorder_call(mr->start(),
        E_RUNTIME_EXCEPTION, "start failed.");
}

ECode CMediaRecorder::Stop()
{
    //ALOGV("stop");
    android::sp<android::MediaRecorder> mr = getMediaRecorder(this);
    return process_media_recorder_call(mr->stop(),
        E_RUNTIME_EXCEPTION, "stop failed.");
}

ECode CMediaRecorder::Reset()
{
    NativeReset();

    if (!mDeletedFile) {
        if (!mPath.IsNull()) {
            AutoPtr< IFile > tmp;
            CFile::New(mPath,(IFile**)&tmp);
            Boolean t;
            tmp->Delete(&t);
            if (t) mDeletedFile = TRUE;
        }
        else if (mFileDescriptor != NULL && mFileDescriptorPath != NULL) {
            AutoPtr< IFile > tmp;
            CFile::New(mFileDescriptorPath, (IFile**)&tmp);
            Boolean t;
            tmp->Delete(&t);
            if (t) mDeletedFile = TRUE;
        }
        else{
            Logger::E(PRIVACY_TAG, "Can't delete temporary File, because all is NULL?!"
                " It could be that we only want to record audio?!");
            mDeletedFile = FALSE;
        }
    }

    mRunner = NULL;

    // make sure none of the listeners get called anymore
    mEventHandler->RemoveCallbacksAndMessages(NULL);
    return NOERROR;
}

ECode CMediaRecorder::NativeReset()
{
    //ALOGV("native_reset");
    android::sp<android::MediaRecorder> mr = getMediaRecorder(this);
    return process_media_recorder_call(mr->reset(),
        E_RUNTIME_EXCEPTION, "native_reset failed.");
}

ECode CMediaRecorder::GetMaxAmplitude(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    android::sp<android::MediaRecorder> mr = getMediaRecorder(this);
    return process_media_recorder_call(mr->getMaxAmplitude(result),
        E_RUNTIME_EXCEPTION, "getMaxAmplitude failed.");
}

ECode CMediaRecorder::SetOnErrorListener(
    /* [in] */ IMediaRecorderOnErrorListener* l)
{
    mOnErrorListener = l;
    return NOERROR;
}

ECode CMediaRecorder::SetOnInfoListener(
    /* [in] */ IMediaRecorderOnInfoListener* listener)
{
    mOnInfoListener = listener;
    return NOERROR;
}

ECode CMediaRecorder::ReleaseResources()
{
    //ALOGV("release");
    android::sp<android::MediaRecorder> mr = setMediaRecorder(this, 0);
    if (mr != NULL) {
        mr->setListener(NULL);
        mr->release();
    }
    return NOERROR;
}

ECode CMediaRecorder::NativeSetup()
{
    //ALOGV("setup");
    android::sp<android::MediaRecorder> mr = new android::MediaRecorder();
    if (mr == NULL) {
        Logger::E(TAG, "Out of memory.");
        return E_RUNTIME_EXCEPTION;
    }

    if (mr->initCheck() != android::NO_ERROR) {
        Logger::E(TAG, "Unable to initialize media recorder.");
        return E_RUNTIME_EXCEPTION;
    }

    // create new listener and give it to MediaRecorder
    AutoPtr<IWeakReference> wr;
    GetWeakReference((IWeakReference**)&wr);
    android::sp<JNIMediaRecorderListener> listener = new JNIMediaRecorderListener(wr);
    mr->setListener(listener);

    setMediaRecorder(this, mr);
    return NOERROR;
}

/*JNI Method*/
ECode CMediaRecorder::NativeFinalize()
{
    //ALOGV("release");
    android::sp<android::MediaRecorder> mr = setMediaRecorder(this, 0);
    if (mr != NULL) {
        mr->setListener(NULL);
        mr->release();
    }
    return NOERROR;
}

/*JNI Method*/
ECode CMediaRecorder::SetParameter(
    /* [in] */ const char* nameValuePair)
{
    //ALOGV("setParameter()");
    if (nameValuePair == NULL){
        Logger::E(TAG, "Invalid or empty params string. This parameter will be ignored.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    android::sp<android::MediaRecorder> mr = getMediaRecorder(this);
    return process_media_recorder_call(mr->setParameters(android::String8(nameValuePair)),
        E_RUNTIME_EXCEPTION, "setParameter failed.");
}

/*static*/
ECode CMediaRecorder::PostEventFromNative(
    /* [in] */ IWeakReference* mediarecorder_ref,
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IInterface* obj)
{
    AutoPtr<IMediaRecorder> strongObj;
    mediarecorder_ref->Resolve(EIID_IMediaRecorder, (IInterface**)&strongObj);
    if (strongObj == NULL) {
        return NOERROR;
    }

    CMediaRecorder* mr = (CMediaRecorder*)strongObj.Get();
    if (mr->mEventHandler != NULL) {
        AutoPtr<IMessage> message;
        mr->mEventHandler->ObtainMessage(what, arg1, arg2, obj, (IMessage**)&message);
        Boolean bval;
        return mr->mEventHandler->SendMessage(message, &bval);
    }
    return NOERROR;
}


} // namespace Media
} // namepsace Droid
} // namespace Elastos
