
#include "ext/frameworkext.h"
#include "content/pm/CProviderInfo.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CProviderInfo::CProviderInfo()
    : mGrantUriPermissions(FALSE)
    , mMultiprocess(FALSE)
    , mInitOrder(0)
    , mFlags(0)
    , mIsSyncable(FALSE)
{}

CProviderInfo::~CProviderInfo()
{}

ECode CProviderInfo::constructor()
{
    return ComponentInfo::constructor();
}

ECode CProviderInfo::constructor(
    /* [in] */ IProviderInfo* orig)
{
    assert(orig != NULL);
    ComponentInfo::constructor((IComponentInfo*)orig);
    orig->GetAuthority(&mAuthority);
    orig->GetReadPermission(&mReadPermission);
    orig->GetWritePermission(&mWritePermission);
    orig->GetGrantUriPermissions(&mGrantUriPermissions);
    orig->GetUriPermissionPatterns((ArrayOf<IPatternMatcher*>**)&mUriPermissionPatterns);
    orig->GetPathPermissions((ArrayOf<IPathPermission*>**)&mPathPermissions);
    orig->GetMultiprocess(&mMultiprocess);
    orig->GetInitOrder(&mInitOrder);
    orig->GetFlags(&mFlags);
    orig->GetIsSyncable(&mIsSyncable);
    return NOERROR;
}

ECode CProviderInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    ComponentInfo::ReadFromParcel(source);
    source->ReadString(&mAuthority);
    source->ReadString(&mReadPermission);
    source->ReadString(&mWritePermission);
    source->ReadBoolean(&mGrantUriPermissions);
    source->ReadArrayOf((Handle32*)&mUriPermissionPatterns);
    source->ReadArrayOf((Handle32*)&mPathPermissions);
    source->ReadBoolean(&mMultiprocess);
    source->ReadInt32(&mInitOrder);
    source->ReadInt32(&mFlags);
    source->ReadBoolean(&mIsSyncable);
    return NOERROR;
}

ECode CProviderInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    ComponentInfo::WriteToParcel(dest);
    dest->WriteString(mAuthority);
    dest->WriteString(mReadPermission);
    dest->WriteString(mWritePermission);
    dest->WriteBoolean(mGrantUriPermissions);
    dest->WriteArrayOf((Handle32)mUriPermissionPatterns.Get());
    dest->WriteArrayOf((Handle32)mPathPermissions.Get());
    dest->WriteBoolean(mMultiprocess);
    dest->WriteInt32(mInitOrder);
    dest->WriteInt32(mFlags);
    dest->WriteBoolean(mIsSyncable);
    return NOERROR;
}

ECode CProviderInfo::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    // return "ContentProviderInfo{name=" + authority + " className=" + name + "}";
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CProviderInfo::LoadLabel(
    /* [in] */ IPackageManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return ComponentInfo::LoadLabel(pm, label);
}

ECode CProviderInfo::LoadIcon(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return ComponentInfo::LoadIcon(pm, icon);
}

ECode CProviderInfo::LoadLogo(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** logo)
{
    VALIDATE_NOT_NULL(logo);
    return ComponentInfo::LoadLogo(pm, logo);;
}

ECode CProviderInfo::LoadXmlMetaData(
    /* [in] */ IPackageManager* pm,
    /* [in] */ const String& name,
    /* [out] */ IXmlResourceParser** resource)
{
    VALIDATE_NOT_NULL(resource);
    return ComponentInfo::LoadXmlMetaData(pm, name, resource);
}

ECode CProviderInfo::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return ComponentInfo::GetName(name);
}

ECode CProviderInfo::SetName(
    /* [in] */ const String& name)
{
    return ComponentInfo::SetName(name);
}

ECode CProviderInfo::GetPackageName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return ComponentInfo::GetPackageName(name);
}

ECode CProviderInfo::SetPackageName(
    /* [in] */ const String& name)
{
    return ComponentInfo::SetPackageName(name);
}

ECode CProviderInfo::GetLabelRes(
    /* [out] */ Int32* labelRes)
{
    VALIDATE_NOT_NULL(labelRes);
    return ComponentInfo::GetLabelRes(labelRes);
}

ECode CProviderInfo::SetLabelRes(
    /* [in] */ Int32 labelRes)
{
    return ComponentInfo::SetLabelRes(labelRes);
}

ECode CProviderInfo::GetNonLocalizedLabel(
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    return ComponentInfo::GetNonLocalizedLabel(label);
}

ECode CProviderInfo::SetNonLocalizedLabel(
    /* [in] */ ICharSequence* label)
{
    return ComponentInfo::SetNonLocalizedLabel(label);
}

ECode CProviderInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    return ComponentInfo::GetIcon(icon);
}

ECode CProviderInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    return ComponentInfo::SetIcon(icon);
}

ECode CProviderInfo::GetLogo(
    /* [out] */ Int32* logo)
{
    VALIDATE_NOT_NULL(logo);
    return ComponentInfo::GetLogo(logo);
}

ECode CProviderInfo::SetLogo(
    /* [in] */ Int32 logo)
{
    return ComponentInfo::SetLogo(logo);
}

ECode CProviderInfo::GetMetaData(
    /* [out] */ IBundle** metaData)
{
    VALIDATE_NOT_NULL(metaData);
    return ComponentInfo::GetMetaData(metaData);
}

ECode CProviderInfo::SetMetaData(
    /* [in] */ IBundle* metaData)
{
    return ComponentInfo::SetMetaData(metaData);
}

ECode CProviderInfo::IsEnabled(
    /* [out] */ Boolean* isEnable)
{
    VALIDATE_NOT_NULL(isEnable);
    return ComponentInfo::IsEnabled(isEnable);
}

