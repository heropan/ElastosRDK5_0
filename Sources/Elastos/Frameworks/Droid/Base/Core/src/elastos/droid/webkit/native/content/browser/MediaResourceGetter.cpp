// wuweizuo automatic build .cpp file from .java file.

#include "MediaResourceGetter.h"

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
    // ==================before translated======================
    // mDurationInMilliseconds = durationInMilliseconds;
    // mWidth = width;
    // mHeight = height;
    // mSuccess = success;
}

Int32 MediaResourceGetter::MediaMetadata::GetDurationInMilliseconds()
{
    // ==================before translated======================
    // return mDurationInMilliseconds;
    assert(0);
    return 0;
}

Int32 MediaResourceGetter::MediaMetadata::GetWidth()
{
    // ==================before translated======================
    // return mWidth;
    assert(0);
    return 0;
}

Int32 MediaResourceGetter::MediaMetadata::GetHeight()
{
    // ==================before translated======================
    // return mHeight;
    assert(0);
    return 0;
}

Boolean MediaResourceGetter::MediaMetadata::IsSuccess()
{
    // ==================before translated======================
    // return mSuccess;
    assert(0);
    return FALSE;
}

String MediaResourceGetter::MediaMetadata::ToString()
{
    // ==================before translated======================
    // return "MediaMetadata["
    //         + "durationInMilliseconds=" + mDurationInMilliseconds
    //         + ", width=" + mWidth
    //         + ", height=" + mHeight
    //         + ", success=" + mSuccess
    //         + "]";
    assert(0);
    return String("");
}

Int32 MediaResourceGetter::MediaMetadata::HashCode()
{
    // ==================before translated======================
    // final int prime = 31;
    // int result = 1;
    // result = prime * result + mDurationInMilliseconds;
    // result = prime * result + mHeight;
    // result = prime * result + (mSuccess ? 1231 : 1237);
    // result = prime * result + mWidth;
    // return result;
    assert(0);
    return 0;
}

Boolean MediaResourceGetter::MediaMetadata::Equals(
    /* [in] */ Object* obj)
{
    // ==================before translated======================
    // if (this == obj)
    //     return true;
    // if (obj == null)
    //     return false;
    // if (getClass() != obj.getClass())
    //     return false;
    // MediaMetadata other = (MediaMetadata)obj;
    // if (mDurationInMilliseconds != other.mDurationInMilliseconds)
    //     return false;
    // if (mHeight != other.mHeight)
    //     return false;
    // if (mSuccess != other.mSuccess)
    //     return false;
    // if (mWidth != other.mWidth)
    //     return false;
    // return true;
    assert(0);
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
    // ==================before translated======================
    // if (!androidDeviceOk(android.os.Build.MODEL, android.os.Build.VERSION.SDK_INT)) {
    //     return EMPTY_METADATA;
    // }
    //
    // configure(fd, offset, length);
    // return doExtractMetadata();
    assert(0);
    AutoPtr<MediaMetadata> empty;
    return empty;
}

AutoPtr<MediaMetadata> MediaResourceGetter::Extract(
    /* [in] */ const IContext* context,
    /* [in] */ const String& url,
    /* [in] */ const String& cookies,
    /* [in] */ const String& userAgent)
{
    // ==================before translated======================
    // if (!androidDeviceOk(android.os.Build.MODEL, android.os.Build.VERSION.SDK_INT)) {
    //     return EMPTY_METADATA;
    // }
    //
    // if (!configure(context, url, cookies, userAgent)) {
    //     Log.e(TAG, "Unable to configure metadata extractor");
    //     return EMPTY_METADATA;
    // }
    // return doExtractMetadata();
    assert(0);
    AutoPtr<MediaMetadata> empty;
    return empty;
}

Boolean MediaResourceGetter::Configure(
    /* [in] */ IContext* context,
    /* [in] */ const String& url,
    /* [in] */ const String& cookies,
    /* [in] */ const String& userAgent)
{
    // ==================before translated======================
    // URI uri;
    // try {
    //     uri = URI.create(url);
    // } catch (IllegalArgumentException  e) {
    //     Log.e(TAG, "Cannot parse uri.", e);
    //     return false;
    // }
    // String scheme = uri.getScheme();
    // if (scheme == null || scheme.equals("file")) {
    //     File file = uriToFile(uri.getPath());
    //     if (!file.exists()) {
    //         Log.e(TAG, "File does not exist.");
    //         return false;
    //     }
    //     if (!filePathAcceptable(file)) {
    //         Log.e(TAG, "Refusing to read from unsafe file location.");
    //         return false;
    //     }
    //     try {
    //         configure(file.getAbsolutePath());
    //         return true;
    //     } catch (RuntimeException e) {
    //         Log.e(TAG, "Error configuring data source", e);
    //         return false;
    //     }
    // } else {
    //     final String host = uri.getHost();
    //     if (!isLoopbackAddress(host) && !isNetworkReliable(context)) {
    //         Log.w(TAG, "non-file URI can't be read due to unsuitable network conditions");
    //         return false;
    //     }
    //     Map<String, String> headersMap = new HashMap<String, String>();
    //     if (!TextUtils.isEmpty(cookies)) {
    //         headersMap.put("Cookie", cookies);
    //     }
    //     if (!TextUtils.isEmpty(userAgent)) {
    //         headersMap.put("User-Agent", userAgent);
    //     }
    //     try {
    //         configure(url, headersMap);
    //         return true;
    //     } catch (RuntimeException e) {
    //         Log.e(TAG, "Error configuring data source", e);
    //         return false;
    //     }
    // }
    assert(0);
    return FALSE;
}

