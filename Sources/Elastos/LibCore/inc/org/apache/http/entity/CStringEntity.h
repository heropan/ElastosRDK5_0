
#ifndef __ORG_APACHE_HTTP_ENTITY_CSTRINGENTITY_H_
#define __ORG_APACHE_HTTP_ENTITY_CSTRINGENTITY_H_

#include "_Org_Apache_Http_Entity_CStringEntity.h"
#include "AbstractHttpEntity.h"

using Elastos::IO::IFile;
using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;

namespace Org {
namespace Apache {
namespace Http {
namespace Entity {

/**
 *  An entity whose content is retrieved from a string.
 *
 * @author <a href="mailto:oleg at ural.ru">Oleg Kalnichevski</a>
 *
 * @version $Revision: 618367 $
 *
 * @since 4.0
 */
CarClass(CStringEntity)
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
        /* [in] */ const String& s,
        /* [in] */ const String& charset);

    CARAPI constructor(
        /* [in] */ const String& s);

    CARAPI constructor();

protected:
    AutoPtr< ArrayOf<Byte> > mContent;
};

} // namespace Entity
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_ENTITY_CSTRINGENTITY_H_
