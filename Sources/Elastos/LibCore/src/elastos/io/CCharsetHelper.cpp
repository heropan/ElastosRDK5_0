#include "CCharsetHelper.h"
#include "Charset.h"

// TODO:
// ECode CCharsetHelper::availableCharsets(
//  SortedMap<String, Charset> charsets)
// {
//  return Charset::availableCharsets(charsets);
// }
namespace Elastos {
namespace IO {
namespace Charset {

CAR_INTERFACE_IMPL(CCharsetHelper, Singleton, ICharsetHelper)

CAR_SINGLETON_IMPL(CCharsetHelper)

ECode CCharsetHelper::ForName(
    /* [in] */ const String& charsetName,
    /* [out] */ ICharset** charset)
{
    return Charset::ForName(charsetName, charset);
}

ECode CCharsetHelper::IsSupported(
    /* [in] */ const String& charsetName,
    /* [out] */ Boolean* result)
{
    return Charset::IsSupported(charsetName, result);
}

ECode CCharsetHelper::DefaultCharset(
    /* [out] */ICharset** charset)
{
    return Charset::DefaultCharset(charset);
}

} // namespace Charset
} // namespace IO
} // namespace Elastos
