
#include "CHttpCookieHelper.h"

namespace Elastos {
namespace Net {

CAR_INTERFACE_IMPL(CHttpCookieHelper, Singleton, IHttpCookieHelper)

CAR_SINGLETON_IMPL(CHttpCookieHelper)

ECode CHttpCookieHelper::IsDomainMatches(
    /* [in] */ const String& domainPattern,
    /* [in] */ const String& host,
    /* [out] */ Boolean* match)
{
    VALIDATE_NOT_NULL(match)


    return NOERROR;
}

Boolean CHttpCookieHelper::IsFullyQualifiedDomainName(
    /* [in] */ const String& s,
    /* [in] */ Int32 firstCharacter)
{
    //Int32 dotPosition = s.indexOf('.', firstCharacter + 1);
    //return dotPosition != -1 && dotPosition < s.length() - 1;

    return FALSE;
}

} // namespace Net
} // namespace Elastos
