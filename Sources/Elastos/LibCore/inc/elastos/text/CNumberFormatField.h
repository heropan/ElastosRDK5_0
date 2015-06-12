
#ifndef __ELASTOS_TEXT_CNUMBERFORMATFIELD_H__
#define __ELASTOS_TEXT_CNUMBERFORMATFIELD_H__

#include "_Elastos_Text_CNumberFormatField.h"
#include <AttributedCharacterIteratorAttribute.h>
#include <AttributedCharacterIteratorAttributeMacro.h>

namespace Elastos {
namespace Text {

CarClass(CNumberFormatField) , public AttributedCharacterIteratorAttribute
{
public:
    CARAPI constructor(
        /* [in] */ const String& fieldName);
};

} // namespace Text
} // namespace Elastos

#endif // __ELASTOS_TEXT_CNUMBERFORMATFIELD_H__
