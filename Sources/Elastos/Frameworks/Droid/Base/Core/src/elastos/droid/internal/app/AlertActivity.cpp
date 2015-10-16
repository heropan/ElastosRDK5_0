
#include "elastos/droid/app/AlertActivity.h"

using Elastos::Droid::App::EIID_IActivity;
using Elastos::Droid::Content::EIID_IContext;
using Elastos::Droid::Content::EIID_IContextWrapper;
using Elastos::Droid::Content::EIID_IDialogInterface;
using Elastos::Droid::Content::EIID_IComponentCallbacks2;
using Elastos::Droid::View::EIID_IWindowCallback;
using Elastos::Droid::View::EIID_IKeyEventCallback;
using Elastos::Droid::View::EIID_IContextThemeWrapper;
using Elastos::Droid::View::EIID_ILayoutInflaterFactory2;
using Elastos::Droid::View::EIID_IViewOnCreateContextMenuListener;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

PInterface AlertActivity::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IActivity*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    else if (riid == EIID_IActivity) {
        return (IActivity*)this;
    }
    else if (riid == EIID_IContextThemeWrapper) {
        return (IContextThemeWrapper*)this;
    }
    else if (riid == EIID_IContextWrapper) {
        return (IContextWrapper*)this;
    }
    else if (riid == EIID_IContext) {
        return (IContext*)this;
    }
    else if (riid == EIID_ILayoutInflaterFactory2) {
        return (ILayoutInflaterFactory2*)this;
    }
    else if (riid == EIID_IWindowCallback) {
        return (IWindowCallback*)this;
    }
    else if (riid == EIID_IKeyEventCallback) {
        return (IKeyEventCallback*)this;
    }
    else if (riid == EIID_IViewOnCreateContextMenuListener) {
        return (IViewOnCreateContextMenuListener*)this;
    }
    else if (riid == EIID_IComponentCallbacks2) {
        return (IComponentCallbacks2*)this;
    }
    else if (riid == EIID_IDialogInterface) {
        return (IDialogInterface*)this;
    }
    else if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource*)this;
    }

    return Activity::Probe(riid);
}

ECode AlertActivity::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IActivity *)this) {
        *pIID = EIID_IActivity;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IObject *)this) {
        *pIID = EIID_IObject;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(ILayoutInflaterFactory2 *)this) {
        *pIID = EIID_ILayoutInflaterFactory2;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IWindowCallback *)this) {
        *pIID = EIID_IWindowCallback;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IKeyEventCallback *)this) {
        *pIID = EIID_IKeyEventCallback;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IViewOnCreateContextMenuListener *)this) {
        *pIID = EIID_IViewOnCreateContextMenuListener;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IComponentCallbacks2 *)this) {
        *pIID = EIID_IComponentCallbacks2;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IDialogInterface *)this) {
        *pIID = EIID_IDialogInterface;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IWeakReferenceSource *)this) {
        *pIID = EIID_IWeakReferenceSource;
        return NOERROR;
    }

    return Activity::GetInterfaceID(pObject, pIID);
}

ECode AlertActivity::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    Activity::OnCreate(savedInstanceState);

    CAlertController::New(this, this, GetWindow(), (IAlertController**)&mAlert);
    CAlertControllerAlertParams::New(this, (IAlertControllerAlertParams**)&mAlertParams);
    return NOERROR;
}

ECode AlertActivity::Cancel()
{
    return Finish();
}

ECode AlertActivity::Dismiss()
{
    // This is called after the click, since we finish when handling the
    // click, don't do that again here.
    Boolean finished;
    IsFinishing(&finished);
    if (!finished) {
        Finish();
    }
    return NOERROR;
}

void AlertActivity::SetupAlert()
{
    mAlertParams->Apply(mAlert);
    mAlert->InstallContent();
}

ECode AlertActivity::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* keyDown)
{
    Boolean result;
    mAlert->OnKeyDown(keyCode, event, &result);
    if (result) {
        *keyDown = TRUE;
        return NOERROR;
    }
    Activity::OnKeyDown(keyCode, event, keyDown);
    return NOERROR;
}

ECode AlertActivity::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* keyUp)
{
    Boolean result;
    mAlert->OnKeyUp(keyCode, event, &result);
    if (result) {
        *keyUp = TRUE;
        return NOERROR;
    }
    Activity::OnKeyUp(keyCode, event, keyUp);
    return NOERROR;
}

} // namespace App
} // namespace Internal
} // namespace Droid
} // namespace Elastos
