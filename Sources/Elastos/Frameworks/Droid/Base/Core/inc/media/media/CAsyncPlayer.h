
#ifndef __CASYNCPLAYER_H__
#define __CASYNCPLAYER_H__

#include "_CAsyncPlayer.h"
#include "ext/frameworkext.h"
#include <elastos/ThreadBase.h>
#include <elastos/List.h>

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::IPowerManagerWakeLock;
using Elastos::Core::Threading::ThreadBase;
using Elastos::Utility::List;

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CAsyncPlayer)
{
public:
    CAsyncPlayer();

    /**
     * Construct an AsyncPlayer object.
     *
     * @param tag a string to use for debugging
     */
    CARAPI constructor(
        /* [in] */ const String& tag);

    /**
     * Start playing the sound.  It will actually start playing at some
     * point in the future.  There are no guarantees about latency here.
     * Calling this before another audio file is done playing will stop
     * that one and start the new one.
     *
     * @param context Your application's context.
     * @param uri The URI to play.  (see {@link MediaPlayer#setDataSource(Context, Uri)})
     * @param looping Whether the audio should loop forever.
     *          (see {@link MediaPlayer#setLooping(boolean)})
     * @param stream the AudioStream to use.
     *          (see {@link MediaPlayer#setAudioStreamType(int)})
     */
    CARAPI Play(
        /* [in] */ IContext* context,
        /* [in] */ IUri* uri,
        /* [in] */ Boolean looping,
        /* [in] */ Int32 stream);

    /**
     * Stop a previously played sound.  It can't be played again or unpaused
     * at this point.  Calling this multiple times has no ill effects.
     */
    CARAPI Stop();

    /**
     * We want to hold a wake lock while we do the prepare and play.  The stop probably is
     * optional, but it won't hurt to have it too.  The problem is that if you start a sound
     * while you're holding a wake lock (e.g. an alarm starting a notification), you want the
     * sound to play, but if the CPU turns off before mThread gets to work, it won't.  The
     * simplest way to deal with this is to make it so there is a wake lock held while the
     * thread is starting or running.  You're going to need the WAKE_LOCK permission if you're
     * going to call this.
     *
     * This must be called before the first time play is called.
     *
     * @hide
     */
    CARAPI SetUsesWakeLock(
        /* [in] */ IContext* context);

private:
    class Command : public ElRefBase
    {
    public:
        CARAPI_(String) ToString();

    public:
        Int32 code;
        AutoPtr<IContext> context;
        AutoPtr<IUri> uri;
        Boolean looping;
        Int32 stream;
        Int64 requestTime;
    };

    CARAPI_(void) StartSound(
        /* [in] */ Command* cmd);

    class MyThread
        : public ThreadBase
    {
    public:
        MyThread(
            /* [in] */ CAsyncPlayer* owner);

        virtual CARAPI Run();

    private:
        CAsyncPlayer* mOwner;
    };

    CARAPI_(void) EnqueueLocked(
        /* [in] */ Command* cmd);

    CARAPI_(void) AcquireWakeLock();

    CARAPI_(void) ReleaseWakeLock();

private:
    static const Int32 PLAY;
    static const Int32 STOP;
    static const Boolean mDebug;

    List< AutoPtr<Command> > mCmdQueue;
    Mutex mCmdQueueLock;

    String mTag;
    AutoPtr<MyThread> mThread;
    AutoPtr<IMediaPlayer> mPlayer;

    //private PowerManager.WakeLock mWakeLock;
    AutoPtr<IPowerManagerWakeLock> mWakeLock;

    // The current state according to the caller.  Reality lags behind
    // because of the asynchronous nature of this class.
    Int32 mState; // = STOP;
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __CASYNCPLAYER_H__
