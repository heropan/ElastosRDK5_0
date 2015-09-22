#ifndef __ELASTOS_DROID_SERVER_INTENTRESOLVER_H__
#define __ELASTOS_DROID_SERVER_INTENTRESOLVER_H__

#include "ext/frameworkext.h"
#include <elastos/utility/etl/Set.h>
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/List.h>
#include <elastos/utility/logging/Slogger.h>
//#include "net/Uri.h"

using Elastos::Utility::Set;
using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Etl::List;
using Elastos::IO::IPrintWriter;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Content::IIntent;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Server {

template <typename F, typename R>
class IntentResolver : public ElRefBase
{
public:
    IntentResolver();

    virtual ~IntentResolver();

    virtual CARAPI_(void) AddFilter(
        /* [in] */ F* filter);

    virtual CARAPI_(void) RemoveFilter(
        /* [in] */ F* f);

    virtual CARAPI_(void) RemoveFilterInternal(
        /* [in] */ F* f);

    // boolean dumpMap(PrintWriter out, String titlePrefix, String title,
    //         String prefix, Map<String, F[]> map, String packageName,
    //         boolean printFilter);

    // public boolean dump(PrintWriter out, String title, String prefix, String packageName,
    //         boolean printFilter);

//    /**
//     * Returns an iterator allowing filters to be removed.
//     */
//    public Iterator<F> filterIterator();

    /**
     * Returns a read-only set of the filters.
     */
    virtual CARAPI_(AutoPtr<Set< AutoPtr<F> > >) FilterSet()
    {
//        return Collections.unmodifiableSet(mFilters);
        return mFilters;
    }

    virtual CARAPI_(AutoPtr< List< AutoPtr<R> > >) QueryIntentFromList(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Boolean defaultOnly,
        /* [in] */ List< AutoPtr< ArrayOf<F*> > >* listCut,
        /* [in] */ Int32 userId);

    virtual CARAPI_(AutoPtr< List< AutoPtr<R> > >) QueryIntent(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ Boolean defaultOnly,
        /* [in] */ Int32 userId);

protected:
    /**
     * Control whether the given filter is allowed to go into the result
     * list.  Mainly intended to prevent adding multiple filters for the
     * same target object.
     */
    virtual CARAPI_(Boolean) AllowFilterResult(
        /* [in] */ F* filter,
        /* [in] */ List< AutoPtr<R> >* dest)
    {
        return TRUE;
    }

    /**
     * Returns whether the object associated with the given filter is
     * "stopped," that is whether it should not be included in the result
     * if the intent requests to excluded stopped objects.
     */
    virtual CARAPI_(Boolean) IsFilterStopped(
        /* [in] */ F* filter,
        /* [in] */ Int32 userId)
    {
        return FALSE;
    }

    virtual CARAPI_(String) PackageForFilter(
        /* [in] */ F* filter) = 0;

    virtual CARAPI_(AutoPtr< ArrayOf<F*> >) NewArray(
        /* [in] */ Int32 size) = 0;

    virtual CARAPI_(AutoPtr<R>) NewResult(
        /* [in] */ F* f,
        /* [in] */ Int32 match,
        /* [in] */ Int32 userId);

    virtual CARAPI_(void) SortResults(
        /* [in] */ List< AutoPtr<R> >* results);

    virtual CARAPI_(void) DumpFilter(
        /* [in] */ IPrintWriter* out,
        /* [in] */ const String& prefix,
        /* [in] */ F* filter);

private:
    CARAPI_(void) AddFilter(
        /* [in] */ HashMap<String, AutoPtr< ArrayOf<F*> > >& map,
        /* [in] */ const String& name,
        /* [in] */ F* filter);

    CARAPI_(Int32) RegisterMimeTypes(
        /* [in] */ F* f,
        /* [in] */ const char* prefix);

    CARAPI_(Int32) UnregisterMimeTypes(
        /* [in] */ F* f,
        /* [in] */ const char* prefix);

    CARAPI_(Int32) RegisterIntentFilter(
        /* [in] */ F* f,
        /* [in] */ ArrayOf<String>* slist,
        /* [in] */ HashMap<String, AutoPtr< ArrayOf<F*> > >* dest,
        /* [in] */ const char* prefix);

    CARAPI_(Int32) UnregisterIntentFilter(
        /* [in] */ F* f,
        /* [in] */ ArrayOf<String>* slist,
        /* [in] */ HashMap<String, AutoPtr< ArrayOf<F*> > >* dest,
        /* [in] */ const char* prefix);

    CARAPI_(void) RemoveAllObjects(
        /* [in] */ HashMap<String, AutoPtr< ArrayOf<F*> > >& map,
        /* [in] */ const String& name,
        /* [in] */ F* object);

    static CARAPI_(AutoPtr< ArrayOf<String> >) GetFastIntentCategories(
        /* [in] */ IIntent* intent);

    CARAPI_(void) BuildResolveList(
        /* [in] */ IIntent* intent,
        /* [in] */ ArrayOf<String>* categories,
        /* [in] */ Boolean debug,
        /* [in] */ Boolean defaultOnly,
        /* [in] */ const String& resolvedType,
        /* [in] */ const String& scheme,
        /* [in] */ ArrayOf<F*>* src,
        /* [in] */ List< AutoPtr<R> >* dest,
        /* [in] */ Int32 userId);

    // private void verifyDataStructures(IntentFilter src);

    // private void compareMaps(IntentFilter src, String name, HashMap<String, F[]> cur,
    //         HashMap<String, ArrayList<F>> old);

private:
    // final private static String TAG = "IntentResolver";
    // final private static boolean DEBUG = false;
    // final private static boolean localLOGV = DEBUG || false;
    // final private static boolean VALIDATE = false;

    // private final IntentResolverOld<F, R> mOldResolver = new IntentResolverOld<F, R>() {
    //     @Override protected String packageForFilter(F filter) {
    //         return IntentResolver.this.packageForFilter(filter);
    //     }
    //     @Override protected boolean allowFilterResult(F filter, List<R> dest) {
    //         return IntentResolver.this.allowFilterResult(filter, dest);
    //     }
    //     @Override protected boolean isFilterStopped(F filter, int userId) {
    //         return IntentResolver.this.isFilterStopped(filter, userId);
    //     }
    //     @Override protected R newResult(F filter, int match, int userId) {
    //         return IntentResolver.this.newResult(filter, match, userId);
    //     }
    //     @Override protected void sortResults(List<R> results) {
    //         IntentResolver.this.sortResults(results);
    //     }
    // };

    /**
     * All filters that have been registered.
     */
    AutoPtr<Set< AutoPtr<F> > > mFilters;

    /**
     * All of the MIME types that have been registered, such as "image/jpeg",
     * "image/ *", or "{@literal *}/ *".
     */
    HashMap<String, AutoPtr< ArrayOf<F*> > > mTypeToFilter;

    /**
     * The base names of all of all fully qualified MIME types that have been
     * registered, such as "image" or "*".  Wild card MIME types such as
     * "image/ *" will not be here.
     */
    HashMap<String, AutoPtr< ArrayOf<F*> > > mBaseTypeToFilter;

     /**
     * The base names of all of the MIME types with a sub-type wildcard that
     * have been registered.  For example, a filter with "image/ *" will be
     * included here as "image" but one with "image/jpeg" will not be
     * included here.  This also includes the "*" for the "{@literal *}/ *"
     * MIME type.
     */
    HashMap<String, AutoPtr< ArrayOf<F*> > > mWildTypeToFilter;

    /**
     * All of the URI schemes (such as http) that have been registered.
     */
    HashMap<String, AutoPtr< ArrayOf<F*> > > mSchemeToFilter;

    /**
     * All of the actions that have been registered, but only those that did
     * not specify data.
     */
    HashMap<String, AutoPtr< ArrayOf<F*> > > mActionToFilter;

    /**
     * All of the actions that have been registered and specified a MIME type.
     */
    HashMap<String, AutoPtr< ArrayOf<F*> > > mTypedActionToFilter;
};

template <typename F, typename R>
IntentResolver<F, R>::IntentResolver()
    : mTypeToFilter(7)
    , mBaseTypeToFilter(7)
    , mWildTypeToFilter(7)
    , mSchemeToFilter(7)
    , mActionToFilter(7)
    , mTypedActionToFilter(7)
{
    mFilters = new Set< AutoPtr<F> >();
}

template <typename F, typename R>
IntentResolver<F, R>::~IntentResolver()
{
    mFilters = NULL;;
    mTypeToFilter.Clear();
    mBaseTypeToFilter.Clear();
    mWildTypeToFilter.Clear();
    mSchemeToFilter.Clear();
    mActionToFilter.Clear();
    mTypedActionToFilter.Clear();
}

template <typename F, typename R>
void IntentResolver<F, R>::AddFilter(
    /* [in] */ F* f)
{
    if (f == NULL) {
        return;
    }
    // if (localLOGV) {
    //     Slog.v(TAG, "Adding filter: " + f);
    //     f.dump(new LogPrinter(Log.VERBOSE, TAG, Log.LOG_ID_SYSTEM), "      ");
    //     Slog.v(TAG, "    Building Lookup Maps:");
    // }

    mFilters->Insert(f);

    AutoPtr< ArrayOf<String> > schemes;
    AutoPtr< ArrayOf<String> > actions;
    f->GetSchemes((ArrayOf<String>**)&schemes);
    Int32 numS = RegisterIntentFilter(
            f, schemes,
            &mSchemeToFilter, "    Scheme: ");
    Int32 numT = RegisterMimeTypes(f, "    Type: ");
    if (numS == 0 && numT == 0) {
        f->GetActions((ArrayOf<String>**)&actions);
        RegisterIntentFilter(
                f, actions,
                &mActionToFilter, "    Action: ");
    }
    if (numT != 0) {
        actions = NULL;
        f->GetActions((ArrayOf<String>**)&actions);
        RegisterIntentFilter(
                f, actions,
                &mTypedActionToFilter, "    TypedAction: ");
    }

    // if (VALIDATE) {
    //     mOldResolver.addFilter(f);
    //     verifyDataStructures(f);
    // }
}

template <typename F, typename R>
void IntentResolver<F, R>::RemoveFilter(
    /* [in] */ F* f)
{
    if (f == NULL) {
        return;
    }

    RemoveFilterInternal(f);
    mFilters->Erase(f);

    // if (VALIDATE) {
    //     mOldResolver.removeFilter(f);
    //     verifyDataStructures(f);
    // }
}

template <typename F, typename R>
void IntentResolver<F, R>::RemoveFilterInternal(
    /* [in] */ F* f)
{
    if (f == NULL) {
        return;
    }
    // if (localLOGV) {
    //     Slog.v(TAG, "Removing filter: " + f);
    //     f.dump(new LogPrinter(Log.VERBOSE, TAG, Log.LOG_ID_SYSTEM), "      ");
    //     Slog.v(TAG, "    Cleaning Lookup Maps:");
    // }

    AutoPtr< ArrayOf<String> > schemes;
    AutoPtr< ArrayOf<String> > actions;
    f->GetSchemes((ArrayOf<String>**)&schemes);
    Int32 numS = UnregisterIntentFilter(
            f, schemes,
            &mSchemeToFilter, "      Scheme: ");
    Int32 numT = UnregisterMimeTypes(f, "      Type: ");
    if (numS == 0 && numT == 0) {
        f->GetActions((ArrayOf<String>**)&actions);
        UnregisterIntentFilter(
                f, actions,
                &mActionToFilter, "      Action: ");
    }
    if (numT != 0) {
        actions = NULL;
        f->GetActions((ArrayOf<String>**)&actions);
        UnregisterIntentFilter(
                f, actions,
                &mTypedActionToFilter, "      TypedAction: ");
    }
}

template <typename F, typename R>
AutoPtr< List< AutoPtr<R> > > IntentResolver<F, R>::QueryIntentFromList(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Boolean defaultOnly,
    /* [in] */ List< AutoPtr< ArrayOf<F*> > >* listCut,
    /* [in] */ Int32 userId)
{
    AutoPtr< List< AutoPtr<R> > > resultList = new List< AutoPtr<R> >();

    Boolean debug = FALSE;
//    Boolean debug = localLOGV ||
//                ((intent.getFlags() & Intent_Flag_Debug_LogResolution) != 0);

    AutoPtr< ArrayOf<String> > categories = GetFastIntentCategories(intent);
    String scheme;
    intent->GetScheme(&scheme);
    typename List< AutoPtr< ArrayOf<F*> > >::Iterator it;
    for (it = listCut->Begin(); it != listCut->End(); ++it) {
        BuildResolveList(intent, categories, debug, defaultOnly, resolvedType,
                scheme, (*it).Get(), resultList, userId);
    }
    SortResults(resultList);
    return resultList;
}

template <typename F, typename R>
AutoPtr< List< AutoPtr<R> > > IntentResolver<F, R>::QueryIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Boolean defaultOnly,
    /* [in] */ Int32 userId)
{
    String scheme;
    intent->GetScheme(&scheme);

    AutoPtr< List< AutoPtr<R> > > finalList = new List< AutoPtr<R> >();

    Boolean debug = FALSE;
//    Boolean debug = localLOGV ||
//                ((intent.getFlags() & Intent.FLAG_DEBUG_LOG_RESOLUTION) != 0);

//    if (debug) Log.v(
//            TAG, "Resolving type " + resolvedType + " scheme " + scheme
//            + " of intent " + intent);

    AutoPtr< ArrayOf<F*> > firstTypeCut;
    AutoPtr< ArrayOf<F*> > secondTypeCut;
    AutoPtr< ArrayOf<F*> > thirdTypeCut;
    AutoPtr< ArrayOf<F*> > schemeCut;

    // If the intent includes a MIME type, then we want to collect all of
    // the filters that match that MIME type.
    if (!resolvedType.IsNull()) {
        Int32 slashpos = resolvedType.IndexOf('/');
        if (slashpos > 0) {
            /*final*/ String baseType = resolvedType.Substring(0, slashpos);
            if (!baseType.Equals("*")) {
                if (resolvedType.GetLength() != slashpos + 2
                        || resolvedType.GetChar(slashpos+1) != '*') {
                    // Not a wild card, so we can just look for all filters that
                    // completely match or wildcards whose base type matches.
                    typename HashMap<String, AutoPtr< ArrayOf<F*> > >::Iterator it = mTypeToFilter.Find(resolvedType);
                    if (it != mTypeToFilter.End()) {
                        firstTypeCut = it->mSecond;
                    }
                    // if (debug) Slog.v(TAG, "First type cut: " + firstTypeCut);
                    it = mWildTypeToFilter.Find(baseType);
                    if (it != mWildTypeToFilter.End()) {
                        secondTypeCut = it->mSecond;
                    }
                    // if (debug) Slog.v(TAG, "Second type cut: " + secondTypeCut);
                } else {
                    // We can match anything with our base type.
                    typename HashMap<String, AutoPtr< ArrayOf<F*> > >::Iterator it = mBaseTypeToFilter.Find(baseType);
                    if (it != mBaseTypeToFilter.End()) {
                        firstTypeCut = it->mSecond;
                    }
                    // if (debug) Slog.v(TAG, "First type cut: " + firstTypeCut);
                    it = mWildTypeToFilter.Find(baseType);
                    if (it != mWildTypeToFilter.End()) {
                        secondTypeCut = it->mSecond;
                    }
                    // if (debug) Slog.v(TAG, "Second type cut: " + secondTypeCut);
                }

                // Any */* types always apply, but we only need to do this
                // if the intent type was not already */*.
                typename HashMap<String, AutoPtr< ArrayOf<F*> > >::Iterator it = mWildTypeToFilter.Find(String("*"));
                if (it != mWildTypeToFilter.End()) {
                    thirdTypeCut = it->mSecond;
                }
                // if (debug) Slog.v(TAG, "Third type cut: " + thirdTypeCut);
            } else {
                String action;
                intent->GetAction(&action);
                if (!action.IsNull()) {
                // The intent specified any type ({@literal *}/*).  This
                // can be a whole heck of a lot of things, so as a first
                // cut let's use the action instead.
                typename HashMap<String, AutoPtr< ArrayOf<F*> > >::Iterator it = mTypedActionToFilter.Find(action);
                if (it != mTypedActionToFilter.End()) {
                    firstTypeCut = it->mSecond;
                }
                // if (debug) Slog.v(TAG, "Typed Action list: " + firstTypeCut);
            }
            }
        }
    }

    // If the intent includes a data URI, then we want to collect all of
    // the filters that match its scheme (we will further refine matches
    // on the authority and path by directly matching each resulting filter).
    if (!scheme.IsNull()) {
        typename HashMap<String, AutoPtr< ArrayOf<F*> > >::Iterator it = mSchemeToFilter.Find(scheme);
        if (it != mSchemeToFilter.End()) {
            schemeCut = it->mSecond;
        }
        // if (debug) Slog.v(TAG, "Scheme list: " + schemeCut);
    }

    // If the intent does not specify any data -- either a MIME type or
    // a URI -- then we will only be looking for matches against empty
    // data.
    String action;
    intent->GetAction(&action);
    if (resolvedType.IsNull() && scheme.IsNull() && !action.IsNull()) {
        typename HashMap<String, AutoPtr< ArrayOf<F*> > >::Iterator it = mActionToFilter.Find(action);
        if (it != mActionToFilter.End()) {
            firstTypeCut = it->mSecond;
        }
        // if (debug) Slog.v(TAG, "Action list: " + firstTypeCut);
    }

    AutoPtr< ArrayOf<String> > categories = GetFastIntentCategories(intent);
    if (firstTypeCut != NULL) {
        BuildResolveList(intent, categories, debug, defaultOnly,
                resolvedType, scheme, firstTypeCut, finalList, userId);
    }
    if (secondTypeCut != NULL) {
        BuildResolveList(intent, categories, debug, defaultOnly,
                resolvedType, scheme, secondTypeCut, finalList, userId);
    }
    if (thirdTypeCut != NULL) {
        BuildResolveList(intent, categories, debug, defaultOnly,
                resolvedType, scheme, thirdTypeCut, finalList, userId);
    }
    if (schemeCut != NULL) {
        BuildResolveList(intent, categories, debug, defaultOnly,
                resolvedType, scheme, schemeCut, finalList, userId);
    }
    SortResults(finalList);

    // if (VALIDATE) {
    //     List<R> oldList = mOldResolver.queryIntent(intent, resolvedType, defaultOnly, userId);
    //     if (oldList.size() != finalList.size()) {
    //         ValidationFailure here = new ValidationFailure();
    //         here.fillInStackTrace();
    //         Log.wtf(TAG, "Query result " + intent + " size is " + finalList.size()
    //                 + "; old implementation is " + oldList.size(), here);
    //     }
    // }

    // if (debug) {
    //     Slog.v(TAG, "Final result list:");
    //     for (R r : finalList) {
    //         Slog.v(TAG, "  " + r);
    //     }
    // }
    return finalList;
}

