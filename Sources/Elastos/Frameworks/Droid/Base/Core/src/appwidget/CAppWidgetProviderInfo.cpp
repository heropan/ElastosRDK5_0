
#include "ext/frameworkdef.h"
#include "appwidget/CAppWidgetProviderInfo.h"
#include "content/CComponentName.h"

using Elastos::Droid::Content::CComponentName;

namespace Elastos {
namespace Droid {
namespace AppWidget {

CAppWidgetProviderInfo::CAppWidgetProviderInfo()
    : mMinWidth(0)
    , mMinHeight(0)
    , mMinResizeWidth(0)
    , mMinResizeHeight(0)
    , mUpdatePeriodMillis(0)
    , mInitialLayout(0)
    , mInitialKeyguardLayout(0)
    , mIcon(0)
    , mAutoAdvanceViewId(0)
    , mPreviewImage(0)
    , mResizeMode(0)
    , mWidgetCategory(0)
{}

ECode CAppWidgetProviderInfo::constructor()
{
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetProvider(
    /* [in] */ IComponentName* name)
{
    mProvider = name;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetProvider(
    /* [out] */ IComponentName** name)
{
    VALIDATE_NOT_NULL(name);
    *name = mProvider;
    REFCOUNT_ADD(*name);
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetMinWidth(
    /* [in] */ Int32 width)
{
    mMinWidth = width;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetMinWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = mMinWidth;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetMinHeight(
    /* [in] */ Int32 height)
{
    mMinHeight = height;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetMinHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = mMinHeight;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetMinResizeWidth(
    /* [in] */ Int32 width)
{
    mMinResizeWidth = width;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetMinResizeWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = mMinResizeWidth;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetMinResizeHeight(
    /* [in] */ Int32 height)
{
    mMinResizeHeight = height;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetMinResizeHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = mMinResizeHeight;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetUpdatePeriodMillis(
    /* [in] */ Int32 period)
{
    mUpdatePeriodMillis = period;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetUpdatePeriodMillis(
    /* [out] */ Int32* period)
{
    VALIDATE_NOT_NULL(period);
    *period = mUpdatePeriodMillis;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetInitialLayout(
    /* [in] */ Int32 layout)
{
    mInitialLayout = layout;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetInitialLayout(
    /* [out] */ Int32* layout)
{
    VALIDATE_NOT_NULL(layout);
    *layout = mInitialLayout;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetInitialKeyguardLayout(
    /* [in] */ Int32 layout)
{
    mInitialKeyguardLayout = layout;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetInitialKeyguardLayout(
    /* [out] */ Int32* layout)
{
    VALIDATE_NOT_NULL(layout);
    *layout = mInitialKeyguardLayout;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetConfigure(
    /* [in] */ IComponentName* name)
{
    mConfigure = name;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetConfigure(
    /* [out] */ IComponentName** name)
{
    VALIDATE_NOT_NULL(name);
    *name = mConfigure;
    REFCOUNT_ADD(*name);
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetLabel(
    /* [in] */ const String& label)
{
    mLabel = label;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetLabel(
    /* [out] */ String* label)
{
    VALIDATE_NOT_NULL(label);
    *label = mLabel;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetIcon(
    /* [in] */ Int32 icon)
{
    mIcon = icon;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    *icon = mIcon;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetAutoAdvanceViewId(
    /* [in] */ Int32 id)
{
    mAutoAdvanceViewId = id;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetAutoAdvanceViewId(
    /* [out]*/ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mAutoAdvanceViewId;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetPreviewImage(
    /* [in] */ Int32 image)
{
    mPreviewImage = image;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetPreviewImage(
    /* [out]*/ Int32* image)
{
    VALIDATE_NOT_NULL(image);
    *image = mPreviewImage;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetResizeMode(
    /* [in] */ Int32 mode)
{
    mResizeMode = mode;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetResizeMode(
    /* [out]*/ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = mResizeMode;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::SetWidgetCategory(
    /* [in] */ Int32 category)
{
    mWidgetCategory = category;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::GetWidgetCategory(
    /* [out]*/ Int32* category)
{
    VALIDATE_NOT_NULL(category);
    *category = mWidgetCategory;
    return NOERROR;
}

ECode CAppWidgetProviderInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadInterfacePtr((Handle32*)&mProvider);
    source->ReadInt32(&mMinWidth);
    source->ReadInt32(&mMinHeight);
    source->ReadInt32(&mMinResizeWidth);
    source->ReadInt32(&mMinResizeHeight);
    source->ReadInt32(&mUpdatePeriodMillis);
    source->ReadInt32(&mInitialLayout);
    source->ReadInt32(&mInitialKeyguardLayout);
    source->ReadInterfacePtr((Handle32*)&mConfigure);
    source->ReadString(&mLabel);
    source->ReadInt32(&mIcon);
    source->ReadInt32(&mPreviewImage);
    source->ReadInt32(&mAutoAdvanceViewId);
    source->ReadInt32(&mResizeMode);
    source->ReadInt32(&mWidgetCategory);
    return NOERROR;
}

ECode CAppWidgetProviderInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInterfacePtr(mProvider);
    dest->WriteInt32(mMinWidth);
    dest->WriteInt32(mMinHeight);
    dest->WriteInt32(mMinResizeWidth);
    dest->WriteInt32(mMinResizeHeight);
    dest->WriteInt32(mUpdatePeriodMillis);
    dest->WriteInt32(mInitialLayout);
    dest->WriteInt32(mInitialKeyguardLayout);
    dest->WriteInterfacePtr(mConfigure);
    dest->WriteString(mLabel);
    dest->WriteInt32(mIcon);
    dest->WriteInt32(mPreviewImage);
    dest->WriteInt32(mAutoAdvanceViewId);
    dest->WriteInt32(mResizeMode);
    dest->WriteInt32(mWidgetCategory);
    return NOERROR;
}

ECode CAppWidgetProviderInfo::Clone(
    /* [out] */ IAppWidgetProviderInfo** info)
{
    VALIDATE_NOT_NULL(info);

    AutoPtr<IAppWidgetProviderInfo> that;
    CAppWidgetProviderInfo::New((IAppWidgetProviderInfo**)&that);
    if (mProvider != NULL) {
        AutoPtr<IComponentName> provider;
        mProvider->Clone((IComponentName**)&provider);
        that->SetProvider(provider);
    }
    that->SetMinWidth(mMinWidth);
    that->SetMinHeight(mMinHeight);
    that->SetMinResizeWidth(mMinResizeWidth);
    that->SetMinResizeHeight(mMinResizeHeight);
    that->SetUpdatePeriodMillis(mUpdatePeriodMillis);
    that->SetInitialLayout(mInitialLayout);
    that->SetInitialKeyguardLayout(mInitialKeyguardLayout);
    if (mConfigure != NULL) {
        AutoPtr<IComponentName> configure;
        mConfigure->Clone((IComponentName**)&configure);
        that->SetConfigure(configure);
    }
    // that.label = this.label == null ? null : this.label.substring(0);
    that->SetLabel(mLabel);
    that->SetIcon(mIcon);
    that->SetPreviewImage(mPreviewImage);
    that->SetAutoAdvanceViewId(mAutoAdvanceViewId);
    that->SetResizeMode(mResizeMode);
    that->SetWidgetCategory(mWidgetCategory);
    *info = that;
    REFCOUNT_ADD(*info);
    return NOERROR;
}

} // namespace AppWidget
} // namespace Droid
} // namespace Elastos
