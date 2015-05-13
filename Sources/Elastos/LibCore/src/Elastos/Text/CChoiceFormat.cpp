
#include "CChoiceFormat.h"

namespace Elastos {
namespace Text {

NUMBERFORMAT_METHODS_IMPL(CChoiceFormat, ChoiceFormat)

ECode CChoiceFormat::constructor(
    /* [in] */ const ArrayOf<Double>& limits,
    /* [in] */ const ArrayOf<String>& formats)
{
    return ChoiceFormat::Init(limits,formats);
}

ECode CChoiceFormat::constructor(
    /* [in] */ const String& tem)
{
    return ChoiceFormat::Init(tem);
}

ECode CChoiceFormat::ApplyPattern(
    /* [in] */ const String& tem)
{
    return ChoiceFormat::ApplyPattern(tem);
}

ECode CChoiceFormat::GetFormats(
    /* [out, callee] */ ArrayOf<IInterface*>** formats)
{
    return ChoiceFormat::GetFormats(formats);
}

ECode CChoiceFormat::GetChoiceFormats(
    /* [out, callee] */ ArrayOf<String>** formats)
{
    return ChoiceFormat::GetChoiceFormats(formats);
}

ECode CChoiceFormat::GetLimits(
    /* [out, callee] */ ArrayOf<Double>** limits)
{
    return ChoiceFormat::GetLimits(limits);
}

ECode CChoiceFormat::SetChoices(
    /* [in] */ const ArrayOf<Double>& limits,
    /* [in] */ const ArrayOf<String>& formats)
{
    return ChoiceFormat::SetChoices(limits,formats);
}

ECode CChoiceFormat::ToPattern(
    /* [out] */ String* pattern)
{
    return ChoiceFormat::ToPattern(pattern);
}

ECode CChoiceFormat::Clone(
    /* [out] */ IInterface ** outface)
{
    return ChoiceFormat::Clone(outface);
}

ECode CChoiceFormat::Equals(
    /* [in] */ IInterface * object,
    /* [out] */ Boolean * value)
{
    return ChoiceFormat::Equals(object,value);
}

ECode CChoiceFormat::GetHashCode(
    /* [out] */ Int32 * value)
{
    return ChoiceFormat::GetHashCode(value);
}

ECode CChoiceFormat::ParseEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ INumber** value)
{
    return ChoiceFormat::ParseEx(string,position,value);
}

ECode CChoiceFormat::FormatDoubleEx(
    /* [in] */ Double value,
    /* [in] */ IStringBuffer * inbuffer,
    /* [in] */ IFieldPosition * field,
    /* [out] */ IStringBuffer ** outbuffer)
{
    return ChoiceFormat::FormatDoubleEx(value ,inbuffer ,field , outbuffer);
}

ECode CChoiceFormat::FormatInt64Ex(
    /* [in] */ Int64 value,
    /* [in] */ IStringBuffer * inbuffer,
    /* [in] */ IFieldPosition * field ,
    /* [out] */ IStringBuffer ** outbuffer)
{
    return ChoiceFormat::FormatInt64Ex(value , inbuffer , field , outbuffer);
}

PInterface CChoiceFormat::Probe(
    /* [in] */ REIID riid)
{
    return _CChoiceFormat::Probe(riid);
}

} // namespace Text
} // namespace Elastos
