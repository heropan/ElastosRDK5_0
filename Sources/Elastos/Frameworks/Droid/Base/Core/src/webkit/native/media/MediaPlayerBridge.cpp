
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

//===============================================================
//             MediaPlayerBridge::LoadDataUriTask
//===============================================================

MediaPlayerBridge::LoadDataUriTask::LoadDataUriTask(
    /* [in] */ MediaPlayerBridge* owner,
    /* [in] */ IContext* context,
    /* [in] */ String data)
    : mOwner(owner)
    , mData(data)
    , mContext(context)
{
}

//@Override
Boolean MediaPlayerBridge::LoadDataUriTask::DoInBackground(Void... params)
{
    AutoPtr<IFileOutputStream> fos;
    // try {
        File::CreateTempFile(String("decoded"), String("mediadata"), (IFile**)&mTempFile);
        CFileOutputStream::New(mTempFile, (IFileOutputStream**)&fos);
        AutoPtr<ArrayOf<Byte> > data = mData->GetBytes();
        AutoPtr<IInputStream> stream;
        CByteArrayInputStream::New(data, (IInputStream**)&stream);
        AutoPtr<IBase64InputStream> decoder;
        CBase64InputStream::New(stream, Base64.DEFAULT, (IBase64InputStream**)&decoder);
        AutoPtr< ArrayOf<Byte> > buffer = ArrayOf<Byte>::Alloc(1024);
        Int32 len;
        while ((decoder->Read(buffer, &len), len) != -1) {
            fos->Write(buffer, 0, len);
        }
        decoder->Close();
    //    return true;
    // } catch (IOException e) {
    //     return false;
    // } finally {
    //     try {
             if (fos != NULL) fos->Close();
    //     } catch (IOException e) {
    //         // Can't do anything.
    //     }
    // }

    return TRUE;
}

//@Override
void MediaPlayerBridge::LoadDataUriTask::OnPostExecute(
    /* [in] */ Boolean result)
{
    if (IsCancelled()) {
        DeleteFile();
        return;
    }

    // try {
        AutoPtr<IUriHelper> helper;
        CUriHelper::AcquireSingleton((IUriHelper**)&helper);
        AutoPtr<IUri> uri;
        helper->FromFile(mTempFile, (IUri**)&uri);
        GetLocalPlayer()->SetDataSource(mContext, uri);
    // } catch (IOException e) {
    //     result = false;
    // }

    DeleteFile();
    Assert (mNativeMediaPlayerBridge != 0);
    NativeOnDidSetDataUriDataSource(mNativeMediaPlayerBridge, result);
}

void MediaPlayerBridge::LoadDataUriTask::DeleteFile()
{
    if (mTempFile == NULL) return;

    Boolean bDelete = FALSE;
    mTempFile->delete(&bDelete);
    if (!bDelete) {
        // File will be deleted when MediaPlayer releases its handler.
//        Log.e(TAG, "Failed to delete temporary file: " + mTempFile);
        assert (FALSE);
    }
}

//===============================================================
//             MediaPlayerBridge::AllowedOperations
//===============================================================

MediaPlayerBridge::AllowedOperations::AllowedOperations(
    /* [in] */ Boolean canPause,
    /* [in] */ Boolean canSeekForward,
    /* [in] */ Boolean canSeekBackward)
    : mCanPause(canPause)
    , mCanSeekForward(canSeekForward)
    , mCanSeekBackward(canSeekBackward)
{
}

//@CalledByNative("AllowedOperations")
Boolean MediaPlayerBridge::AllowedOperations::CanPause()
{
    return mCanPause;
}

//@CalledByNative("AllowedOperations")
Boolean MediaPlayerBridge::AllowedOperations::CanSeekForward()
{
    return mCanSeekForward;
}

//@CalledByNative("AllowedOperations")
Boolean MediaPlayerBridge::AllowedOperations::CanSeekBackward()
{
    return mCanSeekBackward;
}

//===============================================================
//                      MediaPlayerBridge
//===============================================================

const String MediaPlayerBridge::TAG("MediaPlayerBridge");

MediaPlayerBridge::MediaPlayerBridge()
    : mNativeMediaPlayerBridge(0)
{
}

MediaPlayerBridge::MediaPlayerBridge(
    /* [in] */ Int64 nativeMediaPlayerBridge)
    : mNativeMediaPlayerBridge(nativeMediaPlayerBridge)
{
}

//@CalledByNative
AutoPtr<MediaPlayerBridge> MediaPlayerBridge::Create(
    /* [in] */ Int64 nativeMediaPlayerBridge)
{
    AutoPtr<MediaPlayerBridge> mpb = new MediaPlayerBridge(nativeMediaPlayerBridge);
    return mpb;
}

//@CalledByNative
void MediaPlayerBridge::Destroy()
{
    if (mLoadDataUriTask != NULL) {
        mLoadDataUriTask->Cancel(TRUE);
        mLoadDataUriTask = NULL;
    }
    mNativeMediaPlayerBridge = 0;
}

AutoPtr<IMediaPlayer> MediaPlayerBridge::GetLocalPlayer()
{
    if (mPlayer == NULL) {
        mPlayer = new MediaPlayer();
    }
    return mPlayer;
}

//@CalledByNative
void MediaPlayerBridge::SetSurface(
    /* [in] */ ISurface* surface)
{
    GetLocalPlayer()->SetSurface(surface);
}

//@CalledByNative
Boolean MediaPlayerBridge::PrepareAsync()
{
    // try {
        GetLocalPlayer()->PrepareAsync();
    // } catch (IllegalStateException e) {
    //     Log.e(TAG, "Unable to prepare MediaPlayer.", e);
    //     return false;
    // }

    return TRUE;
}

//@CalledByNative
Boolean MediaPlayerBridge::IsPlaying()
{
    Boolean isPlaying;
    GetLocalPlayer()->IsPlaying(&isPlaying);
    return isPlaying;
}

//@CalledByNative
Int32 MediaPlayerBridge::GetVideoWidth()
{
    Int32 width;
    GetLocalPlayer()->GetVideoWidth(&width);
    return width;
}

//@CalledByNative
Int32 MediaPlayerBridge::GetVideoHeight()
{
    Int32 height;
    GetLocalPlayer()->GetVideoHeight(&height);
    return height;
}

//@CalledByNative
Int32 MediaPlayerBridge::GetCurrentPosition()
{
    Int32 position;
    GetLocalPlayer()->GetCurrentPosition(&position);
    return position;
}

//@CalledByNative
Int32 MediaPlayerBridge::GetDuration()
{
    Int32 duration;
    GetLocalPlayer()->GetDuration(&duration);
    return duration;
}

//@CalledByNative
void MediaPlayerBridge::Release()
{
    GetLocalPlayer()->Release();
}

//@CalledByNative
void MediaPlayerBridge::SetVolume(
    /* [in] */ Double volume)
{
    GetLocalPlayer()->SetVolume((Float) volume, (Float) volume);
}

//@CalledByNative
void MediaPlayerBridge::Start()
{
    GetLocalPlayer()->Start();
}

//@CalledByNative
void MediaPlayerBridge::Pause()
{
    GetLocalPlayer()->Pause();
}

//@CalledByNative
void MediaPlayerBridge::SeekTo(
    /* [in] */ Int32 msec)
{
    GetLocalPlayer()->SeekTo(msec);
}

//@CalledByNative
Boolean MediaPlayerBridge::SetDataSource(
    /* [in] */ IContext* context,
    /* [in] */ String url,
    /* [in] */ String cookies,
    /* [in] */ String userAgent,
    /* [in] */ Boolean hideUrlLog)
{
    Uri uri = Uri.parse(url);
    HashMap<String, String> headersMap = new HashMap<String, String>();
    if (hideUrlLog) headersMap.put("x-hide-urls-from-log", "true");
    if (!TextUtils.isEmpty(cookies)) headersMap.put("Cookie", cookies);
    if (!TextUtils.isEmpty(userAgent)) headersMap.put("User-Agent", userAgent);
    // The security origin check is enforced for devices above K. For devices below K,
    // only anonymous media HTTP request (no cookies) may be considered same-origin.
    // Note that if the server rejects the request we must not consider it same-origin.
    if (Build.VERSION.SDK_INT > Build.VERSION_CODES.KITKAT) {
        headersMap.put("allow-cross-domain-redirect", "false");
    }
    try {
        getLocalPlayer().setDataSource(context, uri, headersMap);
        return true;
    } catch (Exception e) {
        return false;
    }
}

//@CalledByNative
Boolean MediaPlayerBridge::SetDataSourceFromFd(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 length)
{
    // try {
        AutoPtr<IParcelFileDescriptorHelper> helper;
        CParcelFileDescriptorHelper::AcquireSingleton((IParcelFileDescriptorHelper**)&helper);
        helper->AdoptFd(fd, (IParcelFileDescriptor**)&parcelFd);
        AutoPtr<IFileDescriptor> des;
        parcelFd->GetFileDescriptor((IFileDescriptor**)&des);
        GetLocalPlayer()->SetDataSource(des, offset, length);
        parcelFd->Close();
        return TRUE;
    // } catch (IOException e) {
    //     Log.e(TAG, "Failed to set data source from file descriptor: " + e);
    //     return false;
    // }
}

