#include "elastos/droid/webkit/native/android_webview/InputStreamUtil.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

const String InputStreamUtil::LOGTAG("AwAssets");
const Int32 InputStreamUtil::CALL_FAILED_STATUS;
const Int32 InputStreamUtil::EXCEPTION_THROWN_STATUS;

String InputStreamUtil::LogMessage(
    /* [in] */ const String& method)
{
    String retStr("Got exception when calling ");
    retStr += method;
    retStr += "() on an InputStream returned from ";
    retStr += "shouldInterceptRequest. This will cause the related request to fail.";

    return retStr;
}

//@CalledByNative
void InputStreamUtil::Close(
    /* [in] */ IInputStream* stream)
{
    // try {
        ECode ecode = stream->Close();
        if (FAILED(ecode))
            Logger::E(LOGTAG, "InputStreamUtil::Close, ecode:0x%x", ecode);
    // } catch (IOException e) {
    //     Log.e(LOGTAG, logMessage("close"), e);
    // }
}

//@CalledByNative
Int32 InputStreamUtil::Available(
    /* [in] */ IInputStream* stream)
{
    // try {
        Int32 available;
        ECode ecode = stream->Available(&available);
        if (FAILED(ecode))
        {
            Logger::E(LOGTAG, "InputStreamUtil::Available, ecode:0x%x", ecode);
            return EXCEPTION_THROWN_STATUS;
        }
        return Elastos::Core::Math::Max(CALL_FAILED_STATUS, available);
    // } catch (IOException e) {
    //     Log.e(LOGTAG, logMessage("available"), e);
    //     return EXCEPTION_THROWN_STATUS;
    // }
}

//@CalledByNative
Int32 InputStreamUtil::Read(
    /* [in] */ IInputStream* stream,
    /* [in] */ ArrayOf<Byte>* b,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len)
{
    // try {
        Int32 length;
        ECode ecode = stream->Read(b, off, len, &length);
        if (FAILED(ecode))
        {
            Logger::E(LOGTAG, "InputStreamUtil::Read, ecode:0x%x", ecode);
            return EXCEPTION_THROWN_STATUS;
        }
        return Elastos::Core::Math::Max(CALL_FAILED_STATUS, length);
    // } catch (IOException e) {
    //     Log.e(LOGTAG, logMessage("read"), e);
    //     return EXCEPTION_THROWN_STATUS;
    // }
}

//@CalledByNative
Int64 InputStreamUtil::Skip(
    /* [in] */ IInputStream* stream,
    /* [in] */ Int64 n)
{
    // try {
        Int64 skip;
        ECode ecode = stream->Skip(n, &skip);
        if (FAILED(ecode))
        {
            Logger::E(LOGTAG, "InputStreamUtil::Skip, ecode:0x%x", ecode);
            return EXCEPTION_THROWN_STATUS;
        }
        return Elastos::Core::Math::Max((Int64)CALL_FAILED_STATUS, skip);
    // } catch (IOException e) {
    //     Log.e(LOGTAG, logMessage("skip"), e);
    //     return EXCEPTION_THROWN_STATUS;
    // }
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
