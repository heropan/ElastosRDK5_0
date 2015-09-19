
#include "view/Display.h"
#include "ext/frameworkext.h"
#include "utility/CDisplayMetrics.h"
#include "os/SystemClock.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Droid::Graphics::IPixelFormat;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Utility::CDisplayMetrics;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace View {

const char* Display::TAG = "Display";
const Boolean Display::DEBUG;
const Int32 Display::CACHED_APP_SIZE_DURATION_MILLIS;

CAR_INTERFACE_IMPL(Display, IDisplay);

Display::Display(
    /* [in] */ IDisplayManagerGlobal* global,
    /* [in] */ Int32 displayId,
    /* [in] */ IDisplayInfo* displayInfo /*not NULL*/,
    /* [in] */ ICompatibilityInfoHolder* compatibilityInfo)
    : mGlobal(global)
    , mDisplayId(displayId)
    , mCompatibilityInfo(compatibilityInfo)
    , mDisplayInfo(displayInfo)
    , mIsValid(TRUE)
    , mLastCachedAppSizeUpdate(0)
    , mCachedAppWidthCompat(0)
    , mCachedAppHeightCompat(0)
{
    assert(displayInfo);
    // Cache properties that cannot change as Int64 as the display is valid.
    displayInfo->GetLayerStack(&mLayerStack);
    displayInfo->GetFlags(&mFlags);
    displayInfo->GetType(&mType);
    displayInfo->GetAddress(&mAddress);
    CDisplayMetrics::New((IDisplayMetrics**)&mTempMetrics);
}

ECode Display::GetDisplayId(
    /* [out] */ Int32* displayId)
{
    VALIDATE_NOT_NULL(displayId);
    *displayId = mDisplayId;
    return NOERROR;
}

ECode Display::IsValid(
    /* [out] */ Boolean* isValid)
{
    VALIDATE_NOT_NULL(isValid);

    AutoLock lock(mSelfLock);
    UpdateDisplayInfoLocked();
    *isValid = mIsValid;

    return NOERROR;
}

ECode Display::GetDisplayInfo(
    /* [in] */ IDisplayInfo* outDisplayInfo,
    /* [out] */ Boolean* isValid)
{
    VALIDATE_NOT_NULL(isValid);

    AutoLock lock(mSelfLock);
    UpdateDisplayInfoLocked();
    outDisplayInfo->CopyFrom(mDisplayInfo);
    *isValid = mIsValid;

    return NOERROR;
}

ECode Display::GetLayerStack(
    /* [out] */ Int32* layerStack)
{
    VALIDATE_NOT_NULL(layerStack);
    *layerStack = mLayerStack;

    return NOERROR;
}

ECode Display::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;

    return NOERROR;
}

ECode Display::GetType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mType;

    return NOERROR;
}

ECode Display::GetAddress(
    /* [out] */ String* address)
{
    VALIDATE_NOT_NULL(address);
    *address = mAddress;

    return NOERROR;
}

ECode Display::GetCompatibilityInfo(
    /* [out] */ ICompatibilityInfoHolder** compatibilityInfo)
{
    VALIDATE_NOT_NULL(compatibilityInfo);
    *compatibilityInfo =  mCompatibilityInfo;
    REFCOUNT_ADD(*compatibilityInfo);

    return NOERROR;
}

ECode Display::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    AutoLock lock(mSelfLock);
    UpdateDisplayInfoLocked();
    return mDisplayInfo->GetName(name);
}

ECode Display::GetSize(
    /* [in] */ IPoint* outSize)
{
    AutoLock lock(mSelfLock);
    UpdateDisplayInfoLocked();
    mDisplayInfo->GetAppMetrics(mTempMetrics.Get(), mCompatibilityInfo);
    Int32 widthPixels, heightPixels;
    mTempMetrics->GetWidthPixels(&widthPixels);
    mTempMetrics->GetHeightPixels(&heightPixels);
    outSize->Set(widthPixels, heightPixels);

    return NOERROR;
}

ECode Display::GetRectSize(
    /* [in] */ IRect* outSize)
{
    AutoLock lock(mSelfLock);
    UpdateDisplayInfoLocked();
    mDisplayInfo->GetAppMetrics(mTempMetrics.Get(), mCompatibilityInfo);
    Int32 widthPixels, heightPixels;
    mTempMetrics->GetWidthPixels(&widthPixels);
    mTempMetrics->GetHeightPixels(&heightPixels);
    outSize->Set(0, 0, widthPixels, heightPixels);

    return NOERROR;
}

ECode Display::GetCurrentSizeRange(
    /* [in] */ IPoint* outSmallestSize,
    /* [in] */ IPoint* outLargestSize)
{
    AutoLock lock(mSelfLock);
    UpdateDisplayInfoLocked();
    Int32 x, y;
    mDisplayInfo->GetSmallestNominalAppWidth(&x);
    mDisplayInfo->GetSmallestNominalAppHeight(&y);
    outSmallestSize->Set(x, y);
    mDisplayInfo->GetLargestNominalAppWidth(&x);
    mDisplayInfo->GetLargestNominalAppHeight(&y);
    outLargestSize->Set(x, y);

    return NOERROR;
}

ECode Display::GetMaximumSizeDimension(
    /* [out] */ Int32* maximumSizeDimension)
{
    VALIDATE_NOT_NULL(maximumSizeDimension);

    AutoLock lock(mSelfLock);
    UpdateDisplayInfoLocked();
    Int32 logicalWidth, logicalHeight;
    mDisplayInfo->GetLogicalWidth(&logicalWidth);
    mDisplayInfo->GetLogicalHeight(&logicalHeight);
    *maximumSizeDimension =Elastos::Core::Math::Max(logicalWidth, logicalHeight);

    return NOERROR;
}

ECode Display::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    AutoLock lock(mSelfLock);
    UpdateCachedAppSizeIfNeededLocked();
    *width = mCachedAppWidthCompat;

    return NOERROR;
}

ECode Display::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    AutoLock lock(mSelfLock);
    UpdateCachedAppSizeIfNeededLocked();
    *height = mCachedAppHeightCompat;

    return NOERROR;
}

ECode Display::GetRotation(
    /* [out] */ Int32* rotation)
{
    VALIDATE_NOT_NULL(rotation);
    AutoLock lock(mSelfLock);
    UpdateDisplayInfoLocked();
    return mDisplayInfo->GetRotation(rotation);
}

ECode Display::GetOrientation(
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);
    return GetRotation(orientation);
}

ECode Display::GetPixelFormat(
    /* [out] */ Int32* pixelFormat)
{
    VALIDATE_NOT_NULL(pixelFormat);
    *pixelFormat = IPixelFormat::RGBA_8888;

    return NOERROR;
}

ECode Display::GetRefreshRate(
    /* [out] */ Float* refreshRate)
{
    VALIDATE_NOT_NULL(refreshRate);
    AutoLock lock(mSelfLock);
    UpdateDisplayInfoLocked();
    return mDisplayInfo->GetRefreshRate(refreshRate);
}

ECode Display::GetMetrics(
    /* [in] */ IDisplayMetrics* outMetrics)
{
    AutoLock lock(mSelfLock);
    UpdateDisplayInfoLocked();
    mDisplayInfo->GetAppMetrics(outMetrics, mCompatibilityInfo);

    return NOERROR;
}

ECode Display::GetRealSize(
    /* [in] */ IPoint* outSize)
{
    AutoLock lock(mSelfLock);
    UpdateDisplayInfoLocked();
    Int32 logicalWidth, logicalHeight;
    mDisplayInfo->GetLogicalWidth(&logicalWidth);
    mDisplayInfo->GetLogicalHeight(&logicalHeight);
    outSize->Set(logicalWidth, logicalHeight);

    return NOERROR;
}

ECode Display::GetRealMetrics(
    /* [in] */ IDisplayMetrics* outMetrics)
{
    AutoLock lock(mSelfLock);
    UpdateDisplayInfoLocked();
    return mDisplayInfo->GetLogicalMetrics(outMetrics, NULL);
}

ECode Display::GetRawWidth(
    /* [out] */ Int32* width)
{
    return mDisplayInfo->GetLogicalWidth(width);
}

ECode Display::GetRawHeight(
    /* [out] */ Int32* height)
{
    return mDisplayInfo->GetLogicalHeight(height);
}

void Display::UpdateDisplayInfoLocked()
{
    // Note: The display manager caches display info objects on our behalf.
    AutoPtr<IDisplayInfo> newInfo;
    mGlobal->GetDisplayInfo(mDisplayId, (IDisplayInfo**)&newInfo);
    if (newInfo == NULL) {
        // Preserve the old mDisplayInfo after the display is removed.
        if (mIsValid) {
            mIsValid = FALSE;
            if (DEBUG) {
                Logger::D(TAG, "Logical display %d was removed.", mDisplayId);
            }
        }
    }
    else {
        // Use the new display info.  (It might be the same object if nothing changed.)
        mDisplayInfo = newInfo;
        if (!mIsValid) {
            mIsValid = TRUE;
            if (DEBUG) {
                Logger::D(TAG, "Logical display %d was recreated.", mDisplayId);
            }
        }
    }
}

void Display::UpdateCachedAppSizeIfNeededLocked()
{
    Int64 now = SystemClock::GetUptimeMillis();
    if (now > mLastCachedAppSizeUpdate + CACHED_APP_SIZE_DURATION_MILLIS) {
        UpdateDisplayInfoLocked();
        mDisplayInfo->GetAppMetrics(mTempMetrics.Get(), mCompatibilityInfo);
        mTempMetrics->GetWidthPixels(&mCachedAppWidthCompat);
        mTempMetrics->GetHeightPixels(&mCachedAppHeightCompat);
        mLastCachedAppSizeUpdate = now;
    }
}

// // For debugging purposes
// //@Override
// String ToString()
// {
//     AutoLock lock(mSelfLock);
//     UpdateDisplayInfoLocked();
//     mDisplayInfo->GetAppMetrics(mTempMetrics.Get(), mCompatibilityInfo);
//     return "Display id " + mDisplayId + ": " + mDisplayInfo
//             + ", " + mTempMetrics + ", isValid=" + mIsValid;
// }

String Display::TypeToString(
    /* [in] */ Int32 type)
{
    switch (type) {
        case TYPE_UNKNOWN:
            return String("UNKNOWN");
        case TYPE_BUILT_IN:
            return String("BUILT_IN");
        case TYPE_HDMI:
            return String("HDMI");
        case TYPE_WIFI:
            return String("WIFI");
        case TYPE_OVERLAY:
            return String("OVERLAY");
        default:
            return StringUtils::Int32ToString(type);
    }
}

} // namespace View
} // namepsace Droid
} // namespace Elastos
