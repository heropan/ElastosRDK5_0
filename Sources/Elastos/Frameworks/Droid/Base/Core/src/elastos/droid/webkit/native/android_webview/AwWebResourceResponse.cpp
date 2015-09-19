
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

AwWebResourceResponse::AwWebResourceResponse(
    /* [in] */ String mimeType,
    /* [in] */ String encoding,
    /* [in] */ IInputStream* data)
    : mMimeType(mimeType)
    , mCharset(encoding)
    , mData(data)
    , mStatusCode(0)
{
}

AwWebResourceResponse::AwWebResourceResponse(
    /* [in] */ String mimeType,
    /* [in] */ String encoding,
    /* [in] */ IInputStream* data,
    /* [in] */ Int32 statusCode,
    /* [in] */ String reasonPhrase,
    /* [in] */ Map<String, String> responseHeaders)
    : mMimeType(mimeType)
    , mCharset(encoding)
    , mData(data)
    , mStatusCode(statusCode)
    , mReasonPhrase(reasonPhrase)
{
    if (responseHeaders != null) {
        mResponseHeaderNames = new String[responseHeaders.size()];
        mResponseHeaderValues = new String[responseHeaders.size()];
        int i = 0;
        for (Map.Entry<String, String> entry : responseHeaders.entrySet()) {
            mResponseHeaderNames[i] = entry.getKey();
            mResponseHeaderValues[i] = entry.getValue();
            i++;
        }
    }
}

//@CalledByNative
String AwWebResourceResponse::GetMimeType()
{
    return mMimeType;
}

//@CalledByNative
String AwWebResourceResponse::GetCharset()
{
    return mCharset;
}

//@CalledByNative
AutoPtr<IInputStream> AwWebResourceResponse::GetData()
{
    return mData;
}

//@CalledByNative
Int32 AwWebResourceResponse::GetStatusCode()
{
    return mStatusCode;
}

//@CalledByNative
String AwWebResourceResponse::GetReasonPhrase()
{
    return mReasonPhrase;
}

//@CalledByNative
AutoPtr< ArrayOf<String> > AwWebResourceResponse::GetResponseHeaderNames()
{
    return mResponseHeaderNames;
}

//@CalledByNative
AutoPtr< ArrayOf<String> > AwWebResourceResponse::GetResponseHeaderValues()
{
    return mResponseHeaderValues;
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
