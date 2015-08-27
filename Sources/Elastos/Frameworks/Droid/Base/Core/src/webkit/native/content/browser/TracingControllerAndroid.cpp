// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//            TracingControllerAndroid::TracingIntentFilter
//=====================================================================
TracingControllerAndroid::TracingIntentFilter::TracingIntentFilter(
    /* [in] */ IContext* context)
{
    /*
    addAction(context.getPackageName() + "." + ACTION_START);
    addAction(context.getPackageName() + "." + ACTION_STOP);
    addAction(context.getPackageName() + "." + ACTION_LIST_CATEGORIES);
    */
}

//=====================================================================
//          TracingControllerAndroid::TracingBroadcastReceiver
//=====================================================================
ECode TracingControllerAndroid::TracingBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(intent);
    /*
    if (intent.getAction().endsWith(ACTION_START)) {
        String categories = intent.getStringExtra(CATEGORIES_EXTRA);
        if (TextUtils.isEmpty(categories)) {
            categories = nativeGetDefaultCategories();
        } else {
            categories = categories.replaceFirst(
                    DEFAULT_CHROME_CATEGORIES_PLACE_HOLDER, nativeGetDefaultCategories());
        }
        boolean recordContinuously =
                intent.getStringExtra(RECORD_CONTINUOUSLY_EXTRA) != null;
        String filename = intent.getStringExtra(FILE_EXTRA);
        if (filename != null) {
            startTracing(filename, true, categories, recordContinuously);
        } else {
            startTracing(true, categories, recordContinuously);
        }
    } else if (intent.getAction().endsWith(ACTION_STOP)) {
        stopTracing();
    } else if (intent.getAction().endsWith(ACTION_LIST_CATEGORIES)) {
        getCategoryGroups();
    } else {
        Log.e(TAG, "Unexpected intent: " + intent);
    }
    */
    return NOERROR;
}

//=====================================================================
//                       TracingControllerAndroid
//=====================================================================
const String TracingControllerAndroid::TAG("TracingControllerAndroid");
const String TracingControllerAndroid::ACTION_START("GPU_PROFILER_START");
const String TracingControllerAndroid::ACTION_STOP("GPU_PROFILER_STOP");
const String TracingControllerAndroid::ACTION_LIST_CATEGORIES("GPU_PROFILER_LIST_CATEGORIES");
const String TracingControllerAndroid::FILE_EXTRA("file");
const String TracingControllerAndroid::CATEGORIES_EXTRA("categories");
const String TracingControllerAndroid::RECORD_CONTINUOUSLY_EXTRA("continuous");
const String TracingControllerAndroid::DEFAULT_CHROME_CATEGORIES_PLACE_HOLDER("_DEFAULT_CHROME_CATEGORIES");
const String TracingControllerAndroid::PROFILER_STARTED_FMT("Profiler started: %s");
const String TracingControllerAndroid::PROFILER_FINISHED_FMT("Profiler finished. Results are in %s.");

TracingControllerAndroid::TracingControllerAndroid(
    /* [in] */ IContext* context)
{
    /*
    mContext = context;
    mBroadcastReceiver = new TracingBroadcastReceiver();
    mIntentFilter = new TracingIntentFilter(context);
    */
}

AutoPtr<IBroadcastReceiver> TracingControllerAndroid::GetBroadcastReceiver()
{
    /*
    return mBroadcastReceiver;
    */
}

AutoPtr<IIntentFilter> TracingControllerAndroid::GetIntentFilter()
{
    /*
    return mIntentFilter;
    */
}

ECode TracingControllerAndroid::RegisterReceiver(
    /* [in] */ IContext* context)
{
    VALIDATE_NOT_NULL(context);
    /*
    context.registerReceiver(getBroadcastReceiver(), getIntentFilter());
    */
    return NOERROR;
}

ECode TracingControllerAndroid::UnregisterReceiver(
    /* [in] */ IContext* context)
{
    VALIDATE_NOT_NULL(context);
    /*
    context.unregisterReceiver(getBroadcastReceiver());
    */
    return NOERROR;
}

Boolean TracingControllerAndroid::IsTracing()
{
    /*
    return mIsTracing;
    */
}

String TracingControllerAndroid::GetOutputPath()
{
    /*
    return mFilename;
    */
}

Boolean TracingControllerAndroid::StartTracing(
    /* [in] */ Boolean showToasts,
    /* [in] */ String categories,
    /* [in] */ Boolean recordContinuously)
{
    /*
    mShowToasts = showToasts;


    String filePath = generateTracingFilePath();
    if (filePath == null) {
      logAndToastError(
          mContext.getString(R.string.profiler_no_storage_toast));
    }
    return startTracing(filePath, showToasts, categories, recordContinuously);
    */
}

