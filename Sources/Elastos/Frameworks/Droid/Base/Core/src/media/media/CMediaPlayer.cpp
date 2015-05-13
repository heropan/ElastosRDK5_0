#include "media/CMediaPlayer.h"
#include "ext/frameworkext.h"
#include "media/CMetadata.h"
#include "media/CTimedText.h"
#include "media/CMediaPlayerSubInfo.h"
#include "media/CTrackInfoVendor.h"
#include "media/Media_Utils.h"
#include "os/Looper.h"
#include <elastos/Slogger.h>
#include <elastos/Logger.h>
#include <binder/Parcel.h>
#include <gui/Surface.h>
#include <utils/String8.h>
#include <media/mediaplayer.h>

using Elastos::Droid::Content::Res::IAssetFileDescriptor;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Os::IPowerManager;
using Elastos::Droid::Os::Looper;
using Elastos::Net::IInetAddress;
using Elastos::Utility::Logging::Logger;
using Elastos::Utility::Logging::Slogger;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::IO::CFileDescriptor;
using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;

namespace Elastos {
namespace Droid {
namespace Media {

const String CMediaPlayer::TAG("CMediaPlayer");
const Boolean CMediaPlayer::LOGV = FALSE;

// Name of the remote interface for the media player. Must be kept
// in sync with the 2nd parameter of the IMPLEMENT_META_INTERFACE
// macro invocation in IMediaPlayer.cpp
const String CMediaPlayer::IMEDIA_PLAYER("android.media.IMediaPlayer");

const String CMediaPlayer::SOFTWINNER_DLNA_SOURCE_DETECTOR("com.softwinner.dlnasourcedetector");

const Int32 CMediaPlayer::INVOKE_ID_GET_TRACK_INFO = 1;
const Int32 CMediaPlayer::INVOKE_ID_ADD_EXTERNAL_SOURCE = 2;
const Int32 CMediaPlayer::INVOKE_ID_ADD_EXTERNAL_SOURCE_FD = 3;
const Int32 CMediaPlayer::INVOKE_ID_SELECT_TRACK = 4;
const Int32 CMediaPlayer::INVOKE_ID_DESELECT_TRACK = 5;
const Int32 CMediaPlayer::INVOKE_ID_SET_VIDEO_SCALE_MODE = 6;

const Int32 CMediaPlayer::MEDIA_NOP = 0; // interface test message
const Int32 CMediaPlayer::MEDIA_PREPARED = 1;
const Int32 CMediaPlayer::MEDIA_PLAYBACK_COMPLETE = 2;
const Int32 CMediaPlayer::MEDIA_BUFFERING_UPDATE = 3;
const Int32 CMediaPlayer::MEDIA_SEEK_COMPLETE = 4;
const Int32 CMediaPlayer::MEDIA_SET_VIDEO_SIZE = 5;
const Int32 CMediaPlayer::MEDIA_TIMED_TEXT = 99;
const Int32 CMediaPlayer::MEDIA_ERROR = 100;
const Int32 CMediaPlayer::MEDIA_INFO = 200;
const Int32 CMediaPlayer::MEDIA_SOURCE_DETECTED = 234;   //Add by Bevis.
const Int32 CMediaPlayer::MEDIA_IO_ERROR = 300;  //Add by leven

Int32 CMediaPlayer::mRawDataMode = 1;//IMediaPlayer::AUDIO_DATA_MODE_PCM;

static Mutex sLock;

//===================================================================
//                  JNIMediaPlayerListener
//===================================================================

// ----------------------------------------------------------------------------
// ref-counted object for callbacks
class JNIMediaPlayerListener : public android::MediaPlayerListener
{
public:
    JNIMediaPlayerListener(IWeakReference* weak_thiz);

    ~JNIMediaPlayerListener();

    void notify(int msg, int ext1, int ext2, const android::Parcel* obj);

private:
    JNIMediaPlayerListener();
    AutoPtr<IWeakReference> mWeakObject;    // Weak ref to MediaPlayer Java object to call on
};

JNIMediaPlayerListener::JNIMediaPlayerListener(IWeakReference* weak_thiz)
{
    // We use a weak reference so the MediaPlayer object can be garbage collected.
    // The reference is only used as a proxy for callbacks.
    mWeakObject  = weak_thiz;
}

JNIMediaPlayerListener::~JNIMediaPlayerListener()
{
}

void JNIMediaPlayerListener::notify(int msg, int ext1, int ext2, const android::Parcel* obj)
{
    CMediaPlayer::PostEventFromNative(mWeakObject, msg, ext1, ext2, 0);
}

static android::sp<android::MediaPlayer> getMediaPlayer(CMediaPlayer* thiz)
{
    Mutex::Autolock l(sLock);
    android::MediaPlayer* const p = (android::MediaPlayer*)thiz->mNativeContext;
    return android::sp<android::MediaPlayer>(p);
}

static android::sp<android::ISurfaceTexture> getVideoSurfaceTexture(CMediaPlayer* thiz)
{
    android::ISurfaceTexture * const p = reinterpret_cast<android::ISurfaceTexture *>(thiz->mNativeSurfaceTexture);
    return android::sp<android::ISurfaceTexture>(p);
}

static android::sp<android::MediaPlayer> setMediaPlayer(CMediaPlayer* thiz, const android::sp<android::MediaPlayer>& player)
{
    Mutex::Autolock l(sLock);
    android::sp<android::MediaPlayer> old = (android::MediaPlayer*)thiz->mNativeContext;
    if (player.get()) {
        player->incStrong(thiz);
    }
    if (old != 0) {
        old->decStrong(thiz);
    }
    thiz->mNativeContext = (Int32)player.get();
    return old;
}

// If exception is NULL and opStatus is not OK, this method sends an error
// event to the client application; otherwise, if exception is not NULL and
// opStatus is not OK, this method throws the given exception to the client
// application.
static ECode process_media_player_call(CMediaPlayer* thiz, android::status_t opStatus,
    ECode ec, const char* msg = NULL)
{
    if (ec == NOERROR) {  // Don't throw exception. Instead, send an event.
        if (opStatus != (android::status_t) android::OK) {
            if (CMediaPlayer::LOGV) {
                Logger::E(CMediaPlayer::TAG, "process_media_player_call %d failed: %s", opStatus, msg);
            }
            android::sp<android::MediaPlayer> mp = getMediaPlayer(thiz);
            if (mp != 0) mp->notify(android::MEDIA_ERROR, opStatus, 0);
        }
    }
    else {  // Throw exception!
        if (opStatus == (android::status_t)android::INVALID_OPERATION ) {
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        else if ( opStatus == (android::status_t) android::PERMISSION_DENIED ) {
            return E_SECURITY_EXCEPTION;
        }
        else if (opStatus != (android::status_t) android::OK) {
            if (msg != NULL) {
                Logger::E(CMediaPlayer::TAG, "process_media_player_call %d failed: %s", opStatus, msg);
            }
            return ec;
        }
    }

    return NOERROR;
}

static android::sp<android::Surface> android_view_Surface_getSurface(ISurface* surfaceObj)
{
    Handle32 surface;
    return reinterpret_cast<android::Surface*>(surfaceObj->GetSurface(&surface), surface);
}

//===================================================================
//                  CMediaPlayer::EventHandler
//===================================================================
CMediaPlayer::EventHandler::EventHandler(
    /* [in] */ IWeakReference* host,
    /* [in] */ ILooper* looper)
    : HandlerBase(looper)
    , mMediaPlayer(host)
{
}

ECode CMediaPlayer::EventHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);

    AutoPtr<IMediaPlayer> mp;
    mMediaPlayer->Resolve(EIID_IMediaPlayer, (IInterface**)&mp);
    if (mp == NULL) {
        return NOERROR;
    }

    CMediaPlayer* mediaPlayer = (CMediaPlayer*)mp.Get();
    if (mediaPlayer->mNativeContext == 0) {
        Logger::W(CMediaPlayer::TAG, "mediaplayer went away with unhandled events");
        return NOERROR;
    }

    Int32 what, arg1, arg2;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    msg->GetArg2(&arg2);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);

