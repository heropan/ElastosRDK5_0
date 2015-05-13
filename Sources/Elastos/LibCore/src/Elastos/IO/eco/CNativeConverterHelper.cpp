
#include "CNativeConverterHelper.h"
#include "NativeConverter.h"

namespace Elastos {
namespace IO {

ECode CNativeConverterHelper::constructor()
{
    return NOERROR;
}

ECode CNativeConverterHelper::Decode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ const ArrayOf<Byte>& input,
    /* [in] */ Int32 inEnd,
    /* [in] */ const ArrayOf<Char32>& output,
    /* [in] */ Int32 outEnd,
    /* [in] */ const ArrayOf<Int32>& data,
    /* [in] */ Boolean flush,
    /* [out] */ Int32* result)
{
    return NativeConverter::Decode(converterHandle,const_cast<ArrayOf<Byte> *>(&input),
                                    inEnd,const_cast<ArrayOf<Char32> *>(&output),
                                    outEnd,const_cast<ArrayOf<Int32> *>(&data),flush, result);
}

ECode CNativeConverterHelper::Encode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ const ArrayOf<Char32>& input,
    /* [in] */ Int32 inEnd,
    /* [in] */ const ArrayOf<Byte>& output,
    /* [in] */ Int32 outEnd,
    /* [in] */ const ArrayOf<Int32>& data,
    /* [in] */ Boolean flush,
    /* [out] */ Int32* result)
{
    return NativeConverter::Encode(converterHandle,const_cast<ArrayOf<Char32> *>(&input),
                                    inEnd,const_cast<ArrayOf<Byte> *>(&output),
                                    outEnd,const_cast<ArrayOf<Int32> *>(&data),flush, result);
}

ECode CNativeConverterHelper::OpenConverter(
    /* [in] */ const String& encoding,
    /* [out] */ Int64 * value)
{
    return NativeConverter::OpenConverter(encoding,value);
}

ECode CNativeConverterHelper::CloseConverter(
    /* [in] */ Int64 converterHandle)
{
    NativeConverter::CloseConverter(converterHandle);
    return NOERROR;
}

ECode CNativeConverterHelper::ResetByteToChar(
    /* [in] */ Int64 converterHandle)
{
    NativeConverter::ResetByteToChar(converterHandle);
    return NOERROR;
}

ECode CNativeConverterHelper::ResetCharToByte(
    /* [in] */ Int64 converterHandle)
{
    NativeConverter::ResetCharToByte(converterHandle);
    return NOERROR;
}

ECode CNativeConverterHelper::GetSubstitutionBytes(
    /* [in] */ Int64 converterHandle,
    /* [out,callee] */ ArrayOf<Byte> ** outarray)
{
    VALIDATE_NOT_NULL(outarray)

    AutoPtr<ArrayOf<Byte> > ans = NativeConverter::GetSubstitutionBytes(converterHandle);
    *outarray = ans;
    INTERFACE_ADDREF(*outarray);
    return NOERROR;
}

ECode CNativeConverterHelper::GetMaxBytesPerChar(
    /* [in] */ Int64 converterHandle,
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value)

    *value = NativeConverter::GetMaxBytesPerChar(converterHandle);
    return NOERROR;
}

ECode CNativeConverterHelper::GetMinBytesPerChar(
    /* [in] */ Int64 converterHandle,
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value)

    *value = NativeConverter::GetMinBytesPerChar(converterHandle);
    return NOERROR;
}

ECode CNativeConverterHelper::GetAveBytesPerChar(
    /* [in] */ Int64 converterHandle,
    /* [out] */ Float * value)
{
    VALIDATE_NOT_NULL(value)

    *value = NativeConverter::GetAveBytesPerChar(converterHandle);
    return NOERROR;
}

ECode CNativeConverterHelper::GetAveCharsPerByte(
    /* [in] */ Int64 converterHandle,
    /* [out] */ Float * value)
{
    VALIDATE_NOT_NULL(value)

    *value = NativeConverter::GetAveCharsPerByte(converterHandle);
    return NOERROR;
}

ECode CNativeConverterHelper::Contains(
    /* [in] */ const String& converterName1,
    /* [in] */ const String& converterName2,
    /* [out] */ Boolean * value)
{
    VALIDATE_NOT_NULL(value)

    *value = NativeConverter::Contains(converterName1,converterName2);
    return NOERROR;
}

ECode CNativeConverterHelper::CanEncode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ Int32 codeUnit,
    /* [out] */ Boolean * value)
{
    VALIDATE_NOT_NULL(value)

    *value = NativeConverter::CanEncode(converterHandle,codeUnit);
    return NOERROR;
}

ECode CNativeConverterHelper::GetAvailableCharsetNames(
    /* [out,callee] */ ArrayOf<String> ** outarray)
{
    VALIDATE_NOT_NULL(outarray)

    AutoPtr<ArrayOf<String> > ans = NativeConverter::GetAvailableCharsetNames();
    *outarray = ans;
    INTERFACE_ADDREF(*outarray);
    return NOERROR;
}

ECode CNativeConverterHelper::CharsetForName(
    /* [in] */ const String& charsetName,
    /* [out] */ ICharset ** outset)
{
    VALIDATE_NOT_NULL(outset);
    NativeConverter::CharsetForName(charsetName, outset);
    return NOERROR;
}

ECode CNativeConverterHelper::TranslateCodingErrorAction(
    /* [in] */ ICodingErrorAction * errorAction,
    /* [out] */ Int32 * value)
{
    return NOERROR;
}

ECode CNativeConverterHelper::SetCallbackDecode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ ICharsetDecoder * decoder)
{
    return NOERROR;
}

ECode CNativeConverterHelper::SetCallbackEncode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ ICharsetEncoder * encoder)
{
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
