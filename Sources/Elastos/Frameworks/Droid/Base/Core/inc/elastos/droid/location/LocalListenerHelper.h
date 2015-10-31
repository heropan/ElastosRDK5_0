
#ifndef __ELASTOS_DROID_LOCATION_LOCALLISTENERHELPER_H__
#define __ELASTOS_DROID_LOCATION_LOCALLISTENERHELPER_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/HashSet.h>
#include <elastos/core/AutoLock.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Etl::HashSet;
using Elastos::Core::AutoLock;
using Elastos::Utility::Logging::Logger;
using Elastos::Utility::ICollection;
using Elastos::Utility::IArrayList;
using Elastos::Utility::IHashSet;

namespace Elastos {
namespace Droid {
namespace Location {

class LocalListenerHelper
    : public Object
    , public ILocalListenerHelper
{
protected:
    LocalListenerHelper(
        /* [in] */ const String& name);

public:
    CAR_INTERFACE_DECL()

    CARAPI Add(
        /* [in] */ IInterface* listener,
        /* [out] */ Boolean* result);

    CARAPI Remove(
        /* [in] */ IInterface* listener);

    virtual CARAPI RegisterWithServer(
        /* [out] */ Boolean* result) = 0;

    virtual CARAPI UnregisterFromServer() = 0;

    CARAPI Foreach(
        /* [in] */ ILocalListenerHelperListenerOperation* operation);

private:
    HashSet<IInterface* > mListeners;
    // AutoPtr<IHashSet> mListeners2;
    const String mTag;

};

}//namespace Location
}//namespace Droid
}//namespace Elastos

#endif // __ELASTOS_DROID_LOCATION_LOCALLISTENERHELPER_H__
