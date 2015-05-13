
#ifndef __CPORTERDUFFXFERMODE_H__
#define __CPORTERDUFFXFERMODE_H__

#include "_CPorterDuffXfermode.h"
#include "graphics/Xfermode.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CPorterDuffXfermode), public Xfermode
{
public:
    /**
     * Create an xfermode that uses the specified porter-duff mode.
     *
     * @param mode           The porter-duff mode that is applied
     */
    CARAPI constructor(
        /* [in] */ PorterDuffMode mode);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetMode(
        /* [out] */ PorterDuffMode* mode);

private:
    static CARAPI_(Int32) NativeCreateXfermode(
        /* [in] */ PorterDuffMode mode);

public:
    PorterDuffMode mMode;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif //__CPORTERDUFFXFERMODE_H__
