#include "app/backup/CFullBackupDataOutput.h"
#include "app/backup/CBackupDataOutput.h"

using Elastos::Droid::App::Backup::CBackupDataOutput;
using Elastos::Droid::App::Backup::IBackupDataOutput;

namespace Elastos {
namespace Droid {
namespace App {
namespace Backup {

ECode CFullBackupDataOutput::constructor(
    /* [in] */ IParcelFileDescriptor* fdes)
{
    assert(fdes != NULL);
    AutoPtr<IFileDescriptor>  fd;
    fdes->GetFileDescriptor((IFileDescriptor**)&fd);
    return CBackupDataOutput::New(fd, (IBackupDataOutput**)&mData);
}

ECode CFullBackupDataOutput::GetData(
    /* [out] */ IBackupDataOutput** data)
{
    *data = mData;
    REFCOUNT_ADD(*data);
    return NOERROR;
}

} // namespace Backup
} // namespace App
} // namespace Droid
} // namespace Elastos
