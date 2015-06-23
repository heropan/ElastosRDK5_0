
#ifndef __COBBACTIONLISTENER_H__
#define __COBBACTIONLISTENER_H__

#include "_CObbActionListener.h"
#include "os/storage/CStorageManager.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;

namespace Elastos {
namespace Droid {
namespace Os {
namespace Storage {

CarClass(CObbActionListener)
{
public:
    CARAPI constructor(
        /* [in] */ IStorageManager* host);

    CARAPI OnObbResult(
        /* [in] */ const String& filename,
        /* [in] */ Int32 nonce,
        /* [in] */ Int32 status);

    Int32 AddListener(
        /* [in] */ IOnObbStateChangeListener* listener);

private:
    CStorageManager* mHost;

    HashMap<Int32, AutoPtr<CStorageManager::ObbListenerDelegate> > mListeners;

    Mutex mListenersLock;
};

} // namespace Storage
} // namespace Os
} // namepsace Droid
} // namespace Elastos

#endif // __COBBACTIONLISTENER_H__
