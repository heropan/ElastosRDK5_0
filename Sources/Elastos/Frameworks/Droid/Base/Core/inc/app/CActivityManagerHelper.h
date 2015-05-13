
#ifndef __CACTIVITYMANAGERHELPER_H__
#define __CACTIVITYMANAGERHELPER_H__

#include "_CActivityManagerHelper.h"


namespace Elastos {
namespace Droid {
namespace App {

CarClass(CActivityManagerHelper)
{
    /** @hide */
    CARAPI GetMemoryClass(
        /* [out] */ Int32* cls);

    /** @hide */
    CARAPI GetLargeMemoryClass(
        /* [out] */ Int32* cls);

    /**
     * Used by persistent processes to determine if they are running on a
     * higher-end device so should be okay using hardware drawing acceleration
     * (which tends to consume a lot more RAM).
     * @hide
     */
    CARAPI IsHighEndGfx(
        /* [out] */ Boolean* result);

    /**
     * Use to decide whether the running device can be considered a "large
     * RAM" device.  Exactly what memory limit large RAM is will vary, but
     * it essentially means there is plenty of RAM to have lots of background
     * processes running under decent loads.
     * @hide
     */
    CARAPI IsLargeRAM(
        /* [out] */ Boolean* result);

    /**
     * Return global memory state information for the calling process.  This
     * does not fill in all fields of the {@link RunningAppProcessInfo}.  The
     * only fields that will be filled in are
     * {@link RunningAppProcessInfo#pid},
     * {@link RunningAppProcessInfo#uid},
     * {@link RunningAppProcessInfo#lastTrimLevel},
     * {@link RunningAppProcessInfo#importance},
     * {@link RunningAppProcessInfo#lru}, and
     * {@link RunningAppProcessInfo#importanceReasonCode}.
     */
    CARAPI GetMyMemoryState(
        /* [out] */ IActivityManagerRunningAppProcessInfo** outState);

    /**
     * Returns "true" if the user interface is currently being messed with
     * by a monkey.
     */
    CARAPI IsUserAMonkey(
        /* [out] */ Boolean* result);

    /**
     * Returns "true" if device is running in a test harness.
     */
    CARAPI IsRunningInTestHarness(
        /* [out] */ Boolean* result);

    /** @hide */
    CARAPI CheckComponentPermission(
        /* [in] */ const String& permission,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 owningUid,
        /* [in] */ Boolean exported,
        /* [out] */ Int32* value);

    /** @hide */
    CARAPI CheckUidPermission(
        /* [in] */ const String& permission,
        /* [in] */ Int32 uid,
        /* [out] */ Int32* value);

    /** @hide */
    CARAPI HandleIncomingUser(
        /* [in] */ Int32 callingPid,
        /* [in] */ Int32 callingUid,
        /* [in] */ Int32 userId,
        /* [in] */ Boolean allowAll,
        /* [in] */ Boolean requireFull,
        /* [in] */ const String& name,
        /* [in] */ const String& callerPackage,
        /* [out] */ Int32* value);

    /** @hide */
    CARAPI GetCurrentUser(
        /* [out] */ Int32* currentUser);
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__CACTIVITYMANAGERHELPER_H__
