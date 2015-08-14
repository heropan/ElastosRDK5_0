
#ifndef __ELASTOS_DROID_CONTENT_RES_CCOLORSTATELISTHELPER_H__
#define __ELASTOS_DROID_CONTENT_RES_CCOLORSTATELISTHELPER_H__

#include "_CColorStateListHelper.h"
#include "content/res/CColorStateList.h"

using Elastos::Droid::Content::Res::IColorStateList;
using Elastos::Droid::Content::Res::IResources;
using Org::Xmlpull::V1::IXmlPullParser;


namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

CarClass(CColorStateListHelper)
{
public:
    CARAPI ValueOf(
        /* [in] */ Int32 color,
        /* [out] */ IColorStateList** ppList);

    CARAPI CreateFromXml(
        /* [in] */ IResources* pR,
        /* [in] */ IXmlPullParser* pParser,
        /* [out] */ IColorStateList** ppList);

};

} // namespace Res
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_CONTENT_RES_CCOLORSTATELISTHELPER_H__
