
#include "elastos/droid/app/CPendingIntent.h"
#include "elastos/droid/app/CFinishedDispatcher.h"
#include "elastos/droid/app/ActivityManagerNative.h"
#include "elastos/droid/os/UserHandle.h"
#include "elastos/droid/os/CUserHandle.h"
#include "elastos/droid/os/CUserHandleHelper.h"
#include "elastos/droid/content/CIntentSender.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::EIID_IIntentReceiver;
using Elastos::Droid::Content::EIID_IIIntentSender;
using Elastos::Droid::Content::CIntentSender;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Os::CUserHandle;
using Elastos::Droid::Os::EIID_IBinder;
using Elastos::Droid::Os::IUserHandleHelper;
using Elastos::Droid::Os::CUserHandleHelper;
using Elastos::Droid::Content::EIID_IIntentReceiver;

namespace Elastos{
namespace Droid{
namespace App{

//============================================================================
//              CPendingIntent
//============================================================================
ECode CPendingIntent::GetIntentSender(
    /* [out] */ IIntentSender** intentSender)
{
    VALIDATE_NOT_NULL(intentSender)
    return CIntentSender::New(mTarget, intentSender);
}

ECode CPendingIntent::Cancel()
{
    // try {
    return ActivityManagerNative::GetDefault()->CancelIntentSender(mTarget);
    // } catch (RemoteException e) {
    // }
}

ECode CPendingIntent::Send()
{
    return Send(NULL, 0, NULL, NULL, NULL, String(NULL));
}

ECode CPendingIntent::Send(
    /* [in] */ Int32 code)
{
    return Send(NULL, code, NULL, NULL, NULL, String(NULL));
}

ECode CPendingIntent::Send(
    /* [in] */ IContext *context,
    /* [in] */ Int32 code,
    /* [in] */ IIntent *intent)
{
    return Send(context, code, intent, NULL, NULL, String(NULL));
}

ECode CPendingIntent::Send(
    /* [in] */ Int32 code,
    /* [in] */ IPendingIntentOnFinished *onFinished,
    /* [in] */ IHandler *handler)
{
    return Send(NULL, code, NULL, onFinished, handler, String(NULL));
}

ECode CPendingIntent::Send(
    /* [in] */ IContext *context,
    /* [in] */ Int32 code,
    /* [in] */ IIntent *intent,
    /* [in] */ IPendingIntentOnFinished *onFinished,
    /* [in] */ IHandler *handler)
{
    return Send(context, code, intent, onFinished, handler, String(NULL));
}

ECode CPendingIntent::Send(
    /* [in] */ IContext *context,
    /* [in] */ Int32 code,
    /* [in] */ IIntent *intent,
    /* [in] */ IPendingIntentOnFinished *onFinished,
    /* [in] */ IHandler *handler,
    /* [in] */ const String& requiredPermission)
{
    // try {
    String resolvedType;
    if (intent != NULL) {
        AutoPtr<IContentResolver> resolver;
        FAIL_RETURN(context->GetContentResolver((IContentResolver **)&resolver));
        FAIL_RETURN(intent->ResolveTypeIfNeeded(resolver, &resolvedType));
    }

    AutoPtr<IIntentReceiver> receiver;
    if (onFinished != NULL) {
        FAIL_RETURN(CFinishedDispatcher::New(this, onFinished, handler, (IIntentReceiver **)&receiver));
    }

    Int32 res;
    FAIL_RETURN(mTarget->Send(code, intent, resolvedType, receiver, requiredPermission, &res));
    if (res < 0) {
        // throw new CanceledException();
        return E_CANCELED_EXCEPTION;
    }

    return NOERROR;
    // } catch (RemoteException e) {
    //     throw new CanceledException(e);
    // }
}

ECode CPendingIntent::GetTargetPackage(
    /* [out] */ String *targetPackage)
{
    //try {
    return ActivityManagerNative::GetDefault()->GetPackageForIntentSender(mTarget, targetPackage);
    //} catch (RemoteException e) {
    // Should never happen.
    //    return null;
    //}
}

ECode CPendingIntent::GetCreatorPackage(
    /* [out] */ String *creatorPackage)
{
    // try {
    return ActivityManagerNative::GetDefault()->GetPackageForIntentSender(mTarget, creatorPackage);
    // } catch (RemoteException e) {
    //     // Should never happen.
    //     return null;
    // }
}

ECode CPendingIntent::GetCreatorUid(
    /* [out] */ Int32 *uid)
{
    //try {
    return ActivityManagerNative::GetDefault()->GetUidForIntentSender(mTarget, uid);
    //} catch (RemoteException e) {
    // Should never happen.
    //    return -1;
    //}
}

ECode CPendingIntent::GetCreatorUserHandle(
    /* [out] */ IUserHandle** userHandle)
{
    VALIDATE_NOT_NULL(userHandle);
    *userHandle = NULL;

    //try {
    AutoPtr<IUserHandle> handle;
    Int32 uid;
    ActivityManagerNative::GetDefault()->GetUidForIntentSender(mTarget, &uid);
    if (uid > 0) {
        AutoPtr<IUserHandleHelper> handleHelper;
        FAIL_RETURN(CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&handleHelper));
        Int32 userId = 0;
        FAIL_RETURN(handleHelper->GetUserId(uid, &userId));
        FAIL_RETURN(CUserHandle::New(userId, (IUserHandle**)&handle));
    }

