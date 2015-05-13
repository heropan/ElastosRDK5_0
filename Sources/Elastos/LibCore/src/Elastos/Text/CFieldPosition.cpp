
#include "cmdef.h"
#include "CFieldPosition.h"

using Elastos::Text::EIID_IFieldPosition;

namespace Elastos {
namespace Text {

CFieldPosition::CFieldPosition()
    : mMyField(0)
    , mBeginIndex(0)
    , mEndIndex(0)
{}

ECode CFieldPosition::constructor(
    /* [in] */ Int32 field)
{
    mMyField = field;
    return NOERROR;
}

ECode CFieldPosition::constructor(
    /* [in] */ IAttributedCharacterIteratorAttribute* attribute)
{
    mMyAttribute = attribute;
    mMyField = -1;
    return NOERROR;
}

ECode CFieldPosition::constructor(
    /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
    /* [in] */ Int32 field)
{
    mMyAttribute = attribute;
    mMyField = field;
    return NOERROR;
}

ECode CFieldPosition::Clear()
{
    mBeginIndex = mEndIndex = 0;
    return NOERROR;
}

ECode CFieldPosition::GetBeginIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    *index = mBeginIndex;
    return NOERROR;
}

ECode CFieldPosition::GetEndIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    *index = mEndIndex;
    return NOERROR;
}

ECode CFieldPosition::GetField(
    /* [out] */ Int32* field)
{
    VALIDATE_NOT_NULL(field);

    *field = mMyField;
    return NOERROR;
}

ECode CFieldPosition::GetFieldAttribute(
    /* [out] */ IAttributedCharacterIteratorAttribute** attribute)
{
    VALIDATE_NOT_NULL(attribute);

    *attribute = mMyAttribute;
    INTERFACE_ADDREF(*attribute);
    return NOERROR;
}

ECode CFieldPosition::SetBeginIndex(
    /* [in] */ Int32 index)
{
    mBeginIndex = index;
    return NOERROR;
}

ECode CFieldPosition::SetEndIndex(
    /* [in] */ Int32 index)
{
    mEndIndex = index;
    return NOERROR;
}

ECode CFieldPosition::Equals(
    /* [in] */ IInterface * object,
    /* [out] */ Boolean * value)
{
    AutoPtr<IFieldPosition> pos = (IFieldPosition *)object->Probe(EIID_IFieldPosition);
    if (!pos) {
        *value = FALSE;
    } else {
        *value = mMyField == ((CFieldPosition *)pos.Get())->mMyField &&
                 mMyAttribute == ((CFieldPosition *)pos.Get())->mMyAttribute &&
                 mBeginIndex == ((CFieldPosition *)pos.Get())->mBeginIndex &&
                 mEndIndex == ((CFieldPosition *)pos.Get())->mEndIndex;
    }
    return NOERROR;
}

ECode CFieldPosition::GetHashCode(
    /* [out] */ Int32 * value)
{
    Int32 attributeHash = 0;
    if (mMyAttribute)
        mMyAttribute->GetHashCode(&attributeHash);
    *value = attributeHash + mMyField * 10 + mBeginIndex * 100 + mEndIndex;
    return NOERROR;
}

ECode CFieldPosition::ToString(
    /* [out] */ String * str)
{
    // return getClass().getName() + "[attribute=" + myAttribute + ", field="
    //     + myField + ", beginIndex=" + beginIndex + ", endIndex="
    //     + endIndex + "]";
    return NOERROR;
}

} // namespace Text
} // namespace Elastos
