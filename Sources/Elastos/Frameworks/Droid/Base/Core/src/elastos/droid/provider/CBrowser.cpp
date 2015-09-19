
#include "provider/CBrowser.h"


const CString CBrowser::LOGTAG = "browser";

ECode CBrowser::GetBOOKMARKSURI(
    /* [out] */ IUri** uri)
{
//    *uri = Uri.parse("content://browser/bookmarks");
}

ECode CBrowser::GetSEARCHESURI(
    /* [out] */ IUri** uri)
{
//    *uri = Uri.parse("content://browser/searches");
}

ECode CBrowser::SaveBookmark(
    /* [in] */ IContext* c,
    /* [in] */ const String& title,
    /* [in] */ const String& url)
{
//     Intent i = new Intent(Intent.ACTION_INSERT, Browser.BOOKMARKS_URI);
//     i.putExtra("title", title);
//     i.putExtra("url", url);
//     c.startActivity(i);
}

ECode CBrowser::SendString(
    /* [in] */ IContext* context,
    /* [in] */ const String& string)
{
//    sendString(context, string, context.getString(com.android.internal.R.string.sendText));
}

ECode CBrowser::SendString(
    /* [in] */ IContext* c,
    /* [in] */ const String& stringToSend,
    /* [in] */ const String& chooserDialogTitle)
{
//     Intent send = new Intent(Intent.ACTION_SEND);
//     send.setType("text/plain");
//     send.putExtra(Intent.EXTRA_TEXT, stringToSend);

//     try {
//         Intent i = Intent.createChooser(send, chooserDialogTitle);
//         // In case this is called from outside an Activity
//         i.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//         c.startActivity(i);
//     } catch(android.content.ActivityNotFoundException ex) {
//         // if no app handles it, do nothing
//     }
}

ECode CBrowser::GetAllBookmarks(
    /* [in] */ IContentResolver* cr,
    /* [out] */ ICursor** bookmarks)
{
//     return cr.query(Bookmarks.CONTENT_URI,
//             new String[] { Bookmarks.URL },
//             Bookmarks.IS_FOLDER + " = 0", null, null);
}

ECode CBrowser::GetAllVisitedUrls(
    /* [in] */ IContentResolver* cr,
    /* [out] */ ICursor** urls)
{
//     return cr.query(Combined.CONTENT_URI,
//             new String[] { Combined.URL }, null, null,
//             Combined.DATE_CREATED + " ASC");
}

void CBrowser::AddOrUrlEquals(
    /* [in] */ StringBuffer* sb)
{
//    sb.append(" OR " + BookmarkColumns.URL + " = ");
}

AutoPtr<ICursor> CBrowser::GetVisitedLike(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& url)
{
//     boolean secure = false;
//     String compareString = url;
//     if (compareString.startsWith("http://")) {
//         compareString = compareString.substring(7);
//     } else if (compareString.startsWith("https://")) {
//         compareString = compareString.substring(8);
//         secure = true;
//     }
//     if (compareString.startsWith("www.")) {
//         compareString = compareString.substring(4);
//     }
//     StringBuilder whereClause = null;
//     if (secure) {
//         whereClause = new StringBuilder(Bookmarks.URL + " = ");
//         DatabaseUtils.appendEscapedSQLString(whereClause,
//                 "https://" + compareString);
//         addOrUrlEquals(whereClause);
//         DatabaseUtils.appendEscapedSQLString(whereClause,
//                 "https://www." + compareString);
//     } else {
//         whereClause = new StringBuilder(Bookmarks.URL + " = ");
//         DatabaseUtils.appendEscapedSQLString(whereClause,
//                 compareString);
//         addOrUrlEquals(whereClause);
//         String wwwString = "www." + compareString;
//         DatabaseUtils.appendEscapedSQLString(whereClause,
//                 wwwString);
//         addOrUrlEquals(whereClause);
//         DatabaseUtils.appendEscapedSQLString(whereClause,
//                 "http://" + compareString);
//         addOrUrlEquals(whereClause);
//         DatabaseUtils.appendEscapedSQLString(whereClause,
//                 "http://" + wwwString);
//     }
//     return cr.query(History.CONTENT_URI, new String[] { History._ID, History.VISITS },
//             whereClause.toString(), null, null);
}

ECode CBrowser::UpdateVisitedHistory(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& url,
    /* [in] */ Boolean real)
{
//     long now = System.currentTimeMillis();
//     Cursor c = null;
//     try {
//         c = getVisitedLike(cr, url);
//         /* We should only get one answer that is exactly the same. */
//         if (c.moveToFirst()) {
//             ContentValues values = new ContentValues();
//             if (real) {
//                 values.put(History.VISITS, c.getInt(1) + 1);
//             } else {
//                 values.put(History.USER_ENTERED, 1);
//             }
//             values.put(History.DATE_LAST_VISITED, now);
//             cr.update(ContentUris.withAppendedId(History.CONTENT_URI, c.getLong(0)),
//                     values, null, null);
//         } else {
//             truncateHistory(cr);
//             ContentValues values = new ContentValues();
//             int visits;
//             int user_entered;
//             if (real) {
//                 visits = 1;
//                 user_entered = 0;
//             } else {
//                 visits = 0;
//                 user_entered = 1;
//             }
//             values.put(History.URL, url);
//             values.put(History.VISITS, visits);
//             values.put(History.DATE_LAST_VISITED, now);
//             values.put(History.TITLE, url);
//             values.put(History.DATE_CREATED, 0);
//             values.put(History.USER_ENTERED, user_entered);
//             cr.insert(History.CONTENT_URI, values);
//         }
//     } catch (IllegalStateException e) {
//         Log.e(LOGTAG, "updateVisitedHistory", e);
//     } finally {
//         if (c != null) c.close();
//     }
}

ECode CBrowser::GetVisitedHistory(
     /* [in] */ IContentResolver* cr,
     /* [out] */ ArrayOf<String>* urls)
{
//     Cursor c = null;
//     String[] str = null;
//     try {
//         String[] projection = new String[] {
//                 History.URL,
//         };
//         c = cr.query(History.CONTENT_URI, projection, History.VISITS + " > 0", null, null);
//         if (c == null) return new String[0];
//         str = new String[c.getCount()];
//         int i = 0;
//         while (c.moveToNext()) {
//             str[i] = c.getString(0);
//             i++;
//         }
//     } catch (IllegalStateException e) {
//         Log.e(LOGTAG, "getVisitedHistory", e);
//         str = new String[0];
//     } finally {
//         if (c != null) c.close();
//     }
//     return str;
}

ECode CBrowser::TruncateHistory(
    /* [in] */ IContentResolver* cr)
{
//     // TODO make a single request to the provider to do this in a single transaction
//     Cursor cursor = null;
//     try {

//         // Select non-bookmark history, ordered by date
//         cursor = cr.query(History.CONTENT_URI,
//                 new String[] { History._ID, History.URL, History.DATE_LAST_VISITED },
//                 null, null, History.DATE_LAST_VISITED + " ASC");

//         if (cursor.moveToFirst() && cursor.getCount() >= MAX_HISTORY_COUNT) {
//             final WebIconDatabase iconDb = WebIconDatabase.getInstance();
//             /* eliminate oldest history items */
//             for (int i = 0; i < TRUNCATE_N_OLDEST; i++) {
//                 cr.delete(ContentUris.withAppendedId(History.CONTENT_URI, cursor.getLong(0)),
//                         null, null);
//                 iconDb.releaseIconForPageUrl(cursor.getString(1));
//                 if (!cursor.moveToNext()) break;
//             }
//         }
//     } catch (IllegalStateException e) {
//         Log.e(LOGTAG, "truncateHistory", e);
//     } finally {
//         if (cursor != null) cursor.close();
//     }
}

ECode CBrowser::CanClearHistory(
    /* [in] */ IContentResolver* cr,
    /* [out] */ Boolean* res)
{
//     Cursor cursor = null;
//     boolean ret = false;
//     try {
//         cursor = cr.query(History.CONTENT_URI,
//             new String [] { History._ID, History.VISITS },
//             null, null, null);
//         ret = cursor.getCount() > 0;
//     } catch (IllegalStateException e) {
//         Log.e(LOGTAG, "canClearHistory", e);
//     } finally {
//         if (cursor != null) cursor.close();
//     }
//     return ret;
}

ECode CBrowser::ClearHistory(
    /* [in] */ IContentResolver* cr)
{
    deleteHistoryWhere(cr, null);
}

void CBrowser::DeleteHistoryWhere(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& whereClause)
{
//     Cursor cursor = null;
//     try {
//         cursor = cr.query(History.CONTENT_URI, new String[] { History.URL }, whereClause,
//                 null, null);
//         if (cursor.moveToFirst()) {
//             final WebIconDatabase iconDb = WebIconDatabase.getInstance();
//             do {
//                 // Delete favicons
//                 // TODO don't release if the URL is bookmarked
//                 iconDb.releaseIconForPageUrl(cursor.getString(0));
//             } while (cursor.moveToNext());

//             cr.delete(History.CONTENT_URI, whereClause, null);
//         }
//     } catch (IllegalStateException e) {
//         Log.e(LOGTAG, "deleteHistoryWhere", e);
//         return;
//     } finally {
//         if (cursor != null) cursor.close();
//     }
}

ECode CBrowser::DeleteHistoryTimeFrame(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Int64 begin,
    /* [in] */ Int64 end)
{
//     String whereClause;
//     String date = BookmarkColumns.DATE;
//     if (-1 == begin) {
//         if (-1 == end) {
//             clearHistory(cr);
//             return;
//         }
//         whereClause = date + " < " + Long.toString(end);
//     } else if (-1 == end) {
//         whereClause = date + " >= " + Long.toString(begin);
//     } else {
//         whereClause = date + " >= " + Long.toString(begin) + " AND " + date
//                 + " < " + Long.toString(end);
//     }
//     deleteHistoryWhere(cr, whereClause);
}

ECode CBrowser::DeleteFromHistory(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& url)
{
//    cr.delete(History.CONTENT_URI, History.URL + "=?", new String[] { url });
}

ECode CBrowser::AddSearchUrl(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& search)
{
//     // The content provider will take care of updating existing searches instead of duplicating
//     ContentValues values = new ContentValues();
//     values.put(Searches.SEARCH, search);
//     values.put(Searches.DATE, System.currentTimeMillis());
//     cr.insert(Searches.CONTENT_URI, values);
}

ECode CBrowser::ClearSearches(
    /* [in] */ IContentResolver* cr)
{
//     // FIXME: Should this clear the urls to which these searches lead?
//     // (i.e. remove google.com/query= blah blah blah)
//     try {
//         cr.delete(Searches.CONTENT_URI, null, null);
//     } catch (IllegalStateException e) {
//         Log.e(LOGTAG, "clearSearches", e);
//     }
}

ECode CBrowser::RequestAllIcons(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& where,
    /* [in] */ IWebIconDatabaseIconListener* listener)
{
//    WebIconDatabase.getInstance().bulkRequestIconForPageUrl(cr, where, listener);
}