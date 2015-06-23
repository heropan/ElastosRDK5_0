
#include "ext/frameworkext.h"
#include "app/CActivityNonConfigurationInstances.h"


namespace Elastos {
namespace Droid {
namespace App {


ECode CActivityNonConfigurationInstances::constructor()
{
    return E_NOT_IMPLEMENTED;
}

ECode CActivityNonConfigurationInstances::constructor(
    /* [in] */ IInterface* activity,
    /* [in] */ IObjectStringMap* children,
    /* [in] */ IObjectContainer* fragments,
    /* [in] */ IObjectStringMap* loaders)
{
    mActivity = activity;
    mChildren = children;
    mFragments = fragments;
    return E_NOT_IMPLEMENTED;
}

ECode CActivityNonConfigurationInstances::SetActivity(
    /* [in] */ IInterface* activity)
{
    mActivity = activity;
    return NOERROR;
}

ECode CActivityNonConfigurationInstances::GetActivity(
    /* [out] */ IInterface** activity)
{
    VALIDATE_NOT_NULL(activity);
    *activity = mActivity;
    REFCOUNT_ADD(*activity);
    return NOERROR;
}

ECode CActivityNonConfigurationInstances::SetChildren(
    /* [in] */ IObjectStringMap* children)
{
    mChildren = children;
    return NOERROR;
}

ECode CActivityNonConfigurationInstances::GetChildren(
    /* [out] */ IObjectStringMap** children)
{
    VALIDATE_NOT_NULL(children);
    *children = mChildren;
    REFCOUNT_ADD(*children);
    return NOERROR;
}

ECode CActivityNonConfigurationInstances::SetFragments(
    /* [in] */ IObjectContainer* fragments)
{
    mFragments = fragments;
    return NOERROR;
}

ECode CActivityNonConfigurationInstances::GetFragments(
    /* [out] */ IObjectContainer** fragments)
{
    VALIDATE_NOT_NULL(fragments);
    *fragments = mFragments;
    REFCOUNT_ADD(*fragments);
    return NOERROR;
}

ECode CActivityNonConfigurationInstances::SetLoaders(
    /* [in] */ IObjectStringMap* loaders)
{
    mLoaders = loaders;
    return NOERROR;
}

ECode CActivityNonConfigurationInstances::GetLoaders(
    /* [out] */ IObjectStringMap** loaders)
{
    VALIDATE_NOT_NULL(loaders);
    *loaders = mLoaders;
    REFCOUNT_ADD(*loaders);
    return NOERROR;
}


} // namespace App
} // namespace Droid
} // namespace Elastos

