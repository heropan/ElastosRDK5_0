#ifndef __CPROPERTYHELPER_H__
#define __CPROPERTYHELPER_H__
#include "_CPropertyHelper.h"

namespace Elastos {
namespace Droid {
namespace Utility {

CarClass(CPropertyHelper)
{
public:
    CPropertyHelper();

    CARAPI constructor();

    CARAPI Of(
        /* [in] */ const ClassID& clsId,
        /* [in] */ CarDataType dataType,
        /* [in] */ const String& name,
        /* [out] */ IProperty** property);

};

} // namespace Utility
} // namepsace Droid
} // namespace Elastos

#endif
