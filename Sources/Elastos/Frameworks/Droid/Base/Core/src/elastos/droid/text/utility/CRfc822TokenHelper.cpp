
#include "text/utility/CRfc822TokenHelper.h"
#include "text/utility/CRfc822Token.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Utility {

ECode CRfc822TokenHelper::QuoteNameIfNecessary(
    /* [in] */ const String& name,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = CRfc822Token::QuoteNameIfNecessary(name);
    return NOERROR;
}

ECode CRfc822TokenHelper::QuoteName(
    /* [in] */ const String& name,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = CRfc822Token::QuoteName(name);
    return NOERROR;
}

ECode CRfc822TokenHelper::QuoteComment(
    /* [in] */ const String& comment,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = CRfc822Token::QuoteComment(comment);
    return NOERROR;
}

} //namespace Utility
} //namespace Text
} //namespace Droid
} //namespace Elastos

