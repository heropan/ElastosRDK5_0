
#ifndef __ORG_APACHE_HTTP_IMPL_AUTH_BASICSCHEME_H__
#define __ORG_APACHE_HTTP_IMPL_AUTH_BASICSCHEME_H__

#include "Elastos.CoreLibrary.Security.h"
#include "Elastos.CoreLibrary.Utility.h"
#include "org/apache/http/impl/auth/RFC2617Scheme.h"

using Elastos::Utility::IMap;
using Org::Apache::Http::IHeader;
using Org::Apache::Http::IHttpRequest;
using Org::Apache::Http::Auth::ICredentials;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Auth {

/**
 * <p>
 * Basic authentication scheme as defined in RFC 2617.
 * </p>
 *
 * @author <a href="mailto:remm@apache.org">Remy Maucherat</a>
 * @author Rodney Waldhoff
 * @author <a href="mailto:jsdever@apache.org">Jeff Dever</a>
 * @author Ortwin Glueck
 * @author Sean C. Sullivan
 * @author <a href="mailto:adrian@ephox.com">Adrian Sutton</a>
 * @author <a href="mailto:mbowler@GargoyleSoftware.com">Mike Bowler</a>
 * @author <a href="mailto:oleg at ural.ru">Oleg Kalnichevski</a>
 *
 * @since 4.0
 */
class BasicScheme : public RFC2617Scheme
{
public:
    BasicScheme();

    /**
     * Returns textual designation of the basic authentication scheme.
     *
     * @return <code>basic</code>
     */
    CARAPI GetSchemeName(
        /* [out] */ String* name);

    /**
     * Processes the Basic challenge.
     *
     * @param header the challenge header
     *
     * @throws MalformedChallengeException is thrown if the authentication challenge
     * is malformed
     */
    CARAPI ProcessChallenge(
        /* [in] */ IHeader* header);

    /**
     * Tests if the Basic authentication process has been completed.
     *
     * @return <tt>true</tt> if Basic authorization has been processed,
     *   <tt>false</tt> otherwise.
     */
    CARAPI IsComplete(
        /* [out] */ Boolean* isComplete);

    /**
     * Returns <tt>false</tt>. Basic authentication scheme is request based.
     *
     * @return <tt>false</tt>.
     */
    CARAPI IsConnectionBased(
        /* [out] */ Boolean* connectionBased);

    /**
     * Produces basic authorization header for the given set of {@link Credentials}.
     *
     * @param credentials The set of credentials to be used for athentication
     * @param request The request being authenticated
     * @throws org.apache.http.auth.InvalidCredentialsException if authentication credentials
     *         are not valid or not applicable for this authentication scheme
     * @throws AuthenticationException if authorization string cannot
     *   be generated due to an authentication failure
     *
     * @return a basic authorization string
     */
    CARAPI Authenticate(
        /* [in] */ ICredentials* credentials,
        /* [in] */ IHttpRequest* request,
        /* [out] */ IHeader** header);

    /**
     * Returns a basic <tt>Authorization</tt> header value for the given
     * {@link Credentials} and charset.
     *
     * @param credentials The credentials to encode.
     * @param charset The charset to use for encoding the credentials
     *
     * @return a basic authorization header
     */
    static CARAPI Authenticate(
        /* [in] */ ICredentials* credentials,
        /* [in] */ const String& charset,
        /* [in] */ Boolean proxy,
        /* [out] */ IHeader** header);

private:
    /** Whether the basic authentication process is complete */
    Boolean mComplete;
};

} // namespace Auth
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_IMPL_AUTH_BASICSCHEME_H__
