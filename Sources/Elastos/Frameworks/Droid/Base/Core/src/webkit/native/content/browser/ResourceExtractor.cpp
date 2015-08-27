// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                ResourceExtractor::InnerFilenameFilter
//=====================================================================
ResourceExtractor::InnerFilenameFilter::InnerFilenameFilter(
    /* [in] */ ResourceExtractor* owner)
    : mOwner(owner)
{
    /*
    mOwner = owner;
    */
}

Boolean ResourceExtractor::InnerFilenameFilter::Accept(
    /* [in] */ IFile* dir,
    /* [in] */ String name)
{
    /*
    return name.startsWith(TIMESTAMP_PREFIX);
    */
    return FALSE;
}

//=====================================================================
//                    ResourceExtractor::ExtractTask
//=====================================================================
const Int32 ResourceExtractor::ExtractTask::BUFFER_SIZE;

ResourceExtractor::ExtractTask::ExtractTask()
{
}

AutoPtr<Void> ResourceExtractor::ExtractTask::DoInBackground(
    /* [in] */  Void)
{
    /*
    final File outputDir = getOutputDir();
    if (!outputDir.exists() && !outputDir.mkdirs()) {
        Log.e(LOGTAG, "Unable to create pak resources directory!");
        return null;
    }


    String timestampFile = checkPakTimestamp(outputDir);
    if (timestampFile != null) {
        deleteFiles();
    }


    SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(mContext);
    HashSet<String> filenames = (HashSet<String>) prefs.getStringSet(
            PAK_FILENAMES, new HashSet<String>());
    String currentLocale = LocalizationUtils.getDefaultLocale();
    String currentLanguage = currentLocale.split("-", 2)[0];


    if (prefs.getString(LAST_LANGUAGE, "").equals(currentLanguage)
            &&  filenames.size() >= sMandatoryPaks.length) {
        boolean filesPresent = true;
        for (String file : filenames) {
            if (!new File(outputDir, file).exists()) {
                filesPresent = false;
                break;
            }
        }
        if (filesPresent) return null;
    } else {
        prefs.edit().putString(LAST_LANGUAGE, currentLanguage).apply();
    }


    StringBuilder p = new StringBuilder();
    for (String mandatoryPak : sMandatoryPaks) {
        if (p.length() > 0) p.append('|');
        p.append("\\Q" + mandatoryPak + "\\E");
    }


    if (sExtractImplicitLocalePak) {
        if (p.length() > 0) p.append('|');
        // As well as the minimum required set of .paks above, we'll also add all .paks that
        // we have for the user's currently selected language.


        p.append(currentLanguage);
        p.append("(-\\w+)?\\.pak");
    }


    Pattern paksToInstall = Pattern.compile(p.toString());


    AssetManager manager = mContext.getResources().getAssets();
    try {
        // Loop through every asset file that we have in the APK, and look for the
        // ones that we need to extract by trying to match the Patterns that we
        // created above.
        byte[] buffer = null;
        String[] files = manager.list("");
        for (String file : files) {
            if (!paksToInstall.matcher(file).matches()) {
                continue;
            }
            boolean isICUData = file.equals(ICU_DATA_FILENAME);
            File output = new File(isICUData ? getAppDataDir() : outputDir, file);
            if (output.exists()) {
                continue;
            }


            InputStream is = null;
            OutputStream os = null;
            try {
                is = manager.open(file);
                os = new FileOutputStream(output);
                Log.i(LOGTAG, "Extracting resource " + file);
                if (buffer == null) {
                    buffer = new byte[BUFFER_SIZE];
                }


                int count = 0;
                while ((count = is.read(buffer, 0, BUFFER_SIZE)) != -1) {
                    os.write(buffer, 0, count);
                }
                os.flush();


                // Ensure something reasonable was written.
                if (output.length() == 0) {
                    throw new IOException(file + " extracted with 0 length!");
                }


                if (!isICUData) {
                    filenames.add(file);
                } else {
                    // icudata needs to be accessed by a renderer process.
                    output.setReadable(true, false);
                }
            } finally {
                try {
                    if (is != null) {
                        is.close();
                    }
                } finally {
                    if (os != null) {
                        os.close();
                    }
                }
            }
        }
    } catch (IOException e) {
        // TODO(benm): See crbug/152413.
        // Try to recover here, can we try again after deleting files instead of
        // returning null? It might be useful to gather UMA here too to track if
        // this happens with regularity.
        Log.w(LOGTAG, "Exception unpacking required pak resources: " + e.getMessage());
        deleteFiles();
        return null;
    }


    // Finished, write out a timestamp file if we need to.


    if (timestampFile != null) {
        try {
            new File(outputDir, timestampFile).createNewFile();
        } catch (IOException e) {
            // Worst case we don't write a timestamp, so we'll re-extract the resource
            // paks next start up.
            Log.w(LOGTAG, "Failed to write resource pak timestamp!");
        }
    }
    // TODO(yusufo): Figure out why remove is required here.
    prefs.edit().remove(PAK_FILENAMES).apply();
    prefs.edit().putStringSet(PAK_FILENAMES, filenames).apply();
    return null;
    */
    return AutoPtr<Void>(this);
}

