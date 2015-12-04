
#include "elastos/droid/internal/policy/impl/GlobalActions.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

//=====================================================================
//                      GlobalActions::PowerAction
//=====================================================================
Boolean GlobalActions::PowerAction::OnLongPress()
{
    // ==================before translated======================
    // mWindowManagerFuncs.rebootSafeMode(true);
    // return true;
    assert(0);
    return FALSE;
}

Boolean GlobalActions::PowerAction::ShowDuringKeyguard()
{
    // ==================before translated======================
    // return true;
    assert(0);
    return FALSE;
}

Boolean GlobalActions::PowerAction::ShowBeforeProvisioning()
{
    // ==================before translated======================
    // return true;
    assert(0);
    return FALSE;
}

ECode GlobalActions::PowerAction::OnPress()
{
    // ==================before translated======================
    // // shutdown by making sure radio and power are handled accordingly.
    // mWindowManagerFuncs.shutdown(false /* confirm */);
    assert(0);
    return NOERROR;
}

GlobalActions::PowerAction::PowerAction()
{
    // ==================before translated======================
    // super(com.android.internal.R.drawable.ic_lock_power_off,
    //     R.string.global_action_power_off);
}

//=====================================================================
//          GlobalActions::GlobalActionsDialog::InnerRunnable3
//=====================================================================
GlobalActions::GlobalActionsDialog::InnerRunnable3::InnerRunnable3(
    /* [in] */ GlobalActionsDialog* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode GlobalActions::GlobalActionsDialog::InnerRunnable3::Run()
{
    // ==================before translated======================
    // dismiss();
    assert(0);
    return NOERROR;
}

//=====================================================================
//                  GlobalActions::GlobalActionsDialog
//=====================================================================
GlobalActions::GlobalActionsDialog::GlobalActionsDialog(
    /* [in] */ IContext* context,
    /* [in] */ IAlertParams* params)
{
    // ==================before translated======================
    // super(context, getDialogTheme(context));
    // mContext = context;
    // mAlert = new AlertController(mContext, this, getWindow());
    // mAdapter = (MyAdapter) params.mAdapter;
    // mWindowTouchSlop = ViewConfiguration.get(context).getScaledWindowTouchSlop();
    // params.apply(mAlert);
}

Boolean GlobalActions::GlobalActionsDialog::DispatchTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    // ==================before translated======================
    // if (mEnableAccessibilityController != null) {
    //     final int action = event.getActionMasked();
    //     if (action == MotionEvent.ACTION_DOWN) {
    //         View decor = getWindow().getDecorView();
    //         final int eventX = (int) event.getX();
    //         final int eventY = (int) event.getY();
    //         if (eventX < -mWindowTouchSlop
    //                 || eventY < -mWindowTouchSlop
    //                 || eventX >= decor.getWidth() + mWindowTouchSlop
    //                 || eventY >= decor.getHeight() + mWindowTouchSlop) {
    //             mCancelOnUp = true;
    //         }
    //     }
    //     try {
    //         if (!mIntercepted) {
    //             mIntercepted = mEnableAccessibilityController.onInterceptTouchEvent(event);
    //             if (mIntercepted) {
    //                 final long now = SystemClock.uptimeMillis();
    //                 event = MotionEvent.obtain(now, now,
    //                         MotionEvent.ACTION_CANCEL, 0.0f, 0.0f, 0);
    //                 event.setSource(InputDevice.SOURCE_TOUCHSCREEN);
    //                 mCancelOnUp = true;
    //             }
    //         } else {
    //             return mEnableAccessibilityController.onTouchEvent(event);
    //         }
    //     } finally {
    //         if (action == MotionEvent.ACTION_UP) {
    //             if (mCancelOnUp) {
    //                 cancel();
    //             }
    //             mCancelOnUp = false;
    //             mIntercepted = false;
    //         }
    //     }
    // }
    // return super.dispatchTouchEvent(event);
    assert(0);
    return FALSE;
}

ECode GlobalActions::GlobalActionsDialog::GetListView(
    /* [out] */ ListView** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAlert.getListView();
    assert(0);
    return NOERROR;
}

Boolean GlobalActions::GlobalActionsDialog::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    // ==================before translated======================
    // if (event.getEventType() == AccessibilityEvent.TYPE_WINDOW_STATE_CHANGED) {
    //     for (int i = 0; i < mAdapter.getCount(); ++i) {
    //         CharSequence label =
    //                 mAdapter.getItem(i).getLabelForAccessibility(getContext());
    //         if (label != null) {
    //             event.getText().add(label);
    //         }
    //     }
    // }
    // return super.dispatchPopulateAccessibilityEvent(event);
    assert(0);
    return FALSE;
}

Boolean GlobalActions::GlobalActionsDialog::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    // ==================before translated======================
    // if (mAlert.onKeyDown(keyCode, event)) {
    //     return true;
    // }
    // return super.onKeyDown(keyCode, event);
    assert(0);
    return FALSE;
}

Boolean GlobalActions::GlobalActionsDialog::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    // ==================before translated======================
    // if (mAlert.onKeyUp(keyCode, event)) {
    //     return true;
    // }
    // return super.onKeyUp(keyCode, event);
    assert(0);
    return FALSE;
}

void GlobalActions::GlobalActionsDialog::OnStart()
{
    // ==================before translated======================
    // // If global accessibility gesture can be performed, we will take care
    // // of dismissing the dialog on touch outside. This is because the dialog
    // // is dismissed on the first down while the global gesture is a long press
    // // with two fingers anywhere on the screen.
    // if (EnableAccessibilityController.canEnableAccessibilityViaGesture(mContext)) {
    //     mEnableAccessibilityController = new EnableAccessibilityController(mContext,
    //             new Runnable() {
    //         @Override
    //         public void run() {
    //             dismiss();
    //         }
    //     });
    //     super.setCanceledOnTouchOutside(false);
    // } else {
    //     mEnableAccessibilityController = null;
    //     super.setCanceledOnTouchOutside(true);
    // }
    //
    // super.onStart();
    assert(0);
}