Boolean TracingControllerAndroid::StartTracing(
    /* [in] */ String filename,
    /* [in] */ Boolean showToasts,
    /* [in] */ String categories,
    /* [in] */ Boolean recordContinuously)
{
    /*
    mShowToasts = showToasts;
    if (isTracing()) {
        // Don't need a toast because this shouldn't happen via the UI.
        Log.e(TAG, "Received startTracing, but we're already tracing");
        return false;
    }
    // Lazy initialize the native side, to allow construction before the library is loaded.
    initializeNativeControllerIfNeeded();
    if (!nativeStartTracing(mNativeTracingControllerAndroid, categories,
            recordContinuously)) {
        logAndToastError(mContext.getString(R.string.profiler_error_toast));
        return false;
    }


    logForProfiler(String.format(PROFILER_STARTED_FMT, categories));
    showToast(mContext.getString(R.string.profiler_started_toast) + ": " + categories);
    mFilename = filename;
    mIsTracing = true;
    return true;
    */
}

ECode TracingControllerAndroid::StopTracing()
{
    /*
    if (isTracing()) {
        nativeStopTracing(mNativeTracingControllerAndroid, mFilename);
    }
    */
    return NOERROR;
}

ECode TracingControllerAndroid::GetCategoryGroups()
{
    /*
    // Lazy initialize the native side, to allow construction before the library is loaded.
    initializeNativeControllerIfNeeded();
    if (!nativeGetKnownCategoryGroupsAsync(mNativeTracingControllerAndroid)) {
        Log.e(TAG, "Unable to fetch tracing record groups list.");
    }
    */
    return NOERROR;
}

ECode TracingControllerAndroid::OnTracingStopped()
{
    /*
    if (!isTracing()) {
        // Don't need a toast because this shouldn't happen via the UI.
        Log.e(TAG, "Received onTracingStopped, but we aren't tracing");
        return;
    }


    logForProfiler(String.format(PROFILER_FINISHED_FMT, mFilename));
    showToast(mContext.getString(R.string.profiler_stopped_toast, mFilename));
    mIsTracing = false;
    mFilename = null;
    */
    return NOERROR;
}

ECode TracingControllerAndroid::Finalize()
{
    /*
    if (mNativeTracingControllerAndroid != 0) {
        nativeDestroy(mNativeTracingControllerAndroid);
        mNativeTracingControllerAndroid = 0;
    }
    */
    return NOERROR;
}

String TracingControllerAndroid::GenerateTracingFilePath()
{
    /*
    String state = Environment.getExternalStorageState();
    if (!Environment.MEDIA_MOUNTED.equals(state)) {
        return null;
    }


    // Generate a hopefully-unique filename using the UTC timestamp.
    // (Not a huge problem if it isn't unique, we'll just append more data.)
    SimpleDateFormat formatter = new SimpleDateFormat(
            "yyyy-MM-dd-HHmmss", Locale.US);
    formatter.setTimeZone(TimeZone.getTimeZone("UTC"));
    File dir = Environment.getExternalStoragePublicDirectory(
            Environment.DIRECTORY_DOWNLOADS);
    File file = new File(
            dir, "chrome-profile-results-" + formatter.format(new Date()));
    return file.getPath();
    */
}

ECode TracingControllerAndroid::InitializeNativeControllerIfNeeded()
{
    /*
    if (mNativeTracingControllerAndroid == 0) {
        mNativeTracingControllerAndroid = nativeInit();
    }
    */
    return NOERROR;
}

ECode TracingControllerAndroid::LogAndToastError(
    /* [in] */ String str)
{
    /*
    Log.e(TAG, str);
    if (mShowToasts) Toast.makeText(mContext, str, Toast.LENGTH_SHORT).show();
    */
    return NOERROR;
}

ECode TracingControllerAndroid::LogForProfiler(
    /* [in] */ String str)
{
    /*
    Log.i(TAG, str);
    */
    return NOERROR;
}

ECode TracingControllerAndroid::ShowToast(
    /* [in] */ String str)
{
    /*
    if (mShowToasts) Toast.makeText(mContext, str, Toast.LENGTH_SHORT).show();
    */
    return NOERROR;
}

Int64 TracingControllerAndroid::NativeInit()
{
}

ECode TracingControllerAndroid::NativeDestroy(
    /* [in] */ Int64 nativeTracingControllerAndroid)
{
    return NOERROR;
}

Boolean TracingControllerAndroid::NativeStartTracing(
    /* [in] */ Int64 nativeTracingControllerAndroid,
    /* [in] */ String categories,
    /* [in] */ Boolean recordContinuously)
{
}

ECode TracingControllerAndroid::NativeStopTracing(
    /* [in] */ Int64 nativeTracingControllerAndroid,
    /* [in] */ String filename)
{
    return NOERROR;
}

Boolean TracingControllerAndroid::NativeGetKnownCategoryGroupsAsync(
    /* [in] */ Int64 nativeTracingControllerAndroid)
{
}

String TracingControllerAndroid::NativeGetDefaultCategories()
{
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


