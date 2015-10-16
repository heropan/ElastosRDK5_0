
#include "elastos/droid/app/CApplication.h"
#include "elastos/droid/app/CContextImpl.h"

using Elastos::Droid::Content::EIID_IContext;
using Elastos::Droid::Content::EIID_IContextWrapper;
using Elastos::Droid::Content::EIID_IComponentCallbacks2;

namespace Elastos {
namespace Droid {
namespace App {

ICONTEXT_METHODS_IMPL(_CApplicationInner, ContextWrapper);

PInterface _CApplicationInner::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IApplication*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    else if (riid == EIID_IContext) {
        return (IContext*)this;
    }
    else if (riid == EIID_IContextWrapper) {
        return (IContextWrapper*)this;
    }
    else if (riid == EIID_IApplication) {
        return (IApplication*)this;
    }
    else if (riid == EIID_IComponentCallbacks2) {
        return (IComponentCallbacks2*)this;
    }
    else if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource*)this;
    }

    return NULL;
}

UInt32 _CApplicationInner::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 _CApplicationInner::Release()
{
    return ElRefBase::Release();
}

ECode _CApplicationInner::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IObject*)this) {
        *pIID = EIID_IObject;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IContext*)this) {
        *pIID = EIID_IContext;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IContextWrapper*)this) {
        *pIID = EIID_IContextWrapper;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IApplication*)this) {
        *pIID = EIID_IApplication;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IComponentCallbacks2*)this) {
        *pIID = EIID_IComponentCallbacks2;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IWeakReferenceSource*)this) {
        *pIID = EIID_IWeakReferenceSource;
        return NOERROR;
    }

    return NOERROR;
}

ECode _CApplicationInner::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return NOERROR;
}

ECode _CApplicationInner::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return NOERROR;
}

ECode _CApplicationInner::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return NOERROR;
}

ECode _CApplicationInner::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result)
    IApplication* o = IApplication::Probe(other);
    if (o != NULL) {
        *result = (o == THIS_PROBE(IApplication));
    }
    return NOERROR;
}

ECode _CApplicationInner::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash)
    *hash = (Int32)THIS_PROBE(IApplication);
    return NOERROR;
}

ECode _CApplicationInner::ToString(
    /* [out] */ String* str)
{
    return NOERROR;
}

