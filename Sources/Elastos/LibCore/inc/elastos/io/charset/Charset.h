#ifndef __ELASTOS_IO_CHARSET_ELASTOS_IO_CHAARSET_CHAARSET_H__
#define __ELASTOS_IO_CHARSET_ELASTOS_IO_CHAARSET_CHAARSET_H__

#include "Elastos.CoreLibrary.IO.h"
#include "Object.h"
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/HashSet.h>
#include <elastos/utility/etl/Set.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Etl::HashSet;
using Elastos::Utility::Etl::Set;
using Elastos::Utility::ILocale;
using Elastos::Utility::ISet;
using Elastos::Utility::ISortedMap;
using Elastos::Utility::IHashSet;
using Elastos::Utility::IHashMap;

namespace Elastos {
namespace IO {
namespace Charset {

class Charset
    : public Object
    , public ICharset
{
public:
    Charset();

    virtual ~Charset();

    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ const String& canonicalName,
        /* [in] */ ArrayOf<String>* aliases);

    static CARAPI AvailableCharsets(
        /* [out] */ ISortedMap** charsets);

    static CARAPI ForName(
        /* [in] */ const String& charsetName,
        /* [out] */ ICharset** charset);

    static CARAPI ForNameUEE(
        /* [in] */ const String& charsetName,
        /* [out] */ ICharset** charset);

    static CARAPI IsSupported(
        /* [in] */ const String& charsetName,
        /* [out] */ Boolean* result);

    virtual CARAPI Contains(
        /* [in] */ ICharset* charset,
        /* [out] */ Boolean* result) = 0;

    virtual CARAPI NewEncoder(
        /* [out] */ ICharsetEncoder** charsetEncoder) = 0;

    virtual CARAPI NewDecoder(
        /* [out] */ ICharsetDecoder** charsetDecoder) = 0;

    CARAPI Name(
        /* [out] */ String* name);

    CARAPI Aliases(
        /* [out] */ ISet** aliases);

    CARAPI DisplayName(
        /* [out] */ String* name);

    CARAPI DisplayName(
        /* [in] */ ILocale* l,
        /* [out] */ String* name);

    CARAPI IsRegistered(
        /* [out] */ Boolean* result);

    CARAPI CanEncode(
        /* [out] */ Boolean* result);

    CARAPI Encode(
        /* [in] */ ICharBuffer* buffer,
        /* [out] */ IByteBuffer** byteBuffer);

    CARAPI Encode(
        /* [in] */ const String& s,
        /* [out] */ IByteBuffer** byteBuffer);

    CARAPI Decode(
        /* [in] */ IByteBuffer* buffer,
        /* [out] */ ICharBuffer** charBuffer);

    CARAPI CompareTo(
        /* [in] */ IInterface* charset,
        /* [out] */ Int32* result);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* result);

    static CARAPI DefaultCharset(
        /* [out] */ ICharset** charset);

private:
    static CARAPI CheckCharsetName(
        /* [in] */ const String& name);

    static CARAPI IsValidCharsetNameStart(
        /* [in] */ const Char32& c,
        /* [out] */ Boolean* result);

    static CARAPI IsValidCharsetNamePart(
        /* [in] */ const Char32& c,
        /* [out] */ Boolean* result);

    static CARAPI CacheCharset(
        /* [in] */ const String& charsetName,
        /* [in] */ ICharset* s,
        /* [out] */ ICharset** charset);

    static AutoPtr<ICharset> GetDefaultCharset();

    static CARAPI_(Boolean) IsInitflag();

public:
    String mCanonicalName;

private:
    static AutoPtr<IHashMap> CACHED_CHARSETS;

    static AutoPtr<ICharset> DEFAULT_CHARSET;

    AutoPtr<IHashSet> mAliasesSet;

    static Boolean mIsflag;
};

} // namespace CharSet
} // namespace IO
} // namespace Elastos

#endif //__ELASTOS_IO_CHARSET_ELASTOS_IO_CHAARSET_CHAARSET_H__
