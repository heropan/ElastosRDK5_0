#include "Charset.h"
#include "CharBuffer.h"
#include "CCodingErrorAction.h"
// #include "NativeConverter.h"
#include "CStringWrapper.h"
#include "AutoLock.h"
#include "CSystem.h"

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::CStringWrapper;
// using Elastos::IO::NativeConverter;

namespace Elastos {
namespace IO {
namespace Charset {

AutoPtr< HashMap<String, AutoPtr<ICharset> > > Charset::CACHED_CHARSETS = new HashMap<String, AutoPtr<ICharset> >();

const AutoPtr<ICharset> Charset::DEFAULT_CHARSET;

static Object gCachedCharsetsLock;

Charset::Charset()
    : mAliasesSet(new HashSet<String>())
{}

Charset::~Charset()
{
}

CAR_INTERFACE_IMPL(Charset, Object, ICharset)

ECode Charset::Init(
    /* [in] */ const String& canonicalName,
    /* [in] */ ArrayOf<String>* aliases)
{
    // check whether the given canonical name is legal
    ASSERT_SUCCEEDED(CheckCharsetName(canonicalName));
    mCanonicalName = canonicalName;
    // check each alias and put into a set
    if (mAliasesSet != NULL) {
        Int32 size = aliases->GetLength();
        for (Int32 i = 0; i < size; i++) {
            ASSERT_SUCCEEDED(CheckCharsetName((*aliases)[i]));
            mAliasesSet->Insert((*aliases)[i]);
        }
    }

    return NOERROR;
}

// TODO:
// public static SortedMap<String, Charset> availableCharsets() {
//     // Start with a copy of the built-in charsets...
//     TreeMap<String, Charset> charsets = new TreeMap<String, Charset>(String.CASE_INSENSITIVE_ORDER);
//     for (String charsetName : NativeConverter.getAvailableCharsetNames()) {
//         Charset charset = NativeConverter.charsetForName(charsetName);
//         charsets.put(charset.name(), charset);
//     }

//     // Add all charsets provided by all charset providers...
//     for (CharsetProvider charsetProvider : ServiceLoader.load(CharsetProvider.class, null)) {
//         Iterator<Charset> it = charsetProvider.charsets();
//         while (it.hasNext()) {
//             Charset cs = it.next();
//             // A CharsetProvider can't override a built-in Charset.
//             if (!charsets.containsKey(cs.name())) {
//                 charsets.put(cs.name(), cs);
//             }
//         }
//     }

//     return Collections.unmodifiableSortedMap(charsets);
// }

ECode Charset::ForName(
    /* [in] */ const String& charsetName,
    /* [out] */ ICharset** charset)
{
    VALIDATE_NOT_NULL(charset);
    // Is this charset in our cache?
    AutoPtr<ICharset> cs;
    {
        AutoLock lock(gCachedCharsetsLock);
        HashMap<String, AutoPtr<ICharset> >::Iterator it = CACHED_CHARSETS->Find(charsetName);
        if (it != CACHED_CHARSETS->End()) {
            *charset = it->mSecond;
            REFCOUNT_ADD(*charset)
            return NOERROR;
        }
    }

    // Is this a built-in charset supported by ICU?
    if (charsetName.IsNull()) {
        return E_ILLEGAL_CHARSET_NAME_EXCEPTION;
    }

    FAIL_RETURN(CheckCharsetName(charsetName));
    // NativeConverter::CharsetForName(charsetName, (ICharset**)&cs);

    if (cs.Get() != NULL) {
        AutoPtr<ICharset> result;
        FAIL_RETURN(CacheCharset(charsetName, cs.Get(), charset));
        return NOERROR;
    }

    // Does a configured CharsetProvider have this charset?
    // TODO:
    // for (CharsetProvider charsetProvider : ServiceLoader.load(CharsetProvider.class, null)) {
    //     cs = charsetProvider.charsetForName(charsetName);
    //     if (cs != null) {
    //         return cacheCharset(charsetName, cs);
    //     }
    // }

    return E_UNSUPPORTED_CHARSET_EXCEPTION;
}

ECode Charset::ForNameUEE(
    /* [in] */ const String& charsetName,
    /* [out] */ ICharset** charset)
{
    VALIDATE_NOT_NULL(charset);
    return Charset::ForName(charsetName, charset);
}

ECode Charset::IsSupported(
    /* [in] */ const String& charsetName,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<ICharset> charset;
    ECode res = Charset::ForName(charsetName, (ICharset**)&charset);
    *result = SUCCEEDED(res) ? TRUE : FALSE;
    return res;
}

ECode Charset::Name(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mCanonicalName;
    return NOERROR;
}

ECode Charset::Aliases(
    /* [out] */ ISet** aliases)
{
    assert(0 && "TODO");
    // return Collections.unmodifiableSet(this.aliasesSet);
    return E_NOT_IMPLEMENTED;
}

ECode Charset::DisplayName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mCanonicalName;
    return NOERROR;
}

ECode Charset::DisplayName(
    /* [in] */ ILocale* l,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mCanonicalName;
    return NOERROR;
}

ECode Charset::IsRegistered(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = !mCanonicalName.StartWith("x-") && !mCanonicalName.StartWith("X-");
    return NOERROR;
}

ECode Charset::CanEncode(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

ECode Charset::Encode(
    /* [in] */ ICharBuffer* buffer,
    /* [out] */ IByteBuffer** byteBuffer)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(byteBuffer);

    AutoPtr<ICharsetEncoder> encoder;
    FAIL_RETURN(NewEncoder((ICharsetEncoder **)&encoder));

    AutoPtr<ICodingErrorAction> action;
    FAIL_RETURN(CCodingErrorAction::New((ICodingErrorAction**)&action))
    AutoPtr<ICodingErrorAction> replace;
    action->GetREPLACE((ICodingErrorAction **)&replace);

    AutoPtr<ICharsetEncoder> malformEncoder;
    FAIL_RETURN(encoder->OnMalformedInput(replace,
        (ICharsetEncoder **)&malformEncoder));

    AutoPtr<ICharsetEncoder> unMappableEncoder;
    FAIL_RETURN(malformEncoder->OnUnmappableCharacter(replace,
        (ICharsetEncoder **)&unMappableEncoder));

    FAIL_RETURN(unMappableEncoder->Encode(buffer, byteBuffer));

    return NOERROR;
}

ECode Charset::Encode(
    /* [in] */ const String& s,
    /* [out] */ IByteBuffer** byteBuffer)
{
    VALIDATE_NOT_NULL(byteBuffer);

    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(s, (ICharSequence**)&cs);
    AutoPtr<ICharBuffer> charBuffer;
    CharBuffer::WrapSequence(cs.Get(), (ICharBuffer**)&charBuffer);

    return Encode(charBuffer, byteBuffer);
}

ECode Charset::Decode(
    /* [in] */ IByteBuffer* buffer,
    /* [out] */ ICharBuffer** charBuffer)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(charBuffer);

