
#include "RFC2617Scheme.h"
#include "BasicHeaderValueParser.h"
#include "CParserCursor.h"
#include <elastos/Logger.h>

using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Utility::IHashMap;
using Elastos::Utility::CHashMap;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IHeaderElement;
using Org::Apache::Http::Message::BasicHeaderValueParser;
using Org::Apache::Http::Message::CParserCursor;
using Org::Apache::Http::Message::IParserCursor;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Auth {

RFC2617Scheme::RFC2617Scheme()
    : AuthSchemeBase()
{}

ECode RFC2617Scheme::ParseChallenge(
    /* [in] */ ICharArrayBuffer* buffer,
    /* [in] */ Int32 pos,
    /* [in] */ Int32 len)
{
    AutoPtr<IHeaderValueParser> parser = BasicHeaderValueParser::DEFAULT;
    Int32 len;
    buffer->GetLength(&len);
    AutoPtr<IParserCursor> cursor;
    CParserCursor::New(pos, len, (IParserCursor**)&cursor);
    AutoPtr< ArrayOf<IHeaderElement*> > elements;
    parser->ParseElements(buffer, cursor, (ArrayOf<IHeaderElement*>**)&elements);
    if (elements->GetLength() == 0) {
        Logger::E("RFC2617Scheme", "Authentication challenge is empty");
        return E_MALFORMED_CHALLENGE_EXCEPTION;
    }

    AutoPtr<IHashMap> hm;
    CHashMap::New(elements->GetLength(), (IHashMap**)&hm);
    mParams = IMap::Probe(hm);
    for (Int32 i = 0; i < elements->GetLength(), ++i) {
        AutoPtr<IHeaderElement> element = (*elements)[i];
        String name, value;
        element->GetName(&name);
        element->GetValue(&value);
        AutoPtr<ICharSequence> nameCs, valueCs;
        CString::New(name, (ICharSequence**)&nameCs);
        CString::New(value, (ICharSequence**)&valueCs);
        mParams->Put(nameCs, valueCs);
    }
    return NOERROR;
}

AutoPtr<IMap> RFC2617Scheme::GetParameters()
{
    if (mParams == NULL) {
        AutoPtr<IHashMap> hm;
        CHashMap::New((IHashMap**)&hm);
        mParams = IMap::Probe(hm);
    }
    return mParams;
}

ECode RFC2617Scheme::GetParameter(
    /* [in] */ const String& name,
    /* [out] */ String* param)
{
    VALIDATE_NOT_NULL(param)
    *param = String(NULL);
    if (name.IsNull()) {
        Logger::E("RFC2617Scheme", "Parameter name may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (mParams == NULL) {
        return NOERROR;
    }
    AutoPtr<ICharSequence> cs;
    CString::New(name.ToLowerCase(ILocale::ENGLISH), (ICharSequence**)&cs);
    AutoPtr<IInterface> value;
    mParams->Get(cs, (IInterface**)&value);
    ICharSequence::Probe(value)->ToString(param);
    return NOERROR;
}

ECode RFC2617Scheme::GetRealm(
    /* [out] */ String* realm)
{
    VALIDATE_NOT_NULL(realm)
    return GetParameter(String("realm"), realm);
}

} // namespace Auth
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org