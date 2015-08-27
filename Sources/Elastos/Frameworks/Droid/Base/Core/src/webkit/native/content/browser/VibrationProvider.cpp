// wuweizuo automatic build .cpp file from .java file.

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
    /*
    mAudioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
    mVibrator = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
    mHasVibratePermission = context.checkCallingOrSelfPermission(
            android.Manifest.permission.VIBRATE) == PackageManager.PERMISSION_GRANTED;
    if (!mHasVibratePermission) {
        Log.w(TAG, "Failed to use vibrate API, requires VIBRATE permission.");
    }
    */
}

AutoPtr<VibrationProvider> VibrationProvider::Create(
    /* [in] */ IContext* context)
{
    /*
    return new VibrationProvider(context);
    */
    return AutoPtr<VibrationProvider>(this);
}

ECode VibrationProvider::Vibrate(
    /* [in] */ Int64 milliseconds)
{
    /*
    if (mAudioManager.getRingerMode() != AudioManager.RINGER_MODE_SILENT &&
            mHasVibratePermission) {
        mVibrator.vibrate(milliseconds);
    }
    */
    return NOERROR;
}

ECode VibrationProvider::CancelVibration()
{
    /*
    if (mHasVibratePermission) mVibrator.cancel();
    */
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


