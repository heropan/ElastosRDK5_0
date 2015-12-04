
#include "elastos/droid/internal/policy/impl/RecentApplicationsDialog.h"

using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::View::EIID_IViewOnClickListener;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

//=====================================================================
//               RecentApplicationsDialog::InnerRunnable
//=====================================================================
CAR_INTERFACE_IMPL(RecentApplicationsDialog::InnerRunnable, Object, IRunnable)

RecentApplicationsDialog::InnerRunnable::InnerRunnable(
    /* [in] */ RecentApplicationsDialog* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode RecentApplicationsDialog::InnerRunnable::Run()
{
    // ==================before translated======================
    // // dump extra memory we're hanging on to
    // for (TextView icon: mIcons) {
    //     icon.setCompoundDrawables(null, null, null, null);
    //     icon.setTag(null);
    // }
    assert(0);
    return NOERROR;
}

//=====================================================================
//           RecentApplicationsDialog::InnerBroadcastReceiver
//=====================================================================
RecentApplicationsDialog::InnerBroadcastReceiver::InnerBroadcastReceiver(
    /* [in] */ RecentApplicationsDialog* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode RecentApplicationsDialog::InnerBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(intent);
    // ==================before translated======================
    // String action = intent.getAction();
    // if (Intent.ACTION_CLOSE_SYSTEM_DIALOGS.equals(action)) {
    //     String reason = intent.getStringExtra(PhoneWindowManager.SYSTEM_DIALOG_REASON_KEY);
    //     if (! PhoneWindowManager.SYSTEM_DIALOG_REASON_RECENT_APPS.equals(reason)) {
    //         dismiss();
    //     }
    // }
    assert(0);
    return NOERROR;
}

//=====================================================================
//                       RecentApplicationsDialog
//=====================================================================
CAR_INTERFACE_IMPL(RecentApplicationsDialog, Dialog, IViewOnClickListener)

const Boolean RecentApplicationsDialog::DBG_FORCE_EMPTY_LIST = FALSE;
AutoPtr<IStatusBarManager> RecentApplicationsDialog::sStatusBar;
const Int32 RecentApplicationsDialog::NUM_BUTTONS;
const Int32 RecentApplicationsDialog::MAX_RECENT_TASKS;

RecentApplicationsDialog::RecentApplicationsDialog()
{
}

ECode RecentApplicationsDialog::constructor(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // super(context, com.android.internal.R.style.Theme_Dialog_RecentApplications);
    return NOERROR;
}

ECode RecentApplicationsDialog::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (keyCode == KeyEvent.KEYCODE_TAB) {
    //     // Ignore all meta keys other than SHIFT.  The app switch key could be a
    //     // fallback action chorded with ALT, META or even CTRL depending on the key map.
    //     // DPad navigation is handled by the ViewRoot elsewhere.
    //     final boolean backward = event.isShiftPressed();
    //     final int numIcons = mIcons.length;
    //     int numButtons = 0;
    //     while (numButtons < numIcons && mIcons[numButtons].getVisibility() == View.VISIBLE) {
    //         numButtons += 1;
    //     }
    //     if (numButtons != 0) {
    //         int nextFocus = backward ? numButtons - 1 : 0;
    //         for (int i = 0; i < numButtons; i++) {
    //             if (mIcons[i].hasFocus()) {
    //                 if (backward) {
    //                     nextFocus = (i + numButtons - 1) % numButtons;
    //                 } else {
    //                     nextFocus = (i + 1) % numButtons;
    //                 }
    //                 break;
    //             }
    //         }
    //         final int direction = backward ? View.FOCUS_BACKWARD : View.FOCUS_FORWARD;
    //         if (mIcons[nextFocus].requestFocus(direction)) {
    //             mIcons[nextFocus].playSoundEffect(
    //                     SoundEffectConstants.getContantForFocusDirection(direction));
    //         }
    //     }
    //
    //     // The dialog always handles the key to prevent the ViewRoot from
    //     // performing the default navigation itself.
    //     return true;
    // }
    //
    // return super.onKeyDown(keyCode, event);
    assert(0);
    return NOERROR;
}

ECode RecentApplicationsDialog::DismissAndSwitch()
{
    // ==================before translated======================
    // final int numIcons = mIcons.length;
    // RecentTag tag = null;
    // for (int i = 0; i < numIcons; i++) {
    //     if (mIcons[i].getVisibility() != View.VISIBLE) {
    //         break;
    //     }
    //     if (i == 0 || mIcons[i].hasFocus()) {
    //         tag = (RecentTag) mIcons[i].getTag();
    //         if (mIcons[i].hasFocus()) {
    //             break;
    //         }
    //     }
    // }
    // if (tag != null) {
    //     switchTo(tag);
    // }
    // dismiss();
    assert(0);
    return NOERROR;
}

ECode RecentApplicationsDialog::OnClick(
    /* [in] */ IView* v)
{
    VALIDATE_NOT_NULL(v);
    // ==================before translated======================
    // for (TextView b: mIcons) {
    //     if (b == v) {
    //         RecentTag tag = (RecentTag)b.getTag();
    //         switchTo(tag);
    //         break;
    //     }
    // }
    // dismiss();
    assert(0);
    return NOERROR;
}

ECode RecentApplicationsDialog::OnStart()
{
    // ==================before translated======================
    // super.onStart();
    // reloadButtons();
    // if (sStatusBar != null) {
    //     sStatusBar.disable(StatusBarManager.DISABLE_EXPAND);
    // }
    //
    // // receive broadcasts
    // getContext().registerReceiver(mBroadcastReceiver, mBroadcastIntentFilter);
    //
    // mHandler.removeCallbacks(mCleanup);
    assert(0);
    return NOERROR;
}

ECode RecentApplicationsDialog::OnStop()
{
    // ==================before translated======================
    // super.onStop();
    //
    // if (sStatusBar != null) {
    //     sStatusBar.disable(StatusBarManager.DISABLE_NONE);
    // }
    //
    // // stop receiving broadcasts
    // getContext().unregisterReceiver(mBroadcastReceiver);
    //
    // mHandler.postDelayed(mCleanup, 100);
    assert(0);
    return NOERROR;
}

ECode RecentApplicationsDialog::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    // ==================before translated======================
    // super.onCreate(savedInstanceState);
    //
    // Context context = getContext();
    //
    // if (sStatusBar == null) {
    //     sStatusBar = (StatusBarManager)context.getSystemService(Context.STATUS_BAR_SERVICE);
    // }
    //
    // Window window = getWindow();
    // window.requestFeature(Window.FEATURE_NO_TITLE);
    // window.setType(WindowManager.LayoutParams.TYPE_RECENTS_OVERLAY);
    // window.setFlags(WindowManager.LayoutParams.FLAG_ALT_FOCUSABLE_IM,
    //         WindowManager.LayoutParams.FLAG_ALT_FOCUSABLE_IM);
    // window.setTitle("Recents");
    //
    // setContentView(com.android.internal.R.layout.recent_apps_dialog);
    //
    // final WindowManager.LayoutParams params = window.getAttributes();
    // params.width = WindowManager.LayoutParams.MATCH_PARENT;
    // params.height = WindowManager.LayoutParams.MATCH_PARENT;
    // window.setAttributes(params);
    // window.setFlags(0, WindowManager.LayoutParams.FLAG_DIM_BEHIND);
    //
    // mIcons[0] = (TextView)findViewById(com.android.internal.R.id.button0);
    // mIcons[1] = (TextView)findViewById(com.android.internal.R.id.button1);
    // mIcons[2] = (TextView)findViewById(com.android.internal.R.id.button2);
    // mIcons[3] = (TextView)findViewById(com.android.internal.R.id.button3);
    // mIcons[4] = (TextView)findViewById(com.android.internal.R.id.button4);
    // mIcons[5] = (TextView)findViewById(com.android.internal.R.id.button5);
    // mIcons[6] = (TextView)findViewById(com.android.internal.R.id.button6);
    // mIcons[7] = (TextView)findViewById(com.android.internal.R.id.button7);
    // mNoAppsText = findViewById(com.android.internal.R.id.no_applications_message);
    //
    // for (TextView b: mIcons) {
    //     b.setOnClickListener(this);
    // }
    assert(0);
    return NOERROR;
}

AutoPtr< ArrayOf< AutoPtr<ITextView> > > RecentApplicationsDialog::MiddleInitMicons()
{
    // ==================before translated======================
    // TextView[] result = new TextView[NUM_BUTTONS];
    assert(0);
    AutoPtr< ArrayOf< AutoPtr<ITextView> > > empty;
    return empty;
}

void RecentApplicationsDialog::SwitchTo(
    /* [in] */ RecentTag* tag)
{
    // ==================before translated======================
    // if (tag.info.id >= 0) {
    //     // This is an active task; it should just go to the foreground.
    //     final ActivityManager am = (ActivityManager)
    //             getContext().getSystemService(Context.ACTIVITY_SERVICE);
    //     am.moveTaskToFront(tag.info.id, ActivityManager.MOVE_TASK_WITH_HOME);
    // } else if (tag.intent != null) {
    //     tag.intent.addFlags(Intent.FLAG_ACTIVITY_LAUNCHED_FROM_HISTORY
    //             | Intent.FLAG_ACTIVITY_TASK_ON_HOME);
    //     try {
    //         getContext().startActivity(tag.intent);
    //     } catch (ActivityNotFoundException e) {
    //         Log.w("Recent", "Unable to launch recent task", e);
    //     }
    // }
    assert(0);
}

void RecentApplicationsDialog::ReloadButtons()
{
    // ==================before translated======================
    //
    // final Context context = getContext();
    // final PackageManager pm = context.getPackageManager();
    // final ActivityManager am = (ActivityManager)
    //         context.getSystemService(Context.ACTIVITY_SERVICE);
    // final List<ActivityManager.RecentTaskInfo> recentTasks =
    //         am.getRecentTasks(MAX_RECENT_TASKS, ActivityManager.RECENT_IGNORE_UNAVAILABLE);
    //
    // ActivityInfo homeInfo =
    //     new Intent(Intent.ACTION_MAIN).addCategory(Intent.CATEGORY_HOME)
    //             .resolveActivityInfo(pm, 0);
    //
    // IconUtilities iconUtilities = new IconUtilities(getContext());
    //
    // // Performance note:  Our android performance guide says to prefer Iterator when
    // // using a List class, but because we know that getRecentTasks() always returns
    // // an ArrayList<>, we'll use a simple index instead.
    // int index = 0;
    // int numTasks = recentTasks.size();
    // for (int i = 0; i < numTasks && (index < NUM_BUTTONS); ++i) {
    //     final ActivityManager.RecentTaskInfo info = recentTasks.get(i);
    //
    //     // for debug purposes only, disallow first result to create empty lists
    //     if (DBG_FORCE_EMPTY_LIST && (i == 0)) continue;
    //
    //     Intent intent = new Intent(info.baseIntent);
    //     if (info.origActivity != null) {
    //         intent.setComponent(info.origActivity);
    //     }
    //
    //     // Skip the current home activity.
    //     if (homeInfo != null) {
    //         if (homeInfo.packageName.equals(
    //                 intent.getComponent().getPackageName())
    //                 && homeInfo.name.equals(
    //                         intent.getComponent().getClassName())) {
    //             continue;
    //         }
    //     }
    //
    //     intent.setFlags((intent.getFlags()&~Intent.FLAG_ACTIVITY_RESET_TASK_IF_NEEDED)
    //             | Intent.FLAG_ACTIVITY_NEW_TASK);
    //     final ResolveInfo resolveInfo = pm.resolveActivity(intent, 0);
    //     if (resolveInfo != null) {
    //         final ActivityInfo activityInfo = resolveInfo.activityInfo;
    //         final String title = activityInfo.loadLabel(pm).toString();
    //         Drawable icon = activityInfo.loadIcon(pm);
    //
    //         if (title != null && title.length() > 0 && icon != null) {
    //             final TextView tv = mIcons[index];
    //             tv.setText(title);
    //             icon = iconUtilities.createIconDrawable(icon);
    //             tv.setCompoundDrawables(null, icon, null, null);
    //             RecentTag tag = new RecentTag();
    //             tag.info = info;
    //             tag.intent = intent;
    //             tv.setTag(tag);
    //             tv.setVisibility(View.VISIBLE);
    //             tv.setPressed(false);
    //             tv.clearFocus();
    //             ++index;
    //         }
    //     }
    // }
    //
    // // handle the case of "no icons to show"
    // mNoAppsText.setVisibility((index == 0) ? View.VISIBLE : View.GONE);
    //
    // // hide the rest
    // for (; index < NUM_BUTTONS; ++index) {
    //     mIcons[index].setVisibility(View.GONE);
    // }
    assert(0);
}

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos


