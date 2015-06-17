#ifndef __UTILITY_CVECTOR_H__
#define __UTILITY_CVECTOR_H__

#include "_Elastos_Utility_CVector.h"
#include "Vector.h"


namespace Elastos {
namespace Utility {

CarClass(CVector)
    , public Vector
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_CVECTOR_H__