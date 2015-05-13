#ifndef __CGROUPMETADATAHELPER_H__
#define __CGROUPMETADATAHELPER_H__

#include "_CGroupMetadataHelper.h"
#include "widget/GroupMetadata.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CGroupMetadataHelper)
{

    CARAPI Obtain(
        /* [in] */ Int32 flPos,
        /* [in] */ Int32 lastChildFlPos,
        /* [in] */ Int32 gPos,
        /* [in] */ Int64 gId,
        /* [out] */ IGroupMetadata** groupMetadata);

};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__CGROUPMETADATAHELPER_H__