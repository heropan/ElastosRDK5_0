
#include "elastos/droid/view/textservice/SpellCheckerSession.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Textservice {

//========================================================================================
//              SpellCheckerSession::
//========================================================================================
CAR_INTERFACE_IMPL_2(SpellCheckerSession, Object, ISpellCheckerSession)

const String SpellCheckerSession::TAG;// = SpellCheckerSession.class.getSimpleName();
const Boolean SpellCheckerSession::DBG = FALSE;
const String SpellCheckerSession::SERVICE_META_DATA("android.view.textservice.scs");
const Int32 SpellCheckerSession::MSG_ON_GET_SUGGESTION_MULTIPLE = 1;
const Int32 SpellCheckerSession::MSG_ON_GET_SUGGESTION_MULTIPLE_FOR_SENTENCE = 2;

const Int32 SpellCheckerSession::SpellCheckerSessionListenerImpl::TASK_CANCEL = 1;
const Int32 SpellCheckerSession::SpellCheckerSessionListenerImpl::TASK_GET_SUGGESTIONS_MULTIPLE = 2;
const Int32 SpellCheckerSession::SpellCheckerSessionListenerImpl::TASK_CLOSE = 3;
const Int32 SpellCheckerSession::SpellCheckerSessionListenerImpl::TASK_GET_SUGGESTIONS_MULTIPLE_FOR_SENTENCE = 4;

SpellCheckerSession::SpellCheckerSession(
    /* [in] */ ISpellCheckerInfo* info,
    /* [in] */ ITextServicesManager* tsm,
    /* [in] */ ISpellCheckerSessionListener* listener,
    /* [in] */ ISpellCheckerSubtype* subtype)
{
    Init(info, tsm, listener, subtype);
}

void SpellCheckerSession::Init(
    /* [in] */ ISpellCheckerInfo* info,
    /* [in] */ ITextServicesManager* tsm,
    /* [in] */ ISpellCheckerSessionListener* listener,
    /* [in] */ ISpellCheckerSubtype* subtype)
{
    assert(info != NULL && listener != NULL && tsm != NULL);

    mSpellCheckerInfo = info;
    mSpellCheckerSessionListenerImpl = new SpellCheckerSessionListenerImpl(mHandler);
    mInternalListener = new InternalListener(mSpellCheckerSessionListenerImpl);
    mTextServicesManager = tsm;
    mIsUsed = TRUE;
    mSpellCheckerSessionListener = listener;
    mSubtype = subtype;
}

Boolean SpellCheckerSession::IsSessionDisconnected()
{
    return mSpellCheckerSessionListenerImpl->IsDisconnected();
}

AutoPtr<ISpellCheckerInfo> SpellCheckerSession::GetSpellChecker()
{
    return mSpellCheckerSessionListenerImpl;
}

void SpellCheckerSession::Cancel()
{
    mSpellCheckerSessionListenerImpl->Cancel();
}

void SpellCheckerSession::Close()
{
    mIsUsed = FALSE;
    //try {
        mSpellCheckerSessionListenerImpl->Close();
        mTextServicesManager->FinishSpellCheckerService(mSpellCheckerSessionListenerImpl);
    //} catch (RemoteException e) {
        // do nothing
    //}
}

void SpellCheckerSession::GetSentenceSuggestions(
    /* [in] */ ArrayOf<ITextInfo*>* textInfos,
    /* [in] */ Int32 suggestionsLimit)
{
    mSpellCheckerSessionListenerImpl->GetSentenceSuggestionsMultiple(
                textInfos, suggestionsLimit);
}

void SpellCheckerSession::GetSuggestions(
    /* [in] */ ITextInfo* textInfo,
    /* [in] */ Int32 suggestionsLimit)
{
    AutoPtr<ArrayOf<ITextInfo*> > textInfoArray = ArrayOf<ITextInfo*>::Alloc(1);
    textInfoArray[0] = textInfo;
    GetSuggestions(textInfoArray, suggestionsLimit, FALSE);
}

void SpellCheckerSession::GetSuggestions(
    /* [in] */ ArrayOf<ITextInfo*>* textInfos,
    /* [in] */ Int32 suggestionsLimit,
    /* [in] */ Boolean sequentialWords)
{
//    if (DBG) {
//        Log.w(TAG, "getSuggestions from " + mSpellCheckerInfo.getId());
//    }
    mSpellCheckerSessionListenerImpl->GetSuggestionsMultiple(
            textInfos, suggestionsLimit, sequentialWords);
}

AutoPtr<ITextServicesSessionListener> SpellCheckerSession::GetTextServicesSessionListener()
{
    return mInternalListener;
}

AutoPtr<ISpellCheckerSessionListener> SpellCheckerSession::GetSpellCheckerSessionListener()
{
    return mSpellCheckerSessionListenerImpl;
}

SpellCheckerSession::SpellCheckerSessionListenerImpl::SpellCheckerSessionListenerImpl(
    /* [in] */ Handler handler)
{
    mOpened = FALSE;
    mHandler = handler;
}

void SpellCheckerSession::SpellCheckerSessionListenerImpl::OnServiceConnected(
    /* [in] */ ISpellCheckerSession* session)
{
    Object::AutoLock lock(mLock);
//    if (DBG) {
//        Log.w(TAG, "SpellCheckerSession connected.");
//    }
    mParentSpellCheckerSessionListenerImpl->OnServiceConnected(session);
}

void SpellCheckerSession::SpellCheckerSessionListenerImpl::Cancel()
{
//    if (DBG) {
//        Log.w(TAG, "cancel");
//    }
    AutoPtr<SpellCheckerParams> params = new SpellCheckerParams(TASK_CANCEL, NULL, 0, FALSE);
    ProcessOrEnqueueTask(params);
}

void SpellCheckerSession::SpellCheckerSessionListenerImpl::GetSuggestionsMultiple(
    /* [in] */ ArrayOf<ITextInfo*>* textInfos,
    /* [in] */ Int32 suggestionsLimit,
    /* [in] */ Boolean sequentialWords)
{
//  if (DBG) {
//      Log.w(TAG, "getSuggestionsMultiple");
//  }
    AutoPtr<SpellCheckerParams> params = new SpellCheckerParams(TASK_GET_SUGGESTIONS_MULTIPLE, textInfos,
            suggestionsLimit, sequentialWords);
    ProcessOrEnqueueTask(params);
}

void SpellCheckerSession::SpellCheckerSessionListenerImpl::GetSentenceSuggestionsMultiple(
    /* [in] */ ArrayOf<ITextInfo*>* textInfos,
    /* [in] */ Int32 suggestionsLimit)
{
//    if (DBG) {
//        Log.w(TAG, "getSentenceSuggestionsMultiple");
//    }
    AutoPtr<SpellCheckerParams> params = new SpellCheckerParams(TASK_GET_SUGGESTIONS_MULTIPLE_FOR_SENTENCE,
            textInfos, suggestionsLimit, FALSE);
    ProcessOrEnqueueTask(params);
}

void SpellCheckerSession::SpellCheckerSessionListenerImpl::Close()
{
//    if (DBG) {
//        Log.w(TAG, "close");
//    }

    ProcessOrEnqueueTask(new SpellCheckerParams(TASK_CLOSE, NULL, 0, FALSE));
}

Boolean SpellCheckerSession::SpellCheckerSessionListenerImpl::IsDisconnected()
{
    return mOpened && mISpellCheckerSession == NULL;
}

//@Override
void SpellCheckerSession::SpellCheckerSessionListenerImpl::OnGetSuggestions(
    /* [in] */ ArrayOf<ISuggestionsInfo*>* results)
{
    //synchronized(this)
    {
        Object::AutoLock lock(mLock);
//        if (mHandler != null) {
//            mHandler.sendMessage(Message.obtain(mHandler,
//                    MSG_ON_GET_SUGGESTION_MULTIPLE, results));
//        }
    }
}

void SpellCheckerSession::SpellCheckerSessionListenerImpl::OnGetSentenceSuggestions(
    /* [in] */ ArrayOf<ISentenceSuggestionsInfo*>* results)
{
//    mHandler.sendMessage(
//        Message.obtain(mHandler, MSG_ON_GET_SUGGESTION_MULTIPLE_FOR_SENTENCE, results));
}

void SpellCheckerSession::SpellCheckerSessionListenerImpl::ProcessTask(
    /* [in] */ ISpellCheckerSession* session,
    /* [in] */ SpellCheckerParams* scp,
    /* [in] */ Boolean async)
{/*
    if (async || mAsyncHandler == null) {
        switch (scp.mWhat) {
            case TASK_CANCEL:
                if (DBG) {
                    Log.w(TAG, "Cancel spell checker tasks.");
                }
                try {
                    session.onCancel();
                } catch (RemoteException e) {
                    Log.e(TAG, "Failed to cancel " + e);
                }
                break;
            case TASK_GET_SUGGESTIONS_MULTIPLE:
                if (DBG) {
                    Log.w(TAG, "Get suggestions from the spell checker.");
                }
                try {
                    session.onGetSuggestionsMultiple(scp.mTextInfos,
                            scp.mSuggestionsLimit, scp.mSequentialWords);
                } catch (RemoteException e) {
                    Log.e(TAG, "Failed to get suggestions " + e);
                }
                break;
            case TASK_GET_SUGGESTIONS_MULTIPLE_FOR_SENTENCE:
                if (DBG) {
                    Log.w(TAG, "Get sentence suggestions from the spell checker.");
                }
                try {
                    session.onGetSentenceSuggestionsMultiple(
                            scp.mTextInfos, scp.mSuggestionsLimit);
                } catch (RemoteException e) {
                    Log.e(TAG, "Failed to get suggestions " + e);
                }
                break;
            case TASK_CLOSE:
                if (DBG) {
                    Log.w(TAG, "Close spell checker tasks.");
                }
                try {
                    session.onClose();
                } catch (RemoteException e) {
                    Log.e(TAG, "Failed to close " + e);
                }
                break;
        }
    } else {
        // The interface is to a local object, so need to execute it
        // asynchronously.
        scp.mSession = session;
        mAsyncHandler.sendMessage(Message.obtain(mAsyncHandler, 1, scp));
    }

    if (scp.mWhat == TASK_CLOSE) {
        // If we are closing, we want to clean up our state now even
        // if it is pending as an async operation.
        synchronized(this) {
            mISpellCheckerSession = null;
            mHandler = null;
            if (mThread != null) {
                mThread.quit();
            }
            mThread = null;
            mAsyncHandler = null;
        }
    }*/
}

void SpellCheckerSession::SpellCheckerSessionListenerImpl::ProcessOrEnqueueTask(
    /* [in] */ SpellCheckerParams* scp)
{
//    if (DBG) {
//        Log.d(TAG, "process or enqueue task: " + mISpellCheckerSession);
//    }
/*
    AutoPtr<ISpellCheckerSession> session;
    //synchronized(this)
    {
        Object::AutoLock lock(mLock);
        session = mISpellCheckerSession;
        if (session == NULL) {
            SpellCheckerParams* closeTask = NULL;
            if (scp->mWhat == TASK_CANCEL) {
                while (!mPendingTasks.isEmpty()) {
                    final SpellCheckerParams tmp = mPendingTasks.poll();
                    if (tmp.mWhat == TASK_CLOSE) {
                        // Only one close task should be processed, while we need to remove
                        // all close tasks from the queue
                        closeTask = tmp;
                    }
                }
            }
            mPendingTasks.offer(scp);
            if (closeTask != null) {
                mPendingTasks.offer(closeTask);
            }
            return;
        }
    }
    processTask(session, scp, false);
    */
}

SpellCheckerSession::InternalListener::InternalListener(
    /* [in] */ SpellCheckerSessionListenerImpl* spellCheckerSessionListenerImpl)
{
    mParentSpellCheckerSessionListenerImpl = spellCheckerSessionListenerImpl;
}

void SpellCheckerSession::InternalListener::OnServiceConnected(
    /* [in] */ ISpellCheckerSession* session)
{
//    if (DBG) {
//        Log.w(TAG, "SpellCheckerSession connected.");
//    }
    mParentSpellCheckerSessionListenerImpl->OnServiceConnected(session);
}

void SpellCheckerSession::HandleOnGetSuggestionsMultiple(
    /* [in] */ ArrayOf<ISuggestionsInfo*>* suggestionInfos)
{
    mSpellCheckerSessionListener->OnGetSuggestions(suggestionInfos);
}

void SpellCheckerSession::HandleOnGetSentenceSuggestionsMultiple(
    /* [in] */ ArrayOf<ISentenceSuggestionsInfo*>* suggestionInfos)
{
    mSpellCheckerSessionListener->OnGetSentenceSuggestions(suggestionInfos);
}

}   //namespace Textservice
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
