
#ifndef __ASN1SETOF_H__
#define __ASN1SETOF_H__

#include <ASN1ValueCollection.h>

using Elastos::Utility::ICollection;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

class ASN1SetOf : public ASN1ValueCollection
{
public:
    virtual CARAPI Decode(
        /* [in] */ IBerInputStream* bis,
        /* [out] */ IInterface** ret);

    virtual CARAPI EncodeContent(
        /* [in] */ IBerOutputStream* bos);

    virtual CARAPI SetEncodingContent(
        /* [in] */ IBerOutputStream* bos);

    CARAPI Init(
        /* [in] */ IASN1Type* type);
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __ASN1SETOF_H__