Boolean MediaResourceGetter::IsNetworkReliable(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // if (context.checkCallingOrSelfPermission(
    //         android.Manifest.permission.ACCESS_NETWORK_STATE) !=
    //         PackageManager.PERMISSION_GRANTED) {
    //     Log.w(TAG, "permission denied to access network state");
    //     return false;
    // }
    //
    // Integer networkType = getNetworkType(context);
    // if (networkType == null) {
    //     return false;
    // }
    // switch (networkType.intValue()) {
    //     case ConnectivityManager.TYPE_ETHERNET:
    //     case ConnectivityManager.TYPE_WIFI:
    //         Log.d(TAG, "ethernet/wifi connection detected");
    //         return true;
    //     case ConnectivityManager.TYPE_WIMAX:
    //     case ConnectivityManager.TYPE_MOBILE:
    //     default:
    //         Log.d(TAG, "no ethernet/wifi connection detected");
    //         return false;
    // }
    assert(0);
    return FALSE;
}

Boolean MediaResourceGetter::FilePathAcceptable(
    /* [in] */ IFile* file)
{
    // ==================before translated======================
    // final String path;
    // try {
    //     path = file.getCanonicalPath();
    // } catch (IOException e) {
    //     // Canonicalization has failed. Assume malicious, give up.
    //     Log.w(TAG, "canonicalization of file path failed");
    //     return false;
    // }
    // // In order to properly match the roots we must also canonicalize the
    // // well-known paths we are matching against. If we don't, then we can
    // // get unusual results in testing systems or possibly on rooted devices.
    // // Note that canonicalized directory paths always end with '/'.
    // List<String> acceptablePaths = canonicalize(getRawAcceptableDirectories());
    // acceptablePaths.add(getExternalStorageDirectory());
    // Log.d(TAG, "canonicalized file path: " + path);
    // for (String acceptablePath : acceptablePaths) {
    //     if (path.startsWith(acceptablePath)) {
    //         return true;
    //     }
    // }
    // return false;
    assert(0);
    return FALSE;
}

Boolean MediaResourceGetter::AndroidDeviceOk(
    /* [in] */ const String& model,
    /* [in] */ const Int32& sdkVersion)
{
    // ==================before translated======================
    // return !("GT-I9100".contentEquals(model) &&
    //          sdkVersion < android.os.Build.VERSION_CODES.JELLY_BEAN);
    assert(0);
    return FALSE;
}

AutoPtr<IFile> MediaResourceGetter::UriToFile(
    /* [in] */ const String& path)
{
    // ==================before translated======================
    // return new File(path);
    assert(0);
    AutoPtr<IFile> empty;
    return empty;
}

AutoPtr<Integer> MediaResourceGetter::GetNetworkType(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // // TODO(qinmin): use ConnectionTypeObserver to listen to the network type change.
    // ConnectivityManager mConnectivityManager =
    //         (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
    // if (mConnectivityManager == null) {
    //     Log.w(TAG, "no connectivity manager available");
    //     return null;
    // }
    // NetworkInfo info = mConnectivityManager.getActiveNetworkInfo();
    // if (info == null) {
    //     Log.d(TAG, "no active network");
    //     return null;
    // }
    // return info.getType();
    assert(0);
    AutoPtr<Integer> empty;
    return empty;
}

String MediaResourceGetter::GetExternalStorageDirectory()
{
    // ==================before translated======================
    // return PathUtils.getExternalStorageDirectory();
    assert(0);
    return String("");
}

