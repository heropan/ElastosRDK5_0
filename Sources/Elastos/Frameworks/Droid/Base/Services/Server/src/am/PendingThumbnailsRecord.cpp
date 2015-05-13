
#include "am/PendingThumbnailsRecord.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {


PendingThumbnailsRecord::PendingThumbnailsRecord(
    /* [in] */ IThumbnailReceiver* receiver)
    :  mFinished(FALSE)
{
    mReceiver = receiver;
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
