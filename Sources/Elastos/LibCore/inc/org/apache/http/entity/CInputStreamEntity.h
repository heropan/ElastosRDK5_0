
#ifndef __ORG_APACHE_HTTP_ENTITY_CINPUTSTREAMENTITY_H_
#define __ORG_APACHE_HTTP_ENTITY_CINPUTSTREAMENTITY_H_

#include "_Org_Apache_Http_Entity_CInputStreamEntity.h"
#include "org/apache/http/entity/AbstractHttpEntity.h"

using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;

namespace Org {
namespace Apache {
namespace Http {
namespace Entity {

/**
 * A streamed entity obtaining content from an {@link InputStream InputStream}.
 *
 * @author <a href="mailto:oleg at ural.ru">Oleg Kalnichevski</a>
 *
 * @version $Revision: 617591 $
 *
 * @since 4.0
 */
CarClass(CInputStreamEntity), public AbstractHttpEntity
{
public:
    CInputStreamEntity();

    CAR_OBJECT_DECL()

    CARAPI IsRepeatable(
        /* [out] */ Boolean* isRepeatable);

    CARAPI GetContentLength(
        /* [out] */ Int64* length);

    CARAPI GetContent(
        /* [out] */ IInputStream** inputStream);

    CARAPI WriteTo(
        /* [in] */ IOutputStream* outstream);

    CARAPI IsStreaming(
        /* [out] */ Boolean* isStreaming);

    CARAPI ConsumeContent();

    CARAPI constructor(
        /* [in] */ IInputStream* instream,
        /* [in] */ Int64 length);

private:
    static const Int32 BUFFER_SIZE = 2048;

    AutoPtr<IInputStream> mContent;
    Int64 mLength;
    Boolean mConsumed;
};

} // namespace Entity
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_ENTITY_CINPUTSTREAMENTITY_H_
