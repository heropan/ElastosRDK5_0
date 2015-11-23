#ifndef __ELASTOS_DROID_WIDGET_CEXPANDABLELISTPOSITIONHELPER_H__
#define __ELASTOS_DROID_WIDGET_CEXPANDABLELISTPOSITIONHELPER_H__

#include "_Elastos_Droid_Widget_CExpandableListPositionHelper.h"
#include "elastos/droid/widget/ExpandableListPosition.h"

namespace Elastos {
namespace Droid {
namespace Widget {

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

#endif //__ELASTOS_DROID_WIDGET_CEXPANDABLELISTPOSITIONHELPER_H__
