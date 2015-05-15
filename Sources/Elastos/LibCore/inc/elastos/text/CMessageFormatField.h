
#ifndef __CMESSAGEFORMATFIELD_H__
#define __CMESSAGEFORMATFIELD_H__

#include "_CMessageFormatField.h"
#include <cmdef.h>
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

#endif // __CMESSAGEFORMATFIELD_H__
