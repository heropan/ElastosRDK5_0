
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//==============================================================================
//               AwContentsClientCallbackHelper::DownloadInfo
//==============================================================================

AwContentsClientCallbackHelper::DownloadInfo::DownloadInfo(
    /* [in] */ String url,
    /* [in] */ String userAgent,
    /* [in] */ String contentDisposition,
    /* [in] */ String mimeType,
    /* [in] */ Int64 contentLength)
    : mUrl(url)
    , mUserAgent(userAgent)
    , mContentDisposition(contentDisposition)
    , mMimeType(mimeType)
    , mContentLength(contentLength)
{
}

//==============================================================================
//              AwContentsClientCallbackHelper::LoginRequestInfo
//==============================================================================

AwContentsClientCallbackHelper::LoginRequestInfo::LoginRequestInfo(
    /* [in] */ String realm,
    /* [in] */ String account,
    /* [in] */ String args)
{
    mRealm = realm;
    mAccount = account;
    mArgs = args;
}

//==============================================================================
//             AwContentsClientCallbackHelper::OnReceivedErrorInfo
//==============================================================================

AwContentsClientCallbackHelper::OnReceivedErrorInfo::OnReceivedErrorInfo(
    /* [in] */ Int32 errorCode,
    /* [in] */ String description,
    /* [in] */ String failingUrl)
    : mErrorCode(errorCode)
    , mDescription(description)
    , mFailingUrl(failingUrl)
{
}

//==============================================================================
//                  AwContentsClientCallbackHelper::MyHandler
//==============================================================================

AwContentsClientCallbackHelper::MyHandler::MyHandler(
    /* [in] */ AwContentsClientCallbackHelper* owner,
    /* [in] */ ILooper* looper)
    : mOwner(owner)
{
    super(looper);
}

ECode AwContentsClientCallbackHelper::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    switch(what) {
        case MSG_ON_LOAD_RESOURCE: {
            AutoPtr<ICharSequence> cs;
            msg->GetObj((IInterface**)&cs);
            String url;
            cs->ToString(&url);
            mContentsClient->OnLoadResource(url);
            break;
        }
        case MSG_ON_PAGE_STARTED: {
            AutoPtr<ICharSequence> cs;
            msg->GetObj((IInterface**)&cs);
            String url;
            cs->ToString(&url);
            mContentsClient->OnPageStarted(url);
            break;
        }
        case MSG_ON_DOWNLOAD_START: {
            AutoPtr<DownloadInfo> info;
            msg->GetObj((IInterface**)&info);
            mContentsClient->OnDownloadStart(info->mUrl, info->mUserAgent,
                    info->mContentDisposition, info->mMimeType, info->mContentLength);
            break;
        }
        case MSG_ON_RECEIVED_LOGIN_REQUEST: {
            AutoPtr<LoginRequestInfo> info;
            msg->GetObj((IInterface**)&info);
            mContentsClient->OnReceivedLoginRequest(info->mRealm, info->mAccount, info->mArgs);
            break;
        }
        case MSG_ON_RECEIVED_ERROR: {
            AutoPtr<OnReceivedErrorInfo> info;
            msg->GetObj((IInterface**)&info);
            mContentsClient->OnReceivedError(info->mErrorCode, info->mDescription,
                    info->mFailingUrl);
            break;
        }
        case MSG_ON_NEW_PICTURE: {
            AutoPtr<IPicture> picture;
            //try {
                AutoPtr<IInterface> obj;
                msg->GetObj((IInterface**)&obj);
                if (obj != NULL) {
                    AutoPtr<ICallable> callable = obj->Probe(EIID_ICallable);
                    callable->Call((IPicture**)&picture);
                }
            //} catch (Exception e) {
            //    throw new RuntimeException("Error getting picture", e);
            //}
            mContentsClient->OnNewPicture(picture);
            mLastPictureTime = SystemClock::UptimeMillis();
            mHasPendingOnNewPicture = FALSE;
            break;
        }
        case MSG_ON_SCALE_CHANGED_SCALED: {
            float oldScale = Float.intBitsToFloat(msg.arg1);
            float newScale = Float.intBitsToFloat(msg.arg2);
            mContentsClient.onScaleChangedScaled(oldScale, newScale);
            break;
        }
        default:
            throw new IllegalStateException(
                    "AwContentsClientCallbackHelper: unhandled message " + msg.what);
    }
}

//==============================================================================
//                     AwContentsClientCallbackHelper
//==============================================================================

