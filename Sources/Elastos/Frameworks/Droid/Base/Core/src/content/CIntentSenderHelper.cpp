
#include "content/CIntentSenderHelper.h"

namespace Elastos {
namespace Droid {
namespace Content {

ECode CIntentSenderHelper::WriteIntentSenderOrNullToParcel(
    /* [in] */ IIntentSender* sender,
    /* [in] */ IParcel* parcel)
{
//    out.writeStrongBinder(sender != null ? sender.mTarget.asBinder() : null);
    return E_NOT_IMPLEMENTED;
}

ECode CIntentSenderHelper::ReadIntentSenderOrNullFromParcel(
    /* [in] */ IParcel* parcel,
    /* [out] */ IIntentSender** intentSender)
{
//    IBinder b = in.readStrongBinder();
//    return b != null ? new IntentSender(b) : null;
    return E_NOT_IMPLEMENTED;
}

}
}
}

