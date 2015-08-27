// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                  MediaResourceGetter::MediaMetadata
//=====================================================================
MediaResourceGetter::MediaMetadata::MediaMetadata(
    /* [in] */ Int32 durationInMilliseconds,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean success)
{
    /*
    mDurationInMilliseconds = durationInMilliseconds;
    mWidth = width;
    mHeight = height;
    mSuccess = success;
    */
}

Int32 MediaResourceGetter::MediaMetadata::GetDurationInMilliseconds()
{
    /*
    return mDurationInMilliseconds;
    */
    return 0;
}

Int32 MediaResourceGetter::MediaMetadata::GetWidth()
{
    /*
    return mWidth;
    */
    return 0;
}

Int32 MediaResourceGetter::MediaMetadata::GetHeight()
{
    /*
    return mHeight;
    */
    return 0;
}

Boolean MediaResourceGetter::MediaMetadata::IsSuccess()
{
    /*
    return mSuccess;
    */
    return FALSE;
}

String MediaResourceGetter::MediaMetadata::ToString()
{
    /*
    return "MediaMetadata["
            + "durationInMilliseconds=" + mDurationInMilliseconds
            + ", width=" + mWidth
            + ", height=" + mHeight
            + ", success=" + mSuccess
            + "]";
    */
    return String("");
}

Int32 MediaResourceGetter::MediaMetadata::HashCode()
{
    /*
    final int prime = 31;
    int result = 1;
    result = prime * result + mDurationInMilliseconds;
    result = prime * result + mHeight;
    result = prime * result + (mSuccess ? 1231 : 1237);
    result = prime * result + mWidth;
    return result;
    */
    return 0;
}

Boolean MediaResourceGetter::MediaMetadata::Equals(
    /* [in] */ Object* obj)
{
    /*
    if (this == obj)
        return true;
    if (obj == null)
        return false;
    if (getClass() != obj.getClass())
        return false;
    MediaMetadata other = (MediaMetadata)obj;
    if (mDurationInMilliseconds != other.mDurationInMilliseconds)
        return false;
    if (mHeight != other.mHeight)
        return false;
    if (mSuccess != other.mSuccess)
        return false;
    if (mWidth != other.mWidth)
        return false;
    return true;
    */
    return FALSE;
}

//=====================================================================
//                         MediaResourceGetter
//=====================================================================
const String MediaResourceGetter::TAG("MediaResourceGetter");

AutoPtr<MediaMetadata> MediaResourceGetter::Extract(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 length)
{
    /*
    if (!androidDeviceOk(android.os.Build.MODEL, android.os.Build.VERSION.SDK_INT)) {
        return EMPTY_METADATA;
    }


    configure(fd, offset, length);
    return doExtractMetadata();
    */
    return AutoPtr<MediaMetadata>(this);
}

const AutoPtr<MediaMetadata> MediaResourceGetter::Extract(
    /* [in] */ IContext* context,
    /* [in] */ final String* final String* url,
    /* [in] */ final String* final String* cookies,
    /* [in] */ final String* final String* userAgent)
{
    /*
    if (!androidDeviceOk(android.os.Build.MODEL, android.os.Build.VERSION.SDK_INT)) {
        return EMPTY_METADATA;
    }


    if (!configure(context, url, cookies, userAgent)) {
        Log.e(TAG, "Unable to configure metadata extractor");
        return EMPTY_METADATA;
    }
    return doExtractMetadata();
    */
    return AutoPtr<MediaMetadata>(this);
}

Boolean MediaResourceGetter::Configure(
    /* [in] */ IContext* context,
    /* [in] */ String url,
    /* [in] */ String cookies,
    /* [in] */ String userAgent)
{
    /*
    URI uri;
    try {
        uri = URI.create(url);
    } catch (IllegalArgumentException  e) {
        Log.e(TAG, "Cannot parse uri.", e);
        return false;
    }
    String scheme = uri.getScheme();
    if (scheme == null || scheme.equals("file")) {
        File file = uriToFile(uri.getPath());
        if (!file.exists()) {
            Log.e(TAG, "File does not exist.");
            return false;
        }
        if (!filePathAcceptable(file)) {
            Log.e(TAG, "Refusing to read from unsafe file location.");
            return false;
        }
        try {
            configure(file.getAbsolutePath());
            return true;
        } catch (RuntimeException e) {
            Log.e(TAG, "Error configuring data source", e);
            return false;
        }
    } else {
        final String host = uri.getHost();
        if (!isLoopbackAddress(host) && !isNetworkReliable(context)) {
            Log.w(TAG, "non-file URI can't be read due to unsuitable network conditions");
            return false;
        }
        Map<String, String> headersMap = new HashMap<String, String>();
        if (!TextUtils.isEmpty(cookies)) {
            headersMap.put("Cookie", cookies);
        }
        if (!TextUtils.isEmpty(userAgent)) {
            headersMap.put("User-Agent", userAgent);
        }
        try {
            configure(url, headersMap);
            return true;
        } catch (RuntimeException e) {
            Log.e(TAG, "Error configuring data source", e);
            return false;
        }
    }
    */
    return FALSE;
}

