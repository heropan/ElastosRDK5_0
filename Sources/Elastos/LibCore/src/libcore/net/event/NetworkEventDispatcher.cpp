
#include "NetworkEventDispatcher.h"

namespace Libcore {
namespace Net {
namespace Event {

CAR_INTERFACE_IMPL(NetworkEventDispatcher, Object, INetworkEventDispatcher);

AutoPtr<INetworkEventDispatcher> NetworkEventDispatcher::sInstance = new NetworkEventDispatcher();

AutoPtr<INetworkEventDispatcher> NetworkEventDispatcher::GetInstance()
{
    return sInstance;
}

NetworkEventDispatcher::NetworkEventDispatcher()
{
}

ECode NetworkEventDispatcher::AddListener(
    /* [in] */ INetworkEventListener* toAdd)
{
    //TODO
    assert(0);
    // if (toAdd == null) {
    //   throw new NullPointerException("toAdd == null");
    // }
    // listeners.add(toAdd);
    return NOERROR;
}

ECode NetworkEventDispatcher::RemoveListener(
    /* [in] */ INetworkEventListener* toRemove)
{
    //TODO
    assert(0);
    // for (NetworkEventListener listener : listeners) {
    //   if (listener == toRemove) {
    //     listeners.remove(listener);
    //     return;
    //   }
    // }
    return NOERROR;
}

ECode NetworkEventDispatcher::OnNetworkConfigurationChanged()
{
    //TODO
    assert(0);
    // for (NetworkEventListener listener : listeners) {
    //   try {
    //     listener.onNetworkConfigurationChanged();
    //   } catch (RuntimeException e) {
    //     System.logI("Exception thrown during network event propagation", e);
    //   }
    // }
    return NOERROR;
}

} // namespace Event
} // namespace Net
} // namespace Libcore