ECode MediaResourceGetter::Configure(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 length)
{
    // ==================before translated======================
    // ParcelFileDescriptor parcelFd = ParcelFileDescriptor.adoptFd(fd);
    // try {
    //     mRetriever.setDataSource(parcelFd.getFileDescriptor(),
    //             offset, length);
    // } finally {
    //     try {
    //         parcelFd.close();
    //     } catch (IOException e) {
    //         Log.e(TAG, "Failed to close file descriptor: " + e);
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode MediaResourceGetter::Configure(
    /* [in] */ const String& url,
    /* [in] */ IMap<String, String>* headers)
{
    VALIDATE_NOT_NULL(headers);
    // ==================before translated======================
    // mRetriever.setDataSource(url, headers);
    assert(0);
    return NOERROR;
}

ECode MediaResourceGetter::Configure(
    /* [in] */ const String& path)
{
    // ==================before translated======================
    // mRetriever.setDataSource(path);
    assert(0);
    return NOERROR;
}

String MediaResourceGetter::ExtractMetadata(
    /* [in] */ Int32 key)
{
    // ==================before translated======================
    // return mRetriever.extractMetadata(key);
    assert(0);
    return String("");
}

AutoPtr<MediaMetadata> MediaResourceGetter::ExtractMediaMetadata(
    /* [in] */ const IContext* context,
    /* [in] */ const String& url,
    /* [in] */ const String& cookies,
    /* [in] */ const String& userAgent)
{
    // ==================before translated======================
    // return new MediaResourceGetter().extract(
    //         context, url, cookies, userAgent);
    assert(0);
    AutoPtr<MediaMetadata> empty;
    return empty;
}

AutoPtr<MediaMetadata> MediaResourceGetter::ExtractMediaMetadataFromFd(
    /* [in] */ Int32 fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 length)
{
    // ==================before translated======================
    // return new MediaResourceGetter().extract(fd, offset, length);
    assert(0);
    AutoPtr<MediaMetadata> empty;
    return empty;
}

AutoPtr<MediaMetadata> MediaResourceGetter::DoExtractMetadata()
{
    // ==================before translated======================
    // try {
    //     String durationString = extractMetadata(
    //             MediaMetadataRetriever.METADATA_KEY_DURATION);
    //     if (durationString == null) {
    //         Log.w(TAG, "missing duration metadata");
    //         return EMPTY_METADATA;
    //     }
    //
    //     int durationMillis = 0;
    //     try {
    //         durationMillis = Integer.parseInt(durationString);
    //     } catch (NumberFormatException e) {
    //         Log.w(TAG, "non-numeric duration: " + durationString);
    //         return EMPTY_METADATA;
    //     }
    //
    //     int width = 0;
    //     int height = 0;
    //     boolean hasVideo = "yes".equals(extractMetadata(
    //             MediaMetadataRetriever.METADATA_KEY_HAS_VIDEO));
    //     Log.d(TAG, (hasVideo ? "resource has video" : "resource doesn't have video"));
    //     if (hasVideo) {
    //         String widthString = extractMetadata(
    //                 MediaMetadataRetriever.METADATA_KEY_VIDEO_WIDTH);
    //         if (widthString == null) {
    //             Log.w(TAG, "missing video width metadata");
    //             return EMPTY_METADATA;
    //         }
    //         try {
    //             width = Integer.parseInt(widthString);
    //         } catch (NumberFormatException e) {
    //             Log.w(TAG, "non-numeric width: " + widthString);
    //             return EMPTY_METADATA;
    //         }
    //
    //         String heightString = extractMetadata(
    //                 MediaMetadataRetriever.METADATA_KEY_VIDEO_HEIGHT);
    //         if (heightString == null) {
    //             Log.w(TAG, "missing video height metadata");
    //             return EMPTY_METADATA;
    //         }
    //         try {
    //             height = Integer.parseInt(heightString);
    //         } catch (NumberFormatException e) {
    //             Log.w(TAG, "non-numeric height: " + heightString);
    //             return EMPTY_METADATA;
    //         }
    //     }
    //     MediaMetadata result = new MediaMetadata(durationMillis, width, height, true);
    //     Log.d(TAG, "extracted valid metadata: " + result.toString());
    //     return result;
    // } catch (RuntimeException e) {
    //     Log.e(TAG, "Unable to extract medata", e);
    //     return EMPTY_METADATA;
    // }
    assert(0);
    AutoPtr<MediaMetadata> empty;
    return empty;
}

Boolean MediaResourceGetter::IsLoopbackAddress(
    /* [in] */ const String& host)
{
    // ==================before translated======================
    // return host != null && (host.equalsIgnoreCase("localhost")  // typical hostname
    //         || host.equals("127.0.0.1")  // typical IP v4 expression
    //         || host.equals("[::1]"));  // typical IP v6 expression
    assert(0);
    return FALSE;
}

AutoPtr< IList<String> > MediaResourceGetter::GetRawAcceptableDirectories()
{
    // ==================before translated======================
    // List<String> result = new ArrayList<String>();
    // result.add("/mnt/sdcard/");
    // result.add("/sdcard/");
    // return result;
    assert(0);
    AutoPtr< IList<String> > empty;
    return empty;
}

AutoPtr< IList<String> > MediaResourceGetter::Canonicalize(
    /* [in] */ IList<String>* paths)
{
    // ==================before translated======================
    // List<String> result = new ArrayList<String>(paths.size());
    // try {
    //     for (String path : paths) {
    //         result.add(new File(path).getCanonicalPath());
    //     }
    //     return result;
    // } catch (IOException e) {
    //     // Canonicalization has failed. Assume malicious, give up.
    //     Log.w(TAG, "canonicalization of file path failed");
    // }
    // return result;
    assert(0);
    AutoPtr< IList<String> > empty;
    return empty;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


