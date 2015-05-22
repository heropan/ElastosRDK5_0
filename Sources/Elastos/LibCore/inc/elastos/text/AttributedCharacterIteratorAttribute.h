
#ifndef __ATTRIBUTEDCHARACTERITERATORATTRIBUTE_H__
#define __ATTRIBUTEDCHARACTERITERATORATTRIBUTE_H__

#include "Elastos.CoreLibrary_server.h"

using Elastos::Text::IAttributedCharacterIteratorAttribute;

namespace Elastos {
namespace Text {

class AttributedCharacterIteratorAttribute
{
public:
    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    virtual CARAPI Equals(
        /* [in] */ IInterface * obj,
        /* [out] */ Boolean * value);

    virtual CARAPI GetHashCode(
        /* [out] */ Int32 * value);

    virtual CARAPI ToString(
        /* [out] */ String * str);

protected:
    CARAPI Init(
        /* [in] */ const String& name);

    virtual CARAPI GetClassID(
        /* [out] */ ClassID* clsid) = 0;

    virtual CARAPI GetName(
        /* [out] */ String * str);

    virtual CARAPI ReadResolve(
        /* [out] */ IInterface ** outface);

public:
    const static AutoPtr<IAttributedCharacterIteratorAttribute> INPUT_METHOD_SEGMENT;

    const static AutoPtr<IAttributedCharacterIteratorAttribute> LANGUAGE;

    const static AutoPtr<IAttributedCharacterIteratorAttribute> READING;

private:
    String mName;
};

} // namespace Text
} // namespace Elastos

#endif //__ATTRIBUTEDCHARACTERITERATORATTRIBUTE_H__