Boolean MediaResourceGetter::IsNetworkReliable(
    /* [in] */ IContext* context)
{
    /*
    if (context.checkCallingOrSelfPermission(
            android.Manifest.permission.ACCESS_NETWORK_STATE) !=
            PackageManager.PERMISSION_GRANTED) {
        Log.w(TAG, "permission denied to access network state");
        return false;
    }


    Integer networkType = getNetworkType(context);
    if (networkType == null) {
        return false;
    }
    switch (networkType.intValue()) {
        case ConnectivityManager.TYPE_ETHERNET:
        case ConnectivityManager.TYPE_WIFI:
            Log.d(TAG, "ethernet/wifi connection detected");
            return true;
        case ConnectivityManager.TYPE_WIMAX:
        case ConnectivityManager.TYPE_MOBILE:
        default:
            Log.d(TAG, "no ethernet/wifi connection detected");
            return false;
    }
    */
    return FALSE;
}

Boolean MediaResourceGetter::FilePathAcceptable(
    /* [in] */ IFile* file)
{
    /*
    final String path;
    try {
        path = file.getCanonicalPath();
    } catch (IOException e) {
        // Canonicalization has failed. Assume malicious, give up.
        Log.w(TAG, "canonicalization of file path failed");
        return false;
    }
    // In order to properly match the roots we must also canonicalize the
    // well-known paths we are matching against. If we don't, then we can
    // get unusual results in testing systems or possibly on rooted devices.
    // Note that canonicalized directory paths always end with '/'.
    List<String> acceptablePaths = canonicalize(getRawAcceptableDirectories());
    acceptablePaths.add(getExternalStorageDirectory());
    Log.d(TAG, "canonicalized file path: " + path);
    for (String acceptablePath : acceptablePaths) {
        if (path.startsWith(acceptablePath)) {
            return true;
        }
    }
    return false;
    */
    return FALSE;
}

const Boolean MediaResourceGetter::AndroidDeviceOk(
    /* [in] */ String model,
    /* [in] */ final int* final int* sdkVersion)
{
    /*
    return !("GT-I9100".contentEquals(model) &&
             sdkVersion < android.os.Build.VERSION_CODES.JELLY_BEAN);
    */
    return FALSE;
}

AutoPtr<IFile> MediaResourceGetter::UriToFile(
    /* [in] */ String path)
{
    /*
    return new File(path);
    */
    return AutoPtr<IFile>(this);
}

AutoPtr<Integer> MediaResourceGetter::GetNetworkType(
    /* [in] */ IContext* context)
{
    /*
    // TODO(qinmin): use ConnectionTypeObserver to listen to the network type change.
    ConnectivityManager mConnectivityManager =
            (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
    if (mConnectivityManager == null) {
        Log.w(TAG, "no connectivity manager available");
        return null;
    }
    NetworkInfo info = mConnectivityManager.getActiveNetworkInfo();
    if (info == null) {
        Log.d(TAG, "no active network");
        return null;
    }
    return info.getType();
    */
    return AutoPtr<Integer>(this);
}

String MediaResourceGetter::GetExternalStorageDirectory()
{
    /*
    return PathUtils.getExternalStorageDirectory();
    */
    return String("");
}

ECode MediaResourceGetter::Configure(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 length)
{
    /*
    ParcelFileDescriptor parcelFd = ParcelFileDescriptor.adoptFd(fd);
    try {
        mRetriever.setDataSource(parcelFd.getFileDescriptor(),
                offset, length);
    } finally {
        try {
            parcelFd.close();
        } catch (IOException e) {
            Log.e(TAG, "Failed to close file descriptor: " + e);
        }
    }
    */
    return NOERROR;
}

ECode MediaResourceGetter::Configure(
    /* [in] */ String url,
    /* [in] */ IMap<String, String>* headers)
{
    VALIDATE_NOT_NULL(headers);
    /*
    mRetriever.setDataSource(url, headers);
    */
    return NOERROR;
}

