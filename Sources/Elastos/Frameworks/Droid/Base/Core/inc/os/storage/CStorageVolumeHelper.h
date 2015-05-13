
#ifndef __CSTORAGEVOLUMEHELPER_H__
#define __CSTORAGEVOLUMEHELPER_H__

#include "_CStorageVolumeHelper.h"
#include "os/storage/CStorageVolume.h"
#include "ext/frameworkdef.h"

using Elastos::IO::IFile;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::Storage::IStorageVolume;

namespace Elastos {
namespace Droid {
namespace Os {
namespace Storage {

CarClass(CStorageVolumeHelper)
{
public:
    FromTemplate(
        /* [in] */ IStorageVolume* temp,
        /* [in] */ IFile* path,
        /* [in] */ IUserHandle* owner,
        /* [out] */ IStorageVolume** volume);
};

} // namespace Storage
} // namespace Os
} // namepsace Droid
} // namespace Elastos


#endif // __CSTORAGEVOLUMEHELPER_H__