void GlobalActions::GlobalActionsDialog::OnStop()
{
    // ==================before translated======================
    // if (mEnableAccessibilityController != null) {
    //     mEnableAccessibilityController.onDestroy();
    // }
    // super.onStop();
    assert(0);
}

void GlobalActions::GlobalActionsDialog::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    // ==================before translated======================
    // super.onCreate(savedInstanceState);
    // mAlert.installContent();
    assert(0);
}

Int32 GlobalActions::GlobalActionsDialog::GetDialogTheme(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // TypedValue outValue = new TypedValue();
    // context.getTheme().resolveAttribute(com.android.internal.R.attr.alertDialogTheme,
    //         outValue, true);
    // return outValue.resourceId;
    assert(0);
    return 0;
}

//=====================================================================
//                     GlobalActions::InnerIntent1
//=====================================================================
GlobalActions::InnerIntent1::InnerIntent1(
    /* [in] */ GlobalActions* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

GlobalActions::InnerIntent1::ChangeAirplaneModeSystemSetting(
    /* [in] */  on)
{
}

//=====================================================================
// GlobalActions::InnerDialogInterfaceOnClickListener1::InnerRunnable1
//=====================================================================
GlobalActions::InnerDialogInterfaceOnClickListener1::InnerRunnable1::InnerRunnable1(
    /* [in] */ InnerDialogInterfaceOnClickListener1* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

GlobalActions::InnerDialogInterfaceOnClickListener1::InnerRunnable1::Catch(
    /* [in] */ IRemoteException* e)
{
}

//=====================================================================
//         GlobalActions::InnerDialogInterfaceOnClickListener1
//=====================================================================
GlobalActions::InnerDialogInterfaceOnClickListener1::InnerDialogInterfaceOnClickListener1(
    /* [in] */ GlobalActions* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode GlobalActions::InnerDialogInterfaceOnClickListener1::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    VALIDATE_NOT_NULL(dialog);
    // ==================before translated======================
    // // don't actually trigger the bugreport if we are running stability
    // // tests via monkey
    // if (ActivityManager.isUserAMonkey()) {
    //     return;
    // }
    // // Add a little delay before executing, to give the
    // // dialog a chance to go away before it takes a
    // // screenshot.
    // mHandler.postDelayed(new Runnable() {
    //     @Override public void run() {
    //         try {
    //             ActivityManagerNative.getDefault()
    //                     .requestBugReport();
    //         } catch (RemoteException e) {
    //         }
    //     }
    // }, 500);
    assert(0);
    return NOERROR;
}

//=====================================================================
//                GlobalActions::InnerSinglePressAction1
//=====================================================================
GlobalActions::InnerSinglePressAction1::InnerSinglePressAction1(
    /* [in] */ GlobalActions* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode GlobalActions::InnerSinglePressAction1::OnPress()
{
    // ==================before translated======================
    // Intent intent = new Intent(Settings.ACTION_SETTINGS);
    // intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP);
    // mContext.startActivity(intent);
    assert(0);
    return NOERROR;
}

Boolean GlobalActions::InnerSinglePressAction1::ShowDuringKeyguard()
{
    // ==================before translated======================
    // return true;
    assert(0);
    return FALSE;
}

Boolean GlobalActions::InnerSinglePressAction1::ShowBeforeProvisioning()
{
    // ==================before translated======================
    // return true;
    assert(0);
    return FALSE;
}

//=====================================================================
//                GlobalActions::InnerSinglePressAction3
//=====================================================================
GlobalActions::InnerSinglePressAction3::InnerSinglePressAction3(
    /* [in] */ GlobalActions* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode GlobalActions::InnerSinglePressAction3::OnPress()
{
    // ==================before translated======================
    // new LockPatternUtils(mContext).requireCredentialEntry(UserHandle.USER_ALL);
    // try {
    //     WindowManagerGlobal.getWindowManagerService().lockNow(null);
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Error while trying to lock device.", e);
    // }
    assert(0);
    return NOERROR;
}

Boolean GlobalActions::InnerSinglePressAction3::ShowDuringKeyguard()
{
    // ==================before translated======================
    // return true;
    assert(0);
    return FALSE;
}

Boolean GlobalActions::InnerSinglePressAction3::ShowBeforeProvisioning()
{
    // ==================before translated======================
    // return false;
    assert(0);
    return FALSE;
}

//=====================================================================
//                       GlobalActions::MyAdapter
//=====================================================================
Int32 GlobalActions::MyAdapter::GetCount()
{
    // ==================before translated======================
    // int count = 0;
    //
    // for (int i = 0; i < mItems.size(); i++) {
    //     final Action action = mItems.get(i);
    //
    //     if (mKeyguardShowing && !action.showDuringKeyguard()) {
    //         continue;
    //     }
    //     if (!mDeviceProvisioned && !action.showBeforeProvisioning()) {
    //         continue;
    //     }
    //     count++;
    // }
    // return count;
    assert(0);
    return 0;
}

Boolean GlobalActions::MyAdapter::IsEnabled(
    /* [in] */ Int32 position)
{
    // ==================before translated======================
    // return getItem(position).isEnabled();
    assert(0);
    return FALSE;
}

Boolean GlobalActions::MyAdapter::AreAllItemsEnabled()
{
    // ==================before translated======================
    // return false;
    assert(0);
    return FALSE;
}

ECode GlobalActions::MyAdapter::GetItem(
    /* [in] */ Int32 position,
    /* [out] */ Action** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    //
    // int filteredPos = 0;
    // for (int i = 0; i < mItems.size(); i++) {
    //     final Action action = mItems.get(i);
    //     if (mKeyguardShowing && !action.showDuringKeyguard()) {
    //         continue;
    //     }
    //     if (!mDeviceProvisioned && !action.showBeforeProvisioning()) {
    //         continue;
    //     }
    //     if (filteredPos == position) {
    //         return action;
    //     }
    //     filteredPos++;
    // }
    //
    // throw new IllegalArgumentException("position " + position
    //         + " out of range of showable actions"
    //         + ", filtered count=" + getCount()
    //         + ", keyguardshowing=" + mKeyguardShowing
    //         + ", provisioned=" + mDeviceProvisioned);
    assert(0);
    return NOERROR;
}

ECode GlobalActions::MyAdapter::GetItemId(
    /* [in] */ Int32 position,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return position;
    assert(0);
    return NOERROR;
}

ECode GlobalActions::MyAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ View** result)
{
    VALIDATE_NOT_NULL(convertView);
    VALIDATE_NOT_NULL(parent);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // Action action = getItem(position);
    // return action.create(mContext, convertView, parent, LayoutInflater.from(mContext));
    assert(0);
    return NOERROR;
}

//=====================================================================
//                   GlobalActions::SinglePressAction
//=====================================================================
ECode GlobalActions::SinglePressAction::IsEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return true;
    assert(0);
    return NOERROR;
}

ECode GlobalActions::SinglePressAction::GetStatus(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return null;
    assert(0);
    return NOERROR;
}

ECode GlobalActions::SinglePressAction::GetLabelForAccessibility(
    /* [in] */ IContext* context,
    /* [out] */ CharSequence** result)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (mMessage != null) {
    //     return mMessage;
    // } else {
    //     return context.getString(mMessageResId);
    // }
    assert(0);
    return NOERROR;
}

ECode GlobalActions::SinglePressAction::Create(
    /* [in] */ IContext* context,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [in] */ ILayoutInflater* inflater,
    /* [out] */ View** result)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(convertView);
    VALIDATE_NOT_NULL(parent);
    VALIDATE_NOT_NULL(inflater);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // View v = inflater.inflate(R.layout.global_actions_item, parent, false);
    //
    // ImageView icon = (ImageView) v.findViewById(R.id.icon);
    // TextView messageView = (TextView) v.findViewById(R.id.message);
    //
    // TextView statusView = (TextView) v.findViewById(R.id.status);
    // final String status = getStatus();
    // if (!TextUtils.isEmpty(status)) {
    //     statusView.setText(status);
    // } else {
    //     statusView.setVisibility(View.GONE);
    // }
    // if (mIcon != null) {
    //     icon.setImageDrawable(mIcon);
    //     icon.setScaleType(ScaleType.CENTER_CROP);
    // } else if (mIconResId != 0) {
    //     icon.setImageDrawable(context.getDrawable(mIconResId));
    // }
    // if (mMessage != null) {
    //     messageView.setText(mMessage);
    // } else {
    //     messageView.setText(mMessageResId);
    // }
    //
    // return v;
    assert(0);
    return NOERROR;
}

GlobalActions::SinglePressAction::SinglePressAction(
    /* [in] */ Int32 iconResId,
    /* [in] */ Int32 messageResId)
{
    // ==================before translated======================
    // mIconResId = iconResId;
    // mMessageResId = messageResId;
    // mMessage = null;
    // mIcon = null;
}

GlobalActions::SinglePressAction::SinglePressAction(
    /* [in] */ Int32 iconResId,
    /* [in] */ IDrawable* icon,
    /* [in] */ CharSequence* message)
{
    // ==================before translated======================
    // mIconResId = iconResId;
    // mMessageResId = 0;
    // mMessage = message;
    // mIcon = icon;
}

GlobalActions::SinglePressAction::SinglePressAction(
    /* [in] */ Int32 iconResId,
    /* [in] */ CharSequence* message)
{
    // ==================before translated======================
    // mIconResId = iconResId;
    // mMessageResId = 0;
    // mMessage = message;
    // mIcon = null;
}

//=====================================================================
//                     GlobalActions::ToggleAction
//=====================================================================
GlobalActions::ToggleAction::ToggleAction(
    /* [in] */ Int32 enabledIconResId,
    /* [in] */ Int32 disabledIconResid,
    /* [in] */ Int32 message,
    /* [in] */ Int32 enabledStatusMessageResId,
    /* [in] */ Int32 disabledStatusMessageResId)
{
    // ==================before translated======================
    // mEnabledIconResId = enabledIconResId;
    // mDisabledIconResid = disabledIconResid;
    // mMessageResId = message;
    // mEnabledStatusMessageResId = enabledStatusMessageResId;
    // mDisabledStatusMessageResId = disabledStatusMessageResId;
}

ECode GlobalActions::ToggleAction::WillCreate()
{
    assert(0);
    return NOERROR;
}

AutoPtr<CharSequence> GlobalActions::ToggleAction::GetLabelForAccessibility(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // return context.getString(mMessageResId);
    assert(0);
    AutoPtr<CharSequence> empty;
    return empty;
}

ECode GlobalActions::ToggleAction::Create(
    /* [in] */ IContext* context,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [in] */ ILayoutInflater* inflater,
    /* [out] */ View** result)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(convertView);
    VALIDATE_NOT_NULL(parent);
    VALIDATE_NOT_NULL(inflater);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // willCreate();
    //
    // View v = inflater.inflate(R
    //                 .layout.global_actions_item, parent, false);
    //
    // ImageView icon = (ImageView) v.findViewById(R.id.icon);
    // TextView messageView = (TextView) v.findViewById(R.id.message);
    // TextView statusView = (TextView) v.findViewById(R.id.status);
    // final boolean enabled = isEnabled();
    //
    // if (messageView != null) {
    //     messageView.setText(mMessageResId);
    //     messageView.setEnabled(enabled);
    // }
    //
    // boolean on = ((mState == State.On) || (mState == State.TurningOn));
    // if (icon != null) {
    //     icon.setImageDrawable(context.getDrawable(
    //             (on ? mEnabledIconResId : mDisabledIconResid)));
    //     icon.setEnabled(enabled);
    // }
    //
    // if (statusView != null) {
    //     statusView.setText(on ? mEnabledStatusMessageResId : mDisabledStatusMessageResId);
    //     statusView.setVisibility(View.VISIBLE);
    //     statusView.setEnabled(enabled);
    // }
    // v.setEnabled(enabled);
    //
    // return v;
    assert(0);
    return NOERROR;
}

const ECode GlobalActions::ToggleAction::OnPress()
{
    // ==================before translated======================
    // if (mState.inTransition()) {
    //     Log.w(TAG, "shouldn't be able to toggle when in transition");
    //     return;
    // }
    //
    // final boolean nowOn = !(mState == State.On);
    // onToggle(nowOn);
    // changeStateFromPress(nowOn);
    assert(0);
    return NOERROR;
}

ECode GlobalActions::ToggleAction::IsEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return !mState.inTransition();
    assert(0);
    return NOERROR;
}

ECode GlobalActions::ToggleAction::UpdateState(
    /* [in] */ State* state)
{
    VALIDATE_NOT_NULL(state);
    // ==================before translated======================
    // mState = state;
    assert(0);
    return NOERROR;
}

void GlobalActions::ToggleAction::ChangeStateFromPress(
    /* [in] */ Boolean buttonOn)
{
    // ==================before translated======================
    // mState = buttonOn ? State.On : State.Off;
    assert(0);
}

//=====================================================================
//                GlobalActions::SilentModeToggleAction
//=====================================================================
GlobalActions::SilentModeToggleAction::SilentModeToggleAction()
{
    // ==================before translated======================
    // super(R.drawable.ic_audio_vol_mute,
    //         R.drawable.ic_audio_vol,
    //         R.string.global_action_toggle_silent_mode,
    //         R.string.global_action_silent_mode_on_status,
    //         R.string.global_action_silent_mode_off_status);
}

ECode GlobalActions::SilentModeToggleAction::OnToggle(
    /* [in] */ Boolean on)
{
    // ==================before translated======================
    // if (on) {
    //     mAudioManager.setRingerMode(AudioManager.RINGER_MODE_SILENT);
    // } else {
    //     mAudioManager.setRingerMode(AudioManager.RINGER_MODE_NORMAL);
    // }
    assert(0);
    return NOERROR;
}

ECode GlobalActions::SilentModeToggleAction::ShowDuringKeyguard(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return true;
    assert(0);
    return NOERROR;
}

ECode GlobalActions::SilentModeToggleAction::ShowBeforeProvisioning(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return false;
    assert(0);
    return NOERROR;
}

//=====================================================================
//               GlobalActions::SilentModeTriStateAction
//=====================================================================
GlobalActions::SilentModeTriStateAction::SilentModeTriStateAction(
    /* [in] */ IContext* context,
    /* [in] */ IAudioManager* audioManager,
    /* [in] */ IHandler* handler)
{
    // ==================before translated======================
    // mAudioManager = audioManager;
    // mHandler = handler;
    // mContext = context;
}

AutoPtr<CharSequence> GlobalActions::SilentModeTriStateAction::GetLabelForAccessibility(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // return null;
    assert(0);
    AutoPtr<CharSequence> empty;
    return empty;
}

ECode GlobalActions::SilentModeTriStateAction::Create(
    /* [in] */ IContext* context,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [in] */ ILayoutInflater* inflater,
    /* [out] */ View** result)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(convertView);
    VALIDATE_NOT_NULL(parent);
    VALIDATE_NOT_NULL(inflater);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // View v = inflater.inflate(R.layout.global_actions_silent_mode, parent, false);
    //
    // int selectedIndex = ringerModeToIndex(mAudioManager.getRingerMode());
    // for (int i = 0; i < 3; i++) {
    //     View itemView = v.findViewById(ITEM_IDS[i]);
    //     itemView.setSelected(selectedIndex == i);
    //     // Set up click handler
    //     itemView.setTag(i);
    //     itemView.setOnClickListener(this);
    // }
    // return v;
    assert(0);
    return NOERROR;
}

ECode GlobalActions::SilentModeTriStateAction::OnPress()
{
    assert(0);
    return NOERROR;
}

ECode GlobalActions::SilentModeTriStateAction::ShowDuringKeyguard(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return true;
    assert(0);
    return NOERROR;
}

ECode GlobalActions::SilentModeTriStateAction::ShowBeforeProvisioning(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return false;
    assert(0);
    return NOERROR;
}

ECode GlobalActions::SilentModeTriStateAction::IsEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return true;
    assert(0);
    return NOERROR;
}

ECode GlobalActions::SilentModeTriStateAction::WillCreate()
{
    assert(0);
    return NOERROR;
}

ECode GlobalActions::SilentModeTriStateAction::OnClick(
    /* [in] */ IView* v)
{
    VALIDATE_NOT_NULL(v);
    // ==================before translated======================
    // if (!(v.getTag() instanceof Integer)) return;
    //
    // int index = (Integer) v.getTag();
    // mAudioManager.setRingerMode(indexToRingerMode(index));
    // mHandler.sendEmptyMessageDelayed(MESSAGE_DISMISS, DIALOG_DISMISS_DELAY);
    assert(0);
    return NOERROR;
}

AutoPtr< ArrayOf<Int32> > GlobalActions::SilentModeTriStateAction::MiddleInitItemIds()
{
    // ==================before translated======================
    // ->WWZ_SIGN: ARRAY_INIT_START {
    // R.id.option1, R.id.option2, R.id.option3
    // ->WWZ_SIGN: ARRAY_INIT_END }
    assert(0);
    AutoPtr< ArrayOf<Int32> > empty;
    return empty;
}

Int32 GlobalActions::SilentModeTriStateAction::RingerModeToIndex(
    /* [in] */ Int32 ringerMode)
{
    // ==================before translated======================
    // // They just happen to coincide
    // return ringerMode;
    assert(0);
    return 0;
}