template <typename F, typename R>
AutoPtr<R> IntentResolver<F, R>::NewResult(
    /* [in] */ F* f,
    /* [in] */ Int32 match,
    /* [in] */ Int32 userId)
{
    return (R*)f;
}

template <typename F, typename R>
void IntentResolver<F, R>::SortResults(
    /* [in] */ List< AutoPtr<R> >* results)
{
    PRINT_FILE_LINE_EX("TODO: SortResults");
    // Collections.sort(results, mResolvePrioritySorter);
}

template <typename F, typename R>
void IntentResolver<F, R>::DumpFilter(
    /* [in] */ IPrintWriter* out,
    /* [in] */ const String& prefix,
    /* [in] */ F* filter)
{
//    out.print(prefix); out.println(filter);
    assert(0);
}

template <typename F, typename R>
void IntentResolver<F, R>::AddFilter(
    /* [in] */ HashMap<String, AutoPtr< ArrayOf<F*> > >& map,
    /* [in] */ const String& name,
    /* [in] */ F* filter)
{
    AutoPtr< ArrayOf<F*> > array;
    typename HashMap<String, AutoPtr< ArrayOf<F*> > >::Iterator it = map.Find(name);
    if (it != map.End()) {
        array = it->mSecond;
    }
    if (array == NULL) {
        array = NewArray(2);
        map[name] = array;
        array->Set(0, filter);
    }
    else {
        const Int32 N = array->GetLength();
        Int32 i = N;
        while (i > 0 && (*array)[i-1] == NULL) {
            i--;
        }
        if (i < N) {
            array->Set(i, filter);
        }
        else {
            AutoPtr< ArrayOf<F*> > newa = NewArray((N * 3) / 2);
            for (i = 0; i < N; ++i) {
                newa->Set(i, (*array)[i]);
            }
            newa->Set(N, filter);
            map[name] = newa;
        }
    }
}

