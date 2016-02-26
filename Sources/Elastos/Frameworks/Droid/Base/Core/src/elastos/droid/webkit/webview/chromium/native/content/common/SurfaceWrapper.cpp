
#include "Elastos.Droid.View.h"
#include "elastos/droid/webkit/webview/chromium/native/content/common/SurfaceWrapper.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {
namespace Content {
namespace Common {

// TODO
// static final Parcelable.Creator<SurfaceWrapper> CREATOR
//         = new Parcelable.Creator<SurfaceWrapper>() {
//     public SurfaceWrapper createFromParcel(Parcel in) {
//         Surface surface = Surface.CREATOR.createFromParcel(in);
//         return new SurfaceWrapper(surface);
//     }

//     public SurfaceWrapper[] newArray(int size) {
//         return new SurfaceWrapper[size];
//     }
// };

SurfaceWrapper::SurfaceWrapper(
    /* [in] */ ISurface* surface)
    : mSurface(surface)
{
}

AutoPtr<ISurface> SurfaceWrapper::GetSurface()
{
    return mSurface;
}

// TODO
// //@Override
// ECode SurfaceWrapper::WriteToParcel(
//     /* [in] */ IParcel* out,
//     /* [in] */ Int32 flags)
// {
//     // Ignore flags so that the Surface won't call release()
//     return mSurface->WriteToParcel(out, 0);
// }

} // namespace Common
} // namespace Content
} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
