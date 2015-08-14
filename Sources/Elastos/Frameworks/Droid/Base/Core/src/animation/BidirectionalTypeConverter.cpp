
#include "BidirectionalTypeConverter.h"

namespace Elastos {
namespace Droid {
namespace Animation {

BidirectionalTypeConverter::InvertedConverter::InvertedConverter(
    /* [in] */ BidirectionalTypeConverter/*<To, From>*/* converter)
{
    super(converter.getTargetType(), converter.getSourceType());
    BidirectionalTypeConverter::constructor(fromClass, toClass);
    mConverter = converter;
}

ECode BidirectionalTypeConverter::InvertedConverter::ConvertBack(
    /* [in] */ IInterface* value,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    return mConverter->Convert(value, result);
}

ECode BidirectionalTypeConverter::InvertedConverter::Convert(
    /* [in] */ IInterface* value,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    return mConverter->ConvertBack(value);
}

BidirectionalTypeConverter::BidirectionalTypeConverter()
{}

BidirectionalTypeConverter::BidirectionalTypeConverter(
    /* [in] */ IMethodInfo* fromClass,
    /* [in] */ IMethodInfo* toClass)
{
    TypeConverter::constructor(fromClass, toClass);
}

AutoPtr<BidirectionalTypeConverter> BidirectionalTypeConverter::Invert()
{
    if (mInvertedConverter == NULL) {
        mInvertedConverter = new InvertedConverter(this);
    }
    return mInvertedConverter;
}

ECode BidirectionalTypeConverter::constructor(
    /* [in] */ IMethodInfo* fromClass,
    /* [in] */ IMethodInfo* toClass)
{
    return TypeConverter::constructor(fromClass, toClass);
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
