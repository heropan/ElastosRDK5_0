
#ifndef __ELASTOS_TEXT_CSIMPLEDATEFORMAT_H__
#define __ELASTOS_TEXT_CSIMPLEDATEFORMAT_H__

#include "_Elastos_Text_CSimpleDateFormat.h"
#include "SimpleDateFormat.h"
#include "DateFormatMacro.h"

using Elastos::Utility::ILocale;
using Elastos::Text::IFieldPosition;
using Elastos::Text::IParsePosition;
using Elastos::Utility::ICurrency;
using Elastos::Utility::IDate;

namespace Elastos {
namespace Text {

CarClass(CSimpleDateFormat) , public SimpleDateFormat
{
public:
    DATEFORMAT_METHODS_DECL()

    CARAPI FormatObject(
        /* [in] */ IInterface* object,
        /* [in] */ IStringBuffer * buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ IStringBuffer ** value);

    CARAPI ParseObject(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ IInterface** object);

    CARAPI FormatDate(
        /* [in] */ IDate* date,
        /* [in] */ IStringBuffer * buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ IStringBuffer ** result);


    CARAPI Parse(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ IDate** date);

    CARAPI ApplyLocalizedPattern(
        /* [in] */ const String& tem);

    CARAPI ApplyPattern(
        /* [in] */ const String& tem);

    CARAPI Get2DigitYearStart(
        /* [out] */ IDate** date);

    CARAPI GetDateFormatSymbols(
        /* [out] */ IDateFormatSymbols** dateFormatSymbols);

    CARAPI Set2DigitYearStart(
        /* [in] */ IDate* date);

    CARAPI SetDateFormatSymbols(
        /* [in] */ IDateFormatSymbols* value);

    CARAPI ToLocalizedPattern(
        /* [out] */ String* pattern);

    CARAPI ToPattern(
        /* [out] */ String* pattern);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& pattern);

    CARAPI constructor(
        /* [in] */ const String& temp,
        /* [in] */ IDateFormatSymbols* value);

    CARAPI constructor(
        /* [in] */ const String& temp,
        /* [in] */ ILocale* locale);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);
};

} // namespace Text
} // namespace Elastos

#endif // __ELASTOS_TEXT_CSIMPLEDATEFORMAT_H__