    switch (what) {
    case CMediaPlayer::MEDIA_PREPARED:
        if (mediaPlayer->mOnPreparedListener != NULL)
            mediaPlayer->mOnPreparedListener->OnPrepared(mediaPlayer);
        return NOERROR;

    case CMediaPlayer::MEDIA_PLAYBACK_COMPLETE:
        if (mediaPlayer->mOnCompletionListener != NULL)
            mediaPlayer->mOnCompletionListener->OnCompletion(mediaPlayer);
        mediaPlayer->StayAwake(FALSE);
        return NOERROR;

    case CMediaPlayer::MEDIA_BUFFERING_UPDATE:
        if (mediaPlayer->mOnBufferingUpdateListener != NULL)
            mediaPlayer->mOnBufferingUpdateListener->OnBufferingUpdate(
                mediaPlayer, arg1);
        return NOERROR;

    case CMediaPlayer::MEDIA_SEEK_COMPLETE:
      if (mediaPlayer->mOnSeekCompleteListener != NULL)
          mediaPlayer->mOnSeekCompleteListener->OnSeekComplete(mediaPlayer);
      return NOERROR;

    case CMediaPlayer::MEDIA_SET_VIDEO_SIZE:
      if (mediaPlayer->mOnVideoSizeChangedListener != NULL)
          mediaPlayer->mOnVideoSizeChangedListener->OnVideoSizeChanged(
            mediaPlayer, arg1, arg2);
      return NOERROR;

    case CMediaPlayer::MEDIA_ERROR: {
        Logger::E(CMediaPlayer::TAG, "Error (%d, %d)", arg1, arg2);
        Boolean error_was_handled = FALSE;
        if (mediaPlayer->mOnErrorListener != NULL) {
            mediaPlayer->mOnErrorListener->OnError(
                mediaPlayer, arg1, arg2, &error_was_handled);
        }
        if (mediaPlayer->mOnCompletionListener != NULL && ! error_was_handled) {
            mediaPlayer->mOnCompletionListener->OnCompletion(mediaPlayer);
        }
        mediaPlayer->StayAwake(FALSE);
        return NOERROR;
    }

    case CMediaPlayer::MEDIA_INFO:
        if (arg1 != CMediaPlayer::MEDIA_INFO_VIDEO_TRACK_LAGGING) {
            Logger::I(CMediaPlayer::TAG, "Info (%d, %d)", arg1, arg2);
        }
        if (mediaPlayer->mOnInfoListener != NULL) {
            Boolean result;
            mediaPlayer->mOnInfoListener->OnInfo(mediaPlayer, arg1, arg2, &result);
        }
        // No real default action so far.
        return NOERROR;

    case CMediaPlayer::MEDIA_TIMED_TEXT:
        if (mediaPlayer->mOnTimedTextListener == NULL)
            return NOERROR;
        if (obj == NULL) {
            mediaPlayer->mOnTimedTextListener->OnTimedText(mediaPlayer, NULL);
        }
        else {
            IParcel* parcel = IParcel::Probe(obj.Get());
            if (parcel != NULL) {
                AutoPtr<ITimedText> text;
                CTimedText::New(parcel, (ITimedText**)&text);
                // parcel->Recycle();

                mediaPlayer->mOnTimedTextListener->OnTimedText(mediaPlayer, text);
            }
        }
        return NOERROR;

    case CMediaPlayer::MEDIA_NOP: // interface test message - ignore
        break;

    /*Start by Bevis. Detect http data source from other application.*/
    case CMediaPlayer::MEDIA_SOURCE_DETECTED:
        if (mediaPlayer->mDlnaSourceDetector != NULL && obj != NULL) {
            IParcel* parcel = IParcel::Probe(obj.Get());
            if (parcel != NULL) {
                String url;
                parcel->ReadString(&url);
                Logger::D(CMediaPlayer::TAG, "######MEDIA_SOURCE_DETECTED! url = %s", url.string());
                mediaPlayer->mDlnaSourceDetector->OnSourceDetected(url);
            }
        }
        return NOERROR;

    /*End by Bevis. Detect http data source from other application. */
    default:
        Logger::E(CMediaPlayer::TAG, "Unknown message type %d", what);
        return NOERROR;
    }
    return NOERROR;
}

//===================================================================
//                  CMediaPlayer
//===================================================================

CMediaPlayer::CMediaPlayer()
    : mNativeContext(0)
    , mNativeSurfaceTexture(0)
    , mListenerContext(0)
    , mScreenOnWhilePlaying(FALSE)
    , mStayAwake(FALSE)
{
}

CMediaPlayer::~CMediaPlayer()
{
    NativeFinalize();
}

ECode CMediaPlayer::constructor()
{
    AutoPtr<ILooper> looper = Looper::MyLooper();
    if (looper != NULL) {
        AutoPtr<IWeakReference> wr;
        GetWeakReference((IWeakReference**)&wr);
        mEventHandler = new EventHandler(wr, looper);
    }
    else if ((looper = Looper::GetMainLooper()) != NULL) {
        AutoPtr<IWeakReference> wr;
        GetWeakReference((IWeakReference**)&wr);
        mEventHandler = new EventHandler(wr, looper);
    }
    else {
        mEventHandler = NULL;
    }

    /* Native setup requires a weak reference to our object.
    * It's easier to create it here than in C++.
    */
    ECode ec = NativeSetup();
    ASSERT_SUCCEEDED(ec);
    return ec;
}

ECode CMediaPlayer::DecVideoSurfaceRef()
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL) {
        return NOERROR;
    }

    android::sp<android::ISurfaceTexture> old_st = getVideoSurfaceTexture(this);
    if (old_st != NULL) {
        old_st->decStrong(this);
    }
    return NOERROR;
}

ECode CMediaPlayer::SetVideoSurface(
    /* [in] */ ISurface* insurface,
    /* [in] */ Boolean mediaPlayerMustBeAlive)
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if ( mp == NULL ) {
        if (mediaPlayerMustBeAlive) {
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        return NOERROR;
    }

    DecVideoSurfaceRef();

    android::sp<android::ISurfaceTexture> new_st;
    if (insurface) {
        android::sp<android::Surface> surface(android_view_Surface_getSurface(insurface));
        if (surface != NULL) {
            new_st = surface->getSurfaceTexture();
            if (new_st == NULL) {
                Logger::E(TAG, "The surface does not have a binding SurfaceTexture!");
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
            new_st->incStrong(this);
        }
        else {
            Logger::E(TAG, "The surface has been released!");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }
    // env->SetIntField(thiz, fields.surface_texture, (int)new_st.get());
    mNativeSurfaceTexture = reinterpret_cast<Handle32>(new_st.get());
    // This will fail if the media player has not been initialized yet. This
    // can be the case if setDisplay() on MediaPlayer.java has been called
    // before setDataSource(). The redundant call to setVideoSurfaceTexture()
    // in prepare/prepareAsync covers for this case.
    mp->setVideoSurfaceTexture(new_st);
    return NOERROR;
}

ECode CMediaPlayer::NativeSetVideoSurface(
    /* [in] */ ISurface* surface )
{
    return SetVideoSurface(surface, TRUE /* mediaPlayerMustBeAlive */);
}

ECode CMediaPlayer::NewRequest(
    /* [out] */ IParcel ** parcel)
{
    VALIDATE_NOT_NULL(parcel);

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    assert(0 && "TODO");
    // Parcel parcel = Parcel.obtain();
    // parcel.writeInterfaceToken(IMEDIA_PLAYER);
    // return parcel;
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::Invoke(
    /* [in] */ IParcel* request,
    /* [in] */ IParcel* reply)
{
    Int32 retcode;
    FAIL_RETURN(NativeInvoke(request, reply, &retcode));
    reply->SetDataPosition(0);

    if (retcode != 0) {
        Logger::E(TAG, "failure code: %d", retcode);
        return E_RUNTIME_EXCEPTION;
    }

    return NOERROR;
}

ECode CMediaPlayer::SetDisplay(
    /* [in] */ ISurfaceHolder* sh)
{
    mSurfaceHolder = sh;
    AutoPtr<ISurface> surface;
    if (sh != NULL) {
        sh->GetSurface((ISurface**)&surface);
    }

    FAIL_RETURN(NativeSetVideoSurface(surface));
    return UpdateSurfaceScreenOn();
}

ECode CMediaPlayer::SetSurface(
    /* [in] */ ISurface* surface)
{
    if (mScreenOnWhilePlaying && surface != NULL) {
        Logger::W(TAG, "setScreenOnWhilePlaying(true) is ineffective for Surface");
    }

    mSurfaceHolder = NULL;
    FAIL_RETURN(NativeSetVideoSurface(surface));
    return UpdateSurfaceScreenOn();
}

ECode CMediaPlayer::SetVideoScalingMode(
    /* [in] */ Int32 mode)
{
    if (!IsVideoScalingModeSupported(mode)) {
        Logger::E(TAG, "Scaling mode %d  is not supported.", mode);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    assert(0 && "TODO");
    // AutoPtr<IParcel> request = Parcel->Obtain();
    // AutoPtr<IParcel> reply = Parcel->Obtain();
    // //try {
    //     request->WriteInterfaceToken(IMEDIA_PLAYER);
    //     request->WriteInt(INVOKE_ID_SET_VIDEO_SCALE_MODE);
    //     request->WriteInt(mode);
    //     Invoke(request, reply);
    // //} finally {
    //     request->Recycle();
    //     reply->Recycle();
    // //}

    return NOERROR;
}

ECode CMediaPlayer::Create(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri,
    /* [out] */ IMediaPlayer** player)
{
    return Create(context, uri, NULL, player);
}

ECode CMediaPlayer::Create(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri,
    /* [in] */ ISurfaceHolder* holder,
    /* [out] */ IMediaPlayer** player)
{
    VALIDATE_NOT_NULL(player);
    *player = NULL;

    //try {
    AutoPtr<IMediaPlayer> mp;
    ECode ec = CMediaPlayer::New((IMediaPlayer**)&mp);
    FAIL_GOTO(ec, _EXIT_);

    ec = mp->SetDataSource(context, uri);
    FAIL_GOTO(ec, _EXIT_);
    if (holder != NULL) {
        ec = mp->SetDisplay(holder);
        FAIL_GOTO(ec, _EXIT_);
    }

    ec = mp->Prepare();
    FAIL_GOTO(ec, _EXIT_);

    *player = mp;
    INTERFACE_ADDREF(*player);
    return NOERROR;

_EXIT_:
    Logger::D(TAG, "create MediaPlayer failed: %08x", ec);
    return ec;
}

ECode CMediaPlayer::Create(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resid,
    /* [out] */ IMediaPlayer** player)
{
    VALIDATE_NOT_NULL(player);
    *player = NULL;
    VALIDATE_NOT_NULL(context);

    //try {
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IAssetFileDescriptor> afd;
    res->OpenRawResourceFd(resid, (IAssetFileDescriptor**)&afd);
    if (afd == NULL) {
        return NOERROR;
    }

    AutoPtr<IFileDescriptor> fd;
    Int64 offset, length;

    AutoPtr<IMediaPlayer> mp;
    ECode ec = CMediaPlayer::New((IMediaPlayer**)&mp);

    afd->GetFileDescriptor((IFileDescriptor**)&fd);
    afd->GetStartOffset(&offset);
    afd->GetLength(&length);

    ec = mp->SetDataSourceEx5(fd, offset, length);
    FAIL_GOTO(ec, _EXIT_);

    if (afd != NULL) {
        afd->Close();
        afd = NULL;
    }

    ec = mp->Prepare();
    FAIL_GOTO(ec, _EXIT_);

    *player = mp ;
    INTERFACE_ADDREF(*player);
    return NOERROR;

_EXIT_:
    if (afd != NULL) {
        afd->Close();
        afd = NULL;
    }

    Logger::D(TAG, "create MediaPlayer failed: %08x", ec);
    return ec;
}

ECode CMediaPlayer::SetDataSource(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri)
{
    if (LOGV) Logger::V(TAG, "SetDataSource uri");
    return SetDataSourceEx(context, uri, NULL);
}

ECode CMediaPlayer::SetDataSourceEx(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri,
    /* [in] */ IObjectStringMap* headers)
{
    if (LOGV) Logger::V(TAG, "SetDataSourceEx uri headers");

    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(uri);

    String scheme;
    uri->GetScheme(&scheme);
    if(scheme.IsNull() || scheme.Equals("file")) {
        String path;
        uri->GetPath(&path);
        return SetDataSourceEx3(path, headers);
    }

    AutoPtr<IAssetFileDescriptor> afd;
    // try {
    AutoPtr<IContentResolver> resolver;
    context->GetContentResolver((IContentResolver**)&resolver);
    resolver->OpenAssetFileDescriptor(uri, String("r"), (IAssetFileDescriptor**)&afd);
    if (afd == NULL) {
       return NOERROR;
    }

    // Note: using getDeclaredLength so that our behavior is the same
    // as previous versions when the content provider is returning
    // a full file.
    ECode ec = NOERROR;
    String uriStr;
    Int64 length, offset;
    AutoPtr<IFileDescriptor> fd;
    ec = afd->GetStartOffset(&offset);
    FAIL_GOTO(ec, _EXIT_)

    afd->GetDeclaredLength(&length);
    FAIL_GOTO(ec, _EXIT_)

    afd->GetFileDescriptor((IFileDescriptor**)&fd);
    FAIL_GOTO(ec, _EXIT_)

    if (length < 0) {
       ec = SetDataSourceEx4(fd);
    }
    else {
       ec = SetDataSourceEx5(fd, offset, length);
    }

_EXIT_:
    if (afd) {
        afd->Close();
        afd = NULL;
    }

    if (ec != E_SECURITY_EXCEPTION && ec != E_IO_EXCEPTION) {
        return ec;
    }

    Logger::D(TAG, "Couldn't open file on client side, trying server side");
    uri->ToString(&uriStr);
    return SetDataSourceEx3(uriStr, headers);
}

ECode CMediaPlayer::SetDataSourceEx2(
    /* [in] */ const String& path)
{
    ECode ec = SetDataSource(path, NULL, NULL);
    if (LOGV) Logger::V(TAG, "SetDataSourceEx2 path: %s", path.string());
    return ec;
}

ECode CMediaPlayer::SetDataSourceEx3(
    /* [in] */ const String& path,
    /* [in] */ IObjectStringMap* headers)
{
    AutoPtr<ArrayOf<String> > keys;
    AutoPtr<ArrayOf<String> > values;

    if (headers != NULL) {
        headers->GetKeys((ArrayOf<String>**)&keys);
        if (keys != NULL)
        {
            AutoPtr<IInterface> tmpObj;
            ICharSequence* seq;
            String value, nullStr;
            for (Int32 i = 0; i < keys->GetLength(); ++i) {
                tmpObj = NULL;
                headers->Get((*keys)[i], (IInterface**)&tmpObj);
                seq = ICharSequence::Probe(tmpObj.Get());
                if (seq != NULL) {
                    seq->ToString(&value);
                    values->Set(i, value);
                }
                else {
                    values->Set(i, nullStr);
                }
            }
        }
    }

    return SetDataSource(path, keys, values);
}

ECode CMediaPlayer::SetDataSource(
    /* [in] */ const String& path,
    /* [in] */ ArrayOf<String>* keys,
    /* [in] */ ArrayOf<String>* values)
{
    AutoPtr<IFile> file;
    CFile::New(path, (IFile**)&file);

    if (LOGV) Logger::V(TAG, "SetDataSource keys/values withpath: %s", path.string());
    // Boolean bol;
    if (FALSE) {
    // if (file != NULL && (file->Exists(&bol), bol)) {
        AutoPtr<IFileInputStream> is;
        CFileInputStream::New(file, (IFileInputStream**)&is);

        AutoPtr<IFileDescriptor> fd;
        is->GetFD((IFileDescriptor**)&fd);
        ECode ec = SetDataSourceEx4(fd);
        is->Close();
        return ec;
    }
    else {
        return NativeSetDataSource(path, keys, values);
    }
}

ECode CMediaPlayer::NativeSetDataSource(
    /* [in] */ const String& path,
    /* [in] */ ArrayOf<String>* keys,
    /* [in] */ ArrayOf<String>* values)
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (path.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (LOGV) Logger::V(TAG, "NativeSetDataSource: path %s", path.string());

    android::String8 pathStr(path.string());

    // headers is a Map<String, String>.
    // We build a similar KeyedVector out of it.
    android::KeyedVector<android::String8, android::String8> headersVector;
    FAIL_RETURN(Media_Utils::ConvertKeyValueArraysToKeyedVector(
        keys, values, &headersVector));

    android::status_t opStatus = mp->setDataSource(
        pathStr, headersVector.size() > 0? &headersVector : NULL);
    return process_media_player_call(this, opStatus, E_IO_EXCEPTION, "NativeSetDataSource failed.");
}

ECode CMediaPlayer::SetDataSourceEx4(
    /* [in] */ IFileDescriptor* fd)
{
    // intentionally less than LONG_MAX
    return SetDataSourceEx5(fd, 0, 0x7ffffffffffffffll);
}

ECode CMediaPlayer::SetDataSourceEx5(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 length)
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (fd == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 num;
    fd->GetDescriptor(&num);

    if (LOGV) Logger::V(TAG, "SetDataSourceFD %d, offset %d, length %d", num, offset, length);

    return process_media_player_call(this,
        mp->setDataSource(num, offset, length),
        E_IO_EXCEPTION, "setDataSourceFD failed.");
}

ECode CMediaPlayer::Prepare()
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    android::sp<android::ISurfaceTexture> st = getVideoSurfaceTexture(this);
    mp->setVideoSurfaceTexture(st);

    return process_media_player_call(this, mp->prepare(),
        E_IO_EXCEPTION, "Prepare failed.");
}

ECode CMediaPlayer::PrepareAsync()
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    android::sp<android::ISurfaceTexture> st = getVideoSurfaceTexture(this);
    mp->setVideoSurfaceTexture(st);

    return process_media_player_call(this, mp->prepareAsync(),
        E_IO_EXCEPTION, "Prepare Async failed.");
}

ECode CMediaPlayer::Start()
{
    StayAwake(TRUE);
    return NativeStart();
}

ECode CMediaPlayer::NativeStart()
{
    if (LOGV) Logger::V(TAG, " > start");
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return process_media_player_call(this, mp->start(), NOERROR, "NativeStart");
}

ECode CMediaPlayer::Stop()
{
    StayAwake(FALSE);
    return NativeStop();
}

ECode CMediaPlayer::NativeStop()
{
    if (LOGV) Logger::V(TAG, " > stop");
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return process_media_player_call(this, mp->stop(), NOERROR, "NativeStop");
}

ECode CMediaPlayer::Pause()
{
    StayAwake(FALSE);
    return NativePause();
}

ECode CMediaPlayer::NativePause()
{
    if (LOGV) Logger::V(TAG, " > pause");
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return process_media_player_call(this, mp->pause(), NOERROR, "NativePause");
}

ECode CMediaPlayer::SetWakeMode(
    /* [in] */ IContext* context,
    /* [in] */ Int32 mode)
{
    Boolean washeld = FALSE;
    if (mWakeLock != NULL) {
        Boolean bval;
        mWakeLock->IsHeld(&bval);
        if (bval) {
            washeld = TRUE;
            mWakeLock->ReleaseLock();
        }
        mWakeLock = NULL;
    }

    AutoPtr<IInterface> tmpObj;
    context->GetSystemService(IContext::POWER_SERVICE, (IInterface**)&tmpObj);
    AutoPtr<IPowerManager> pm = IPowerManager::Probe(tmpObj.Get());
    if (pm != NULL) {
        pm->NewWakeLock(mode | IPowerManager::ON_AFTER_RELEASE, String("CMediaPlayer"),
            (IPowerManagerWakeLock**)&mWakeLock);
        mWakeLock->SetReferenceCounted(FALSE);
        if (washeld) {
            mWakeLock->AcquireLock();
        }
    }

    return NOERROR;
}

ECode CMediaPlayer::SetScreenOnWhilePlaying(
    /* [in] */ Boolean screenOn)
{
    if (mScreenOnWhilePlaying != screenOn) {
        if (screenOn && mSurfaceHolder == NULL) {
            Logger::W(TAG, "setScreenOnWhilePlaying(true) is ineffective without a SurfaceHolder");
        }
        mScreenOnWhilePlaying = screenOn;
        return UpdateSurfaceScreenOn();
    }
    return NOERROR;
}

ECode CMediaPlayer::StayAwake(
    /* [in] */ Boolean awake)
{
    if (mWakeLock != NULL) {
        Boolean bval;
        mWakeLock->IsHeld(&bval);
        if (awake && !bval) {
            mWakeLock->AcquireLock();
        }
        else if (!awake && bval) {
            mWakeLock->ReleaseLock();
        }
    }

    mStayAwake = awake;
    return UpdateSurfaceScreenOn();
}

ECode CMediaPlayer::UpdateSurfaceScreenOn()
{
    if (mSurfaceHolder != NULL) {
        return mSurfaceHolder->SetKeepScreenOn(mScreenOnWhilePlaying && mStayAwake);
    }
    return NOERROR;
}

/**
 * Returns the width of the video.
 *
 * @return the width of the video, or 0 if there is no video,
 * no display surface was set, or the width has not been determined
 * yet. The OnVideoSizeChangedListener can be registered via
 * {@link #setOnVideoSizeChangedListener(OnVideoSizeChangedListener)}
 * to provide a notification when the width is available.
 */
ECode CMediaPlayer::GetVideoWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    int w;
    if (0 != mp->getVideoWidth(&w)) {
        Logger::E(TAG, "getVideoWidth failed");
        w = 0;
    }

    if (LOGV) Logger::V(TAG, "getVideoWidth: %d", w);
    *width = w;
    return NOERROR;
}

ECode CMediaPlayer::GetVideoHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    int h;
    if (0 != mp->getVideoHeight(&h)) {
        Logger::E(TAG, "getVideoHeight failed");
        h = 0;
    }

    if (LOGV) Logger::V(TAG, "getVideoHeight: %d", h);
    *height = h;
    return NOERROR;
}