ECode CProviderInfo::GetIconResource(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    return ComponentInfo::GetIconResource(icon);
}

ECode CProviderInfo::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return ComponentInfo::GetApplicationInfo(info);
}

ECode CProviderInfo::SetApplicationInfo(
    /* [in] */ IApplicationInfo* info)
{
    return ComponentInfo::SetApplicationInfo(info);
}

ECode CProviderInfo::GetProcessName(
    /* [out] */ String* processName)
{
    VALIDATE_NOT_NULL(processName);
    return ComponentInfo::GetProcessName(processName);
}

ECode CProviderInfo::SetProcessName(
    /* [in] */ const String& processName)
{
    return ComponentInfo::SetProcessName(processName);
}

ECode CProviderInfo::GetDescriptionRes(
    /* [out] */ Int32* desRes)
{
    VALIDATE_NOT_NULL(desRes);
    return ComponentInfo::GetDescriptionRes(desRes);
}

ECode CProviderInfo::SetDescriptionRes(
    /* [in] */ Int32 desRes)
{
    return ComponentInfo::SetDescriptionRes(desRes);
}

ECode CProviderInfo::GetEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    return ComponentInfo::GetEnabled(enabled);
}

ECode CProviderInfo::SetEnabled(
    /* [in] */ Boolean enabled)
{
    return ComponentInfo::SetEnabled(enabled);
}

ECode CProviderInfo::GetExported(
    /* [out] */ Boolean* exported)
{
    VALIDATE_NOT_NULL(exported);
    return ComponentInfo::GetExported(exported);
}

ECode CProviderInfo::SetExported(
    /* [in] */ Boolean exported)
{
    return ComponentInfo::SetExported(exported);
}

ECode CProviderInfo::GetAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority);
    *authority = mAuthority;
    return NOERROR;
}

ECode CProviderInfo::SetAuthority(
    /* [in] */ const String& authority)
{
    mAuthority = authority;
    return NOERROR;
}

ECode CProviderInfo::GetReadPermission(
    /* [out] */ String* readPermission)
{
    VALIDATE_NOT_NULL(readPermission);
    *readPermission = mReadPermission;
    return NOERROR;
}

ECode CProviderInfo::SetReadPermission(
    /* [in] */ const String& readPermission)
{
    mReadPermission = readPermission;
    return NOERROR;
}

ECode CProviderInfo::GetWritePermission(
    /* [out] */ String* writePermission)
{
    VALIDATE_NOT_NULL(writePermission);
    *writePermission = mWritePermission;
    return NOERROR;
}

ECode CProviderInfo::SetWritePermission(
    /* [in] */ const String& writePermission)
{
    mWritePermission = writePermission;
    return NOERROR;
}

ECode CProviderInfo::GetGrantUriPermissions(
    /* [out] */ Boolean* grantUriPermissions)
{
    VALIDATE_NOT_NULL(grantUriPermissions);
    *grantUriPermissions = mGrantUriPermissions;
    return NOERROR;
}

ECode CProviderInfo::SetGrantUriPermissions(
    /* [in] */ Boolean grantUriPermissions)
{
    mGrantUriPermissions = grantUriPermissions;
    return NOERROR;
}

ECode CProviderInfo::GetUriPermissionPatterns(
    /* [out, callee] */ ArrayOf<IPatternMatcher*>** patterns)
{
    VALIDATE_NOT_NULL(patterns);
    *patterns = mUriPermissionPatterns;
    INTERFACE_ADDREF(*patterns);
    return NOERROR;
}

ECode CProviderInfo::SetUriPermissionPatterns(
    /* [in] */ ArrayOf<IPatternMatcher*>* patterns)
{
    mUriPermissionPatterns = patterns;
    return NOERROR;
}

ECode CProviderInfo::GetPathPermissions(
    /* [out, callee] */ ArrayOf<IPathPermission*>** pathPermissions)
{
    VALIDATE_NOT_NULL(pathPermissions);
    *pathPermissions = mPathPermissions;
    INTERFACE_ADDREF(*pathPermissions);
    return NOERROR;
}

ECode CProviderInfo::SetPathPermissions(
    /* [in] */ ArrayOf<IPathPermission*>* pathPermissions)
{
    mPathPermissions = pathPermissions;
    return NOERROR;
}

ECode CProviderInfo::GetMultiprocess(
    /* [out] */ Boolean* multiprocess)
{
    VALIDATE_NOT_NULL(multiprocess);
    *multiprocess = mMultiprocess;
    return NOERROR;
}

ECode CProviderInfo::SetMultiprocess(
    /* [in] */ Boolean multiprocess)
{
    mMultiprocess = multiprocess;
    return NOERROR;
}

ECode CProviderInfo::GetInitOrder(
    /* [out] */ Int32* initOrder)
{
    VALIDATE_NOT_NULL(initOrder);
    *initOrder = mInitOrder;
    return NOERROR;
}

ECode CProviderInfo::SetInitOrder(
    /* [in] */ Int32 initOrder)
{
    mInitOrder = initOrder;
    return NOERROR;
}

ECode CProviderInfo::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;
    return NOERROR;
}

ECode CProviderInfo::SetFlags(
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return NOERROR;
}

ECode CProviderInfo::GetIsSyncable(
    /* [out] */ Boolean* isSyncable)
{
    VALIDATE_NOT_NULL(isSyncable);
    *isSyncable = mIsSyncable;
    return NOERROR;
}

ECode CProviderInfo::SetIsSyncable(
    /* [in] */ Boolean isSyncable)
{
    mIsSyncable = isSyncable;
    return NOERROR;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
