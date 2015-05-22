
#ifndef __CRAWHEADERS_H__
#define __CRAWHEADERS_H__

#include "_CRawHeaders.h"
#include "cmdef.h"
#include "Elastos.CoreLibrary_server.h"
#include "elastos/List.h"
#include "CCollections.h"
#include "CArrayList.h"
#include "CTreeMap.h"

using Elastos::Core::IComparator;
using Elastos::Utility::IMap;
using Elastos::Utility::List;
using Elastos::Utility::ISet;
using Elastos::Utility::IList;
using Elastos::Utility::CArrayList;
using Elastos::Utility::IMapEntry;
using Elastos::Utility::ICollections;
using Elastos::Utility::CCollections;
using Elastos::Utility::CTreeMap;

namespace Elastos {
namespace Net {
namespace Http {

CarClass(CRawHeaders)
{
private:
    class Comparator
        : public IComparator
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI Compare(
            /* [in] */ IInterface* lhs,
            /* [in] */ IInterface* rhs,
            /* [out] */ Int32* result);
    };
public:
    CRawHeaders();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IRawHeaders* copyFrom);

    CARAPI SetStatusLine(
        /* [in] */ const String& statusLine);

    CARAPI GetStatusLine(
        /* [out] */ String* str);

    CARAPI GetHttpMinorVersion(
        /* [out] */ Int32* value);

    CARAPI GetResponseCode(
        /* [out] */ Int32* value);

    CARAPI GetResponseMessage(
        /* [out] */ String* str);

    CARAPI AddLine(
        /* [in] */ const String& line);

    CARAPI Add(
        /* [in] */ const String& fieldName,
        /* [in] */ const String& value);

    CARAPI RemoveAll(
        /* [in] */ const String& fieldName);

    CARAPI AddAll(
        /* [in] */ const String& fieldName,
        /* [in] */ IList* headerFields);

    CARAPI Set(
        /* [in] */ const String& fieldName,
        /* [in] */ const String& value);

    CARAPI Length(
        /* [out] */ Int32* value);

    CARAPI GetFieldName(
        /* [in] */ Int32 index,
        /* [out] */ String* str);

    CARAPI GetValue(
        /* [in] */ Int32 index,
        /* [out] */ String* str);

    CARAPI Get(
        /* [in] */ const String& fieldName,
        /* [out] */ String* str);

    CARAPI GetAll(
        /* [in] */ ISet* fieldNames,
        /* [out] */ IRawHeaders** rh);

    CARAPI ToHeaderString(
        /* [out] */ String* str);

    CARAPI ToMultimap(
        /* [out] */ IMap** strmap);

    static CARAPI FromMultimap(
        /* [in] */ IMap* map,
        /* [out] */ IRawHeaders** rh);

private:
    CARAPI_(String) GetStringByIndex(
        /* [in] */ Int32 index);

private:
    static const AutoPtr<IComparator> FIELD_NAME_COMPARATOR;
    AutoPtr<IList> mNamesAndValues;
    String mStatusLine;
    Int32 mHttpMinorVersion;
    Int32 mResponseCode;
    String mResponseMessage;
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__CRAWHEADERS_H__