ECode CMediaPlayer::IsPlaying(
    /* [out] */ Boolean* isPlaying)
{
    VALIDATE_NOT_NULL(isPlaying);
    *isPlaying = FALSE;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *isPlaying = mp->isPlaying();

    // if (LOGV) Logger::V(TAG, "isPlaying: %d", *isPlaying);
    return NOERROR;
}

ECode CMediaPlayer::SeekTo(
    /* [in] */ Int32 msec)
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (LOGV) Logger::V(TAG, "seekTo: %d(msec)", msec);
    return process_media_player_call(this, mp->seekTo(msec), NOERROR, "SeekTo");
}

ECode CMediaPlayer::GetCurrentPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        *position = 0;
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    int msec;
    ECode ec = process_media_player_call(this, mp->getCurrentPosition(&msec), NOERROR, "GetCurrentPosition");
    if (LOGV) Logger::V(TAG, "getCurrentPosition: %d (msec) ec: %08x", msec, ec);
    *position = msec;
    return ec;
}

ECode CMediaPlayer::GetDuration(
    /* [out] */ Int32* duration)
{
    VALIDATE_NOT_NULL(duration);
    *duration = 0;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    int msec;
    ECode ec = process_media_player_call(this, mp->getDuration(&msec), NOERROR);
    if (LOGV) Logger::V(TAG, "getDuration: %d (msec), ec=%08x", msec, ec);
    *duration = msec;
    return ec;
}

ECode CMediaPlayer::GetMetadata(
    /* [in] */ Boolean update_only,
    /* [in] */ Boolean apply_filter,
    /* [out] */ IMetadata** metadata)
{
    VALIDATE_NOT_NULL(metadata);
    *metadata = NULL;

    AutoPtr<IParcel> reply;
    CCallbackParcel::New((IParcel**)&reply);

    AutoPtr<IMetadata> data;
    FAIL_RETURN(CMetadata::New((IMetadata**)&data));
    Boolean succeeded;
    FAIL_RETURN(NativeGetMetadata(update_only, apply_filter, reply, &succeeded));
    if (!succeeded) {
        return NOERROR;
    }

    // Metadata takes over the parcel, don't recycle it unless
    // there is an error.
    Boolean res;
    FAIL_RETURN(data->Parse(reply, &res));
    if (!res) {
        // reply->Recycle();
        return NOERROR;
    }
    *metadata = data;
    INTERFACE_ADDREF(*metadata);
    return NOERROR;
}

ECode CMediaPlayer::SetMetadataFilter(
    /* [in] */ IObjectContainer* allow,
    /* [in] */ IObjectContainer* block)
{
    assert(0 && "TODO");
    // Do our serialization manually instead of calling
    // Parcel.writeArray since the sets are made of the same type
    // we avoid paying the price of calling writeValue (used by
    // writeArray) which burns an extra Int32 per element to encode
    // the type.
    AutoPtr<IParcel> request;
    FAIL_RETURN(NewRequest((IParcel**)&request));

    // The parcel starts already with an interface token. There
    // are 2 filters. Each one starts with a 4bytes number to
    // store the len followed by a number of Int32 (4 bytes as well)
    // representing the metadata type.
    Int32 capacity;
    //request->DataSize(&capacity);
    //capacity += 4 * (1 + allow->Size() + 1 + block->Size());

    Int32 cap;
    //request->DataCapacity(&cap);
    if (cap < capacity) {
        //request->SetDataCapacity(capacity);
    }

    //request->WriteInt32(allow->size());
    /*for(Integer t: allow) {
        request->WriteInt32(t);
    }*/
    //request->WriteInt32(block->Size());
    /*for(Integer t: block) {
        request->WriteInt32(t);
    }*/
    return NativeSetMetadataFilter(request);
}

