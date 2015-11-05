
#ifndef __ORG_APACHE_HTTP_UTILITY_ENCODINGUTILS_H_
#define __ORG_APACHE_HTTP_UTILITY_ENCODINGUTILS_H_

#ifdef ELASTOS_CORELIBRARY
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif
#include "elastos/coredef.h"

namespace Org {
namespace Apache {
namespace Http {
namespace Utility {

/**
 * The home for utility methods that handle various encoding tasks.
 *
 * @author Michael Becke
 * @author <a href="mailto:oleg at ural.ru">Oleg Kalnichevski</a>
 *
 * @since 4.0
 */
class ECO_PUBLIC EncodingUtils
{
public:
    /**
     * Converts the byte array of HTTP content characters to a string. If
     * the specified charset is not supported, default system encoding
     * is used.
     *
     * @param data the byte array to be encoded
     * @param offset the index of the first byte to encode
     * @param length the number of bytes to encode
     * @param charset the desired character encoding
     * @return The result of the conversion.
     */
    static CARAPI GetString(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ const String& charset,
        /* [out] */ String* str);


    /**
     * Converts the byte array of HTTP content characters to a string. If
     * the specified charset is not supported, default system encoding
     * is used.
     *
     * @param data the byte array to be encoded
     * @param charset the desired character encoding
     * @return The result of the conversion.
     */
    static CARAPI GetString(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ const String& charset,
        /* [out] */ String* str);

    /**
     * Converts the specified string to a byte array.  If the charset is not supported the
     * default system charset is used.
     *
     * @param data the string to be encoded
     * @param charset the desired character encoding
     * @return The resulting byte array.
     */
    static CARAPI GetBytes(
        /* [in] */ const String& data,
        /* [in] */ const String& charset,
        /* [out] */ ArrayOf<Byte>** bytes);

    /**
     * Converts the specified string to byte array of ASCII characters.
     *
     * @param data the string to be encoded
     * @return The string as a byte array.
     */
    static CARAPI GetAsciiBytes(
        /* [in] */ const String& data,
        /* [out] */ ArrayOf<Byte>** bytes);

    /**
     * Converts the byte array of ASCII characters to a string. This method is
     * to be used when decoding content of HTTP elements (such as response
     * headers)
     *
     * @param data the byte array to be encoded
     * @param offset the index of the first byte to encode
     * @param length the number of bytes to encode
     * @return The string representation of the byte array
     */
    static CARAPI GetAsciiString(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ String* str);

    /**
     * Converts the byte array of ASCII characters to a string. This method is
     * to be used when decoding content of HTTP elements (such as response
     * headers)
     *
     * @param data the byte array to be encoded
     * @return The string representation of the byte array
     */
    static CARAPI GetAsciiString(
        /* [in] */ ArrayOf<Byte>* data,
        /* [out] */ String* str);

private:
    EncodingUtils() {}
};

} // namespace Utility
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_UTILITY_ENCODINGUTILS_H_
