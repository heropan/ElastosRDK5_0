#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/CExpandableListPosition.h"

namespace Elastos{
namespace Droid{
namespace Widget{

PInterface CExpandableListPosition::Probe(
    /* [in] */ REIID riid)
{
    return _CExpandableListPosition::Probe(riid);
}

ECode CExpandableListPosition::GetPackedPosition(
    /* [out] */ Int64* pos)
{
    VALIDATE_NOT_NULL(pos);
    *pos = ExpandableListPosition::GetPackedPosition();
    return NOERROR;
}

ECode CExpandableListPosition::Recycle()
{
    return ExpandableListPosition::Recycle();
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