ECode CMediaPlayer::SetNextMediaPlayer(
    /* [in] */ IMediaPlayer* next)
{
    if (LOGV) Logger::V(TAG, "setNextMediaPlayer");
    android::sp<android::MediaPlayer> thisplayer = getMediaPlayer(this);
    if (thisplayer == NULL) {
        Logger::E(TAG, "This player not initialized");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    android::sp<android::MediaPlayer> nextplayer = NULL;
    if (next != NULL) {
        nextplayer = getMediaPlayer((CMediaPlayer*)next);
    }

    if (nextplayer == NULL && next != NULL) {
        Logger::E(TAG, "Next player not initialized");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (nextplayer == thisplayer) {
        Logger::E(TAG, "Next player can't be self");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // tie the two players together
    return process_media_player_call(this, thisplayer->setNextMediaPlayer(nextplayer),
        E_ILLEGAL_ARGUMENT_EXCEPTION, "setNextMediaPlayer failed.");
}

ECode CMediaPlayer::ReleaseResources()
{
    StayAwake(FALSE);
    UpdateSurfaceScreenOn();
    mOnPreparedListener = NULL;
    mOnBufferingUpdateListener = NULL;
    mOnCompletionListener = NULL;
    mOnSeekCompleteListener = NULL;
    mOnErrorListener = NULL;
    mOnInfoListener = NULL;
    mOnVideoSizeChangedListener = NULL;
    mOnTimedTextListener = NULL;
    NativeRelease();

    return NOERROR;
}

ECode CMediaPlayer::NativeRelease()
{
    if (LOGV) Logger::V(TAG, "release");
    DecVideoSurfaceRef();
    android::sp<android::MediaPlayer> mp = setMediaPlayer(this, NULL);
    if (mp != NULL) {
        // this prevents native callbacks after the object is released
        mp->setListener(0);
        mp->disconnect();
    }
    return NOERROR;
}

ECode CMediaPlayer::Reset()
{
    StayAwake(FALSE);
    FAIL_RETURN(NativeReset());

    // make sure none of the listeners get called anymore
    return mEventHandler->RemoveCallbacksAndMessages(NULL);
}

ECode CMediaPlayer::NativeReset()
{
    if (LOGV) Logger::V(TAG, "reset");
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return process_media_player_call(this, mp->reset(), NOERROR, "NativeReset");
}

ECode CMediaPlayer::SetAudioStreamType(
    /* [in] */ Int32 streamtype)
{
    if (LOGV) Logger::V(TAG, "setAudioStreamType: %d", streamtype);
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return process_media_player_call(this,
        mp->setAudioStreamType((audio_stream_type_t)streamtype), NOERROR, "SetAudioStreamType");
}

ECode CMediaPlayer::SetLooping(
    /* [in] */ Boolean looping)
{
    if (LOGV) Logger::V(TAG, "setLooping: %d", looping);
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return process_media_player_call(this, mp->setLooping(looping), NOERROR, "SetLooping");
}

ECode CMediaPlayer::IsLooping(
    /* [out] */ Boolean* isLooping)
{
    VALIDATE_NOT_NULL(isLooping);
    *isLooping = FALSE;

    if (LOGV) Logger::V(TAG, "isLooping");
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    *isLooping = mp->isLooping();
    return NOERROR;
}

ECode CMediaPlayer::SetVolume(
    /* [in] */ Float leftVolume,
    /* [in] */ Float rightVolume)
{
    if (LOGV) Logger::V(TAG, "setVolume: left %f  right %f", leftVolume, rightVolume);
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return process_media_player_call(this, mp->setVolume(leftVolume, rightVolume),
        NOERROR, NULL);
}

ECode CMediaPlayer::GetFrameAt(
    /* [in] */ Int32 msec,
    /* [out] */ IBitmap** frame)
{
    VALIDATE_NOT_NULL(frame);
    *frame = NULL;
    return NOERROR;
}

ECode CMediaPlayer::SetAudioSessionId(
    /* [in] */ Int32 sessionId)
{
    if (LOGV) Logger::V(TAG, "set_session_id(): %d", sessionId);
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return process_media_player_call(this, mp->setAudioSessionId(sessionId), NOERROR);
}

ECode CMediaPlayer::GetAudioSessionId(
    /* [out] */ Int32* sessionID)
{
    VALIDATE_NOT_NULL(sessionID);
    *sessionID = 0;

    if (LOGV) Logger::V(TAG, "get_session_id()");
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *sessionID = mp->getAudioSessionId();
    return NOERROR;
}

ECode CMediaPlayer::AttachAuxEffect(
    /* [in] */ Int32 effectId)
{
    if (LOGV) Logger::V(TAG, "attachAuxEffect(): %d", effectId);
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return process_media_player_call(this, mp->attachAuxEffect(effectId), NOERROR);
}

ECode CMediaPlayer::SetParameter(
    /* [in] */ Int32 key,
    /* [in] */ IParcel* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    if (LOGV) Logger::V(TAG, "setParameter: key %d", key);
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    android::Parcel* _request;
    value->GetElementPayload((Handle32*)&_request);
    android::status_t err = mp->setParameter(key, *_request);
    if (err != android::OK) {
        return E_FAIL;
    }

    *result = TRUE;
    return NOERROR;
}

ECode CMediaPlayer::SetParameterEx(
    /* [in] */ Int32 key,
    /* [in] */ const String& value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(value);

    ECode ec = SetParameter(key, params, result);
    //p->Recycle();
    return ec;
}

ECode CMediaPlayer::SetParameterEx2(
    /* [in] */ Int32 key,
    /* [in] */ Int32 value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IParcel> p ;
    CCallbackParcel::New((IParcel**)&p);
    p->WriteInt32(value);

    ECode ec = SetParameter(key, p, result);
    //p->Recycle();
    return ec;
}

ECode CMediaPlayer::GetParameter(
    /* [in] */ Int32 key,
    /* [in] */ IParcel* reply)
{
    if (LOGV) Logger::V(TAG, "getParameter: key %d", key);
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    android::Parcel* _reply;
    reply->GetElementPayload((Handle32*)&_reply);
    return process_media_player_call(this, mp->getParameter(key, _reply), NOERROR);
}

ECode CMediaPlayer::GetParcelParameter(
    /* [in] */ Int32 key,
    /* [out] */ IParcel** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    AutoPtr<IParcel> p;
    CCallbackParcel::New((IParcel**)&p);
    FAIL_RETURN(GetParameter(key, p));
    *result = p;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CMediaPlayer::GetStringParameter(
    /* [in] */ Int32 key,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = String(NULL);

    AutoPtr<IParcel> p;
    CCallbackParcel::New((IParcel**)&p);
    FAIL_RETURN(GetParameter(key, p));
    return p->ReadString(result);
}

ECode CMediaPlayer::GetInt32Parameter(
    /* [in] */ Int32 key,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    AutoPtr<IParcel> p;
    CCallbackParcel::New((IParcel**)&p);
    FAIL_RETURN(GetParameter(key, p));
    return p->ReadInt32(result);
}

ECode CMediaPlayer::GetSubInfo(
    /* [out, callee] */ ArrayOf<IMediaPlayerSubInfoN*>** subInfoN)
{
    VALIDATE_NOT_NULL(subInfoN);
    assert(0 && "TODO");
    // AutoPtr<IParcel> params;
    // CCallbackParcel::New((IParcel**)&params);
//     Parcel p = Parcel.obtain();
//     getParameter(2, p);
//     int num = p.readInt();
//     int i;
//     Log.e("Mediaplayer.java","getSubinfo num="+num);
//     SubInfon_[] sub=null;
//     if( num > 0)
//     {
//         sub= new SubInfon_[num];
//         Log.e("Mediaplayer.java"," 1num="+num);
//         for( i=0; i<num; i++)
//         { Log.e("Mediaplayer.java"," 2");
//             sub[i]=new SubInfon_();
//             sub[i].format=p.readString();
//             Log.e("Mediaplayer.java","1 format="+sub[i].format);
//             //   sub[i].charset=p.readString();
//             //Log.e("Mediaplayer.java","1 charset="+sub[i].charset);
//             sub[i].lang=p.readString();
//             Log.e("Mediaplayer.java"," lang="+sub[i].lang);
//             sub[i].inline=p.readInt();
//             Log.e("Mediaplayer.java"," inline="+sub[i].lang);
//             sub[i].num=num;

//         }
//         Log.e("Mediaplayer.java","getSubinfo 6");
//     }
//     p.recycle();
//     Log.e("Mediaplayer.java","getSubinfo7");
//     return sub;
    return NOERROR;
}

ECode CMediaPlayer::GetAudioInfo(
    /* [out, callee] */ ArrayOf<IMediaPlayerAudioInfoN*>** audioInfoN)
{
    VALIDATE_NOT_NULL(audioInfoN);
    assert(0 && "TODO");

    // AutoPtr<IParcel> params;
    // CCallbackParcel::New((IParcel**)&params);
//     Parcel p = Parcel.obtain();
//     getParameter(3, p);
//     int n = p.readInt();
//     int i;
//     Log.e("Mediaplayer.java","getAudioinfo 0");
//     AudioInfon_[] ainfo=null;
//     if( n > 0)
//     {
//         ainfo= new AudioInfon_[n];
//         Log.e("Mediaplayer.java"," 1num="+n);
//         for( i=0; i<n; i++)
//         {

//             Log.e("Mediaplayer.java"," 2");
//             ainfo[i]=new AudioInfon_();
//             ainfo[i].lang=p.readString();

//             Log.e("Mediaplayer.java","1 lang="+ainfo[i].lang);
//             ainfo[i].format=p.readString();
//             Log.e("Mediaplayer.java","1 format="+ainfo[i].format);

//             ainfo[i].channle_l=p.readInt();
//             Log.e("Mediaplayer.java","1 channle_l="+ainfo[i].channle_l);
//             ainfo[i].audiochannel=p.readInt();
//             Log.e("Mediaplayer.java","1 audiochannel="+ainfo[i].audiochannel);
//             ainfo[i].sample=p.readInt();
//             Log.e("Mediaplayer.java","1 sample="+ainfo[i].sample);


//             ainfo[i].num=n;
//         }
//         Log.e("Mediaplayer.java","getAudioinfo 6");
//     }
//     p.recycle();
//     Log.e("Mediaplayer.java","getAudioinfo7");
//     return ainfo;
    return NOERROR;
}

ECode CMediaPlayer::SetAuxEffectSendLevel(
    /* [in] */ Float level)
{
    if (LOGV) Logger::V(TAG, "setAuxEffectSendLevel: level %f", level);
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return process_media_player_call(this, mp->setAuxEffectSendLevel(level), NOERROR);
}

ECode CMediaPlayer::NativeInvoke(
    /* [in] */ IParcel* request,
    /* [in] */ IParcel* reply,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    android::sp<android::MediaPlayer> media_player = getMediaPlayer(this);
    if (media_player == NULL ) {
        *result = android::UNKNOWN_ERROR;
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    android::Parcel* _request;
    request->GetElementPayload((Handle32*)&_request);
    android::Parcel* _reply;
    reply->GetElementPayload((Handle32*)&_reply);

    // Don't use process_media_player_call which use the async loop to
    // report errors, instead returns the status.
    *result = media_player->invoke(*_request, _reply);
    return NOERROR;
}

ECode CMediaPlayer::NativeGetMetadata(
    /* [in] */ Boolean update_only,
    /* [in] */ Boolean apply_filter,
    /* [in] */ IParcel* reply,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    *succeeded = FALSE;

    android::sp<android::MediaPlayer> media_player = getMediaPlayer(this);
    if (media_player == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    android::Parcel* metadata;
    reply->GetElementPayload((Handle32*)&metadata);
    if (metadata == NULL ) {
        Logger::E(TAG, "Reply parcel is NULL");
        return E_FAIL;
    }

    metadata->freeData();
    // On return metadata is positioned at the beginning of the
    // metadata. Note however that the parcel actually starts with the
    // return code so you should not rewind the parcel using
    // setDataPosition(0).
    *succeeded = media_player->getMetadata(update_only, apply_filter, metadata) == 0;
    return NOERROR;
}

ECode CMediaPlayer::NativeSetMetadataFilter(
    /* [in] */ IParcel* request)
{
    android::sp<android::MediaPlayer> media_player = getMediaPlayer(this);
    if (media_player == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    android::Parcel* filter;
    request->GetElementPayload((Handle32*)&filter);

    if (filter == NULL ) {
        Logger::E(TAG, "Filter is NULL.");
        return E_FAIL;
    }

    android::status_t err = media_player->setMetadataFilter(*filter);
    return (err == android::OK) ? NOERROR : E_FAIL;
}

ECode CMediaPlayer::NativeSetup()
{
    if (LOGV) Logger::V(TAG, "NativeSetup");
    android::sp<android::MediaPlayer> mp = new android::MediaPlayer();
    if (mp == NULL) {
        Logger::E(TAG, "Out of memory");
        return E_RUNTIME_EXCEPTION;
    }

    // create new listener and give it to MediaPlayer
    AutoPtr<IWeakReference> wr;
    GetWeakReference((IWeakReference**)&wr);
    android::sp<JNIMediaPlayerListener> listener = new JNIMediaPlayerListener(wr);
    mp->setListener(listener);

    // Stow our new C++ MediaPlayer in an opaque field in the Java object.
    setMediaPlayer(this, mp);
    return NOERROR;
}

ECode CMediaPlayer::GetTrackInfo(
    /* [out, callee] */ ArrayOf<IMediaPlayerTrackInfo>** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;
    assert(0 && "TODO");
    /*
    AutoPtr<IParcel> request;
    CCallbackParcel::New((IParcel**)&request);
    AutoPtr<IParcel> reply;
    CCallbackParcel::New((IParcel**)&reply);
    //try {
        request->WriteInterfaceToken(IMEDIA_PLAYER);
        request->WriteInt32(INVOKE_ID_GET_TRACK_INFO);
        Invoke(request, reply);
        AutoPtr<ArrayOf<IMediaPlayerTrackInfo> > trackInfo = reply->CreateTypedArray(TrackInfo_CREATOR);
        *result = trackInfo;
        INTERFACE_ADDREF(*result);
    //} finally {
    //    request->Recycle();
    //    reply->Recycle();
    //}
    */
    return NOERROR;
}

Boolean CMediaPlayer::AvailableMimeTypeForExternalSource(
    /* [in] */ const String& mimeType)
{
    return (mimeType == MEDIA_MIMETYPE_TEXT_SUBRIP);
}

ECode CMediaPlayer::AddTimedTextSource(
    /* [in] */ const String& path,
    /* [in] */ const String& mimeType)
{
    if (!AvailableMimeTypeForExternalSource(mimeType)) {
        Logger::E(TAG, "Illegal mimeType for timed text source: %s", mimeType.string());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IFile> file;
    CFile::New(path, (IFile**)&file);
    Boolean exists;
    if (file->Exists(&exists), exists) {
        AutoPtr<IFileInputStream> is;
        CFileInputStream::New(file, (IFileInputStream**)&is);

        AutoPtr<IFileDescriptor> fd;
        is->GetFD((IFileDescriptor**)&fd);
        ECode ec = AddTimedTextSourceEx2(fd, mimeType);
        is->Close();
        return ec;
    }
    else {
        // We do not support the case where the path is not a file.
        return E_IO_EXCEPTION;
    }
}

ECode CMediaPlayer::AddTimedTextSourceEx(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri,
    /* [in] */ const String& mimeType)
{
    VALIDATE_NOT_NULL(uri);

    String scheme;
    uri->GetScheme(&scheme);
    if(scheme.IsNull() || scheme.Equals("file")) {
        String path;
        uri->GetPath(&path);
        return AddTimedTextSource(path, mimeType);
    }

    AutoPtr<IContentResolver> resolver;
    context->GetContentResolver((IContentResolver**)&resolver);
    AutoPtr<IAssetFileDescriptor> fd;
    resolver->OpenAssetFileDescriptor(uri, String("r"), (IAssetFileDescriptor**)&fd);
    if (fd == NULL) {
        return NOERROR;
    }

    AutoPtr<IFileDescriptor> des;
    fd->GetFileDescriptor((IFileDescriptor**)&des);
    ECode ec = AddTimedTextSourceEx2(des, mimeType);
    fd->Close();
    return ec;
    //} catch (SecurityException ex) {
    //} catch (IOException ex) {
    //} finally {
    //    if (fd != NULL) {
    //        fd->Close();
    //    }
    //}
}

ECode CMediaPlayer::AddTimedTextSourceEx2(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ const String& mimeType)
{
    // intentionally less than LONG_MAX
    return AddTimedTextSourceEx3(fd, 0, 0x7ffffffffffffffL, mimeType);
}

ECode CMediaPlayer::AddTimedTextSourceEx3(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 length,
    /* [in] */ const String& mimeType)
{
    if (!AvailableMimeTypeForExternalSource(mimeType)) {
        Logger::E(TAG, "Illegal mimeType for timed text source: %s", mimeType.string());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IParcel> request;
    CCallbackParcel::New((IParcel**)&request);
    AutoPtr<IParcel> reply;
    CCallbackParcel::New((IParcel**)&reply);

    assert(0 && "TODO");
    //try {
    // request->WriteInterfaceToken(IMEDIA_PLAYER);
    // request->WriteInt(INVOKE_ID_ADD_EXTERNAL_SOURCE_FD);
    // request->WriteFileDescriptor(fd);
    // request->WriteLong(offset);
    // request->WriteLong(length);
    // request->WriteString(mimeType);
    ECode ec = Invoke(request, reply);
    //} finally {
        // request->Recycle();
        // reply->Recycle();
    //}

    return ec;
}

ECode CMediaPlayer::SelectTrack(
    /* [in] */ Int32 index)
{
    return SelectOrDeselectTrack(index, TRUE /* select */);
}

ECode CMediaPlayer::DeselectTrack(
    /* [in] */ Int32 index)
{
    return SelectOrDeselectTrack(index, FALSE /* select */);
}

ECode CMediaPlayer::SelectOrDeselectTrack(
    /* [in] */ Int32 index,
    /* [in] */ Boolean select)
{
    assert(0 && "TODO");
    /*
    AutoPtr<IParcel> request = Parcel->Obtain();
    AutoPtr<IParcel> reply = Parcel->Obtain();
    //try {
        request->WriteInterfaceToken(IMEDIA_PLAYER);
        request->WriteInt(select? INVOKE_ID_SELECT_TRACK: INVOKE_ID_DESELECT_TRACK);
        request->WriteInt(index);
        Invoke(request, reply);
    //} finally {
        request->Recycle();
        reply->Recycle();
    //}
    */
   return NOERROR;
}

ECode CMediaPlayer::NativePullBatteryData(
    /* [in] */ IParcel* reply,
    /* [out] */ Int32* result)
{
    assert(0 && "TODO");
    /*
    android::sp<IBinder> binder = defaultServiceManager()->getService(String16("media.player"));
    android::sp<IMediaPlayerService> service = interface_cast<IMediaPlayerService>(binder);
    if (service->Get() == NULL) {
        //jniThrowException(env, "java/lang/RuntimeException", "cannot get MediaPlayerService");
        return UNKNOWN_ERROR;
    }

    AutoPtr<IParcel> mReply = reply;
    */
    // return service->pullBatteryData(mReply);
    return E_NOT_IMPLEMENTED;
}

ECode CMediaPlayer::SetRetransmitEndpoint(
    /* [in] */ IInetSocketAddress* endpoint)
{
    String addrString;
    Int32 port = 0;
    if (NULL != endpoint) {
        AutoPtr<IInetAddress> address;
        endpoint->GetAddress((IInetAddress**)&address);
        address->GetHostAddress(&addrString);
        endpoint->GetPort(&port);
    }

    return NativeSetRetransmitEndpoint(addrString, port);
}

ECode CMediaPlayer::NativeSetRetransmitEndpoint(
    /* [in] */ const String& addrString,
    /* [in] */ Int32 port)
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    const char *cAddrString = NULL;

    if (!addrString.IsNull()) {
        cAddrString = addrString.string();
    }

    if (LOGV) Logger::V(TAG, "setRetransmitEndpoint: %s:%d",
            cAddrString ? cAddrString : "(NULL)", port);

    android::status_t ret;
    if (cAddrString && (port > 0xFFFF)) {
        ret = android::BAD_VALUE;
    }
    else {
        ret = mp->setRetransmitEndpoint(cAddrString,
                static_cast<uint16_t>(port));
    }

    return (ret == android::OK) ? NOERROR : E_FAIL;
}

ECode CMediaPlayer::NativeFinalize()
{
    if (LOGV) Logger::V(TAG, "native_finalize");
    return NativeRelease();
}

ECode CMediaPlayer::PostEventFromNative(
    /* [in] */ IWeakReference* mediaplayer_ref,
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IInterface* obj)
{
    if (LOGV) {
        Logger::V(TAG, "PostEventFromNative: what=%d, arg1=%d arg2=%d.", what, arg1, arg2);
    }

    AutoPtr<IMediaPlayer> strObj;
    mediaplayer_ref->Resolve(EIID_IMediaPlayer, (IInterface**)&strObj);
    if (strObj == NULL) {
        if (LOGV) {
            Logger::V(TAG, "PostEventFromNative: CMediaPlayer already destroyed.");
        }
        return NOERROR;
    }

    CMediaPlayer* mp = (CMediaPlayer*)strObj.Get();
    if (what == MEDIA_INFO && arg1 == MEDIA_INFO_STARTED_AS_NEXT) {
        // this acquires the wakelock if needed, and sets the client side state
        mp->Start();
    }

    if (mp->mEventHandler != NULL) {
        AutoPtr<IMessage> message;
        mp->mEventHandler->ObtainMessageEx3(what, arg1, arg2, obj, (IMessage**)&message);
        Boolean bval;
        return mp->mEventHandler->SendMessage(message, &bval);
    }

    return NOERROR;
}

ECode CMediaPlayer::SetOnPreparedListener(
    /* [in] */ IMediaPlayerOnPreparedListener* listener)
{
    mOnPreparedListener = listener;
    return NOERROR;
}

ECode CMediaPlayer::SetOnCompletionListener(
    /* [in] */ IMediaPlayerOnCompletionListener* listener)
{
    mOnCompletionListener = listener;
    return NOERROR;
}

ECode CMediaPlayer::SetOnBufferingUpdateListener(
    /* [in] */ IMediaPlayerOnBufferingUpdateListener* listener)
{
    mOnBufferingUpdateListener = listener;
    return NOERROR;
}

ECode CMediaPlayer::SetOnSeekCompleteListener(
    /* [in] */ IMediaPlayerOnSeekCompleteListener* listener)
{
    mOnSeekCompleteListener = listener;
    return NOERROR;
}

ECode CMediaPlayer::SetOnVideoSizeChangedListener(
    /* [in] */ IMediaPlayerOnVideoSizeChangedListener* listener)
{
    mOnVideoSizeChangedListener = listener;
    return NOERROR;
}

ECode CMediaPlayer::SetOnTimedTextListener(
    /* [in] */ IMediaPlayerOnTimedTextListener* listener)
{
    mOnTimedTextListener = listener;
    return NOERROR;
}

ECode CMediaPlayer::SetOnErrorListener(
    /* [in] */ IMediaPlayerOnErrorListener* listener)
{
    mOnErrorListener = listener;
    return NOERROR;
}

ECode CMediaPlayer::SetOnInfoListener(
    /* [in] */ IMediaPlayerOnInfoListener* listener)
{
    mOnInfoListener = listener;
    return NOERROR;
}

Boolean CMediaPlayer::IsVideoScalingModeSupported(
    /* [in] */ Int32 mode)
{
    return (mode == VIDEO_SCALING_MODE_SCALE_TO_FIT ||
            mode == VIDEO_SCALING_MODE_SCALE_TO_FIT_WITH_CROPPING);
}

ECode CMediaPlayer::SetScreen(
    /* [in] */ Int32 screen)
{
    android::MediaPlayer::setScreen(screen);
    return NOERROR;
}

ECode CMediaPlayer::GetScreen(
    /* [out] */ Int32* screen)
{
    VALIDATE_NOT_NULL(screen);
    int screen_;
    if (android::MediaPlayer::getScreen(&screen_) != android::OK) {
        *screen = MASTER_SCREEN;
    }
    else {
        *screen = screen_;
    }
    return NOERROR;
}

ECode CMediaPlayer::IsPlayingVideo(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    bool playing;
    if (android::OK != android::MediaPlayer::isPlayingVideo(&playing)){
        Logger::E(TAG, "Fail in isPlayingVideo()");
        return E_FAIL;
    }
    *result = playing;
    return NOERROR;
}

static AutoPtr<IMediaPlayerSubInfo> _composeObjSubInfo(
    MediaPlayer_SubInfo * info)
{
    String charset(info->charset);
    AutoPtr<ArrayOf<Byte> > name = ArrayOf<Byte>::Alloc(info->len);
    memcpy(name->GetPayload(), info->name, info->len);
    AutoPtr<IMediaPlayerSubInfo> subInfo;
    CMediaPlayerSubInfo::New(
        name, charset, info->type, (IMediaPlayerSubInfo**)&subInfo);
    return subInfo;
}
ECode CMediaPlayer::GetSubList(
    /* [out, callee] */ ArrayOf<IMediaPlayerSubInfo*>** infoArray)
{
    VALIDATE_NOT_NULL(infoArray);
    *infoArray = NULL;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32 count = mp->getSubCount();
    if (count < 0) {
        return NOERROR;
    }

    ECode ec = NOERROR;
    MediaPlayer_SubInfo *csubList = NULL;

    AutoPtr<ArrayOf<IMediaPlayerSubInfo*> > infos = ArrayOf<IMediaPlayerSubInfo*>::Alloc(count);
    if (infos == NULL) {
        ec = E_OUT_OF_MEMORY;
        goto error;
    }

    csubList = new MediaPlayer_SubInfo[count];
    if (csubList == NULL) {
        ec = E_OUT_OF_MEMORY;
        goto error;
    }

    count = mp->getSubList(csubList, count);
    if(count <= 0){
        Logger::E(TAG, "Fail in getting sublist.");
        goto error;
    }

    for (Int32 i = 0; i < count; ++i) {
        infos->Set(i, _composeObjSubInfo(csubList + i));
    }

    *infoArray = infos;
    INTERFACE_ADDREF(*infoArray);

error:
    if (csubList != NULL) delete[] csubList;
    return ec;
}

ECode CMediaPlayer::GetCurSub(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = -1;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *index = mp->getCurSub();
    return NOERROR;
}

ECode CMediaPlayer::SwitchSub(
    /* [in] */ Int32 index)
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32 err = mp->switchSub(index);
    return (err == android::OK) ? NOERROR : E_FAIL;
}

ECode CMediaPlayer::SetSubGate(
    /* [in] */ Boolean showSub)
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32 err = mp->setSubGate((bool)showSub);
    return (err == android::OK) ? NOERROR : E_FAIL;
}

ECode CMediaPlayer::GetSubGate(
    /* [out] */ Boolean* showSub)
{
    VALIDATE_NOT_NULL(showSub);
    *showSub = FALSE;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *showSub = mp->getSubGate();
    return NOERROR;
}

ECode CMediaPlayer::SetSubColor(
    /* [in] */ Int32 color)
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32 err = mp->setSubColor(color);
    return (err == android::OK) ? NOERROR : E_FAIL;
}

ECode CMediaPlayer::GetSubColor(
   /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);
    *color = 0;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *color = mp->getSubColor();
    return NOERROR;
}

ECode CMediaPlayer::SetSubFrameColor(
    /* [in] */ Int32 color)
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32 err = mp->setSubFrameColor(color);
    return (err == android::OK) ? NOERROR : E_FAIL;
}

ECode CMediaPlayer::GetSubFrameColor(
    /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);
    *color = 0;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *color = mp->getSubFrameColor();
    return NOERROR;
}

ECode CMediaPlayer::SetSubFontSize(
    /* [in] */ Int32 size)
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32 err = mp->setSubFontSize(size);
    return (err == android::OK) ? NOERROR : E_FAIL;
}

ECode CMediaPlayer::GetSubFontSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    *size = 0;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *size = mp->getSubFontSize();
    return NOERROR;
}

