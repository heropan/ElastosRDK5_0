
#ifndef __CBROWSER_H__
#define __CBROWSER_H__

#include "_CBrowser.h"

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CBrowser)
{
public:
    CARAPI GetBOOKMARKSURI(
        /* [out] */ IUri** uri);

    CARAPI GetSEARCHESURI(
        /* [out] */ IUri** uri);

    /**
     *  Open an activity to save a bookmark. Launch with a title
     *  and/or a url, both of which can be edited by the user before saving.
     *
     *  @param c        Context used to launch the activity to add a bookmark.
     *  @param title    Title for the bookmark. Can be null or empty string.
     *  @param url      Url for the bookmark. Can be null or empty string.
     */
    CARAPI SaveBookmark(
        /* [in] */ IContext* c,
        /* [in] */ const String& title,
        /* [in] */ const String& url);

    /**
     * Sends the given string using an Intent with {@link Intent#ACTION_SEND} and a mime type
     * of text/plain. The string is put into {@link Intent#EXTRA_TEXT}.
     *
     * @param context the context used to start the activity
     * @param string the string to send
     */
    CARAPI SendString(
        /* [in] */ IContext* context,
        /* [in] */ const String& string);

    /**
     *  Find an application to handle the given string and, if found, invoke
     *  it with the given string as a parameter.
     *  @param c Context used to launch the new activity.
     *  @param stringToSend The string to be handled.
     *  @param chooserDialogTitle The title of the dialog that allows the user
     *  to select between multiple applications that are all capable of handling
     *  the string.
     *  @hide pending API council approval
     */
    CARAPI SendString(
        /* [in] */ IContext* c,
        /* [in] */ const String& stringToSend,
        /* [in] */ const String& chooserDialogTitle);

    /**
     *  Return a cursor pointing to a list of all the bookmarks. The cursor will have a single
     *  column, {@link BookmarkColumns#URL}.
     *  <p>
     *  Requires {@link android.Manifest.permission#READ_HISTORY_BOOKMARKS}
     *
     *  @param cr   The ContentResolver used to access the database.
     */
    CARAPI GetAllBookmarks(
        /* [in] */ IContentResolver* cr,
        /* [out] */ ICursor** bookmarks);

    /**
     *  Return a cursor pointing to a list of all visited site urls. The cursor will
     *  have a single column, {@link BookmarkColumns#URL}.
     *  <p>
     *  Requires {@link android.Manifest.permission#READ_HISTORY_BOOKMARKS}
     *
     *  @param cr   The ContentResolver used to access the database.
     */
    CARAPI GetAllVisitedUrls(
        /* [in] */ IContentResolver* cr,
        /* [out] */ ICursor** urls);

    /**
     *  Update the visited history to acknowledge that a site has been
     *  visited.
     *  Requires {@link android.Manifest.permission#READ_HISTORY_BOOKMARKS}
     *  Requires {@link android.Manifest.permission#WRITE_HISTORY_BOOKMARKS}
     *  @param cr   The ContentResolver used to access the database.
     *  @param url  The site being visited.
     *  @param real If true, this is an actual visit, and should add to the
     *              number of visits.  If false, the user entered it manually.
     */
    CARAPI UpdateVisitedHistory(
        /* [in] */ IContentResolver* cr,
        /* [in] */ const String& url,
        /* [in] */ Boolean real);

    /**
     *  Returns all the URLs in the history.
     *  Requires {@link android.Manifest.permission#READ_HISTORY_BOOKMARKS}
     *  @param cr   The ContentResolver used to access the database.
     *  @hide pending API council approval
     */
    CARAPI GetVisitedHistory(
        /* [in] */ IContentResolver* cr,
        /* [out] */ ArrayOf<String>* urls);

    /**
     * If there are more than MAX_HISTORY_COUNT non-bookmark history
     * items in the bookmark/history table, delete TRUNCATE_N_OLDEST
     * of them.  This is used to keep our history table to a
     * reasonable size.  Note: it does not prune bookmarks.  If the
     * user wants 1000 bookmarks, the user gets 1000 bookmarks.
     *  Requires {@link android.Manifest.permission#READ_HISTORY_BOOKMARKS}
     *  Requires {@link android.Manifest.permission#WRITE_HISTORY_BOOKMARKS}
     *
     * @param cr The ContentResolver used to access the database.
     */
    CARAPI TruncateHistory(
        /* [in] */ IContentResolver* cr);

    /**
     * Returns whether there is any history to clear.
     *  Requires {@link android.Manifest.permission#READ_HISTORY_BOOKMARKS}
     * @param cr   The ContentResolver used to access the database.
     * @return boolean  True if the history can be cleared.
     */
    CARAPI CanClearHistory(
        /* [in] */ IContentResolver* cr,
        /* [out] */ Boolean* res);

    /**
     *  Delete all entries from the bookmarks/history table which are
     *  not bookmarks.  Also set all visited bookmarks to unvisited.
     *  Requires {@link android.Manifest.permission#WRITE_HISTORY_BOOKMARKS}
     *  @param cr   The ContentResolver used to access the database.
     */
    CARAPI ClearHistory(
        /* [in] */ IContentResolver* cr);

    /**
     * Delete all history items from begin to end.
     *  Requires {@link android.Manifest.permission#WRITE_HISTORY_BOOKMARKS}
     * @param cr    The ContentResolver used to access the database.
     * @param begin First date to remove.  If -1, all dates before end.
     *              Inclusive.
     * @param end   Last date to remove. If -1, all dates after begin.
     *              Non-inclusive.
     */
    CARAPI DeleteHistoryTimeFrame(
        /* [in] */ IContentResolver* cr,
        /* [in] */ Int64 begin,
        /* [in] */ Int64 end);

    /**
     * Remove a specific url from the history database.
     *  Requires {@link android.Manifest.permission#WRITE_HISTORY_BOOKMARKS}
     * @param cr    The ContentResolver used to access the database.
     * @param url   url to remove.
     */
    CARAPI DeleteFromHistory(
        /* [in] */ IContentResolver* cr,
        /* [in] */ const String& url);

    /**
     * Add a search string to the searches database.
     *  Requires {@link android.Manifest.permission#READ_HISTORY_BOOKMARKS}
     *  Requires {@link android.Manifest.permission#WRITE_HISTORY_BOOKMARKS}
     * @param cr   The ContentResolver used to access the database.
     * @param search    The string to add to the searches database.
     */
    CARAPI AddSearchUrl(
        /* [in] */ IContentResolver* cr,
        /* [in] */ const String& search);

    /**
     * Remove all searches from the search database.
     *  Requires {@link android.Manifest.permission#WRITE_HISTORY_BOOKMARKS}
     * @param cr   The ContentResolver used to access the database.
     */
    CARAPI ClearSearches(
        /* [in] */ IContentResolver* cr);

    /**
     *  Request all icons from the database.  This call must either be called
     *  in the main thread or have had Looper.prepare() invoked in the calling
     *  thread.
     *  Requires {@link android.Manifest.permission#READ_HISTORY_BOOKMARKS}
     *  @param  cr The ContentResolver used to access the database.
     *  @param  where Clause to be used to limit the query from the database.
     *          Must be an allowable string to be passed into a database query.
     *  @param  listener IconListener that gets the icons once they are
     *          retrieved.
     */
    CARAPI RequestAllIcons(
        /* [in] */ IContentResolver* cr,
        /* [in] */ const String& where,
        /* [in] */ IWebIconDatabaseIconListener* listener);

private:
    CARAPI_(void) AddOrUrlEquals(
        /* [in] */ StringBuffer* sb);

    CARAPI_(AutoPtr<ICursor>) GetVisitedLike(
        /* [in] */ IContentResolver* cr,
        /* [in] */ const String& url);

    /**
     * Helper function to delete all history items and release the icons for them in the
     * {@link WebIconDatabase}.
     *
     * Requires {@link android.Manifest.permission#READ_HISTORY_BOOKMARKS}
     * Requires {@link android.Manifest.permission#WRITE_HISTORY_BOOKMARKS}
     *
     * @param cr   The ContentResolver used to access the database.
     * @param whereClause   String to limit the items affected.
     *                      null means all items.
     */
    CARAPI_(void) DeleteHistoryWhere(
        /* [in] */ IContentResolver* cr,
        /* [in] */ const String& whereClause);

public:
    /**
     * A table containing both bookmarks and history items. The columns of the table are defined in
     * {@link BookmarkColumns}. Reading this table requires the
     * {@link android.Manifest.permission#READ_HISTORY_BOOKMARKS} permission and writing to it
     * requires the {@link android.Manifest.permission#WRITE_HISTORY_BOOKMARKS} permission.
     */
    static const AutoPtr<IUri> BOOKMARKS_URI;

    /**
     * A table containing a log of browser searches. The columns of the table are defined in
     * {@link SearchColumns}. Reading this table requires the
     * {@link android.Manifest.permission#READ_HISTORY_BOOKMARKS} permission and writing to it
     * requires the {@link android.Manifest.permission#WRITE_HISTORY_BOOKMARKS} permission.
     */
    static const AutoPtr<IUri> SEARCHES_URI;

private:
    static const CString LOGTAG;

    /* Set a cap on the count of history items in the history/bookmark
    table, to prevent db and layout operations from dragging to a
    crawl.  Revisit this cap when/if db/layout performance
    improvements are made.  Note: this does not affect bookmark
    entries -- if the user wants more bookmarks than the cap, they
    get them. */
    static const Int32 MAX_HISTORY_COUNT = 250;
};

}
}
}

#endif //__CBROWSER_H__