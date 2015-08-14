#ifndef __ELASTOS_DROID_WIDGET_CEXPANDABLELISTPOSITION_H__
#define __ELASTOS_DROID_WIDGET_CEXPANDABLELISTPOSITION_H__

#include "_CExpandableListPosition.h"
#include "widget/ExpandableListPosition.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CExpandableListPosition), public ExpandableListPosition
{
public:

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetPackedPosition(
        /* [out] */ Int64* pos);

    CARAPI Recycle();
};


} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CEXPANDABLELISTPOSITION_H__