    AutoPtr<ICharsetDecoder> decoder;
    FAIL_RETURN(NewDecoder((ICharsetDecoder **)&decoder));

    AutoPtr<ICodingErrorAction> action;
    FAIL_RETURN(CCodingErrorAction::New((ICodingErrorAction** )&action))

    AutoPtr<ICodingErrorAction> replace;
    action->GetREPLACE((ICodingErrorAction **)&replace);

    AutoPtr<ICharsetDecoder> malformDecoder;
    FAIL_RETURN(decoder->OnMalformedInput(replace, (ICharsetDecoder **)&malformDecoder));

    AutoPtr<ICharsetDecoder> unMappableDecoder;
    FAIL_RETURN(malformDecoder->OnUnmappableCharacter(replace, (ICharsetDecoder **)&unMappableDecoder));

    FAIL_RETURN(unMappableDecoder->Decode(buffer, charBuffer));

    return NOERROR;
}

ECode Charset::CompareTo(
    /* [in] */ IInterface* charset,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(charset);
    VALIDATE_NOT_NULL(result);
    AutoPtr<ICharset> charsetInterface = ICharset::Probe(charset);
    *result = mCanonicalName.CompareIgnoreCase(((Charset *)charsetInterface.Get())->mCanonicalName);
    return NOERROR;
}

ECode Charset::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* result)
{
    Int32 compare;
    CompareTo(other, compare);
    if (compare == 0)
    {
        *result = TRUE;
    } else {
        *result = FALSE;
    }
    return NOERROR;
}