ECode MediaResourceGetter::Configure(
    /* [in] */ String path)
{
    /*
    mRetriever.setDataSource(path);
    */
    return NOERROR;
}

String MediaResourceGetter::ExtractMetadata(
    /* [in] */ Int32 key)
{
    /*
    return mRetriever.extractMetadata(key);
    */
    return String("");
}

const AutoPtr<MediaMetadata> MediaResourceGetter::ExtractMediaMetadata(
    /* [in] */ IContext* context,
    /* [in] */ final String* final String* url,
    /* [in] */ final String* final String* cookies,
    /* [in] */ final String* final String* userAgent)
{
    /*
    return new MediaResourceGetter().extract(
            context, url, cookies, userAgent);
    */
    return AutoPtr<MediaMetadata>(this);
}

AutoPtr<MediaMetadata> MediaResourceGetter::ExtractMediaMetadataFromFd(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 length)
{
    /*
    return new MediaResourceGetter().extract(fd, offset, length);
    */
    return AutoPtr<MediaMetadata>(this);
}

AutoPtr<MediaMetadata> MediaResourceGetter::DoExtractMetadata()
{
    /*
    try {
        String durationString = extractMetadata(
                MediaMetadataRetriever.METADATA_KEY_DURATION);
        if (durationString == null) {
            Log.w(TAG, "missing duration metadata");
            return EMPTY_METADATA;
        }


        int durationMillis = 0;
        try {
            durationMillis = Integer.parseInt(durationString);
        } catch (NumberFormatException e) {
            Log.w(TAG, "non-numeric duration: " + durationString);
            return EMPTY_METADATA;
        }


        int width = 0;
        int height = 0;
        boolean hasVideo = "yes".equals(extractMetadata(
                MediaMetadataRetriever.METADATA_KEY_HAS_VIDEO));
        Log.d(TAG, (hasVideo ? "resource has video" : "resource doesn't have video"));
        if (hasVideo) {
            String widthString = extractMetadata(
                    MediaMetadataRetriever.METADATA_KEY_VIDEO_WIDTH);
            if (widthString == null) {
                Log.w(TAG, "missing video width metadata");
                return EMPTY_METADATA;
            }
            try {
                width = Integer.parseInt(widthString);
            } catch (NumberFormatException e) {
                Log.w(TAG, "non-numeric width: " + widthString);
                return EMPTY_METADATA;
            }


            String heightString = extractMetadata(
                    MediaMetadataRetriever.METADATA_KEY_VIDEO_HEIGHT);
            if (heightString == null) {
                Log.w(TAG, "missing video height metadata");
                return EMPTY_METADATA;
            }
            try {
                height = Integer.parseInt(heightString);
            } catch (NumberFormatException e) {
                Log.w(TAG, "non-numeric height: " + heightString);
                return EMPTY_METADATA;
            }
        }
        MediaMetadata result = new MediaMetadata(durationMillis, width, height, true);
        Log.d(TAG, "extracted valid metadata: " + result.toString());
        return result;
    } catch (RuntimeException e) {
        Log.e(TAG, "Unable to extract medata", e);
        return EMPTY_METADATA;
    }
    */
    return AutoPtr<MediaMetadata>(this);
}

Boolean MediaResourceGetter::IsLoopbackAddress(
    /* [in] */ String host)
{
    /*
    return host != null && (host.equalsIgnoreCase("localhost")  // typical hostname
            || host.equals("127.0.0.1")  // typical IP v4 expression
            || host.equals("[::1]"));  // typical IP v6 expression
    */
    return FALSE;
}

AutoPtr< IList<String> > MediaResourceGetter::GetRawAcceptableDirectories()
{
    /*
    List<String> result = new ArrayList<String>();
    result.add("/mnt/sdcard/");
    result.add("/sdcard/");
    return result;
    */
    return AutoPtr< IList<String> >(this);
}

AutoPtr< IList<String> > MediaResourceGetter::Canonicalize(
    /* [in] */ IList<String>* paths)
{
    /*
    List<String> result = new ArrayList<String>(paths.size());
    try {
        for (String path : paths) {
            result.add(new File(path).getCanonicalPath());
        }
        return result;
    } catch (IOException e) {
        // Canonicalization has failed. Assume malicious, give up.
        Log.w(TAG, "canonicalization of file path failed");
    }
    return result;
    */
    return AutoPtr< IList<String> >(this);
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