ECode CMediaPlayer::SetSubFontPath(
    /* [in] */ const String& path)
{
    if (path.IsNull())
        return E_ILLEGAL_ARGUMENT_EXCEPTION;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32 err = mp->setSubFontPath(path.string());
    return (err == android::OK) ? NOERROR : E_FAIL;
}

ECode CMediaPlayer::SetSubCharset(
    /* [in] */ const String& charset)
{
    if (charset.IsNull())
        return E_ILLEGAL_ARGUMENT_EXCEPTION;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32 err = mp->setSubCharset(charset.string());
    return (err == android::OK) ? NOERROR : E_FAIL;
}

ECode CMediaPlayer::GetSubCharset(
    /* [out] */ String* charset)
{
    VALIDATE_NOT_NULL(charset);
    *charset = String(NULL);

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    char *ccharset = new char[MEDIAPLAYER_NAME_LEN_MAX];
    if(ccharset == NULL){
        Logger::E(TAG, "Fail in allocating memory.");
        return E_OUT_OF_MEMORY;
    }

    android::status_t ret = mp->getSubCharset(ccharset);
    if (ret == android::OK){
        *charset = String(ccharset);
    }

    delete[] ccharset;
    return NOERROR;
}

ECode CMediaPlayer::SetSubPosition(
    /* [in] */ Int32 percent)
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32 err = mp->setSubPosition(percent);
    return (err == android::OK) ? NOERROR : E_FAIL;
}

