
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

private static final String AwAssets::LOGTAG = "AwAssets";

//@CalledByNative
AutoPtr< ArrayOf<Int64> > AwAssets::OspenAsset(
    /* [in] */ IContext* context,
    /* [in] */ String fileName)
{
    AutoPtr<IAssetFileDescriptor> afd;
    //try {
        AutoPtr<IAssetManager> manager;
        context->GetAssets((IAssetManager**)&manager);
        manager->OpenFd(fileName, (IAssetFileDescriptor**)&afd);
        AutoPtr<IParcelFileDescriptor> descriptor;
        afd->GetParcelFileDescriptor((IParcelFileDescriptor**)&descriptor);
        Int64 fd, offset, length;
        afd->DetachFd(&fd);
        afd->GetStartOffset(&offset);
        afd->GetLength(&length);
        AutoPtr< ArrayOf<Int64> > array = ArrayOf<Int64>::Alloc(3);
        (*array)[0] = fd;
        (*array)[1] = offset;
        (*array)[2] = length;

        return array;
    //} catch (IOException e) {
    //    Log.e(LOGTAG, "Error while loading asset " + fileName + ": " + e);
    //    return new long[] {-1, -1, -1};
    //} finally {
    //    try {
    //        if (afd != null) {
    //            afd.close();
    //        }
    //    } catch (IOException e2) {
    //        Log.e(LOGTAG, "Unable to close AssetFileDescriptor", e2);
    //    }
    //}
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos