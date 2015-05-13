#ifndef __CEXPANDABLELISTVIEWHELPER_H_
#define __CEXPANDABLELISTVIEWHELPER_H_

#include "_CExpandableListViewHelper.h"
#include "widget/ExpandableListView.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CExpandableListViewHelper)
{
public:
    CARAPI GetPackedPositionType(
        /* [in] */ Int64 packedPosition,
        /* [out] */ Int32* type);

    CARAPI GetPackedPositionGroup(
        /* [in] */ Int64 packedPosition,
        /* [out] */ Int32* position);

    CARAPI GetPackedPositionChild(
        /* [in] */ Int64 packedPosition,
        /* [out] */ Int32* position);

    CARAPI GetPackedPositionForChild(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Int32 childPosition,
        /* [out] */ Int64* position);

    CARAPI GetPackedPositionForGroup(
        /* [in] */ Int32 groupPosition,
        /* [out] */ Int64* position);

};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__CEXPANDABLELISTVIEWHELPER_H_