
#ifndef __ELASTOS_DROID_INTERNAL_APP_EXTERNALMEDIAFORMATACTIVITY_H__
#define __ELASTOS_DROID_INTERNAL_APP_EXTERNALMEDIAFORMATACTIVITY_H__

#include "content/BroadcastReceiver.h"
#include "app/AlertActivity.h"
#include "_CExternalMediaFormatActivity.h"

using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IBroadcastReceiver;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

class CExternalMediaFormatActivity : public AlertActivity
{
private:
    class StorageReceiver : public BroadcastReceiver
    {
    public:
        StorageReceiver(
            /* [in] */ CExternalMediaFormatActivity* host);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("StorageReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CExternalMediaFormatActivity* mHost;
    };

public:
    CExternalMediaFormatActivity();

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnStart();

    CARAPI OnResume();

    CARAPI OnPause();

    CARAPI OnStop();

    CARAPI OnDestroy();

    CARAPI OnClick(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 which);

private:
    static const Int32 POSITIVE_BUTTON = IAlertDialog::BUTTON_POSITIVE;

    AutoPtr<IBroadcastReceiver> mStorageReceiver;
};

} //namespace App
} //namespace Internal
} //namespace Droid
} //namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_APP_EXTERNALMEDIAFORMATACTIVITY_H__
