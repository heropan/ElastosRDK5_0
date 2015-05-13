#include "ext/frameworkext.h"
#include "widget/CExpandableListPositionHelper.h"

using Elastos::Droid::Widget::IExpandableListPosition;

namespace Elastos{
namespace Droid{
namespace Widget{

ECode CExpandableListPositionHelper::ObtainGroupPosition(
    /* [in] */ Int32 groupPosition,
    /* [out] */ IExpandableListPosition** position)
{
    VALIDATE_NOT_NULL(position);
    *position = ExpandableListPosition::ObtainGroupPosition(groupPosition);
    return NOERROR;
}


ECode CExpandableListPositionHelper::ObtainChildPosition(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition,
    /* [out] */ IExpandableListPosition** position)
{
    VALIDATE_NOT_NULL(position);
    *position = ExpandableListPosition::ObtainChildPosition(groupPosition, childPosition);
    return NOERROR;
}

ECode CExpandableListPositionHelper::ObtainPosition(
    /* [in] */ Int64 packedPosition,
    /* [out] */ IExpandableListPosition** position)
{
    VALIDATE_NOT_NULL(position);
    *position = ExpandableListPosition::ObtainPosition(packedPosition);
    return NOERROR;
}

ECode CExpandableListPositionHelper::Obtain(
    /* [in] */ Int32 type,
    /* [in] */ Int32 groupPos,
    /* [in] */ Int32 childPos,
    /* [in] */ Int32 flatListPos,
    /* [out] */ IExpandableListPosition** position)
{
    VALIDATE_NOT_NULL(position);
    *position = ExpandableListPosition::Obtain(type, groupPos, childPos, flatListPos);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos