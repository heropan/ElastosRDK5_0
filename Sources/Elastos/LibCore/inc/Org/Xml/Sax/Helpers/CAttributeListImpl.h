
#ifndef __CATTRIBUTELISTIMPL_H__
#define __CATTRIBUTELISTIMPL_H__

#include "_CAttributeListImpl.h"
#include "cmdef.h"
#include <elastos/List.h>

using Elastos::Utility::List;

namespace Org {
namespace Xml {
namespace Sax {
namespace Helpers {

CarClass(CAttributeListImpl)
{
public:
    CAttributeListImpl();

public:
    CARAPI GetLength(
        /* [out] */ Int32* length);

    CARAPI GetName(
        /* [in] */ Int32 i,
        /* [out] */ String* name);

    CARAPI GetType(
        /* [in] */ Int32 i,
        /* [out] */ String* type);

    CARAPI GetValue(
        /* [in] */ Int32 i,
        /* [out] */ String* value);

    CARAPI GetType(
        /* [in] */ const String& name,
        /* [out] */ String* type);

    CARAPI GetValue(
        /* [in] */ const String& name,
        /* [out] */ String* value);

    CARAPI SetAttributeList(
        /* [in] */ IAttributeList* atts);

    CARAPI AddAttribute(
        /* [in] */ const String& name,
        /* [in] */ const String& type,
        /* [in] */ const String& value);

    CARAPI RemoveAttribute(
        /* [in] */ const String& name);

    CARAPI Clear();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IAttributeList* atts);

private:
    List<String> mNames;
    List<String> mTypes;
    List<String> mValues;

    CARAPI_(Int32) GetListIndex(
        /* [in] */ List<String>* list,
        /* [in] */ const String& value);
//    AutoPtr<IObjectContainer> mNames; //  = new ArrayList<String>();
//    AutoPtr<IObjectContainer> mTypes; // = new ArrayList<String>();
//    AutoPtr<IObjectContainer> mValues; // = new ArrayList<String>();
};

} // namespace Helpers
} // namespace Sax
} // namespace Xml
} // namespace Org

#endif // __CATTRIBUTELISTIMPL_H__
