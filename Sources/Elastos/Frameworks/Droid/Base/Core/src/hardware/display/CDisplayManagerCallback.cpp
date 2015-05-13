
#include "hardware/display/CDisplayManagerCallback.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Display {

ECode CDisplayManagerCallback::constructor(
    /* [in] */ Handle32 owner)
{
    mOwner = (DisplayManagerGlobal*)owner;
    return NOERROR;
}

//@Override
ECode CDisplayManagerCallback::OnDisplayEvent(
    /* [in] */ Int32 displayId,
    /* [in] */ Int32 event)
{
    if (mOwner->DEBUG) {
        Logger::D(mOwner->TAG, "onDisplayEvent: displayId=%d, event=%d",
            displayId, event);
    }
    mOwner->HandleDisplayEvent(displayId, event);

    return NOERROR;
}

ECode CDisplayManagerCallback::ToString(
    /* [out] */ String* str)
{
    return E_NOT_IMPLEMENTED;
}

} // namespace Display
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos