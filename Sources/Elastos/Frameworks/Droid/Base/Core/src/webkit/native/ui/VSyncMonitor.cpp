
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

const Int32 VSyncMonitor::MAX_AUTO_ONVSYNC_COUNT;
const Int64 VSyncMonitor::NANOSECONDS_PER_SECOND;
const Int64 VSyncMonitor::NANOSECONDS_PER_MILLISECOND;
const Int64 VSyncMonitor::NANOSECONDS_PER_MICROSECOND;

//===============================================================
// 		VSyncMonitor::InnerChoreographerFrameCallback
//===============================================================
VSyncMonitor::InnerChoreographerFrameCallback::InnerChoreographerFrameCallback(
    /* in */ VSyncMonitor* owner)
    : mOwner(owner)
{
}

//@Override
ECode VSyncMonitor::InnerChoreographerFrameCallback::DoFrame(
    /* in */ Int64 frameTimeNanos)
{
    CTraceEvent::Begin("VSync");
    mGoodStartingPointNano = frameTimeNanos;
    mOwner->OnVSyncCallback(frameTimeNanos, mOwner->GetCurrentNanoTime());
    CTraceEvent::End("VSync");
    return NOERROR;
}

//===============================================================
// 					VSyncMonitor::InnerVSyncRunnable
//===============================================================
VSyncMonitor::InnerVSyncRunnable::InnerVSyncRunnable(
    /* in */ VSyncMonitor* owner)
    : mOwner(owner)
{
}

//@Override
ECode VSyncMonitor::InnerVSyncRunnable::Run()
{
    CTraceEvent::Begin("VSyncTimer");
    const Int64 currentTime = mOwner->GetCurrentNanoTime();
    mOwner->OnVSyncCallback(currentTime, currentTime);
    CTraceEvent::End("VSyncTimer");
    return NOERROR;
}

//===============================================================
// 					VSyncMonitor::SyntheticVSyncRunnable
//===============================================================
VSyncMonitor::SyntheticVSyncRunnable::SyntheticVSyncRunnable(
    /* in */ VSyncMonitor* owner)
    : mOwner(owner)
{
}

//@Override
ECode VSyncMonitor::SyntheticVSyncRunnable::Run()
{
    CTraceEvent::Begin("VSyncSynthetic");
    const Int64 currentTime = mOwner->GetCurrentNanoTime();
    mOwner->OnVSyncCallback(mOwner->EstimateLastVSyncTime(currentTime), currentTime);
    CTraceEvent::End("VSyncSynthetic");
    return NOERROR;
}

//===============================================================
// 		                    VSyncMonitor
//===============================================================
VSyncMonitor::VSyncMonitor(
    /* in */ IContext* context,
    /* in */ VSyncMonitor::Listener* listener)
{
    VSyncMonitor(context, listener, TRUE);
}

VSyncMonitor::VSyncMonitor(
    /* in */ IContext* context,
    /* in */ VSyncMonitor::Listener* listener,
    /* in */ Boolean enableJBVSync)
    : mListener(listener)
    , mHandler(new Handler())
    , mRefreshPeriodNano(0)
    , mHaveRequestInFlight(FALSE)
    , mTriggerNextVSyncCount(0)
    , mGoodStartingPointNano(0)
    , mLastPostedNano(0)
    , mLastVSyncCpuTimeNano(0)
{
	AutoPtr<IWindowManager> winManager;
	context->GetSystemService(IContext::WINDOW_SERVICE, (IWindowManager**)&winManager);
	AutoPtr<IDisplay> defaultDisplay;
	winManager->GetDefaultDisplay((IDisplay**)&defaultDisplay);
    Float refreshRate;
    defaultDisplay->GetRefreshRate(&refreshRate);

    if (refreshRate <= 0)
    	refreshRate = 60;

    mRefreshPeriodNano = (Int64) (NANOSECONDS_PER_SECOND / refreshRate);
    mTriggerNextVSyncCount = 0;

    if (enableJBVSync && IBuild::VERSION::SDK_INT >= IBuild::VERSION_CODES::JELLY_BEAN) {
        // Use Choreographer on JB+ to get notified of vsync.
        CChoreographer::New((IChoreographer**)&mChoreographer);
        CChoreographer::New((IChoreographer::IFrameCallback**)&mChoreographer);

        mVSyncFrameCallback = new VSyncMonitor::InnerChoreographerFrameCallback(this);
        mVSyncRunnableCallback = NULL;
    }
    else {
        // On ICS we just hope that running tasks is relatively predictable.
        mChoreographer = NULL;
        mVSyncFrameCallback = NULL;
        mVSyncRunnableCallback = new InnerVSyncRunnable(this);
        mLastPostedNano = 0;
    }
    mSyntheticVSyncRunnable = new SyntheticVSyncRunnable(this);
    mGoodStartingPointNano = GetCurrentNanoTime();
}

