#include "ChoiceFormat.h"
#include <elastos/Math.h>
#include <elastos/StringBuffer.h>
#include <elastos/StringBuilder.h>
#include <elastos/Character.h>
#include <elastos/StringUtils.h>
#include "CChoiceFormat.h"
#include "CStringWrapper.h"
#include "CDouble.h"

using Elastos::Core::StringUtils;
using Elastos::Core::Character;
using Elastos::Core::StringBuffer;
using Elastos::Core::StringBuilder;
using Elastos::Core::Math;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Core::CDouble;
using Elastos::Core::IDouble;
using Elastos::Text::IChoiceFormat;
using Elastos::Text::EIID_IChoiceFormat;
using Elastos::Text::CParsePosition;

namespace Elastos {
namespace Text {

ChoiceFormat::~ChoiceFormat()
{
}

ECode ChoiceFormat::Init(
    /* [in] */ const ArrayOf<Double>& limits,
    /* [in] */ const ArrayOf<String>& formats)
{
    return SetChoices(limits, formats);
}

ECode ChoiceFormat::Init(
    /* [in] */ const String& tem)
{
    return ApplyPattern(tem);
}

ECode ChoiceFormat::ApplyPattern(
    /* [in] */ const String& tem)
{
    AutoPtr<ArrayOf<Double> > limits = ArrayOf<Double>::Alloc(5);
    List<String> formats;
    AutoPtr<ArrayOf<Char32> > charArray = tem.GetChars();
    Int32 length = charArray->GetLength();
    Int32 limitCount = 0, index = 0;
    StringBuffer buffer("");

    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> uslocale;
    localeHelper->GetUS((ILocale**)&uslocale);
    AutoPtr<INumberFormat> format;
    NumberFormat::GetInstance(uslocale, (INumberFormat**)&format);

    AutoPtr<IParsePosition> position;
    CParsePosition::New(0, (IParsePosition**)&position);

    Char32 ch;
    Int32 errorIndex;
    Double next;
    while (TRUE) {
        index = SkipWhitespace(tem, index);
        if (index >= length) {
            if (limitCount == limits->GetLength()) {
                mChoiceLimits = limits;
            }
            else {
                mChoiceLimits = ArrayOf<Double>::Alloc(limitCount);
                for (Int32 i = 0; i < limitCount; i++) {
                    (*mChoiceLimits)[i] = (*limits)[i];
                }
            }
            mChoiceFormats = ArrayOf<String>::Alloc(formats.GetSize());
            List<String>::Iterator it = formats.Begin();
            for (Int32 i = 0; it != formats.End(); ++it, ++i) {
                (*mChoiceFormats)[i] = *it;
            }
            return NOERROR;
        }

        position->SetIndex(index);
        AutoPtr<INumber> value;
        format->ParseEx(tem, position, (INumber**)&value);
        position->GetIndex(&index);
        index = SkipWhitespace(tem, index);
        position->GetErrorIndex(&errorIndex);
        if (errorIndex != -1 || index >= length) {
            // Fix Harmony 540
            mChoiceLimits = ArrayOf<Double>::Alloc(0);
            mChoiceFormats = ArrayOf<String>::Alloc(0);
            return NOERROR;
        }
        ch = (*charArray)[index++];
        if (limitCount == limits->GetLength()) {
            AutoPtr<ArrayOf<Double> > newLimits = ArrayOf<Double>::Alloc(limitCount * 2);
            for (Int32 i = 0; i < limitCount; i++) {
                (*newLimits)[i] = (*limits)[i];
            }
            limits = newLimits;
        }
        next = 0.0;
        switch (ch) {
            case '#':
            case 0x2264/*'\u2264'*/://'\u2264'
                value->DoubleValue(&next);
                break;
            case '<':
                Double d;
                value->DoubleValue(&d);
                next = NextDouble(d);
                break;
            default:
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        if (limitCount > 0 && next <= (*limits)[limitCount - 1]) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        buffer.Reset();
        position->SetIndex(index);
        Boolean succeeded;
        UpTo(tem, position, buffer, '|', &succeeded);
        position->GetIndex(&index);
        (*limits)[limitCount++] = next;
        formats.PushBack(buffer.ToString());
    }

    return NOERROR;
}

ECode ChoiceFormat::Clone(
    /* [out] */ IInterface ** outface)
{
    VALIDATE_NOT_NULL(outface);
    AutoPtr<IChoiceFormat> clone ;// = (ChoiceFormat) super.clone();
    FAIL_RETURN(CChoiceFormat::New(*mChoiceLimits,*mChoiceFormats,(IChoiceFormat **)&clone));

    *outface = clone;
    INTERFACE_ADDREF(*outface);
    return NOERROR;
}

ECode ChoiceFormat::Equals(
    /* [in] */ IInterface * object,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result)
    if (this->Probe(EIID_IChoiceFormat) == IChoiceFormat::Probe(object)) {
        *result = TRUE;
        return NOERROR;
    }
    if (object->Probe(EIID_IChoiceFormat) == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<IChoiceFormat> choice = (IChoiceFormat*) object;
    AutoPtr<ArrayOf<Double> > limits;
    choice->GetLimits((ArrayOf<Double>**)&limits);
    AutoPtr<ArrayOf<String> > formats;
    choice->GetChoiceFormats((ArrayOf<String>**)&formats);
    return mChoiceLimits->Equals(limits)
            && mChoiceFormats->Equals(formats);
}

ECode ChoiceFormat::FormatDoubleEx(
    /* [in] */ Double value,
    /* [in] */ IStringBuffer * inbuffer,
    /* [in] */ IFieldPosition * field,
    /* [out] */ IStringBuffer ** outbuffer)
{
    VALIDATE_NOT_NULL(inbuffer);
    VALIDATE_NOT_NULL(field);
    VALIDATE_NOT_NULL(outbuffer);
    for (Int32 i = mChoiceLimits->GetLength() - 1; i >= 0; i--) {
        if ((*mChoiceLimits)[i] <= value) {
            inbuffer->AppendString((*mChoiceFormats)[i]);
            *outbuffer = inbuffer;
            INTERFACE_ADDREF(*outbuffer);
            return NOERROR;
        }
    }
    if (mChoiceFormats->GetLength())
    {
        inbuffer->AppendString((*mChoiceFormats)[0]);
    }
    *outbuffer = inbuffer;
    INTERFACE_ADDREF(*outbuffer);
    return NOERROR;
}

ECode ChoiceFormat::FormatInt64Ex(
    /* [in] */ Int64 value,
    /* [in] */ IStringBuffer * inbuffer,
    /* [in] */ IFieldPosition * field ,
    /* [out] */ IStringBuffer ** outbuffer)
{
    return FormatDoubleEx((Double)value,inbuffer,field,outbuffer);
}

ECode ChoiceFormat::GetHashCode(
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value)
    Int32 hashCode = 0;
    for (Int32 i = 0; i < mChoiceLimits->GetLength(); i++) {
        Int64 v = Elastos::Core::Math::DoubleToInt64Bits((*mChoiceLimits)[i]);
        hashCode += (Int32) (v ^ (v >> 32)) + (*mChoiceFormats)[i].GetHashCode();
    }
    *value = hashCode;
    return NOERROR;
}

ECode ChoiceFormat::GetFormats(
    /* [out, callee] */ ArrayOf<IInterface*>** arrayOfFormattedString)
{
    VALIDATE_NOT_NULL(arrayOfFormattedString);
    AutoPtr<ArrayOf<IInterface*> > temp = ArrayOf<IInterface*>::Alloc(mChoiceFormats->GetLength());
    for (Int32 i = 0; i < mChoiceFormats->GetLength(); ++i) {
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New((*mChoiceFormats)[i], (ICharSequence**)&cs);
        temp->Set(i , (IInterface*)cs.Get());
    }
    *arrayOfFormattedString = temp;
    INTERFACE_ADDREF(*arrayOfFormattedString)
    return NOERROR;
}

ECode ChoiceFormat::GetChoiceFormats(
    /* [out, callee] */ ArrayOf<String>** formats)
{
    VALIDATE_NOT_NULL(formats)
    *formats = mChoiceFormats;
    INTERFACE_ADDREF(*formats)
    return NOERROR;
}

ECode ChoiceFormat::GetLimits(
    /* [out, callee] */ ArrayOf<Double>** arrayOfDoubles)
{
    VALIDATE_NOT_NULL(arrayOfDoubles);
    *arrayOfDoubles = mChoiceLimits;
    INTERFACE_ADDREF(*arrayOfDoubles)
    return NOERROR;
}

Double ChoiceFormat::NextDouble(
    /* [in] */ Double value)
{
    if (value == Elastos::Core::Math::DOUBLE_POSITIVE_INFINITY) {
        return value;
    }
    Int64 bits;
    // Handle -0.0
    if (value == 0) {
        bits = 0;
    }
    else {
        bits = Elastos::Core::Math::DoubleToInt64Bits(value);
    }

    return Elastos::Core::Math::Int64BitsToDouble(value < 0 ? bits - 1 : bits + 1);
}

Double ChoiceFormat::NextDouble(
    /* [in] */ Double value,
    /* [in] */ Boolean increment)
{
    return increment ? NextDouble(value) : PreviousDouble(value);
}

ECode ChoiceFormat::ParseEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ INumber** value)
{
    VALIDATE_NOT_NULL(value)

    Int32 offset = 0;
    position->GetIndex(&offset);
    String str;
    for (Int32 i = 0; i < mChoiceFormats->GetLength(); i++) {
        str = string.Substring(offset, (*mChoiceFormats)[i].GetLength());
        if (str.StartWith((*mChoiceFormats)[i])) {
            position->SetIndex(offset + (*mChoiceFormats)[i].GetLength());
            AutoPtr<IDouble> outdouble;
            FAIL_RETURN(CDouble::New((*mChoiceLimits)[i],(IDouble **)&outdouble))
            *value = outdouble;
            INTERFACE_ADDREF(*value);
            return NOERROR;
        }
    }
    position->SetErrorIndex(offset);
    AutoPtr<IDouble> outdouble;
    FAIL_RETURN(CDouble::New(Elastos::Core::Math::DOUBLE_NAN,(IDouble **)&outdouble))
    *value = outdouble;
    INTERFACE_ADDREF(*value);
    return NOERROR;
}

Double ChoiceFormat::PreviousDouble(
    /* [in] */ Double value)
{
    if (value == Elastos::Core::Math::DOUBLE_NEGATIVE_INFINITY) {
        return value;
    }
    Int64 bits;
    // Handle 0.0
    if (value == 0) {
        bits = 0x8000000000000000ll;
    }
    else {
        bits = Elastos::Core::Math::DoubleToInt64Bits(value);
    }
    return Elastos::Core::Math::Int64BitsToDouble(value <= 0 ? bits + 1 : bits - 1);
}

ECode ChoiceFormat::SetChoices(
    /* [in] */ const ArrayOf<Double>& limits,
    /* [in] */ const ArrayOf<String>& formats)
{
    if (limits.GetLength() != formats.GetLength()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mChoiceLimits = limits.Clone();
    mChoiceFormats = formats.Clone();
    return NOERROR;
}

Int32 ChoiceFormat::SkipWhitespace(
    /* [in] */ const String& string,
    /* [in] */ Int32 index)
{
    AutoPtr<ArrayOf<Char32> > charArray = string.GetChars();
    Int32 length = charArray->GetLength();
    while (index < length && Character::IsWhitespace((*charArray)[index])) {
        index++;
    }
    return index;
}

ECode ChoiceFormat::ToPattern(
    /* [out] */ String* pattern)
{
    VALIDATE_NOT_NULL(pattern);

    StringBuilder buffer;
    for (Int32 i = 0; i < mChoiceLimits->GetLength(); i++) {
        if (i != 0) {
            buffer.AppendChar('|');
        }
        String previous = StringUtils::DoubleToString(PreviousDouble((*mChoiceLimits)[i]));
        String limit = StringUtils::DoubleToString((*mChoiceLimits)[i]);
        if (previous.GetLength() < limit.GetLength()) {
            buffer.AppendString(previous);
            buffer.AppendChar('<');
        } else {
            buffer.AppendString(limit);
            buffer.AppendChar('#');
        }
        Boolean quote = ((*mChoiceFormats)[i].IndexOf('|') != -1);
        if (quote) {
            buffer.AppendChar('\'');
        }
        buffer.AppendString((*mChoiceFormats)[i]);
        if (quote) {
            buffer.AppendChar('\'');
        }
    }

    return buffer.Substring(0, pattern);
}

} // namespace Text
} // namespace Elastos
