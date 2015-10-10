// wuweizuo automatic build .h file from .java file.
// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_RESOURCEEXTRACTOR_H_
#define _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_RESOURCEEXTRACTOR_H_

#include "elastos/droid/ext/frameworkext.h"
#include "os/AsyncTask.h"

using Elastos::IO::IFile;
using Elastos::IO::IFilenameFilter;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::ISharedPreferences;
using Elastos::Droid::Content::Pm::IPackageInfo;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Res::IAssetManager;
using Elastos::Droid::Os::AsyncTask;
// import android.preference.PreferenceManager;
// import android.util.Log;
// import org.chromium.base.PathUtils;
// import org.chromium.ui.base.LocalizationUtils;
// import java.io.File;
// import java.io.FileOutputStream;
// import java.io.FilenameFilter;
// import java.io.IOException;
// import java.io.InputStream;
// import java.io.OutputStream;
// import java.util.HashSet;
// import java.util.concurrent.CancellationException;
// import java.util.concurrent.ExecutionException;
// import java.util.regex.Pattern;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
  * Handles extracting the necessary resources bundled in an APK and moving them to a location on
  * the file system accessible from the native code.
  */
class ResourceExtractor : public Object
{
public:
    class InnerFilenameFilter
        : public Object
        , public IFilenameFilter
    {
    public:
        InnerFilenameFilter(
            /* [in] */ ResourceExtractor* owner);

        // @Override
        CARAPI Accept(
            /* [in] */ IFile* dir,
            /* [in] */ const String& name,
            /* [out] */ Boolean* result);

    private:
        ResourceExtractor* mOwner;
    };

private:
    class ExtractTask
        : public AsyncTask/*<Void, Void, Void>*/
    {
    public:
        ExtractTask();

    protected:
        // @Override
        CARAPI DoInBackground(
          /* [in] */ ArrayOf<IInterface*>* params,
          /* [out] */ IInterface** result);

    private:
        // Looks for a timestamp file on disk that indicates the version of the APK that
        // the resource paks were extracted from. Returns null if a timestamp was found
        // and it indicates that the resources match the current APK. Otherwise returns
        // a String that represents the filename of a timestamp to create.
        // Note that we do this to avoid adding a BroadcastReceiver on
        // android.content.Intent#ACTION_PACKAGE_CHANGED as that causes process churn
        // on (re)installation of *all* APK files.
        CARAPI_(String) CheckPakTimestamp(
            /* [in] */ IFile* outputDir);

    private:
        static const Int32 BUFFER_SIZE = 16 * 1024;
    };

public:
    static CARAPI_(AutoPtr<ResourceExtractor>) Get(
        /* [in] */ IContext* context);

    /**
      * Specifies the .pak files that should be extracted from the APK's asset resources directory
      * and moved to {@link #getOutputDirFromContext(Context)}.
      * @param mandatoryPaks The list of pak files to be loaded. If no pak files are
      *     required, pass a single empty string.
      */
    static CARAPI SetMandatoryPaksToExtract(
        /* [in] */  String);

    /**
      * By default the ResourceExtractor will attempt to extract a pak resource for the users
      * currently specified locale. This behavior can be changed with this function and is
      * only needed by tests.
      * @param extract False if we should not attempt to extract a pak file for
      *         the users currently selected locale and try to extract only the
      *         pak files specified in sMandatoryPaks.
      */
    static CARAPI SetExtractImplicitLocaleForTesting(
        /* [in] */ Boolean extract);

    virtual CARAPI WaitForCompletion();

    /**
      * This will extract the application pak resources in an
      * AsyncTask. Call waitForCompletion() at the point resources
      * are needed to block until the task completes.
      */
    virtual CARAPI StartExtractingResources();

private:
    ResourceExtractor(
        /* [in] */ IContext* context);

    CARAPI_(AutoPtr<IFile>) GetAppDataDir();

    CARAPI_(AutoPtr<IFile>) GetOutputDir();

    /**
      * Pak files (UI strings and other resources) should be updated along with
      * Chrome. A version mismatch can lead to a rather broken user experience.
      * The ICU data (icudtl.dat) is less version-sensitive, but still can
      * lead to malfunction/UX misbehavior. So, we regard failing to update them
      * as an error.
      */
    CARAPI DeleteFiles();

    /**
      * Pak extraction not necessarily required by the embedder; we allow them to skip
      * this process if they call setMandatoryPaksToExtract with a single empty String.
      */
    static CARAPI_(Boolean) ShouldSkipPakExtraction();

private:
    static const String LOGTAG;
    static const String LAST_LANGUAGE;
    static const String PAK_FILENAMES;
    static const String ICU_DATA_FILENAME;
    static AutoPtr< ArrayOf<String> > sMandatoryPaks;
    // By default, we attempt to extract a pak file for the users
    // current device locale. Use setExtractImplicitLocale() to
    // change this behavior.
    static Boolean sExtractImplicitLocalePak;
    /*const*/ AutoPtr<IContext> mContext;
    AutoPtr<ExtractTask> mExtractTask;
    static AutoPtr<ResourceExtractor> sInstance;
};

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_RESOURCEEXTRACTOR_H_
