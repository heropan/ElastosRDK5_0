
#ifndef __ICU_CTIMEZONENAMES_H__
#define __ICU_CTIMEZONENAMES_H__

#include "_Libcore_ICU_CTimeZoneNames.h"
#include "TimeZoneNames.h"

using Elastos::Core::IArrayOf;

namespace Libcore {
namespace ICU {

CarClass(CTimeZoneNames)
	, public TimeZoneNames
{
public:
    CAR_SINGLETON_DECL()
};

} // namespace ICU
} // namespace Libcore

#endif //__ICU_CTIMEZONENAMES_H__
