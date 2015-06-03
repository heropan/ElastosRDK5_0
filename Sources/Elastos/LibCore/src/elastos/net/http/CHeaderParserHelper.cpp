
#include <cmdef.h>
#include <CHeaderParserHelper.h>
#include <elastos/core/StringUtils.h>
#include <elastos/Math.h>
#include <CObjectContainer.h>
#include <CChallenge.h>

using Elastos::Core::StringUtils;
using Elastos::Core::CObjectContainer;

namespace Elastos {
namespace Net {
namespace Http {

ECode CHeaderParserHelper::ParseCacheControl(
    /* [in] */ const String& value,
    /* [in] */ ICacheControlHandler* handler)
{
    Int32 pos = 0;
    while (pos < value.GetLength()) {
        Int32 tokenStart = pos;
        pos = SkipUntil(value, pos, String("=,"));
        String directive = value.Substring(tokenStart, pos).Trim();

        if (pos == value.GetLength() || value.GetChar(pos) == ',') {
            pos++; // consume ',' (if necessary)
            handler->Handle(directive, String(NULL));
            continue;
        }

        pos++; // consume '='
        pos = SkipWhitespace(value, pos);

        String parameter;

        // quoted string
        if (pos < value.GetLength() && value.GetChar(pos) == '\"') {
            pos++; // consume '"' open quote
            Int32 parameterStart = pos;
            pos = SkipUntil(value, pos, String("\""));
            parameter = value.Substring(parameterStart, pos);
            pos++; // consume '"' close quote (if necessary)

        // unquoted string
        } else {
            Int32 parameterStart = pos;
            pos = SkipUntil(value, pos, String(","));
            parameter = value.Substring(parameterStart, pos).Trim();
        }

        handler->Handle(directive, parameter);
    }

    return NOERROR;
}

ECode CHeaderParserHelper::ParseChallenges(
    /* [in] */ IRawHeaders* responseHeaders,
    /* [in] */ const String& challengeHeader,
    /* [out] */ IObjectContainer** obj)
{
    VALIDATE_NOT_NULL(obj)

    /*
     * auth-scheme = token
     * auth-param  = token "=" ( token | quoted-string )
     * challenge   = auth-scheme 1*SP 1#auth-param
     * realm       = "realm" "=" realm-value
     * realm-value = quoted-string
     */
    // List<Challenge> result = new ArrayList<Challenge>();
    AutoPtr<IObjectContainer> result;
    FAIL_RETURN(CObjectContainer::New((IObjectContainer**)&result));
    Int32 reslen = 0;
    responseHeaders->Length(&reslen);
    for (Int32 h = 0; h < reslen; h++) {
        String resname;
        responseHeaders->GetFieldName(h, &resname);
        if (!challengeHeader.EqualsIgnoreCase(resname)) {
            continue;
        }
        String value;
        responseHeaders->GetValue(h, &value);
        Int32 pos = 0;
        while (pos < value.GetLength()) {
            Int32 tokenStart = pos;
            pos = SkipUntil(value, pos, String(" "));

            String scheme = value.Substring(tokenStart, pos).Trim();
            pos = SkipWhitespace(value, pos);

            // TODO: This currently only handles schemes with a 'realm' parameter;
            //       It needs to be fixed to handle any scheme and any parameters
            //       http://code.google.com/p/android/issues/detail?id=11140

            if (!value.RegionMatches(pos, String("realm=\""), 0, String("realm=\"").GetLength())) {
                break; // unexpected challenge parameter; give up
            }

            pos += String("realm=\"").GetLength();
            Int32 realmStart = pos;
            pos = SkipUntil(value, pos, String("\""));
            String realm = value.Substring(realmStart, pos);
            pos++; // consume '"' close quote
            pos = SkipUntil(value, pos, String(","));
            pos++; // consume ',' comma
            pos = SkipWhitespace(value, pos);
            AutoPtr<IChallenge> challiter;
            FAIL_RETURN(CChallenge::New(scheme, realm, (IChallenge**)&challiter));
            result->Add(challiter);
            // result.add(new Challenge(scheme, realm));
        }
    }

    *obj = result;
    REFCOUNT_ADD(*obj)

    return NOERROR;
}

ECode CHeaderParserHelper::ParseSeconds(
    /* [in] */ const String& value,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)

    // try {
    Int64 seconds = StringUtils::ParseInt64(value);
    if (seconds > Elastos::Core::Math::INT64_MAX_VALUE) {
        *number = Elastos::Core::Math::INT64_MAX_VALUE;
    } else if (seconds < 0) {
        *number = 0;
    } else {
        *number = (Int32)seconds;
    }
    // } catch (NumberFormatException e) {
    //     return -1;
    // }
    return NOERROR;
}

Int32 CHeaderParserHelper::SkipUntil(
    /* [in] */ const String& input,
    /* [in] */ Int32 pos,
    /* [in] */ const String& characters)
{
    AutoPtr<ArrayOf<Char32> > char32Array = input.GetChars();
    for (; pos < char32Array->GetLength(); pos++) {
        if (characters.IndexOf((*char32Array)[pos]) != -1) {
            break;
        }
    }
    return pos;
}

Int32 CHeaderParserHelper::SkipWhitespace(
    /* [in] */ const String& input,
    /* [in] */ Int32 pos)
{
    AutoPtr<ArrayOf<Char32> > char32Array = input.GetChars();
    for (; pos < char32Array->GetLength(); pos++) {
        if ((*char32Array)[pos] != ' ' && (*char32Array)[pos] != '\t') {
            break;
        }
    }
    return pos;
}

} // namespace Http
} // namespace Net
} // namespace Elastos
