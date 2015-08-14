
#ifndef __ELASTOS_DROID_WEBKIT_CERTTOOL_H__
#define __ELASTOS_DROID_WEBKIT_CERTTOOL_H__

#include "ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CertTool
{
public:
    static CARAPI_(AutoPtr< ArrayOf<String> >) GetKeyStrengthList();

    static CARAPI_(String) GetSignedPublicKey(
        /* [in] */ IContext* context,
        /* [in] */ Int32 index,
        /* [in] */ const String& challenge);

    static CARAPI_(void) AddCertificate(
        /* [in] */ IContext* context,
        /* [in] */ const String& type,
        /* [in] */ ArrayOf<Byte>* value);

    static CARAPI_(String) GetCertType(
        /* [in] */ const String& mimeType);

private:
    CertTool();

    static const String LOGTAG;
//    static const AutoPtr<IAlgorithmIdentifier> MD5_WITH_RSA;/* =
//            new AlgorithmIdentifier(PKCSObjectIdentifiers.md5WithRSAEncryption);*/
    static HashMap<String, String> sCertificateTypeMap;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_CERTTOOL_H__
