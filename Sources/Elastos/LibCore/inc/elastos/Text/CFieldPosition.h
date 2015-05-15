
#ifndef __CFIELDPOSITION_H__
#define __CFIELDPOSITION_H__

#include "_CFieldPosition.h"

namespace Elastos {
namespace Text {

CarClass(CFieldPosition)
{
public:
    CFieldPosition();

    CARAPI constructor(
        /* [in] */ Int32 field);

    CARAPI constructor(
        /* [in] */ IAttributedCharacterIteratorAttribute* attribute);

    CARAPI constructor(
        /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
        /* [in] */ Int32 field);

    CARAPI Clear();

    CARAPI GetBeginIndex(
        /* [out] */ Int32* index);

    CARAPI GetEndIndex(
        /* [out] */ Int32* index);

    CARAPI GetField(
        /* [out] */ Int32* field);

    CARAPI GetFieldAttribute(
        /* [out] */ IAttributedCharacterIteratorAttribute** attribute);

    CARAPI SetBeginIndex(
        /* [in] */ Int32 index);

    CARAPI SetEndIndex(
        /* [in] */ Int32 index);

    CARAPI Equals(
        /* [in] */ IInterface * object,
        /* [out] */ Boolean * value);

    CARAPI GetHashCode(
    /* [out] */ Int32 * value);

    CARAPI ToString(
    /* [out] */ String * str);

public:
    Int32 mMyField;
    Int32 mBeginIndex;
    Int32 mEndIndex;

    AutoPtr<IAttributedCharacterIteratorAttribute> mMyAttribute;
};

} // namespace Text
} // namespace Elastos

#endif // __CFIELDPOSITION_H__
