
#include "elastos/droid/view/accessibility/CCollectionInfo.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

const Int32 CCollectionInfo::MAX_POOL_SIZE = 20;
AutoPtr<Pools::SynchronizedPool<ICollectionInfo> > CCollectionInfo::sPool =
        new Pools::SynchronizedPool<ICollectionInfo>(MAX_POOL_SIZE);

CAR_INTERFACE_IMPL(CCollectionInfo, Object, ICollectionInfo)

CAR_OBJECT_IMPL(CCollectionInfo)

CCollectionInfo::CCollectionInfo()
    : mRowCount(0)
    , mColumnCount(0)
    , mHierarchical(FALSE)
    , mSelectionMode(0)
{
}

CCollectionInfo::~CCollectionInfo()
{
}

ECode CCollectionInfo::constructor()
{
    return NOERROR;
}

ECode CCollectionInfo::constructor(
    /* [in] */ Int32 rowCount,
    /* [in] */ Int32 columnCount,
    /* [in] */ Boolean hierarchical,
    /* [in] */ Int32 selectionMode)
{
    mRowCount = rowCount;
    mColumnCount = columnCount;
    mHierarchical = hierarchical;
    mSelectionMode = selectionMode;
    return NOERROR;
}

ECode CCollectionInfo::Obtain(
    /* [in] */ ICollectionInfo* other,
    /* [out] */ ICollectionInfo** info)
{
    VALIDATE_NOT_NULL(info);
    Int32 rowCount, columnCount, selectionMode;
    Boolean hierarchical;
    other->GetRowCount(&rowCount);
    other->GetColumnCount(&columnCount);
    other->IsHierarchical(&hierarchical);
    other->GetSelectionMode(&selectionMode);

    return Obtain(rowCount, columnCount, hierarchical, selectionMode, info);
}

ECode CCollectionInfo::Obtain(
    /* [in] */ Int32 rowCount,
    /* [in] */ Int32 columnCount,
    /* [in] */ Boolean hierarchical,
    /* [out] */ ICollectionInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return Obtain(rowCount, columnCount, hierarchical, SELECTION_MODE_NONE, info);
}

ECode CCollectionInfo::Obtain(
    /* [in] */ Int32 rowCount,
    /* [in] */ Int32 columnCount,
    /* [in] */ Boolean hierarchical,
    /* [in] */ Int32 selectionMode,
    /* [out] */ ICollectionInfo** info)
{
    VALIDATE_NOT_NULL(info);
    AutoPtr<ICollectionInfo> _info = sPool->AcquireItem();

    if (_info == NULL) {
        AutoPtr<CCollectionInfo> collectionInfo;
        CCollectionInfo::NewByFriend(rowCount, columnCount, hierarchical, selectionMode, (CCollectionInfo**)&collectionInfo);
        *info = (ICollectionInfo*)collectionInfo.Get();
        REFCOUNT_ADD(*info);
        return NOERROR;
    }

    AutoPtr<CCollectionInfo> cinfo = (CCollectionInfo*)_info.Get();
    cinfo->mRowCount = rowCount;
    cinfo->mColumnCount = columnCount;
    cinfo->mHierarchical = hierarchical;
    cinfo->mSelectionMode = selectionMode;

    AutoPtr<ICollectionInfo> info2 = (ICollectionInfo*)cinfo.Get();

    *info = info2;
    REFCOUNT_ADD(*info);

    return NOERROR;
}

ECode CCollectionInfo::GetRowCount(
    /* [out] */ Int32* rowCount)
{
    VALIDATE_NOT_NULL(rowCount)
    *rowCount = mRowCount;
    return NOERROR;
}

ECode CCollectionInfo::GetColumnCount(
    /* [out] */ Int32* columnCount)
{
    VALIDATE_NOT_NULL(columnCount)
    *columnCount = mColumnCount;
    return NOERROR;
}

ECode CCollectionInfo::IsHierarchical(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mHierarchical;
    return NOERROR;
}

ECode CCollectionInfo::GetSelectionMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode)
    *mode = mSelectionMode;
    return NOERROR;
}

ECode CCollectionInfo::Recycle()
{
    Clear();
    sPool->ReleaseItem(THIS_PROBE(ICollectionInfo));
    return NOERROR;
}

void CCollectionInfo::Clear()
{
    mRowCount = 0;
    mColumnCount = 0;
    mHierarchical = FALSE;
    mSelectionMode = SELECTION_MODE_NONE;
}

} // Accessibility
} // View
} // Droid
} // Elastos
