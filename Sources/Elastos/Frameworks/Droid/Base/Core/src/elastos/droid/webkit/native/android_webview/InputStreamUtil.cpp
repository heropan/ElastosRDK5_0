
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

const String InputStreamUtil::LOGTAG("AwAssets");
const Int32 InputStreamUtil::CALL_FAILED_STATUS;
const Int32 InputStreamUtil::EXCEPTION_THROWN_STATUS;

String InputStreamUtil::LogMessage(
    /* [in] */ String method)
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
        stream->Close();
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
        stream->Available(&available);
        return Math::Max(CALL_FAILED_STATUS, available);
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
        stream->Read(b, off, len, &length)
        return Math::Max(CALL_FAILED_STATUS, length);
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
        Int32 skip;
        stream->Skip(n, &skip);
        return Math::Max(CALL_FAILED_STATUS, skip);
    // } catch (IOException e) {
    //     Log.e(LOGTAG, logMessage("skip"), e);
    //     return EXCEPTION_THROWN_STATUS;
    // }
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
