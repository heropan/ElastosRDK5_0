
#include "ext/frameworkext.h"
#include "content/pm/CUserInfo.h"
#include "os/CUserHandle.h"

using Elastos::Droid::Os::CUserHandle;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CUserInfo::CUserInfo()
    : mId(0)
    , mSerialNumber(0)
    , mFlags(0)
    , mCreationTime(0)
    , mLastLoggedInTime(0)
    , mPartial(FALSE)
{}

CUserInfo::~CUserInfo()
{}

ECode CUserInfo::constructor()
{
    return NOERROR;
}

ECode CUserInfo::constructor(
    /* [in] */ IUserInfo* orig)
{
    assert(orig != NULL);
    orig->GetId(&mId);
    orig->GetSerialNumber(&mSerialNumber);
    orig->GetName(&mName);
    orig->GetIconPath(&mIconPath);
    orig->GetFlags(&mFlags);
    orig->GetCreationTime(&mCreationTime);
    orig->GetLastLoggedInTime(&mLastLoggedInTime);
    orig->GetPartial(&mPartial);
    return NOERROR;
}

ECode CUserInfo::constructor(
    /* [in] */ Int32 id,
    /* [in] */ const String& name,
    /* [in] */ Int32 flags)
{
    return Init(id, name, String(NULL), flags);
}

ECode CUserInfo::constructor(
    /* [in] */ Int32 id,
    /* [in] */ const String& name,
    /* [in] */ const String& iconPath,
    /* [in] */ Int32 flags)
{
    return Init(id, name, iconPath, flags);
}

ECode CUserInfo::Init(
    /* [in] */ Int32 id,
    /* [in] */ const String& name,
    /* [in] */ const String& iconPath,
    /* [in] */ Int32 flags)
{
    mId = id;
    mName = name;
    mFlags = flags;
    mIconPath = iconPath;
    return NOERROR;
}

ECode CUserInfo::IsPrimary(
    /* [out] */ Boolean* isPrimary)
{
    VALIDATE_NOT_NULL(isPrimary);
    *isPrimary = (mFlags & FLAG_PRIMARY) == FLAG_PRIMARY;
    return NOERROR;
}

ECode CUserInfo::IsAdmin(
    /* [out] */ Boolean* isAdmin)
{
    VALIDATE_NOT_NULL(isAdmin);
    *isAdmin = (mFlags & FLAG_ADMIN) == FLAG_ADMIN;
    return NOERROR;
}

ECode CUserInfo::IsGuest(
    /* [out] */ Boolean* isGuest)
{
    VALIDATE_NOT_NULL(isGuest);
    *isGuest = (mFlags & FLAG_GUEST) == FLAG_GUEST;
    return NOERROR;
}

ECode CUserInfo::GetUserHandle(
    /* [out] */ IUserHandle** userHandle)
{
    VALIDATE_NOT_NULL(userHandle);
    return CUserHandle::New(mId, userHandle);
}

ECode CUserInfo::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    // return "UserInfo{" + id + ":" + name + ":" + Integer.toHexString(flags) + "}";
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CUserInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    assert(source != NULL);
    source->ReadInt32(&mId);
    source->ReadInt32(&mSerialNumber);
    source->ReadString(&mName);
    source->ReadInt32(&mFlags);
    source->ReadInt64(&mCreationTime);
    source->ReadInt64(&mLastLoggedInTime);
    source->ReadString(&mIconPath);
    source->ReadBoolean(&mPartial);
    return NOERROR;
}

ECode CUserInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    assert(dest != NULL);
    dest->WriteInt32(mId);
    dest->WriteInt32(mSerialNumber);
    dest->WriteString(mName);
    dest->WriteInt32(mFlags);
    dest->WriteInt64(mCreationTime);
    dest->WriteInt64(mLastLoggedInTime);
    dest->WriteString(mIconPath);
    dest->WriteBoolean(mPartial);
    return NOERROR;
}

ECode CUserInfo::GetId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;
    return NOERROR;
}

ECode CUserInfo::SetId(
    /* [in] */ Int32 id)
{
    mId = id;
    return NOERROR;
}

ECode CUserInfo::GetSerialNumber(
    /* [out] */ Int32* serialNumber)
{
    VALIDATE_NOT_NULL(serialNumber);
    *serialNumber = mSerialNumber;
    return NOERROR;
}

ECode CUserInfo::SetSerialNumber(
    /* [in] */ Int32 serialNumber)
{
    mSerialNumber = serialNumber;
    return NOERROR;
}

ECode CUserInfo::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode CUserInfo::SetName(
    /* [in] */ const String& name)
{
    mName = name;
    return NOERROR;
}

ECode CUserInfo::GetIconPath(
    /* [out] */ String* iconPath)
{
    VALIDATE_NOT_NULL(iconPath);
    *iconPath = mIconPath;
    return NOERROR;
}

ECode CUserInfo::SetIconPath(
    /* [in] */ const String& iconPath)
{
    mIconPath = iconPath;
    return NOERROR;
}

ECode CUserInfo::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;
    return NOERROR;
}

ECode CUserInfo::SetFlags(
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return NOERROR;
}

ECode CUserInfo::GetCreationTime(
    /* [out] */ Int64* creationTime)
{
    VALIDATE_NOT_NULL(creationTime);
    *creationTime = mCreationTime;
    return NOERROR;
}

ECode CUserInfo::SetCreationTime(
    /* [in] */ Int64 creationTime)
{
    mCreationTime = creationTime;
    return NOERROR;
}

ECode CUserInfo::GetLastLoggedInTime(
    /* [out] */ Int64* lastLoggedInTime)
{
    VALIDATE_NOT_NULL(lastLoggedInTime);
    *lastLoggedInTime = mLastLoggedInTime;
    return NOERROR;
}

ECode CUserInfo::SetLastLoggedInTime(
    /* [in] */ Int64 lastLoggedInTime)
{
    mLastLoggedInTime = lastLoggedInTime;
    return NOERROR;
}

ECode CUserInfo::GetPartial(
    /* [out] */ Boolean* partial)
{
    VALIDATE_NOT_NULL(partial);
    *partial = mPartial;
    return NOERROR;
}

ECode CUserInfo::SetPartial(
    /* [in] */ Boolean partial)
{
    mPartial = partial;
    return NOERROR;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
