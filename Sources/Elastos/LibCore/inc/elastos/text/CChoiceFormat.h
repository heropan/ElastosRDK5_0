
#ifndef __CCHOICEFORMAT_H__
#define __CCHOICEFORMAT_H__

#include "_CChoiceFormat.h"
#include <cmdef.h>
#include "ChoiceFormat.h"
#include "NumberFormatMacro.h"

using Elastos::Core::IStringBuffer;
using Elastos::Text::IFieldPosition;
using Elastos::Text::IParsePosition;

namespace Elastos {
namespace Text {

CarClass(CChoiceFormat) , public ChoiceFormat
{
public:
    NUMBERFORMAT_METHODS_DECL()

    CARAPI Parse(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ INumber** value);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Double>& limits,
        /* [in] */ const ArrayOf<String>& formats);

    CARAPI constructor(
        /* [in] */ const String& tem);

    CARAPI ApplyPattern(
        /* [in] */ const String& tem);

    CARAPI GetFormats(
        /* [out, callee] */ ArrayOf<IInterface*>** formats);

    CARAPI GetChoiceFormats(
        /* [out, callee] */ ArrayOf<String>** formats);

    CARAPI GetLimits(
        /* [out, callee] */ ArrayOf<Double>** limits);

    CARAPI SetChoices(
        /* [in] */ const ArrayOf<Double>& limits,
        /* [in] */ const ArrayOf<String>& formats);

    CARAPI ToPattern(
        /* [out] */ String* pattern);

    CARAPI Clone(
        /* [out] */ IInterface ** outface);

    CARAPI Equals(
        /* [in] */ IInterface * object,
        /* [out] */ Boolean * value);

    CARAPI GetHashCode(
        /* [out] */ Int32 * value);

    CARAPI FormatDouble(
        /* [in] */ Double value,
        /* [in] */ IStringBuffer * inbuffer,
        /* [in] */ IFieldPosition * field,
        /* [out] */ IStringBuffer ** outbuffer);

    CARAPI FormatInt64(
        /* [in] */ Int64 value,
        /* [in] */ IStringBuffer * inbuffer,
        /* [in] */ IFieldPosition * field ,
        /* [out] */ IStringBuffer ** outbuffer);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

} // namespace Text
} // namespace Elastos

#endif // __CCHOICEFORMAT_H__
