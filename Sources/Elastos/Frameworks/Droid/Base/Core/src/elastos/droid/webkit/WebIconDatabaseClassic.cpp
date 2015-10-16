
#include "elastos/droid/os/CMessageHelper.h"
#include "elastos/droid/webkit/WebIconDatabaseClassic.h"

using Elastos::Core::CStringWrapper;
using Elastos::Core::ICharSequence;
using Elastos::IO::CFile;
using Elastos::IO::IFile;
using Elastos::Utility::CHashMap;
using Elastos::Utility::IMap;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IMessageHelper;
using Elastos::Droid::Os::CMessageHelper;
//using Elastos::Droid::Provider::IBrowser;
//using Elastos::Droid::Provider::IBrowserBookmarkColumns;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//       WebIconDatabaseClassic::EventHandler::IconResult
//===============================================================
WebIconDatabaseClassic::EventHandler::IconResult::IconResult(
    /* [in] */ const String& url,
    /* [in] */ IBitmap* icon,
    /* [in] */ IIconListener* l)
   : mUrl(url)
   , mIcon(icon)
   , mListener(l)
{}

CAR_INTERFACE_IMPL_LIGHT(WebIconDatabaseClassic::EventHandler::IconResult, IInterface);

void WebIconDatabaseClassic::EventHandler::IconResult::Dispatch()
{
    mListener->OnReceivedIcon(mUrl, mIcon);
}


//===============================================================
//      WebIconDatabaseClassic::EventHandler::InnerHandler
//===============================================================
WebIconDatabaseClassic::EventHandler::InnerHandler::InnerHandler(
    /* [in] */ EventHandler* owner)
    : mOwner(owner)
{}

ECode WebIconDatabaseClassic::EventHandler::InnerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    // Note: This is the message handler for the WebCore
    // thread.
    Int32 what = 0;
    msg->GetWhat(&what);
    switch (what) {
        case OPEN: {
                AutoPtr<ICharSequence> cs;
                String str;
                msg->GetObj((IInterface**)&cs);
                cs->ToString(&str);
                WebIconDatabaseClassic::NativeOpen(str);
            }
            break;

        case CLOSE:
            WebIconDatabaseClassic::NativeClose();
            break;

        case REMOVE_ALL:
            WebIconDatabaseClassic::NativeRemoveAllIcons();
            break;

        case REQUEST_ICON: {
                AutoPtr<IIconListener> l;
                msg->GetObj((IInterface**)&l);
                AutoPtr<IBundle> data;
                msg->GetData((IBundle**)&data);
                String url;
                data->GetString(String("url"), &url);
                mOwner->RequestIconAndSendResult(url, l);
            }
            break;

        case BULK_REQUEST_ICON:
            mOwner->BulkRequestIcons(msg);
            break;

        case RETAIN_ICON: {
                AutoPtr<ICharSequence> cs;
                String str;
                msg->GetObj((IInterface**)&cs);
                cs->ToString(&str);
                WebIconDatabaseClassic::NativeRetainIconForPageUrl(str);
            }
            break;

        case RELEASE_ICON: {
                AutoPtr<ICharSequence> cs;
                String str;
                msg->GetObj((IInterface**)&cs);
                cs->ToString(&str);
                WebIconDatabaseClassic::NativeReleaseIconForPageUrl(str);
            }
            break;
    }
    return NOERROR;
}


//===============================================================
//           WebIconDatabaseClassic::EventHandler
//===============================================================
// Message ids
const Int32 WebIconDatabaseClassic::EventHandler::OPEN;
const Int32 WebIconDatabaseClassic::EventHandler::CLOSE;
const Int32 WebIconDatabaseClassic::EventHandler::REMOVE_ALL;
const Int32 WebIconDatabaseClassic::EventHandler::REQUEST_ICON;
const Int32 WebIconDatabaseClassic::EventHandler::RETAIN_ICON;
const Int32 WebIconDatabaseClassic::EventHandler::RELEASE_ICON;
const Int32 WebIconDatabaseClassic::EventHandler::BULK_REQUEST_ICON;
const Int32 WebIconDatabaseClassic::EventHandler::ICON_RESULT;

