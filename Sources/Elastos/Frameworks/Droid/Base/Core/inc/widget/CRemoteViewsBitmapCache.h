#ifndef __CREMOTEVIEWSBITMAPCACHE_H_
#define __CREMOTEVIEWSBITMAPCACHE_H_

#include "_CRemoteViewsBitmapCache.h"
#include "ext/frameworkext.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Droid::Graphics::IBitmap;

namespace Elastos{
namespace Droid{
namespace Widget{

class MemoryUsageCounter;

CarClass(CRemoteViewsBitmapCache)
{
public:
    CARAPI constructor();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetBitmapId(
        /* [in] */ IBitmap* b,
        /* [out] */ Int32* id);

    CARAPI GetBitmapForId(
        /* [in] */ Int32 id,
        /* [out] */ IBitmap** bitmap);

    CARAPI Assimilate(
        /* [in] */ IRemoteViewsBitmapCache* bitmapCache);

    CARAPI_(void) AddBitmapMemory(
        /* [[in] */ MemoryUsageCounter* memoryCounter);

private:
    List<AutoPtr<IBitmap> > mBitmaps;
};


}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__CREMOTEVIEWSBITMAPCACHE_H_