template <typename F, typename R>
Int32 IntentResolver<F, R>::RegisterMimeTypes(
    /* [in] */ F* filter,
    /* [in] */ const char* prefix)
{
    AutoPtr< ArrayOf<String> > types;
    filter->GetTypes((ArrayOf<String>**)&types);
    if (types == NULL) return 0;

    Int32 i;
    for(i = 0; i < types->GetLength(); ++i) {
        String name = (*types)[i];
        // if (localLOGV) Slog.v(TAG, prefix + name);
        String baseName = name;
        const Int32 slashpos = name.IndexOf('/');
        if (slashpos > 0) {
            baseName = name.Substring(0, slashpos);
        }
        else {
            name = name + "/*";
        }

        AddFilter(mTypeToFilter, name, filter);

        if (slashpos > 0) {
            AddFilter(mBaseTypeToFilter, baseName, filter);
        }
        else {
            AddFilter(mWildTypeToFilter, baseName, filter);
        }
    }

    return i;
}

template <typename F, typename R>
Int32 IntentResolver<F, R>::UnregisterMimeTypes(
    /* [in] */ F* filter,
    /* [in] */ const char* prefix)
{
    AutoPtr< ArrayOf<String> > types;
    filter->GetTypes((ArrayOf<String>**)&types);
    if (types == NULL) return 0;

    Int32 i;
    for(i = 0; i < types->GetLength(); ++i) {
        String name = (*types)[i];
        // if (localLOGV) Slog.v(TAG, prefix + name);
        String baseName = name;
        Int32 slashpos = name.IndexOf('/');
        if (slashpos > 0) {
            baseName = name.Substring(0, slashpos);
        }
        else {
            name = name + "/*";
        }

        RemoveAllObjects(mTypeToFilter, name, filter);

        if (slashpos > 0) {
            RemoveAllObjects(mBaseTypeToFilter, baseName, filter);
        }
        else {
            RemoveAllObjects(mWildTypeToFilter, baseName, filter);
        }
    }
    return i;
}

