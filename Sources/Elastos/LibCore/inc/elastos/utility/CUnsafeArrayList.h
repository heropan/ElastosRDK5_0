#ifndef __UTILITY_CUNSAFEARRAYLIST_H__
#define __UTILITY_CUNSAFEARRAYLIST_H__

#include "_Elastos_Utility_CUnsafeArrayList.h"
#include "UnsafeArrayList.h"

namespace Elastos {
namespace Utility {

CarClass(CUnsafeArrayList)
    , public UnsafeArrayList
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_CUNSAFEARRAYLIST_H__
