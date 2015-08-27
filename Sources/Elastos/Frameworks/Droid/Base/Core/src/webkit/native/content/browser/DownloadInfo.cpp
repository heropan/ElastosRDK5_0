// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                        DownloadInfo::Builder
//=====================================================================
AutoPtr<Builder> DownloadInfo::Builder::SetUrl(
    /* [in] */ String url)
{
    /*
    mUrl = url;
    return this;
    */
    return AutoPtr<Builder>(this);
}

AutoPtr<Builder> DownloadInfo::Builder::SetUserAgent(
    /* [in] */ String userAgent)
{
    /*
    mUserAgent = userAgent;
    return this;
    */
    return AutoPtr<Builder>(this);
}

AutoPtr<Builder> DownloadInfo::Builder::SetMimeType(
    /* [in] */ String mimeType)
{
    /*
    mMimeType = mimeType;
    return this;
    */
    return AutoPtr<Builder>(this);
}

AutoPtr<Builder> DownloadInfo::Builder::SetCookie(
    /* [in] */ String cookie)
{
    /*
    mCookie = cookie;
    return this;
    */
    return AutoPtr<Builder>(this);
}

AutoPtr<Builder> DownloadInfo::Builder::SetFileName(
    /* [in] */ String fileName)
{
    /*
    mFileName = fileName;
    return this;
    */
    return AutoPtr<Builder>(this);
}

AutoPtr<Builder> DownloadInfo::Builder::SetDescription(
    /* [in] */ String description)
{
    /*
    mDescription = description;
    return this;
    */
    return AutoPtr<Builder>(this);
}

AutoPtr<Builder> DownloadInfo::Builder::SetFilePath(
    /* [in] */ String filePath)
{
    /*
    mFilePath = filePath;
    return this;
    */
    return AutoPtr<Builder>(this);
}

AutoPtr<Builder> DownloadInfo::Builder::SetReferer(
    /* [in] */ String referer)
{
    /*
    mReferer = referer;
    return this;
    */
    return AutoPtr<Builder>(this);
}

AutoPtr<Builder> DownloadInfo::Builder::SetContentLength(
    /* [in] */ Int64 contentLength)
{
    /*
    mContentLength = contentLength;
    return this;
    */
    return AutoPtr<Builder>(this);
}

AutoPtr<Builder> DownloadInfo::Builder::SetIsGETRequest(
    /* [in] */ Boolean isGETRequest)
{
    /*
    mIsGETRequest = isGETRequest;
    return this;
    */
    return AutoPtr<Builder>(this);
}

AutoPtr<Builder> DownloadInfo::Builder::SetHasDownloadId(
    /* [in] */ Boolean hasDownloadId)
{
    /*
    mHasDownloadId = hasDownloadId;
    return this;
    */
    return AutoPtr<Builder>(this);
}

AutoPtr<Builder> DownloadInfo::Builder::SetDownloadId(
    /* [in] */ Int32 downloadId)
{
    /*
    mDownloadId = downloadId;
    return this;
    */
    return AutoPtr<Builder>(this);
}

AutoPtr<Builder> DownloadInfo::Builder::SetIsSuccessful(
    /* [in] */ Boolean isSuccessful)
{
    /*
    mIsSuccessful = isSuccessful;
    return this;
    */
    return AutoPtr<Builder>(this);
}

AutoPtr<Builder> DownloadInfo::Builder::SetContentDisposition(
    /* [in] */ String contentDisposition)
{
    /*
    mContentDisposition = contentDisposition;
    return this;
    */
    return AutoPtr<Builder>(this);
}

AutoPtr<Builder> DownloadInfo::Builder::SetPercentCompleted(
    /* [in] */ Int32 percentCompleted)
{
    /*
    assert percentCompleted <= 100;
    mPercentCompleted = percentCompleted;
    return this;
    */
    return AutoPtr<Builder>(this);
}

AutoPtr<Builder> DownloadInfo::Builder::SetTimeRemainingInMillis(
    /* [in] */ Int64 timeRemainingInMillis)
{
    /*
    mTimeRemainingInMillis = timeRemainingInMillis;
    return this;
    */
    return AutoPtr<Builder>(this);
}

