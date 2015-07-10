
#include "StandardCharsets.h"
#include "Charset.h"

namespace Elastos {
namespace IO {
namespace Charset {

static const AutoPtr<ICharset> StandardCharsets::ISO_8859_1;

static const AutoPtr<ICharset> StandardCharsets::US_ASCII;

static const AutoPtr<ICharset> StandardCharsets::UTF_8;

static const AutoPtr<ICharset> StandardCharsets::UTF_16;

static const AutoPtr<ICharset> StandardCharsets::UTF_16BE;

static const AutoPtr<ICharset> StandardCharsets::UTF_16LE;

StandardCharsets::StandardCharsets()
{}

Boolean StandardCharsets::isInit()
{
    Charset::ForName(String("ISO-8859-1"), (ICharset**)&ISO_8859_1);
    Charset::ForName(String("US-ASCII"), (ICharset**)&US_ASCII);
    Charset::ForName(String("UTF-8"), (ICharset**)&UTF_8);
    Charset::ForName(String("UTF-16"), (ICharset**)&UTF_16);
    Charset::ForName(String("UTF-16BE"), (ICharset**)&UTF_16BE);
    Charset::ForName(String("UTF-16LE"), (ICharset**)&UTF_16LE);
    return TRUE;
}

Boolean StandardCharsets::isinitflag = isInit();

} // namespace Charset
} // namespace IO
} // namespace Elastos
