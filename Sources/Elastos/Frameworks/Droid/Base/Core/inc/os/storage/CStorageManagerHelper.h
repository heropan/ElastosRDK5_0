
#ifndef __CSTORAGEMANAGERHELPER_H__
#define __CSTORAGEMANAGERHELPER_H__

#include "_CStorageManagerHelper.h"
#include "ext/frameworkext.h"

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Os {
namespace Storage {

CarClass(CStorageManagerHelper)
{
public:
    CARAPI From(
        /* [in] */IContext* context,
        /* [out] */ IStorageManager** manager);

    CARAPI GetPrimaryVolume(
        /* [in] */ ArrayOf<IStorageVolume*>* volumes,
        /* [out] */ IStorageVolume** result );
};

} // namespace Storage
} // namespace Os
} // namepsace Droid
} // namespace Elastos

#endif // __CSTORAGEMANAGERHELPER_H__