AutoPtr<DownloadInfo> DownloadInfo::Builder::Build()
{
    /*
    return new DownloadInfo(this);
    */
    return AutoPtr<DownloadInfo>(this);
}

const AutoPtr<Builder> DownloadInfo::Builder::FromDownloadInfo(
    /* [in] */ DownloadInfo* downloadInfo)
{
    /*
    Builder builder = new Builder();
    builder
            .setUrl(downloadInfo.getUrl())
            .setUserAgent(downloadInfo.getUserAgent())
            .setMimeType(downloadInfo.getMimeType())
            .setCookie(downloadInfo.getCookie())
            .setFileName(downloadInfo.getFileName())
            .setDescription(downloadInfo.getDescription())
            .setFilePath(downloadInfo.getFilePath())
            .setReferer(downloadInfo.getReferer())
            .setContentLength(downloadInfo.getContentLength())
            .setHasDownloadId(downloadInfo.hasDownloadId())
            .setDownloadId(downloadInfo.getDownloadId())
            .setContentDisposition(downloadInfo.getContentDisposition())
            .setIsGETRequest(downloadInfo.isGETRequest())
            .setIsSuccessful(downloadInfo.isSuccessful())
            .setPercentCompleted(downloadInfo.getPercentCompleted())
            .setTimeRemainingInMillis(downloadInfo.getTimeRemainingInMillis());
    return builder;
    */
    return AutoPtr<Builder>(this);
}

//=====================================================================
//                             DownloadInfo
//=====================================================================
String DownloadInfo::GetUrl()
{
    /*
    return mUrl;
    */
    return String("");
}

String DownloadInfo::GetUserAgent()
{
    /*
    return mUserAgent;
    */
    return String("");
}

String DownloadInfo::GetMimeType()
{
    /*
    return mMimeType;
    */
    return String("");
}

String DownloadInfo::GetCookie()
{
    /*
    return mCookie;
    */
    return String("");
}

String DownloadInfo::GetFileName()
{
    /*
    return mFileName;
    */
    return String("");
}

String DownloadInfo::GetDescription()
{
    /*
    return mDescription;
    */
    return String("");
}

String DownloadInfo::GetFilePath()
{
    /*
    return mFilePath;
    */
    return String("");
}

String DownloadInfo::GetReferer()
{
    /*
    return mReferer;
    */
    return String("");
}

Int64 DownloadInfo::GetContentLength()
{
    /*
    return mContentLength;
    */
    return 0;
}

Boolean DownloadInfo::IsGETRequest()
{
    /*
    return mIsGETRequest;
    */
    return FALSE;
}

Boolean DownloadInfo::HasDownloadId()
{
    /*
    return mHasDownloadId;
    */
    return FALSE;
}

Int32 DownloadInfo::GetDownloadId()
{
    /*
    return mDownloadId;
    */
    return 0;
}

Boolean DownloadInfo::IsSuccessful()
{
    /*
    return mIsSuccessful;
    */
    return FALSE;
}

String DownloadInfo::GetContentDisposition()
{
    /*
    return mContentDisposition;
    */
    return String("");
}

Int32 DownloadInfo::GetPercentCompleted()
{
    /*
    return mPercentCompleted;
    */
    return 0;
}

Int64 DownloadInfo::GetTimeRemainingInMillis()
{
    /*
    return mTimeRemainingInMillis;
    */
    return 0;
}

DownloadInfo::DownloadInfo(
    /* [in] */ Builder* builder)
{
    /*
    mUrl = builder.mUrl;
    mUserAgent = builder.mUserAgent;
    mMimeType = builder.mMimeType;
    mCookie = builder.mCookie;
    mFileName = builder.mFileName;
    mDescription = builder.mDescription;
    mFilePath = builder.mFilePath;
    mReferer = builder.mReferer;
    mContentLength = builder.mContentLength;
    mHasDownloadId = builder.mHasDownloadId;
    mDownloadId = builder.mDownloadId;
    mIsSuccessful = builder.mIsSuccessful;
    mIsGETRequest = builder.mIsGETRequest;
    mContentDisposition = builder.mContentDisposition;
    mPercentCompleted = builder.mPercentCompleted;
    mTimeRemainingInMillis = builder.mTimeRemainingInMillis;
    */
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