Int64 VSyncMonitor::GetVSyncPeriodInMicroseconds()
{
    return mRefreshPeriodNano / NANOSECONDS_PER_MICROSECOND;
}

ECode VSyncMonitor::Stop()
{
    mTriggerNextVSyncCount = 0;
    return NOERROR;
}

ECode VSyncMonitor::RequestUpdate()
{
    mTriggerNextVSyncCount = MAX_AUTO_ONVSYNC_COUNT;
    PostCallback();
    return NOERROR;
}

ECode VSyncMonitor::SetVSyncPointForICS(
    /* in */ Int64 goodStartingPointNano)
{
    mGoodStartingPointNano = goodStartingPointNano;
    return NOERROR;
}

Boolean VSyncMonitor::IsVSyncSignalAvailable()
{
    return (Boolean)(mChoreographer != NULL);
}

Int64 VSyncMonitor::GetCurrentNanoTime()
{
    return System::NanoTime();
}

ECode VSyncMonitor::OnVSyncCallback(
    /* in */ Int64 frameTimeNanos,
    /* in */ Int64 currentTimeNanos)
{
    assert (mHaveRequestInFlight);
    mHaveRequestInFlight = FALSE;
    mLastVSyncCpuTimeNano = currentTimeNanos;
    if (mTriggerNextVSyncCount >= 0) {
        mTriggerNextVSyncCount--;
        PostCallback();
    }
    if (mListener != NULL) {
        mListener->OnVSync(this, frameTimeNanos / NANOSECONDS_PER_MICROSECOND);
    }
    return NOERROR;
}

ECode VSyncMonitor::PostCallback()
{
    if (mHaveRequestInFlight)
    	return NOERROR;

    mHaveRequestInFlight = TRUE;
    if (postSyntheticVSync())
    	return NOERROR;

    if (IsVSyncSignalAvailable()) {
        mChoreographer->PostFrameCallback(mVSyncFrameCallback);
    }
    else {
        PostRunnableCallback();
    }
    return NOERROR;
}

Boolean VSyncMonitor::PostSyntheticVSync()
{
    const Int64 currentTime = GetCurrentNanoTime();
    // Only trigger a synthetic vsync if we've been idle for long enough and the upcoming real
    // vsync is more than half a frame away.
    if (currentTime - mLastVSyncCpuTimeNano < 2 * mRefreshPeriodNano) return FALSE;
    if (currentTime - EstimateLastVSyncTime(currentTime) > mRefreshPeriodNano / 2) return FALSE;
    mHandler->Post(mSyntheticVSyncRunnable);
    return TRUE;
}

Int64 VSyncMonitor::EstimateLastVSyncTime(
    /* in */ Int64 currentTime)
{
    const Int64 lastRefreshTime = mGoodStartingPointNano +
            ((currentTime - mGoodStartingPointNano) / mRefreshPeriodNano) * mRefreshPeriodNano;
    return lastRefreshTime;
}

ECode VSyncMonitor::PostRunnableCallback()
{
    assert (!IsVSyncSignalAvailable());
    const Int64 currentTime = GetCurrentNanoTime();
    const Int64 lastRefreshTime = EstimateLastVSyncTime(currentTime);
    Int64 delay = (lastRefreshTime + mRefreshPeriodNano) - currentTime;
    assert (delay > 0 && delay <= mRefreshPeriodNano);

    if (currentTime + delay <= mLastPostedNano + mRefreshPeriodNano / 2) {
        delay += mRefreshPeriodNano;
    }

    mLastPostedNano = currentTime + delay;
    if (delay == 0)
    	mHandler->Post(mVSyncRunnableCallback);
    else
    	mHandler->PostDelayed(mVSyncRunnableCallback, delay / NANOSECONDS_PER_MILLISECOND);

    return NOERROR;
}


} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

