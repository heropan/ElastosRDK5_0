#ifndef __CPOSITIONMETADATAHELPER_H_
#define __CPOSITIONMETADATAHELPER_H_

#include "_CPositionMetadataHelper.h"
#include "widget/PositionMetadata.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CPositionMetadataHelper)
{
public:

    CARAPI Obtain(
        /* [in] */ Int32 flatListPos,
        /* [in] */ Int32 type,
        /* [in] */ Int32 groupPos,
        /* [in] */ Int32 childPos,
        /* [in] */ IGroupMetadata* groupMetadata,
        /* [in] */ Int32 groupInsertIndex,
        /* [out] */ IPositionMetadata** posMetadata);

};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif
