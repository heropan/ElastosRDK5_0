#ifndef __ELASTOS_DROID_MEDIA_MediaTimeProvider_H__
#define __ELASTOS_DROID_MEDIA_MediaTimeProvider_H__

#include "Elastos.Droid.Media.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

namespace Elastos {
namespace Droid {
namespace Media {

class MediaTimeProvider
    : public Object
    , public IMediaTimeProvider
{
public:
    CAR_INTERFACE_DECL()

    virtual CARAPI NotifyAt(
        /* [in] */ Int64 timeUs,
        /* [in] */ IMediaTimeProviderOnMediaTimeListener* listener) = 0;

    virtual CARAPI ScheduleUpdate(
        /* [in] */ IMediaTimeProviderOnMediaTimeListener* listener) = 0;

    virtual CARAPI CancelNotifications(
        /* [in] */ IMediaTimeProviderOnMediaTimeListener* listener) = 0;

    virtual CARAPI GetCurrentTimeUs(
        /* [in] */ Boolean precise,
        /* [in] */ Boolean monotonic) = 0;

    class OnMediaTimeListener
        : public Object
        , public IMediaTimeProviderOnMediaTimeListener
    {
    public:
        CAR_INTERFACE_DECL()

        virtual CARAPI OnTimedEvent(
            /* [in] */ Int64 timeUs) = 0;

        virtual CARAPI OnSeek(
            /* [in] */ Int64 timeUs) = 0;

        virtual CARAPI OnStop() = 0;
    };
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif// __ELASTOS_DROID_MEDIA_MediaTimeProvider_H__