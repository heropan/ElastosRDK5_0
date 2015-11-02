
#include "elastos/droid/view/accessibility/CCollectionItemInfo.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

const Int32 CCollectionItemInfo::MAX_POOL_SIZE = 20;
AutoPtr<Pools::SynchronizedPool<ICollectionItemInfo> > CCollectionItemInfo::sPool =
        new Pools::SynchronizedPool<ICollectionItemInfo>(MAX_POOL_SIZE);

CAR_INTERFACE_IMPL(CCollectionItemInfo, Object, ICollectionItemInfo)

CAR_OBJECT_IMPL(CCollectionItemInfo)

CCollectionItemInfo::CCollectionItemInfo()
    : mHeading(FALSE)
    , mColumnIndex(0)
    , mRowIndex(0)
    , mColumnSpan(0)
    , mRowSpan(0)
    , mSelected(FALSE)
{
}

CCollectionItemInfo::~CCollectionItemInfo()
{
}

ECode CCollectionItemInfo::constructor()
{
    return NOERROR;
}

ECode CCollectionItemInfo::constructor(
    /* [in] */ Int32 rowIndex,
    /* [in] */ Int32 rowSpan,
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int32 columnSpan,
    /* [in] */ Boolean heading,
    /* [in] */ Boolean selected)
{
    mRowIndex = rowIndex;
    mRowSpan = rowSpan;
    mColumnIndex = columnIndex;
    mColumnSpan = columnSpan;
    mHeading = heading;
    mSelected = selected;

    return NOERROR;
}

ECode CCollectionItemInfo::Obtain(
    /* [in] */ ICollectionItemInfo* other,
    /* [out] */ ICollectionItemInfo** info)
{
    VALIDATE_NOT_NULL(info);
    Int32 rowIndex, rowSpan, columnIndex, columnSpan;
    Boolean heading, selected;
    other->GetRowIndex(&rowIndex);
    other->GetRowSpan(&rowSpan);
    other->GetColumnIndex(&columnIndex);
    other->GetColumnSpan(&columnSpan);
    other->IsHeading(&heading);
    other->IsSelected(&selected);

    return Obtain(rowIndex, rowSpan, columnIndex, columnSpan, heading, selected, info);
}

ECode CCollectionItemInfo::Obtain(
    /* [in] */ Int32 rowIndex,
    /* [in] */ Int32 rowSpan,
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int32 columnSpan,
    /* [in] */ Boolean heading,
    /* [out] */ ICollectionItemInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return Obtain(rowIndex, rowSpan, columnIndex, columnSpan, heading, FALSE, info);
}

ECode CCollectionItemInfo::Obtain(
    /* [in] */ Int32 rowIndex,
    /* [in] */ Int32 rowSpan,
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int32 columnSpan,
    /* [in] */ Boolean heading,
    /* [in] */ Boolean selected,
    /* [out] */ ICollectionItemInfo** info)
{
    VALIDATE_NOT_NULL(info);
    AutoPtr<ICollectionItemInfo> _info = sPool->AcquireItem();

    if (_info == NULL) {
        AutoPtr<CCollectionItemInfo> collectionItemInfo;
        CCollectionItemInfo::NewByFriend(rowIndex, rowSpan, columnIndex, columnSpan, heading, selected,
                (CCollectionItemInfo**)&collectionItemInfo);
        *info = (ICollectionItemInfo*)collectionItemInfo.Get();
        REFCOUNT_ADD(*info);
        return NOERROR;
    }


    AutoPtr<CCollectionItemInfo> cinfo = (CCollectionItemInfo*)_info.Get();
    cinfo->mRowIndex = rowIndex;
    cinfo->mRowSpan = rowSpan;
    cinfo->mColumnIndex = columnIndex;
    cinfo->mColumnSpan = columnSpan;
    cinfo->mHeading = heading;
    cinfo->mSelected = selected;

    AutoPtr<ICollectionItemInfo> info2 = (ICollectionItemInfo*)cinfo.Get();

    *info = info2;
    REFCOUNT_ADD(*info);

    return NOERROR;
}

ECode CCollectionItemInfo::GetColumnIndex(
    /* [out] */ Int32* columnIndex)
{
    VALIDATE_NOT_NULL(columnIndex)
    *columnIndex = mColumnIndex;
    return NOERROR;
}

ECode CCollectionItemInfo::GetRowIndex(
    /* [out] */ Int32* rowIndex)
{
    VALIDATE_NOT_NULL(rowIndex)
    *rowIndex = mRowIndex;
    return NOERROR;
}

ECode CCollectionItemInfo::GetColumnSpan(
    /* [out] */ Int32* columnSpan)
{
    VALIDATE_NOT_NULL(columnSpan)
    *columnSpan = mColumnSpan;
    return NOERROR;
}

ECode CCollectionItemInfo::GetRowSpan(
    /* [out] */ Int32* rowSpan)
{
    VALIDATE_NOT_NULL(rowSpan)
    *rowSpan = mRowSpan;
    return NOERROR;
}

ECode CCollectionItemInfo::IsHeading(
    /* [out] */ Boolean* heading)
{
    VALIDATE_NOT_NULL(heading)
    *heading = mHeading;
    return NOERROR;
}

ECode CCollectionItemInfo::IsSelected(
    /* [out] */ Boolean* selected)
{
    VALIDATE_NOT_NULL(selected)
    *selected = mSelected;
    return NOERROR;
}

ECode CCollectionItemInfo::Recycle()
{
    Clear();
    sPool->ReleaseItem(THIS_PROBE(ICollectionItemInfo));
    return NOERROR;
}

void CCollectionItemInfo::Clear()
{
    mColumnIndex = 0;
    mColumnSpan = 0;
    mRowIndex = 0;
    mRowSpan = 0;
    mHeading = FALSE;
    mSelected = FALSE;
}

} // Accessibility
} // View
} // Droid
} // Elastos
