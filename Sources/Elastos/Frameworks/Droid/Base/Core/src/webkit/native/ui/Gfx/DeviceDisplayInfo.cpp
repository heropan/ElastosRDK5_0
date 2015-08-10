// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

DeviceDisplayInfo::DeviceDisplayInfo(
    /* in */ IContext* context)
{
	AutoPtr<IContext> applicationContext;
	context->GetApplicationContext((IContext**)&applicationContext);
    mAppContext = applicationContext;
    mAppContext->GetSystemService(IContext::WINDOW_SERVICE, (IWindowManager**)&mWinManager);
}

Int32 DeviceDisplayInfo::GetDisplayHeight()
{
    GetDisplay()->GetSize(mTempPoint);
    return mTempPoint->y;
}

Int32 DeviceDisplayInfo::GetDisplayWidth()
{
    GetDisplay()->GetSize(mTempPoint);
    return mTempPoint-x;
}

Int32 DeviceDisplayInfo::GetPhysicalDisplayHeight()
{
    if (IBuild::VERSION::SDK_INT < IBuild::VERSION_CODES::JELLY_BEAN_MR1) {
        return 0;
    }
    GetDisplay()->GetRealSize(mTempPoint);
    return mTempPoint->y;
}

Int32 DeviceDisplayInfo::GetPhysicalDisplayWidth()
{
    if (IBuild::VERSION::SDK_INT < IBuild::VERSION_CODES::JELLY_BEAN_MR1) {
        return 0;
    }
    GetDisplay()->GetRealSize(mTempPoint);
    return mTempPoint->x;
}

Int32 DeviceDisplayInfo::GetBitsPerPixel()
{
    Int32 format = GetPixelFormat();
    AutoPtr<IPixelFormat> info;
    CPixelFormat::New((IPixelFormat**)&info);
	CPixelFormat::GetPixelFormatInfo(format, &info);
	return info->bitsPerPixel;
}

Int32 DeviceDisplayInfo::GetBitsPerComponent()
{
    Int32 format = GetPixelFormat();
    switch (format) {
		case IPixelFormat::RGBA_4444:
            return 4;
		case IPixelFormat::RGBA_5551:
            return 5;
		case IPixelFormat::RGBA_8888:
		case IPixelFormat::RGBA_8888:
        case IPixelFormat::RGBX_8888:
        case IPixelFormat::RGBA_8888:
        case IPixelFormat::RGBA_8888:
        case IPixelFormat::RGBX_8888:
        case IPixelFormat::RGB_888:
            return 8;
		case IPixelFormat::RGB_332:
            return 2;
		case IPixelFormat::RGB_565:
            return 5;
		case IPixelFormat::A_8:
		case IPixelFormat::A_8:
        case IPixelFormat::LA_88:
        case IPixelFormat::A_8:
        case IPixelFormat::A_8:
        case IPixelFormat::LA_88:
        case IPixelFormat::L_8:
            return 0;
		default:
            return 8;
    }
}

Double DeviceDisplayInfo::GetDIPScale()
{
    GetDisplay()->GetMetrics(&mTempMetrics);
    return mTempMetrics->density;
}

ECode DeviceDisplayInfo::UpdateNativeSharedDisplayInfo()
{
	NativeUpdateSharedDeviceDisplayInfo(
                GetDisplayHeight(), GetDisplayWidth(),
                GetPhysicalDisplayHeight(), GetPhysicalDisplayWidth(),
                GetBitsPerPixel(), GetBitsPerComponent(),
                GetDIPScale(), GetSmallestDIPWidth(), GetRotationDegrees());
    return NOERROR;
}

AutoPtr<DeviceDisplayInfo> DeviceDisplayInfo::Create(
    /* in */ IContext* context)
{
	AutoPtr<DeviceDisplayInfo> deviceDisplayInfo = new DeviceDisplayInfo(context);
    return deviceDisplayInfo;
}

Int32 DeviceDisplayInfo::GetPixelFormat()
{
    if (IBuild::VERSION::SDK_INT < IBuild::VERSION_CODES::JELLY_BEAN_MR1) {
        return GetDisplay()->GetPixelFormat();
    }
    return IPixelFormat::RGBA_8888;
}

Int32 DeviceDisplayInfo::GetSmallestDIPWidth()
{
	AutoPtr<IResources> resources;
	mAppContext->GetResources((IResources**)&resources);

	AutoPtr<IConfiguration> configuration;
	resources->GetConfiguration((IConfiguration**)&configuration);
    return configuration->smallestScreenWidthDp;
}

Int32 DeviceDisplayInfo::GetRotationDegrees()
{
    switch (GetDisplay()->GetRotation()) {
		case ISurface::ROTATION_0:
            return 0;
		case ISurface::ROTATION_90:
            return 90;
		case ISurface::ROTATION_180:
            return 180;
		case ISurface::ROTATION_270:
            return 270;
    }
    assert (FALSE);
    return 0;
}

AutoPtr<IDisplay> DeviceDisplayInfo::GetDisplay()
{
	AutoPtr<IDisplay> display;
	mWinManager->GetDefaultDisplay((IDisplay**)&display);
    return display;
}

ECode DeviceDisplayInfo::NativeUpdateSharedDeviceDisplayInfo(
    /* in */ Int32 displayHeight,
    /* in */ Int32 displayWidth,
    /* in */ Int32 physicalDisplayHeight,
    /* in */ Int32 physicalDisplayWidth,
    /* in */ Int32 bitsPerPixel,
    /* in */ Int32 bitsPerComponent,
    /* in */ Double dipScale,
    /* in */ Int32 smallestDIPWidth,
    /* in */ Int32 rotationDegrees)
{
    return NOERROR;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