ECode Charset::DefaultCharset(
    /* [out] */ ICharset** charset)
{
    VALIDATE_NOT_NULL(charset);
    *charset = GetDefaultCharset();
    REFCOUNT_ADD(*charset)
    return NOERROR;
}

ECode Charset::CheckCharsetName(
    /* [in] */ const String& name)
{
    if (name.IsNullOrEmpty()) {
        return E_ILLEGAL_CHARSET_NAME_EXCEPTION;
    }

    Boolean result = FALSE;
    AutoPtr<ArrayOf<Char32> > charArray = name.GetChars();
    Int32 length = charArray->GetLength();
    for (Int32 i = 0; i < length; ++i) {
        IsValidCharsetNameCharacter((*charArray)[i], &result);
        if (!result) {
            return E_ILLEGAL_CHARSET_NAME_EXCEPTION;
        }
    }

    return NOERROR;
}

ECode Charset::IsValidCharsetNameCharacter(
    /* [in] */ const Char32& c,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')
        || c == '-' || c == '.' || c == ':' || c == '_';
    return NOERROR;
}

ECode Charset::CacheCharset(
    /* [in] */ const String& charsetName,
    /* [in] */ ICharset* cs,
    /* [out] */ ICharset** charset)
{
    VALIDATE_NOT_NULL(charset);
    *charset = NULL;
    VALIDATE_NOT_NULL(cs);

    AutoLock lock(gCachedCharsetsLock);

    // Get the canonical name for this charset, and the canonical instance from the table.
    String canonicalName;
    cs->GetName(&canonicalName);
    AutoPtr<ICharset> canonicalCharset;

    HashMap<String, AutoPtr<ICharset> >::Iterator it = CACHED_CHARSETS->Find(canonicalName);
    if (it == CACHED_CHARSETS->End()) {
        canonicalCharset = cs;
    }
    else
    {
        canonicalCharset = it->mSecond;
    }

    // Cache the charset by its canonical name...
    (*CACHED_CHARSETS)[canonicalName] = canonicalCharset;

    // And the name the user used... (Section 1.4 of http://unicode.org/reports/tr22/ means
    // that many non-alias, non-canonical names are valid. For example, "utf8" isn't an
    // alias of the canonical name "UTF-8", but we shouldn't penalize consistent users of
    // such names unduly.)
    (*CACHED_CHARSETS)[charsetName] = canonicalCharset;

    // And all its aliases...
    HashSet<String>::Iterator itr;
    for (itr = ((Charset*)cs)->mAliasesSet->Begin(); itr != ((Charset*)cs)->mAliasesSet->End(); itr++) {
        String alias = *itr;
        (*CACHED_CHARSETS)[alias] = canonicalCharset;
    }

    *charset = canonicalCharset;
    REFCOUNT_ADD(*charset)
    return NOERROR;
}

AutoPtr<ICharset> Charset::GetDefaultCharset()
{
    if (DEFAULT_CHARSET.Get() == NULL) {

    AutoPtr<ISystem> system;
#ifdef ELASTOS_CORELIBRARY
    AutoPtr<Elastos::Core::CSystem> cs;
    Elastos::Core::CSystem::AcquireSingletonByFriend((Elastos::Core::CSystem**)&cs);
    system = (ISystem*)cs.Get();
#else
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
#endif

        String encoding;
        system->GetProperty(String("file.encoding"), &encoding);
        AutoPtr<ICharset> result;
        ECode ec = ForName(encoding, (ICharset**)&result);
        if (result == NULL || ec == E_UNSUPPORTED_CHARSET_EXCEPTION) {
            result = NULL;
            ForName(String("UTF-8"), (ICharset**)&result);
        }
        DEFAULT_CHARSET = result;
    }

    return DEFAULT_CHARSET;
}

} // namespace Charset
} // namespace IO
} // namespace Elastos
