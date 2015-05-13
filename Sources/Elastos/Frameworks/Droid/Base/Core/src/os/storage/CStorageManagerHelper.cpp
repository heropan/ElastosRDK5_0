
#include "os/storage/CStorageManagerHelper.h"
#include "os/storage/CStorageManager.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Os {
namespace Storage {

/** {@hide} */
ECode CStorageManagerHelper::From(
    /* [in] */ IContext* context,
    /* [out] */ IStorageManager** manager)
{
    VALIDATE_NOT_NULL(manager);

    AutoPtr<IInterface> service;
    context->GetSystemService(IContext::STORAGE_SERVICE, (IInterface**)&service);
    *manager = IStorageManager::Probe(service);
    INTERFACE_ADDREF(*manager);

    return NOERROR;
}

/** {@hide} */
ECode CStorageManagerHelper::GetPrimaryVolume(
    /* [in] */ ArrayOf<IStorageVolume*>* volumes,
    /* [out] */ IStorageVolume** result )
{
    VALIDATE_NOT_NULL(result);

    for (Int32 i = 0; i < volumes->GetLength(); i++) {
        AutoPtr<IStorageVolume> volume = (*volumes)[i];
        Boolean primary = FALSE;
        if (volume->IsPrimary(&primary), primary) {
            *result = volume;
            INTERFACE_ADDREF(*result);
            return NOERROR;
        }
    }
    Logger::W(CStorageManager::TAG, "No primary storage defined");
    *result = NULL;
    return NOERROR;
}

} // namespace Storage
} // namespace Os
} // namepsace Droid
} // namespace Elastos