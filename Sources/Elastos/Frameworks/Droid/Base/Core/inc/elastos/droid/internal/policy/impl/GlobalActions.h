#ifndef __ELASTOS_DROID_INTERNAL_POLICY_IMPL_GLOBALACTIONS_H__
#define __ELASTOS_DROID_INTERNAL_POLICY_IMPL_GLOBALACTIONS_H__

#include "elastos/droid/ext/frameworkext.h"


namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

/**
  * Helper to show the global actions dialog.  Each item is an {@link Action} that
  * may show depending on whether the keyguard is showing, and whether the device
  * is provisioned.
  */
class GlobalActions
    : public Object
    , public IDialogInterfaceOnDismissListener
    , public IDialogInterfaceOnClickListener
{
private:
    class PowerAction
        : public Object
        , public SinglePressAction
        , public LongPressAction
    {
    public:
        // @Override
        CARAPI_(Boolean) OnLongPress();

        // @Override
        CARAPI_(Boolean) ShowDuringKeyguard();

        // @Override
        CARAPI_(Boolean) ShowBeforeProvisioning();

        // @Override
        CARAPI OnPress();

    private:
        PowerAction();
    };

    class GlobalActionsDialog
        : public Object
        , public IDialog
        , public IDialogInterface
    {
    private:
        class InnerRunnable3
            : public Object
            , public Runnable
        {
        public:
            InnerRunnable3(
                /* [in] */ GlobalActionsDialog* owner);

            // @Override
            CARAPI Run();

        private:
            GlobalActionsDialog* mOwner;
        };

    public:
        GlobalActionsDialog(
            /* [in] */ IContext* context,
            /* [in] */ IAlertParams* params);

        // @Override
        CARAPI_(Boolean) DispatchTouchEvent(
            /* [in] */ IMotionEvent* event);

        virtual CARAPI GetListView(
            /* [out] */ ListView** result);

        // @Override
        CARAPI_(Boolean) DispatchPopulateAccessibilityEvent(
            /* [in] */ IAccessibilityEvent* event);

        // @Override
        CARAPI_(Boolean) OnKeyDown(
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event);

        // @Override
        CARAPI_(Boolean) OnKeyUp(
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event);

    protected:
        // @Override
        CARAPI_(void) OnStart();

        // @Override
        CARAPI_(void) OnStop();

        // @Override
        CARAPI_(void) OnCreate(
            /* [in] */ IBundle* savedInstanceState);

    private:
        static CARAPI_(Int32) GetDialogTheme(
            /* [in] */ IContext* context);

    private:
        /*const*/ AutoPtr<IContext> mContext;
        /*const*/ Int32 mWindowTouchSlop;
        /*const*/ AutoPtr<IAlertController> mAlert;
        /*const*/ AutoPtr<MyAdapter> mAdapter;
        AutoPtr<EnableAccessibilityController> mEnableAccessibilityController;
        Boolean mIntercepted;
        Boolean mCancelOnUp;
    };

private:
    class InnerIntent1
        : public Object
        , public IIntent
    {
    public:
        InnerIntent1(
            /* [in] */ GlobalActions* owner);

        ChangeAirplaneModeSystemSetting(
            /* [in] */  on);

    private:
        GlobalActions* mOwner;
    };

    class InnerDialogInterfaceOnClickListener1
        : public Object
        , public DialogInterface::OnClickListener
    {
    private:
        class InnerRunnable1
            : public Object
            , public Runnable
        {
        public:
            InnerRunnable1(
                /* [in] */ InnerDialogInterfaceOnClickListener1* owner);

            // @Override public void run() {
            Catch(
                /* [in] */ IRemoteException* e);

        private:
            AutoPtr<}

 InnerDialogInterfaceOnClickListener1*> mOwner;
        };

    public:
        InnerDialogInterfaceOnClickListener1(
            /* [in] */ GlobalActions* owner);

        // @Override
        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        GlobalActions* mOwner;
    };

    class InnerSinglePressAction1
        : public Object
        , public SinglePressAction
    {
    public:
        InnerSinglePressAction1(
            /* [in] */ GlobalActions* owner);

        // @Override
        CARAPI OnPress();

        // @Override
        CARAPI_(Boolean) ShowDuringKeyguard();

        // @Override
        CARAPI_(Boolean) ShowBeforeProvisioning();

    private:
        GlobalActions* mOwner;
    };

    class InnerSinglePressAction3
        : public Object
        , public SinglePressAction
    {
    public:
        InnerSinglePressAction3(
            /* [in] */ GlobalActions* owner);

        // @Override
        CARAPI OnPress();

        // @Override
        CARAPI_(Boolean) ShowDuringKeyguard();

        // @Override
        CARAPI_(Boolean) ShowBeforeProvisioning();

    private:
        GlobalActions* mOwner;
    };

    /**
      * The adapter used for the list within the global actions dialog, taking
      * into account whether the keyguard is showing via
      * {@link GlobalActions#mKeyguardShowing} and whether the device is provisioned
      * via {@link GlobalActions#mDeviceProvisioned}.
      */
    class MyAdapter
        : public Object
        , public IBaseAdapter
    {
    public:
        virtual CARAPI_(Int32) GetCount();

        // @Override
        CARAPI_(Boolean) IsEnabled(
            /* [in] */ Int32 position);

        // @Override
        CARAPI_(Boolean) AreAllItemsEnabled();

        virtual CARAPI GetItem(
            /* [in] */ Int32 position,
            /* [out] */ Action** result);

        virtual CARAPI GetItemId(
            /* [in] */ Int32 position,
            /* [out] */ Int64* result);

        virtual CARAPI GetView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent,
            /* [out] */ View** result);
    };

    // note: the scheme below made more sense when we were planning on having
    // 8 different things in the global actions dialog.  seems overkill with
    // only 3 items now, but may as well keep this flexible approach so it will
    // be easy should someone decide at the last minute to include something
    // else, such as 'enable wifi', or 'enable bluetooth'
    /**
      * What each item in the global actions dialog must be able to support.
      */
    class Action
    {
    public:
        /**
          * @return Text that will be announced when dialog is created.  null
          *     for none.
          */
        virtual CARAPI_(AutoPtr<CharSequence>) GetLabelForAccessibility(
            /* [in] */ IContext* context) = 0;

        virtual CARAPI_(AutoPtr<IView>) Create(
            /* [in] */ IContext* context,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent,
            /* [in] */ ILayoutInflater* inflater) = 0;

        virtual CARAPI OnPress() = 0;

        /**
          * @return whether this action should appear in the dialog when the keygaurd
          *    is showing.
          */
        virtual CARAPI_(Boolean) ShowDuringKeyguard() = 0;

        /**
          * @return whether this action should appear in the dialog before the
          *   device is provisioned.
          */
        virtual CARAPI_(Boolean) ShowBeforeProvisioning() = 0;

        virtual CARAPI_(Boolean) IsEnabled() = 0;
    };

    /**
      * An action that also supports long press.
      */
    class LongPressAction
    {
    public:
        virtual CARAPI_(Boolean) OnLongPress() = 0;
    };

    /**
      * A single press action maintains no state, just responds to a press
      * and takes an action.
      */
    class SinglePressAction
        : public Object
        , public Action
    {
    public:
        virtual CARAPI IsEnabled(
            /* [out] */ Boolean* result);

        virtual CARAPI GetStatus(
            /* [out] */ String* result);

        virtual CARAPI OnPress() = 0;

        virtual CARAPI GetLabelForAccessibility(
            /* [in] */ IContext* context,
            /* [out] */ CharSequence** result);

        virtual CARAPI Create(
            /* [in] */ IContext* context,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent,
            /* [in] */ ILayoutInflater* inflater,
            /* [out] */ View** result);

    protected:
        SinglePressAction(
            /* [in] */ Int32 iconResId,
            /* [in] */ Int32 messageResId);

        SinglePressAction(
            /* [in] */ Int32 iconResId,
            /* [in] */ IDrawable* icon,
            /* [in] */ CharSequence* message);

        SinglePressAction(
            /* [in] */ Int32 iconResId,
            /* [in] */ CharSequence* message);

    private:
        /*const*/ Int32 mIconResId;
        /*const*/ AutoPtr<IDrawable> mIcon;
        /*const*/ Int32 mMessageResId;
        /*const*/ AutoPtr<CharSequence> mMessage;
    };

    /**
      * A toggle action knows whether it is on or off, and displays an icon
      * and status message accordingly.
      */
    class ToggleAction
        : public Object
        , public Action
    {
    public:
        /**
          * @param enabledIconResId The icon for when this action is on.
          * @param disabledIconResid The icon for when this action is off.
          * @param essage The general information message, e.g 'Silent Mode'
          * @param enabledStatusMessageResId The on status message, e.g 'sound disabled'
          * @param disabledStatusMessageResId The off status message, e.g. 'sound enabled'
          */
        ToggleAction(
            /* [in] */ Int32 enabledIconResId,
            /* [in] */ Int32 disabledIconResid,
            /* [in] */ Int32 message,
            /* [in] */ Int32 enabledStatusMessageResId,
            /* [in] */ Int32 disabledStatusMessageResId);

        /**
          * Override to make changes to resource IDs just before creating the
          * View.
          */
        virtual CARAPI WillCreate();

        // @Override
        CARAPI_(AutoPtr<CharSequence>) GetLabelForAccessibility(
            /* [in] */ IContext* context);

        virtual CARAPI Create(
            /* [in] */ IContext* context,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent,
            /* [in] */ ILayoutInflater* inflater,
            /* [out] */ View** result);

        virtual const CARAPI OnPress();

        virtual CARAPI IsEnabled(
            /* [out] */ Boolean* result);

        virtual CARAPI OnToggle(
            /* [in] */ Boolean on) = 0;

        virtual CARAPI UpdateState(
            /* [in] */ State* state);

    protected:
        /**
          * Implementations may override this if their state can be in on of the intermediate
          * states until some notification is received (e.g airplane mode is 'turning off' until
          * we know the wireless connections are back online
          * @param buttonOn Whether the button was turned on or off
          */
        virtual CARAPI_(void) ChangeStateFromPress(
            /* [in] */ Boolean buttonOn);

    protected:
        AutoPtr<State> mState;
        // prefs
        Int32 mEnabledIconResId;
        Int32 mDisabledIconResid;
        Int32 mMessageResId;
        Int32 mEnabledStatusMessageResId;
        Int32 mDisabledStatusMessageResId;
    };

    class SilentModeToggleAction
        : public Object
        , public ToggleAction
    {
    public:
        SilentModeToggleAction();

        virtual CARAPI OnToggle(
            /* [in] */ Boolean on);

        virtual CARAPI ShowDuringKeyguard(
            /* [out] */ Boolean* result);

        virtual CARAPI ShowBeforeProvisioning(
            /* [out] */ Boolean* result);
    };

    class SilentModeTriStateAction
        : public Object
        , public Action
        , public View::OnClickListener
    {
    public:
        SilentModeTriStateAction(
            /* [in] */ IContext* context,
            /* [in] */ IAudioManager* audioManager,
            /* [in] */ IHandler* handler);

        // @Override
        CARAPI_(AutoPtr<CharSequence>) GetLabelForAccessibility(
            /* [in] */ IContext* context);

        virtual CARAPI Create(
            /* [in] */ IContext* context,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent,
            /* [in] */ ILayoutInflater* inflater,
            /* [out] */ View** result);

        virtual CARAPI OnPress();

        virtual CARAPI ShowDuringKeyguard(
            /* [out] */ Boolean* result);

        virtual CARAPI ShowBeforeProvisioning(
            /* [out] */ Boolean* result);

        virtual CARAPI IsEnabled(
            /* [out] */ Boolean* result);

        virtual CARAPI WillCreate();

        virtual CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        CARAPI_(AutoPtr< ArrayOf<Int32> >) MiddleInitItemIds();

        CARAPI_(Int32) RingerModeToIndex(
            /* [in] */ Int32 ringerMode);

        CARAPI_(Int32) IndexToRingerMode(
            /* [in] */ Int32 index);

    private:
        AutoPtr< ArrayOf<Int32> > ITEM_IDS;
        /*const*/ AutoPtr<IAudioManager> mAudioManager;
        /*const*/ AutoPtr<IHandler> mHandler;
        /*const*/ AutoPtr<IContext> mContext;
    };

    class InnerBroadcastReceiver
        : public Object
        , public IBroadcastReceiver
    {
    public:
        InnerBroadcastReceiver(
            /* [in] */ GlobalActions* owner);

        virtual CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        GlobalActions* mOwner;
    };

    class InnerPhoneStateListener
        : public Object
        , public IPhoneStateListener
    {
    public:
        InnerPhoneStateListener(
            /* [in] */ GlobalActions* owner);

        // @Override
        CARAPI OnServiceStateChanged(
            /* [in] */ IServiceState* serviceState);

    private:
        GlobalActions* mOwner;
    };

    class InnerBroadcastReceiver1
        : public Object
        , public IBroadcastReceiver
    {
    public:
        InnerBroadcastReceiver1(
            /* [in] */ GlobalActions* owner);

        // @Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        GlobalActions* mOwner;
    };

    class InnerContentObserver
        : public Object
        , public IContentObserver
    {
    public:
        InnerContentObserver(
            /* [in] */ GlobalActions* owner);

        // @Override
        CARAPI OnChange(
            /* [in] */ Boolean selfChange);

    private:
        GlobalActions* mOwner;
    };

    class InnerHandler
        : public Object
        , public IHandler
    {
    public:
        InnerHandler(
            /* [in] */ GlobalActions* owner);

        virtual CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        GlobalActions* mOwner;
    };

public:
    CAR_INTERFACE_DECL()

    /**
      * @param context everything needs a context :(
      */
    GlobalActions(
        /* [in] */ IContext* context,
        /* [in] */ IWindowManagerFuncs* windowManagerFuncs);

    /**
      * Show the global actions dialog (creating if necessary)
      * @param keyguardShowing True if keyguard is showing
      */
    virtual CARAPI ShowDialog(
        /* [in] */ Boolean keyguardShowing,
        /* [in] */ Boolean isDeviceProvisioned);

    /** {@inheritDoc} */
    virtual CARAPI OnDismiss(
        /* [in] */ IDialogInterface* dialog);

    /** {@inheritDoc} */
    virtual CARAPI OnClick(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 which);

private:
    CARAPI_(void) AwakenIfNecessary();

    CARAPI_(void) HandleShow();

    /**
      * Create the global actions dialog.
      * @return A new dialog.
      */
    CARAPI_(AutoPtr<GlobalActionsDialog>) CreateDialog();

    CARAPI_(AutoPtr<Action>) GetBugReportAction();

    CARAPI_(AutoPtr<Action>) GetSettingsAction();

    CARAPI_(AutoPtr<Action>) GetLockdownAction();

    CARAPI_(AutoPtr<IUserInfo>) GetCurrentUser();

    CARAPI_(Boolean) IsCurrentUserOwner();

    CARAPI_(void) AddUsersToMenu(
        /* [in] */ IArrayList<Action*>* items);

    CARAPI_(void) PrepareDialog();

    CARAPI_(void) RefreshSilentMode();

    CARAPI_(void) OnAirplaneModeChanged();

    /**
      * Change the airplane mode system setting
      */
    CARAPI_(void) ChangeAirplaneModeSystemSetting(
        /* [in] */ Boolean on);

public:
    AutoPtr<IPhoneStateListener> mPhoneStateListener;

private:
    static const String TAG;
    static const Boolean SHOW_SILENT_TOGGLE;
    /* Valid settings for global actions keys.
      * see config.xml config_globalActionList */
    static const String GLOBAL_ACTION_KEY_POWER;
    static const String GLOBAL_ACTION_KEY_AIRPLANE;
    static const String GLOBAL_ACTION_KEY_BUGREPORT;
    static const String GLOBAL_ACTION_KEY_SILENT;
    static const String GLOBAL_ACTION_KEY_USERS;
    static const String GLOBAL_ACTION_KEY_SETTINGS;
    static const String GLOBAL_ACTION_KEY_LOCKDOWN;
    /*const*/ AutoPtr<IContext> mContext;
    /*const*/ AutoPtr<IWindowManagerFuncs> mWindowManagerFuncs;
    /*const*/ AutoPtr<IAudioManager> mAudioManager;
    /*const*/ AutoPtr<IIDreamManager> mDreamManager;
    AutoPtr< IArrayList<Action> > mItems;
    AutoPtr<GlobalActionsDialog> mDialog;
    AutoPtr<Action> mSilentModeAction;
    AutoPtr<ToggleAction> mAirplaneModeOn;
    AutoPtr<MyAdapter> mAdapter;
    Boolean mKeyguardShowing;
    Boolean mDeviceProvisioned;
    AutoPtr<ToggleAction::State> mAirplaneState;
    Boolean mIsWaitingForEcmExit;
    Boolean mHasTelephony;
    Boolean mHasVibrator;
    /*const*/ Boolean mShowSilentToggle;
    AutoPtr<-
 GlobalActions*> mOwner;
    AutoPtr<IBroadcastReceiver> mBroadcastReceiver;
    AutoPtr<IBroadcastReceiver> mRingerModeReceiver;
    AutoPtr<IContentObserver> mAirplaneModeObserver;
    static const Int32 MESSAGE_DISMISS = 0;
    static const Int32 MESSAGE_REFRESH = 1;
    static const Int32 MESSAGE_SHOW = 2;
    static const Int32 DIALOG_DISMISS_DELAY = 300;
    // ms
    AutoPtr<IHandler> mHandler;
};

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_POLICY_IMPL_GLOBALACTIONS_H__

