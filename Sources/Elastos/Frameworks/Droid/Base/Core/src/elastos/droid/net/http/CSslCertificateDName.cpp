
#include "CSslCertificateDName.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elautoptr.h"
#include "elastos/Vector.h"

using namespace Elastos::Core;

using Org::Bouncycastle::Asn1::X509::IX509Name;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

ECode CSslCertificateDName::constructor(
    /* [in] */ const String& dName)
{
    if (dName != NULL) {
        mDName = dName;

        AutoPtr<IX509Name> x509Name;
        // TODO:
        // CX509Name::New(dName, (IX509Name**)&x509Name);

        Vector<String>* val;
        // x509Name->GetValues(&val);
        Vector<String>* oid;
        // x509Name->GetOIDs(&oid);

        // for (Int32 i = 0; i < oid->GetSize(); i++) {
        //     if (oid->At(i).Equals(IX509Name::CN)) {
        //         mCName = (String) val->At(i);
        //         continue;
        //     }

        //     if (oid->At(i).Equals(IX509Name::O)) {
        //         mOName = (String) val->At(i);
        //         continue;
        //     }

        //     if (oid->At(i).Equals(IX509Name::OU)) {
        //         mUName = (String) val->At(i);
        //         continue;
        //     }
        // }
    }
    return NOERROR;
}

ECode CSslCertificateDName::GetDName(
    /* [out] */ String* dName)
{
    VALIDATE_NOT_NULL(dName);
    if (mDName.IsNullOrEmpty()) {
        *dName = String("");
    } else {
        *dName = mDName;
    }
    return NOERROR;
}

ECode CSslCertificateDName::GetCName(
    /* [out] */ String* cName)
{
    VALIDATE_NOT_NULL(cName);
    if (mCName.IsNullOrEmpty()) {
        *cName = String("");
    } else {
        *cName = mCName;
    }
    return NOERROR;
}

ECode CSslCertificateDName::GetOName(
    /* [out] */ String* oName)
{
    VALIDATE_NOT_NULL(oName);
    if (mOName.IsNullOrEmpty()) {
        *oName = String("");
    } else {
        *oName = mOName;
    }
    return NOERROR;
}

ECode CSslCertificateDName::GetUName(
    /* [out] */ String* uName)
{
    VALIDATE_NOT_NULL(uName);
    if (mUName.IsNullOrEmpty()) {
        *uName = String("");
    } else {
        *uName = mUName;
    }
    return NOERROR;
}

}
}
}
}

