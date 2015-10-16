
#include "elastos/droid/view/inputmethod/CBaseInputConnection.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace InputMethod {

BASEINPUTCONNECTION_METHODS_IMPL(CBaseInputConnection, BaseInputConnection, BaseInputConnection);

PInterface CBaseInputConnection::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_BaseInputConnection) {
        return reinterpret_cast<PInterface>((BaseInputConnection*)this);
    }

    return _CBaseInputConnection::Probe(riid);
}


ECode CBaseInputConnection::constructor(
    /* [in] */ IInputMethodManager* mgr,
    /* [in] */ Boolean fullEditor)
{
    return BaseInputConnection::Init(mgr, fullEditor);
}

ECode CBaseInputConnection::constructor(
    /* [in] */ IView* targetView,
    /* [in] */ Boolean fullEditor)
{
    return BaseInputConnection::Init(targetView, fullEditor);
}

} // namespace InputMethod
} // namespace View
} // namespace Droid
} // namespace Elastos
