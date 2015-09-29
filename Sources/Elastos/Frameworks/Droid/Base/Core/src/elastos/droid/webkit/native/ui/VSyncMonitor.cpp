
#include "elastos/droid/webkit/native/ui/VSyncMonitor.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

//=====================================================================
//            VSyncMonitor::InnerChoreographerFrameCallback
//=====================================================================
VSyncMonitor::InnerChoreographerFrameCallback::InnerChoreographerFrameCallback(
    /* [in] */ VSyncMonitor* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode VSyncMonitor::InnerChoreographerFrameCallback::DoFrame(
    /* [in] */ Int64 frameTimeNanos)
{
    // ==================before translated======================
    // TraceEvent.begin("VSync");
    // mGoodStartingPointNano = frameTimeNanos;
    // onVSyncCallback(frameTimeNanos, getCurrentNanoTime());
    // TraceEvent.end("VSync");
    assert(0);
    return NOERROR;
}

//=====================================================================
//                     VSyncMonitor::InnerRunnable
//=====================================================================
VSyncMonitor::InnerRunnable::InnerRunnable(
    /* [in] */ VSyncMonitor* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode VSyncMonitor::InnerRunnable::Run()
{
    // ==================before translated======================
    // TraceEvent.begin("VSyncTimer");
    // final long currentTime = getCurrentNanoTime();
    // onVSyncCallback(currentTime, currentTime);
    // TraceEvent.end("VSyncTimer");
    assert(0);
    return NOERROR;
}

//=====================================================================
//                     VSyncMonitor::InnerRunnable1
//=====================================================================
VSyncMonitor::InnerRunnable1::InnerRunnable1(
    /* [in] */ VSyncMonitor* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode VSyncMonitor::InnerRunnable1::Run()
{
    // ==================before translated======================
    // TraceEvent.begin("VSyncSynthetic");
    // final long currentTime = getCurrentNanoTime();
    // onVSyncCallback(estimateLastVSyncTime(currentTime), currentTime);
    // TraceEvent.end("VSyncSynthetic");
    assert(0);
    return NOERROR;
}

//=====================================================================
//                             VSyncMonitor
//=====================================================================
const Int32 VSyncMonitor::MAX_AUTO_ONVSYNC_COUNT;
const Int64 VSyncMonitor::NANOSECONDS_PER_SECOND;
const Int64 VSyncMonitor::NANOSECONDS_PER_MILLISECOND;
const Int64 VSyncMonitor::NANOSECONDS_PER_MICROSECOND;

VSyncMonitor::VSyncMonitor(
    /* [in] */ IContext* context,
    /* [in] */ Listener* listener)
{
    // ==================before translated======================
    // this(context, listener, true);
}

VSyncMonitor::VSyncMonitor(
    /* [in] */ IContext* context,
    /* [in] */ Listener* listener,
    /* [in] */ Boolean enableJBVSync)
{
    // ==================before translated======================
    // mListener = listener;
    // float refreshRate = ((WindowManager) context.getSystemService(Context.WINDOW_SERVICE))
    //         .getDefaultDisplay().getRefreshRate();
    // if (refreshRate <= 0) refreshRate = 60;
    // mRefreshPeriodNano = (long) (NANOSECONDS_PER_SECOND / refreshRate);
    // mTriggerNextVSyncCount = 0;
    //
    // if (enableJBVSync && Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
    //     // Use Choreographer on JB+ to get notified of vsync.
    //     mChoreographer = Choreographer.getInstance();
    //     mVSyncFrameCallback = new Choreographer.FrameCallback() {
    //         @Override
    //         public void doFrame(long frameTimeNanos) {
    //             TraceEvent.begin("VSync");
    //             mGoodStartingPointNano = frameTimeNanos;
    //             onVSyncCallback(frameTimeNanos, getCurrentNanoTime());
    //             TraceEvent.end("VSync");
    //         }
    //     };
    //     mVSyncRunnableCallback = null;
    // } else {
    //     // On ICS we just hope that running tasks is relatively predictable.
    //     mChoreographer = null;
    //     mVSyncFrameCallback = null;
    //     mVSyncRunnableCallback = new Runnable() {
    //         @Override
    //         public void run() {
    //             TraceEvent.begin("VSyncTimer");
    //             final long currentTime = getCurrentNanoTime();
    //             onVSyncCallback(currentTime, currentTime);
    //             TraceEvent.end("VSyncTimer");
    //         }
    //     };
    //     mLastPostedNano = 0;
    // }
    // mSyntheticVSyncRunnable = new Runnable() {
    //     @Override
    //     public void run() {
    //         TraceEvent.begin("VSyncSynthetic");
    //         final long currentTime = getCurrentNanoTime();
    //         onVSyncCallback(estimateLastVSyncTime(currentTime), currentTime);
    //         TraceEvent.end("VSyncSynthetic");
    //     }
    // };
    // mGoodStartingPointNano = getCurrentNanoTime();
}

Int64 VSyncMonitor::GetVSyncPeriodInMicroseconds()
{
    // ==================before translated======================
    // return mRefreshPeriodNano / NANOSECONDS_PER_MICROSECOND;
    assert(0);
    return 0;
}

ECode VSyncMonitor::Stop()
{
    // ==================before translated======================
    // mTriggerNextVSyncCount = 0;
    assert(0);
    return NOERROR;
}

ECode VSyncMonitor::RequestUpdate()
{
    // ==================before translated======================
    // mTriggerNextVSyncCount = MAX_AUTO_ONVSYNC_COUNT;
    // postCallback();
    assert(0);
    return NOERROR;
}

ECode VSyncMonitor::SetVSyncPointForICS(
    /* [in] */ Int64 goodStartingPointNano)
{
    // ==================before translated======================
    // mGoodStartingPointNano = goodStartingPointNano;
    assert(0);
    return NOERROR;
}

Boolean VSyncMonitor::IsVSyncSignalAvailable()
{
    // ==================before translated======================
    // return mChoreographer != null;
    assert(0);
    return FALSE;
}

Int64 VSyncMonitor::GetCurrentNanoTime()
{
    // ==================before translated======================
    // return System.nanoTime();
    assert(0);
    return 0;
}

ECode VSyncMonitor::OnVSyncCallback(
    /* [in] */ Int64 frameTimeNanos,
    /* [in] */ Int64 currentTimeNanos)
{
    // ==================before translated======================
    // assert mHaveRequestInFlight;
    // mHaveRequestInFlight = false;
    // mLastVSyncCpuTimeNano = currentTimeNanos;
    // if (mTriggerNextVSyncCount >= 0) {
    //     mTriggerNextVSyncCount--;
    //     postCallback();
    // }
    // if (mListener != null) {
    //     mListener.onVSync(this, frameTimeNanos / NANOSECONDS_PER_MICROSECOND);
    // }
    assert(0);
    return NOERROR;
}

ECode VSyncMonitor::PostCallback()
{
    // ==================before translated======================
    // if (mHaveRequestInFlight) return;
    // mHaveRequestInFlight = true;
    // if (postSyntheticVSync()) return;
    // if (isVSyncSignalAvailable()) {
    //     mChoreographer.postFrameCallback(mVSyncFrameCallback);
    // } else {
    //     postRunnableCallback();
    // }
    assert(0);
    return NOERROR;
}

Boolean VSyncMonitor::PostSyntheticVSync()
{
    // ==================before translated======================
    // final long currentTime = getCurrentNanoTime();
    // // Only trigger a synthetic vsync if we've been idle for long enough and the upcoming real
    // // vsync is more than half a frame away.
    // if (currentTime - mLastVSyncCpuTimeNano < 2 * mRefreshPeriodNano) return false;
    // if (currentTime - estimateLastVSyncTime(currentTime) > mRefreshPeriodNano / 2) return false;
    // mHandler.post(mSyntheticVSyncRunnable);
    // return true;
    assert(0);
    return FALSE;
}

Int64 VSyncMonitor::EstimateLastVSyncTime(
    /* [in] */ Int64 currentTime)
{
    // ==================before translated======================
    // final long lastRefreshTime = mGoodStartingPointNano +
    //         ((currentTime - mGoodStartingPointNano) / mRefreshPeriodNano) * mRefreshPeriodNano;
    // return lastRefreshTime;
    assert(0);
    return 0;
}

ECode VSyncMonitor::PostRunnableCallback()
{
    // ==================before translated======================
    // assert !isVSyncSignalAvailable();
    // final long currentTime = getCurrentNanoTime();
    // final long lastRefreshTime = estimateLastVSyncTime(currentTime);
    // long delay = (lastRefreshTime + mRefreshPeriodNano) - currentTime;
    // assert delay > 0 && delay <= mRefreshPeriodNano;
    //
    // if (currentTime + delay <= mLastPostedNano + mRefreshPeriodNano / 2) {
    //     delay += mRefreshPeriodNano;
    // }
    //
    // mLastPostedNano = currentTime + delay;
    // if (delay == 0) mHandler.post(mVSyncRunnableCallback);
    // else mHandler.postDelayed(mVSyncRunnableCallback, delay / NANOSECONDS_PER_MILLISECOND);
    assert(0);
    return NOERROR;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


