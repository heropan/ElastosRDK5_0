
#include "ContentHandler.h"

namespace Elastos {
namespace Net {

// 2e95c642-2c96-4ec8-939c-b2e82baa697a
extern "C" const InterfaceID EIID_ContentHandler =
    { 0x2e95c642, 0x2c96, 0x4ec8, { 0x93, 0x9c, 0xb2, 0xe8, 0x2b, 0xaa, 0x69, 0x7a } };

ECode ContentHandler::GetClassID(
    /* [out] */ ClassID* clsid)
{
    VALIDATE_NOT_NULL(clsid);

    *clsid = EIID_ContentHandler;
    return NOERROR;
}

ECode ContentHandler::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    result->AppendFormat("\nClass[%s]\n", String("ContentHandler"));
    return NOERROR;
}

ECode ContentHandler::GetContent(
    /* [in] */ IURLConnection* uConn,
    /* [in] */ const ArrayOf<InterfaceID>& types,
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj);

    AutoPtr<IInterface> content;
    FAIL_RETURN(GetContent(uConn, (IInterface**)&content));
    for (Int32 i = 0; i < types.GetLength(); i++) {
        if (content->Probe(types[i]) != NULL) {
            *obj = content;
            REFCOUNT_ADD(*obj);
            return NOERROR;
        }
    }
    *obj = NULL;
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
