
#ifndef __ATTRIBUTECACHE_H__
#define __ATTRIBUTECACHE_H__

#include "ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>
#include "elastos/Mutex.h"

using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Res::ITypedArray;

#ifndef HASH_EQUALTO_FUNC_FOR_AUTOPTR_INTARRAY
#define HASH_EQUALTO_FUNC_FOR_AUTOPTR_INTARRAY
_ETL_NAMESPACE_BEGIN
template<> struct Hash<AutoPtr<ArrayOf<Int32> > >
{
    size_t operator()(const AutoPtr<ArrayOf<Int32> > s) const {
        Int32 len = s != NULL ? s->GetLength() : 0;
        size_t size = 0;
        for (Int32 i = 0; i < len; i++)
            size += (*s)[i];
        return size;
    }
};

template<> struct EqualTo<AutoPtr<ArrayOf<Int32> > >
{
    size_t operator()(const AutoPtr<ArrayOf<Int32> > x, const AutoPtr<ArrayOf<Int32> > y) const
    {
        assert(x != NULL);
        return x->Equals(y);
    }
};
_ETL_NAMESPACE_END
#endif

namespace Elastos {
namespace Droid {
namespace Server {

class AttributeCache : public ElRefBase
{
public:
    class Entry : public ElRefBase
    {
    public:
        Entry(
            /* [in] */ IContext* c,
            /* [in] */ ITypedArray* ta);

        AutoPtr<IContext> mContext;
        AutoPtr<ITypedArray> mArray;
    };


    class Package
        : public ElRefBase
    {
    public:
        Package(
            /* [in] */ IContext* c);

        ~Package();

        AutoPtr<IContext> mContext;

    private:
        HashMap<Int32, HashMap<AutoPtr<ArrayOf<Int32> >, AutoPtr<Entry> >* > mMap;
        typedef HashMap<Int32, HashMap<AutoPtr<ArrayOf<Int32> >, AutoPtr<Entry> >* > EntryMap;
        typedef EntryMap::Iterator EntryIterator;
        friend class AttributeCache;
    };


public:
    static CARAPI_(void) Init(
        /* [in] */ IContext* context);

    static CARAPI_(AutoPtr<AttributeCache>) GetInstance();

    AttributeCache(
        /* [in] */ IContext* context);

    ~AttributeCache();

    CARAPI_(void) UpdateConfiguration(
        /* [in] */ IConfiguration* config);

    CARAPI RemoveUser(
        /* [in] */ Int32 userId);

    AutoPtr<Entry> Get(
        /* [in] */ Int32 userId,
        /* [in] */ const String& packageName,
        /* [in] */ Int32 resId,
        /* [in] */ ArrayOf<Int32>* styleable);

    CARAPI_(void) RemovePackage(
        /* [in] */ const String& packageName);

private:
    static AutoPtr<AttributeCache> sInstance;
    AutoPtr<IContext> mContext;
    HashMap<Int32, HashMap<String, AutoPtr<Package> >* > mPackages;
    AutoPtr<IConfiguration> mConfiguration;

    Object mLock;
    typedef HashMap<String, AutoPtr<Package> > PackageMap;
    typedef HashMap<String, AutoPtr<Package> >::Iterator PackageIterator;

    typedef HashMap<AutoPtr<ArrayOf<Int32> >, AutoPtr<Entry> > EntryMap;
    typedef HashMap<AutoPtr<ArrayOf<Int32> >, AutoPtr<Entry> >::Iterator EntryIterator;

};


}// Server
}// Droid
}// Elastos

#endif // __ATTRIBUTECACHE_H__
