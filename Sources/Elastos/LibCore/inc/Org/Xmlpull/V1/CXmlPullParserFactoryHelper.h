
#ifndef __CXMLPULLPARSERFACTORYHELPER_H__
#define __CXMLPULLPARSERFACTORYHELPER_H__

#include "_CXmlPullParserFactoryHelper.h"

namespace Org {
namespace Xmlpull {
namespace V1 {

CarClass(CXmlPullParserFactoryHelper)
{
public:
    CARAPI NewInstance(
        /* [out] */ IXmlPullParserFactory** xmlPullParserFac);

    CARAPI NewInstance(
        /* [in] */ const String& classNames,
     // /* [in] */  Class context,
        /* [out] */ IXmlPullParserFactory** xmlPullParserFac);
};

} // namespace V1
} // namespace Xmlpull
} // namespace Org

#endif // __CXMLPULLPARSERFACTORYHELPER_H__
