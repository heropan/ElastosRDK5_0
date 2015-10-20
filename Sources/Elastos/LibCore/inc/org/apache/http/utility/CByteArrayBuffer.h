
#ifndef __ORG_APACHE_HTTP_UTILITY_CBYTEARRAYBUFFER_H_
#define __ORG_APACHE_HTTP_UTILITY_CBYTEARRAYBUFFER_H_

#include "_Org_Apache_Http_Utility_CByteArrayBuffer.h"
#include <elastos/core/Object.h>

namespace Org {
namespace Apache {
namespace Http {
namespace Utility {

/**
 * A resizable byte array.
 *
 * @author <a href="mailto:oleg@ural.ru">Oleg Kalnichevski</a>
 *
 * @version $Revision: 496070 $
 *
 * @since 4.0
 */
CarClass(CByteArrayBuffer)
    , public Object
    , public IByteArrayBuffer
{
public:
    CByteArrayBuffer();

    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI Append(
        /* [in] */ ArrayOf<Byte>* b,
        /* [in] */ Int32 off,
        /* [in] */ Int32 len);

    CARAPI Append(
        /* [in] */ Int32 b);

    CARAPI Append(
        /* [in] */ ArrayOf<Char32>* b,
        /* [in] */ Int32 off,
        /* [in] */ Int32 len);

    CARAPI Append(
        /* [in] */ ICharArrayBuffer* b,
        /* [in] */ Int32 off,
        /* [in] */ Int32 len);

    CARAPI Clear();

    CARAPI ToByteArray(
        /* [out, callee] */ ArrayOf<Byte>** ByteArray);

    CARAPI ByteAt(
        /* [in] */ Int32 i,
        /* [out] */ Int32* byteAt);

    CARAPI GetCapacity(
        /* [out] */ Int32* capacity);

    CARAPI GetLength(
        /* [out] */ Int32* length);

    CARAPI GetBuffer(
        /* [out, callee] */ ArrayOf<Byte>** buffer);

    CARAPI SetLength(
        /* [in] */ Int32 len);

    CARAPI IsEmpty(
        /* [out] */ Boolean* isEmpty);

    CARAPI IsFull(
        /* [out] */ Boolean* isFull);

    CARAPI constructor(
        /* [in] */ Int32 capacity);

private:
    CARAPI_(void) Expand(
        /* [in] */ Int32 newlen);

private:
    AutoPtr< ArrayOf<Byte> > mBuffer;
    Int32 mLen;

};

} // namespace Utility
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_UTILITY_CBYTEARRAYBUFFER_H_
