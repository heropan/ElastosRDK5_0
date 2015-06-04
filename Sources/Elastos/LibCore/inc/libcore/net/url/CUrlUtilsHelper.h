
#ifndef __CURLUTILSHELPER_H__
#define __CURLUTILSHELPER_H__

#include "_CUrlUtilsHelper.h"

namespace Libcore {
namespace Net {
namespace Url {

CarClass(CUrlUtilsHelper)
{
public:
    CARAPI CanonicalizePath(
        /* [in] */ const String& path,
        /* [in] */ Boolean discardRelativePrefix,
        /* [out] */ String* str);

    CARAPI AuthoritySafePath(
        /* [in] */ const String& authority,
        /* [in] */ const String& path,
        /* [out] */ String* str);

    CARAPI GetSchemePrefix(
        /* [in] */ const String& spec,
        /* [out] */ String* str);

    CARAPI IsValidSchemeChar(
        /* [in] */ Int32 index,
        /* [in] */ Char32 c,
        /* [out] */ Boolean* value);

    CARAPI FindFirstOf(
        /* [in] */ const String& string,
        /* [in] */ const String& chars,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ Int32* value);
};

} // namespace Url
} // namespace Net
} // namespace Libcore

#endif //__CURLUTILSHELPER_H__
