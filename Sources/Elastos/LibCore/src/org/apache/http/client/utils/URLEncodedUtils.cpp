
#include "URLEncodedUtils.h"
#include "CBasicNameValuePair.h"
#include "params/HttpClientParams.h"
#include "URLDecoder.h"
#include "StringUtils.h"
#include <StringBuilder.h>
#include <elastos/Logger.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Net::CURI;
using Elastos::Utility::ICollection;
using Elastos::Utility::CCollections;
using Elastos::Utility::ICollections;
using Elastos::Utility::CArrayList;
using Elastos::Utility::IArrayList;
using Elastos::Utility::IIterable;
using Elastos::Utility::IIterator;
using Elastos::Utility::CScanner;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IHeader;
using Org::Apache::Http::INameValuePair;
using Org::Apache::Http::Message::CBasicNameValuePair;

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Utils {

const String URLEncodedUtils::CONTENT_TYPE("application/x-www-form-urlencoded");
const String URLEncodedUtils::PARAMETER_SEPARATOR("&");
const String URLEncodedUtils::NAME_VALUE_SEPARATOR("=");

AutoPtr<IList> URLEncodedUtils::Parse(
    /* [in] */ IURI* uri,
    /* [in] */ const String& encoding)
{
    AutoPtr<ICollections> col;
    CCollections::AcquireSingleton((ICollections**)&col);
    AutoPtr<IList> result;
    col->EmptyList((IList**)&result);
    String query;
    uri->GetRawQuery(&query);
    if (!query.IsNull() && query->GetLength() > 0) {
        AutoPtr<IArrayList> alist;
        CArrayList::New((IArrayList**)&alist);
        result = IList::Probe(alist);
        AutoPtr<IScanner> scanner;
        CScanner::New(query, (IScanner**)&scanner);
        Parse(result, scanner, encoding);
    }
    return result;
}

AutoPtr<IList> URLEncodedUtils::Parse(
    /* [in] */ IHttpEntity* entity)
{
    AutoPtr<ICollections> col;
    CCollections::AcquireSingleton((ICollections**)&col);
    AutoPtr<IList> result;
    col->EmptyList((IList**)&result);
    if (IsEncoded(entity)) {
        String content;
        EntityUtils::ToString(entity, &content);
        AutoPtr<IHeader> encoding;
        entity->GetContentEncoding((IHeader**)&encoding);
        if (!content.IsNull() && content->GetLength() > 0) {
            AutoPtr<IArrayList> alist;
            CArrayList::New((IArrayList**)&alist);
            result = IList::Probe(alist);
            AutoPtr<IScanner> scanner;
            CScanner::New(content, (IScanner**)&scanner);
            String value = String(NULL);
            if (encoding) {
                encoding->GetValue(&value);
            }
            Parse(result, scanner, value);
        }
    }
    return result;
}

Boolean URLEncodedUtils::IsEncoded(
    /* [in] */ IHttpEntity* entity)
{
    AutoPtr<IHeader> contentType;
    entity->GetContentType((IHeader**)&contentType);
    if (contentType != NULL) {
        String value;
        contentType->GetValue(&value);
        return value.EqualsIgnoreCase(CONTENT_TYPE);
    }
    return FALSE;
}

ECode URLEncodedUtils::Parse(
    /* [in] */ IList* parameters,
    /* [in] */ IScanner* scanner,
    /* [in] */ const String& encoding)
{
    scanner->UseDelimiter(PARAMETER_SEPARATOR);
    AutoPtr<ICollection> col = ICollection::Probe(parameters);
    Boolean hasNext;
    while(scanner->HasNext(&hasNext), hasNext) {
        String scannerValue;
        scanner->Next(&scannerValue);
        AutoPtr< ArrayOf<String> > nameValue;
        StringUtils::Split(scannerValue, NAME_VALUE_SEPARATOR, (ArrayOf<String>**)&nameValue);
        if (nameValue->GetLength() == 0 || nameValue->GetLength() > 2) {
            Logger::E("URLEncodedUtils", "bad parameter");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        String name;
        Decode((*nameValue)[0], encoding, &name);
        String value = String(NULL);
        if (nameValue->GetLength() == 2) {
            Decode((*nameValue)[1], encoding, &value);
        }
        AutoPtr<INameValuePair> pair;
        CBasicNameValuePair::New(name, value, (INameValuePair**)&pair);
        col->Add(pair);
    }
}

String URLEncodedUtils::Format(
    /* [in] */ IList* parameters,
    /* [in] */ const String& encoding)
{
    StringBuilder result;
    AutoPtr<IIterable> iterable = IIterable::Probe(parameters);
    AutoPtr<IIterator> it = iterable->GetIterator((IIterator**)&it);
    Boolean hasNext;
    while(it->HasNext(&hasNext), hasNext) {
        AutoPtr<INameValuePair> parameter;
        it->Next((IInterface**)&parameter);
        String name;
        parameter->GetName(&name);
        String encodedName;
        Encode(name, encoding, &encodedName);
        String value;
        parameter->GetValue(&value);
        String encodedValue = String("");
        if (!value.IsNull()) {
            Encode(value, encoding, &encodedValue);
        }
        if (result.GetLength() > 0) result.Append(PARAMETER_SEPARATOR);
        result.Append(encodedName);
        result.Append(NAME_VALUE_SEPARATOR);
        result.Append(encodedValue);
    }
    return result.ToString();
}

ECode URLEncodedUtils::Decode(
    /* [in] */ const String& content,
    /* [in] */ const String& encoding,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)
    // try {
    if (FAILED(URLDecoder::Decode(content,
            !encoding.IsNull() ? encoding : IHTTP::DEFAULT_CONTENT_CHARSET, value))) {
        *value = String(NULL);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // } catch (UnsupportedEncodingException problem) {
    //     throw new IllegalArgumentException(problem);
    // }
    return NOERROR;
}

ECode URLEncodedUtils::Encode(
    /* [in] */ const String& content,
    /* [in] */ const String& encoding,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)
    // try {
    if (FAILED(URLDecoder::Encode(content,
            !encoding.IsNull() ? encoding : IHTTP::DEFAULT_CONTENT_CHARSET, value))) {
        *value = String(NULL);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // } catch (UnsupportedEncodingException problem) {
    //     throw new IllegalArgumentException(problem);
    // }
}

} // namespace Utils
} // namespace Client
} // namespace Http
} // namespace Apache
} // namespace Org