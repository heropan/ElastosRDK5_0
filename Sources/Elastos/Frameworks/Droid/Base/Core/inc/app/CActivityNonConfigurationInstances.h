
#ifndef __ELASTOS_DROID_APP_CACTIVITYNONCONFIGURATIONINSTANCES_H__
#define __ELASTOS_DROID_APP_CACTIVITYNONCONFIGURATIONINSTANCES_H__

#include "_Elastos_Droid_App_CActivityNonConfigurationInstances.h"

using Elastos::Utility::IObjectStringMap;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CActivityNonConfigurationInstances)
{
public:

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IInterface* activity,
        /* [in] */ IObjectStringMap* children,
        /* [in] */ IObjectContainer* fragments,
        /* [in] */ IObjectStringMap* loaders);

    CARAPI SetActivity(
        /* [in] */ IInterface* activity);

    CARAPI GetActivity(
        /* [out] */ IInterface** activity);

    CARAPI SetChildren(
        /* [in] */ IObjectStringMap* children);

    CARAPI GetChildren(
        /* [out] */ IObjectStringMap** children);

    CARAPI SetFragments(
        /* [in] */ IObjectContainer* fragments);

    CARAPI GetFragments(
        /* [out] */ IObjectContainer** fragments);

    CARAPI SetLoaders(
        /* [in] */ IObjectStringMap* loaders);

    CARAPI GetLoaders(
        /* [out] */ IObjectStringMap** loaders);

private:
    AutoPtr<IInterface> mActivity;
    AutoPtr<IObjectStringMap> mChildren;
    AutoPtr<IObjectContainer> mFragments;
    AutoPtr<IObjectStringMap> mLoaders;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_CACTIVITYNONCONFIGURATIONINSTANCES_H__
