
#include "elastos/droid/webkit/CertTool.h"
#include "elastos/droid/webkit/KeyStoreHandler.h"

using Elastos::Droid::Os::EIID_IHandler;

namespace Elastos {
namespace Droid {
namespace Webkit {

const String KeyStoreHandler::LOGTAG("KeyStoreHandler");

KeyStoreHandler::KeyStoreHandler(
    /* [in] */ const String& mimeType)
    : mMimeType(mimeType)
    , mDataBuilder(new ByteArrayBuilder())
{
}

/**
 * Add data to the internal collection of data.
 * @param data A byte array containing the content.
 * @param length The length of data.
 */
void KeyStoreHandler::DidReceiveData(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 length)
{
    AutoLock lock(mDataBuilderLock);
    mDataBuilder->Append(data, 0, length);
}

void KeyStoreHandler::InstallCert(
    /* [in] */ IContext* context)
{
    String type = CertTool::GetCertType(mMimeType);
    if (type.IsNull()) return;

    // This must be synchronized so that no more data can be added
    // after getByteSize returns.
    {
        AutoLock lock(mDataBuilderLock);
        // In the case of downloading certificate, we will save it
        // to the KeyStore and stop the current loading so that it
        // will not generate a new history page
        AutoPtr< ArrayOf<Byte> > cert = ArrayOf<Byte>::Alloc(mDataBuilder->GetByteSize());
        Int32 offset = 0;
        while (TRUE) {
            AutoPtr<ByteArrayBuilder::Chunk> c = mDataBuilder->GetFirstChunk();
            if (c == NULL) break;

            if (c->mLength != 0) {
                for (Int32 i = 0; i < c->mLength; ++i) {
                    (*cert)[i + offset] = (*(c->mArray))[i];
                }
                offset += c->mLength;
            }
            c->ReleaseResources();
        }
        CertTool::AddCertificate(context, type, cert);
        return;
    }
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
