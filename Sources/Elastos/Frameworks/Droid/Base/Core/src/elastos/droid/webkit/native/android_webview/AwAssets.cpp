#include "elastos/droid/webkit/native/android_webview/AwAssets.h"
#include "elastos/utility/logging/Logger.h"

using Elastos::Droid::Content::Res::IAssetFileDescriptor;
using Elastos::Droid::Content::Res::IAssetManager;
using Elastos::Droid::Os::IParcelFileDescriptor;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

const String AwAssets::LOGTAG = String("AwAssets");

//@CalledByNative
AutoPtr<ArrayOf<Int64> > AwAssets::OpenAsset(
    /* [in] */ IContext* context,
    /* [in] */ const String& fileName)
{
    AutoPtr<IAssetFileDescriptor> afd;
    //try {
        AutoPtr<IAssetManager> manager;
        context->GetAssets((IAssetManager**)&manager);
        ECode ecode = manager->OpenFd(fileName, (IAssetFileDescriptor**)&afd);
        if (FAILED(ecode))
        {
            Logger::E(LOGTAG, "Error while loading asset %s, ecode:0x%x", fileName.string(), ecode);
            AutoPtr<ArrayOf<Int64> > array = ArrayOf<Int64>::Alloc(3);
            (*array)[0] = -1;
            (*array)[1] = -1;
            (*array)[2] = -1;
            return array;
        }
        AutoPtr<IParcelFileDescriptor> descriptor;
        afd->GetParcelFileDescriptor((IParcelFileDescriptor**)&descriptor);
        Int32 fd;
        Int64 offset, length;
        descriptor->DetachFd(&fd);
        afd->GetStartOffset(&offset);
        afd->GetLength(&length);
        AutoPtr<ArrayOf<Int64> > array = ArrayOf<Int64>::Alloc(3);
        (*array)[0] = fd;
        (*array)[1] = offset;
        (*array)[2] = length;

        ecode = afd->Close();
        if (FAILED(ecode))
            Logger::E(LOGTAG, "Unable to close AssetFileDescriptor,ecode:0x%x", ecode);

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
