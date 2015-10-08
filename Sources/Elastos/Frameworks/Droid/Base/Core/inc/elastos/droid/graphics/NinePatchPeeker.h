
#ifndef __ELASTOS_DROID_GRAPHICS_NINEPATCHPEEKER_H__
#define __ELASTOS_DROID_GRAPHICS_NINEPATCHPEEKER_H__

#include <skia/core/SkImageDecoder.h>
#include <androidfw/ResourceTypes.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

////////////////////////////////////////////////////////////////////////////////
// NinePatchPeeker

class NinePatchPeeker : public SkImageDecoder::Peeker
{
public:
    NinePatchPeeker(SkImageDecoder* host)
    {
        // the host lives longer than we do, so a raw ptr is safe
        mHost = host;
        mPatch = NULL;
        mLayoutBounds = NULL;
    }

    ~NinePatchPeeker()
    {
        free(mPatch);
        if (mLayoutBounds != NULL) {
            delete mLayoutBounds;
            mLayoutBounds = NULL;
        }
    }

    virtual bool peek(const char tag[], const void* data, size_t length);

public:
    SkImageDecoder* mHost;
    android::Res_png_9patch*  mPatch;
    int    *mLayoutBounds;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elast

#endif //__ELASTOS_DROID_GRAPHICS_NINEPATCHPEEKER_H__
