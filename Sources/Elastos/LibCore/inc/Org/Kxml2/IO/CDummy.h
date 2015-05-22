
#ifndef  ___KXML2_CDUMMY_H__
#define  ___KXML2_CDUMMY_H__

#include "_Org_Kxml2_IO_CDummy.h"
#include <elastos/core/Object.h>

using Elastos::Core::Object;

namespace Org {
namespace Kxml2 {
namespace IO {


/**
 * Dummy class
 */
CarClass(CDummy)
    , public Object
{
public:
    CAR_OBJECT_DECL()
};


} //namespace IO
} //namespace Kxml2
} //namespace Org

#endif //__CKXMLPARSER_H__
