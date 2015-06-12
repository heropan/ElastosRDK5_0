
#ifndef __ELASTOS_TEXT_CCOLLATORHELPER_H__
#define __ELASTOS_TEXT_CCOLLATORHELPER_H__

#include "_Elastos_Text_CCollatorHelper.h"

using Elastos::Utility::ILocale;

namespace Elastos {
namespace Text {

CarClass(CCollatorHelper)
{
public:
    CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetInstance(
        /* [out] */ ICollator** instance);

    CARAPI GetInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ ICollator** instance);
};

} // namespace Text
} // namespace Elastos

#endif // __ELASTOS_TEXT_CCOLLATORHELPER_H__
