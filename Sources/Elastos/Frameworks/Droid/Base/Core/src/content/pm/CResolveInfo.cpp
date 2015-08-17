
#include "ext/frameworkext.h"
#include "content/pm/CResolveInfo.h"

using Elastos::Core::CString;

#include <elastos/utility/logging/Slogger.h>
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CResolveInfo::CResolveInfo()
    : mPriority(0)
    , mPreferredOrder(0)
    , mMatch(0)
    , mSpecificIndex(-1)
    , mIsDefault(FALSE)
    , mLabelRes(0)
    , mIcon(0)
    , mSystem(FALSE)
{}

CResolveInfo::~CResolveInfo()
{
}

ECode CResolveInfo::constructor()
{
    return NOERROR;
}

ECode CResolveInfo::constructor(
    /* [in] */ IResolveInfo* orig)
{
    VALIDATE_NOT_NULL(orig);
    orig->GetActivityInfo((IActivityInfo**)&mActivityInfo);
    orig->GetServiceInfo((IServiceInfo**)&mServiceInfo);
    orig->GetFilter((IIntentFilter**)&mFilter);
    orig->GetPriority(&mPriority);
    orig->GetPreferredOrder(&mPreferredOrder);
    orig->GetMatch(&mMatch);
    orig->GetSpecificIndex(&mSpecificIndex);
    orig->GetIsDefault(&mIsDefault);
    orig->GetLabelRes(&mLabelRes);
    orig->GetNonLocalizedLabel((ICharSequence**)&mNonLocalizedLabel);
    orig->GetIcon(&mIcon);
    orig->GetResolvePackageName(&mResolvePackageName);
    orig->GetSystem(&mSystem);
    return NOERROR;
}

ECode CResolveInfo::LoadLabel(
    /* [in] */ IPackageManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    *label = NULL;

    if (mNonLocalizedLabel != NULL) {
        *label = mNonLocalizedLabel;
        REFCOUNT_ADD(*label);
        return NOERROR;
    }

    if (!mResolvePackageName.IsNull() && mLabelRes != 0) {
        pm->GetText(mResolvePackageName, mLabelRes, NULL, label);
        if (*label != NULL) {
            String str;
            (*label)->ToString(&str);
            *label = NULL;
            return CString::New(str.Trim(), label);
        }
    }

    AutoPtr<IComponentInfo> ci = mActivityInfo != NULL
            ? (IComponentInfo*)mActivityInfo.Get() : (IComponentInfo*)mServiceInfo.Get();
    AutoPtr<IApplicationInfo> ai;
    ci->GetApplicationInfo((IApplicationInfo**)&ai);
    if (mLabelRes != 0) {
        String packageName;
        ci->GetPackageName(&packageName);
        pm->GetText(packageName, mLabelRes, ai, label);
        if (*label != NULL) {
            String str;
            (*label)->ToString(&str);
            *label = NULL;
            return CString::New(str.Trim(), label);
        }
    }

    AutoPtr<ICharSequence> data;
    ci->LoadLabel(pm, (ICharSequence**)&data);
    // Make the data safe
    if (data != NULL) {
        String str;
        data->ToString(&str);
        CString::New(str.Trim(), label);
    }

    return NOERROR;
}

ECode CResolveInfo::LoadIcon(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    *icon = NULL;

    if (!mResolvePackageName.IsNull() && mIcon != 0) {
        pm->GetDrawable(mResolvePackageName, mIcon, NULL, icon);
        if (*icon != NULL) {
            return NOERROR;
        }
    }

    AutoPtr<IComponentInfo> ci = mActivityInfo != NULL
            ? (IComponentInfo*)mActivityInfo.Get() : (IComponentInfo*)mServiceInfo.Get();
    AutoPtr<IApplicationInfo> ai;
    ci->GetApplicationInfo((IApplicationInfo**)&ai);
    if (mIcon != 0) {
        String packageName;
        ci->GetPackageName(&packageName);
        pm->GetDrawable(packageName, mIcon, ai, icon);
        if (*icon != NULL) {
            return NOERROR;
        }
    }
    return ci->LoadIcon(pm, icon);
}

