#include "text/util/Rfc822Token.h"
#include <elastos/StringBuilder.h>

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Utility {

Rfc822Token::Rfc822Token()
{}

Rfc822Token::Rfc822Token(
    /* [in] */ const String& name,
    /* [in] */ const String& address,
    /* [in] */ const String& comment)
{
    Init(name, address, comment);
}

void Rfc822Token::Init(
    /* [in] */ const String& name,
    /* [in] */ const String& address,
    /* [in] */ const String& comment)
{
    mName = name;
    mAddress = address;
    mComment = comment;
}

String Rfc822Token::GetName()
{
    return mName;
}

String Rfc822Token::GetAddress()
{
    return mAddress;
}

String Rfc822Token::GetComment()
{
    return mComment;
}

ECode Rfc822Token::SetName(
    /* [in] */ const String& name)
{
    mName = name;
    return NOERROR;
}

ECode Rfc822Token::SetAddress(
    /* [in] */ const String& address)
{
    mAddress = address;
    return NOERROR;
}

ECode Rfc822Token::SetComment(
    /* [in] */ const String& comment)
{
    mComment = comment;
    return NOERROR;
}

String Rfc822Token::ToString()
{
    StringBuilder sb;

    if (!mName.IsNullOrEmpty()) {
        sb.AppendString(QuoteNameIfNecessary(mName));
        sb.AppendChar(' ');
    }

    if (!mComment.IsNullOrEmpty()) {
            sb.AppendChar('(');
            sb.AppendString(QuoteComment(mComment));
            sb.AppendCStr(") ");
    }

    if (!mAddress.IsNullOrEmpty()) {
        sb.AppendChar('<');
        sb.AppendString(mAddress);
        sb.AppendChar('>');
    }

    String strRet;
    sb.ToString(&strRet);

    return strRet;
}

String Rfc822Token::QuoteNameIfNecessary(
    /* [in] */ const String& name)
{
    AutoPtr<ArrayOf<Char32> > chars = name.GetChars();
    Int32 len = chars->GetLength();
    Char32 c;
    for (Int32 i = 0; i < len; i++) {
        c = (*chars)[i];
        if (! ((c >= 'A' && c <= 'Z') ||
               (c >= 'a' && c <= 'z') ||
               (c == ' ') ||
               (c >= '0' && c <= '9'))) {
            return String("\"") + QuoteName(name) + String("\"");
        }
    }

    return name;
}

String Rfc822Token::QuoteName(
    /* [in] */ const String& name)
{
    StringBuilder sb;

    AutoPtr<ArrayOf<Char32> > chars = name.GetChars();
    Int32 len = chars->GetLength();
    Char32 c;
    for (Int32 i = 0; i < len; i++) {
        c = (*chars)[i];
        if (c == '\\' || c == '"') {
            sb.AppendChar('\\');
        }

        sb.AppendChar(c);
    }

    String strRet;
    sb.ToString(&strRet);
    return strRet;
}

String Rfc822Token::QuoteComment(
    /* [in] */ const String& comment)
{
    StringBuilder sb;

    AutoPtr<ArrayOf<Char32> > chars = comment.GetChars();
    Int32 len = chars->GetLength();
    Char32 c;
    for (Int32 i = 0; i < len; i++) {
        c = (*chars)[i];
        if (c == '(' || c == ')' || c == '\\') {
            sb.AppendChar('\\');
        }

        sb.AppendChar(c);
    }

    String strRet;
    sb.ToString(&strRet);
    return strRet;
}

Int32 Rfc822Token::GetHashCode()
{
    Int32 result = 17;
    if (mName.IsNullOrEmpty()) {
        result = 31 * result + mName.GetHashCode();
    }
    if (mAddress.IsNullOrEmpty()) {
        result = 31 * result + mAddress.GetHashCode();
    }
    if (mComment.IsNullOrEmpty()) {
        result = 31 * result + mComment.GetHashCode();
    }
    return result;
}

Boolean Rfc822Token::StringEquals(
    /* [in] */ const String& a,
    /* [in] */ const String& b)
{
    if (a.IsNullOrEmpty()) {
        return (b.IsNullOrEmpty());
    } else {
        return (a.Equals(b));
    }
}

Boolean Rfc822Token::Equals(
    /* [in] */ IInterface* o)
{
    IRfc822Token* rfc822Token = IRfc822Token::Probe(o);
    if(rfc822Token == NULL) {
        return FALSE;
    }
    IRfc822Token* other = rfc822Token;
    String name, address, comment;

    return (StringEquals(mName, (other->GetName(&name), name)) &&
            StringEquals(mAddress, (other->GetAddress(&address), address)) &&
            StringEquals(mComment, (other->GetComment(&comment), comment)));
}

} // namespace Utility
} // namespace Text
} // namepsace Droid
} // namespace Elastos