const Int32 AwContentsClientCallbackHelper::MSG_ON_LOAD_RESOURCE;
const Int32 AwContentsClientCallbackHelper::MSG_ON_PAGE_STARTED;
const Int32 AwContentsClientCallbackHelper::MSG_ON_DOWNLOAD_START;
const Int32 AwContentsClientCallbackHelper::MSG_ON_RECEIVED_LOGIN_REQUEST;
const Int32 AwContentsClientCallbackHelper::MSG_ON_RECEIVED_ERROR;
const Int32 AwContentsClientCallbackHelper::MSG_ON_NEW_PICTURE;
const Int32 AwContentsClientCallbackHelper::MSG_ON_SCALE_CHANGED_SCALED;

// Minimum period allowed between consecutive onNewPicture calls, to rate-limit the callbacks.
const Int64 AwContentsClientCallbackHelper::ON_NEW_PICTURE_MIN_PERIOD_MILLIS;
// Timestamp of the most recent onNewPicture callback.
private long mLastPictureTime = 0;
// True when a onNewPicture callback is currenly in flight.
private boolean mHasPendingOnNewPicture = false;

AwContentsClientCallbackHelper::AwContentsClientCallbackHelper(
    /* [in] */ ILooper* looper,
    /* [in] */ AwContentsClient* contentsClient)
    : mLastPictureTime(0)
    , mHasPendingOnNewPicture(FALSE)
    , mContentsClient(contentsClient)
{
    mHandler = new MyHandler(this, looper);
}

void AwContentsClientCallbackHelper::PostOnLoadResource(
    /* [in] */ String url)
{
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(MSG_ON_LOAD_RESOURCE, url, (IMessage**)&msg);
    mHandler->SendMessage(msg);
}

void AwContentsClientCallbackHelper::PostOnPageStarted(
    /* [in] */ String url)
{
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(MSG_ON_PAGE_STARTED, url, (IMessage**)&msg);
    mHandler->SendMessage(msg);
}

void AwContentsClientCallbackHelper::PostOnDownloadStart(
    /* [in] */ String url,
    /* [in] */ String userAgent,
    /* [in] */ String contentDisposition,
    /* [in] */ String mimeType,
    /* [in] */ Int64 contentLength)
{
    AutoPtr<DownloadInfo info> = new DownloadInfo(url, userAgent, contentDisposition, mimeType,
            contentLength);
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(MSG_ON_DOWNLOAD_START, info, (IMessage**)&msg);
    mHandler->SendMessage(msg);
}

void AwContentsClientCallbackHelper::PostOnReceivedLoginRequest(
    /* [in] */ String realm,
    /* [in] */ String account,
    /* [in] */ String args)
{
    AutoPtr<LoginRequestInfo> info = new LoginRequestInfo(realm, account, args);
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(MSG_ON_RECEIVED_LOGIN_REQUEST, info, (IMessage**)&msg);
    mHandler->SendMessage(msg);
}

void AwContentsClientCallbackHelper::PostOnReceivedError(
    /* [in] */ Int32 errorCode,
    /* [in] */ String description,
    /* [in] */ String failingUrl)
{
    AutoPtr<OnReceivedErrorInfo> info = new OnReceivedErrorInfo(errorCode, description, failingUrl);
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(MSG_ON_RECEIVED_ERROR, info, (IMessage**)&msg);
    mHandler->SendMessage(msg);
}

void AwContentsClientCallbackHelper::PostOnNewPicture(
    /* [in] */ ICallable* pictureProvider)
{
    if (mHasPendingOnNewPicture) return;
    mHasPendingOnNewPicture = TRUE;
    Int64 pictureTime = Math::Max(mLastPictureTime + ON_NEW_PICTURE_MIN_PERIOD_MILLIS,
            SystemClock::UptimeMillis());
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(MSG_ON_NEW_PICTURE, pictureProvider, (IMessage**)&msg);
    mHandler->SendMessageAtTime(msg, pictureTime);
}

void AwContentsClientCallbackHelper::PostOnScaleChangedScaled(
    /* [in] */ Float oldScale,
    /* [in] */ Float newScale)
{
    // The float->int->float conversion here is to avoid unnecessary allocations. The
    // documentation states that intBitsToFloat(floatToIntBits(a)) == a for all values of a
    // (except for NaNs which are collapsed to a single canonical NaN, but we don't care for
    // that case).
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(MSG_ON_SCALE_CHANGED_SCALED,
                Float.floatToIntBits(oldScale), Float.floatToIntBits(newScale), (IMessage**)&msg);
    mHandler->SendMessage(msg);
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos