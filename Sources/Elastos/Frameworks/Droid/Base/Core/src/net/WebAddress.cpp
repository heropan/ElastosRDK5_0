
#include "net/WebAddress.h"

namespace Elastos {
namespace Droid {
namespace Net {

const Int32 WebAddress::MATCH_GROUP_SCHEME = 1;
const Int32 WebAddress::MATCH_GROUP_AUTHORITY = 2;
const Int32 WebAddress::MATCH_GROUP_HOST = 3;
const Int32 WebAddress::MATCH_GROUP_PORT = 4;
const Int32 WebAddress::MATCH_GROUP_PATH = 5;

static AutoPtr<IPattern> InitWebAddressPattern()
{
    // /* scheme    */ "(?:(http|https|file)\\:\\/\\/)?" +
    // /* authority */ "(?:([-A-Za-z0-9$_.+!*'(),;?&=]+(?:\\:[-A-Za-z0-9$_.+!*'(),;?&=]+)?)@)?" +
    // /* host      */ "([" + GOOD_IRI_CHAR + "%_-][" + GOOD_IRI_CHAR + "%_\\.-]*|\\[[0-9a-fA-F:\\.]+\\])?" +
    // /* port      */ "(?:\\:([0-9]*))?" +
    // /* path      */ "(\\/?[^#]*)?" +
    // /* anchor    */ ".*", Pattern.CASE_INSENSITIVE

    StringBuffer buf;
    buf += "(?:(http|https|file)\\:\\/\\/)?";
    buf += "(?:([-A-Za-z0-9$_.+!*'(),;?&=]+(?:\\:[-A-Za-z0-9$_.+!*'(),;?&=]+)?)@)?";
    buf += "([";
    buf += IPatterns::GOOD_IRI_CHAR;
    buf += "%_-][";
    buf += IPatterns::GOOD_IRI_CHAR;
    buf += "%_\\.-]*|\\[[0-9a-fA-F:\\.]+\\])?";
    buf += "(?:\\:([0-9]*))?";
    buf += "(\\/?[^#]*)?";
    buf += ".*";

    AutoPtr<IPatternHelper> patternHelper;
    CPatternHelper::AcquireSingleton((IPatternHelper**)&patternHelper);

    AutoPtr<IPattern> pattern;
    patternHelper->Compile((String)buf, IPattern::CASE_INSENSITIVE, (IPattern**)&pattern);

    return pattern;
}

AutoPtr<IPattern> WebAddress::sAddressPattern = InitWebAddressPattern();

WebAddress::WebAddress(
    /* [in] */ const String& address)
{
    if (address.IsNull()) {
        //throw new NullPointerException();
        assert(0);
    }

    mScheme = "";
    mHost = "";
    mPort = -1;
    mPath = "/";
    mAuthInfo = "";

    AutoPtr<ICharSequence> charSeq;
    CStringWrapper::New(address, (ICharSequence**)&charSeq);

    AutoPtr<IMatcher> matcher;
    sAddressPattern->Matcher(charSeq->Get(), (IMatcher**)&matcher);

    Boolean isMatched;
    matcher->Matches(&isMatched);

    String t;
    if (isMatched) {
        matcher->Group(MATCH_GROUP_SCHEME, &t);
        if (!t.IsNull()) {
            mScheme = t.ToLowerCase();
        }

        matcher->Group(MATCH_GROUP_AUTHORITY, &t);
        if (!t.IsNull()) {
            mAuthInfo = t;
        }

        matcher->Group(MATCH_GROUP_HOST, &t);
        if (!t.IsNull()) {
            mHost = t;
        }

        matcher->Group(MATCH_GROUP_PORT, &t);
        if (!t.IsNull() && t.GetLength() > 0) {
            // The ':' character is not returned by the regex.
            mPort = StringUtils::ParseInt32(t);
        }

        matcher->Group(MATCH_GROUP_PATH, &t);
        if (!t.IsNull() && t.GetLength() > 0) {
            /* handle busted myspace frontpage redirect with
               missing initial "/" */
            if (t.GetChar(0) == '/') {
                mPath = t;
            } else {
                mPath = "/" + t;
            }
        }
    } else {
        // nothing found... outa here
        //throw new ParseException("Bad address");
        assert(0);
    }

    /* Get port from scheme or scheme from port, if necessary and
       possible */
    if (mPort == 443 && mScheme.Equals("")) {
        mScheme = "https";
    } else if (mPort == -1) {
        if (mScheme.Equals("https")) {
            mPort = 443;
        } else {
            mPort = 80; // default
        }
    }

    if (mScheme.Equals("")) {
        mScheme = "http";
    }
}

String WebAddress::ToString()
{
    StringBuffer buf;

    buf += mScheme;
    buf += "://";

    if (mAuthInfo.GetLength() > 0) {
        buf += mAuthInfo;
        buf += "@";
    }

    buf += mHost;

    if ((mPort != 443 && mScheme.Equals("https")) || (mPort != 80 && mScheme.Equals("http"))) {
        buf += ":";
        buf += mPort;
    }

    buf += mPath;

    //return mScheme + "://" + authInfo + mHost + port + mPath;
    return (String)buf;
}

void WebAddress::SetScheme(
    /* [in] */ const String& scheme)
{
    mScheme = scheme;
}

String WebAddress::GetScheme()
{
    return mScheme;
}

void WebAddress::SetHost(
    /* [in] */ const String& host)
{
    mHost = host;
}

String WebAddress::GetHost()
{
    return mHost;
}

void WebAddress::SetPort(
    /* [in] */ Int32 port)
{
    mPort = port;
}

Int32 WebAddress::GetPort()
{
    return mPort;
}

void WebAddress::SetPath(
    /* [in] */ const String& path)
{
    mPath = path;
}

String WebAddress::GetPath()
{
    return mPath;
}

void WebAddress::SetAuthInfo(
    /* [in] */ const String& authInfo)
{
    mAuthInfo = authInfo;
}

String WebAddress::GetAuthInfo()
{
    return mAuthInfo;
}


ECode CVpnServiceHelper::Prepare(
    /* [in] */ IContext* context,
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent);
    AutoPtr<IIntent> temp = CVpnService::PrepareImpl(context);
    *intent = temp;
    return NOERROR;
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
