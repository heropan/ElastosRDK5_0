
#ifndef __NATIVECONVERTER_H__
#define __NATIVECONVERTER_H__

#include "Elastos.CoreLibrary_server.h"

using Elastos::IO::Charset::ICharset;
using Elastos::IO::Charset::ICharsetDecoder;
using Elastos::IO::Charset::ICharsetEncoder;
using Elastos::IO::Charset::ICodingErrorAction;

namespace Elastos {
namespace IO {

class NativeConverter
{
public:
    static CARAPI Decode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ ArrayOf<Byte>* input,
        /* [in] */ Int32 inEnd,
        /* [in] */ ArrayOf<Char32>* output,
        /* [in] */ Int32 outEnd,
        /* [in] */ ArrayOf<Int32>* data,
        /* [in] */ Boolean flush,
        /* [out] */ Int32* result);

    static CARAPI Encode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ ArrayOf<Char32>* input,
        /* [in] */ Int32 inEnd,
        /* [in] */ ArrayOf<Byte>* output,
        /* [in] */ Int32 outEnd,
        /* [in] */ ArrayOf<Int32>* data,
        /* [in] */ Boolean flush,
        /* [out] */ Int32* result);

    static CARAPI OpenConverter(
        /* [in] */ const String& encoding,
        /* [out] */ Int64* result);

    static CARAPI_(void) CloseConverter(
        /* [in] */ Int64 converterHandle);

    static CARAPI_(void) ResetByteToChar(
        /* [in] */ Int64 converterHandle);

    static CARAPI_(void) ResetCharToByte(
        /* [in] */ Int64 converterHandle);

    static CARAPI_(AutoPtr<ArrayOf<Byte> >) GetSubstitutionBytes(
        /* [in] */ Int64 converterHandle);

    static CARAPI_(Int32) GetMaxBytesPerChar(
        /* [in] */ Int64 converterHandle);

    static CARAPI_(Int32) GetMinBytesPerChar(
        /* [in] */ Int64 converterHandle);

    static CARAPI_(Float) GetAveBytesPerChar(
        /* [in] */ Int64 converterHandle);

    static CARAPI_(Float) GetAveCharsPerByte(
        /* [in] */ Int64 converterHandle);

    static CARAPI_(Boolean) Contains(
        /* [in] */ const String& converterName1,
        /* [in] */ const String& converterName2);

    static CARAPI_(Boolean) CanEncode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ Int32 codeUnit);

    static CARAPI_(AutoPtr<ArrayOf<String> >) GetAvailableCharsetNames();

    static CARAPI CharsetForName(
        /* [in] */ const String& charsetName,
        /* [out] */ ICharset** charset);

    static CARAPI SetCallbackDecode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ ICharsetDecoder* decoder);

    static CARAPI SetCallbackEncode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ ICharsetEncoder* encoder);

private:
    // Translates from Java's enum to the magic numbers #defined in "NativeConverter.cpp".
    static CARAPI TranslateCodingErrorAction(
        /* [in] */ ICodingErrorAction* action,
        /* [out] */ Int32* result);

    static CARAPI SetCallbackDecode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ Int32 onMalformedInput,
        /* [in] */ Int32 onUnmappableInput,
        /* [in] */ const String& subChars);

    static CARAPI SetCallbackEncode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ Int32 onMalformedInput,
        /* [in] */ Int32 onUnmappableInput,
        /* [in] */ ArrayOf<Byte>* subBytes);
};

} // namespace IO
} // namespace Elastos

#endif //__NATIVECONVERTER_H__
