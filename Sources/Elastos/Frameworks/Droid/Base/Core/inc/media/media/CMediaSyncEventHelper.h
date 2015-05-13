
#ifndef __CMEDIASYNCEVENTHELPER_H__
#define __CMEDIASYNCEVENTHELPER_H__

#include "_CMediaSyncEventHelper.h"

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CMediaSyncEventHelper)
{
public:
    /**
     * Creates a synchronization event of the sepcified type.
     *
     * <p>The type specifies which kind of event is monitored.
     * For instance, event {@link #SYNC_EVENT_PRESENTATION_COMPLETE} corresponds to the audio being
     * presented to the user on a particular audio session.
     * @param eventType the synchronization event type.
     * @return the MediaSyncEvent created.
     * @throws java.lang.IllegalArgumentException
     */
    CARAPI CreateEvent(
        /* [in] */ Int32 eventType,
        /* [out] */ IMediaSyncEvent** result);
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __CMEDIASYNCEVENTHELPER_H__
