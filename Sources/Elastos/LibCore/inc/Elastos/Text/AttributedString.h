#ifndef __ATTRIBUTEDSTRING_H__
#define __ATTRIBUTEDSTRING_H__

#include "cmdef.h"
#include "Elastos.Core_server.h"
#include <elastos.h>
#include <elastos/HashMap.h>
#include <elastos/List.h>
#include <elastos/HashSet.h>

using Elastos::Utility::List;
using Elastos::Utility::HashSet;
using Elastos::Utility::HashMap;
// using Elastos::Utility::IObjectMap;
using Elastos::Text::IAttributedCharacterIteratorAttribute;

_ETL_NAMESPACE_BEGIN
template<> struct Hash<AutoPtr<IAttributedCharacterIteratorAttribute> >
{
    size_t operator()(AutoPtr<IAttributedCharacterIteratorAttribute> s) const
    {
        assert(s != NULL);
        return (size_t)s.Get();
    }
};
_ETL_NAMESPACE_END

namespace Elastos {
namespace Text {

typedef HashMap<AutoPtr<IAttributedCharacterIteratorAttribute>, AutoPtr<IInterface> > AttributeObjectMap;
typedef typename AttributeObjectMap::Iterator AttributeObjectMapIterator;

class AttributedString
{
public:
    class Range : public ElRefBase
    {
    public:
        Range(
            /* [in] */ Int32 s,
            /* [in] */ Int32 e,
            /* [in] */ IInterface* v);

        Range& operator = (
            /* [in] */ const Range& other);

    public:
        Int32 mStart;
        Int32 mEnd;
        AutoPtr<IInterface> mValue;
    };

    class AttributedIterator
            : public ElRefBase
            , public IAttributedCharacterIterator
    {
    public:
        AttributedIterator(
            /* [in] */ AttributedString* attrString);

        AttributedIterator(
            /* [in] */ AttributedString* attrString,
            /* [in] */ ArrayOf<IAttributedCharacterIteratorAttribute*>* attributes,
            /* [in] */ Int32 begin,
            /* [in] */ Int32 end);

        ~AttributedIterator();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI Clone(
            /* [out] */ ICharacterIterator** copy);

        CARAPI Current(
            /* [out] */ Char32* value);

        CARAPI First(
            /* [out] */ Char32* value);

        CARAPI GetBeginIndex(
            /* [out] */ Int32* index);

        CARAPI GetEndIndex(
            /* [out] */ Int32* index);

        CARAPI GetIndex(
            /* [out] */ Int32* index);

        CARAPI GetAllAttributeKeys(
            /* [out] */ IObjectContainer** allAttributedKeys);

        CARAPI GetAttribute(
            /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
            /* [out] */ IInterface** instance);

        CARAPI GetAttributes(
                /* [out, callee] */ AttributeObjectMap** attributes);

        CARAPI GetRunLimit(
            /* [out] */ Int32* index);

        CARAPI GetRunLimitEx(
            /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
            /* [out] */ Int32* index);

        CARAPI GetRunLimitEx2(
            /* [in] */ IObjectContainer* attributes,
            /* [out] */ Int32* index);

        CARAPI GetRunStart(
            /* [out] */ Int32* index);

        CARAPI GetRunStartEx(
            /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
            /* [out] */ Int32* index);

        CARAPI GetRunStartEx2(
            /* [in] */ IObjectContainer* attributes,
            /* [out] */ Int32* index);

        CARAPI Last(
            /* [out] */ Char32* lastValue);

        CARAPI Next(
            /* [out] */ Char32* nextValue);

        CARAPI Previous(
            /* [out] */ Char32* previousValue);

        CARAPI SetIndex(
            /* [in] */ Int32 location,
            /* [out] */ Char32* newChar);

    private:
        CARAPI_(Boolean) InRange(
            /* [in] */ Range* range);

        CARAPI_(Boolean) InRange(
            /* [in] */ List<AutoPtr<Range> >* ranges);

        CARAPI_(AutoPtr<IInterface>) CurrentValue(
            /* [in] */ List<AutoPtr<Range> >* ranges);

        CARAPI_(Int32) RunLimit(
            /* [in] */ List<AutoPtr<Range> >* ranges);

        CARAPI_(Int32) RunStart(
            /* [in] */ List<AutoPtr<Range> >* ranges);

    private:
        Int32 mBegin;
        Int32 mEnd;
        Int32 mOffset;

        AttributedString* mAttrString;

        HashSet<AutoPtr<IAttributedCharacterIteratorAttribute> > mAttributesAllowed;
    };

public:
    ~AttributedString();

    CARAPI Init(
        /* [in] */ IAttributedCharacterIterator* iterator);

    CARAPI Init(
        /* [in] */ IAttributedCharacterIterator* iterator,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ArrayOf<IAttributedCharacterIteratorAttribute*>* attributes);

    CARAPI Init(
        /* [in] */ IAttributedCharacterIterator* iterator,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI Init(
        /* [in] */ const String& value);

    // CARAPI Init(
    //     /* [in] */ const String& value,
    //     /* [in] */ IObjectMap * attributes);

    virtual CARAPI AddAttribute(
        /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
        /* [in] */ IInterface* value);

    virtual CARAPI AddAttributeEx(
        /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
        /* [in] */ IInterface* value,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    virtual CARAPI AddAttributes(
        /* [in] */ AttributeObjectMap* attributes,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    virtual CARAPI GetIterator(
        /* [out] */ IAttributedCharacterIterator** iterator);

    virtual CARAPI GetIteratorEx(
        /* [in] */ ArrayOf<IAttributedCharacterIteratorAttribute*>* attributes,
        /* [out] */ IAttributedCharacterIterator** iterator);

    virtual CARAPI GetIteratorEx2(
        /* [in] */ ArrayOf<IAttributedCharacterIteratorAttribute*>* attributes,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ IAttributedCharacterIterator** iterator);

private:
    CARAPI Init(
        /* [in] */ IAttributedCharacterIterator* iterator,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IObjectContainer* attributes);

public:
    String mText;

    typedef HashMap<AutoPtr<IAttributedCharacterIteratorAttribute>, AutoPtr<List<AutoPtr<Range> > > > AttributeRangeMap;
    typedef typename AttributeRangeMap::Iterator AttributeRangeMapIterator;

    AttributeRangeMap mAttributeMap;
};

} // namespace Text
} // namespace Elastos

#endif //__ATTRIBUTEDSTRING_H__
