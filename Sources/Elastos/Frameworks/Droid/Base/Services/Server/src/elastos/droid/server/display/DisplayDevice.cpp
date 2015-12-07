
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/server/display/DisplayDevice.h"

using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::View::ISurfaceHelper;
using Elastos::Droid::View::CSurfaceHelper;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

DisplayDevice::DisplayDevice(
    /* [in] */ DisplayAdapter* displayAdapter,
    /* [in] */ IBinder* displayToken)
    : mDisplayAdapter(displayAdapter)
    , mDisplayToken(displayToken)
    , mCurrentLayerStack(-1)
    , mCurrentOrientation(-1)
{}

AutoPtr<DisplayAdapter> DisplayDevice::GetAdapterLocked()
{
    return mDisplayAdapter;
}

AutoPtr<IBinder> DisplayDevice::GetDisplayTokenLocked()
{
    return mDisplayToken;
}

String DisplayDevice::GetNameLocked()
{
    return GetDisplayDeviceInfoLocked()->mName;
}

void DisplayDevice::ApplyPendingDisplayDeviceInfoChangesLocked()
{
}

void DisplayDevice::PerformTraversalInTransactionLocked()
{
}

void DisplayDevice::BlankLocked()
{
}

void DisplayDevice::UnblankLocked()
{
}

void DisplayDevice::SetLayerStackInTransactionLocked(
    /* [in] */ Int32 layerStack)
{
    if (mCurrentLayerStack != layerStack) {
        mCurrentLayerStack = layerStack;
        AutoPtr<ISurfaceHelper> surfaceHelper;
        CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&surfaceHelper);
        surfaceHelper->SetDisplayLayerStack(mDisplayToken, layerStack);
    }
}

void DisplayDevice::SetProjectionInTransactionLocked(
    /* [in] */ Int32 orientation,
    /* [in] */ IRect* layerStackRect,
    /* [in] */ IRect* displayRect)
{
    Boolean isEquals;
    if (mCurrentOrientation != orientation
        || mCurrentLayerStackRect == NULL
        || !(mCurrentLayerStackRect->Equals(layerStackRect, &isEquals), isEquals)
        || mCurrentDisplayRect == NULL
        || !(mCurrentDisplayRect->Equals(displayRect, &isEquals), isEquals)) {
        mCurrentOrientation = orientation;

        if (mCurrentLayerStackRect == NULL) {
            ASSERT_SUCCEEDED(CRect::New((IRect**)&mCurrentLayerStackRect));
        }
        mCurrentLayerStackRect->Set(layerStackRect);

        if (mCurrentDisplayRect == NULL) {
            ASSERT_SUCCEEDED(CRect::New((IRect**)&mCurrentDisplayRect));
        }
        mCurrentDisplayRect->Set(displayRect);

        AutoPtr<ISurfaceHelper> surfaceHelper;
        CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&surfaceHelper);
        surfaceHelper->SetDisplayProjection(mDisplayToken,
            orientation, layerStackRect, displayRect);
    }
}

void DisplayDevice::SetSurfaceInTransactionLocked(
    /* [in] */ ISurface* surface)
{
    if (mCurrentSurface.Get() != surface) {
        mCurrentSurface = surface;
        AutoPtr<ISurfaceHelper> surfaceHelper;
        CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&surfaceHelper);
        surfaceHelper->SetDisplaySurface(mDisplayToken, surface);
    }
}

void DisplayDevice::PopulateViewportLocked(
    /* [in] */ DisplayViewport* viewport)
{
    viewport->mOrientation = mCurrentOrientation;

    if (mCurrentLayerStackRect != NULL) {
        viewport->mLogicalFrame->Set(mCurrentLayerStackRect);
    }
    else {
        viewport->mLogicalFrame->SetEmpty();
    }

    if (mCurrentDisplayRect != NULL) {
        viewport->mPhysicalFrame->Set(mCurrentDisplayRect);
    }
    else {
        viewport->mPhysicalFrame->SetEmpty();
    }

    Boolean isRotated = (mCurrentOrientation == ISurface::ROTATION_90
        || mCurrentOrientation == ISurface::ROTATION_270);
    AutoPtr<DisplayDeviceInfo> info = GetDisplayDeviceInfoLocked();
    viewport->mDeviceWidth = isRotated ? info->mHeight : info->mWidth;
    viewport->mDeviceHeight = isRotated ? info->mWidth : info->mHeight;
}

void DisplayDevice::DumpLocked(
    /* [in] */ IPrintWriter* pw)
{
    //pw->PrintStringln(String("mAdapter=") + mDisplayAdapter->GetName());
    // pw->PrintStringln(String("mDisplayToken=") + mDisplayToken);
    // pw->PrintStringln(String("mCurrentLayerStack=") + mCurrentLayerStack);
    // pw->PrintStringln(String("mCurrentOrientation=") + mCurrentOrientation);
    // pw->PrintStringln(String("mCurrentLayerStackRect=") + mCurrentLayerStackRect);
    // pw->PrintStringln(String("mCurrentDisplayRect=") + mCurrentDisplayRect);
    // pw->PrintStringln(String("mCurrentSurface=") + mCurrentSurface);
    assert(0);
}

} // namespace Display
} // namespace Server
} // namespace Droid
} // namespace Elastos