Int32 GlobalActions::SilentModeTriStateAction::IndexToRingerMode(
    /* [in] */ Int32 index)
{
    // ==================before translated======================
    // // They just happen to coincide
    // return index;
    assert(0);
    return 0;
}

//=====================================================================
//                GlobalActions::InnerBroadcastReceiver
//=====================================================================
GlobalActions::InnerBroadcastReceiver::InnerBroadcastReceiver(
    /* [in] */ GlobalActions* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode GlobalActions::InnerBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(intent);
    // ==================before translated======================
    // String action = intent.getAction();
    // if (Intent.ACTION_CLOSE_SYSTEM_DIALOGS.equals(action)
    //         || Intent.ACTION_SCREEN_OFF.equals(action)) {
    //     String reason = intent.getStringExtra(PhoneWindowManager.SYSTEM_DIALOG_REASON_KEY);
    //     if (!PhoneWindowManager.SYSTEM_DIALOG_REASON_GLOBAL_ACTIONS.equals(reason)) {
    //         mHandler.sendEmptyMessage(MESSAGE_DISMISS);
    //     }
    // } else if (TelephonyIntents.ACTION_EMERGENCY_CALLBACK_MODE_CHANGED.equals(action)) {
    //     // Airplane mode can be changed after ECM exits if airplane toggle button
    //     // is pressed during ECM mode
    //     if (!(intent.getBooleanExtra("PHONE_IN_ECM_STATE", false)) &&
    //             mIsWaitingForEcmExit) {
    //         mIsWaitingForEcmExit = false;
    //         changeAirplaneModeSystemSetting(true);
    //     }
    // }
    assert(0);
    return NOERROR;
}

//=====================================================================
//                GlobalActions::InnerPhoneStateListener
//=====================================================================
GlobalActions::InnerPhoneStateListener::InnerPhoneStateListener(
    /* [in] */ GlobalActions* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode GlobalActions::InnerPhoneStateListener::OnServiceStateChanged(
    /* [in] */ IServiceState* serviceState)
{
    VALIDATE_NOT_NULL(serviceState);
    // ==================before translated======================
    // if (!mHasTelephony) return;
    // final boolean inAirplaneMode = serviceState.getState() == ServiceState.STATE_POWER_OFF;
    // mAirplaneState = inAirplaneMode ? ToggleAction.State.On : ToggleAction.State.Off;
    // mAirplaneModeOn.updateState(mAirplaneState);
    // mAdapter.notifyDataSetChanged();
    assert(0);
    return NOERROR;
}

//=====================================================================
//                GlobalActions::InnerBroadcastReceiver1
//=====================================================================
GlobalActions::InnerBroadcastReceiver1::InnerBroadcastReceiver1(
    /* [in] */ GlobalActions* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode GlobalActions::InnerBroadcastReceiver1::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(intent);
    // ==================before translated======================
    // if (intent.getAction().equals(AudioManager.RINGER_MODE_CHANGED_ACTION)) {
    //     mHandler.sendEmptyMessage(MESSAGE_REFRESH);
    // }
    assert(0);
    return NOERROR;
}

//=====================================================================
//                 GlobalActions::InnerContentObserver
//=====================================================================
GlobalActions::InnerContentObserver::InnerContentObserver(
    /* [in] */ GlobalActions* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode GlobalActions::InnerContentObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    // ==================before translated======================
    // onAirplaneModeChanged();
    assert(0);
    return NOERROR;
}

//=====================================================================
//                     GlobalActions::InnerHandler
//=====================================================================
GlobalActions::InnerHandler::InnerHandler(
    /* [in] */ GlobalActions* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode GlobalActions::InnerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);
    // ==================before translated======================
    // switch (msg.what) {
    // case MESSAGE_DISMISS:
    //     if (mDialog != null) {
    //         mDialog.dismiss();
    //         mDialog = null;
    //     }
    //     break;
    // case MESSAGE_REFRESH:
    //     refreshSilentMode();
    //     mAdapter.notifyDataSetChanged();
    //     break;
    // case MESSAGE_SHOW:
    //     handleShow();
    //     break;
    // }
    assert(0);
    return NOERROR;
}

//=====================================================================
//                            GlobalActions
//=====================================================================
const String GlobalActions::TAG("GlobalActions");
const Boolean GlobalActions::SHOW_SILENT_TOGGLE = true;
const String GlobalActions::GLOBAL_ACTION_KEY_POWER("power");
const String GlobalActions::GLOBAL_ACTION_KEY_AIRPLANE("airplane");
const String GlobalActions::GLOBAL_ACTION_KEY_BUGREPORT("bugreport");
const String GlobalActions::GLOBAL_ACTION_KEY_SILENT("silent");
const String GlobalActions::GLOBAL_ACTION_KEY_USERS("users");
const String GlobalActions::GLOBAL_ACTION_KEY_SETTINGS("settings");
const String GlobalActions::GLOBAL_ACTION_KEY_LOCKDOWN("lockdown");
const Int32 GlobalActions::MESSAGE_DISMISS;
const Int32 GlobalActions::MESSAGE_REFRESH;
const Int32 GlobalActions::MESSAGE_SHOW;
const Int32 GlobalActions::DIALOG_DISMISS_DELAY;

