#include "elastos/droid/view/accessibility/CCollectionInfoHelper.h"
#include "elastos/droid/view/accessibility/CCollectionInfo.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

CAR_INTERFACE_IMPL(CCollectionInfoHelper, Singleton, ICollectionInfoHelper)

CAR_SINGLETON_IMPL(CCollectionInfoHelper)

ECode CCollectionInfoHelper::Obtain(
    /* [in] */ ICollectionInfo* other,
    /* [out] */ ICollectionInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return CCollectionInfo::Obtain(other, info);
}

ECode CCollectionInfoHelper::Obtain(
    /* [in] */ Int32 rowCount,
    /* [in] */ Int32 columnCount,
    /* [in] */ Boolean hierarchical,
    /* [out] */ ICollectionInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return CCollectionInfo::Obtain(rowCount, columnCount, hierarchical, info);
}

ECode CCollectionInfoHelper::Obtain(
    /* [in] */ Int32 rowCount,
    /* [in] */ Int32 columnCount,
    /* [in] */ Boolean hierarchical,
    /* [in] */ Int32 selectionMode,
    /* [out] */ ICollectionInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return CCollectionInfo::Obtain(rowCount, columnCount, hierarchical, selectionMode, info);
}

} // Accessibility
} // View
} // Droid
} // Elastos
