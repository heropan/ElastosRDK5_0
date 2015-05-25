
#ifndef  ___XML_SAX_CDUMMY_H__
#define  ___XML_SAX_CDUMMY_H__

#include "_Org_Xml_Sax_CDummy.h"
#include <elastos/core/Object.h>

using Elastos::Core::Object;

namespace Org {
namespace Xml {
namespace Sax {


/**
 * Dummy class
 */
CarClass(CDummy)
    , public Object
{
public:
    CAR_OBJECT_DECL()
};


} //namespace Sax
} //namespace Xml
} //namespace Org

#endif //__CKXMLPARSER_H__
