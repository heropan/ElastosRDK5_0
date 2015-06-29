#ifndef __ELASTOS_TEXT_SIMPLEDATEFORMAT_H__
#define __ELASTOS_TEXT_SIMPLEDATEFORMAT_H__

#include "DateFormat.h"
#include <elastos/utility/etl/List.h>
#include "StringBuffer.h"
#include "Character.h"

using Elastos::Core::StringBuffer;
using Elastos::Utility::Etl::List;
using Elastos::Core::Character;
using Elastos::Core::INumber;
using Elastos::Text::IAttributedCharacterIterator;;
using Elastos::Text::IFieldPosition;
using Elastos::Text::IParsePosition;

extern "C" const InterfaceID EIID_SimpleDateFormat;

namespace Elastos {
namespace Text {

class SimpleDateFormat
    : public DateFormat
    , public ISimpleDateFormat
{
public:
    CAR_INTERFACE_DECL()

    CARAPI constructor();

    virtual ~SimpleDateFormat();

    CARAPI constructor(
        /* [in] */ const String& pattern);

    CARAPI constructor(
        /* [in] */ const String& tem,
        /* [in] */ IDateFormatSymbols* value);

    CARAPI constructor(
        /* [in] */ const String& tem,
        /* [in] */ ILocale* locale);

    CARAPI constructor(
        /* [in] */ ILocale* locale);

    virtual CARAPI ApplyLocalizedPattern(
        /* [in] */ const String& tem);

    virtual CARAPI ApplyPattern(
        /* [in] */ const String& tem);

    //@Override
    CARAPI FormatToCharacterIterator(
        /* [in] */ IInterface* object,
        /* [out] */ IAttributedCharacterIterator** charactorIterator);

    //@Override
    CARAPI Format(
        /* [in] */ IDate* date,
        /* [in] */ IStringBuffer * buffer,
        /* [in] */ IFieldPosition* fieldPos,
        /* [out] */ IStringBuffer ** formatString);

    virtual CARAPI Get2DigitYearStart(
        /* [out] */ IDate** start);

    virtual CARAPI GetDateFormatSymbols(
        /* [out] */ IDateFormatSymbols** symbols);

    //@Override
    CARAPI Parse(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ IDate** date);

    virtual CARAPI Set2DigitYearStart(
        /* [in] */ IDate* date);

    virtual CARAPI SetDateFormatSymbols(
        /* [in] */ IDateFormatSymbols* value);

    virtual CARAPI ToLocalizedPattern(
        /* [out] */ String* pattern);

    virtual CARAPI ToPattern(
        /* [out] */ String* pattern);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI Clone(
        /* [out] */ IInterface** outface);

private:
    CARAPI ValidateFormat(
        /* [in] */ Char32 format);

    CARAPI ValidatePattern(
        /* [in] */ const String& tem);

    static CARAPI DefaultPattern(
        /* [out] */ String* pattern);

    CARAPI FormatToCharacterIteratorImpl(
        /* [in] */ IDate* date,
        /* [out] */ IAttributedCharacterIterator** charactorIterator);

    CARAPI FormatImpl(
        /* [in] */ IDate* date,
        /* [in] */ StringBuffer* buffer,
        /* [in] */ IFieldPosition* field,
        /* [in] */ List< AutoPtr<IFieldPosition> >* fields,
        /* [out] */ StringBuffer* formattedDate);

    CARAPI Append(
        /* [in] */ StringBuffer* buffer,
        /* [in] */ IFieldPosition* position,
        /* [in] */ List< AutoPtr<IFieldPosition> >* fields,
        /* [in] */ Char32 format,
        /* [in] */ Int32 count);

    CARAPI AppendDayOfWeek(
        /* [in] */ StringBuffer* buffer,
        /* [in] */ Int32 count,
        /* [in] */ ArrayOf<String>* longs,
        /* [in] */ ArrayOf<String>* shorts);

    CARAPI AppendMonth(
        /* [in] */ StringBuffer* buffer,
        /* [in] */ Int32 count,
        /* [in] */ ArrayOf<String>* longs,
        /* [in] */ ArrayOf<String>* shorts);

    CARAPI AppendTimeZone(
        /* [in] */ StringBuffer* buffer,
        /* [in] */ Int32 count,
        /* [in] */ Boolean generalTimeZone);

    CARAPI AppendNumericTimeZone(
        /* [in] */ StringBuffer* buffer,
        /* [in] */ Boolean generalTimeZone);

    CARAPI AppendNumber(
        /* [in] */ StringBuffer* buffer,
        /* [in] */ Int32 count,
        /* [in] */ Int32 value);

    CARAPI Error(
        /* [in] */ IParsePosition* position,
        /* [in] */ Int32 offset,
        /* [in] */ ITimeZone* zone,
        /* [out] */ IDate** date);

    CARAPI Parse(
        /* [in] */ const String& string,
        /* [in] */ Int32 offset,
        /* [in] */ Char32 format,
        /* [in] */ Int32 count,
        /* [out] */ Int32* value);

    CARAPI ParseDayOfWeek(
        /* [in] */ const String& string,
        /* [in] */ Int32 offset,
        /* [in] */ ArrayOf<String>* longs,
        /* [in] */ ArrayOf<String>* shorts,
        /* [out] */ Int32* value);

    CARAPI ParseMonth(
        /* [in] */ const String& string,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ Int32 absolute,
        /* [in] */ ArrayOf<String>* longs,
        /* [in] */ ArrayOf<String>* shorts,
        /* [out] */ Int32* value);

    CARAPI ParseNumber(
        /* [in] */ Int32 max,
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ INumber** number);

    CARAPI ParseNumber(
        /* [in] */ Int32 max,
        /* [in] */ const String& string,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 field,
        /* [in] */ Int32 skew,
        /* [out] */ Int32* value);

    CARAPI ParseText(
        /* [in] */ const String& string,
        /* [in] */ Int32 offset,
        /* [in] */ ArrayOf<String>* text,
        /* [in] */ Int32 field,
        /* [out] */ Int32* value);

    CARAPI ParseTimeZone(
        /* [in] */ const String& string,
        /* [in] */ Int32 offset,
        /* [out] */ Int32* value);

    static CARAPI ConvertPattern(
        /* [in] */ const String& tem,
        /* [in] */ const String& fromChars,
        /* [in] */ const String& toChars,
        /* [in] */ Boolean check,
        /* [out] */ String* pattern);

    //CARAPI WriteObject(ObjectOutputStream stream);

    //CARAPI ReadObject(ObjectInputStream stream);

protected:
    const static String PATTERN_CHARS;

private:
    //const static Int64 serialVersionUID = 4774881970558875024L;

    const static Int32 RFC_822_TIMEZONE_FIELD;

    const static Int32 STAND_ALONE_MONTH_FIELD;

    const static Int32 STAND_ALONE_DAY_OF_WEEK_FIELD;

    String mPattern;

    AutoPtr<IDateFormatSymbols> mFormatData;

    mutable Int32 mCreationYear;

    AutoPtr<IDate> mDefaultCenturyStart;

//    private static final ObjectStreamField[] serialPersistentFields = {
//            new ObjectStreamField("defaultCenturyStart", Date.class),
//            new ObjectStreamField("formatData", DateFormatSymbols.class),
//            new ObjectStreamField("pattern", String.class),
//            new ObjectStreamField("serialVersionOnStream", int.class), };
};

} // namespace Text
} // namespace Elastos

#endif //__ELASTOS_TEXT_SIMPLEDATEFORMAT_H__
