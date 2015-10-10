
#ifndef __ELASTOS_DROID_TEXT_CEDITABLEFACTORY_H__
#define __ELASTOS_DROID_TEXT_CEDITABLEFACTORY_H__

#include "_Elastos_Droid_Text_CEditableFactory.h"
#include <elastos/core/Singleton.h>

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CEditableFactory)
{
public:
    CARAPI NewEditable(
        /* [in] */ ICharSequence* source,
        /* [out] */ IEditable** editable);

};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_TEXT_CEDITABLEFACTORY_H__
