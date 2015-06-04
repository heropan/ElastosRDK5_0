#ifndef __LIBCORE_NET_URICODEC_H__
#define __LIBCORE_NET_URICODEC_H__

#include <elatypes.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::IO::Charset::ICharset;

namespace Libcore {
namespace Net {

extern "C"  const InterfaceID EIID_UriCodec;

class UriCodec : public Object
{
public :
    CARAPI GetClassID(
        /* [out] */ ClassID* clsid);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI Validate(
        /* [in] */ const String& uri,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ const String& name,
        /* [out] */ String* result) const;

    static CARAPI ValidateSimple(
        /* [in] */ const String& s,
        /* [in] */ const String& legal);

    CARAPI Encode(
        /* [in] */ const String& s,
        /* [in] */ ICharset* charset,
        /* [out] */ String* result);

    CARAPI AppendEncoded(
        /* [in] */ StringBuilder& builder,
        /* [in] */ const String& s) const;

    CARAPI AppendPartiallyEncoded(
        /* [in] */ StringBuilder& builder,
        /* [in] */ const String& s);

    static CARAPI Decode(
        /* [in] */ const String& s,
        /* [out] */ String* result);

    static CARAPI Decode(
        /* [in] */ const String& s,
        /* [in] */ Boolean convertPlus,
        /* [in] */ ICharset* charset,
        /* [in] */ Boolean throwOnFailure,
        /* [out] */ String* result);

    static CARAPI_(AutoPtr<ICharset>) GetDefaultCharset();

private:
    CARAPI AppendEncoded(
        /* [in] */ StringBuilder& builder,
        /* [in] */ const String& s,
        /* [in] */ ICharset* charset,
        /* [in] */ Boolean isPartiallyEncoded) const;

    static CARAPI_(AutoPtr<ArrayOf<Byte> >) GetBytes(
        /* [in] */ const char* cPtr,
        /* [in] */ ICharset* charSet);

    static CARAPI_(Int32) HexToInt(
        /* [in] */ char c);

    static CARAPI AppendHex(
        /* [in] */ StringBuilder& builder,
        /* [in] */ Byte b);

    static CARAPI AppendHex(
        /* [in] */ StringBuilder& builder,
        /* [in] */ const String& s,
        /* [in] */ ICharset* charset);

protected:
    virtual Boolean IsRetained (
        /* [in] */ char c) const = 0;
};

} // namespace Net
} // namespace Libcore

#endif // __LIBCORE_NET_URICODEC_H__
