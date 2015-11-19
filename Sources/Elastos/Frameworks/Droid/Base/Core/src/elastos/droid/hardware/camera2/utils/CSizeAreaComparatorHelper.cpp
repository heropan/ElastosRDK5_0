
#include "elastos/droid/hardware/camera2/utils/CSizeAreaComparatorHelper.h"
#include "elastos/droid/hardware/camera2/utils/SizeAreaComparator.h"

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Camera2 {
namespace Utils {

CAR_INTERFACE_IMPL(CSizeAreaComparatorHelper, Singleton, ISizeAreaComparatorHelper)

CAR_SINGLETON_IMPL(CSizeAreaComparatorHelper)

ECode CSizeAreaComparatorHelper::FindLargestByArea(
    /* [in] */ IList* sizes,
    /* [out] */ ISize** outsz)
{
    VALIDATE_NOT_NULL(outsz);

    return SizeAreaComparator::FindLargestByArea(sizes, outsz);
}

} // namespace Utils
} // namespace Camera2
} // namespace Hardware
} // namespace Droid
} // namespace Elastos