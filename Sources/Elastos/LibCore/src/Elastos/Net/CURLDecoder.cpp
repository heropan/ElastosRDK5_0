
#include "CURLDecoder.h"
#include "URLDecoder.h"

namespace Elastos {
namespace Net {

ECode CURLDecoder::Decode(
    /* [in] */ const String& s,
    /* [out] */ String* result)
{
    return URLDecoder::Decode(s, result);
}

ECode CURLDecoder::DecodeEx(
    /* [in] */ const String& s,
    /* [in] */ const String& charsetName,
    /* [out] */ String* result)
{
    return URLDecoder::Decode(s, charsetName, result);
}

} // namespace Net
} // namespace Elastos
