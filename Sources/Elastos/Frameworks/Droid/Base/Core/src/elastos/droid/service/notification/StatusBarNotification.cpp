#include "elastos/droid/app/CNotification.h"
#include "elastos/droid/service/notification/CStatusBarNotification.h"
#include "elastos/droid/service/notification/StatusBarNotification.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Droid::App::CNotification;
using Elastos::Droid::Service::Notification::CStatusBarNotification;
using Elastos::Core::CSystem;
using Elastos::Core::ICloneable;
using Elastos::Core::ISystem;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Service {
namespace Notification {

CAR_INTERFACE_IMPL_2(StatusBarNotification, Object, IStatusBarNotification, IParcelable)

StatusBarNotification::StatusBarNotification()
    : mId(0)
    , mUid(0)
    , mInitialPid(0)
    , mPostTime(0L)
    , mScore(0)
{
}

ECode StatusBarNotification::constructor(
    /* [in] */ const String& pkg,
    /* [in] */ const String& opPkg,
    /* [in] */ Int32 id,
    /* [in] */ const String& tag,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 initialPid,
    /* [in] */ Int32 score,
    /* [in] */ INotification* notification,
    /* [in] */ IUserHandle* user)
{
    AutoPtr<ISystem> system;
    CSystem::AcquireSingleton((ISystem**)&system);
    Int64 milis;
    system->GetCurrentTimeMillis(&milis);
    return constructor(pkg, opPkg, id, tag, uid, initialPid, score, notification, user, milis);
}

ECode StatusBarNotification::constructor(
    /* [in] */ const String& pkg,
    /* [in] */ const String& opPkg,
    /* [in] */ Int32 id,
    /* [in] */ const String& tag,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 initialPid,
    /* [in] */ Int32 score,
    /* [in] */ INotification* notification,
    /* [in] */ IUserHandle* user,
    /* [in] */ Int64 postTime)
{
    if (pkg.IsNull()) return E_NULL_POINTER_EXCEPTION;
    if (notification == NULL) return E_NULL_POINTER_EXCEPTION;

    mPkg = pkg;
    mOpPkg = opPkg;
    mId = id;
    mTag = tag;
    mUid = uid;
    mInitialPid = initialPid;
    mScore = score;
    mNotification = notification;
    mUser = user;
    mPostTime = postTime;
    Key(&mKey);
    GroupKey(&mGroupKey);
    return NOERROR;
}

ECode StatusBarNotification::constructor(
    /* [in] */ IParcelable* in)
{
    // this.pkg = in.readString();
    // this.opPkg = in.readString();
    // this.id = in.readInt();
    // if (in.readInt() != 0) {
    //     this.tag = in.readString();
    // } else {
    //     this.tag = null;
    // }
    // this.uid = in.readInt();
    // this.initialPid = in.readInt();
    // this.score = in.readInt();
    // this.notification = new Notification(in);
    // this.user = UserHandle.readFromParcel(in);
    // this.postTime = in.readLong();
    // this.key = key();
    // this.groupKey = groupKey();
    //TODO
    return NOERROR;
}

ECode StatusBarNotification::Key(
    /* [out] */ String* key)
{
    VALIDATE_NOT_NULL(key)
    Int32 identifier;
    mUser->GetIdentifier(&identifier);
    StringBuilder sb(identifier);
    sb += "|";
    sb += mPkg;
    sb += "|";
    sb += mId;
    sb += "|";
    sb += mTag;
    sb += "|";
    sb += mUid;
    *key = sb.ToString();
    return NOERROR;
}

ECode StatusBarNotification::GroupKey(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<INotification> notification;
    GetNotification((INotification**)&notification);
    String group;
    notification->GetGroup(&group);
    String sortKey;
    notification->GetSortKey(&sortKey);
    if (group.IsNull() && sortKey.IsNull()) {
        // a group of one
        *result = mKey;
        return NOERROR;
    }
    Int32 identifier;
    mUser->GetIdentifier(&identifier);
    StringBuilder sb(identifier);
    sb += "|";
    sb += mPkg;
    sb += "|";
    if (group.IsNull()) {
        Int32 priority;
        notification->GetPriority(&priority);
        sb += "p:";
        sb += priority;
    }
    else {
        sb += "g:";
        sb += group;
    }
    *result = sb.ToString();
    return NOERROR;
}

ECode StatusBarNotification::ReadFromParcel(
    /* [in] */ IParcel* in)
{
    //TODO
    return NOERROR;
}

ECode StatusBarNotification::WriteToParcel(
    /* [in] */ IParcel* out)
{
    // out.writeString(this.pkg);
    // out.writeString(this.opPkg);
    // out.writeInt(this.id);
    // if (this.tag != null) {
    //     out.writeInt(1);
    //     out.writeString(this.tag);
    // } else {
    //     out.writeInt(0);
    // }
    // out.writeInt(this.uid);
    // out.writeInt(this.initialPid);
    // out.writeInt(this.score);
    // this.notification.writeToParcel(out, flags);
    // user.writeToParcel(out, flags);

    // out.writeLong(this.postTime);
    //TODO
    return NOERROR;
}

ECode StatusBarNotification::CloneLight(
    /* [out] */ IStatusBarNotification** notification)
{
    VALIDATE_NOT_NULL(notification)
    const AutoPtr<INotification> no;
    CNotification::New((INotification**)&no);
    mNotification->CloneInto(no, FALSE); // light copy
    CStatusBarNotification::New(mPkg, mOpPkg, mId, mTag, mUid, mInitialPid,
        mScore, no, mUser, mPostTime, notification);
    return NOERROR;
}

ECode StatusBarNotification::Clone(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj)
    AutoPtr<IInterface> o;
    ICloneable::Probe(mNotification)->Clone((IInterface**)&o);
    AutoPtr<IStatusBarNotification> notification;
    CStatusBarNotification::New(mPkg, mOpPkg, mId, mTag, mUid, mInitialPid,
        mScore, INotification::Probe(o), mUser.Get(), mPostTime,
        (IStatusBarNotification**)&notification);
    *obj = IInterface::Probe(notification);
    REFCOUNT_ADD(*obj)
    return NOERROR;
}

ECode StatusBarNotification::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    StringBuilder sb("StatusBarNotification(pkg=");
    sb += mPkg;
    sb += " user=";
    sb += mUser;
    sb += " id=";
    sb += mId;
    sb += " tag=";
    sb += mTag;
    sb += " score=";
    sb += mScore;
    sb += " key=";
    sb += mKey;
    sb += ": ";
    sb += mNotification;
    *result = sb.ToString();
    return NOERROR;
}