//@CalledByNative
Boolean MediaPlayerBridge::SetDataUriDataSource(
    /* [in] */ IContext* context,
    /* [in] */ const String& url)
{
    if (mLoadDataUriTask != NULL) {
        mLoadDataUriTask->Cancel(TRUE);
        mLoadDataUriTask = NULL;
    }

    if (!url.StartsWith("data:")) return FALSE;
    Int32 headerStop = url.IndexOf(',');
    if (headerStop == -1) return FALSE;
    String header = url.Substring(0, headerStop);
    const String data = url.Substring(headerStop + 1);

    String headerContent = header.Substring(5);
    AutoPtr< ArrayOf<String> > headerInfo = headerContent.Split(";");
    if (headerInfo->GetLength() != 2) return FALSE;
    if (!String("base64").Equals((*headerInfo)[1])) return FALSE;

    mLoadDataUriTask = new LoadDataUriTask(context, data);
    mLoadDataUriTask->Execute();
    return TRUE;
}


void MediaPlayerBridge::SetOnBufferingUpdateListener(
    /* [in] */ IMediaPlayerOnBufferingUpdateListener* listener)
{
    GetLocalPlayer()->SetOnBufferingUpdateListener(listener);
}

void MediaPlayerBridge::SetOnCompletionListener(
    /* [in] */ IMediaPlayerOnCompletionListener* listener)
{
    GetLocalPlayer()->SetOnCompletionListener(listener);
}

void MediaPlayerBridge::SetOnErrorListener(
    /* [in] */ IMediaPlayerOnErrorListener* listener)
{
    GetLocalPlayer()->SetOnErrorListener(listener);
}

void MediaPlayerBridge::SetOnPreparedListener(
    /* [in] */ IMediaPlayerOnPreparedListener* listener)
{
    GetLocalPlayer()->SetOnPreparedListener(listener);
}

void MediaPlayerBridge::SetOnSeekCompleteListener(
    /* [in] */ IMediaPlayerOnSeekCompleteListener* listener)
{
    GetLocalPlayer()->SetOnSeekCompleteListener(listener);
}

void MediaPlayerBridge::SetOnVideoSizeChangedListener(
    /* [in] */ IMediaPlayerOnVideoSizeChangedListener* listener)
{
    GetLocalPlayer()->SetOnVideoSizeChangedListener(listener);
}

/**
 * Returns an AllowedOperations object to show all the operations that are
 * allowed on the media player.
 */
//@CalledByNative
AutoPtr<AllowedOperations> MediaPlayerBridge::GetAllowedOperations()
{
    AutoPtr<IMediaPlayer> player = GetLocalPlayer();
    Boolean canPause = TRUE;
    Boolean canSeekForward = TRUE;
    Boolean canSeekBackward = TRUE;
    // try {
        Method getMetadata = player.getClass().getDeclaredMethod(
                "getMetadata", boolean.class, boolean.class);
        getMetadata.setAccessible(true);
        Object data = getMetadata.invoke(player, false, false);
        if (data != null) {
            Class<?> metadataClass = data.getClass();
            Method hasMethod = metadataClass.getDeclaredMethod("has", int.class);
            Method getBooleanMethod = metadataClass.getDeclaredMethod("getBoolean", int.class);

            int pause = (Integer) metadataClass.getField("PAUSE_AVAILABLE").get(null);
            int seekForward =
                (Integer) metadataClass.getField("SEEK_FORWARD_AVAILABLE").get(null);
            int seekBackward =
                    (Integer) metadataClass.getField("SEEK_BACKWARD_AVAILABLE").get(null);
            hasMethod.setAccessible(true);
            getBooleanMethod.setAccessible(true);
            canPause = !((Boolean) hasMethod.invoke(data, pause))
                    || ((Boolean) getBooleanMethod.invoke(data, pause));
            canSeekForward = !((Boolean) hasMethod.invoke(data, seekForward))
                    || ((Boolean) getBooleanMethod.invoke(data, seekForward));
            canSeekBackward = !((Boolean) hasMethod.invoke(data, seekBackward))
                    || ((Boolean) getBooleanMethod.invoke(data, seekBackward));
        }
    // } catch (NoSuchMethodException e) {
    //     Log.e(TAG, "Cannot find getMetadata() method: " + e);
    // } catch (InvocationTargetException e) {
    //     Log.e(TAG, "Cannot invoke MediaPlayer.getMetadata() method: " + e);
    // } catch (IllegalAccessException e) {
    //     Log.e(TAG, "Cannot access metadata: " + e);
    // } catch (NoSuchFieldException e) {
    //     Log.e(TAG, "Cannot find matching fields in Metadata class: " + e);
    // }

    AutoPtr<AllowedOperations> ret = new AllowedOperations(canPause, canSeekForward, canSeekBackward);

    return ret;
}

void MediaPlayerBridge::NativeOnDidSetDataUriDataSource(
    /* [in] */ Int64 nativeMediaPlayerBridge,
    /* [in] */ Boolean success)
{
}

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos