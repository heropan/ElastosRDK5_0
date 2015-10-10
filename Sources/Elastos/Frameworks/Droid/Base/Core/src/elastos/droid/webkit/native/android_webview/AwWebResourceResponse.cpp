#include "elastos/droid/webkit/native/android_webview/AwWebResourceResponse.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

AwWebResourceResponse::AwWebResourceResponse(
    /* [in] */ const String& mimeType,
    /* [in] */ const String& encoding,
    /* [in] */ IInputStream* data)
    : mMimeType(mimeType)
    , mCharset(encoding)
    , mData(data)
    , mStatusCode(0)
{
}

AwWebResourceResponse::AwWebResourceResponse(
    /* [in] */ const String& mimeType,
    /* [in] */ const String& encoding,
    /* [in] */ IInputStream* data,
    /* [in] */ Int32 statusCode,
    /* [in] */ const String& reasonPhrase,
    /* [in] */ Map<String, String> &responseHeaders)
    : mMimeType(mimeType)
    , mCharset(encoding)
    , mData(data)
    , mStatusCode(statusCode)
    , mReasonPhrase(reasonPhrase)
{
    Int32 size = responseHeaders.GetSize();
    //if (responseHeaders != null)
    if (size > 0)
    {
        mResponseHeaderNames = ArrayOf<String>::Alloc(size);//new String[responseHeaders.size()];
        mResponseHeaderValues = ArrayOf<String>::Alloc(size);//new String[responseHeaders.size()];
        int i = 0;
        Map<String, String>::Iterator iter = responseHeaders.Begin();
        //for (Map.Entry<String, String> entry : responseHeaders.entrySet())
        while (iter != responseHeaders.End())
        {
            (*mResponseHeaderNames)[i] = iter->mFirst;//entry.getKey();
            (*mResponseHeaderValues)[i] = iter->mSecond;//entry.getValue();
            ++iter;
            ++i;
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
AutoPtr<ArrayOf<String> > AwWebResourceResponse::GetResponseHeaderNames()
{
    return mResponseHeaderNames;
}

//@CalledByNative
AutoPtr<ArrayOf<String> > AwWebResourceResponse::GetResponseHeaderValues()
{
    return mResponseHeaderValues;
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
