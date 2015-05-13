
#ifndef __CEDITABLEFACTORY_H__
#define __CEDITABLEFACTORY_H__

#include "_CEditableFactory.h"

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

#endif //__CEDITABLEFACTORY_H__