
#ifndef __CMESSAGEFORMAT_H__
#define __CMESSAGEFORMAT_H__

#include "_CMessageFormat.h"
#include "MessageFormat.h"

using Libcore::ICU::ILocale;
using Elastos::Text::IAttributedCharacterIterator;;
using Elastos::Text::IFieldPosition;
using Elastos::Text::IParsePosition;

namespace Elastos {
namespace Text {

CarClass(CMessageFormat) , public MessageFormat
{
public:
    CARAPI FormatObject(
        /* [in] */ IInterface* object,
        /* [out] */ String* value);

    CARAPI FormatObjectEx(
        /* [in] */ IInterface* object,
        /* [in] */ IStringBuffer * buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ IStringBuffer ** value);

    CARAPI FormatToCharacterIterator(
        /* [in] */ IInterface* object,
        /* [out] */ IAttributedCharacterIterator** charactorIterator);

    CARAPI ParseObject(
        /* [in] */ const String& string,
        /* [out] */ IInterface** object);

    CARAPI ParseObjectEx(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ IInterface** object);

    CARAPI constructor(
        /* [in] */ const String& tem,
        /* [in] */ ILocale* locale);

    CARAPI constructor(
        /* [in] */ const String& tem);

    CARAPI ApplyPattern(
        /* [in] */ const String& temp);

    CARAPI FormatObjects(
        /* [in] */ ArrayOf<IInterface*>* objects,
        /* [in] */ IStringBuffer * buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ IStringBuffer ** value);

    CARAPI GetFormats(
        /* [out, callee] */ ArrayOf<IFormat*>** formats);

    CARAPI GetFormatsByArgumentIndex(
        /* [out, callee] */ ArrayOf<IFormat*>** formatsByArgumentIndex);

    CARAPI SetFormatByArgumentIndex(
        /* [in] */ Int32 argIndex,
        /* [in] */ IFormat* format);

    CARAPI SetFormatsByArgumentIndexEx(
        /* [in] */ ArrayOf<IFormat*>* formats);

    CARAPI GetLocale(
        /* [out] */ ILocale** locale);

    CARAPI Parse(
        /* [in] */ const String& string,
        /* [out, callee] */ ArrayOf<IInterface*>** result);

    CARAPI ParseEx(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out, callee] */ ArrayOf<IInterface*>** result);

    CARAPI SetFormat(
        /* [in] */ Int32 offset,
        /* [in] */ IFormat* format);

    CARAPI SetFormats(
        /* [in] */ ArrayOf<IFormat*>* formats);

    CARAPI SetLocale(
        /* [in] */ ILocale* locale);

    CARAPI ToPattern(
        /* [out] */ String* pattern);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetMaxOffset(
        /* [out] */ Int32* maxOffset);

    CARAPI GetArgumentNumbers(
        /* [out,callee] */ ArrayOf<Int32>** argumentNumbers);

    CARAPI GetStrings(
        /* [out,callee] */ ArrayOf<String>** strings);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);
};

} // namespace Text
} // namespace Elastos

#endif // __CMESSAGEFORMAT_H__
