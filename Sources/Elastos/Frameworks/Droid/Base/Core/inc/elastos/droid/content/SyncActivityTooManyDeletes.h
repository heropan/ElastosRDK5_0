
#ifndef __ELASTOS_DROID_CONTENT_SYNCACTIVITYTOOMANYDELETES_H__
#define __ELASTOS_DROID_CONTENT_SYNCACTIVITYTOOMANYDELETES_H__

//#include "app/Activity.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

//using Elastos::Droid::App::Activity;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::View::IView;
using Elastos::Droid::Widget::IAdapterViewOnItemClickListener;
using Elastos::Droid::Widget::IAdapterView;
using Elastos::Droid::Accounts::IAccount;

namespace Elastos {
namespace Droid {
namespace Content {

/**
 * Presents multiple options for handling the case where a sync was aborted because there
 * were too many pending deletes. One option is to force the delete, another is to rollback
 * the deletes, the third is to do nothing.
 * @hide
 */
class SyncActivityTooManyDeletes
    : public Object/*Activity*/
    , public IAdapterViewOnItemClickListener
{
public:
    CAR_INTERFACE_DECL()

    SyncActivityTooManyDeletes();

    virtual ~SyncActivityTooManyDeletes();

    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnItemClick(
        /* [in] */ IAdapterView* parent,
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

private:
    CARAPI StartSyncReallyDelete();

    CARAPI StartSyncUndoDeletes();

private:
    Int64 mNumDeletes;
    AutoPtr<IAccount> mAccount;
    String mAuthority;
    String mProvider;

};

}
}
}

#endif // __ELASTOS_DROID_CONTENT_SYNCACTIVITYTOOMANYDELETES_H__
