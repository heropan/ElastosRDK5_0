#ifndef __CEXPANDABLELISTPOSITIONHELPER_H_
#define __CEXPANDABLELISTPOSITIONHELPER_H_

#include "_CExpandableListPositionHelper.h"
#include "widget/ExpandableListPosition.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CExpandableListPositionHelper)
{
public:

    CARAPI ObtainGroupPosition(
        /* [in] */ Int32 groupPosition,
        /* [out] */ IExpandableListPosition** position);

    CARAPI ObtainChildPosition(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Int32 childPosition,
        /* [out] */ IExpandableListPosition** position);

    CARAPI ObtainPosition(
        /* [in] */ Int64 packedPosition,
        /* [out] */ IExpandableListPosition** position);

    CARAPI Obtain(
        /* [in] */ Int32 type,
        /* [in] */ Int32 groupPos,
        /* [in] */ Int32 childPos,
        /* [in] */ Int32 flatListPos,
        /* [out] */ IExpandableListPosition** position);
};


} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__CEXPANDABLELISTPOSITIONHELPER_H_