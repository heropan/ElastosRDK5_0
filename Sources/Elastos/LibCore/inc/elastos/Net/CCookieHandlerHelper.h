#ifndef __CCOOKIEHANDLERHELPER_H__
#define __CCOOKIEHANDLERHELPER_H__

#include "_CCookieHandlerHelper.h"

namespace Elastos {
namespace Net {

CarClass(CCookieHandlerHelper)
{
public:
    CARAPI GetDefault(
        /* [out] */ ICookieHandler** handler);

    CARAPI SetDefault(
        /* [in] */ ICookieHandler* handler);

private:
   AutoPtr<ICookieHandler> mSystemWideCookieHandler;
};

} // namespace Net
} // namespace Elastos

#endif
