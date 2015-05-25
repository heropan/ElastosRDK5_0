
#ifndef __ICU_CNATIVEPLURALRULESHELPER_H__
#define __ICU_CNATIVEPLURALRULESHELPER_H__

#include "_CNativePluralRulesHelper.h"
#include "cmdef.h"

namespace Libcore {
namespace ICU {

CarClass(CNativePluralRulesHelper)
{
public:
    CARAPI ForLocale(
        /* [in] */ ILocale * locale,
        /* [out] */ INativePluralRules ** outrules);
};

} // namespace ICU
} // namespace Libcore

#endif //__ICU_CNATIVEPLURALRULESHELPER_H__