    *userHandle = handle;
    REFCOUNT_ADD(*userHandle);
    return NOERROR;
}

ECode CPendingIntent::IsTargetedToPackage(
    /* [out] */ Boolean *result)
{
    //try {
    return ActivityManagerNative::GetDefault()->IsIntentSenderTargetedToPackage(mTarget, result);
    //} catch (RemoteException e) {
    // Should never happen.
    //    return false;
    //}
}

ECode CPendingIntent::IsActivity(
    /* [out] */ Boolean *result)
{
    //try {
        return ActivityManagerNative::GetDefault()->IsIntentSenderAnActivity(mTarget, result);
    //} catch (RemoteException e) {
    // Should never happen.
    //    return false;
    //}
}

ECode CPendingIntent::GetIntent(
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent);
    // try {
    return ActivityManagerNative::GetDefault()->GetIntentForIntentSender(mTarget, intent);
    // } catch (RemoteException e) {
    //     // Should never happen.
    //     return null;
    // }
}

ECode CPendingIntent::GetTarget(
    /* [out] */ IIIntentSender **target)
{
    VALIDATE_NOT_NULL(target);

    *target = mTarget;
    REFCOUNT_ADD(*target);
    return NOERROR;
}

ECode CPendingIntent::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    AutoPtr<IInterface> obj;
    source->ReadInterfacePtr((Handle32*)(IInterface**)&obj);
    mTarget = IIIntentSender::Probe(obj.Get());
    return NOERROR;
}

ECode CPendingIntent::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    IBinder* binder = IBinder::Probe(mTarget.Get());
    return dest->WriteInterfacePtr((IInterface*)binder);
}

ECode CPendingIntent::constructor()
{
    return NOERROR;
}

ECode CPendingIntent::constructor(
    /* [in] */ IIIntentSender* target1)
{
    mTarget = target1;
    return NOERROR;
}

ECode CPendingIntent::constructor(
    /* [in] */ IBinder* target2)
{
    mTarget = IIIntentSender::Probe(target2);
    return NOERROR;
}

ECode CPendingIntent::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    *hash = 0;

    IObject* obj = IObject::Probe(mTarget.Get());
    if (obj != NULL) {
        return obj->GetHashCode(hash);
    }
    return NOERROR;
}

ECode CPendingIntent::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* rst)
{
    VALIDATE_NOT_NULL(rst);
    *rst = FALSE;

    if (o == NULL){
        return NOERROR;
    }

    AutoPtr<IPendingIntent> temp = IPendingIntent::Probe(o);
    if (temp != NULL) {
        if(temp.Get() == (IPendingIntent*)this) {
            *rst = TRUE;
            return NOERROR;
        }

        AutoPtr<IIIntentSender> target;
        temp->GetTarget((IIIntentSender**)&target);
        if(target.Get() == mTarget) {
            *rst = TRUE;
        }
    }
    return NOERROR;
}

ECode CPendingIntent::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    StringBuilder sb("CPendingIntent: target: 0X");
    sb += StringUtils::Int32ToHexString((Int32)(mTarget.Get()), TRUE);
    *info = sb.ToString();
    return NOERROR;
}


}
}
}
