
#include "elastos/droid/hardware/camera2/utils/SizeAreaComparator.h"
#include "elastos/droid/hardware/camera2/utils/CSizeAreaComparator.h"
#include "elastos/droid/internal/utility/Preconditions.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::Internal::Utility::Preconditions;
using Elastos::Droid::Hardware::Camera2::Utils::CSizeAreaComparator;
using Elastos::Core::EIID_IComparator;
using Elastos::Utility::ICollections;
using Elastos::Utility::CCollections;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Camera2 {
namespace Utils {

CAR_INTERFACE_IMPL_2(SizeAreaComparator, Object, ISizeAreaComparator, IComparator)

ECode SizeAreaComparator::Compare(
    /* [in] */ IInterface* lhs,
    /* [in] */ IInterface* rhs,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    AutoPtr<ISize> size = ISize::Probe(lhs);
    AutoPtr<ISize> size2 = ISize::Probe(rhs);

    FAIL_RETURN(Preconditions::CheckNotNull(size, String("size must not be null")))
    FAIL_RETURN(Preconditions::CheckNotNull(size2, String("size2 must not be null")))

    Boolean _result;
    size->Equals(size2, &_result);
    if (_result) {
        *result = 0;
        return NOERROR;
    }

    Int32 width;
    size->GetWidth(&width);
    Int32 width2;
    size2->GetWidth(&width2);
    Int32 height1;
    size->GetHeight(&height1);
    Int64 area = width * height1;
    Int32 height2;
    size2->GetHeight(&height2);
    Int64 area2 = width2 * height2;

    if (area == area2) {
        *result = (width > width2) ? 1 : -1;
        return NOERROR;
    }

    *result = (area > area2) ? 1 : -1;
    return NOERROR;
}

ECode SizeAreaComparator::FindLargestByArea(
    /* [in] */ IList* sizes,
    /* [out] */ ISize** outsz)
{
    VALIDATE_NOT_NULL(outsz);
    *outsz = NULL;

    FAIL_RETURN(Preconditions::CheckNotNull(sizes, String("sizes must not be null")))

    AutoPtr<ISizeAreaComparator> comparator;
    CSizeAreaComparator::New((ISizeAreaComparator**)&comparator);
    AutoPtr<IInterface> obj;

    AutoPtr<ICollections> helper;
    CCollections::AcquireSingleton((ICollections**)&helper);
    helper->Max(ICollection::Probe(sizes), IComparator::Probe(comparator), (IInterface**)&obj);
    *outsz = ISize::Probe(obj);
    REFCOUNT_ADD(*outsz);
    return NOERROR;
}

} // namespace Utils
} // namespace Camera2
} // namespace Hardware
} // namespace Droid
} // namespace Elastos