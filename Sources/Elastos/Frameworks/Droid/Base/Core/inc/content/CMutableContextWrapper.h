
#ifndef __ELASTOS_DROID_CONTENT_CMUTABLECONTEXTWRAPPER_H__
#define __ELASTOS_DROID_CONTENT_CMUTABLECONTEXTWRAPPER_H__

#include "_CMutableContextWrapper.h"
#include "ContextWrapper.h"

using namespace Elastos::IO;
using namespace Elastos::Droid::App;
using namespace Elastos::Droid::Content::Res;
using namespace Elastos::Droid::Content::Pm;
using namespace Elastos::Droid::Os;
using namespace Elastos::Droid::View;
using Elastos::Core::ICharSequence;
using Elastos::Core::IClassLoader;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Database::IDatabaseErrorHandler;
using Elastos::Droid::Database::Sqlite::ISQLiteDatabase;
using Elastos::Droid::Database::Sqlite::ISQLiteDatabaseCursorFactory;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Utility::IAttributeSet;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CMutableContextWrapper)
    , public ContextWrapper
{
public:
    ICONTEXT_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetBaseContext(
        /* [out] */ IContext** context);

    /**
     * Change the base context for this ContextWrapper. All calls will then be
     * delegated to the base context.  Unlike ContextWrapper, the base context
     * can be changed even after one is already set.
     *
     * @param base The new base context for this wrapper.
     */
    CARAPI SetBaseContext(
        /* [in] */ IContext* base);

    CARAPI constructor(
        /* [in] */ IContext* base);

private:
    // TODO: Add your private member variables here.
};

}
}
}

#endif // __ELASTOS_DROID_CONTENT_CMUTABLECONTEXTWRAPPER_H__
