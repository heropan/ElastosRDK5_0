#ifndef __ELASTOS_DROID_APP_CKEYGUARDMANAGER_H__
#define __ELASTOS_DROID_APP_CKEYGUARDMANAGER_H__

#include "ext/frameworkdef.h"
#include "_Elastos_Droid_App_CKeyguardManager.h"

using Elastos::Droid::Os::IBinder;
using Elastos::Droid::View::IIWindowManager;

namespace Elastos {
namespace Droid {
namespace App {

/**
 * Class that can be used to lock and unlock the keyboard. Get an instance of this
 * class by calling {@link android.content.Context#getSystemService(java.lang.String)}
 * with argument {@link android.content.Context#KEYGUARD_SERVICE}. The
 * actual class to control the keyboard locking is
 * {@link android.app.KeyguardManager.KeyguardLock}.
 */
CarClass(CKeyguardManager)
{
public:
    class KeyguardLock
        : public ElRefBase
        , public IKeyguardManagerKeyguardLock
    {
    public:
        KeyguardLock(
            /* [in] */ const String& tag,
            /* [in] */ CKeyguardManager* owner)
            : mTag(tag)
            , mOwner(owner)
        {}

        CAR_INTERFACE_DECL();

        /**
         * Disable the keyguard from showing.  If the keyguard is currently
         * showing, hide it.  The keyguard will be prevented from showing again
         * until {@link #reenableKeyguard()} is called.
         *
         * A good place to call this is from {@link android.app.Activity#onResume()}
         *
         * Note: This call has no effect while any {@link android.app.admin.DevicePolicyManager}
         * is enabled that requires a password.
         *
         * <p>This method requires the caller to hold the permission
         * {@link android.Manifest.permission#DISABLE_KEYGUARD}.
         *
         * @see #reenableKeyguard()
         */
        CARAPI DisableKeyguard();

        /**
         * Reenable the keyguard.  The keyguard will reappear if the previous
         * call to {@link #disableKeyguard()} caused it to be hidden.
         *
         * A good place to call this is from {@link android.app.Activity#onPause()}
         *
         * Note: This call has no effect while any {@link android.app.admin.DevicePolicyManager}
         * is enabled that requires a password.
         *
         * <p>This method requires the caller to hold the permission
         * {@link android.Manifest.permission#DISABLE_KEYGUARD}.
         *
         * @see #disableKeyguard()
         */
        CARAPI ReenableKeyguard();

    private:
        AutoPtr<IBinder> mToken;
        String mTag;
        AutoPtr<CKeyguardManager> mOwner;
    };

public:
    CARAPI constructor();

    /**
     * @deprecated Use {@link android.view.WindowManager.LayoutParams#FLAG_DISMISS_KEYGUARD}
     * and/or {@link android.view.WindowManager.LayoutParams#FLAG_SHOW_WHEN_LOCKED}
     * instead; this allows you to seamlessly hide the keyguard as your application
     * moves in and out of the foreground and does not require that any special
     * permissions be requested.
     *
     * Enables you to lock or unlock the keyboard. Get an instance of this class by
     * calling {@link android.content.Context#getSystemService(java.lang.String) Context.getSystemService()}.
     * This class is wrapped by {@link android.app.KeyguardManager KeyguardManager}.
     * @param tag A tag that informally identifies who you are (for debugging who
     *   is disabling he keyguard).
     *
     * @return A {@link KeyguardLock} handle to use to disable and reenable the
     *   keyguard.
     */
    CARAPI NewKeyguardLock(
        /* [in] */ const String& tag,
        /* [out] */ IKeyguardManagerKeyguardLock** keyguardLock);

    /**
     * Return whether the keyguard is currently locked.
     *
     * @return true if keyguard is locked.
     */
    CARAPI IsKeyguardLocked(
        /* [out] */ Boolean* isLocked);

    /**
     * Return whether the keyguard requires a password to unlock.
     *
     * @return true if keyguard is secure.
     */
    CARAPI IsKeyguardSecure(
        /* [out] */ Boolean* isSecure);

    /**
     * If keyguard screen is showing or in restricted key input mode (i.e. in
     * keyguard password emergency screen). When in such mode, certain keys,
     * such as the Home key and the right soft keys, don't work.
     *
     * @return true if in keyguard restricted input mode.
     *
     * @see android.view.WindowManagerPolicy#inKeyguardRestrictedKeyInputMode
     */
    CARAPI InKeyguardRestrictedInputMode(
        /* [out] */ Boolean* isInMode);

    /**
     * @deprecated Use {@link android.view.WindowManager.LayoutParams#FLAG_DISMISS_KEYGUARD}
     * and/or {@link android.view.WindowManager.LayoutParams#FLAG_SHOW_WHEN_LOCKED}
     * instead; this allows you to seamlessly hide the keyguard as your application
     * moves in and out of the foreground and does not require that any special
     * permissions be requested.
     *
     * Exit the keyguard securely.  The use case for this api is that, after
     * disabling the keyguard, your app, which was granted permission to
     * disable the keyguard and show a limited amount of information deemed
     * safe without the user getting past the keyguard, needs to navigate to
     * something that is not safe to view without getting past the keyguard.
     *
     * This will, if the keyguard is secure, bring up the unlock screen of
     * the keyguard.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#DISABLE_KEYGUARD}.
     *
     * @param callback Let's you know whether the operation was succesful and
     *   it is safe to launch anything that would normally be considered safe
     *   once the user has gotten past the keyguard.
     */
    CARAPI ExitKeyguardSecurely(
        /* [in] */ IKeyguardManagerOnKeyguardExitResult* cb);

public:
    AutoPtr<IIWindowManager> mWM;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_APP_CKEYGUARDMANAGER_H__