WebIconDatabaseClassic::EventHandler::EventHandler()
{
    mMessages = new Vector< AutoPtr<IMessage> >();
}

ECode WebIconDatabaseClassic::EventHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    // Note: This is the message handler for the UI thread.
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case ICON_RESULT: {
                AutoPtr<IInterface> obj;
                msg->GetObj((IInterface**)&obj);
                IconResult* iconResult = (IconResult*)obj.Get();
                iconResult->Dispatch();
            }
            break;
    }
    return NOERROR;
}

// Called by WebCore thread to create the actual handler
void WebIconDatabaseClassic::EventHandler::CreateHandler()
{
    AutoLock lock(mLock);

    if (mHandler == NULL) {
        mHandler = new InnerHandler(this);
        // Transfer all pending messages
        //for (int size = mMessages.size(); size > 0; size--) {
        //    mHandler.sendMessage(mMessages.remove(0));
        //}
        Vector< AutoPtr<IMessage> >::Iterator it;
        for (it = mMessages->Begin(); it != mMessages->End(); ++it) {
            Boolean result = FALSE;
            mHandler->SendMessage(*it, &result);
        }
        mMessages = NULL;
    }
}

Boolean WebIconDatabaseClassic::EventHandler::HasHandler()
{
    AutoLock lock(mLock);
    return mHandler != NULL;
}

void WebIconDatabaseClassic::EventHandler::PostMessage(
    /* [in] */ IMessage* msg)
{
    AutoLock lock(mLock);
    if (mMessages != NULL) {
        mMessages->PushBack(msg);
    }
    else {
        Boolean result;
        mHandler->SendMessage(msg, &result);
    }
}

void WebIconDatabaseClassic::EventHandler::BulkRequestIcons(
    /* [in] */ IMessage* msg)
{
    AutoPtr<IMap> map;
    msg->GetObj((IInterface**)&map);
    AutoPtr<ICharSequence> key;
    CStringWrapper::New(String("listener"), (ICharSequence**)&key);
    AutoPtr<IIconListener> listener;
    map->Get(key, (IInterface**)&listener);
    key = NULL;
    CStringWrapper::New(String("contentResolver"), (ICharSequence**)&key);
    AutoPtr<IContentResolver> cr;
    map->Get(key, (IInterface**)&cr);
    key = NULL;
    CStringWrapper::New(String("where"), (ICharSequence**)&key);
    AutoPtr<ICharSequence> whereValue;
    map->Get(key, (IInterface**)&whereValue);
    String where;
    whereValue->ToString(&where);

    AutoPtr<ICursor> c;
    assert(0);
    //try {
//        AutoPtr< ArrayOf<String> > strs = ArrayOf<String>::Alloc(1);
//        strs[0] = IBrowserBookmarkColumns::URL;
//        cr->Query(
//                IBrowser::BOOKMARKS_URI,
//                strs,
//                where, NULL, NULL, (ICursor**)&c);
//
//        Boolean bMoveToFirst = FALSE;
//        c->MoveToFirst(&bMoveToFirst);
//        if (bMoveToFirst) {
//            Boolean bMoveToNext = FALSE;
//            do {
//                String url;
//                c->GetString(0, &url);
//                RequestIconAndSendResult(url, listener);
//            } while (c->MoveToNext(&bMoveToNext), bMoveToNext);
//        }
    //} catch (IllegalStateException e) {
    //    Log.e(LOGTAG, "BulkRequestIcons", e);
    //} finally {
    //    if (c != null) c.close();
    //}
}

void WebIconDatabaseClassic::EventHandler::RequestIconAndSendResult(
    /* [in] */ const String& url,
    /* [in] */ IIconListener* listener)
{
    AutoPtr<IBitmap> icon = WebIconDatabaseClassic::NativeIconForPageUrl(url);
    if (icon != NULL) {
        AutoPtr<IInterface> iconResult = new IconResult(url, icon, listener);
        AutoPtr<IMessage> msg;
        ObtainMessage(ICON_RESULT, iconResult, (IMessage**)&msg);
        Boolean result = FALSE;
        SendMessage(msg, &result);
    }
}


//===============================================================
//                   WebIconDatabaseClassic
//===============================================================
const String WebIconDatabaseClassic::LOGTAG("WebIconDatabase");

// Global instance of a WebIconDatabase
AutoPtr<WebIconDatabaseClassic> WebIconDatabaseClassic::sIconDatabase;

/**
 * Private constructor to avoid WebIconDatabaseClassic::anyone else creating an instance.
 */
WebIconDatabaseClassic::WebIconDatabaseClassic()
{
    mEventHandler = new EventHandler();
}

CAR_INTERFACE_IMPL(WebIconDatabaseClassic, IWebIconDatabase)

ECode WebIconDatabaseClassic::Open(
    /* [in] */ const String& path)
{
    if (!path.IsNull()) {
        // Make the directories and parents if they don't exist
        AutoPtr<IFile> db;
        CFile::New(path, (IFile**)&db);
        Boolean result = FALSE;
        if (db->Exists(&result), !result) {
            db->Mkdirs(&result);
        }
        String absolutePath;
        db->GetAbsolutePath(&absolutePath);
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(absolutePath, (ICharSequence**)&cs);
        AutoPtr<IMessageHelper> msghelper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&msghelper);
        AutoPtr<IMessage> msg;
        msghelper->Obtain(NULL, EventHandler::OPEN, cs, (IMessage**)&msg);
        mEventHandler->PostMessage(msg);
    }
    return NOERROR;
}

ECode WebIconDatabaseClassic::Close()
{
    AutoPtr<IMessageHelper> msghelper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&msghelper);
    AutoPtr<IMessage> msg;
    msghelper->Obtain(NULL, EventHandler::CLOSE, (IMessage**)&msg);
    mEventHandler->PostMessage(msg);
    return NOERROR;
}

ECode WebIconDatabaseClassic::RemoveAllIcons()
{
    AutoPtr<IMessageHelper> msghelper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&msghelper);
    AutoPtr<IMessage> msg;
    msghelper->Obtain(NULL, EventHandler::REMOVE_ALL, (IMessage**)&msg);
    mEventHandler->PostMessage(msg);
    return NOERROR;
}

/**
 * Request the Bitmap representing the icon for the given page
 * url. If the icon exists, the listener will be called with the result.
 * @param url The page's url.
 * @param listener An implementation on IconListener to receive the result.
 */
ECode WebIconDatabaseClassic::RequestIconForPageUrl(
    /* [in] */ const String& url,
    /* [in] */ IIconListener* listener)
{
    if (listener == NULL || url.IsNull()) {
        return NOERROR;
    }
    AutoPtr<IMessageHelper> msghelper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&msghelper);
    AutoPtr<IMessage> msg;
    msghelper->Obtain(NULL, EventHandler::REQUEST_ICON, listener, (IMessage**)&msg);
    AutoPtr<IBundle> data;
    msg->GetData((IBundle**)&data);
    data->PutString(String("url"), url);
    mEventHandler->PostMessage(msg);
    return NOERROR;
}

/** {@hide}
 */
