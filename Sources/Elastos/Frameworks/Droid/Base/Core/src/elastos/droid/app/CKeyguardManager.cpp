
#include "elastos/droid/app/CKeyguardManager.h"
#include "elastos/droid/app/COnKeyguardExitResult.h"
#include "elastos/droid/view/CWindowManagerGlobal.h"

using Elastos::Droid::View::IOnKeyguardExitResult;
using Elastos::Droid::View::CWindowManagerGlobal;

namespace Elastos {
namespace Droid {
namespace App {

CAR_INTERFACE_IMPL(CKeyguardManager::KeyguardLock, IKeyguardManagerKeyguardLock)

ECode CKeyguardManager::KeyguardLock::DisableKeyguard()
{
    // try {
    mOwner->mWM->DisableKeyguard(mToken, mTag);
    return NOERROR;
    // } catch (RemoteException ex) {
    // }
}

ECode CKeyguardManager::KeyguardLock::ReenableKeyguard()
{
    // try {
    //     mWM.reenableKeyguard(mToken);
    mOwner->mWM->ReenableKeyguard(mToken);
    return NOERROR;
    // } catch (RemoteException ex) {
    // }
}


ECode CKeyguardManager::constructor()
{
    mWM = CWindowManagerGlobal::GetWindowManagerService();
    return NOERROR;
}

ECode CKeyguardManager::NewKeyguardLock(
    /* [in] */ const String& tag,
    /* [out] */ IKeyguardManagerKeyguardLock** keyguardLock)
{
    VALIDATE_NOT_NULL(keyguardLock);
    *keyguardLock = new KeyguardLock(tag, this);
    REFCOUNT_ADD(*keyguardLock);
    return NOERROR;
}

ECode CKeyguardManager::IsKeyguardLocked(
    /* [out] */ Boolean* isLocked)
{
    VALIDATE_NOT_NULL(isLocked);
    // try {
    ECode ec = mWM->IsKeyguardLocked(isLocked);
    if (FAILED(ec)) *isLocked = FALSE;
    return NOERROR;
    // } catch (RemoteException ex) {
    //     return false;
    // }
}

ECode CKeyguardManager::IsKeyguardSecure(
    /* [out] */ Boolean* isSecure)
{
    VALIDATE_NOT_NULL(isSecure);
    // try {
    ECode ec = mWM->IsKeyguardSecure(isSecure);
    if (FAILED(ec)) *isSecure = FALSE;
    return NOERROR;
    // } catch (RemoteException ex) {
    //     return false;
    // }
}

ECode CKeyguardManager::InKeyguardRestrictedInputMode(
    /* [out] */ Boolean* isInMode)
{
    VALIDATE_NOT_NULL(isInMode);
    // try {
    ECode ec = mWM->InKeyguardRestrictedInputMode(isInMode);
    if (FAILED(ec)) *isInMode = FALSE;
    return NOERROR;
    // } catch (RemoteException ex) {
    //     return false;
    // }
}

ECode CKeyguardManager::ExitKeyguardSecurely(
    /* [in] */ IKeyguardManagerOnKeyguardExitResult* cb)
{
    AutoPtr<IOnKeyguardExitResult> callback;
    COnKeyguardExitResult::New(cb, (IOnKeyguardExitResult**)&callback);
    // try {
    mWM->ExitKeyguardSecurely(callback);
    return NOERROR;
    //     mWM.exitKeyguardSecurely(new IOnKeyguardExitResult.Stub() {
    //         public void onKeyguardExitResult(boolean success) throws RemoteException {
    //             callback.onKeyguardExitResult(success);
    //         }
    //     });
    // } catch (RemoteException e) {

    // }
}

} // namespace App
} // namespace Droid
} // namespace Elastos
