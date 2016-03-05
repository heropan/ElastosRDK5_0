#include "elastos/droid/telecomm/telecom/AudioState.h"

namespace Elastos {
namespace Droid {
namespace Telecomm {
namespace Telecom {

AudioState::AudioState()
{}

ECode AudioState::constructor()
{
    return NOERROR;
}

ECode AudioState::constructor(
    /* [in] */ Boolean isMuted,
    /* [in] */ Int32 route,
    /* [in] */ Int32 supportedRouteMask)
{
    mIsMuted = isMuted;
    mRoute = route;
    mSupportedRouteMask = supportedRouteMask;

    return NOERROR;
}

ECode AudioState::constructor(
    /* [in] */ IAudioState* state)
{
    AudioState* impl = (AudioState*)state;
    mIsMuted = impl->mIsMuted;
    mRoute = impl->mRoute;
    mSupportedRouteMask = impl->mSupportedRouteMask;

    return NOERROR;
}

ECode AudioState::Equals(
    /* [in] */ IInterface* obj,
    /* [in] */ Boolean* res)
{
    if (obj == NULL) {
        *res = FALSE;
    }
    if (IAudioState::Probe(obj) == NULL) {
        *res = FALSE;
    }
    IAudioState* state = IAudioState::Probe(obj);
    AudioState* impl = (AudioState*)state;
    *res = (mIsMuted == impl->mIsMuted && mRoute == impl->mRoute &&
            mSupportedRouteMask == impl->mSupportedRouteMask);
    return NOERROR;
}

ECode AudioState::ToString(
    /* [out] */ String* str)
{
    return NOERROR;
}

ECode AudioState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return NOERROR;
}

ECode AudioState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return NOERROR;
}

String AudioState::AudioRouteToString(
    /* [in] */ Int32 route)
{
    String res;
    return res;
}

void AudioState::ListAppend(
    /* [in] */ StringBuffer buffer,
    /* [in] */ const String& str)
{
}

} // namespace Telecom
} // namespace Telecomm
} // namespace Droid
} // namespace Elastos