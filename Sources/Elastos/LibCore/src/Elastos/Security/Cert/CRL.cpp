
#include "cmdef.h"
#ifdef ELASTOS_CORE
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif
#include "CRL.h"

namespace Elastos {
namespace Security {
namespace Cert {

CRL::CRL(
    /* [in] */ const String& type)
: mType(type)
{}

ECode CRL::GetType(
    /* [out] */ String* type) const
{
    VALIDATE_NOT_NULL(type)
    *type = mType;
    return NOERROR;
}

} // end Cert
} // end Security
} // end Elastos