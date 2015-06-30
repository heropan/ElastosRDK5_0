
#ifndef __ORG_APACHE_HTTP_ENTITY_CFILEENTITY_H_
#define __ORG_APACHE_HTTP_ENTITY_CFILEENTITY_H_

#include "_Org_Apache_Http_Entity_CFileEntity.h"
#include "AbstractHttpEntity.h"

using Elastos::IO::IFile;
using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;

namespace Org {
namespace Apache {
namespace Http {
namespace Entity {

/**
 *  An entity whose content is retrieved from a byte array.
 *
 * @author <a href="mailto:oleg at ural.ru">Oleg Kalnichevski</a>
 *
 * @version $Revision: 604625 $
 *
 * @since 4.0
 */
CarClass(CFileEntity)
    , public AbstractHttpEntity
    , public ICloneable
{
public:
    CAR_INTERFACE_DECL()

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

    CARAPI Clone(
        /* [out] */ IInterface** o);

    CARAPI constructor(
        /* [in] */ IFile* file,
        /* [in] */ const String& contentType);

    CARAPI constructor();

private:
    AutoPtr<IFile> mFile;
};

} // namespace Entity
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_ENTITY_CFILEENTITY_H_
