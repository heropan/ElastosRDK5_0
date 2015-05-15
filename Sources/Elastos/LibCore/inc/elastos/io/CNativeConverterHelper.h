
#ifndef __IO_CNATIVECONVERTERHELPER_H__
#define __IO_CNATIVECONVERTERHELPER_H__

#include "_CNativeConverterHelper.h"
#include "cmdef.h"

using Elastos::IO::Charset::ICharset;
using Elastos::IO::Charset::ICharsetDecoder;
using Elastos::IO::Charset::ICharsetEncoder;
using Elastos::IO::Charset::ICodingErrorAction;

namespace Elastos {
namespace IO {

CarClass(CNativeConverterHelper)
{
public:
    CARAPI constructor();

    CARAPI Decode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ const ArrayOf<Byte>& input,
        /* [in] */ Int32 inEnd,
        /* [in] */ const ArrayOf<Char32>& output,
        /* [in] */ Int32 outEnd,
        /* [in] */ const ArrayOf<Int32>& data,
        /* [in] */ Boolean flush,
        /* [out] */ Int32* result);

    CARAPI Encode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ const ArrayOf<Char32>& input,
        /* [in] */ Int32 inEnd,
        /* [in] */ const ArrayOf<Byte>& output,
        /* [in] */ Int32 outEnd,
        /* [in] */ const ArrayOf<Int32>& data,
        /* [in] */ Boolean flush,
        /* [out] */ Int32* result);

    CARAPI OpenConverter(
        /* [in] */ const String& encoding,
        /* [out] */ Int64 * value);

    CARAPI CloseConverter(
        /* [in] */ Int64 converterHandle);

    CARAPI ResetByteToChar(
        /* [in] */ Int64 converterHandle);

    CARAPI ResetCharToByte(
        /* [in] */ Int64 converterHandle);

    CARAPI GetSubstitutionBytes(
        /* [in] */ Int64 converterHandle,
        /* [out,callee] */ ArrayOf<Byte> ** outarray);

    CARAPI GetMaxBytesPerChar(
        /* [in] */ Int64 converterHandle,
        /* [out] */ Int32 * value);

    CARAPI GetMinBytesPerChar(
        /* [in] */ Int64 converterHandle,
        /* [out] */ Int32 * value);

    CARAPI GetAveBytesPerChar(
        /* [in] */ Int64 converterHandle,
        /* [out] */ Float * value);

    CARAPI GetAveCharsPerByte(
        /* [in] */ Int64 converterHandle,
        /* [out] */ Float * value);

    CARAPI Contains(
        /* [in] */ const String& converterName1,
        /* [in] */ const String& converterName2,
        /* [out] */ Boolean * value);

    CARAPI CanEncode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ Int32 codeUnit,
        /* [out] */ Boolean * value);

    CARAPI GetAvailableCharsetNames(
        /* [out,callee] */ ArrayOf<String> ** outarray);

    CARAPI CharsetForName(
        /* [in] */ const String& charsetName,
        /* [out] */ ICharset ** outset);

    // Translates from Java's enum to the magic numbers #defined in "NativeConverter.cpp".
    CARAPI TranslateCodingErrorAction(
        /* [in] */ ICodingErrorAction * errorAction,
        /* [out] */ Int32 * value);

    CARAPI SetCallbackDecode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ ICharsetDecoder * decoder);

    CARAPI SetCallbackEncode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ ICharsetEncoder * encoder);
};

} // namespace IO
} // namespace Elastos

#endif //__IO_CNATIVECONVERTERHELPER_H__
