
#ifndef __KEHSTOREHANDLER_H__
#define __KEHSTOREHANDLER_H__

#include "ext/frameworkext.h"
#include <elastos/Mutex.h>
#include "os/HandlerBase.h"
#include "webkit/ByteArrayBuilder.h"
#include <elastos/Mutex.h>

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
    Mutex mDataBuilderLock;
    String mMimeType;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__KEHSTOREHANDLER_H__
