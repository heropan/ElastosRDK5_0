
#include "elastos/droid/bluetooth/le/TruncatedFilter.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {
namespace LE {

//=====================================================================
//                           TruncatedFilter
//=====================================================================
CAR_INTERFACE_IMPL(TruncatedFilter, Object, ITruncatedFilter);

TruncatedFilter::TruncatedFilter()
{
}

TruncatedFilter::TruncatedFilter(
    /* [in] */ IScanFilter* filter,
    /* [in] */ IList* storageDescriptors)
{
    // ==================before translated======================
    // mFilter = filter;
    // mStorageDescriptors = storageDescriptors;
}

ECode TruncatedFilter::GetFilter(
    /* [out] */ IScanFilter** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mFilter;
    assert(0);
    return NOERROR;
}

ECode TruncatedFilter::GetStorageDescriptors(
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mStorageDescriptors;
    assert(0);
    return NOERROR;
}

} // namespace LE
} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos
