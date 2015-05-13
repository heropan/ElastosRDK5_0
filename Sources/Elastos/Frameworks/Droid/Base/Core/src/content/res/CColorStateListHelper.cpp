
#include "content/res/CColorStateListHelper.h"
#include "content/res/CColorStateList.h"


namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

ECode CColorStateListHelper::ValueOf(
    /* [in] */ Int32 color,
    /* [out] */ IColorStateList** list)
{
    return CColorStateList::ValueOf(color, list);
}

ECode CColorStateListHelper::CreateFromXml(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ IColorStateList** list)
{
    return CColorStateList::CreateFromXml(r, parser, list);
}

} // namespace Res
} // namespace Content
} // namespace Droid
} // namespace Elastos

