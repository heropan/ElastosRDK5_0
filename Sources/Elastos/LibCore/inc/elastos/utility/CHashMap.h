#ifndef __UTILITY_CHASHMAP_H__
#define __UTILITY_CHASHMAP_H__

#include "_Elastos_Utility_CHashMap.h"
#include "HashMap.h"

namespace Elastos {
namespace Utility {

CarClass(CHashMap)
    , public HashMap
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_CHASHMAP_H__
