
#include "net/CTrafficStats.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <elstrtoken.h>
#include <elastos/StringBuilder.h>

namespace Elastos {
namespace Droid {
namespace Net {

Ecode CUrlQuerySanitizerHelper::GetAllIllegal(
    /* [out] */ IValueSanitizer** result)
{
    return CUrlQuerySanitizer::GetAllIllegal(result);
}

Ecode CUrlQuerySanitizerHelper::GetAllButNulLegal(
    /* [out] */ IValueSanitizer** result)
{
    return CUrlQuerySanitizer::GetAllButNulLegal(result);
}

Ecode CUrlQuerySanitizerHelper::GetAllButWhitespaceLegal(
    /* [out] */ IValueSanitizer** result)
{
    return CUrlQuerySanitizer::GetAllButWhitespaceLegal(result);
}

Ecode CUrlQuerySanitizerHelper::GetUrlLegal(
    /* [out] */ IValueSanitizer** result)
{
    return CUrlQuerySanitizer::GetUrlLegal(result);
}

Ecode CUrlQuerySanitizerHelper::GetUrlAndSpaceLegal(
    /* [out] */ IValueSanitizer** result)
{
    return CUrlQuerySanitizer::GetUrlAndSpaceLegal(result);
}

Ecode CUrlQuerySanitizerHelper::GetAmpLegal(
    /* [out] */ IValueSanitizer** result)
{
    return CUrlQuerySanitizer::GetAmpLegal(result);
}

Ecode CUrlQuerySanitizerHelper::GetAmpAndSpaceLegal(
    /* [out] */ IValueSanitizer** result)
{
    return CUrlQuerySanitizer::GetAmpAndSpaceLegal(result);
}

Ecode CUrlQuerySanitizerHelper::GetSpaceLegal(
    /* [out] */ IValueSanitizer** result)
{
    return CUrlQuerySanitizer::GetSpaceLegal(result);
}

Ecode CUrlQuerySanitizerHelper::GetAllButNulAndAngleBracketsLegal(
    /* [out] */ IValueSanitizer** result)
{
    return CUrlQuerySanitizer::GetAllButNulAndAngleBracketsLegal(result);
}




} // namespace Net
} // namepsace Droid
} // namespace Elastos