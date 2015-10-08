#ifndef __ELASTOS_DROID_GRAPHICS_UTILS_H__
#define __ELASTOS_DROID_GRAPHICS_UTILS_H__

#include "Elastos.Droid.Core_server.h"
#include <skia/core/SkStream.h>
#include <androidfw/Asset.h>
#include <unistd.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

class AssetStreamAdaptor : public SkStream
{
public:
    AssetStreamAdaptor(android::Asset* a) : mAsset(a) {}
    virtual bool rewind();
    virtual size_t read(void* buffer, size_t size);

private:
    android::Asset*  mAsset;
};

/**
 *  Make a deep copy of the asset, and return it as a stream, or NULL if there
 *  was an error.
 *  FIXME: If we could "ref/reopen" the asset, we may not need to copy it here.
 */

SkMemoryStream* CopyAssetToStream(android::Asset*);

/** Restore the file descriptor's offset in our destructor
 */
class AutoFDSeek {
public:
    AutoFDSeek(int fd) : mFD(fd)
    {
        mCurr = ::lseek(fd, 0, SEEK_CUR);
    }

    ~AutoFDSeek()
    {
        if (mCurr >= 0) {
            ::lseek(mFD, mCurr, SEEK_SET);
        }
    }
private:
    int     mFD;
    off64_t   mCurr;
};

}; // namespace Graphics
}; // namespace Droid
}; // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_UTILS_H__
