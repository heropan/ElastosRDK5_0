#ifndef __CXMLPULLATTRIBUTES_H__
#define __CXMLPULLATTRIBUTES_H__

#include "_CXmlPullAttributes.h"

using Org::Xmlpull::V1::IXmlPullParser;

namespace Elastos {
namespace Droid {
namespace Utility {

CarClass(CXmlPullAttributes)
{
public:

    CARAPI constructor(
        /* [in] */ IXmlPullParser* parser);

    CARAPI GetAttributeCount(
        /* [out] */ Int32 * count);

    CARAPI GetAttributeName(
        /* [in] */ Int32 index,
        /* [out] */ String * name);

    CARAPI GetAttributeValue(
        /* [in] */ Int32 index,
        /* [out] */ String * value);

    CARAPI GetAttributeValueEx(
        /* [in] */ const String& namespace1,
        /* [in] */ const String& name,
        /* [out] */ String * value);

    CARAPI GetPositionDescription(
        /* [out] */ String * des);

    CARAPI GetAttributeNameResource(
        /* [in] */ Int32 index,
        /* [out] */ Int32 * resId);

    CARAPI GetAttributeListValue(
        /* [in] */ const String& namespace1,
        /* [in] */ const String& attribute,
        /* [in] */ const ArrayOf<String> &options,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32 * index);

    CARAPI GetAttributeBooleanValue(
        /* [in] */ const String& namespace1,
        /* [in] */ const String& attribute,
        /* [in] */ Boolean defaultValue,
        /* [out] */ Boolean * value);

    CARAPI GetAttributeResourceValue(
        /* [in] */ const String& namespace1,
        /* [in] */ const String& attribute,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32 * value);

    CARAPI GetAttributeIntValue(
        /* [in] */ const String& namespace1,
        /* [in] */ const String& attribute,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32 * value);

    CARAPI GetAttributeUnsignedIntValue(
        /* [in] */ const String& namespace1,
        /* [in] */ const String& attribute,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32 * value);

    CARAPI GetAttributeFloatValue(
        /* [in] */ const String& namespace1,
        /* [in] */ const String& attribute,
        /* [in] */ Float defaultValue,
        /* [out] */ Float * value);

    CARAPI GetAttributeListValueEx(
        /* [in] */ Int32 index,
        /* [in] */ const ArrayOf<String> &options,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32 * value);

    CARAPI GetAttributeBooleanValueEx(
        /* [in] */ Int32 index,
        /* [in] */ Boolean defaultValue,
        /* [out] */ Boolean * value);

    CARAPI GetAttributeResourceValueEx(
        /* [in] */ Int32 index,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32 * value);

    CARAPI GetAttributeIntValueEx(
        /* [in] */ Int32 index,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32 * value);

    CARAPI GetAttributeUnsignedIntValueEx(
        /* [in] */ Int32 index,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32 * value);

    CARAPI GetAttributeFloatValueEx(
        /* [in] */ Int32 index,
        /* [in] */ Float defaultValue,
        /* [out] */ Float * value);

    CARAPI GetIdAttribute(
        /* [out] */ String * attr);

    CARAPI GetClassAttribute(
        /* [out] */ String * attr);

    CARAPI GetIdAttributeResourceValue(
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32 * value);

    CARAPI GetStyleAttribute(
        /* [out] */ Int32 * value);
private:

    AutoPtr<IXmlPullParser> mParser;
};

} // namespace Utility
} // namespace Droid
} // namespace Elastos

#endif
