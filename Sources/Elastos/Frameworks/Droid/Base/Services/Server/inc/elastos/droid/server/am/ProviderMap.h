
#ifndef __ELASTOS_DROID_SERVER_AM_PROVIDERMAP_H__
#define __ELASTOS_DROID_SERVER_AM_PROVIDERMAP_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Etl::List;
using Elastos::Droid::Content::IComponentName;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class ContentProviderRecord;
class CActivityManagerService;

class ProviderMap : public Object
{
public:

    typedef HashMap<String, AutoPtr<ContentProviderRecord> > NameRecordHashMap;
    typedef HashMap<AutoPtr<IComponentName>, AutoPtr<ContentProviderRecord> > ClassRecordHashMap;
    typedef typename NameRecordHashMap::ValueType NameRecordValueType;
    typedef typename NameRecordHashMap::Iterator NameRecordIterator;
    typedef typename ClassRecordHashMap::ValueType ClassRecordValueType;
    typedef typename ClassRecordHashMap::Iterator ClassRecordIterator;

    ProviderMap(
        /* [in] */ CActivityManagerService* am);

    virtual ~ProviderMap();

    CARAPI_(AutoPtr<ContentProviderRecord>) GetProviderByName(
        /* [in] */ const String& name);

    CARAPI_(AutoPtr<ContentProviderRecord>) GetProviderByName(
        /* [in] */ const String& name,
        /* [in] */ Int32 userId);

    CARAPI_(AutoPtr<ContentProviderRecord>) GetProviderByClass(
        /* [in] */ IComponentName* name);

    CARAPI_(AutoPtr<ContentProviderRecord>) GetProviderByClass(
        /* [in] */ IComponentName* name,
        /* [in] */ Int32 userId);

    CARAPI PutProviderByName(
        /* [in] */ const String& name,
        /* [in] */ ContentProviderRecord* provider);

    CARAPI PutProviderByClass(
        /* [in] */ IComponentName* name,
        /* [in] */ ContentProviderRecord* provider);

    CARAPI RemoveProviderByName(
        /* [in] */ const String& name,
        /* [in] */ Int32 userId);

    CARAPI RemoveProviderByClass(
        /* [in] */ IComponentName* name,
        /* [in] */ Int32 userId);

    CARAPI_(AutoPtr<ProviderMap::NameRecordHashMap>) GetProvidersByName(
        /* [in] */ Int32 userId);

    CARAPI_(AutoPtr<ProviderMap::ClassRecordHashMap>) GetProvidersByClass(
        /* [in] */ Int32 userId);

    CARAPI_(Boolean) CollectForceStopProvidersLocked(
        /* [in] */ const String& name,
        /* [in] */ Int32 appId,
        /* [in] */ Boolean doit,
        /* [in] */ Boolean evenPersistent,
        /* [in] */ Int32 userId,
        /* [in] */ ProviderMap::ClassRecordHashMap* providers,
        /* [in] */ List<AutoPtr<ContentProviderRecord> >* result);

    CARAPI_(Boolean) CollectForceStopProviders(
        /* [in] */ const String& name,
        /* [in] */ Int32 appId,
        /* [in] */ Boolean doit,
        /* [in] */ Boolean evenPersistent,
        /* [in] */ Int32 userId,
        /* [in] */ List<AutoPtr<ContentProviderRecord> >* result);

private:
    static const String TAG;
    static const Boolean DBG;

    CActivityManagerService* mAm;

    NameRecordHashMap mSingletonByName;
    ClassRecordHashMap mSingletonByClass;

    HashMap<Int32, AutoPtr<NameRecordHashMap> > mProvidersByNamePerUser;
    HashMap<Int32, AutoPtr<ClassRecordHashMap> > mProvidersByClassPerUser;

    typedef typename HashMap<Int32, AutoPtr<NameRecordHashMap> >::ValueType ProviderByNameValueType;
    typedef typename HashMap<Int32, AutoPtr<ClassRecordHashMap> >::ValueType ProviderByClassValueType;

};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_AM_PROVIDERMAP_H__
