
#ifndef __THUMBNAILHOLDER_H__
#define __THUMBNAILHOLDER_H__

#include <Elastos.Droid.Core.h>

using Elastos::Core::ICharSequence;
using Elastos::Droid::Graphics::IBitmap;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class ThumbnailHolder : public ElRefBase
{
public:
    AutoPtr<IBitmap> mLastThumbnail;         // Last thumbnail captured for this item.
    AutoPtr<ICharSequence> mLastDescription; // Last description captured for this item.
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__THUMBNAILHOLDER_H__
