
#include "elastos/droid/view/CWindowIdFocusObserver.h"
#include "elastos/droid/view/WindowIdFocusObserver.h"
#include <elastos/core/AutoLock.h>

namespace Elastos {
namespace Droid {
namespace View {

CAR_OBJECT_IMPL(CWindowIdFocusObserver);
CAR_INTERFACE_IMPL(CWindowIdFocusObserver, Object, IIWindowFocusObserver);
ECode CWindowIdFocusObserver::constructor(
    /* [in] */ IWindowIdFocusObserver* observer)
{
    mObserver = observer;
    return NOERROR;
}

ECode CWindowIdFocusObserver::FocusGained(
    /* [in] */ IBinder* inputToken)
{
    WindowIdFocusObserver* observer = (WindowIdFocusObserver*)mObserver.Get();
    AutoPtr<IWindowId> token;
    AutoPtr<IHashMap> map = observer->mRegistrations;
    synchronized(map) {
        map->Get(inputToken, (IInterface**)&token);
    }
    if (observer->mHandler != NULL) {
        AutoPtr<IMessage> msg;
        Boolean reslult = FALSE;
        observer->mHandler->ObtainMessage(1, token, (IMessage**)&msg);
        observer->mHandler->SendMessage(msg, &reslult);
    } else {
        observer->OnFocusGained(token);
    }
    return NOERROR;
}

ECode CWindowIdFocusObserver::FocusLost(
    /* [in] */ IBinder* inputToken)
{
    WindowIdFocusObserver* observer = (WindowIdFocusObserver*)mObserver.Get();
    AutoPtr<IWindowId> token;
    AutoPtr<IHashMap> map = observer->mRegistrations;
    synchronized(map) {
        map->Get(inputToken, (IInterface**)&token);
    }
    if (observer->mHandler != NULL) {
        AutoPtr<IMessage> msg;
        Boolean reslult = FALSE;
        observer->mHandler->ObtainMessage(2, token, (IMessage**)&msg);
        observer->mHandler->SendMessage(msg, &reslult);
    } else {
        observer->OnFocusLost(token);
    }
    return NOERROR;
}

ECode CWindowIdFocusObserver::ToString(
    /* [out] */ String* str)
{
    return NOERROR;
}

} // namespace View
} // namepsace Droid
} // namespace Elastos
