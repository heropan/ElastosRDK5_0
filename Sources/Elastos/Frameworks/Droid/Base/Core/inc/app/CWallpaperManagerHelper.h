#ifndef __CWALLPAPERMANAGERHELPER_H__
#define __CWALLPAPERMANAGERHELPER_H__

#include "_CWallpaperManagerHelper.h"

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CWallpaperManagerHelper)
{
public:
    /**
     * Retrieve a WallpaperManager associated with the given Context.
     */
    CARAPI GetInstance(
        /* [in] */ IContext* context,
        /* [out] */ IWallpaperManager** manager);
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __CWALLPAPERMANAGERHELPER_H__