String ResourceExtractor::ExtractTask::CheckPakTimestamp(
    /* [in] */ IFile* outputDir)
{
    /*
    final String TIMESTAMP_PREFIX = "pak_timestamp-";
    PackageManager pm = mContext.getPackageManager();
    PackageInfo pi = null;


    try {
        pi = pm.getPackageInfo(mContext.getPackageName(), 0);
    } catch (PackageManager.NameNotFoundException e) {
        return TIMESTAMP_PREFIX;
    }


    if (pi == null) {
        return TIMESTAMP_PREFIX;
    }


    String expectedTimestamp = TIMESTAMP_PREFIX + pi.versionCode + "-" + pi.lastUpdateTime;


    String[] timestamps = outputDir.list(// [wuweizuo auto add note here: new InnerFilenameFilter(this)] new FilenameFilter() {
        @Override
        public boolean accept(File dir, String name) {
            return name.startsWith(TIMESTAMP_PREFIX);
        }
    });


    if (timestamps.length != 1) {
        // If there's no timestamp, nuke to be safe as we can't tell the age of the files.
        // If there's multiple timestamps, something's gone wrong so nuke.
        return expectedTimestamp;
    }


    if (!expectedTimestamp.equals(timestamps[0])) {
        return expectedTimestamp;
    }


    // timestamp file is already up-to date.
    return null;
    */
    return String("");
}

//=====================================================================
//                          ResourceExtractor
//=====================================================================
const String ResourceExtractor::LOGTAG("ResourceExtractor");
const String ResourceExtractor::LAST_LANGUAGE("Last language");
const String ResourceExtractor::PAK_FILENAMES("Pak filenames");
const String ResourceExtractor::ICU_DATA_FILENAME("icudtl.dat");
AutoPtr< ArrayOf<String> > ResourceExtractor::sMandatoryPaks = null;
Boolean ResourceExtractor::sExtractImplicitLocalePak = true;
AutoPtr<ResourceExtractor> ResourceExtractor::sInstance;

AutoPtr<ResourceExtractor> ResourceExtractor::Get(
    /* [in] */ IContext* context)
{
    /*
    if (sInstance == null) {
        sInstance = new ResourceExtractor(context);
    }
    return sInstance;
    */
    return AutoPtr<ResourceExtractor>(this);
}

ECode ResourceExtractor::SetMandatoryPaksToExtract(
    /* [in] */  String)
{
    /*
    assert (sInstance == null || sInstance.mExtractTask == null)
            : "Must be called before startExtractingResources is called";
    sMandatoryPaks = mandatoryPaks;
    */
    return NOERROR;
}

ECode ResourceExtractor::SetExtractImplicitLocaleForTesting(
    /* [in] */ Boolean extract)
{
    /*
    assert (sInstance == null || sInstance.mExtractTask == null)
            : "Must be called before startExtractingResources is called";
    sExtractImplicitLocalePak = extract;
    */
    return NOERROR;
}

ECode ResourceExtractor::WaitForCompletion()
{
    /*
    if (shouldSkipPakExtraction()) {
        return;
    }


    assert mExtractTask != null;


    try {
        mExtractTask.get();
    } catch (CancellationException e) {
        // Don't leave the files in an inconsistent state.
        deleteFiles();
    } catch (ExecutionException e2) {
        deleteFiles();
    } catch (InterruptedException e3) {
        deleteFiles();
    }
    */
    return NOERROR;
}

ECode ResourceExtractor::StartExtractingResources()
{
    /*
    if (mExtractTask != null) {
        return;
    }


    if (shouldSkipPakExtraction()) {
        return;
    }


    mExtractTask = new ExtractTask();
    mExtractTask.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    */
    return NOERROR;
}

ResourceExtractor::ResourceExtractor(
    /* [in] */ IContext* context)
{
    /*
    mContext = context.getApplicationContext();
    */
}

AutoPtr<IFile> ResourceExtractor::GetAppDataDir()
{
    /*
    return new File(PathUtils.getDataDirectory(mContext));
    */
    return AutoPtr<IFile>(this);
}

AutoPtr<IFile> ResourceExtractor::GetOutputDir()
{
    /*
    return new File(getAppDataDir(), "paks");
    */
    return AutoPtr<IFile>(this);
}

ECode ResourceExtractor::DeleteFiles()
{
    /*
    File icudata = new File(getAppDataDir(), ICU_DATA_FILENAME);
    if (icudata.exists() && !icudata.delete()) {
        Log.e(LOGTAG, "Unable to remove the icudata " + icudata.getName());
    }
    File dir = getOutputDir();
    if (dir.exists()) {
        File[] files = dir.listFiles();
        for (File file : files) {
            if (!file.delete()) {
                Log.e(LOGTAG, "Unable to remove existing resource " + file.getName());
            }
        }
    }
    */
    return NOERROR;
}

Boolean ResourceExtractor::ShouldSkipPakExtraction()
{
    /*
    // Must call setMandatoryPaksToExtract before beginning resource extraction.
    assert sMandatoryPaks != null;
    return sMandatoryPaks.length == 1 && "".equals(sMandatoryPaks[0]);
    */
    return FALSE;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


