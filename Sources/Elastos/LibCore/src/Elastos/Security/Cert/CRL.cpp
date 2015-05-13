
#include "cmdef.h"
#ifdef ELASTOS_CORE
#include "Elastos.Core_server.h"
#else
#include "Elastos.Core.h"
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