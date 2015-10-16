
#ifndef __ELASTOS_DROID_VIEW_INPUTMETHOD_CBASEINPUTCONNECTION_H__
#define  __ELASTOS_DROID_VIEW_INPUTMETHOD_CBASEINPUTCONNECTION_H__

#include "_Elastos_Droid_View_InputMethod_CBaseInputConnection.h"
#include "elastos/droid/view/inputmethod/BaseInputConnection.h"


namespace Elastos {
namespace Droid {
namespace View {
namespace InputMethod {

/**
 * Base class for implementors of the InputConnection interface, taking care
 * of most of the common behavior for providing a connection to an Editable.
 * Implementors of this class will want to be sure to implement
 * {@link #getEditable} to provide access to their own editable object.
 */
CarClass(CBaseInputConnection), public BaseInputConnection
{
public:
    BASEINPUTCONNECTION_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IInputMethodManager* mgr,
        /* [in] */ Boolean fullEditor);

    CARAPI constructor(
        /* [in] */ IView* targetView,
        /* [in] */ Boolean fullEditor);
};

} // namespace InputMethod
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_VIEW_INPUTMETHOD_CBASEINPUTCONNECTION_H__
