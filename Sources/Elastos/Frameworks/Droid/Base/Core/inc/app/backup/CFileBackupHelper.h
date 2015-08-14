#ifndef __ELASTOS_DROID_APP_BACKUP_CFILEBACKUPHELPER_H__
#define __ELASTOS_DROID_APP_BACKUP_CFILEBACKUPHELPER_H__

#include "_CFileBackupHelper.h"
#include "ext/frameworkext.h"
#include "app/backup/FileBackupHelperBase.h"

using Elastos::Droid::Content::IContext;

namespace Elastos{
namespace Droid{
namespace App{
namespace Backup{

CarClass(CFileBackupHelper), public FileBackupHelperBase
{
public:

    /** @hide */
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ ArrayOf<String>* files);

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
    AutoPtr<IFile> mFileDir;
    AutoPtr<ArrayOf<String> > mFiles;
};

} // namespace Backup
} // namespace App
} // namespace Droid
} // namespace Elastos

#endif
