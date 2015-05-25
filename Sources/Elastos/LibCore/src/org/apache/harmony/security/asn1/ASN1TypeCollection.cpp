
#include "ASN1TypeCollection.h"
#include <Org.Apache.Harmony_server.h>
#include <cmdef.h>

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

ASN1TypeCollection::ASN1TypeCollection(
    /* [in] */ Int32 tagNumber,
    /* [in] */ ArrayOf<IASN1Type*>* type)
    : ASN1Constructed(tagNumber, type)
{
    mType = type;
    mOPTIONAL = ArrayOf<Boolean>::Alloc(type->GetLength());
    mDEFAULT = ArrayOf<IInterface*>::Alloc(type->GetLength());
}

ECode ASN1TypeCollection::SetOptional(
    /* [in] */ Int32 index)
{
    (*mOPTIONAL)[index] = TRUE;
    return NOERROR;
}

ECode ASN1TypeCollection::SetDefault(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 index)
{
    (*mOPTIONAL)[index] = TRUE;
    mDEFAULT->Set(index, object);
    return NOERROR;
}

ECode ASN1TypeCollection::GetValues(
    /* [in] */ IInterface* object,
    /* [in] */ ArrayOf<IInterface*>* values)
{
    return E_RUNTIME_EXCEPTION;
}

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org