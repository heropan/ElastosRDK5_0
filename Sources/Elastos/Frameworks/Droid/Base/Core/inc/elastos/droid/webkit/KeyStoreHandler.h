
#ifndef __ELASTOS_DROID_WEBKIT_KEHSTOREHANDLER_H__
#define __ELASTOS_DROID_WEBKIT_KEHSTOREHANDLER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "os/HandlerBase.h"
#include "webkit/ByteArrayBuilder.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::HandlerBase;

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * KeyStoreHandler: class responsible for certificate installation to
 * the system key store. It reads the certificates file from network
 * then pass the bytes to class CertTool.
 * This class is only needed if the Chromium HTTP stack is used.
 */
class KeyStoreHandler : public HandlerBase
{
public:
    KeyStoreHandler(
        /* [in] */ const String& mimeType);

    /**
     * Add data to the internal collection of data.
     * @param data A byte array containing the content.
     * @param length The length of data.
     */
    CARAPI_(void) DidReceiveData(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 length);

    CARAPI_(void) InstallCert(
        /* [in] */ IContext* context);

private:
    static const String LOGTAG;
    AutoPtr<ByteArrayBuilder> mDataBuilder;
    Object mDataBuilderLock;
    String mMimeType;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_KEHSTOREHANDLER_H__