GlobalActions::GlobalActions(
    /* [in] */ IContext* context,
    /* [in] */ IWindowManagerFuncs* windowManagerFuncs)
{
    // ==================before translated======================
    // mContext = context;
    // mWindowManagerFuncs = windowManagerFuncs;
    // mAudioManager = (AudioManager) mContext.getSystemService(Context.AUDIO_SERVICE);
    // mDreamManager = IDreamManager.Stub.asInterface(
    //         ServiceManager.getService(DreamService.DREAM_SERVICE));
    //
    // // receive broadcasts
    // IntentFilter filter = new IntentFilter();
    // filter.addAction(Intent.ACTION_CLOSE_SYSTEM_DIALOGS);
    // filter.addAction(Intent.ACTION_SCREEN_OFF);
    // filter.addAction(TelephonyIntents.ACTION_EMERGENCY_CALLBACK_MODE_CHANGED);
    // context.registerReceiver(mBroadcastReceiver, filter);
    //
    // ConnectivityManager cm = (ConnectivityManager)
    //         context.getSystemService(Context.CONNECTIVITY_SERVICE);
    // mHasTelephony = cm.isNetworkSupported(ConnectivityManager.TYPE_MOBILE);
    //
    // // get notified of phone state changes
    // TelephonyManager telephonyManager =
    //         (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
    // telephonyManager.listen(mPhoneStateListener, PhoneStateListener.LISTEN_SERVICE_STATE);
    // mContext.getContentResolver().registerContentObserver(
    //         Settings.Global.getUriFor(Settings.Global.AIRPLANE_MODE_ON), true,
    //         mAirplaneModeObserver);
    // Vibrator vibrator = (Vibrator) mContext.getSystemService(Context.VIBRATOR_SERVICE);
    // mHasVibrator = vibrator != null && vibrator.hasVibrator();
    //
    // mShowSilentToggle = SHOW_SILENT_TOGGLE && !mContext.getResources().getBoolean(
    //         com.android.internal.R.bool.config_useFixedVolume);
}

ECode GlobalActions::ShowDialog(
    /* [in] */ Boolean keyguardShowing,
    /* [in] */ Boolean isDeviceProvisioned)
{
    // ==================before translated======================
    // mKeyguardShowing = keyguardShowing;
    // mDeviceProvisioned = isDeviceProvisioned;
    // if (mDialog != null) {
    //     mDialog.dismiss();
    //     mDialog = null;
    //     // Show delayed, so that the dismiss of the previous dialog completes
    //     mHandler.sendEmptyMessage(MESSAGE_SHOW);
    // } else {
    //     handleShow();
    // }
    assert(0);
    return NOERROR;
}

ECode GlobalActions::OnDismiss(
    /* [in] */ IDialogInterface* dialog)
{
    VALIDATE_NOT_NULL(dialog);
    // ==================before translated======================
    // if (mShowSilentToggle) {
    //     try {
    //         mContext.unregisterReceiver(mRingerModeReceiver);
    //     } catch (IllegalArgumentException ie) {
    //         // ignore this
    //         Log.w(TAG, ie);
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode GlobalActions::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    VALIDATE_NOT_NULL(dialog);
    // ==================before translated======================
    // if (!(mAdapter.getItem(which) instanceof SilentModeTriStateAction)) {
    //     dialog.dismiss();
    // }
    // mAdapter.getItem(which).onPress();
    assert(0);
    return NOERROR;
}

void GlobalActions::AwakenIfNecessary()
{
    // ==================before translated======================
    // if (mDreamManager != null) {
    //     try {
    //         if (mDreamManager.isDreaming()) {
    //             mDreamManager.awaken();
    //         }
    //     } catch (RemoteException e) {
    //         // we tried
    //     }
    // }
    assert(0);
}

void GlobalActions::HandleShow()
{
    // ==================before translated======================
    // awakenIfNecessary();
    // mDialog = createDialog();
    // prepareDialog();
    //
    // // If we only have 1 item and it's a simple press action, just do this action.
    // if (mAdapter.getCount() == 1
    //         && mAdapter.getItem(0) instanceof SinglePressAction
    //         && !(mAdapter.getItem(0) instanceof LongPressAction)) {
    //     ((SinglePressAction) mAdapter.getItem(0)).onPress();
    // } else {
    //     WindowManager.LayoutParams attrs = mDialog.getWindow().getAttributes();
    //     attrs.setTitle("GlobalActions");
    //     mDialog.getWindow().setAttributes(attrs);
    //     mDialog.show();
    //     mDialog.getWindow().getDecorView().setSystemUiVisibility(View.STATUS_BAR_DISABLE_EXPAND);
    // }
    assert(0);
}

