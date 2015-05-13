
#include "graphics/Utils.h"
#include <elastos/Slogger.h>

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

}; // namespace Graphics
}; // namespace Droid
}; // namespace Elastos
