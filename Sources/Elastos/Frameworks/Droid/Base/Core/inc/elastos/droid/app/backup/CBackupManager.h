#ifndef __ELASTOS_DROID_APP_BACKUP_CBACKUPMANAGER_H__
#define __ELASTOS_DROID_APP_BACKUP_CBACKUPMANAGER_H__

#include "_Elastos_Droid_App_Backup_CBackupManager.h"
#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::App::Backup::IIBackupManager;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace App {
namespace Backup {

CarClass(CBackupManager)
{
public:
    CBackupManager();

    ~CBackupManager();

    CARAPI constructor(
        /* [in] */ IContext * context);

    /**
     * Notifies the Android backup system that your application wishes to back up
     * new changes to its data.  A backup operation using your application's
     * {@link android.app.backup.BackupAgent} subclass will be scheduled when you
     * call this method.
     */
    CARAPI DataChanged();

    /**
     * Convenience method for callers who need to indicate that some other package
     * needs a backup pass.  This can be useful in the case of groups of packages
     * that share a uid.
     * <p>
     * This method requires that the application hold the "android.permission.BACKUP"
     * permission if the package named in the argument does not run under the same uid
     * as the caller.
     *
     * @param packageName The package name identifying the application to back up.
     */
    static CARAPI DataChanged(
        /* [in] */ const String & packageName);

    /**
     * Restore the calling application from backup.  The data will be restored from the
     * current backup dataset if the application has stored data there, or from
     * the dataset used during the last full device setup operation if the current
     * backup dataset has no matching data.  If no backup data exists for this application
     * in either source, a nonzero value will be returned.
     *
     * <p>If this method returns zero (meaning success), the OS will attempt to retrieve
     * a backed-up dataset from the remote transport, instantiate the application's
     * backup agent, and pass the dataset to the agent's
     * {@link android.app.backup.BackupAgent#onRestore(BackupDataInput, int, android.os.ParcelFileDescriptor) onRestore()}
     * method.
     *
     * @param observer The {@link RestoreObserver} to receive callbacks during the restore
     * operation. This must not be null.
     *
     * @return Zero on success; nonzero on error.
     */
    CARAPI RequestRestore(
        /* [in] */ IRestoreObserver * observer,
        /* [out] */ Int32 * result);

    /**
     * Begin the process of restoring data from backup.  See the
     * {@link android.app.backup.RestoreSession} class for documentation on that process.
     * @hide
     */
    CARAPI BeginRestoreSession(
        /* [out] */ IRestoreSession **session);

public:
    static const String TAG;
    AutoPtr<IContext> mContext;
    static AutoPtr<IIBackupManager> sService;

private:
    static CARAPI_(void) CheckServiceBinder();
};

}
}
}
}
#endif // __ELASTOS_DROID_APP_BACKUP_CBACKUPMANAGER_H__