AutoPtr<GlobalActionsDialog> GlobalActions::CreateDialog()
{
    // ==================before translated======================
    // // Simple toggle style if there's no vibrator, otherwise use a tri-state
    // if (!mHasVibrator) {
    //     mSilentModeAction = new SilentModeToggleAction();
    // } else {
    //     mSilentModeAction = new SilentModeTriStateAction(mContext, mAudioManager, mHandler);
    // }
    // mAirplaneModeOn = new ToggleAction(
    //         R.drawable.ic_lock_airplane_mode,
    //         R.drawable.ic_lock_airplane_mode_off,
    //         R.string.global_actions_toggle_airplane_mode,
    //         R.string.global_actions_airplane_mode_on_status,
    //         R.string.global_actions_airplane_mode_off_status) {
    //
    //     void onToggle(boolean on) {
    //         if (mHasTelephony && Boolean.parseBoolean(
    //                 SystemProperties.get(TelephonyProperties.PROPERTY_INECM_MODE))) {
    //             mIsWaitingForEcmExit = true;
    //             // Launch ECM exit dialog
    //             Intent ecmDialogIntent =
    //                     new Intent(TelephonyIntents.ACTION_SHOW_NOTICE_ECM_BLOCK_OTHERS, null);
    //             ecmDialogIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
    //             mContext.startActivity(ecmDialogIntent);
    //         } else {
    //             changeAirplaneModeSystemSetting(on);
    //         }
    //     }
    //
    //     @Override
    //     protected void changeStateFromPress(boolean buttonOn) {
    //         if (!mHasTelephony) return;
    //
    //         // In ECM mode airplane state cannot be changed
    //         if (!(Boolean.parseBoolean(
    //                 SystemProperties.get(TelephonyProperties.PROPERTY_INECM_MODE)))) {
    //             mState = buttonOn ? State.TurningOn : State.TurningOff;
    //             mAirplaneState = mState;
    //         }
    //     }
    //
    //     public boolean showDuringKeyguard() {
    //         return true;
    //     }
    //
    //     public boolean showBeforeProvisioning() {
    //         return false;
    //     }
    // };
    // onAirplaneModeChanged();
    //
    // mItems = new ArrayList<Action>();
    // String[] defaultActions = mContext.getResources().getStringArray(
    //         com.android.internal.R.array.config_globalActionsList);
    //
    // ArraySet<String> addedKeys = new ArraySet<String>();
    // for (int i = 0; i < defaultActions.length; i++) {
    //     String actionKey = defaultActions[i];
    //     if (addedKeys.contains(actionKey)) {
    //         // If we already have added this, don't add it again.
    //         continue;
    //     }
    //     if (GLOBAL_ACTION_KEY_POWER.equals(actionKey)) {
    //         mItems.add(new PowerAction());
    //     } else if (GLOBAL_ACTION_KEY_AIRPLANE.equals(actionKey)) {
    //         mItems.add(mAirplaneModeOn);
    //     } else if (GLOBAL_ACTION_KEY_BUGREPORT.equals(actionKey)) {
    //         if (Settings.Global.getInt(mContext.getContentResolver(),
    //                 Settings.Global.BUGREPORT_IN_POWER_MENU, 0) != 0 && isCurrentUserOwner()) {
    //             mItems.add(getBugReportAction());
    //         }
    //     } else if (GLOBAL_ACTION_KEY_SILENT.equals(actionKey)) {
    //         if (mShowSilentToggle) {
    //             mItems.add(mSilentModeAction);
    //         }
    //     } else if (GLOBAL_ACTION_KEY_USERS.equals(actionKey)) {
    //         if (SystemProperties.getBoolean("fw.power_user_switcher", false)) {
    //             addUsersToMenu(mItems);
    //         }
    //     } else if (GLOBAL_ACTION_KEY_SETTINGS.equals(actionKey)) {
    //         mItems.add(getSettingsAction());
    //     } else if (GLOBAL_ACTION_KEY_LOCKDOWN.equals(actionKey)) {
    //         mItems.add(getLockdownAction());
    //     } else {
    //         Log.e(TAG, "Invalid global action key " + actionKey);
    //     }
    //     // Add here so we don't add more than one.
    //     addedKeys.add(actionKey);
    // }
    //
    // mAdapter = new MyAdapter();
    //
    // AlertParams params = new AlertParams(mContext);
    // params.mAdapter = mAdapter;
    // params.mOnClickListener = this;
    // params.mForceInverseBackground = true;
    //
    // GlobalActionsDialog dialog = new GlobalActionsDialog(mContext, params);
    // dialog.setCanceledOnTouchOutside(false); // Handled by the custom class.
    //
    // dialog.getListView().setItemsCanFocus(true);
    // dialog.getListView().setLongClickable(true);
    // dialog.getListView().setOnItemLongClickListener(
    //         new AdapterView.OnItemLongClickListener() {
    //             @Override
    //             public boolean onItemLongClick(AdapterView<?> parent, View view, int position,
    //                     long id) {
    //                 final Action action = mAdapter.getItem(position);
    //                 if (action instanceof LongPressAction) {
    //                     return ((LongPressAction) action).onLongPress();
    //                 }
    //                 return false;
    //             }
    // });
    // dialog.getWindow().setType(WindowManager.LayoutParams.TYPE_KEYGUARD_DIALOG);
    //
    // dialog.setOnDismissListener(this);
    //
    // return dialog;
    assert(0);
    AutoPtr<GlobalActionsDialog> empty;
    return empty;
}

AutoPtr<Action> GlobalActions::GetBugReportAction()
{
    // ==================before translated======================
    // return new SinglePressAction(com.android.internal.R.drawable.ic_lock_bugreport,
    //         R.string.bugreport_title) {
    //
    //     public void onPress() {
    //         AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
    //         builder.setTitle(com.android.internal.R.string.bugreport_title);
    //         builder.setMessage(com.android.internal.R.string.bugreport_message);
    //         builder.setNegativeButton(com.android.internal.R.string.cancel, null);
    //         builder.setPositiveButton(com.android.internal.R.string.report,
    //                 new DialogInterface.OnClickListener() {
    //                     @Override
    //                     public void onClick(DialogInterface dialog, int which) {
    //                         // don't actually trigger the bugreport if we are running stability
    //                         // tests via monkey
    //                         if (ActivityManager.isUserAMonkey()) {
    //                             return;
    //                         }
    //                         // Add a little delay before executing, to give the
    //                         // dialog a chance to go away before it takes a
    //                         // screenshot.
    //                         mHandler.postDelayed(new Runnable() {
    //                             @Override public void run() {
    //                                 try {
    //                                     ActivityManagerNative.getDefault()
    //                                             .requestBugReport();
    //                                 } catch (RemoteException e) {
    //                                 }
    //                             }
    //                         }, 500);
    //                     }
    //                 });
    //         AlertDialog dialog = builder.create();
    //         dialog.getWindow().setType(WindowManager.LayoutParams.TYPE_KEYGUARD_DIALOG);
    //         dialog.show();
    //     }
    //
    //     public boolean showDuringKeyguard() {
    //         return true;
    //     }
    //
    //     public boolean showBeforeProvisioning() {
    //         return false;
    //     }
    //
    //     @Override
    //     public String getStatus() {
    //         return mContext.getString(
    //                 com.android.internal.R.string.bugreport_status,
    //                 Build.VERSION.RELEASE,
    //                 Build.ID);
    //     }
    // };
    assert(0);
    AutoPtr<Action> empty;
    return empty;
}

AutoPtr<Action> GlobalActions::GetSettingsAction()
{
    // ==================before translated======================
    // return new SinglePressAction(com.android.internal.R.drawable.ic_settings,
    //         R.string.global_action_settings) {
    //
    //     @Override
    //     public void onPress() {
    //         Intent intent = new Intent(Settings.ACTION_SETTINGS);
    //         intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP);
    //         mContext.startActivity(intent);
    //     }
    //
    //     @Override
    //     public boolean showDuringKeyguard() {
    //         return true;
    //     }
    //
    //     @Override
    //     public boolean showBeforeProvisioning() {
    //         return true;
    //     }
    // };
    assert(0);
    AutoPtr<Action> empty;
    return empty;
}

AutoPtr<Action> GlobalActions::GetLockdownAction()
{
    // ==================before translated======================
    // return new SinglePressAction(com.android.internal.R.drawable.ic_lock_lock,
    //         R.string.global_action_lockdown) {
    //
    //     @Override
    //     public void onPress() {
    //         new LockPatternUtils(mContext).requireCredentialEntry(UserHandle.USER_ALL);
    //         try {
    //             WindowManagerGlobal.getWindowManagerService().lockNow(null);
    //         } catch (RemoteException e) {
    //             Log.e(TAG, "Error while trying to lock device.", e);
    //         }
    //     }
    //
    //     @Override
    //     public boolean showDuringKeyguard() {
    //         return true;
    //     }
    //
    //     @Override
    //     public boolean showBeforeProvisioning() {
    //         return false;
    //     }
    // };
    assert(0);
    AutoPtr<Action> empty;
    return empty;
}

AutoPtr<IUserInfo> GlobalActions::GetCurrentUser()
{
    // ==================before translated======================
    // try {
    //     return ActivityManagerNative.getDefault().getCurrentUser();
    // } catch (RemoteException re) {
    //     return null;
    // }
    assert(0);
    AutoPtr<IUserInfo> empty;
    return empty;
}

Boolean GlobalActions::IsCurrentUserOwner()
{
    // ==================before translated======================
    // UserInfo currentUser = getCurrentUser();
    // return currentUser == null || currentUser.isPrimary();
    assert(0);
    return FALSE;
}

void GlobalActions::AddUsersToMenu(
    /* [in] */ IArrayList<Action*>* items)
{
    // ==================before translated======================
    // UserManager um = (UserManager) mContext.getSystemService(Context.USER_SERVICE);
    // if (um.isUserSwitcherEnabled()) {
    //     List<UserInfo> users = um.getUsers();
    //     UserInfo currentUser = getCurrentUser();
    //     for (final UserInfo user : users) {
    //         if (user.supportsSwitchTo()) {
    //             boolean isCurrentUser = currentUser == null
    //                     ? user.id == 0 : (currentUser.id == user.id);
    //             Drawable icon = user.iconPath != null ? Drawable.createFromPath(user.iconPath)
    //                     : null;
    //             SinglePressAction switchToUser = new SinglePressAction(
    //                     com.android.internal.R.drawable.ic_menu_cc, icon,
    //                     (user.name != null ? user.name : "Primary")
    //                     + (isCurrentUser ? " \u2714" : "")) {
    //                 public void onPress() {
    //                     try {
    //                         ActivityManagerNative.getDefault().switchUser(user.id);
    //                     } catch (RemoteException re) {
    //                         Log.e(TAG, "Couldn't switch user " + re);
    //                     }
    //                 }
    //
    //                 public boolean showDuringKeyguard() {
    //                     return true;
    //                 }
    //
    //                 public boolean showBeforeProvisioning() {
    //                     return false;
    //                 }
    //             };
    //             items.add(switchToUser);
    //         }
    //     }
    // }
    assert(0);
}

void GlobalActions::PrepareDialog()
{
    // ==================before translated======================
    // refreshSilentMode();
    // mAirplaneModeOn.updateState(mAirplaneState);
    // mAdapter.notifyDataSetChanged();
    // mDialog.getWindow().setType(WindowManager.LayoutParams.TYPE_KEYGUARD_DIALOG);
    // if (mShowSilentToggle) {
    //     IntentFilter filter = new IntentFilter(AudioManager.RINGER_MODE_CHANGED_ACTION);
    //     mContext.registerReceiver(mRingerModeReceiver, filter);
    // }
    assert(0);
}

void GlobalActions::RefreshSilentMode()
{
    // ==================before translated======================
    // if (!mHasVibrator) {
    //     final boolean silentModeOn =
    //             mAudioManager.getRingerMode() != AudioManager.RINGER_MODE_NORMAL;
    //     ((ToggleAction)mSilentModeAction).updateState(
    //             silentModeOn ? ToggleAction.State.On : ToggleAction.State.Off);
    // }
    assert(0);
}

void GlobalActions::OnAirplaneModeChanged()
{
    // ==================before translated======================
    // // Let the service state callbacks handle the state.
    // if (mHasTelephony) return;
    //
    // boolean airplaneModeOn = Settings.Global.getInt(
    //         mContext.getContentResolver(),
    //         Settings.Global.AIRPLANE_MODE_ON,
    //         0) == 1;
    // mAirplaneState = airplaneModeOn ? ToggleAction.State.On : ToggleAction.State.Off;
    // mAirplaneModeOn.updateState(mAirplaneState);
    assert(0);
}

void GlobalActions::ChangeAirplaneModeSystemSetting(
    /* [in] */ Boolean on)
{
    // ==================before translated======================
    // Settings.Global.putInt(
    //         mContext.getContentResolver(),
    //         Settings.Global.AIRPLANE_MODE_ON,
    //         on ? 1 : 0);
    // Intent intent = new Intent(Intent.ACTION_AIRPLANE_MODE_CHANGED);
    // intent.addFlags(Intent.FLAG_RECEIVER_REPLACE_PENDING);
    // intent.putExtra("state", on);
    // mContext.sendBroadcastAsUser(intent, UserHandle.ALL);
    // if (!mHasTelephony) {
    //     mAirplaneState = on ? ToggleAction.State.On : ToggleAction.State.Off;
    // }
    assert(0);
}

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos


