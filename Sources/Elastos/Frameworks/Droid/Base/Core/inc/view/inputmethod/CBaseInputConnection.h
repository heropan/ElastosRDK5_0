
#ifndef  __CBASEINPUTCONNECTION_H__
#define  __CBASEINPUTCONNECTION_H__

#include "_CBaseInputConnection.h"
#include "view/inputmethod/BaseInputConnection.h"
#include "view/inputmethod/BaseInputConnectionMacro.h"

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

#endif  //__CBASEINPUTCONNECTION_H__
