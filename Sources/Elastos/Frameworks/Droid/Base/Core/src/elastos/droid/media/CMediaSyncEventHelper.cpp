#include "elastos/droid/media/CMediaSyncEventHelper.h"
#include "elastos/droid/media/CMediaSyncEvent.h"

namespace Elastos {
namespace Droid {
namespace Media {

ECode CMediaSyncEventHelper::CreateEvent(
    /* [in] */ Int32 eventType,
    /* [out] */ IMediaSyncEvent** result)
{
    return CMediaSyncEvent::CreateEvent(eventType, result);
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
