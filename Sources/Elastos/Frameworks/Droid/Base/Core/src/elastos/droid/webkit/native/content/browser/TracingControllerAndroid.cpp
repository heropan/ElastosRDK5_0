
#include "elastos/droid/webkit/native/content/browser/TracingControllerAndroid.h"
#include "elastos/droid/text/TextUtils.h"
// TODO #include "elastos/droid/os/CEnvironment.h"
// TODO #include "elastos/droid/widget/CToastHelper.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::Content::EIID_IBroadcastReceiver;
// TODO using Elastos::Droid::Os::CEnvironment;
using Elastos::Droid::Text::TextUtils;
// TODO using Elastos::Droid::Widget::CToastHelper;
using Elastos::Droid::Widget::IToastHelper;
// TODO using Elastos::IO::CFile;
// TODO using Elastos::Text::CSimpleDateFormat
using Elastos::Text::ISimpleDateFormat;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//          TracingControllerAndroid::TracingBroadcastReceiver
//=====================================================================

TracingControllerAndroid::TracingBroadcastReceiver::TracingBroadcastReceiver(
    /* [in] */ TracingControllerAndroid* owner)
    : mOwner(owner)
{
}

ECode TracingControllerAndroid::TracingBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    if (action.EndWith(ACTION_START)) {
        String categories;
        intent->GetStringExtra(CATEGORIES_EXTRA, &categories);
        if (TextUtils::IsEmpty(categories)) {
            categories = mOwner->NativeGetDefaultCategories();
        }
        else {
            assert(0);
            // TODO
            // categories = categories.ReplaceFirst(
            //         DEFAULT_CHROME_CATEGORIES_PLACE_HOLDER, mOwner->NativeGetDefaultCategories());
        }
        String strExtra;
        intent->GetStringExtra(RECORD_CONTINUOUSLY_EXTRA, &strExtra);
        Boolean recordContinuously = strExtra != NULL;
        String filename;
        intent->GetStringExtra(FILE_EXTRA, &filename);
        if (filename != NULL) {
            mOwner->StartTracing(filename, TRUE, categories, recordContinuously);
        }
        else {
            mOwner->StartTracing(TRUE, categories, recordContinuously);
        }
    }
    else if (action.EndWith(ACTION_STOP)) {
        mOwner->StopTracing();
    }
    else if (action.EndWith(ACTION_LIST_CATEGORIES)) {
        mOwner->GetCategoryGroups();
    }
    else {
        String log("Unexpected intent: ");
        String str;
        intent->ToString(&str);
        log += str;
        Slogger::E(TAG, str);
    }

    return NOERROR;
}

//=====================================================================
//            TracingControllerAndroid::TracingIntentFilter
//=====================================================================

TracingControllerAndroid::TracingIntentFilter::TracingIntentFilter(
    /* [in] */ IContext* context)
{
    String packageName;
    context->GetPackageName(&packageName);
    String start(packageName);
    start += ".";
    start += ACTION_START;
    AddAction(start);
    String stop(packageName);
    stop += ".";
    stop += ACTION_STOP;
    AddAction(stop);
    String categories(packageName);
    categories += ".";
    categories += ACTION_LIST_CATEGORIES;
    AddAction(categories);
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
    : mContext(context)
{
    mBroadcastReceiver = new TracingBroadcastReceiver(this);
    mIntentFilter = new TracingIntentFilter(context);
}

AutoPtr<IBroadcastReceiver> TracingControllerAndroid::GetBroadcastReceiver()
{
    return mBroadcastReceiver;
}

AutoPtr<IIntentFilter> TracingControllerAndroid::GetIntentFilter()
{
    return mIntentFilter;
}

ECode TracingControllerAndroid::RegisterReceiver(
    /* [in] */ IContext* context)
{
    AutoPtr<IIntent> intent;
    return context->RegisterReceiver(GetBroadcastReceiver(), GetIntentFilter(), (IIntent**)&intent);
}

ECode TracingControllerAndroid::UnregisterReceiver(
    /* [in] */ IContext* context)
{
    return context->UnregisterReceiver(GetBroadcastReceiver());
}

Boolean TracingControllerAndroid::IsTracing()
{
    return mIsTracing;
}

String TracingControllerAndroid::GetOutputPath()
{
    return mFilename;
}

Boolean TracingControllerAndroid::StartTracing(
    /* [in] */ Boolean showToasts,
    /* [in] */ const String& categories,
    /* [in] */ Boolean recordContinuously)
{
    mShowToasts = showToasts;

    String filePath = GenerateTracingFilePath();
    if (filePath == NULL) {
        String str;
        assert(0);
        // TODO
        // mContext->GetString(R::string::profiler_no_storage_toast, &str);
        LogAndToastError(str);
    }

    return StartTracing(filePath, showToasts, categories, recordContinuously);
}

Boolean TracingControllerAndroid::StartTracing(
    /* [in] */ const String& filename,
    /* [in] */ Boolean showToasts,
    /* [in] */ const String& categories,
    /* [in] */ Boolean recordContinuously)
{
    mShowToasts = showToasts;
    if (IsTracing()) {
        // Don't need a toast because this shouldn't happen via the UI.
        Slogger::E(TAG, "Received startTracing, but we're already tracing");
        return FALSE;
    }
    // Lazy initialize the native side, to allow construction before the library is loaded.
    InitializeNativeControllerIfNeeded();
    if (!NativeStartTracing(mNativeTracingControllerAndroid, categories,
            recordContinuously)) {
        String str;
        assert(0);
        // TODO
        // mContext->GetString(R::string::profiler_error_toast, &str);
        LogAndToastError(str);
        return FALSE;
    }

    assert(0);
    // TODO
    // LogForProfiler(String.format(PROFILER_STARTED_FMT, categories));
    String toast;
    assert(0);
    // TODO
    // mContext->GetString(R::string::profiler_started_toast, &toast);
    toast += ": ";
    toast += categories;
    ShowToast(toast);
    mFilename = filename;
    mIsTracing = TRUE;

    return TRUE;
}

ECode TracingControllerAndroid::StopTracing()
{
    if (IsTracing()) {
        NativeStopTracing(mNativeTracingControllerAndroid, mFilename);
    }

    return NOERROR;
}

ECode TracingControllerAndroid::GetCategoryGroups()
{
    // Lazy initialize the native side, to allow construction before the library is loaded.
    InitializeNativeControllerIfNeeded();
    if (!NativeGetKnownCategoryGroupsAsync(mNativeTracingControllerAndroid)) {
        Slogger::E(TAG, "Unable to fetch tracing record groups list.");
    }

    return NOERROR;
}

ECode TracingControllerAndroid::OnTracingStopped()
{
    if (!IsTracing()) {
        // Don't need a toast because this shouldn't happen via the UI.
        Slogger::E(TAG, "Received onTracingStopped, but we aren't tracing");
        return NOERROR;
    }

    assert(0);
    // TODO
    // LogForProfiler(String.format(PROFILER_FINISHED_FMT, mFilename));
    String str;
    assert(0);
    // TODO
    // mContext->GetString(R::string::profiler_stopped_toast, mFilename, &str);
    ShowToast(str);
    mIsTracing = FALSE;
    mFilename = NULL;

    return NOERROR;
}

ECode TracingControllerAndroid::Finalize()
{
    if (mNativeTracingControllerAndroid != 0) {
        NativeDestroy(mNativeTracingControllerAndroid);
        mNativeTracingControllerAndroid = 0;
    }

    return NOERROR;
}

String TracingControllerAndroid::GenerateTracingFilePath()
{
    AutoPtr<IEnvironment> env;
    assert(0);
    // TODO
    // CEnvironment::AcquireSingleton((IEnvironment**)&env);
    String state;
    env->GetExternalStorageState(&state);
    if (!IEnvironment::MEDIA_MOUNTED.Equals(state)) {
        return String(NULL);
    }

    // Generate a hopefully-unique filename using the UTC timestamp.
    // (Not a huge problem if it isn't unique, we'll just append more data.)
    AutoPtr<ISimpleDateFormat> formatter;
    assert(0);
    // TODO
    // CSimpleDateFormat::New(
    //         String("yyyy-MM-dd-HHmmss"), ILocale::US,
    //         (ISimpleDateFormat**)&formatter);
    // formatter.setTimeZone(TimeZone.getTimeZone("UTC"));
    AutoPtr<IFile> dir;
    env->GetExternalStoragePublicDirectory(
            IEnvironment::DIRECTORY_DOWNLOADS, (IFile**)&dir);
    AutoPtr<IFile> file;
    AutoPtr<IDate> date;
    assert(0);
    // TODO
    // CDate::New((IDate**)&date);
    String formatStr;
    assert(0);
    // TODO
    // formatter->Format(date, &formatStr);
    String str("chrome-profile-results-");
    str += formatStr;
    assert(0);
    // TODO
    // CFile::New(
    //         dir,  str, (IFile**)&file);
    String path;
    file->GetPath(&path);
    return path;
}

ECode TracingControllerAndroid::InitializeNativeControllerIfNeeded()
{
    if (mNativeTracingControllerAndroid == 0) {
        mNativeTracingControllerAndroid = NativeInit();
    }

    return NOERROR;
}

ECode TracingControllerAndroid::LogAndToastError(
    /* [in] */ const String& str)
{
    Slogger::E(TAG, str);
    if (mShowToasts) {
        AutoPtr<IToastHelper> helper;
        assert(0);
        // TODO
        // CToastHelper::AcquireSingleton((IToastHelper**)&helper);
        AutoPtr<IToast> toast;
        assert(0);
        // TODO
        // helper->MakeText(mContext, str, IToast::LENGTH_SHORT, (IToast**)&toast);
        toast->Show();
    }

    return NOERROR;
}

ECode TracingControllerAndroid::LogForProfiler(
    /* [in] */ const String& str)
{
    Slogger::I(TAG, str);
    return NOERROR;
}

ECode TracingControllerAndroid::ShowToast(
    /* [in] */ const String& str)
{
    if (mShowToasts) {
        AutoPtr<IToastHelper> helper;
        assert(0);
        // TODO
        // CToastHelper::AcquireSingleton((IToastHelper**)&helper);
        AutoPtr<IToast> toast;
        assert(0);
        // TODO
        // helper->MakeText(mContext, str, IToast::LENGTH_SHORT, (IToast**)&toast)
        toast->Show();
    }

    return NOERROR;
}

Int64 TracingControllerAndroid::NativeInit()
{
    return 0;
}

ECode TracingControllerAndroid::NativeDestroy(
    /* [in] */ Int64 nativeTracingControllerAndroid)
{
    return NOERROR;
}

Boolean TracingControllerAndroid::NativeStartTracing(
    /* [in] */ Int64 nativeTracingControllerAndroid,
    /* [in] */ const String& categories,
    /* [in] */ Boolean recordContinuously)
{
    return FALSE;
}

ECode TracingControllerAndroid::NativeStopTracing(
    /* [in] */ Int64 nativeTracingControllerAndroid,
    /* [in] */ const String& filename)
{
    return NOERROR;
}

Boolean TracingControllerAndroid::NativeGetKnownCategoryGroupsAsync(
    /* [in] */ Int64 nativeTracingControllerAndroid)
{
    return FALSE;
}

String TracingControllerAndroid::NativeGetDefaultCategories()
{
    return String("");
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
