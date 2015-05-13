
#ifndef __PENDINGTHUMBNAILSRECORD_H__
#define __PENDINGTHUMBNAILSRECORD_H__

#include "ext/frameworkext.h"
#include <elastos/HashSet.h>
#include "am/ActivityRecord.h"

using Elastos::Utility::HashSet;
using Elastos::Droid::App::IThumbnailReceiver;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

/**
 * This class keeps track of calls to getTasks() that are still
 * waiting for thumbnail images.
 */
class PendingThumbnailsRecord : public ElRefBase
{
public:

    PendingThumbnailsRecord(
        /* [in] */ IThumbnailReceiver* receiver);

public:
    AutoPtr<IThumbnailReceiver> mReceiver;   // who is waiting.
    HashSet<AutoPtr<ActivityRecord> > mPendingRecords; // HistoryRecord objects we still wait for.
    Boolean mFinished;       // Is pendingRecords empty?
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__PENDINGTHUMBNAILSRECORD_H__
