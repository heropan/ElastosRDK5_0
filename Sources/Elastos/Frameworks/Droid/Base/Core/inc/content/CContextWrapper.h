
#ifndef __ELASTOS_DROID_CONTENT_CCONTEXTWRAPPER_H__
#define __ELASTOS_DROID_CONTENT_CCONTEXTWRAPPER_H__

#include "_Elastos_Droid_Content_CContextWrapper.h"
#include <ext/frameworkext.h>
#include "ContextWrapper.h"

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::IO;
using namespace Elastos::Droid::Database;
using namespace Elastos::Droid::Graphics;
using namespace Elastos::Droid::View;
using namespace Elastos::Droid::Net;
using namespace Elastos::Droid::Os;
using namespace Elastos::Droid::Utility;
using namespace Elastos::Droid::Content::Pm;
using namespace Elastos::Droid::Content::Res;
using namespace Elastos::Droid::Database::Sqlite;
using namespace Elastos::Droid::Graphics::Drawable;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CContextWrapper), public ContextWrapper
{

public:
    ICONTEXT_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /**
     * @return the base context as set by the constructor or setBaseContext
     */
    CARAPI GetBaseContext(
        /* [out] */ IContext** context);

    CARAPI constructor(
        /* [in] */ IContext* context);

private:
    // TODO: Add your private member variables here.
};

}
}
}

#endif // __ELASTOS_DROID_CONTENT_CCONTEXTWRAPPER_H__
