
#ifndef __ELASTOS_DROID_CONTENT_CSYNCSTORAGEENGINEHELPER_H__
#define __ELASTOS_DROID_CONTENT_CSYNCSTORAGEENGINEHELPER_H__

#include "_CSyncStorageEngineHelper.h"

using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CSyncStorageEngineHelper)
{
public:
    // TODO: i18n -- grab these out of resources.
    /** String names for the sync event types. */
    CARAPI GetEVENTS(
        /* [out, callee] */ ArrayOf<String>** events);

    CARAPI GetSYNC_CONNECTION_SETTING_CHANGED_INTENT(
        /* [out] */ IIntent** intent);

    // TODO: i18n -- grab these out of resources.
    /** String names for the sync source types. */
    CARAPI GetSOURCES(
        /* [out, callee] */ ArrayOf<String>** sources);

    CARAPI NewTestInstance(
        /* [in] */ IContext* context,
        /* [out] */ ISyncStorageEngine** newEngine);

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI GetSingleton(
        /* [out] */ ISyncStorageEngine** syncStoregeEngine);

    CARAPI Equals(
        /* [in] */ IBundle* bundleOne,
        /* [in] */ IBundle* bundleTwo,
        /* [out] */ Boolean* isEqual);

private:
    // TODO: Add your private member variables here.
};

}
}
}

#endif // __ELASTOS_DROID_CONTENT_CSYNCSTORAGEENGINEHELPER_H__
