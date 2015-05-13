
#include "view/CDisplayInfo.h"
#include "ext/frameworkext.h"

using Elastos::Droid::Content::Res::ICompatibilityInfo;

namespace Elastos {
namespace Droid {
namespace View {

CDisplayInfo::CDisplayInfo()
    : mLayerStack(0)
    , mFlags(0)
    , mType(0)
    , mAppWidth(0)
    , mAppHeight(0)
    , mSmallestNominalAppWidth(0)
    , mSmallestNominalAppHeight(0)
    , mLargestNominalAppWidth(0)
    , mLargestNominalAppHeight(0)
    , mLogicalWidth(0)
    , mLogicalHeight(0)
    , mRotation(0)
    , mRefreshRate(0.0f)
    , mLogicalDensityDpi(0)
    , mPhysicalXDpi(0.0f)
    , mPhysicalYDpi(0.0f)
{
}

ECode CDisplayInfo::constructor()
{
    return NOERROR;
}

ECode CDisplayInfo::constructor(
    /* [in] */ IDisplayInfo* other)
{
    return CopyFrom(other);
}

ECode CDisplayInfo::GetLayerStack(
    /* [out] */ Int32* layerStack)
{
    VALIDATE_NOT_NULL(layerStack);
    *layerStack = mLayerStack;
    return NOERROR;
}

ECode CDisplayInfo::SetLayerStack(
    /* [in] */ Int32 layerStack)
{
    mLayerStack = layerStack;
    return NOERROR;
}

ECode CDisplayInfo::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;
    return NOERROR;
}

ECode CDisplayInfo::SetFlags(
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return NOERROR;
}

ECode CDisplayInfo::GetType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mType;
    return NOERROR;
}

ECode CDisplayInfo::SetType(
    /* [in] */ Int32 type)
{
    mType = type;
    return NOERROR;
}

ECode CDisplayInfo::GetAddress(
    /* [out] */ String* address)
{
    VALIDATE_NOT_NULL(address);
    *address = mAddress;
    return NOERROR;
}

ECode CDisplayInfo::SetAddress(
    /* [in] */ const String& address)
{
    mAddress = address;
    return NOERROR;
}

ECode CDisplayInfo::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode CDisplayInfo::SetName(
    /* [in] */ const String& name)
{
    mName = name;
    return NOERROR;
}

ECode CDisplayInfo::GetAppWidth(
    /* [out] */ Int32* appWidth)
{
    VALIDATE_NOT_NULL(appWidth);
    *appWidth = mAppWidth;
    return NOERROR;
}

ECode CDisplayInfo::SetAppWidth(
    /* [in] */ Int32 appWidth)
{
    mAppWidth = appWidth;
    return NOERROR;
}

ECode CDisplayInfo::GetAppHeight(
    /* [out] */ Int32* appHeight)
{
    VALIDATE_NOT_NULL(appHeight);
    *appHeight = mAppHeight;
    return NOERROR;
}

ECode CDisplayInfo::SetAppHeight(
    /* [in] */ Int32 appHeight)
{
    mAppHeight = appHeight;
    return NOERROR;
}

ECode CDisplayInfo::GetSmallestNominalAppWidth(
    /* [out] */ Int32* smallestNominalAppWidth)
{
    VALIDATE_NOT_NULL(smallestNominalAppWidth);
    *smallestNominalAppWidth = mSmallestNominalAppWidth;
    return NOERROR;
}

ECode CDisplayInfo::SetSmallestNominalAppWidth(
    /* [in] */ Int32 smallestNominalAppWidth)
{
    mSmallestNominalAppWidth = smallestNominalAppWidth;
    return NOERROR;
}

ECode CDisplayInfo::GetSmallestNominalAppHeight(
    /* [out] */ Int32* smallestNominalAppHeight)
{
    VALIDATE_NOT_NULL(smallestNominalAppHeight);
    *smallestNominalAppHeight = mSmallestNominalAppHeight;
    return NOERROR;
}

ECode CDisplayInfo::SetSmallestNominalAppHeight(
    /* [in] */ Int32 smallestNominalAppHeight)
{
    mSmallestNominalAppHeight = smallestNominalAppHeight;
    return NOERROR;
}

ECode CDisplayInfo::GetLargestNominalAppWidth(
    /* [out] */ Int32* largestNominalAppWidth)
{
    VALIDATE_NOT_NULL(largestNominalAppWidth);
    *largestNominalAppWidth = mLargestNominalAppWidth;
    return NOERROR;
}

ECode CDisplayInfo::SetLargestNominalAppWidth(
    /* [in] */ Int32 largestNominalAppWidth)
{
    mLargestNominalAppWidth = largestNominalAppWidth;
    return NOERROR;
}

ECode CDisplayInfo::GetLargestNominalAppHeight(
    /* [out] */ Int32* largestNominalAppHeight)
{
    VALIDATE_NOT_NULL(largestNominalAppHeight);
    *largestNominalAppHeight = mLargestNominalAppHeight;
    return NOERROR;
}

ECode CDisplayInfo::SetLargestNominalAppHeight(
    /* [in] */ Int32 largestNominalAppHeight)
{
    mLargestNominalAppHeight = largestNominalAppHeight;
    return NOERROR;
}

ECode CDisplayInfo::GetLogicalWidth(
    /* [out] */ Int32* logicalWidth)
{
    VALIDATE_NOT_NULL(logicalWidth);
    *logicalWidth = mLogicalWidth;
    return NOERROR;
}

ECode CDisplayInfo::SetLogicalWidth(
    /* [in] */ Int32 logicalWidth)
{
    mLogicalWidth = logicalWidth;
    return NOERROR;
}

ECode CDisplayInfo::GetLogicalHeight(
    /* [out] */ Int32* logicalHeight)
{
    VALIDATE_NOT_NULL(logicalHeight);
    *logicalHeight = mLogicalHeight;
    return NOERROR;
}

ECode CDisplayInfo::SetLogicalHeight(
    /* [in] */ Int32 logicalHeight)
{
    mLogicalHeight = logicalHeight;
    return NOERROR;
}

ECode CDisplayInfo::GetRotation(
    /* [out] */ Int32* rotation)
{
    VALIDATE_NOT_NULL(rotation);
    *rotation = mRotation;
    return NOERROR;
}

ECode CDisplayInfo::SetRotation(
    /* [in] */ Int32 rotation)
{
    mRotation = rotation;
    return NOERROR;
}

ECode CDisplayInfo::GetRefreshRate(
    /* [out] */ Float* refreshRate)
{
    VALIDATE_NOT_NULL(refreshRate);
    *refreshRate = mRefreshRate;
    return NOERROR;
}

ECode CDisplayInfo::SetRefreshRate(
    /* [in] */ Float refreshRate)
{
    mRefreshRate = refreshRate;
    return NOERROR;
}

ECode CDisplayInfo::GetLogicalDensityDpi(
    /* [out] */ Int32* logicalDensityDpi)
{
    VALIDATE_NOT_NULL(logicalDensityDpi);
    *logicalDensityDpi = mLogicalDensityDpi;
    return NOERROR;
}

ECode CDisplayInfo::SetLogicalDensityDpi(
    /* [in] */ Int32 logicalDensityDpi)
{
    mLogicalDensityDpi = logicalDensityDpi;
    return NOERROR;
}

ECode CDisplayInfo::GetPhysicalXDpi(
    /* [out] */ Float* physicalXDpi)
{
    VALIDATE_NOT_NULL(physicalXDpi);
    *physicalXDpi = mPhysicalXDpi;
    return NOERROR;
}

ECode CDisplayInfo::SetPhysicalXDpi(
    /* [in] */ Float physicalXDpi)
{
    mPhysicalXDpi = physicalXDpi;
    return NOERROR;
}

ECode CDisplayInfo::GetPhysicalYDpi(
    /* [out] */ Float* physicalYDpi)
{
    VALIDATE_NOT_NULL(physicalYDpi);
    *physicalYDpi = mPhysicalYDpi;
    return NOERROR;
}

ECode CDisplayInfo::SetPhysicalYDpi(
    /* [in] */ Float physicalYDpi)
{
    mPhysicalYDpi = physicalYDpi;
    return NOERROR;
}

ECode CDisplayInfo::EqualsEx(
    /* [in] */ IDisplayInfo* otherObj,
    /* [out] */ Boolean* isEquals)
{
    VALIDATE_NOT_NULL(isEquals);
    CDisplayInfo* other = (CDisplayInfo*)otherObj;

    *isEquals = other != NULL
        && mLayerStack == other->mLayerStack
        && mFlags == other->mFlags
        && mType == other->mType
        && mAddress.Equals(other->mAddress)
        && mName.Equals(other->mName)
        && mAppWidth == other->mAppWidth
        && mAppHeight == other->mAppHeight
        && mSmallestNominalAppWidth == other->mSmallestNominalAppWidth
        && mSmallestNominalAppHeight == other->mSmallestNominalAppHeight
        && mLargestNominalAppWidth == other->mLargestNominalAppWidth
        && mLargestNominalAppHeight == other->mLargestNominalAppHeight
        && mLogicalWidth == other->mLogicalWidth
        && mLogicalHeight == other->mLogicalHeight
        && mRotation == other->mRotation
        && mRefreshRate == other->mRefreshRate
        && mLogicalDensityDpi == other->mLogicalDensityDpi
        && mPhysicalXDpi == other->mPhysicalXDpi
        && mPhysicalYDpi == other->mPhysicalYDpi;

    return NOERROR;
}

ECode CDisplayInfo::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(other);

