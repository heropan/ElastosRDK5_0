#ifndef __CEXPANDABLELISTPOSITION_H_
#define __CEXPANDABLELISTPOSITION_H_

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

#endif //__CEXPANDABLELISTPOSITION_H_