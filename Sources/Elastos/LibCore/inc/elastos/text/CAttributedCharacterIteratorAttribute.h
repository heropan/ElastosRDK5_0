
#ifndef __CATTRIBUTEDCHARACTERITERATORATTRIBUTE_H__
#define __CATTRIBUTEDCHARACTERITERATORATTRIBUTE_H__

#include "_CAttributedCharacterIteratorAttribute.h"
#include "AttributedCharacterIteratorAttribute.h"

namespace Elastos {
namespace Text {

CarClass(CAttributedCharacterIteratorAttribute) , public AttributedCharacterIteratorAttribute
{
public:
    CARAPI constructor(
        /* [in] */ const String& name);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetClassID(
        /* [out] */ ClassID* clsid);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI Equals(
        /* [in] */ IInterface * obj,
        /* [out] */ Boolean * value);

    CARAPI GetHashCode(
        /* [out] */ Int32 * value);

    CARAPI ReadResolve(
        /* [out] */ IInterface** resolve);

    CARAPI ToString(
        /* [out] */ String * str);
};

} // namespace Text
} // namespace Elastos

#endif // __CATTRIBUTEDCHARACTERITERATORATTRIBUTE_H__
