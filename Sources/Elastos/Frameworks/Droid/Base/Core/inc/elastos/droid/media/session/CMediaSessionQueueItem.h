#ifndef __ELASTOS_DROID_MEDIA_SESSION_CMEDIASESSIONQUEUEITEM_H__
#define __ELASTOS_DROID_MEDIA_SESSION_CMEDIASESSIONQUEUEITEM_H__

#include "_Elastos_Droid_Media_Session_CMediaSessionQueueItem.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

namespace Elastos {
namespace Droid {
namespace Media {
namespace Session {

/**
 * A single item that is part of the play queue. It contains a description
 * of the item and its id in the queue.
 */
CarClass(CMediaSessionQueueItem)
    , public Object
    , public IMediaSessionQueueItem
    , public IParcelable
{
public:
    CMediaSessionQueueItem();

    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    virtual ~CMediaSessionQueueItem();

    /**
     * Create a new {@link MediaSession.QueueItem}.
     *
     * @param description The {@link MediaDescription} for this item.
     * @param id An identifier for this item. It must be unique within the
     *            play queue and cannot be {@link #UNKNOWN_ID}.
     */
    CARAPI constructor(
        /* [in] */ IMediaDescription * description,
        /* [in] */ Int64 id);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    /**
     * Get the description for this item.
     */
    CARAPI GetDescription(
        /* [out] */ IMediaDescription ** result);

    /**
     * Get the queue id for this item.
     */
    CARAPI GetQueueId(
        /* [out] */ Int64 * result);

    CARAPI ToString(
        /* [out] */ String * result);

private:
    AutoPtr<IMediaDescription> mDescription;
    Int64 mId;

};

} // namespace Session
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_MEDIA_SESSION_CMEDIASESSIONQUEUEITEM_H__
