
#include "elastos/droid/app/CPendingIntentHelper.h"
#include "elastos/droid/app/CPendingIntent.h"
#include "elastos/droid/app/ActivityManagerNative.h"
#include "elastos/droid/os/UserHandle.h"
#include "elastos/droid/os/CUserHandle.h"
#include "elastos/droid/os/CUserHandleHelper.h"

using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IIIntentSender;
using Elastos::Droid::Os::IUserHandleHelper;
using Elastos::Droid::Os::CUserHandleHelper;
using Elastos::Droid::Os::CUserHandle;
using Elastos::Droid::Os::UserHandle;

namespace Elastos{
namespace Droid{
namespace App{

/**
 * Retrieve a PendingIntent that will start a new activity, like calling
 * {@link Context#startActivity(Intent) Context.startActivity(Intent)}.
 * Note that the activity will be started outside of the context of an
 * existing activity, so you must use the {@link Intent#FLAG_ACTIVITY_NEW_TASK
 * Intent.FLAG_ACTIVITY_NEW_TASK} launch flag in the Intent.
 *
 * @param context The Context in which this PendingIntent should start
 * the activity.
 * @param requestCode Private request code for the sender (currently
 * not used).
 * @param intent Intent of the activity to be launched.
 * @param flags May be {@link #FLAG_ONE_SHOT}, {@link #FLAG_NO_CREATE},
 * {@link #FLAG_CANCEL_CURRENT}, {@link #FLAG_UPDATE_CURRENT},
 * or any of the flags as supported by
 * {@link Intent#fillIn Intent.fillIn()} to control which unspecified parts
 * of the intent that can be supplied when the actual send happens.
 *
 * @return Returns an existing or new PendingIntent matching the given
 * parameters.  May return null only if {@link #FLAG_NO_CREATE} has been
 * supplied.
 */
ECode CPendingIntentHelper::GetActivity(
    /* [in] */ IContext *context,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent *intent,
    /* [in] */ Int32 flags,
    /* [out] */ IPendingIntent **pendingIntent)
{
    return GetActivity(context, requestCode, intent, flags, NULL, pendingIntent);
}

/**
 * Retrieve a PendingIntent that will start a new activity, like calling
 * {@link Context#startActivity(Intent) Context.startActivity(Intent)}.
 * Note that the activity will be started outside of the context of an
 * existing activity, so you must use the {@link Intent#FLAG_ACTIVITY_NEW_TASK
 * Intent.FLAG_ACTIVITY_NEW_TASK} launch flag in the Intent.
 *
 * @param context The Context in which this PendingIntent should start
 * the activity.
 * @param requestCode Private request code for the sender (currently
 * not used).
 * @param intent Intent of the activity to be launched.
 * @param flags May be {@link #FLAG_ONE_SHOT}, {@link #FLAG_NO_CREATE},
 * {@link #FLAG_CANCEL_CURRENT}, {@link #FLAG_UPDATE_CURRENT},
 * or any of the flags as supported by
 * {@link Intent#fillIn Intent.fillIn()} to control which unspecified parts
 * of the intent that can be supplied when the actual send happens.
 * @param options Additional options for how the Activity should be started.
 * May be null if there are no options.
 *
 * @return Returns an existing or new PendingIntent matching the given
 * parameters.  May return null only if {@link #FLAG_NO_CREATE} has been
 * supplied.
 */
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
    intent->MigrateExtraStreamToClipData();
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

/**
 * @hide
 * Note that UserHandle.CURRENT will be interpreted at the time the
 * activity is started, not when the pending intent is created.
 */
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
    intent->MigrateExtraStreamToClipData();
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

/**
 * Like {@link #getActivity(Context, int, Intent, int)}, but allows an
 * array of Intents to be supplied.  The first Intent in the array is
 * taken as the primary key for the PendingIntent, like the single Intent
 * given to {@link #getActivity(Context, int, Intent, int)}.  Upon sending
 * the resulting PendingIntent, all of the Intents are started in the same
 * way as they would be by passing them to {@link Context#startActivities(Intent[])}.
 *
 * <p class="note">
 * The <em>first</em> intent in the array will be started outside of the context of an
 * existing activity, so you must use the {@link Intent#FLAG_ACTIVITY_NEW_TASK
 * Intent.FLAG_ACTIVITY_NEW_TASK} launch flag in the Intent.  (Activities after
 * the first in the array are started in the context of the previous activity
 * in the array, so FLAG_ACTIVITY_NEW_TASK is not needed nor desired for them.)
 * </p>
 *
 * <p class="note">
 * The <em>last</em> intent in the array represents the key for the
 * PendingIntent.  In other words, it is the significant element for matching
 * (as done with the single intent given to {@link #getActivity(Context, int, Intent, int)},
 * its content will be the subject of replacement by
 * {@link #send(Context, int, Intent)} and {@link #FLAG_UPDATE_CURRENT}, etc.
 * This is because it is the most specific of the supplied intents, and the
 * UI the user actually sees when the intents are started.
 * </p>
 *
 * @param context The Context in which this PendingIntent should start
 * the activity.
 * @param requestCode Private request code for the sender (currently
 * not used).
 * @param intents Array of Intents of the activities to be launched.
 * @param flags May be {@link #FLAG_ONE_SHOT}, {@link #FLAG_NO_CREATE},
 * {@link #FLAG_CANCEL_CURRENT}, {@link #FLAG_UPDATE_CURRENT},
 * or any of the flags as supported by
 * {@link Intent#fillIn Intent.fillIn()} to control which unspecified parts
 * of the intent that can be supplied when the actual send happens.
 *
 * @return Returns an existing or new PendingIntent matching the given
 * parameters.  May return null only if {@link #FLAG_NO_CREATE} has been
 * supplied.
 */
ECode CPendingIntentHelper::GetActivities(
    /* [in] */ IContext *context,
    /* [in] */ Int32 requestCode,
    /* [in] */ ArrayOf<IIntent *> *intents,
    /* [in] */ Int32 flags,
    /* [out] */ IPendingIntent **pendingIntent)
{
    return GetActivities(context, requestCode, intents, flags, NULL, pendingIntent);
}

/**
 * Like {@link #getActivity(Context, int, Intent, int)}, but allows an
 * array of Intents to be supplied.  The first Intent in the array is
 * taken as the primary key for the PendingIntent, like the single Intent
 * given to {@link #getActivity(Context, int, Intent, int)}.  Upon sending
 * the resulting PendingIntent, all of the Intents are started in the same
 * way as they would be by passing them to {@link Context#startActivities(Intent[])}.
 *
 * <p class="note">
 * The <em>first</em> intent in the array will be started outside of the context of an
 * existing activity, so you must use the {@link Intent#FLAG_ACTIVITY_NEW_TASK
 * Intent.FLAG_ACTIVITY_NEW_TASK} launch flag in the Intent.  (Activities after
 * the first in the array are started in the context of the previous activity
 * in the array, so FLAG_ACTIVITY_NEW_TASK is not needed nor desired for them.)
 * </p>
 *
 * <p class="note">
 * The <em>last</em> intent in the array represents the key for the
 * PendingIntent.  In other words, it is the significant element for matching
 * (as done with the single intent given to {@link #getActivity(Context, int, Intent, int)},
 * its content will be the subject of replacement by
 * {@link #send(Context, int, Intent)} and {@link #FLAG_UPDATE_CURRENT}, etc.
 * This is because it is the most specific of the supplied intents, and the
 * UI the user actually sees when the intents are started.
 * </p>
 *
 * @param context The Context in which this PendingIntent should start
 * the activity.
 * @param requestCode Private request code for the sender (currently
 * not used).
 * @param intents Array of Intents of the activities to be launched.
 * @param flags May be {@link #FLAG_ONE_SHOT}, {@link #FLAG_NO_CREATE},
 * {@link #FLAG_CANCEL_CURRENT}, {@link #FLAG_UPDATE_CURRENT},
 * or any of the flags as supported by
 * {@link Intent#fillIn Intent.fillIn()} to control which unspecified parts
 * of the intent that can be supplied when the actual send happens.
 *
 * @return Returns an existing or new PendingIntent matching the given
 * parameters.  May return null only if {@link #FLAG_NO_CREATE} has been
 * supplied.
 */
ECode CPendingIntentHelper::GetActivities(
    /* [in] */ IContext *context,
    /* [in] */ Int32 requestCode,
    /* [in] */ ArrayOf<IIntent *> *intents,
    /* [in] */ Int32 flags,
    /* [in] */ IBundle *options,
    /* [out] */ IPendingIntent **pendingIntent)
{
    VALIDATE_NOT_NULL(pendingIntent);

    String packageName;
    context->GetPackageName(&packageName);
    AutoPtr<IContentResolver> cr;
    context->GetContentResolver((IContentResolver**)&cr);
    AutoPtr<ArrayOf<String> > resolvedTypes = ArrayOf<String>::Alloc(intents->GetLength());
    for (Int32 i = 0; i < intents->GetLength(); i++) {
        (*intents)[i]->MigrateExtraStreamToClipData();
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

/**
* @hide
* Note that UserHandle.CURRENT will be interpreted at the time the
* activity is started, not when the pending intent is created.
*/
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

    String packageName;
    context->GetPackageName(&packageName);
    AutoPtr<IContentResolver> cr;
    context->GetContentResolver((IContentResolver**)&cr);
    AutoPtr<ArrayOf<String> > resolvedTypes = ArrayOf<String>::Alloc(intents->GetLength());
    for (Int32 i = 0; i < intents->GetLength(); i++) {
        (*intents)[i]->MigrateExtraStreamToClipData();
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

/**
 * Retrieve a PendingIntent that will perform a broadcast, like calling
 * {@link Context#sendBroadcast(Intent) Context.sendBroadcast()}.
 *
 * @param context The Context in which this PendingIntent should perform
 * the broadcast.
 * @param requestCode Private request code for the sender (currently
 * not used).
 * @param intent The Intent to be broadcast.
 * @param flags May be {@link #FLAG_ONE_SHOT}, {@link #FLAG_NO_CREATE},
 * {@link #FLAG_CANCEL_CURRENT}, {@link #FLAG_UPDATE_CURRENT},
 * or any of the flags as supported by
 * {@link Intent#fillIn Intent.fillIn()} to control which unspecified parts
 * of the intent that can be supplied when the actual send happens.
 *
 * @return Returns an existing or new PendingIntent matching the given
 * parameters.  May return null only if {@link #FLAG_NO_CREATE} has been
 * supplied.
 */
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

/**
 * @hide
 * Note that UserHandle.CURRENT will be interpreted at the time the
 * broadcast is sent, not when the pending intent is created.
 */
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

/**
 * Retrieve a PendingIntent that will start a service, like calling
 * {@link Context#startService Context.startService()}.  The start
 * arguments given to the service will come from the extras of the Intent.
 *
 * @param context The Context in which this PendingIntent should start
 * the service.
 * @param requestCode Private request code for the sender (currently
 * not used).
 * @param intent An Intent describing the service to be started.
 * @param flags May be {@link #FLAG_ONE_SHOT}, {@link #FLAG_NO_CREATE},
 * {@link #FLAG_CANCEL_CURRENT}, {@link #FLAG_UPDATE_CURRENT},
 * or any of the flags as supported by
 * {@link Intent#fillIn Intent.fillIn()} to control which unspecified parts
 * of the intent that can be supplied when the actual send happens.
 *
 * @return Returns an existing or new PendingIntent matching the given
 * parameters.  May return null only if {@link #FLAG_NO_CREATE} has been
 * supplied.
 */
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

/**
 * Convenience function for writing either a PendingIntent or null pointer to
 * a Parcel.  You must use this with {@link #readPendingIntentOrNullFromParcel}
 * for later reading it.
 *
 * @param sender The PendingIntent to write, or null.
 * @param out Where to write the PendingIntent.
 */
ECode CPendingIntentHelper::WritePendingIntentOrNullToParcel(
    /* [in] */ IPendingIntent *sender,
    /* [in] */ IParcel *out)
{
    assert(0);
    // out.writeStrongBinder(sender != null ? sender.mTarget.asBinder()
    //         : null);
    return E_NOT_IMPLEMENTED;
}

/**
 * Convenience function for reading either a Messenger or null pointer from
 * a Parcel.  You must have previously written the Messenger with
 * {@link #writePendingIntentOrNullToParcel}.
 *
 * @param in The Parcel containing the written Messenger.
 *
 * @return Returns the Messenger read from the Parcel, or null if null had
 * been written.
 */
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