ECode CResolveInfo::GetIconResource(
    /* [out] */ Int32* iconRes)
{
    VALIDATE_NOT_NULL(iconRes);

    if (mIcon != 0) {
        *iconRes = mIcon;
        return NOERROR;
    }
    if (mActivityInfo != NULL) {
        return mActivityInfo->GetIconResource(iconRes);
    }
    if (mServiceInfo != NULL) {
        return mServiceInfo->GetIconResource(iconRes);
    }
    *iconRes = 0;
    return NOERROR;
}

ECode CResolveInfo::Dump(
    /* [in] */ IPrinter* pw,
    /* [in] */ const String& prefix)
{
    // if (filter != null) {
    //     pw.println(prefix + "Filter:");
    //     filter.dump(pw, prefix + "  ");
    // }
    // pw.println(prefix + "priority=" + priority
    //         + " preferredOrder=" + preferredOrder
    //         + " match=0x" + Integer.toHexString(match)
    //         + " specificIndex=" + specificIndex
    //         + " isDefault=" + isDefault);
    // if (resolvePackageName != null) {
    //     pw.println(prefix + "resolvePackageName=" + resolvePackageName);
    // }
    // if (labelRes != 0 || nonLocalizedLabel != null || icon != 0) {
    //     pw.println(prefix + "labelRes=0x" + Integer.toHexString(labelRes)
    //             + " nonLocalizedLabel=" + nonLocalizedLabel
    //             + " icon=0x" + Integer.toHexString(icon));
    // }
    // if (activityInfo != null) {
    //     pw.println(prefix + "ActivityInfo:");
    //     activityInfo.dump(pw, prefix + "  ");
    // } else if (serviceInfo != null) {
    //     pw.println(prefix + "ServiceInfo:");
    //     serviceInfo.dump(pw, prefix + "  ");
    // }
    // assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CResolveInfo::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = String("CResolveInfo{");
    IComponentInfo* ci = IComponentInfo::Probe(mActivityInfo.Get());
    if (ci == NULL) {
        IComponentInfo::Probe(mServiceInfo.Get());
    } ;
    String name;
    if (ci) {
        ci->GetName(&name);
    }

    (*str).AppendFormat("%p %s p=%d o=%d m=0x%08x}",
        this, name.string(), mPriority, mPreferredOrder, mMatch);
    return NOERROR;
}

ECode CResolveInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadInterfacePtr((Handle32*)&mActivityInfo);
    source->ReadInterfacePtr((Handle32*)&mServiceInfo);
    source->ReadInterfacePtr((Handle32*)&mFilter);
    source->ReadInt32(&mPriority);
    source->ReadInt32(&mPreferredOrder);
    source->ReadInt32(&mMatch);
    source->ReadInt32(&mSpecificIndex);
    source->ReadBoolean(&mIsDefault);
    source->ReadInt32(&mLabelRes);
    source->ReadInterfacePtr((Handle32*)&mNonLocalizedLabel);
    source->ReadInt32(&mIcon);
    source->ReadString(&mResolvePackageName);
    source->ReadBoolean(&mSystem);
    return NOERROR;
}

ECode CResolveInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInterfacePtr(mActivityInfo);
    dest->WriteInterfacePtr(mServiceInfo);
    dest->WriteInterfacePtr(mFilter);
    dest->WriteInt32(mPriority);
    dest->WriteInt32(mPreferredOrder);
    dest->WriteInt32(mMatch);
    dest->WriteInt32(mSpecificIndex);
    dest->WriteBoolean(mIsDefault);
    dest->WriteInt32(mLabelRes);
    dest->WriteInterfacePtr(mNonLocalizedLabel);
    dest->WriteInt32(mIcon);
    dest->WriteString(mResolvePackageName);
    dest->WriteBoolean(mSystem);
    return NOERROR;
}

ECode CResolveInfo::GetActivityInfo(
    /* [out] */ IActivityInfo** activityInfo)
{
    VALIDATE_NOT_NULL(activityInfo);
    *activityInfo = mActivityInfo;
    REFCOUNT_ADD(*activityInfo);
    return NOERROR;
}

ECode CResolveInfo::SetActivityInfo(
    /* [in] */ IActivityInfo* activityInfo)
{
    mActivityInfo = activityInfo;
    return NOERROR;
}

ECode CResolveInfo::GetServiceInfo(
    /* [out] */ IServiceInfo** serviceInfo)
{
    VALIDATE_NOT_NULL(serviceInfo);
    *serviceInfo = mServiceInfo;
    REFCOUNT_ADD(*serviceInfo);
    return NOERROR;
}

ECode CResolveInfo::SetServiceInfo(
    /* [in] */ IServiceInfo* serviceInfo)
{
    mServiceInfo = serviceInfo;
    return NOERROR;
}

ECode CResolveInfo::GetFilter(
    /* [out] */ IIntentFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    *filter = mFilter;
    REFCOUNT_ADD(*filter);
    return NOERROR;
}

ECode CResolveInfo::SetFilter(
    /* [in] */ IIntentFilter* filter)
{
    mFilter = filter;
    return NOERROR;
}

ECode CResolveInfo::GetPriority(
    /* [out] */ Int32* priority)
{
    VALIDATE_NOT_NULL(priority);
    *priority = mPriority;
    return NOERROR;
}

ECode CResolveInfo::SetPriority(
    /* [in] */ Int32 priority)
{
    mPriority = priority;
    return NOERROR;
}

ECode CResolveInfo::GetPreferredOrder(
    /* [out] */ Int32* preferredOrder)
{
    VALIDATE_NOT_NULL(preferredOrder);
    *preferredOrder = mPreferredOrder;
    return NOERROR;
}

ECode CResolveInfo::SetPreferredOrder(
    /* [in] */ Int32 preferredOrder)
{
    mPreferredOrder = preferredOrder;
    return NOERROR;
}

ECode CResolveInfo::GetMatch(
    /* [out] */ Int32* match)
{
    VALIDATE_NOT_NULL(match);
    *match = mMatch;
    return NOERROR;
}

ECode CResolveInfo::SetMatch(
    /* [in] */ Int32 match)
{
    mMatch = match;
    return NOERROR;
}

ECode CResolveInfo::GetSpecificIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = mSpecificIndex;
    return NOERROR;
}

ECode CResolveInfo::SetSpecificIndex(
    /* [in] */ Int32 index)
{
    mSpecificIndex = index;
    return NOERROR;
}

ECode CResolveInfo::GetIsDefault(
    /* [out] */ Boolean* isDefault)
{
    VALIDATE_NOT_NULL(isDefault);
    *isDefault = mIsDefault;
    return NOERROR;
}

ECode CResolveInfo::SetIsDefault(
    /* [in] */ Boolean isDefault)
{
    mIsDefault = isDefault;
    return NOERROR;
}

ECode CResolveInfo::GetLabelRes(
    /* [out] */ Int32* labelRes)
{
    VALIDATE_NOT_NULL(labelRes);
    *labelRes = mLabelRes;
    return NOERROR;
}

ECode CResolveInfo::SetLabelRes(
    /* [in] */ Int32 labelRes)
{
    mLabelRes = labelRes;
    return NOERROR;
}

ECode CResolveInfo::GetNonLocalizedLabel(
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    *label = mNonLocalizedLabel;
    REFCOUNT_ADD(*label);
    return NOERROR;
}

ECode CResolveInfo::SetNonLocalizedLabel(
    /* [in] */ ICharSequence* label)
{
    mNonLocalizedLabel = label;
    return NOERROR;
}

ECode CResolveInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    *icon = mIcon;
    return NOERROR;
}

ECode CResolveInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    mIcon = icon;
    return NOERROR;
}

ECode CResolveInfo::GetResolvePackageName(
    /* [out] */ String* resolvePackageName)
{
    VALIDATE_NOT_NULL(resolvePackageName);
    *resolvePackageName = mResolvePackageName;
    return NOERROR;
}

ECode CResolveInfo::SetResolvePackageName(
    /* [in] */ const String& resolvePackageName)
{
    mResolvePackageName = resolvePackageName;
    return NOERROR;
}

ECode CResolveInfo::GetSystem(
    /* [out] */ Boolean* sys)
{
    VALIDATE_NOT_NULL(sys);
    *sys = mSystem;
    return NOERROR;
}

ECode CResolveInfo::SetSystem(
    /* [in] */ Boolean sys)
{
    mSystem = sys;
    return NOERROR;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

