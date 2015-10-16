
#include "elastos/droid/graphics/NinePatchPeeker.h"
#include <skia/core/SkBitmap.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

bool NinePatchPeeker::peek(const char tag[], const void* data, size_t length)
{
    if (strcmp("npTc", tag) == 0 && length >= sizeof(android::Res_png_9patch)) {
        android::Res_png_9patch* patch = (android::Res_png_9patch*) data;
        size_t patchSize = patch->serializedSize();
        assert(length == patchSize);
        // You have to copy the data because it is owned by the png reader
        android::Res_png_9patch* patchNew = (android::Res_png_9patch*) malloc(patchSize);
        memcpy(patchNew, patch, patchSize);
        // this relies on deserialization being done in place
        android::Res_png_9patch::deserialize(patchNew);
        patchNew->fileToDevice();
        free(mPatch);
        mPatch = patchNew;
        //printf("9patch: (%d,%d)-(%d,%d)\n",
        //       fPatch.sizeLeft, fPatch.sizeTop,
        //       fPatch.sizeRight, fPatch.sizeBottom);

        // now update our host to force index or 32bit config
        // 'cause we don't want 565 predithered, since as a 9patch, we know
        // we will be stretched, and therefore we want to dither afterwards.
        static const SkBitmap::Config gNo565Pref[] = {
            SkBitmap::kIndex8_Config,
            SkBitmap::kIndex8_Config,
            SkBitmap::kARGB_8888_Config,
            SkBitmap::kARGB_8888_Config,
            SkBitmap::kARGB_8888_Config,
            SkBitmap::kARGB_8888_Config,
        };
        mHost->setPrefConfigTable(gNo565Pref);
    }
    else if (strcmp("npLb", tag) == 0 && length == sizeof(int) * 4) {
        mLayoutBounds = new int[4];
        memcpy(mLayoutBounds, data, sizeof(int) * 4);
    }
    return true;    // keep on decoding
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elast

