#ifndef __ELASTOS_DROID_TELECOM_AUDIOSTATE_H__
#define __ELASTOS_DROID_TELECOM_AUDIOSTATE_H__

#include "Elastos.Droid.Telecomm.h"

#include <elastos/core/Object.h>
#include <elastos/core/StringBuffer.h>

using Elastos::Core::Object;
using Elastos::Core::StringBuffer;

namespace Elastos {
namespace Droid {
namespace Telecomm {
namespace Telecom {

class AudioState
    : public Object
    , public IAudioState
    , public IParcelable
{
public:
    AudioState();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Boolean isMuted,
        /* [in] */ Int32 route,
        /* [in] */ Int32 supportedRouteMask);

    CARAPI constructor(
        /* [in] */ IAudioState* state);

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [in] */ Boolean* res);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    static CARAPI_(String) AudioRouteToString(
        /* [in] */ Int32 route);

private:
    static CARAPI_(void) ListAppend(
        /* [in] */ StringBuffer buffer,
        /* [in] */ String str);

public:
    /** True if the call is muted, false otherwise. */
    Boolean mIsMuted;

    /** The route to use for the audio stream. */
    Int32 mRoute;

    /** Bit vector of all routes supported by this call. */
    Int32 mSupportedRouteMask;
};

} // namespace Telecom
} // namespace Telecomm
} // namespace Droid
} // namespace Elastos
#endif