ECode WebIconDatabaseClassic::BulkRequestIconForPageUrl(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& where,
    /* [in] */ IIconListener* listener)
{
    if (listener == NULL) {
        return NOERROR;
    }

    // Special case situation: we don't want to add this message to the
    // queue if there is no handler because we may never have a real
    // handler to service the messages and the cursor will never get
    // closed.
    if (mEventHandler->HasHandler()) {
        // Don't use Bundle as it is parcelable.
        AutoPtr<IMap> map;
        CHashMap::New((IMap**)&map);
        AutoPtr<ICharSequence> key;
        CStringWrapper::New(String("contentResolver"), (ICharSequence**)&key);
        AutoPtr<IInterface> oldVale;
        map->Put(key, cr, (IInterface**)&oldVale);
        key = NULL;
        CStringWrapper::New(String("where"), (ICharSequence**)&key);
        AutoPtr<ICharSequence> whereCS;
        CStringWrapper::New(where, (ICharSequence**)&whereCS);
        oldVale = NULL;
        map->Put(key, whereCS, (IInterface**)&oldVale);
        key = NULL;
        CStringWrapper::New(String("listener"), (ICharSequence**)&key);
        oldVale = NULL;
        map->Put(key, listener, (IInterface**)&oldVale);

        AutoPtr<IMessageHelper> msghelper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&msghelper);
        AutoPtr<IMessage> msg;
        msghelper->Obtain(NULL, EventHandler::BULK_REQUEST_ICON, map, (IMessage**)&msg);
        mEventHandler->PostMessage(msg);
    }
    return NOERROR;
}

ECode WebIconDatabaseClassic::RetainIconForPageUrl(
    /* [in] */ const String& url)
{
    if (!url.IsNull()) {
        AutoPtr<ICharSequence> urlCS;
        CStringWrapper::New(url, (ICharSequence**)&urlCS);
        AutoPtr<IMessageHelper> msghelper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&msghelper);
        AutoPtr<IMessage> msg;
        msghelper->Obtain(NULL, EventHandler::RETAIN_ICON, urlCS, (IMessage**)&msg);
        mEventHandler->PostMessage(msg);
    }
    return NOERROR;
}

ECode WebIconDatabaseClassic::ReleaseIconForPageUrl(
    /* [in] */ const String& url)
{
    if (!url.IsNull()) {
        AutoPtr<ICharSequence> urlCS;
        CStringWrapper::New(url, (ICharSequence**)&urlCS);
        AutoPtr<IMessageHelper> msghelper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&msghelper);
        AutoPtr<IMessage> msg;
        msghelper->Obtain(NULL, EventHandler::RELEASE_ICON, urlCS, (IMessage**)&msg);
        mEventHandler->PostMessage(msg);
    }
    return NOERROR;
}

/**
 * Get the global instance of WebIconDatabase.
 * @return A single instance of WebIconDatabase. It will be the same
 *         instance for the current process each time this method is
 *         called.
 */
AutoPtr<WebIconDatabaseClassic> WebIconDatabaseClassic::GetInstance()
{
    // XXX: Must be created in the UI thread.
    if (sIconDatabase == NULL) {
        sIconDatabase = new WebIconDatabaseClassic();
    }
    return sIconDatabase;
}

/**
 * Create the internal handler and transfer all pending messages.
 * XXX: Called by WebCore thread only!
 */
/*package*/
void WebIconDatabaseClassic::CreateHandler()
{
    mEventHandler->CreateHandler();
}

// Native functions
void WebIconDatabaseClassic::NativeOpen(
    /* [in] */ const String& path)
{
    Elastos_WebIconDatabase_nativeOpen(path);
}

void WebIconDatabaseClassic::NativeClose()
{
    Elastos_WebIconDatabase_nativeClose();
}

void WebIconDatabaseClassic::NativeRemoveAllIcons()
{
    Elastos_WebIconDatabase_nativeRemoveAllIcons();
}

AutoPtr<IBitmap> WebIconDatabaseClassic::NativeIconForPageUrl(
    /* [in] */ const String& url)
{
    return IBitmap::Probe(Elastos_WebIconDatabase_nativeIconForPageUrl(url));
}

void WebIconDatabaseClassic::NativeRetainIconForPageUrl(
    /* [in] */ const String& url)
{
    Elastos_WebIconDatabase_nativeRetainIconForPageUrl(url);
}

void WebIconDatabaseClassic::NativeReleaseIconForPageUrl(
    /* [in] */ const String& url)
{
    Elastos_WebIconDatabase_nativeReleaseIconForPageUrl(url);
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
