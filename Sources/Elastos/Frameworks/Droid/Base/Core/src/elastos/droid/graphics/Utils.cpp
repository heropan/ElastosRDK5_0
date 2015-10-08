
#include "graphics/Utils.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Graphics {

static const char* TAG = "AssetStreamAdaptor";

bool AssetStreamAdaptor::rewind()
{
    off64_t pos = mAsset->seek(0, SEEK_SET);
    if (pos == (off64_t)-1) {
        Slogger::E(TAG, "----- mAsset->seek(rewind) failed");
        return false;
    }
    return true;
}

size_t AssetStreamAdaptor::read(void* buffer, size_t size)
{
    ssize_t amount;

    if (NULL == buffer) {
        if (0 == size) {  // caller is asking us for our total length
            return mAsset->getLength();
        }
        // asset->seek returns new total offset
        // we want to return amount that was skipped

        off64_t oldOffset = mAsset->seek(0, SEEK_CUR);
        if (-1 == oldOffset) {
            Slogger::E(TAG, "---- mAsset->seek(oldOffset) failed");
            return 0;
        }
        off64_t newOffset = mAsset->seek(size, SEEK_CUR);
        if (-1 == newOffset) {
            Slogger::E(TAG, "---- mAsset->seek(%d) failed\n", size);
            return 0;
        }
        amount = newOffset - oldOffset;
    }
    else {
        amount = mAsset->read(buffer, size);
        if (amount <= 0) {
            Slogger::E(TAG, "---- mAsset->read(%d) returned %d\n", size, amount);
        }
    }

    if (amount < 0) {
        amount = 0;
    }
    return amount;
}

SkMemoryStream* CopyAssetToStream(android::Asset* asset) {
    if (NULL == asset) {
        return NULL;
    }

    off64_t size = asset->seek(0, SEEK_SET);
    if ((off64_t)-1 == size) {
        SkDebugf("---- copyAsset: asset rewind failed\n");
        return NULL;
    }

    size = asset->getLength();
    if (size <= 0) {
        SkDebugf("---- copyAsset: asset->getLength() returned %d\n", size);
        return NULL;
    }

    SkMemoryStream* stream = new SkMemoryStream(size);
    void* data = const_cast<void*>(stream->getMemoryBase());
    off64_t len = asset->read(data, size);
    if (len != size) {
        SkDebugf("---- copyAsset: asset->read(%d) returned %d\n", size, len);
        delete stream;
        stream = NULL;
    }
    return stream;
}

}; // namespace Graphics
}; // namespace Droid
}; // namespace Elastos
