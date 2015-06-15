
#ifndef __ICU_CLOCALEDATA_H__
#define __ICU_CLOCALEDATA_H__

#include "_Libcore_ICU_CLocaleData.h"
#include "LocaleData.h"

using Elastos::Core::IInteger32;

namespace Libcore {
namespace ICU {

CarClass(CLocaleData)
    , public LocaleData
{
public:
    CAR_OBJECT_DECL()
};

} // namespace ICU
} // namespace Libcore

#endif //__ICU_CLOCALEDATA_H__

