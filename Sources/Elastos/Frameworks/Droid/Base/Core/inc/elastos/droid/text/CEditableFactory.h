
#ifndef __ELASTOS_DROID_TEXT_CEDITABLEFACTORY_H__
#define __ELASTOS_DROID_TEXT_CEDITABLEFACTORY_H__

#include "_Elastos_Droid_Text_CEditableFactory.h"
#include <elastos/droid/text/EditableFactory.h>

namespace Elastos {
namespace Droid {
namespace Text {

CarClass(EditableFactory)
    , public EditableFactory
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_TEXT_CEDITABLEFACTORY_H__