template <typename F, typename R>
Int32 IntentResolver<F, R>::RegisterIntentFilter(
    /* [in] */ F* filter,
    /* [in] */ ArrayOf<String>* slist,
    /* [in] */ HashMap<String, AutoPtr< ArrayOf<F*> > >* dest,
    /* [in] */ const char* prefix)
{
    if (slist == NULL) return 0;

    Int32 i;
    for(i = 0; i < slist->GetLength(); ++i) {
        String name = (*slist)[i];
        // if (localLOGV) Slog.v(TAG, prefix + name);
        AddFilter(*dest, name, filter);
    }
    return i;
}

template <typename F, typename R>
Int32 IntentResolver<F, R>::UnregisterIntentFilter(
    /* [in] */ F* filter,
    /* [in] */ ArrayOf<String>* slist,
    /* [in] */ HashMap<String, AutoPtr< ArrayOf<F*> > >* dest,
    /* [in] */ const char* prefix)
{
    if (slist == NULL) return 0;

    Int32 i;
    for(i = 0; i < slist->GetLength(); ++i) {
        String name = (*slist)[i];
        // if (localLOGV) Slog.v(TAG, prefix + name);
        RemoveAllObjects(*dest, name, filter);
    }
    return i;
}

template <typename F, typename R>
void IntentResolver<F, R>::RemoveAllObjects(
    /* [in] */ HashMap<String, AutoPtr< ArrayOf<F*> > >& map,
    /* [in] */ const String& name,
    /* [in] */ F* object)
{
    AutoPtr< ArrayOf<F*> > array;
    typename HashMap<String, AutoPtr< ArrayOf<F*> > >::Iterator it = map.Find(name);
    if (it != map.End()) {
        array = it->mSecond;
    }
    if (array != NULL) {
        Int32 LAST = array->GetLength() - 1;
        while (LAST >= 0 && (*array)[LAST] == NULL) {
            LAST--;
        }
        for (Int32 idx = LAST; idx >= 0; idx--) {
            if ((*array)[idx] == object) {
                const Int32 remain = LAST - idx;
                if (remain > 0) {
                    for (Int32 i = idx; i < idx + remain; ++i) {
                        array->Set(i, (*array)[i + 1]);
                    }
                }
                array->Set(LAST, NULL);
                LAST--;
            }
        }
        if (LAST < 0) {
            map.Erase(it);
        }
        else if (LAST < (array->GetLength() / 2)) {
            AutoPtr< ArrayOf<F*> > newa = NewArray(LAST + 2);
            for (Int32 i = 0; i < LAST + 1; ++i) {
                newa->Set(i, (*array)[i]);
            }
            map[name] = newa;
        }
    }
}

