
#ifndef __CCOLLATORHELPER_H__
#define __CCOLLATORHELPER_H__

#include "_CCollatorHelper.h"
#include <cmdef.h>

using Libcore::ICU::ILocale;

namespace Elastos {
namespace Text {

CarClass(CCollatorHelper)
{
public:
    CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetInstance(
        /* [out] */ ICollator** instance);

    CARAPI GetInstanceEx(
        /* [in] */ ILocale* locale,
        /* [out] */ ICollator** instance);
};

} // namespace Text
} // namespace Elastos

#endif // __CCOLLATORHELPER_H__
