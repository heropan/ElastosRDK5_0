#ifndef __ELASTOS_DROID_APP_BACKUP_CSHAREDPREFERENCESBACKUPHELPER_H__
#define __ELASTOS_DROID_APP_BACKUP_CSHAREDPREFERENCESBACKUPHELPER_H__

#include "_Elastos_Droid_App_Backup_CSharedPreferencesBackupHelper.h"
#include "elastos/droid/ext/frameworkext.h"
#include "app/backup/FileBackupHelperBase.h"

using Elastos::Droid::Content::IContext;

namespace Elastos{
namespace Droid{
namespace App{
namespace Backup{

CarClass(CSharedPreferencesBackupHelper), public FileBackupHelperBase
{
public:

    /** @hide */
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ ArrayOf<String>* prefGroups);

    CARAPI PerformBackup(
        /* [in] */ IParcelFileDescriptor* oldState,
        /* [in] */ IBackupDataOutput* data,
        /* [in] */ IParcelFileDescriptor* newState);

    CARAPI RestoreEntity(
        /* [in] */ IBackupDataInputStream* data);

    CARAPI WriteNewStateDescription(
        /* [in] */ IParcelFileDescriptor* fd);

private:
    static const String TAG;
    static const Boolean DEBUG;
    AutoPtr<IContext> mContext;
    AutoPtr<ArrayOf<String> > mPrefGroups;
};

} // namespace Backup
} // namespace App
} // namespace Droid
} // namespace Elastos
#endif
