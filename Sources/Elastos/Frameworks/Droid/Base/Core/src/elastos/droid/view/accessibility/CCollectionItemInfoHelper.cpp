#include "elastos/droid/view/accessibility/CCollectionItemInfoHelper.h"
#include "elastos/droid/view/accessibility/CCollectionItemInfo.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

CAR_INTERFACE_IMPL(CCollectionItemInfoHelper, Singleton, ICollectionItemInfoHelper)

CAR_SINGLETON_IMPL(CCollectionItemInfoHelper)

ECode CCollectionItemInfoHelper::Obtain(
    /* [in] */ ICollectionItemInfo* other,
    /* [out] */ ICollectionItemInfo** info)
{
    VALIDATE_NOT_NULL(info);

    return CCollectionItemInfo::Obtain(other, info);
}

ECode CCollectionItemInfoHelper::Obtain(
    /* [in] */ Int32 rowIndex,
    /* [in] */ Int32 rowSpan,
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int32 columnSpan,
    /* [in] */ Boolean heading,
    /* [out] */ ICollectionItemInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return CCollectionItemInfo::Obtain(rowIndex, rowSpan, columnIndex, columnSpan, heading, info);
}

ECode CCollectionItemInfoHelper::Obtain(
    /* [in] */ Int32 rowIndex,
    /* [in] */ Int32 rowSpan,
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int32 columnSpan,
    /* [in] */ Boolean heading,
    /* [in] */ Boolean selected,
    /* [out] */ ICollectionItemInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return CCollectionItemInfo::Obtain(rowIndex, rowSpan, columnIndex, columnSpan, heading, selected, info);
}

} // Accessibility
} // View
} // Droid
} // Elastos
