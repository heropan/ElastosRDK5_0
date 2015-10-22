#ifndef __ELASTOS_DROID_WEBKIT_MEDIA_VIDEOCAPTUREANDROID_H__
#define __ELASTOS_DROID_WEBKIT_MEDIA_VIDEOCAPTUREANDROID_H__
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/webkit/native/media/VideoCapture.h"

using Elastos::Droid::Hardware::IHardwareCamera;
using Elastos::Droid::Hardware::IParameters;
using Elastos::Droid::Hardware::IPreviewCallback;

// import android.content.Context;
// import android.graphics.ImageFormat;
// import android.hardware.Camera;
// import android.hardware.Camera.Size;
// import android.util.Log;

// import java.util.ArrayList;
// import java.util.List;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

/**
 * This class extends the VideoCapture base class for manipulating normal video
 * capture devices in Android, including receiving copies of preview frames via
 * Java-allocated buffers. It also includes class BuggyDeviceHack to deal with
 * troublesome devices.
 **/
class VideoCaptureAndroid : public VideoCapture
{
private:
    // Some devices don't support YV12 format correctly, even with JELLY_BEAN or
    // newer OS. To work around the issues on those devices, we have to request
    // NV21. Some other devices have troubles with certain capture resolutions
    // under a given one: for those, the resolution is swapped with a known
    // good. Both are supposed to be temporary hacks.
    class BuggyDeviceHack
    {
    public:
        class IdAndSizes
        :public Object
        {
        public:
            IdAndSizes(
                /* [in] */ const String& model,
                /* [in] */ const String& device,
                /* [in] */ Int32 minWidth,
                /* [in] */ Int32 minHeight);

        public:
            const String mModel;
            const String mDevice;
            const Int32 mMinWidth;
            const Int32 mMinHeight;
        };

    public:
        static CARAPI_(void) ApplyMinDimensions(
            /* [in] */ CaptureFormat* format);

        static CARAPI_(Int32) GetImageFormat();

    private:
        static const AutoPtr<ArrayOf<IdAndSizes*> > s_CAPTURESIZE_BUGGY_DEVICE_LIST;

        static const AutoPtr<ArrayOf<String> > s_COLORSPACE_BUGGY_DEVICE_LIST;
    };

public:
    VideoCaptureAndroid(
        /* [in] */ IContext* context,
        /* [in] */ Int32 id,
        /* [in] */ Int64 nativeVideoCaptureDeviceAndroid);

    static CARAPI_(AutoPtr< ArrayOf<VideoCapture::CaptureFormat*> >) GetDeviceSupportedFormats(
        /* [in] */ Int32 id);

    //@Override
    CARAPI OnPreviewFrame(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ IHardwareCamera* camera);

protected:
    //@Override
    CARAPI_(void) SetCaptureParameters(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 frameRate,
        /* [in] */ IParameters* cameraParameters);

    //@Override
    CARAPI_(void) AllocateBuffers();

    //@Override
    CARAPI_(void) SetPreviewCallback(
        /* [in] */ IPreviewCallback* cb);

    // TODO(wjia): investigate whether reading from texture could give better
    // performance and frame rate, using onFrameAvailable().

    static AutoPtr<ArrayOf<BuggyDeviceHack::IdAndSizes*> > s_CAPTURESIZE_BUGGY_DEVICE_LIST_Init();
private:
    Int32 mExpectedFrameSize;
    static const Int32 NUM_CAPTURE_BUFFERS = 3;
    static const String TAG;
};

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

DEFINE_CONVERSION_FOR(Elastos::Droid::Webkit::Media::VideoCaptureAndroid::BuggyDeviceHack::IdAndSizes, IInterface);

#endif//__ELASTOS_DROID_WEBKIT_MEDIA_VIDEOCAPTUREANDROID_H__
