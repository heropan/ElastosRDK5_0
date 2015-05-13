
#include "cmdef.h"
#include "ContentHandler.h"

namespace Elastos {
namespace Net {

ECode ContentHandler::GetContentEx(
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
            INTERFACE_ADDREF(*obj);
            return NOERROR;
        }
    }
    *obj = NULL;
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
