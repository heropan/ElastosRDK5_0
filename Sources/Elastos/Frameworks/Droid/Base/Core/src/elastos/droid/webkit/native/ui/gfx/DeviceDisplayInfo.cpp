// wuweizuo automatic build .cpp file from .java file.

#include "DeviceDisplayInfo.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Gfx {

//=====================================================================
//                          DeviceDisplayInfo
//=====================================================================
Int32 DeviceDisplayInfo::GetDisplayHeight()
{
    // ==================before translated======================
    // getDisplay().getSize(mTempPoint);
    // return mTempPoint.y;
    assert(0);
    return 0;
}

Int32 DeviceDisplayInfo::GetDisplayWidth()
{
    // ==================before translated======================
    // getDisplay().getSize(mTempPoint);
    // return mTempPoint.x;
    assert(0);
    return 0;
}

Int32 DeviceDisplayInfo::GetPhysicalDisplayHeight()
{
    // ==================before translated======================
    // if (Build.VERSION.SDK_INT < Build.VERSION_CODES.JELLY_BEAN_MR1) {
    //     return 0;
    // }
    // getDisplay().getRealSize(mTempPoint);
    // return mTempPoint.y;
    assert(0);
    return 0;
}

Int32 DeviceDisplayInfo::GetPhysicalDisplayWidth()
{
    // ==================before translated======================
    // if (Build.VERSION.SDK_INT < Build.VERSION_CODES.JELLY_BEAN_MR1) {
    //     return 0;
    // }
    // getDisplay().getRealSize(mTempPoint);
    // return mTempPoint.x;
    assert(0);
    return 0;
}

Int32 DeviceDisplayInfo::GetBitsPerPixel()
{
    // ==================before translated======================
    // int format = getPixelFormat();
    // PixelFormat info = new PixelFormat();
    // PixelFormat.getPixelFormatInfo(format, info);
    // return info.bitsPerPixel;
    assert(0);
    return 0;
}

Int32 DeviceDisplayInfo::GetBitsPerComponent()
{
    // ==================before translated======================
    // int format = getPixelFormat();
    // switch (format) {
    //     case PixelFormat.RGBA_4444:
    //         return 4;
    //
    //     case PixelFormat.RGBA_5551:
    //         return 5;
    //
    //     case PixelFormat.RGBA_8888:
    //     case PixelFormat.RGBX_8888:
    //     case PixelFormat.RGB_888:
    //         return 8;
    //
    //     case PixelFormat.RGB_332:
    //         return 2;
    //
    //     case PixelFormat.RGB_565:
    //         return 5;
    //
    //     // Non-RGB formats.
    //     case PixelFormat.A_8:
    //     case PixelFormat.LA_88:
    //     case PixelFormat.L_8:
    //         return 0;
    //
    //     // Unknown format. Use 8 as a sensible default.
    //     default:
    //         return 8;
    // }
    assert(0);
    return 0;
}

Double DeviceDisplayInfo::GetDIPScale()
{
    // ==================before translated======================
    // getDisplay().getMetrics(mTempMetrics);
    // return mTempMetrics.density;
    assert(0);
    return 0.0f;
}

ECode DeviceDisplayInfo::UpdateNativeSharedDisplayInfo()
{
    // ==================before translated======================
    // nativeUpdateSharedDeviceDisplayInfo(
    //         getDisplayHeight(), getDisplayWidth(),
    //         getPhysicalDisplayHeight(), getPhysicalDisplayWidth(),
    //         getBitsPerPixel(), getBitsPerComponent(),
    //         getDIPScale(), getSmallestDIPWidth(), getRotationDegrees());
    assert(0);
    return NOERROR;
}

AutoPtr<DeviceDisplayInfo> DeviceDisplayInfo::Create(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // return new DeviceDisplayInfo(context);
    assert(0);
    AutoPtr<DeviceDisplayInfo> empty;
    return empty;
}

DeviceDisplayInfo::DeviceDisplayInfo(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // mAppContext = context.getApplicationContext();
    // mWinManager = (WindowManager) mAppContext.getSystemService(Context.WINDOW_SERVICE);
}

Int32 DeviceDisplayInfo::GetPixelFormat()
{
    // ==================before translated======================
    // if (Build.VERSION.SDK_INT < Build.VERSION_CODES.JELLY_BEAN_MR1) {
    //     return getDisplay().getPixelFormat();
    // }
    // // JellyBean MR1 and later always uses RGBA_8888.
    // return PixelFormat.RGBA_8888;
    assert(0);
    return 0;
}

Int32 DeviceDisplayInfo::GetSmallestDIPWidth()
{
    // ==================before translated======================
    // return mAppContext.getResources().getConfiguration().smallestScreenWidthDp;
    assert(0);
    return 0;
}

Int32 DeviceDisplayInfo::GetRotationDegrees()
{
    // ==================before translated======================
    // switch (getDisplay().getRotation()) {
    //     case Surface.ROTATION_0:
    //         return 0;
    //     case Surface.ROTATION_90:
    //         return 90;
    //     case Surface.ROTATION_180:
    //         return 180;
    //     case Surface.ROTATION_270:
    //         return 270;
    // }
    //
    // // This should not happen.
    // assert false;
    // return 0;
    assert(0);
    return 0;
}

AutoPtr<IDisplay> DeviceDisplayInfo::GetDisplay()
{
    // ==================before translated======================
    // return mWinManager.getDefaultDisplay();
    assert(0);
    AutoPtr<IDisplay> empty;
    return empty;
}

ECode DeviceDisplayInfo::NativeUpdateSharedDeviceDisplayInfo(
    /* [in] */ Int32 displayHeight,
    /* [in] */ Int32 displayWidth,
    /* [in] */ Int32 physicalDisplayHeight,
    /* [in] */ Int32 physicalDisplayWidth,
    /* [in] */ Int32 bitsPerPixel,
    /* [in] */ Int32 bitsPerComponent,
    /* [in] */ Double dipScale,
    /* [in] */ Int32 smallestDIPWidth,
    /* [in] */ Int32 rotationDegrees)
{
    assert(0);
    return NOERROR;
}

} // namespace Gfx
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


