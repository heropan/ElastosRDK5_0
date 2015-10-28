
#include "elastos/droid/app/CPendingIntentHelper.h"
#include "elastos/droid/app/CPendingIntent.h"
#include "elastos/droid/app/ActivityManagerNative.h"
#include "elastos/droid/os/UserHandle.h"
#include "elastos/droid/os/CUserHandle.h"

using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IIIntentSender;
using Elastos::Droid::Os::CUserHandle;
using Elastos::Droid::Os::UserHandle;

namespace Elastos{
namespace Droid{
namespace App{

CAR_INTERFACE_IMPL(CPendingIntentHelper, Singleton, IPendingIntentHelper)

CAR_SINGLETON_IMPL(CPendingIntentHelper)

ECode CPendingIntentHelper::GetActivity(
    /* [in] */ IContext *context,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent *intent,
    /* [in] */ Int32 flags,
    /* [out] */ IPendingIntent **pendingIntent)
{
    return GetActivity(context, requestCode, intent, flags, NULL, pendingIntent);
}

ECode CPendingIntentHelper::GetActivity(
    /* [in] */ IContext *context,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent *intent,
    /* [in] */ Int32 flags,
    /* [in] */ IBundle *options,
    /* [out] */ IPendingIntent **pendingIntent)
{
    VALIDATE_NOT_NULL(pendingIntent);

    String packageName;
    context->GetPackageName(&packageName);
    String resolvedType;
    if (intent != NULL) {
        AutoPtr<IContentResolver> cr;
        context->GetContentResolver((IContentResolver**)&cr);
        intent->ResolveTypeIfNeeded(cr,&resolvedType);
    }
    //try {
    Boolean bval;
    intent->MigrateExtraStreamToClipData(&bval);
    intent->PrepareToLeaveProcess();

    AutoPtr<ArrayOf<IIntent*> > intents = ArrayOf<IIntent*>::Alloc(1);
    intents->Set(0, intent);
    AutoPtr<ArrayOf<String> > resolvedTypes;
    if (!resolvedType.IsNullOrEmpty()) {
        resolvedTypes = ArrayOf<String>::Alloc(1);
        resolvedTypes->Set(0, resolvedType);
    }

    String nullStr;
    AutoPtr<IIIntentSender> target;
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    am->GetIntentSender(
        IActivityManager::INTENT_SENDER_ACTIVITY, packageName,
        NULL, nullStr, requestCode,
        intents, resolvedTypes,
        flags, options, UserHandle::GetMyUserId(), (IIIntentSender**)&target);

    if (target != NULL){
        CPendingIntent::New(target, pendingIntent);
    }
    else{
        *pendingIntent = NULL;
    }
    return NOERROR;
    //} catch (RemoteException e) {
    //}
}

ECode CPendingIntentHelper::GetActivityAsUser(
    /* [in] */ IContext *context,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent *intent,
    /* [in] */ Int32 flags,
    /* [in] */ IBundle *options,
    /* [in] */ IUserHandle *user,
    /* [out] */ IPendingIntent **pendingIntent)
{
    VALIDATE_NOT_NULL(pendingIntent);

    String packageName;
    context->GetPackageName(&packageName);
    String resolvedType;
    if(intent != NULL) {
        AutoPtr<IContentResolver> cr;
        context->GetContentResolver((IContentResolver**)&cr);
        intent->ResolveTypeIfNeeded(cr,&resolvedType);
    }

    //try {
    Boolean bval;
    intent->MigrateExtraStreamToClipData(&bval);
    intent->PrepareToLeaveProcess();

    AutoPtr<IIIntentSender> target;
    Int32 id;
    user->GetIdentifier(&id);

    AutoPtr<ArrayOf<IIntent*> > intents = ArrayOf<IIntent*>::Alloc(1);
    intents->Set(0, intent);
    AutoPtr<ArrayOf<String> > resolvedTypes;
    if (!resolvedType.IsNullOrEmpty()) {
        resolvedTypes = ArrayOf<String>::Alloc(1);
        resolvedTypes->Set(0, resolvedType);
    }

    String nullStr;
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    am->GetIntentSender(
        IActivityManager::INTENT_SENDER_ACTIVITY, packageName,
        NULL, nullStr, requestCode, intents,
        resolvedTypes,
        flags, options, id, (IIIntentSender**)&target);

    if(target != NULL){
        CPendingIntent::New(target, pendingIntent);
    }
    else{
        *pendingIntent = NULL;
    }
    return NOERROR;
    //} catch (RemoteException e) {
    //}
}

ECode CPendingIntentHelper::GetActivities(
    /* [in] */ IContext *context,
    /* [in] */ Int32 requestCode,
    /* [in] */ ArrayOf<IIntent *> *intents,
    /* [in] */ Int32 flags,
    /* [out] */ IPendingIntent **pendingIntent)
{
    return GetActivities(context, requestCode, intents, flags, NULL, pendingIntent);
}

ECode CPendingIntentHelper::GetActivities(
    /* [in] */ IContext *context,
    /* [in] */ Int32 requestCode,
    /* [in] */ ArrayOf<IIntent *> *intents,
    /* [in] */ Int32 flags,
    /* [in] */ IBundle *options,
    /* [out] */ IPendingIntent **pendingIntent)
{
    VALIDATE_NOT_NULL(pendingIntent);

    Boolean bval;
    String packageName;
    context->GetPackageName(&packageName);
    AutoPtr<IContentResolver> cr;
    context->GetContentResolver((IContentResolver**)&cr);
    AutoPtr<ArrayOf<String> > resolvedTypes = ArrayOf<String>::Alloc(intents->GetLength());
    for (Int32 i = 0; i < intents->GetLength(); i++) {
        (*intents)[i]->MigrateExtraStreamToClipData(&bval);
        (*intents)[i]->PrepareToLeaveProcess();
        (*intents)[i]->ResolveTypeIfNeeded(cr.Get(), &((*resolvedTypes)[i]));
    }

    //try {
    String nullStr;
    AutoPtr<IIIntentSender> target;
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    am->GetIntentSender(
        IActivityManager::INTENT_SENDER_ACTIVITY, packageName,
        NULL, nullStr, requestCode, intents, resolvedTypes,
        flags, options, UserHandle::GetMyUserId(), (IIIntentSender**)&target);

    if(target != NULL){
        CPendingIntent::New(target, pendingIntent);
    }
    else{
        *pendingIntent = NULL;
    }
    return NOERROR;
    // } catch (RemoteException e) {
    // }
}

ECode CPendingIntentHelper::GetActivitiesAsUser(
    /* [in] */ IContext *context,
    /* [in] */ Int32 requestCode,
    /* [in] */ ArrayOf<IIntent *> *intents,
    /* [in] */ Int32 flags,
    /* [in] */ IBundle *options,
    /* [in] */ IUserHandle *user,
    /* [out] */ IPendingIntent **pendingIntent)
{
    VALIDATE_NOT_NULL(pendingIntent);

    Boolean bval;
    String packageName;
    context->GetPackageName(&packageName);
    AutoPtr<IContentResolver> cr;
    context->GetContentResolver((IContentResolver**)&cr);
    AutoPtr<ArrayOf<String> > resolvedTypes = ArrayOf<String>::Alloc(intents->GetLength());
    for (Int32 i = 0; i < intents->GetLength(); i++) {
        (*intents)[i]->MigrateExtraStreamToClipData(&bval);
        (*intents)[i]->PrepareToLeaveProcess();
        (*intents)[i]->ResolveTypeIfNeeded(cr, &(*resolvedTypes)[i]);
    }
    //try {
    Int32 id;
    user->GetIdentifier(&id);
    String nullStr;
    AutoPtr<IIIntentSender> target;
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    am->GetIntentSender(
        IActivityManager::INTENT_SENDER_ACTIVITY, packageName,
        NULL, nullStr, requestCode, intents, resolvedTypes,
        flags, options, id, (IIIntentSender**)&target);

    if(target != NULL){
        CPendingIntent::New(target, pendingIntent);
    }
    else{
        *pendingIntent = NULL;
    }
    return NOERROR;
    // } catch (RemoteException e) {
    // }
}

ECode CPendingIntentHelper::GetBroadcast(
    /* [in] */ IContext *context,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent *intent,
    /* [in] */ Int32 flags,
    /* [out] */ IPendingIntent **pendingIntent)
{
    AutoPtr<IUserHandle> handle;
    CUserHandle::New(UserHandle::GetMyUserId(), (IUserHandle**)&handle);
    return GetBroadcastAsUser(context, requestCode, intent, flags, handle, pendingIntent);
}

ECode CPendingIntentHelper::GetBroadcastAsUser(
    /* [in] */ IContext *context,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent *intent,
    /* [in] */ Int32 flags,
    /* [in] */ IUserHandle *userHandle,
    /* [out] */ IPendingIntent **pendingIntent)
{
    VALIDATE_NOT_NULL(pendingIntent);

    String packageName;
    context->GetPackageName(&packageName);
    String resolvedType;
    if(intent != NULL) {
        AutoPtr<IContentResolver> cr;
        context->GetContentResolver((IContentResolver**)&cr);
        intent->ResolveTypeIfNeeded(cr,&resolvedType);
    }

    //try {
    intent->PrepareToLeaveProcess();
    Int32 id;
    userHandle->GetIdentifier(&id);
    AutoPtr<IIIntentSender> target;
    AutoPtr<ArrayOf<IIntent*> > intents= ArrayOf<IIntent*>::Alloc(1);
    intents->Set(0, intent);
    AutoPtr<ArrayOf<String> > resolvedTypes = ArrayOf<String>::Alloc(1);
    (*resolvedTypes)[0] = resolvedType;
    ActivityManagerNative::GetDefault()->GetIntentSender(
        IActivityManager::INTENT_SENDER_BROADCAST, packageName,
        NULL, String(NULL), requestCode, intents,
        !resolvedType.IsNullOrEmpty() ? resolvedTypes : NULL,
        flags, NULL, id, (IIIntentSender**)&target);
    if(target != NULL){
        CPendingIntent::New(target, pendingIntent);
    }
    else{
        *pendingIntent = NULL;
    }
    return NOERROR;
    // } catch (RemoteException e) {
    // }
}

ECode CPendingIntentHelper::GetService(
    /* [in] */ IContext *context,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent *intent,
    /* [in] */ Int32 flags,
    /* [out] */ IPendingIntent **pendingIntent)
{
    VALIDATE_NOT_NULL(pendingIntent);

    String packageName;
    context->GetPackageName(&packageName);
    String resolvedType;
    if(intent != NULL) {
        AutoPtr<IContentResolver> cr;
        context->GetContentResolver((IContentResolver**)&cr);
        intent->ResolveTypeIfNeeded(cr,&resolvedType);
    }
    //try {
    intent->PrepareToLeaveProcess();

    AutoPtr<ArrayOf<IIntent*> > intents = ArrayOf<IIntent*>::Alloc(1);
    intents->Set(0, intent);
    AutoPtr<ArrayOf<String> > resolvedTypes;
    if (!resolvedType.IsNullOrEmpty()) {
        resolvedTypes = ArrayOf<String>::Alloc(1);
        resolvedTypes->Set(0, resolvedType);
    }

    String nullStr;
    AutoPtr<IIIntentSender> target;
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();
    am->GetIntentSender(
        IActivityManager::INTENT_SENDER_SERVICE, packageName,
        NULL, nullStr, requestCode, intents,
        resolvedTypes,
        flags, NULL, UserHandle::GetMyUserId(), (IIIntentSender**)&target);

    if(target != NULL){
        CPendingIntent::New(target, pendingIntent);
    }
    else{
        *pendingIntent = NULL;
    }
    return NOERROR;
    // } catch (RemoteException e) {
    // }
}

ECode CPendingIntentHelper::WritePendingIntentOrNullToParcel(
    /* [in] */ IPendingIntent *sender,
    /* [in] */ IParcel *out)
{
    assert(0);
    // out.writeStrongBinder(sender != null ? sender.mTarget.asBinder()
    //         : null);
    return E_NOT_IMPLEMENTED;
}

ECode CPendingIntentHelper::ReadPendingIntentOrNullFromParcel(
    /* [in] */ IParcel *in,
    /* [out] */ IPendingIntent **messenger)
{
    assert(0);
    // IBinder b = in.readStrongBinder();
    // return b != null ? new PendingIntent(b) : null;
    return E_NOT_IMPLEMENTED;
}

}
}
}
