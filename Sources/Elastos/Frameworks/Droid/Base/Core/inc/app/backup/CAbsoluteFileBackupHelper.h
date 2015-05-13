#ifndef __CABSOLUTEFILEBACKUPHELPER_H__
#define __CABSOLUTEFILEBACKUPHELPER_H__

#include "_CAbsoluteFileBackupHelper.h"
#include "ext/frameworkext.h"
#include "app/backup/FileBackupHelperBase.h"

using Elastos::Droid::Content::IContext;

namespace Elastos{
namespace Droid{
namespace App{
namespace Backup{

CarClass(CAbsoluteFileBackupHelper), public FileBackupHelperBase
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
    AutoPtr<ArrayOf<String> > mFiles;
};
} // namespace Backup
} // namespace App
} // namespace Droid
} // namespace Elastos

#endif
