
#ifndef __ELASTOS_DROID_APP_CACTIVITYNONCONFIGURATIONINSTANCES_H__
#define __ELASTOS_DROID_APP_CACTIVITYNONCONFIGURATIONINSTANCES_H__

#include "_Elastos_Droid_App_CActivityNonConfigurationInstances.h"
#include <elastos/core/Object.h>

using Elastos::Utility::IObjectStringMap;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CActivityNonConfigurationInstances)
    , public Object
    , public IActivityNonConfigurationInstances
{
public:

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IInterface* activity,
        /* [in] */ IHashMap* children,
        /* [in] */ IArrayList* fragments,
        /* [in] */ IArrayMap* loaders,
        /* [in] */ IVoiceInteractor* vi);

    CARAPI SetActivity(
        /* [in] */ IInterface* activity);

    CARAPI GetActivity(
        /* [out] */ IInterface** activity);

    CARAPI SetChildren(
        /* [in] */ IHashMap* children);

    CARAPI GetChildren(
        /* [out] */ IHashMap** children);

    CARAPI SetFragments(
        /* [in] */ IArrayList* fragments);

    CARAPI GetFragments(
        /* [out] */ IArrayList** fragments);

    CARAPI SetLoaders(
        /* [in] */ IArrayMap* loaders);

    CARAPI GetLoaders(
        /* [out] */ IArrayMap** loaders);

private:
    AutoPtr<IInterface> mActivity;
    AutoPtr<IHashMap> mChildren; //HashMap<String, Object>
    AutoPtr<IArrayList> mFragments; //ArrayList<Fragment>
    AutoPtr<IArrayMap> mLoaders;//ArrayMap<String, LoaderManagerImpl>
    AutoPtr<IVoiceInteractor> mVoiceInteractor;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_CACTIVITYNONCONFIGURATIONINSTANCES_H__
