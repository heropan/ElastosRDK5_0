
#ifndef __ELASTOS_DROID_APP_BACKUP_CFILEBACKUPHELPERBASE_H__
#define __ELASTOS_DROID_APP_BACKUP_CFILEBACKUPHELPERBASE_H__

#include "_CFileBackupHelperBase.h"
#include "ext/frameworkext.h"
#include "app/backup/FileBackupHelperBase.h"

using Elastos::IO::IFile;
using Elastos::IO::IFileDescriptor;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::IParcelFileDescriptor;

namespace Elastos {
namespace Droid {
namespace App {
namespace Backup {

CarClass(CFileBackupHelperBase), public FileBackupHelperBase
{
public:
    ~CFileBackupHelperBase();

    /** @hide */
    CARAPI constructor(
        /* [in] */ IContext* constext);

    CARAPI WriteNewStateDescription(
        /* [in] */ IParcelFileDescriptor* fd);


};

} // namespace Backup
} // namespace App
} // namespace Droid
} // namespace Elastos
#endif