ECode CMediaPlayer::GetSubPosition(
    /* [out] */ Int32* percent)
{
    VALIDATE_NOT_NULL(percent);
    *percent = 0;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *percent = mp->getSubPosition();
    return NOERROR;
}

ECode CMediaPlayer::SetSubDelay(
    /* [in] */ Int32 delay)
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32 err = mp->setSubDelay(delay);
    return (err == android::OK) ? NOERROR : E_FAIL;
}

ECode CMediaPlayer::GetSubDelay(
    /* [out] */ Int32* delay)
{
    VALIDATE_NOT_NULL(delay);
    *delay = 0;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *delay = mp->getSubDelay();
    return NOERROR;
}

static AutoPtr<ITrackInfoVendor> _composeObjTrackInfo(
    const MediaPlayer_TrackInfo * info)
{
    AutoPtr<ITrackInfoVendor> vendor;
    String charset(info->charset);
    AutoPtr<ArrayOf<Byte> > name = ArrayOf<Byte>::Alloc(info->len);
    memcpy(name->GetPayload(), info->name, info->len);

    CTrackInfoVendor::New(name, charset, (ITrackInfoVendor**)&vendor);
    return vendor;
}

ECode CMediaPlayer::GetTrackList(
    /* [out, callee] */ ArrayOf<ITrackInfoVendor*>** trackArray)
{
    VALIDATE_NOT_NULL(trackArray);
    *trackArray = NULL;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    int count = mp->getTrackCount();
    if (count <= 0)
        return NOERROR;

    AutoPtr<ArrayOf<ITrackInfoVendor*> > tracks;
    ECode ec = NOERROR;
    Int32 i;
    MediaPlayer_TrackInfo *ctrackList = new MediaPlayer_TrackInfo[count];
    if(ctrackList == NULL ){
        Logger::E(TAG, "Fail in allocating memory.");
        ec = E_OUT_OF_MEMORY;
        goto error;
    }

    count = mp->getTrackList(ctrackList, count);
    if(count < 0){
        Logger::E(TAG, "Fail in getting tracklist.");
        goto error;
    }

    tracks = ArrayOf<ITrackInfoVendor*>::Alloc(count);
    for (i = 0; i < count; ++i) {
        tracks->Set(i, _composeObjTrackInfo(ctrackList+i));
    }

    *trackArray = tracks;
    INTERFACE_ADDREF(*trackArray);

error:
    if (ctrackList) delete[] ctrackList;
    return ec;
}

ECode CMediaPlayer::GetCurTrack(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = 0;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *index = mp->getCurTrack();
    return NOERROR;
}

ECode CMediaPlayer::SwitchTrack(
    /* [in] */ Int32 index)
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    android::status_t err = mp->switchTrack(index);
    return (err == android::OK) ? NOERROR : E_FAIL;
}

ECode CMediaPlayer::SetInputDimensionType(
    /* [in] */ Int32 type)
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32 err = mp->setInputDimensionType(type);
    return (err == android::OK) ? NOERROR : E_FAIL;
}

ECode CMediaPlayer::GetInputDimensionType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = 0;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *type = mp->getInputDimensionType();
    return NOERROR;
}

ECode CMediaPlayer::SetInputDimensionValue(
    /* [in] */ Int32 type,
    /* [in] */ Int32 value)
{
    assert(0 && "TODO");
    // android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    // if (mp == NULL ) {
    //     return E_ILLEGAL_STATE_EXCEPTION;
    // }

    // Int32 err = mp->setInputDimensionValue(delay);
    // if (err != android::OK) return E_FAIL;
    return NOERROR;
}


ECode CMediaPlayer::SetOutputDimensionType(
    /* [in] */ Int32 type)
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32 err = mp->setOutputDimensionType(type);
    return (err == android::OK) ? NOERROR : E_FAIL;
}

ECode CMediaPlayer::GetOutputDimensionType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = 0;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *type = mp->getOutputDimensionType();
    return NOERROR;
}

ECode CMediaPlayer::SetOutputDimensionValue(
    /* [in] */ Int32 type,
    /* [in] */ Int32 value)
{
    assert(0 && "TODO");
    // android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    // if (mp == NULL ) {
    //     return E_ILLEGAL_STATE_EXCEPTION;
    // }

    // Int32 err = mp->setOutputDimensionValue(delay);
    // if (err != android::OK) return E_ILLEGAL_STATE_EXCEPTION;
    return NOERROR;
}

ECode CMediaPlayer::SetAnaglaghType(
    /* [in] */ Int32 type)
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32 err = mp->setAnaglaghType(type);
    return (err == android::OK) ? NOERROR : E_FAIL;
}

ECode CMediaPlayer::GetAnaglaghType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = 0;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *type = mp->getAnaglaghType();
    return NOERROR;
}

ECode CMediaPlayer::GetVideoEncode(
    /* [out] */ String* encode)
{
    VALIDATE_NOT_NULL(encode);
    *encode = String(NULL);

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    char *ccencode = new char[MEDIAPLAYER_NAME_LEN_MAX];
    if (encode == NULL){
        Logger::E(TAG, "Fail in allocating memory.");
        return E_OUT_OF_MEMORY;
    }

    android::status_t ret = mp->getVideoEncode(ccencode);
    ECode ec = E_FAIL;
    if(ret == android::OK){
        ec = NOERROR;
        *encode = String(ccencode);
    }
    delete[] ccencode;
    return ec;
}

ECode CMediaPlayer::GetVideoFrameRate(
    /* [out] */ Int32* rate)
{
    VALIDATE_NOT_NULL(rate);
    *rate = 0;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *rate = mp->getVideoFrameRate();
    return NOERROR;
}

ECode CMediaPlayer::GetAudioEncode(
    /* [out] */ String* encode)
{
    VALIDATE_NOT_NULL(encode);
    *encode = String(NULL);

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    char *ccencode = new char[MEDIAPLAYER_NAME_LEN_MAX];
    if (encode == NULL){
        Logger::E(TAG, "Fail in allocating memory.");
        return E_OUT_OF_MEMORY;
    }

    ECode ec = E_FAIL;
    android::status_t ret = mp->getAudioEncode(ccencode);
    if(ret == android::OK){
        ec = NOERROR;
        *encode = String(ccencode);
    }
    delete[] ccencode;
    return ec;
}

ECode CMediaPlayer::GetAudioBitRate(
    /* [out] */ Int32* rate)
{
    VALIDATE_NOT_NULL(rate);
    *rate = 0;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *rate = mp->getAudioBitRate();
    return NOERROR;
}

ECode CMediaPlayer::GetAudioSampleRate(
    /* [out] */ Int32* rate)
{
    VALIDATE_NOT_NULL(rate);
    *rate = 0;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *rate = mp->getAudioSampleRate();
    return NOERROR;
}

ECode CMediaPlayer::EnableScaleMode(
    /* [in] */ Boolean enable,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32 err = mp->enableScaleMode((bool)enable, width, height);
    return (err == android::OK) ? NOERROR : E_FAIL;
}

ECode CMediaPlayer::SetVppGate(
    /* [in] */ Boolean enableVpp)
{
    android::MediaPlayer::setVppGate(enableVpp);
    return NOERROR;
}

ECode CMediaPlayer::GetVppGate(
    /* [out] */ Boolean* enableVpp)
{
    VALIDATE_NOT_NULL(enableVpp);
    *enableVpp = android::MediaPlayer::getVppGate();
    return NOERROR;
}

ECode CMediaPlayer::SetLumaSharp(
    /* [in] */ Int32 value)
{
    if (android::OK == android::MediaPlayer::setLumaSharp(value)) {
        return NOERROR;
    }
    return E_FAIL;
}

ECode CMediaPlayer::GetLumaSharp(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = android::MediaPlayer::getLumaSharp();
    return NOERROR;
}

ECode CMediaPlayer::SetChromaSharp(
    /* [in] */ Int32 value)
{
    if (android::OK == android::MediaPlayer::setChromaSharp(value)) {
        return NOERROR;
    }
    return E_FAIL;
}

ECode CMediaPlayer::GetChromaSharp(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = android::MediaPlayer::getChromaSharp();
    return NOERROR;
}

ECode CMediaPlayer::SetWhiteExtend(
    /* [in] */ Int32 value)
{
    if (android::OK == android::MediaPlayer::setWhiteExtend(value)) {
        return NOERROR;
    }
    return E_FAIL;
}

ECode CMediaPlayer::GetWhiteExtend(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = android::MediaPlayer::getWhiteExtend();
    return NOERROR;
}

ECode CMediaPlayer::SetBlackExtend(
    /* [in] */ Int32 value)
{
    if (android::OK == android::MediaPlayer::setBlackExtend(value)) {
        return NOERROR;
    }
    return E_FAIL;
}

ECode CMediaPlayer::GetBlackExtend(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = android::MediaPlayer::getBlackExtend();
    return NOERROR;
}

ECode CMediaPlayer::SetChannelMuteMode(
    /* [in] */ Int32 mode)
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32 err = mp->setChannelMuteMode(mode);
    return (err == android::OK) ? NOERROR : E_FAIL;
}

ECode CMediaPlayer::GetChannelMuteMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = 0;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *mode = mp->getChannelMuteMode();
    return NOERROR;
}

ECode CMediaPlayer::SetGlobalSubGate(
    /* [in] */ Boolean showSub)
{
    if (android::OK == android::MediaPlayer::setGlobalSubGate(showSub)) {
        return NOERROR;
    }
    return E_FAIL;
}

ECode CMediaPlayer::GetGlobalSubGate(
    /* [out] */ Boolean* showSub)
{
    VALIDATE_NOT_NULL(showSub);
    *showSub = android::MediaPlayer::getGlobalSubGate();
    return NOERROR;
}

ECode CMediaPlayer::SetBdFolderPlayMode(
    /* [in] */ Boolean enable)
{
    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    int input = enable? 1 : 0;
    android::status_t err = mp->generalInterface(
        MEDIAPLAYER_CMD_SET_BD_FOLDER_PLAY_MODE,
        input, 0, 0, NULL);
    return (err == android::OK) ? NOERROR : E_FAIL;
}

ECode CMediaPlayer::GetBdFolderPlayMode(
    /* [out] */ Boolean* enable)
{
    VALIDATE_NOT_NULL(enable);
    *enable = FALSE;

    android::sp<android::MediaPlayer> mp = getMediaPlayer(this);
    if (mp == NULL ) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    int ienable = 0;
    mp->generalInterface(MEDIAPLAYER_CMD_GET_BD_FOLDER_PLAY_MODE, 0, 0, 0, &ienable);
    *enable = ienable == 1;
    return NOERROR;
}

ECode CMediaPlayer::IsRotatable(
    /* [out] */ Boolean* enable)
{
    VALIDATE_NOT_NULL(enable);
    *enable = 0;

    int ienable = 0;
    android::MediaPlayer::generalGlobalInterface(
        MEDIAPLAYER_CMD_IS_ROTATABLE, 0, 0, 0, &ienable);
    *enable = ienable == 1;
    return NOERROR;
}

ECode CMediaPlayer::SetRotation(
    /* [in] */ Int32 rotation)
{
    if (android::OK == android::MediaPlayer::generalGlobalInterface(
        MEDIAPLAYER_CMD_SET_ROTATION, rotation, 0, 0, NULL)) {
        return NOERROR;
    }
    return E_FAIL;
}

ECode CMediaPlayer::SetHdmiState(
    /* [in] */ Boolean bHdmiPlugged)
{
    if (android::OK == android::MediaPlayer::generalGlobalInterface(
        MEDIAPLAYER_CMD_SET_HDMISTATE, bHdmiPlugged, 0, 0, NULL)) {
        return NOERROR;
    }
    return E_FAIL;
}

ECode CMediaPlayer::SetDlnaSourceDetector(
    /* [in] */ IDlnaSourceDetector* detector)
{
    mDlnaSourceDetector = detector;

    ECode ec = SetDataSourceEx2(SOFTWINNER_DLNA_SOURCE_DETECTOR);
    if (FAILED(ec)) {
        Logger::E(TAG, "Fail to set DlnaSourceDetector..");
    }
    return ec;
}

ECode CMediaPlayer::SetRawDataMode(
    /* [in] */ Int32 audioDataMode)
{
    mRawDataMode = audioDataMode;
    Logger::D(TAG, "setRawDataMode() in MediaPlayer.java");
    Int32 result = android::MediaPlayer::generalGlobalInterface(
        MEDIAPLAYER_GLOBAL_CMD_SET_RAWDATA_MODE, audioDataMode, 0, 0, NULL);
    return result == android::OK ? NOERROR : E_FAIL;
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
