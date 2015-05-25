
#ifndef __CXMLPULLPARSERFACTORYHELPER_H__
#define __CXMLPULLPARSERFACTORYHELPER_H__

#include "__Org_Xmlpull_V1_CXmlPullParserFactoryHelper.h"
#include "Singleton.h"

using Elastos::Core::Singleton;

namespace Org {
namespace Xmlpull {
namespace V1 {

CarClass(CXmlPullParserFactoryHelper)
    , public Singleton
    , public IXmlPullParserFactoryHelper
{
public:
    CAR_SINGLETON_DECL()

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
