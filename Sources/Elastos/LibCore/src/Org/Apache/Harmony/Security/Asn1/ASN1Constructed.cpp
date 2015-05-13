
#include "ASN1Constructed.h"
#include <Org.Apache.Harmony_server.h>
#include <cmdef.h>

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

ASN1Constructed::ASN1Constructed(
    /* [in] */ Int32 tagNumber)
    : ASN1Type(IASN1Constants::CLASS_UNIVERSAL, tagNumber)
{}

ASN1Constructed::ASN1Constructed(
    /* [in] */ Int32 tagClass,
    /* [in] */ Int32 tagNumber)
    : ASN1Type(tagClass, tagNumber)
{}

ECode ASN1Constructed::Init(
    /* [in] */ Int32 tagNumber)
{
    return ASN1Type::Init(IASN1Constants::CLASS_UNIVERSAL, tagNumber);
}

ECode ASN1Constructed::Init(
    /* [in] */ Int32 tagClass,
    /* [in] */ Int32 tagNumber)
{
    return ASN1Type::Init(tagClass, tagNumber);
}

/**
 * Tests provided identifier.
 *
 * @param identifier - identifier to be verified
 * @return - true if identifier correspond to constructed identifier of
 *           this ASN.1 type, otherwise false
 */
ECode ASN1Constructed::CheckTag(
    /* [in] */ Int32 identifier,
    /* [ou] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mConstrId == identifier;
    return NOERROR;
}

ECode ASN1Constructed::EncodeASN(
    /* [in] */ IBerOutputStream* out)
{
    out->EncodeTag(mConstrId);
    return EncodeContent(out);
}

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org