
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

//@CalledByNative
Boolean ResourceBundle::AssetContainedInApk(
    /* in */ IContext* ctx,
    /* in */ String filename)
{
    //try {
        AutoPtr<IAssetManager> am;
        ctx->GetAssets((IAssetManager**)&am);
        AutoPtr<IAssetFileDescriptor> afd;
        am->OpenFd(filename, (IAssetFileDescriptor**)&afd);
        afd->Close();
        return TRUE;
    //} catch (IOException e) {
    //    return false;
    //}
}

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

