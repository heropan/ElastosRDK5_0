
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

AwDevToolsServer::AwDevToolsServer()
{
    mNativeDevToolsServer = nativeInitRemoteDebugging();
}

void AwDevToolsServer::Destroy() {
    NativeDestroyRemoteDebugging(mNativeDevToolsServer);
    mNativeDevToolsServer = 0;
}

void AwDevToolsServer::SetRemoteDebuggingEnabled(
    /* [in] */ Boolean enabled)
{
    NativeSetRemoteDebuggingEnabled(mNativeDevToolsServer, enabled);
}

Int64 AwDevToolsServer::NativeInitRemoteDebugging()
{
}

void AwDevToolsServer::NativeDestroyRemoteDebugging(
    /* [in] */ Int64 devToolsServer)
{
}

void AwDevToolsServer::NativeSetRemoteDebuggingEnabled(
    /* [in] */ Int64 devToolsServer,
    /* [in] */ Boolean enabled)
{
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
