
#ifndef __ELASTOS_DROID_CONTENT_CCONTENTQUERYMAP_H__
#define __ELASTOS_DROID_CONTENT_CCONTENTQUERYMAP_H__

#include "_CContentQueryMap.h"
#include <ext/frameworkext.h>
#include <elastos/utility/etl/HashMap.h>
#include "database/ContentObserver.h"

using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Database::IContentObserver;
using Elastos::Droid::Database::ContentObserver;
using Elastos::Droid::Os::IHandler;
using Elastos::Utility::IObjectStringMap;
using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::IObserver;
using Elastos::Utility::IList;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CContentQueryMap)
{
public:
    CContentQueryMap();

    ~CContentQueryMap();

    /**
     * Change whether or not the ContentQueryMap will register with the cursor's ContentProvider
     * for change notifications. If you use a ContentQueryMap in an activity you should call this
     * with false in onPause(), which means you need to call it with true in onResume()
     * if want it to be kept updated.
     * @param keepUpdated if true the ContentQueryMap should be registered with the cursor's
     * ContentProvider, false otherwise
     */
    CARAPI SetKeepUpdated(
        /* [in] */ Boolean keepUpdated);

    /**
     * Access the ContentValues for the row specified by rowName
     * @param rowName which row to read
     * @return the ContentValues for the row, or null if the row wasn't present in the cursor
     */
    CARAPI GetValues(
        /* [in] */ const String& rowName,
        /* [out] */ IContentValues** contentValues);

    /** Requeries the cursor and reads the contents into the cache */
    CARAPI Requery();

    CARAPI GetRows(
        /* [out] */ IObjectStringMap** rows);

    CARAPI Close();

    CARAPI constructor(
        /* [in] */ ICursor* cursor,
        /* [in] */ const String& columnNameOfKey,
        /* [in] */ Boolean keepUpdated,
        /* [in] */ IHandler* handlerForUpdateNotifications);

    class KeepUpdatedContentObserver
        : public ContentObserver
    {
    public:
        KeepUpdatedContentObserver(
            /* [in] */ IHandler* handler,
            /* [in] */ IWeakReference* host);

        // @Override
        CARAPI OnChange(
            /* [in] */ Boolean selfChange);

    private:
        AutoPtr<IWeakReference> mHost;
    };

protected:
    CARAPI Finalize();

private:
    CARAPI ReadCursorIntoCache(
        /* [in] */ ICursor* cursor);

private:
    friend class KeepUpdatedContentObserver;

    /* volatile */ AutoPtr<ICursor> mCursor;
    AutoPtr<ArrayOf<String> > mColumnNames;
    Int32 mKeyColumn;
    AutoPtr<IHandler> mHandlerForUpdateNotifications;
    Boolean mKeepUpdated;
    HashMap<String, AutoPtr<IContentValues> > mValues;
    AutoPtr<IContentObserver> mContentObserver;

    /** Set when a cursor change notification is received and is cleared on a call to requery(). */
    Boolean mDirty;
    Object mMethodLock;

public:
    //========================================================
    // CObservable
    //========================================================
    CARAPI InitObservable();

    /**
     * Adds the specified observer to the list of observers. If it is already
     * registered, it is not added a second time.
     *
     * @param observer
     *            the Observer to add.
     */
    CARAPI AddObserver(
        /* [in] */ IObserver* observer);

    /**
     * Returns the number of observers registered to this {@code Observable}.
     *
     * @return the number of observers.
     */
    CARAPI CountObservers(
        /* [out] */ Int32* value);

    /**
     * Removes the specified observer from the list of observers. Passing null
     * won't do anything.
     *
     * @param observer
     *            the observer to remove.
     */
    CARAPI DeleteObserver(
        /* [in] */ IObserver* observer);

    /**
     * Removes all observers from the list of observers.
     */
    CARAPI DeleteObservers();

    /**
     * Returns the changed flag for this {@code Observable}.
     *
     * @return {@code true} when the changed flag for this {@code Observable} is
     *         set, {@code false} otherwise.
     */
    CARAPI HasChanged(
        /* [out] */ Boolean* value);

    /**
     * If {@code hasChanged()} returns {@code true}, calls the {@code update()}
     * method for every observer in the list of observers using null as the
     * argument. Afterwards, calls {@code clearChanged()}.
     * <p>
     * Equivalent to calling {@code notifyObservers(null)}.
     */
    CARAPI NotifyObservers();

    /**
     * If {@code hasChanged()} returns {@code true}, calls the {@code update()}
     * method for every Observer in the list of observers using the specified
     * argument. Afterwards calls {@code clearChanged()}.
     *
     * @param data
     *            the argument passed to {@code update()}.
     */
    CARAPI NotifyObservers(
        /* [in] */ IInterface* data);

    /**
     * Clears the changed flag for this {@code Observable}. After calling
     * {@code clearChanged()}, {@code hasChanged()} will return {@code false}.
     */
    CARAPI ClearChanged();

    /**
     * Sets the changed flag for this {@code Observable}. After calling
     * {@code setChanged()}, {@code hasChanged()} will return {@code true}.
     */
    CARAPI SetChanged();

protected:

    AutoPtr<IList> mObservers; // = new ArrayList<Observer>();

    Boolean mChanged; // = false;

private:
    Object mLock;
};

}
}
}

#endif // __ELASTOS_DROID_CONTENT_CCONTENTQUERYMAP_H__