ECode StatusBarNotification::IsOngoing(
    /* [out] */ Boolean* isongoing)
{
    VALIDATE_NOT_NULL(isongoing)
    Int32 flags;
    mNotification->GetFlags(&flags);
    *isongoing = (flags & INotification::FLAG_ONGOING_EVENT) != 0;
    return NOERROR;
}

ECode StatusBarNotification::IsClearable(
    /* [out] */ Boolean* isClearable)
{
    VALIDATE_NOT_NULL(isClearable)
    Int32 flags;
    mNotification->GetFlags(&flags);
    *isClearable = ((flags & INotification::FLAG_ONGOING_EVENT) == 0
        && ((flags & INotification::FLAG_NO_CLEAR) == 0));
    return NOERROR;
}

ECode StatusBarNotification::GetUserId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    mUser->GetIdentifier(id);
    return NOERROR;
}

ECode StatusBarNotification::GetPackageName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    *name = mPkg;
    return NOERROR;
}

ECode StatusBarNotification::GetId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id)
    *id = mId;
    return NOERROR;
}

ECode StatusBarNotification::GetTag(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    *str = mTag;
    return NOERROR;
}

ECode StatusBarNotification::GetUid(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id)
    *id = mUid;
    return NOERROR;
}

ECode StatusBarNotification::GetOpPkg(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    *str = mOpPkg;
    return NOERROR;
}

ECode StatusBarNotification::GetInitialPid(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id)
    *id = mInitialPid;
    return NOERROR;
}

ECode StatusBarNotification::GetNotification(
    /* [out] */ INotification** no)
{
    VALIDATE_NOT_NULL(no)
    *no = mNotification;
    REFCOUNT_ADD(*no)
    return NOERROR;
}

ECode StatusBarNotification::GetUser(
    /* [out] */ IUserHandle** us)
{
    VALIDATE_NOT_NULL(us)
    *us = mUser;
    REFCOUNT_ADD(*us)
    return NOERROR;
}

ECode StatusBarNotification::GetPostTime(
    /* [out] */ Int64* pt)
{
    VALIDATE_NOT_NULL(pt)
    *pt = mPostTime;
    return NOERROR;
}

ECode StatusBarNotification::GetScore(
    /* [out] */ Int32* isongoing)
{
    VALIDATE_NOT_NULL(isongoing)
    *isongoing = mScore;
    return NOERROR;
}

ECode StatusBarNotification::GetKey(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    *str = mKey;
    return NOERROR;
}

ECode StatusBarNotification::GetGroupKey(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    *str = mGroupKey;
    return NOERROR;
}

} // namespace Notification
} // namespace Service
} // namepsace Droid
} // namespace Elastos
