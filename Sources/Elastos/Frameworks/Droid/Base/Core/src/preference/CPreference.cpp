
#include "CPreference.h"

namespace Elastos {
namespace Droid {
namespace Preference {

ECode CPreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    Preference::Init(context, attrs, defStyle);
    return NOERROR;
}

ECode CPreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    Preference::Init(context, attrs);
    return NOERROR;
}

ECode CPreference::constructor(
    /* [in] */ IContext* context)
{
    Preference::Init(context);
    return NOERROR;
}

} // Preference
} // Droid
} // Elastos