    return EqualsEx(IDisplayInfo::Probe(other), result);
}

ECode CDisplayInfo::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    *hash = 0; // don't care
    return NOERROR;
}

ECode CDisplayInfo::GetNaturalWidth(
    /* [out] */ Int32* width)
{
    if(mRotation == ISurface::ROTATION_0 || mRotation == ISurface::ROTATION_180)
    {
        *width = mLogicalWidth;
    }else {
        *width = mLogicalHeight;
    }
    return NOERROR;
}

ECode CDisplayInfo::GetNaturalHeight(
    /* [out] */ Int32* height)
{
    if(mRotation == ISurface::ROTATION_0 || mRotation == ISurface::ROTATION_180)
    {
        *height = mLogicalHeight;
    }else {
        *height = mLogicalWidth;
    }
    return NOERROR;
}

ECode CDisplayInfo::CopyFrom(
    /* [in] */ IDisplayInfo* otherObj)
{
    assert(otherObj);
    CDisplayInfo* other = (CDisplayInfo*)otherObj;
    mLayerStack = other->mLayerStack;
    mFlags = other->mFlags;
    mType = other->mType;
    mAddress = other->mAddress;
    mName = other->mName;
    mAppWidth = other->mAppWidth;
    mAppHeight = other->mAppHeight;
    mSmallestNominalAppWidth = other->mSmallestNominalAppWidth;
    mSmallestNominalAppHeight = other->mSmallestNominalAppHeight;
    mLargestNominalAppWidth = other->mLargestNominalAppWidth;
    mLargestNominalAppHeight = other->mLargestNominalAppHeight;
    mLogicalWidth = other->mLogicalWidth;
    mLogicalHeight = other->mLogicalHeight;
    mRotation = other->mRotation;
    mRefreshRate = other->mRefreshRate;
    mLogicalDensityDpi = other->mLogicalDensityDpi;
    mPhysicalXDpi = other->mPhysicalXDpi;
    mPhysicalYDpi = other->mPhysicalYDpi;

    return NOERROR;
}

ECode CDisplayInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    FAIL_RETURN(source->ReadInt32(&mLayerStack));
    FAIL_RETURN(source->ReadInt32(&mFlags));
    FAIL_RETURN(source->ReadInt32(&mType));
    FAIL_RETURN(source->ReadString(&mAddress));
    FAIL_RETURN(source->ReadString(&mName));
    FAIL_RETURN(source->ReadInt32(&mAppWidth));
    FAIL_RETURN(source->ReadInt32(&mAppHeight));
    FAIL_RETURN(source->ReadInt32(&mSmallestNominalAppWidth));
    FAIL_RETURN(source->ReadInt32(&mSmallestNominalAppHeight));
    FAIL_RETURN(source->ReadInt32(&mLargestNominalAppWidth));
    FAIL_RETURN(source->ReadInt32(&mLargestNominalAppHeight));
    FAIL_RETURN(source->ReadInt32(&mLogicalWidth));
    FAIL_RETURN(source->ReadInt32(&mLogicalHeight));
    FAIL_RETURN(source->ReadInt32(&mRotation));
    FAIL_RETURN(source->ReadFloat(&mRefreshRate));
    FAIL_RETURN(source->ReadInt32(&mLogicalDensityDpi));
    FAIL_RETURN(source->ReadFloat(&mPhysicalXDpi));
    FAIL_RETURN(source->ReadFloat(&mPhysicalYDpi));

    return NOERROR;
}

ECode CDisplayInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteInt32(mLayerStack));
    FAIL_RETURN(dest->WriteInt32(mFlags));
    FAIL_RETURN(dest->WriteInt32(mType));
    FAIL_RETURN(dest->WriteString(mAddress));
    FAIL_RETURN(dest->WriteString(mName));
    FAIL_RETURN(dest->WriteInt32(mAppWidth));
    FAIL_RETURN(dest->WriteInt32(mAppHeight));
    FAIL_RETURN(dest->WriteInt32(mSmallestNominalAppWidth));
    FAIL_RETURN(dest->WriteInt32(mSmallestNominalAppHeight));
    FAIL_RETURN(dest->WriteInt32(mLargestNominalAppWidth));
    FAIL_RETURN(dest->WriteInt32(mLargestNominalAppHeight));
    FAIL_RETURN(dest->WriteInt32(mLogicalWidth));
    FAIL_RETURN(dest->WriteInt32(mLogicalHeight));
    FAIL_RETURN(dest->WriteInt32(mRotation));
    FAIL_RETURN(dest->WriteFloat(mRefreshRate));
    FAIL_RETURN(dest->WriteInt32(mLogicalDensityDpi));
    FAIL_RETURN(dest->WriteFloat(mPhysicalXDpi));
    FAIL_RETURN(dest->WriteFloat(mPhysicalYDpi));

    return NOERROR;
}

ECode CDisplayInfo::GetAppMetrics(
    /* [in] */ IDisplayMetrics* outMetrics,
    /* [in] */ ICompatibilityInfoHolder* cih)
{
    return GetMetricsWithSize(outMetrics, cih, mAppWidth, mAppHeight);
}

ECode CDisplayInfo::GetLogicalMetrics(
    /* [in] */ IDisplayMetrics* outMetrics,
    /* [in] */ ICompatibilityInfoHolder* cih)
{
    return GetMetricsWithSize(outMetrics, cih, mLogicalWidth, mLogicalHeight);
}

ECode CDisplayInfo::GetMetricsWithSize(
    /* [in] */ IDisplayMetrics* outMetrics,
    /* [in] */ ICompatibilityInfoHolder* cih,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    assert(outMetrics);

    outMetrics->SetDensityDpi(mLogicalDensityDpi);
    outMetrics->SetNoncompatDensityDpi(mLogicalDensityDpi);
    outMetrics->SetNoncompatWidthPixels(width);
    outMetrics->SetWidthPixels(width);
    outMetrics->SetNoncompatHeightPixels(height);
    outMetrics->SetHeightPixels(height);

    Float density = mLogicalDensityDpi * IDisplayMetrics::DENSITY_DEFAULT_SCALE;
    outMetrics->SetDensity(density);
    outMetrics->SetNoncompatDensity(density);
    outMetrics->GetDensity(&density);
    outMetrics->SetScaledDensity(density);
    outMetrics->SetNoncompatScaledDensity(density);

    outMetrics->SetXdpi(mPhysicalXDpi);
    outMetrics->SetNoncompatXdpi(mPhysicalXDpi);
    outMetrics->SetYdpi(mPhysicalXDpi);
    outMetrics->SetNoncompatYdpi(mPhysicalXDpi);

    if (cih != NULL) {
        AutoPtr<ICompatibilityInfo> ci;
        cih->GetIfNeeded((ICompatibilityInfo**)&ci);
        if (ci != NULL) {
            FAIL_RETURN(ci->ApplyToDisplayMetrics(outMetrics));
        }
    }

    return NOERROR;
}

// // For debugging purposes
// @Override
// public String toString() {
//     return "DisplayInfo{\"" + name + "\", app " + appWidth + " x " + appHeight
//             + ", real " + logicalWidth + " x " + logicalHeight
//             + ", largest app " + largestNominalAppWidth + " x " + largestNominalAppHeight
//             + ", smallest app " + smallestNominalAppWidth + " x " + smallestNominalAppHeight
//             + ", " + refreshRate + " fps"
//             + ", rotation " + rotation
//             + ", density " + logicalDensityDpi
//             + ", " + physicalXDpi + " x " + physicalYDpi + " dpi"
//             + ", layerStack " + layerStack
//             + ", type " + Display.typeToString(type)
//             + ", address " + address
//             + flagsToString(flags) + "}";
// }

// private static String flagsToString(int flags) {
//     StringBuilder result = new StringBuilder();
//     if ((flags & Display.FLAG_SECURE) != 0) {
//         result.append(", FLAG_SECURE");
//     }
//     if ((flags & Display.FLAG_SUPPORTS_PROTECTED_BUFFERS) != 0) {
//         result.append(", FLAG_SUPPORTS_PROTECTED_BUFFERS");
//     }
//     return result.toString();
// }

} // namespace View
} // namespace Droid
} // namespace Elastos
