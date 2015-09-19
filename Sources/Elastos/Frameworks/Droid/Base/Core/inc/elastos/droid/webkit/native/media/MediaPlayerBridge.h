
#ifndef __ELASTOS_DROID_WEBKIT_MEDIA_MEDIAPLAYERBRIDGE_H__
#define __ELASTOS_DROID_WEBKIT_MEDIA_MEDIAPLAYERBRIDGE_H__

// import android.content.Context;
// import android.media.MediaPlayer;
// import android.net.Uri;
// import android.os.AsyncTask;
// import android.os.Build;
// import android.os.ParcelFileDescriptor;
// import android.text.TextUtils;
// import android.util.Base64;
// import android.util.Base64InputStream;
// import android.util.Log;
// import android.view.Surface;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

// import java.io.ByteArrayInputStream;
// import java.io.File;
// import java.io.FileOutputStream;
// import java.io.IOException;
// import java.io.InputStream;
// import java.lang.reflect.InvocationTargetException;
// import java.lang.reflect.Method;
// import java.util.HashMap;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

/**
* A wrapper around android.media.MediaPlayer that allows the native code to use it.
* See media/base/android/media_player_bridge.cc for the corresponding native code.
*/
//@JNINamespace("media")
class MediaPlayerBridge
{
private:
    class LoadDataUriTask
        : public AsyncTask <Void, Void, Boolean>
    {
    public:
        LoadDataUriTask(
            /* [in] */ MediaPlayerBridge* owner,
            /* [in] */ IContext* context,
            /* [in] */ String data);

        //@Override
        CARAPI_(Boolean) DoInBackground(
            /* [in] */ Void... params);

        //@Override
        CARAPI_(void) OnPostExecute(
            /* [in] */ Boolean result);

    private:
        CARAPI_(void) DeleteFile();

    private:
        MediaPlayerBridge* mOwner;

        const String mData;
        const AutoPtr<IContext> mContext;
        AutoPtr<IFile> mTempFile;
    };

protected:
    class AllowedOperations
    {
    public:
        AllowedOperations(
            /* [in] */ Boolean canPause,
            /* [in] */ Boolean canSeekForward,
            /* [in] */ Boolean canSeekBackward);

        //@CalledByNative("AllowedOperations")
        CARAPI_(Boolean) CanPause();

        //@CalledByNative("AllowedOperations")
        CARAPI_(Boolean) CanSeekForward();

        //@CalledByNative("AllowedOperations")
        CARAPI_(Boolean) CanSeekBackward();

    private:
        const Boolean mCanPause;
        const Boolean mCanSeekForward;
        const Boolean mCanSeekBackward;
    };

protected:
    MediaPlayerBridge();

    MediaPlayerBridge(
        /* [in] */ Int64 nativeMediaPlayerBridge);

    //@CalledByNative
    CARAPI_(void) Destroy();

    CARAPI_(AutoPtr<IMediaPlayer>) GetLocalPlayer();

    //@CalledByNative
    CARAPI_(void) SetSurface(
        /* [in] */ ISurface* surface);

    //@CalledByNative
    CARAPI_(Boolean) PrepareAsync();

    //@CalledByNative
    CARAPI_(Boolean) IsPlaying();

    //@CalledByNative
    CARAPI_(Int32) GetVideoWidth();

    //@CalledByNative
    CARAPI_(Int32) GetVideoHeight();

    //@CalledByNative
    CARAPI_(Int32) GetCurrentPosition();

    //@CalledByNative
    CARAPI_(Int32) GetDuration();

    //@CalledByNative
    CARAPI_(void) Release();

    //@CalledByNative
    CARAPI_(void) SetVolume(
        /* [in] */ Double volume);

    //@CalledByNative
    CARAPI_(void) Start();

    //@CalledByNative
    CARAPI_(void) Pause();

    //@CalledByNative
    CARAPI_(void) SeekTo(
        /* [in] */ Int32 msec);

    //@CalledByNative
    CARAPI_(Boolean) SetDataSource(
        /* [in] */ IContext* context,
        /* [in] */ String url,
        /* [in] */ String cookies,
        /* [in] */ String userAgent,
        /* [in] */ Boolean hideUrlLog);

    //@CalledByNative
    CARAPI_(Boolean) SetDataSourceFromFd(
        /* [in] */ Int32 fd,
        /* [in] */ Int64 offset,
        /* [in] */ Int64 length);

    //@CalledByNative
    CARAPI_(Boolean) SetDataUriDataSource(
        /* [in] */ IContext* context,
        /* [in] */ const String& url);

    CARAPI_(void) SetOnBufferingUpdateListener(
        /* [in] */ IMediaPlayerOnBufferingUpdateListener* listener);

    CARAPI_(void) SetOnCompletionListener(
        /* [in] */ IMediaPlayerOnCompletionListener* listener);

    CARAPI_(void) SetOnErrorListener(
        /* [in] */ IMediaPlayerOnErrorListener* listener);

    CARAPI_(void) SetOnPreparedListener(
        /* [in] */ IMediaPlayerOnPreparedListener* listener);

    CARAPI_(void) SetOnSeekCompleteListener(
        /* [in] */ IMediaPlayerOnSeekCompleteListener* listener);

    CARAPI_(void) SetOnVideoSizeChangedListener(
        /* [in] */ IMediaPlayerOnVideoSizeChangedListener* listener);

    /**
     * Returns an AllowedOperations object to show all the operations that are
     * allowed on the media player.
     */
    //@CalledByNative
    CARAPI_(AutoPtr<AllowedOperations>) GetAllowedOperations();

private:
    //@CalledByNative
    static CARAPI_(AutoPtr<MediaPlayerBridge>) Create(
        /* [in] */ Int64 nativeMediaPlayerBridge);

    CARAPI_(void) NativeOnDidSetDataUriDataSource(
        /* [in] */ Int64 nativeMediaPlayerBridge,
        /* [in] */ Boolean success);

private:
    static const String TAG;

    // Local player to forward this to. We don't initialize it here since the subclass might not
    // want it.
    AutoPtr<LoadDataUriTask> mLoadDataUriTask;
    AutoPtr<IMediaPlayer> mPlayer;
    Int64 mNativeMediaPlayerBridge;
};

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_MEDIA_MEDIAPLAYERBRIDGE_H__
