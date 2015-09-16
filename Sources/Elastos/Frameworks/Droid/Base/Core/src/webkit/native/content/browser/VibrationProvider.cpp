// wuweizuo automatic build .cpp file from .java file.

#include "VibrationProvider.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                          VibrationProvider
//=====================================================================
const String VibrationProvider::TAG("VibrationProvider");

VibrationProvider::VibrationProvider(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // mAudioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
    // mVibrator = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
    // mHasVibratePermission = context.checkCallingOrSelfPermission(
    //         android.Manifest.permission.VIBRATE) == PackageManager.PERMISSION_GRANTED;
    // if (!mHasVibratePermission) {
    //     Log.w(TAG, "Failed to use vibrate API, requires VIBRATE permission.");
    // }
}

AutoPtr<VibrationProvider> VibrationProvider::Create(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // return new VibrationProvider(context);
    assert(0);
    AutoPtr<VibrationProvider> empty;
    return empty;
}

ECode VibrationProvider::Vibrate(
    /* [in] */ Int64 milliseconds)
{
    // ==================before translated======================
    // if (mAudioManager.getRingerMode() != AudioManager.RINGER_MODE_SILENT &&
    //         mHasVibratePermission) {
    //     mVibrator.vibrate(milliseconds);
    // }
    assert(0);
    return NOERROR;
}

ECode VibrationProvider::CancelVibration()
{
    // ==================before translated======================
    // if (mHasVibratePermission) mVibrator.cancel();
    assert(0);
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


