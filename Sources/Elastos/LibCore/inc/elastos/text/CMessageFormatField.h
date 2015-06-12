
#ifndef __ELASTOS_TEXT_CMESSAGEFORMATFIELD_H__
#define __ELASTOS_TEXT_CMESSAGEFORMATFIELD_H__

#include "_Elastos_Text_CMessageFormatField.h"
#include <AttributedCharacterIteratorAttribute.h>
#include <AttributedCharacterIteratorAttributeMacro.h>

namespace Elastos {
namespace Text {

CarClass(CMessageFormatField) , public AttributedCharacterIteratorAttribute
{
public:
    TEXTATTRIBUITEDCHARACTERITERATORATTRIBUTE_METHODS_DECL()

    CARAPI GetClassID(
        /* [out] */ ClassID* clsid);

    CARAPI constructor(
        /* [in] */ const String& name);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

} // namespace Text
} // namespace Elastos

#endif // __ELASTOS_TEXT_CMESSAGEFORMATFIELD_H__
