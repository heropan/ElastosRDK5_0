
#include "elastos/droid/text/utility/CRfc822Token.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Utility {

ECode CRfc822Token::GetName(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Rfc822Token::GetName();
    return NOERROR;
}

ECode CRfc822Token::GetAddress(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Rfc822Token::GetAddress();
    return NOERROR;
}

ECode CRfc822Token::GetComment(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Rfc822Token::GetComment();
    return NOERROR;
}

ECode CRfc822Token::SetName(
    /* [in] */ const String& name)
{
    return Rfc822Token::SetName(name);
}

ECode CRfc822Token::SetAddress(
    /* [in] */ const String& address)
{
    return Rfc822Token::SetAddress(address);
}

ECode CRfc822Token::SetComment(
    /* [in] */ const String& comment)
{
    return Rfc822Token::SetComment(comment);
}

ECode CRfc822Token::ToString(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Rfc822Token::ToString();
    return NOERROR;
}

ECode CRfc822Token::GetHashCode(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Rfc822Token::GetHashCode();
    return NOERROR;
}

ECode CRfc822Token::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Rfc822Token::Equals(o);
    return NOERROR;
}

ECode CRfc822Token::constructor(
    /* [in] */ const String& name,
    /* [in] */ const String& address,
    /* [in] */ const String& comment)
{
    Init(name, address, comment);
    return NOERROR;
}

} //namespace Utility
} //namespace Text
} //namespace Droid
} //namespace Elastos

