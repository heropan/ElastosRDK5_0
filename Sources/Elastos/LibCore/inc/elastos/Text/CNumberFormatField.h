
#ifndef __CNUMBERFORMATFIELD_H__
#define __CNUMBERFORMATFIELD_H__

#include "_CNumberFormatField.h"
#include <cmdef.h>
#include <AttributedCharacterIteratorAttribute.h>
#include <AttributedCharacterIteratorAttributeMacro.h>

namespace Elastos {
namespace Text {

CarClass(CNumberFormatField) , public AttributedCharacterIteratorAttribute
{
public:
    TEXTATTRIBUITEDCHARACTERITERATORATTRIBUTE_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ const String& fieldName);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetClassID(
        /* [out] */ ClassID* clsid);
};

} // namespace Text
} // namespace Elastos

#endif // __CNUMBERFORMATFIELD_H__
