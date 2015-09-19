#ifndef __ELASTOS_DROID_TEXT_UTILITY_Rfc822Token_H__
#define __ELASTOS_DROID_TEXT_UTILITY_Rfc822Token_H__

//#include "ext/frameworkext.h"
#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Utility {

/**
 * This class stores an RFC 822-like name, address, and comment,
 * and provides methods to convert them to quoted strings.
 */
class Rfc822Token
{
public:
    /**
     * Creates a new Rfc822Token with the specified name, address,
     * and comment.
     */
    Rfc822Token(
        /* [in] */ const String& name,
        /* [in] */ const String& address,
        /* [in] */ const String& comment);

    /**
     * Returns the name part.
     */
    CARAPI_(String) GetName();

    /**
     * Returns the address part.
     */
    CARAPI_(String) GetAddress();

    /**
     * Returns the comment part.
     */
    CARAPI_(String) GetComment();

    /**
     * Changes the name to the specified name.
     */
    CARAPI SetName(
        /* [in] */ const String& name);

    /**
     * Changes the address to the specified address.
     */
    CARAPI SetAddress(
        /* [in] */ const String& address);

    /**
     * Changes the comment to the specified comment.
     */
    CARAPI SetComment(
        /* [in] */ const String& comment);

    /**
     * Returns the name (with quoting added if necessary),
     * the comment (in parentheses), and the address (in angle brackets).
     * This should be suitable for inclusion in an RFC 822 address list.
     */
    CARAPI_(String) ToString();

    /**
     * Returns the name, conservatively quoting it if there are any
     * characters that are likely to cause trouble outside of a
     * quoted string, or returning it literally if it seems safe.
     */
    static CARAPI_(String) QuoteNameIfNecessary(
        /* [in] */ const String& name);

    /**
     * Returns the name, with internal backslashes and quotation marks
     * preceded by backslashes.  The outer quote marks themselves are not
     * added by this method.
     */
    static CARAPI_(String) QuoteName(
        /* [in] */ const String& name);

    /**
     * Returns the comment, with internal backslashes and parentheses
     * preceded by backslashes.  The outer parentheses themselves are
     * not added by this method.
     */
    static CARAPI_(String) QuoteComment(
        /* [in] */ const String& comment);

    CARAPI_(Int32) GetHashCode();

    CARAPI_(Boolean) Equals(
        /* [in] */ IInterface* o);

protected:
    Rfc822Token();

    CARAPI_(void) Init(
        /* [in] */ const String& name,
        /* [in] */ const String& address,
        /* [in] */ const String& comment);

private:
    static CARAPI_(Boolean) StringEquals(
        /* [in] */ const String& a,
        /* [in] */ const String& b);

protected:
    String mName, mAddress, mComment;
};

} // namespace Utility
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_UTILITY_Rfc822Token_H__
