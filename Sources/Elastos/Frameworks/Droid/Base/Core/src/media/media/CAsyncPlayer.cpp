
#include "media/CAsyncPlayer.h"
#include "os/SystemClock.h"
#include "media/CMediaPlayer.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::IPowerManager;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Media {

const Int32 CAsyncPlayer::PLAY = 1;
const Int32 CAsyncPlayer::STOP = 2;
const Boolean CAsyncPlayer::mDebug = FALSE;

//================================================================================
//                      CAsyncPlayer::Command
//================================================================================
String CAsyncPlayer::Command::ToString()
{
    String tempText;
    uri->ToString(&tempText);

    StringBuilder sb("{ code=");
    sb += StringUtils::Int32ToString(code);
    sb += " looping=";
    sb += StringUtils::BooleanToString(looping);
    sb += " stream=";
    sb += StringUtils::Int32ToString(stream);
    sb += " uri=";
    sb += tempText;
    sb += " }";
    return sb.ToString();
}

//================================================================================
//                      CAsyncPlayer::MyThread
//================================================================================
CAsyncPlayer::MyThread::MyThread(
    /* [in] */ CAsyncPlayer* owner)
    : mOwner(owner)
{
    Init(String("AsyncPlayer-") + mOwner->mTag);
}

ECode CAsyncPlayer::MyThread::Run()
{
    while (TRUE) {
        AutoPtr<Command> cmd;

        {
            Mutex::Autolock lock(mOwner->mCmdQueueLock);

            if (mOwner->mDebug) Logger::D(mOwner->mTag, "RemoveFirst");

            cmd = mOwner->mCmdQueue.GetFront();
            mOwner->mCmdQueue.PopFront();
        }

        switch (cmd->code) {
            case CAsyncPlayer::PLAY:
            {
                if (mOwner->mDebug) Logger::D(mOwner->mTag, "PLAY");
                mOwner->StartSound(cmd);
                break;
            }
            case CAsyncPlayer::STOP:
            {
                if (mOwner->mDebug) Logger::D(mOwner->mTag, "STOP");
                if (mOwner->mPlayer != NULL) {
                    Int64 delay = SystemClock::GetUptimeMillis() - cmd->requestTime;
                    if (delay > 1000) {
                        Logger::W(mOwner->mTag, String("Notification stop delayed by ") + StringUtils::DoubleToString(delay) + String("msecs"));
                    }
                    mOwner->mPlayer->Stop();
                    mOwner->mPlayer->ReleaseResources();
                    mOwner->mPlayer = NULL;
                }
                else {
                    Logger::W(mOwner->mTag, String("STOP command without a player"));
                }
                break;
            }
        }

        {
            Mutex::Autolock lock(mOwner->mCmdQueueLock);

            if (mOwner->mCmdQueue.IsEmpty()) {
                // nothing left to do, quit
                // doing this check after we're done prevents the case where they
                // added it during the operation from spawning two threads and
                // trying to do them in parallel.
                mOwner->mThread = NULL;
                mOwner->ReleaseWakeLock();
                return NOERROR;
            }
        }
    }
    return NOERROR;
}

//================================================================================
//                      CAsyncPlayer
//================================================================================
CAsyncPlayer::CAsyncPlayer()
    : mState(STOP)
{
}

ECode CAsyncPlayer::constructor(
    /* [in] */ const String& tag)
{
    if (!tag.IsNull()) {
        mTag = tag;
    }
    else {
        mTag = String("AsyncPlayer");
    }
    return NOERROR;
}

void CAsyncPlayer::StartSound(
    /* [in] */ Command* cmd)
{
    // Preparing can be slow, so if there is something else
    // is playing, let it continue until we're done, so there
    // is less of a glitch.
    ECode ec = NOERROR;
    do {
        if (mDebug) Logger::D(mTag, "Starting playback");

        AutoPtr<IMediaPlayer> player;
        CMediaPlayer::New((IMediaPlayer**)&player);
        ec = player->SetAudioStreamType(cmd->stream);
        if (FAILED(ec)) break;

        ec = player->SetDataSource(cmd->context, cmd->uri);
        if (FAILED(ec)) break;

        ec = player->SetLooping(cmd->looping);
        if (FAILED(ec)) break;

        ec = player->Prepare();
        if (FAILED(ec)) break;

        ec = player->Start();
        if (FAILED(ec)) break;

        if (mPlayer != NULL) {
            ec = mPlayer->ReleaseResources();
            if (FAILED(ec)) break;
        }

        mPlayer = player;
        Int64 delay = SystemClock::GetUptimeMillis() - cmd->requestTime;
        if (delay > 1000) {
            Logger::W(mTag, String("Notification sound delayed by ") +
                          StringUtils::DoubleToString(delay) + String("msecs"));
        }
    } while(0);

    if (FAILED(ec)) {
        String tempText;
        cmd->uri->ToString(&tempText);
        Logger::W(mTag, String("error loading sound for ") + tempText/*, e*/);
    }
}

ECode CAsyncPlayer::Play(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri,
    /* [in] */ Boolean looping,
    /* [in] */ Int32 stream)
{
    AutoPtr<Command> cmd = new Command();
    cmd->requestTime = SystemClock::GetUptimeMillis();
    cmd->code = PLAY;
    cmd->context = context;
    cmd->uri = uri;
    cmd->looping = looping;
    cmd->stream = stream;

    {
        Mutex::Autolock lock(mCmdQueueLock);

        EnqueueLocked(cmd);
        mState = PLAY;
    }
    return NOERROR;
}

ECode CAsyncPlayer::Stop()
{
    Mutex::Autolock lock(mCmdQueueLock);

    // This check allows stop to be called multiple times without starting
    // a thread that ends up doing nothing.
    if (mState != STOP) {
        AutoPtr<Command> cmd = new Command();
        cmd->requestTime = SystemClock::GetUptimeMillis();
        cmd->code = STOP;
        EnqueueLocked(cmd);
        mState = STOP;
    }
    return NOERROR;
}

void CAsyncPlayer::EnqueueLocked(
    /* [in] */ Command* cmd)
{
    mCmdQueue.PushBack(cmd);
    if (mThread == NULL) {
        AcquireWakeLock();
        mThread = new MyThread(this);
        mThread->Start();
    }
}

ECode CAsyncPlayer::SetUsesWakeLock(
    /* [in] */ IContext* context)
{
    if (mWakeLock != NULL || mThread != NULL) {
        // if either of these has happened, we've already played something.
        // and our releases will be out of sync.
        return E_RUNTIME_EXCEPTION;
//        throw new RuntimeException("assertion failed mWakeLock=" + mWakeLock
//                + " mThread=" + mThread);
    }

    AutoPtr<IInterface> obj;
    context->GetSystemService(IContext::POWER_SERVICE, (IInterface**)&obj);
    AutoPtr<IPowerManager> pm = IPowerManager::Probe(obj.Get());
    pm->NewWakeLock(IPowerManager::PARTIAL_WAKE_LOCK, mTag, (IPowerManagerWakeLock**)&mWakeLock);
    return NOERROR;
}

void CAsyncPlayer::AcquireWakeLock()
{
    if (mWakeLock != NULL) {
        mWakeLock->AcquireLock();
    }
}

void CAsyncPlayer::ReleaseWakeLock()
{
    if (mWakeLock != NULL) {
        mWakeLock->ReleaseLock();
    }
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