ECode _CApplicationInner::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(THIS_PROBE(IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

ECode _CApplicationInner::GetBaseContext(
    /* [out] */ IContext** context)
{
    return ContextWrapper::GetBaseContext(context);
}


ECode CApplication::constructor()
{
    return ContextWrapper::Init(NULL);
}

ECode CApplication::Initialize()
{
    return NOERROR;
}

ECode CApplication::OnCreate()
{
    return NOERROR;
}

ECode CApplication::OnTerminate()
{
    return NOERROR;
}

ECode CApplication::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    AutoPtr< ArrayOf<IComponentCallbacks*> > callbacks = CollectComponentCallbacks();
    if (callbacks != NULL) {
        for (Int32 i = 0; i < callbacks->GetLength(); i++) {
            (*callbacks)[i]->OnConfigurationChanged(newConfig);
        }
    }
    return NOERROR;
}

ECode CApplication::OnLowMemory()
{
    AutoPtr< ArrayOf<IComponentCallbacks*> > callbacks = CollectComponentCallbacks();
    if (callbacks != NULL) {
        for (Int32 i = 0; i < callbacks->GetLength(); i++) {
            (*callbacks)[i]->OnLowMemory();
        }
    }
    return NOERROR;
}

ECode CApplication::OnTrimMemory(
    /* [in] */ Int32 level)
{
    AutoPtr< ArrayOf<IComponentCallbacks*> > callbacks = CollectComponentCallbacks();
    if (callbacks != NULL) {
        for (Int32 i = 0; i < callbacks->GetLength(); i++) {
            IComponentCallbacks2* cb2 = IComponentCallbacks2::Probe((*callbacks)[i]);
            if (cb2 != NULL) {
                cb2->OnTrimMemory(level);
            }
        }
    }
    return NOERROR;
}

ECode CApplication::RegisterComponentCallbacks(
    /* [in] */ IComponentCallbacks* componentCallback)
{
    AutoLock lock(mComponentCallbacksLock);
    mComponentCallbacks.PushBack(componentCallback);
    return NOERROR;
}

ECode CApplication::UnregisterComponentCallbacks(
    /* [in] */ IComponentCallbacks* componentCallback)
{
    AutoLock lock(mComponentCallbacksLock);
    mComponentCallbacks.Remove(componentCallback);
    return NOERROR;
}

ECode CApplication::RegisterActivityLifecycleCallbacks(
    /* [in] */ IActivityLifecycleCallbacks* cb)
{
    AutoLock lock(mActivityLifecycleCallbacksLock);
    mActivityLifecycleCallbacks.PushBack(cb);
    return NOERROR;
}

ECode CApplication::UnregisterActivityLifecycleCallbacks(
    /* [in] */ IActivityLifecycleCallbacks* cb)
{
    AutoLock lock(mActivityLifecycleCallbacksLock);
    mActivityLifecycleCallbacks.Remove(cb);
    return NOERROR;
}

/**
 * @hide
 */
/* package */
ECode CApplication::Attach(
    /* [in] */ IContext* context)
{
    AttachBaseContext(context);
    mLoadedPkg = CContextImpl::GetImpl(context)->mPackageInfo;
    return NOERROR;
}

ECode CApplication::DispatchActivityCreated(
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* savedInstanceState)
{
    AutoPtr< ArrayOf<IActivityLifecycleCallbacks*> > callbacks = CollectActivityLifecycleCallbacks();
    if (callbacks != NULL) {
        for (Int32 i = 0; i < callbacks->GetLength(); i++) {
            (*callbacks)[i]->OnActivityCreated(activity, savedInstanceState);
        }
    }
    return NOERROR;
}

ECode CApplication::DispatchActivityStarted(
    /* [in] */ IActivity* activity)
{
    AutoPtr< ArrayOf<IActivityLifecycleCallbacks*> > callbacks = CollectActivityLifecycleCallbacks();
    if (callbacks != NULL) {
        for (Int32 i = 0; i < callbacks->GetLength(); i++) {
            (*callbacks)[i]->OnActivityStarted(activity);
        }
    }
    return NOERROR;
}

ECode CApplication::DispatchActivityResumed(
    /* [in] */ IActivity* activity)
{
    AutoPtr< ArrayOf<IActivityLifecycleCallbacks*> > callbacks = CollectActivityLifecycleCallbacks();
    if (callbacks != NULL) {
        for (Int32 i = 0; i < callbacks->GetLength(); i++) {
            (*callbacks)[i]->OnActivityResumed(activity);
        }
    }
    return NOERROR;
}

ECode CApplication::DispatchActivityPaused(
    /* [in] */ IActivity* activity)
{
    AutoPtr< ArrayOf<IActivityLifecycleCallbacks*> > callbacks = CollectActivityLifecycleCallbacks();
    if (callbacks != NULL) {
        for (Int32 i = 0; i < callbacks->GetLength(); i++) {
            (*callbacks)[i]->OnActivityPaused(activity);
        }
    }
    return NOERROR;
}

ECode CApplication::DispatchActivityStopped(
    /* [in] */ IActivity* activity)
{
    AutoPtr< ArrayOf<IActivityLifecycleCallbacks*> > callbacks = CollectActivityLifecycleCallbacks();
    if (callbacks != NULL) {
        for (Int32 i = 0; i < callbacks->GetLength(); i++) {
            (*callbacks)[i]->OnActivityStopped(activity);
        }
    }
    return NOERROR;
}

ECode CApplication::DispatchActivitySaveInstanceState(
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* outState)
{
    AutoPtr< ArrayOf<IActivityLifecycleCallbacks*> > callbacks = CollectActivityLifecycleCallbacks();
    if (callbacks != NULL) {
        for (Int32 i = 0; i < callbacks->GetLength(); i++) {
            (*callbacks)[i]->OnActivitySaveInstanceState(activity, outState);
        }
    }
    return NOERROR;
}

ECode CApplication::DispatchActivityDestroyed(
    /* [in] */ IActivity* activity)
{
    AutoPtr< ArrayOf<IActivityLifecycleCallbacks*> > callbacks = CollectActivityLifecycleCallbacks();
    if (callbacks != NULL) {
        for (Int32 i = 0; i < callbacks->GetLength(); i++) {
            (*callbacks)[i]->OnActivityDestroyed(activity);
        }
    }
    return NOERROR;
}

AutoPtr< ArrayOf<IComponentCallbacks*> > CApplication::CollectComponentCallbacks()
{
    AutoPtr< ArrayOf<IComponentCallbacks*> > callbacks;
    {
        AutoLock lock(mComponentCallbacksLock);

        Int32 size = mComponentCallbacks.GetSize();
        if (size > 0) {
            callbacks = ArrayOf<IComponentCallbacks*>::Alloc(size);
            List< AutoPtr<IComponentCallbacks> >::Iterator it;
            Int32 i;
            for (it = mComponentCallbacks.Begin(), i = 0; it != mComponentCallbacks.End(); ++it, ++i) {
                callbacks->Set(i, *it);
            }
        }
    }
    return callbacks;
}

AutoPtr< ArrayOf<IActivityLifecycleCallbacks*> > CApplication::CollectActivityLifecycleCallbacks()
{
    AutoPtr< ArrayOf<IActivityLifecycleCallbacks*> > callbacks;
    {
        AutoLock lock(mActivityLifecycleCallbacksLock);

        Int32 size = mActivityLifecycleCallbacks.GetSize();
        if (size > 0) {
            callbacks = ArrayOf<IActivityLifecycleCallbacks*>::Alloc(size);
            List< AutoPtr<IActivityLifecycleCallbacks> >::Iterator it;
            Int32 i;
            for (it = mActivityLifecycleCallbacks.Begin(), i = 0; it != mActivityLifecycleCallbacks.End(); ++it, ++i) {
                callbacks->Set(i, *it);
            }
        }

    }
    return callbacks;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
