
#ifndef __ALERTACTIVITY_H__
#define __ALERTACTIVITY_H__

#include "app/CAlertControllerAlertParams.h"
#include "app/CAlertController.h"
#include "app/Activity.h"

using Elastos::Droid::App::Activity;
using Elastos::Droid::Content::IDialogInterface;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

class AlertActivity
    : public Activity
    , public IDialogInterface
{
public:
    AlertActivity()
        : mAlert(NULL)
        , mAlertParams(NULL)
    {}

    virtual ~AlertActivity()
    {}

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI Cancel();

    CARAPI Dismiss();

    /**
     * Sets up the alert, including applying the parameters to the alert model,
     * and installing the alert's content.
     * 
     * @see #mAlert
     * @see #mAlertParams
     */
    CARAPI_(void) SetupAlert();

    CARAPI OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* keyDown);

    CARAPI OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* keyUp);

protected:
    /**
     * The model for the alert.
     * 
     * @see #mAlertParams
     */
    AutoPtr<CAlertController> mAlert;
    
    /**
     * The parameters for the alert.
     */
    AutoPtr<CAlertControllerAlertParams> mAlertParams;
};

} // namespace App
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__ALERTACTIVITY_H__