template <typename F, typename R>
AutoPtr< ArrayOf<String> > IntentResolver<F, R>::GetFastIntentCategories(
    /* [in] */ IIntent* intent)
{
    AutoPtr< ArrayOf<String> > categories;
    intent->GetCategories((ArrayOf<String>**)&categories);
    if (categories == NULL) {
        return NULL;
    }
    // return new FastImmutableArraySet<String>(categories.toArray(new String[categories.size()]));
    return categories;
}

template <typename F, typename R>
void IntentResolver<F, R>::BuildResolveList(
    /* [in] */ IIntent* intent,
    /* [in] */ ArrayOf<String>* categories,
    /* [in] */ Boolean debug,
    /* [in] */ Boolean defaultOnly,
    /* [in] */ const String& resolvedType,
    /* [in] */ const String& scheme,
    /* [in] */ ArrayOf<F*>* src,
    /* [in] */ List< AutoPtr<R> >* dest,
    /* [in] */ Int32 userId)
{
    String action;
    intent->GetAction(&action);
    AutoPtr<IUri> data;
    intent->GetData((IUri**)&data);
    String packageName;
    intent->GetPackage(&packageName);

    Boolean excludingStopped;
    intent->IsExcludingStopped(&excludingStopped);

    const Int32 N = src != NULL ? src->GetLength() : 0;
    Boolean hasNonDefaults = FALSE;
    Int32 i;
    AutoPtr<F> filter;
    for (i = 0; i < N && (filter = (*src)[i]) != NULL; i++) {
        Int32 match;
        // if (debug) Slog.v(TAG, "Matching against filter " + filter);

        if (excludingStopped && IsFilterStopped(filter, userId)) {
            // if (debug) {
            //     Slog.v(TAG, "  Filter's target is stopped; skipping");
            // }
            continue;
        }

        // Is delivery being limited to filters owned by a particular package?
        if (!packageName.IsNull() && !packageName.Equals(PackageForFilter(filter))) {
            // if (debug) {
            //     Slog.v(TAG, "  Filter is not from package " + packageName + "; skipping");
            // }
            continue;
        }

        // Do we already have this one?
        if (!AllowFilterResult(filter, dest)) {
            // if (debug) {
            //     Slog.v(TAG, "  Filter's target already added");
            // }
            continue;
        }

        match = filter->Match(action, resolvedType, scheme, data,
                categories, String("IntentResolver"));

        if (match >= 0) {
//                if (debug) Slog.v(TAG, "  Filter matched!  match=0x" +
//                        Integer.toHexString(match));
            if (!defaultOnly || filter->HasCategory(IIntent::CATEGORY_DEFAULT)) {
                AutoPtr<R> oneResult = NewResult(filter, match, userId);
                if (oneResult != NULL) {
                    dest->PushBack(oneResult);
                }
            }
            else {
                hasNonDefaults = TRUE;
            }
        }
        else {
//                if (debug) {
//                    String reason;
//                    switch (match) {
//                        case IntentFilter.NO_MATCH_ACTION: reason = "action"; break;
//                        case IntentFilter.NO_MATCH_CATEGORY: reason = "category"; break;
//                        case IntentFilter.NO_MATCH_DATA: reason = "data"; break;
//                        case IntentFilter.NO_MATCH_TYPE: reason = "type"; break;
//                        default: reason = "unknown reason"; break;
//                    }
//                    Log.v(TAG, "  Filter did not match: " + reason);
//                }
        }
    }

    if (dest->Begin() == dest->End() && hasNonDefaults) {
//        Slog.w(TAG, "resolveIntent failed: found match, but none with Intent.CATEGORY_DEFAULT");
    }
}

} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_INTENTRESOLVER_H__
