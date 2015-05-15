
#ifndef __CHEADERPARSEHELPER_H__
#define __CHEADERPARSEHELPER_H__

#include "_CHeaderParserHelper.h"

namespace Elastos {
namespace Net {
namespace Http {

CarClass(CHeaderParserHelper)
{
public:
    CARAPI ParseCacheControl(
        /* [in] */ const String& value,
        /* [in] */ ICacheControlHandler* handler);

    CARAPI ParseChallenges(
        /* [in] */ IRawHeaders* responseHeaders,
        /* [in] */ const String& challengeHeader,
        /* [out] */ IObjectContainer** obj);

    CARAPI ParseSeconds(
        /* [in] */ const String& value,
        /* [out] */ Int32* number);

private:
    static CARAPI_(Int32) SkipUntil(
        /* [in] */ const String& input,
        /* [in] */ Int32 pos,
        /* [in] */ const String& characters);

    static CARAPI_(Int32) SkipWhitespace(
        /* [in] */ const String& input,
        /* [in] */ Int32 pos);
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__CHEADERPARSEHELPER_H__
