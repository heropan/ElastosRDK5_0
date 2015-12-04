
#include "elastos/droid/internal/policy/impl/ImmersiveModeConfirmation.h"

using Elastos::Droid::Animation::EIID_IAnimatorUpdateListener;
using Elastos::Droid::Os::EIID_IHandler;
using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::View::EIID_IViewOnClickListener;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

//=====================================================================
//                     ImmersiveModeConfirmation::H
//=====================================================================
const Int32 ImmersiveModeConfirmation::H::SHOW;
const Int32 ImmersiveModeConfirmation::H::HIDE;
const Int32 ImmersiveModeConfirmation::H::PANIC;

CAR_INTERFACE_IMPL(ImmersiveModeConfirmation::H, Object, IHandler)

ImmersiveModeConfirmation::H::H(
    /* [in] */ ImmersiveModeConfirmation* host)
    :mHost(host)
{
}

ECode ImmersiveModeConfirmation::H::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);
    // ==================before translated======================
    // switch(msg.what) {
    //     case SHOW:
    //         handleShow();
    //         break;
    //     case HIDE:
    //         handleHide();
    //         break;
    //     case PANIC:
    //         handlePanic();
    //         break;
    // }
    assert(0);
    return NOERROR;
}

//=====================================================================
//  ImmersiveModeConfirmation::ClingWindowView::InnerOnClickListener1
//=====================================================================
CAR_INTERFACE_IMPL(ImmersiveModeConfirmation::ClingWindowView::InnerOnClickListener1, Object, IViewOnClickListener)
ImmersiveModeConfirmation::ClingWindowView::InnerOnClickListener1::InnerOnClickListener1(
    /* [in] */ ClingWindowView* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode ImmersiveModeConfirmation::ClingWindowView::InnerOnClickListener1::OnClick(
    /* [in] */ IView* v)
{
    VALIDATE_NOT_NULL(v);
    // ==================before translated======================
    // mConfirm.run();
    assert(0);
    return NOERROR;
}

//=====================================================================
//ImmersiveModeConfirmation::ClingWindowView::InnerValueAnimatorAnimatorUpdateListener1
//=====================================================================
CAR_INTERFACE_IMPL(ImmersiveModeConfirmation::ClingWindowView::InnerValueAnimatorAnimatorUpdateListener1, Object, IAnimatorUpdateListener)
ImmersiveModeConfirmation::ClingWindowView::InnerValueAnimatorAnimatorUpdateListener1::InnerValueAnimatorAnimatorUpdateListener1(
    /* [in] */ ClingWindowView* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode ImmersiveModeConfirmation::ClingWindowView::InnerValueAnimatorAnimatorUpdateListener1::OnAnimationUpdate(
    /* [in] */ IValueAnimator* animation)
{
    VALIDATE_NOT_NULL(animation);
    // ==================before translated======================
    // final int c = (Integer) animation.getAnimatedValue();
    // mColor.setColor(c);
    assert(0);
    return NOERROR;
}
//=====================================================================
//               ImmersiveModeConfirmation::InnerRunnable
//=====================================================================
CAR_INTERFACE_IMPL(ImmersiveModeConfirmation::ClingWindowView::InnerRunnable, Object, IRunnable)
ImmersiveModeConfirmation::ClingWindowView::InnerRunnable::InnerRunnable(
    /* [in] */ ClingWindowView* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode ImmersiveModeConfirmation::ClingWindowView::InnerRunnable::Run()
{
    // ==================before translated======================
    // if (mClingLayout != null && mClingLayout.getParent() != null) {
    //     mClingLayout.setLayoutParams(getBubbleLayoutParams());
    // }
    assert(0);
    return NOERROR;
}

//=====================================================================
//          ImmersiveModeConfirmation::InnerBroadcastReceiver
//=====================================================================
ImmersiveModeConfirmation::ClingWindowView::InnerBroadcastReceiver::InnerBroadcastReceiver(
    /* [in] */ ClingWindowView* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode ImmersiveModeConfirmation::ClingWindowView::InnerBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(intent);
    // ==================before translated======================
    // if (intent.getAction().equals(Intent.ACTION_CONFIGURATION_CHANGED)) {
    //     post(mUpdateLayoutRunnable);
    // }
    assert(0);
    return NOERROR;
}


//=====================================================================
//              ImmersiveModeConfirmation::ClingWindowView
//=====================================================================
const Int32 ImmersiveModeConfirmation::ClingWindowView::BGCOLOR;
const Int32 ImmersiveModeConfirmation::ClingWindowView::OFFSET_DP;

ImmersiveModeConfirmation::ClingWindowView::ClingWindowView(
    /* [in] */ IContext* context,
    /* [in] */ Runnable* confirm)
{
    // ==================before translated======================
    // super(context);
    // mConfirm = confirm;
    // setClickable(true);
    // setBackground(mColor);
}

ECode ImmersiveModeConfirmation::ClingWindowView::OnAttachedToWindow()
{
    // ==================before translated======================
    // super.onAttachedToWindow();
    //
    // DisplayMetrics metrics = new DisplayMetrics();
    // mWindowManager.getDefaultDisplay().getMetrics(metrics);
    // float density = metrics.density;
    //
    // // create the confirmation cling
    // mClingLayout = (ViewGroup)
    //         View.inflate(getContext(), R.layout.immersive_mode_cling, null);
    //
    // final Button ok = (Button) mClingLayout.findViewById(R.id.ok);
    // ok.setOnClickListener(new OnClickListener() {
    //     @Override
    //     public void onClick(View v) {
    //         mConfirm.run();
    //     }
    // });
    // addView(mClingLayout, getBubbleLayoutParams());
    //
    // if (ActivityManager.isHighEndGfx()) {
    //     final View bubble = mClingLayout.findViewById(R.id.text);
    //     bubble.setAlpha(0f);
    //     bubble.setTranslationY(-OFFSET_DP*density);
    //     bubble.animate()
    //             .alpha(1f)
    //             .translationY(0)
    //             .setDuration(300)
    //             .setInterpolator(new DecelerateInterpolator())
    //             .start();
    //
    //     ok.setAlpha(0f);
    //     ok.setTranslationY(-OFFSET_DP*density);
    //     ok.animate().alpha(1f)
    //             .translationY(0)
    //             .setDuration(300)
    //             .setStartDelay(200)
    //             .setInterpolator(new DecelerateInterpolator())
    //             .start();
    //
    //     mColorAnim = ValueAnimator.ofObject(new ArgbEvaluator(), 0, BGCOLOR);
    //     mColorAnim.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
    //         @Override
    //         public void onAnimationUpdate(ValueAnimator animation) {
    //             final int c = (Integer) animation.getAnimatedValue();
    //             mColor.setColor(c);
    //         }
    //     });
    //     mColorAnim.setDuration(1000);
    //     mColorAnim.start();
    // } else {
    //     mColor.setColor(BGCOLOR);
    // }
    //
    // mContext.registerReceiver(mReceiver, new IntentFilter(Intent.ACTION_CONFIGURATION_CHANGED));
    assert(0);
    return NOERROR;
}

ECode ImmersiveModeConfirmation::ClingWindowView::OnDetachedFromWindow()
{
    // ==================before translated======================
    // mContext.unregisterReceiver(mReceiver);
    assert(0);
    return NOERROR;
}

Boolean ImmersiveModeConfirmation::ClingWindowView::OnTouchEvent(
    /* [in] */ IMotionEvent* motion)
{
    // ==================before translated======================
    // return true;
    assert(0);
    return FALSE;
}

//=====================================================================
//              ImmersiveModeConfirmation::InnerRunnable1
//=====================================================================
CAR_INTERFACE_IMPL(ImmersiveModeConfirmation::InnerRunnable1, Object, IRunnable)
ImmersiveModeConfirmation::InnerRunnable1::InnerRunnable1(
    /* [in] */ ImmersiveModeConfirmation* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode ImmersiveModeConfirmation::InnerRunnable1::Run()
{
    // ==================before translated======================
    // if (DEBUG) Slog.d(TAG, "mConfirm.run()");
    // if (!mConfirmed) {
    //     mConfirmed = true;
    //     saveSetting();
    // }
    // handleHide();
    assert(0);
    return NOERROR;
}

//=====================================================================
//                      ImmersiveModeConfirmation
//=====================================================================
const String ImmersiveModeConfirmation::TAG("ImmersiveModeConfirmation");
const Boolean ImmersiveModeConfirmation::DEBUG = false;
const Boolean ImmersiveModeConfirmation::DEBUG_SHOW_EVERY_TIME = false;
const String ImmersiveModeConfirmation::CONFIRMED("confirmed");

CAR_INTERFACE_IMPL(ImmersiveModeConfirmation, Object, IImmersiveModeConfirmation)

ImmersiveModeConfirmation::ImmersiveModeConfirmation()
{
}

ECode ImmersiveModeConfirmation::constructor(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // mContext = context;
    // mHandler = new H();
    // mShowDelayMs = getNavBarExitDuration() * 3;
    // mPanicThresholdMs = context.getResources()
    //         .getInteger(R.integer.config_immersive_mode_confirmation_panic);
    // mWindowManager = (WindowManager)
    //         mContext.getSystemService(Context.WINDOW_SERVICE);
    return NOERROR;
}

ECode ImmersiveModeConfirmation::LoadSetting(
    /* [in] */ Int32 currentUserId)
{
    // ==================before translated======================
    // mConfirmed = false;
    // mCurrentUserId = currentUserId;
    // if (DEBUG) Slog.d(TAG, String.format("loadSetting() mCurrentUserId=%d resetForPanic=%s",
    //         mCurrentUserId, mUserPanicResets.get(mCurrentUserId, false)));
    // String value = null;
    // try {
    //     value = Settings.Secure.getStringForUser(mContext.getContentResolver(),
    //             Settings.Secure.IMMERSIVE_MODE_CONFIRMATIONS,
    //             UserHandle.USER_CURRENT);
    //     mConfirmed = CONFIRMED.equals(value);
    //     if (DEBUG) Slog.d(TAG, "Loaded mConfirmed=" + mConfirmed);
    // } catch (Throwable t) {
    //     Slog.w(TAG, "Error loading confirmations, value=" + value, t);
    // }
    assert(0);
    return NOERROR;
}

ECode ImmersiveModeConfirmation::ImmersiveModeChanged(
    /* [in] */ const String& pkg,
    /* [in] */ Boolean isImmersiveMode,
    /* [in] */ Boolean userSetupComplete)
{
    // ==================before translated======================
    // mHandler.removeMessages(H.SHOW);
    // if (isImmersiveMode) {
    //     final boolean disabled = PolicyControl.disableImmersiveConfirmation(pkg);
    //     if (DEBUG) Slog.d(TAG, String.format("immersiveModeChanged() disabled=%s mConfirmed=%s",
    //             disabled, mConfirmed));
    //     if (!disabled && (DEBUG_SHOW_EVERY_TIME || !mConfirmed) && userSetupComplete) {
    //         mHandler.sendEmptyMessageDelayed(H.SHOW, mShowDelayMs);
    //     }
    // } else {
    //     mHandler.sendEmptyMessage(H.HIDE);
    // }
    assert(0);
    return NOERROR;
}

ECode ImmersiveModeConfirmation::OnPowerKeyDown(
    /* [in] */ Boolean isScreenOn,
    /* [in] */ Int64 time,
    /* [in] */ Boolean inImmersiveMode,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (!isScreenOn && (time - mPanicTime < mPanicThresholdMs)) {
    //     // turning the screen back on within the panic threshold
    //     mHandler.sendEmptyMessage(H.PANIC);
    //     return mClingWindow == null;
    // }
    // if (isScreenOn && inImmersiveMode) {
    //     // turning the screen off, remember if we were in immersive mode
    //     mPanicTime = time;
    // } else {
    //     mPanicTime = 0;
    // }
    // return false;
    assert(0);
    return NOERROR;
}

ECode ImmersiveModeConfirmation::ConfirmCurrentPrompt()
{
    // ==================before translated======================
    // if (mClingWindow != null) {
    //     if (DEBUG) Slog.d(TAG, "confirmCurrentPrompt()");
    //     mHandler.post(mConfirm);
    // }
    assert(0);
    return NOERROR;
}

ECode ImmersiveModeConfirmation::GetClingWindowLayoutParams(
    /* [out] */ IWindowManagerLayoutParams** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // final WindowManager.LayoutParams lp = new WindowManager.LayoutParams(
    //         ViewGroup.LayoutParams.MATCH_PARENT,
    //         ViewGroup.LayoutParams.MATCH_PARENT,
    //         WindowManager.LayoutParams.TYPE_TOAST,
    //         0
    //                 | WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN
    //                 | WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE
    //                 | WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED
    //         ,
    //         PixelFormat.TRANSLUCENT);
    // lp.privateFlags |= WindowManager.LayoutParams.PRIVATE_FLAG_SHOW_FOR_ALL_USERS;
    // lp.setTitle("ImmersiveModeConfirmation");
    // lp.windowAnimations = com.android.internal.R.style.Animation_RecentApplications;
    // lp.gravity = Gravity.FILL;
    // return lp;
    assert(0);
    return NOERROR;
}

ECode ImmersiveModeConfirmation::GetBubbleLayoutParams(
    /* [out] */ IFrameLayoutLayoutParams** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return new FrameLayout.LayoutParams(
    //         mContext.getResources().getDimensionPixelSize(
    //                 R.dimen.immersive_mode_cling_width),
    //         ViewGroup.LayoutParams.WRAP_CONTENT,
    //         Gravity.CENTER_HORIZONTAL | Gravity.TOP);
    assert(0);
    return NOERROR;
}

Int64 ImmersiveModeConfirmation::GetNavBarExitDuration()
{
    // ==================before translated======================
    // Animation exit = AnimationUtils.loadAnimation(mContext, R.anim.dock_bottom_exit);
    // return exit != null ? exit.getDuration() : 0;
    assert(0);
    return 0;
}

void ImmersiveModeConfirmation::SaveSetting()
{
    // ==================before translated======================
    // if (DEBUG) Slog.d(TAG, "saveSetting()");
    // try {
    //     final String value = mConfirmed ? CONFIRMED : null;
    //     Settings.Secure.putStringForUser(mContext.getContentResolver(),
    //             Settings.Secure.IMMERSIVE_MODE_CONFIRMATIONS,
    //             value,
    //             UserHandle.USER_CURRENT);
    //     if (DEBUG) Slog.d(TAG, "Saved value=" + value);
    // } catch (Throwable t) {
    //     Slog.w(TAG, "Error saving confirmations, mConfirmed=" + mConfirmed, t);
    // }
    assert(0);
}

void ImmersiveModeConfirmation::HandlePanic()
{
    // ==================before translated======================
    // if (DEBUG) Slog.d(TAG, "handlePanic()");
    // if (mUserPanicResets.get(mCurrentUserId, false)) return;  // already reset for panic
    // mUserPanicResets.put(mCurrentUserId, true);
    // mConfirmed = false;
    // saveSetting();
    assert(0);
}

void ImmersiveModeConfirmation::HandleHide()
{
    // ==================before translated======================
    // if (mClingWindow != null) {
    //     if (DEBUG) Slog.d(TAG, "Hiding immersive mode confirmation");
    //     mWindowManager.removeView(mClingWindow);
    //     mClingWindow = null;
    // }
    assert(0);
}

void ImmersiveModeConfirmation::HandleShow()
{
    // ==================before translated======================
    // if (DEBUG) Slog.d(TAG, "Showing immersive mode confirmation");
    //
    // mClingWindow = new ClingWindowView(mContext, mConfirm);
    //
    // // we will be hiding the nav bar, so layout as if it's already hidden
    // mClingWindow.setSystemUiVisibility(
    //         View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
    //       | View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
    //
    // // show the confirmation
    // WindowManager.LayoutParams lp = getClingWindowLayoutParams();
    // mWindowManager.addView(mClingWindow, lp);
    assert(0);
}

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos


