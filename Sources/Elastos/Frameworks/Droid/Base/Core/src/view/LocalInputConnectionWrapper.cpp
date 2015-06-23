
#include "view/LocalInputConnectionWrapper.h"
#include "view/CInputContextCallback.h"
#include "os/SystemClock.h"

using Elastos::Droid::View::InputMethod::EIID_IInputConnection;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {

const Int32 LocalInputConnectionWrapper::MAX_WAIT_TIME_MILLIS = 2000;

LocalInputConnectionWrapper::LocalInputConnectionWrapper(
    /* [in] */ IInputContext* inputContext)
    : mIInputContext(inputContext)
{
    assert(mIInputContext != NULL);
}

PInterface LocalInputConnectionWrapper::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInputConnection == riid) {
        return (IInputConnection *)this;
    }

    return NULL;
}

UInt32 LocalInputConnectionWrapper::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 LocalInputConnectionWrapper::Release()
{
    return ElRefBase::Release();
}

ECode LocalInputConnectionWrapper::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface*)(IInputConnection*)this) {
        *pIID = EIID_IInputConnection;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode LocalInputConnectionWrapper::GetTextAfterCursor(
    /* [in] */ Int32 n,
    /* [in] */ Int32 flags,
    /* [out] */ ICharSequence** text)
{
    assert(text != NULL);
    *text = NULL;
    AutoPtr<CInputContextCallback> callback = CInputContextCallback::GetInstance();
    mIInputContext->GetTextAfterCursor(n, flags, callback->mSeq, callback.Get());
    {
        Mutex::Autolock lock(callback->_m_syncLock);
        callback->WaitForResultLocked();
        if (callback->mHaveValue) {
            *text = callback->mTextAfterCursor;
        }
    }

    callback->Dispose();
    // catch (RemoteException e) {
    //     return NULL;
    // }
    REFCOUNT_ADD(*text);

    return NOERROR;
}

ECode LocalInputConnectionWrapper::GetTextBeforeCursor(
    /* [in] */ Int32 n,
    /* [in] */ Int32 flags,
    /* [out] */ ICharSequence** text)
{
    assert(text != NULL);
    *text = NULL;
    AutoPtr<CInputContextCallback> callback = CInputContextCallback::GetInstance();
    mIInputContext->GetTextBeforeCursor(n, flags, callback->mSeq, callback.Get());
    {
        Mutex::Autolock lock(callback->_m_syncLock);
        callback->WaitForResultLocked();
        if (callback->mHaveValue) {
            *text = callback->mTextBeforeCursor;
            REFCOUNT_ADD(*text);
        }
    }

    callback->Dispose();
    // } catch (RemoteException e) {
    //     return NULL;
    // }

    return NOERROR;
}

ECode LocalInputConnectionWrapper::GetSelectedText(
    /* [in] */ Int32 flags,
    /* [out] */ ICharSequence** text)
{
    assert(text != NULL);
    *text = NULL;
    AutoPtr<CInputContextCallback> callback = CInputContextCallback::GetInstance();
    mIInputContext->GetSelectedText(flags, callback->mSeq, callback.Get());
    {
        Mutex::Autolock lock(callback->_m_syncLock);
        callback->WaitForResultLocked();
        if (callback->mHaveValue) {
            *text = callback->mSelectedText;
        }
    }

    callback->Dispose();
    // } catch (RemoteException e) {
    //     return NULL;
    // }
    REFCOUNT_ADD(*text);

    return NOERROR;
}

ECode LocalInputConnectionWrapper::GetCursorCapsMode(
    /* [in] */ Int32 reqModes,
    /* [out] */ Int32* capsMode)
{
    assert(capsMode != NULL);
    *capsMode = 0;

    AutoPtr<CInputContextCallback> callback = CInputContextCallback::GetInstance();
    mIInputContext->GetCursorCapsMode(reqModes, callback->mSeq, callback.Get());
    {
        Mutex::Autolock lock(callback->_m_syncLock);
        callback->WaitForResultLocked();
        if (callback->mHaveValue) {
            *capsMode = callback->mCursorCapsMode;
        }
    }

    callback->Dispose();
    // } catch (RemoteException e) {
    //     return 0;
    // }
    return NOERROR;
}

ECode LocalInputConnectionWrapper::GetExtractedText(
    /* [in] */ IExtractedTextRequest* request,
    /* [in] */ Int32 flags,
    /* [out] */ IExtractedText** extractedText)
{
    assert(extractedText != NULL);
    *extractedText = NULL;

    AutoPtr<CInputContextCallback> callback = CInputContextCallback::GetInstance();
    mIInputContext->GetExtractedText(request, flags, callback->mSeq, callback.Get());
    {
        Mutex::Autolock lock(callback->_m_syncLock);
        callback->WaitForResultLocked();
        if (callback->mHaveValue) {
            *extractedText = callback->mExtractedText;
        }
    }

    callback->Dispose();
    // } catch (RemoteException e) {
    //     return NULL;
    // }

    REFCOUNT_ADD(*extractedText);

    return NOERROR;
}

ECode LocalInputConnectionWrapper::CommitText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    // try {
    ECode ec = mIInputContext->CommitText(text, newCursorPosition);
    *flag = SUCCEEDED(ec) ? TRUE : FALSE;
    return ec;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::CommitCompletion(
    /* [in] */ ICompletionInfo* text,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    // try {
    ECode ec = mIInputContext->CommitCompletion(text);
    *flag = SUCCEEDED(ec) ? TRUE : FALSE;
    return ec;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::CommitCorrection(
    /* [in] */ ICorrectionInfo* correctionInfo,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    // try {
    ECode ec = mIInputContext->CommitCorrection(correctionInfo);
    *flag = SUCCEEDED(ec) ? TRUE : FALSE;
    return ec;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    // try {
    ECode ec = mIInputContext->SetSelection(start, end);
    *flag = SUCCEEDED(ec) ? TRUE : FALSE;
    return ec;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::PerformEditorAction(
    /* [in] */ Int32 editorAction,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    // try {
    ECode ec = mIInputContext->PerformEditorAction(editorAction);
    *flag = SUCCEEDED(ec) ? TRUE : FALSE;
    return ec;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::PerformContextMenuAction(
    /* [in] */ Int32 id,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    // try {
    ECode ec = mIInputContext->PerformContextMenuAction(id);
    *flag = SUCCEEDED(ec) ? TRUE : FALSE;
    return ec;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::SetComposingRegion(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    // try {
    ECode ec = mIInputContext->SetComposingRegion(start, end);
    *flag = SUCCEEDED(ec) ? TRUE : FALSE;
    return ec;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::SetComposingText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    // try {
    ECode ec = mIInputContext->SetComposingText(text, newCursorPosition);
    *flag = SUCCEEDED(ec) ? TRUE : FALSE;
    return ec;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::FinishComposingText(
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    // try {
    ECode ec = mIInputContext->FinishComposingText();
    *flag = SUCCEEDED(ec) ? TRUE : FALSE;
    return ec;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::BeginBatchEdit(
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    // try {
    ECode ec = mIInputContext->BeginBatchEdit();
    *flag = SUCCEEDED(ec) ? TRUE : FALSE;
    return ec;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::EndBatchEdit(
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    // try {
    ECode ec = mIInputContext->EndBatchEdit();
    *flag = SUCCEEDED(ec) ? TRUE : FALSE;
    return ec;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::SendKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    // try {
    ECode ec = mIInputContext->SendKeyEvent(event);
    *flag = SUCCEEDED(ec) ? TRUE : FALSE;
    return ec;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::ClearMetaKeyStates(
    /* [in] */ Int32 states,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    // try {
    ECode ec = mIInputContext->ClearMetaKeyStates(states);
    *flag = SUCCEEDED(ec) ? TRUE : FALSE;
    return ec;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::DeleteSurroundingText(
    /* [in] */ Int32 leftLength,
    /* [in] */ Int32 rightLength,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    // try {
    ECode ec = mIInputContext->DeleteSurroundingText(leftLength, rightLength);
    *flag = SUCCEEDED(ec) ? TRUE : FALSE;
    return ec;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::ReportFullscreenMode(
    /* [in] */ Boolean enabled,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    // try {
    ECode ec = mIInputContext->ReportFullscreenMode(enabled);
    *flag = SUCCEEDED(ec) ? TRUE : FALSE;
    return ec;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::PerformPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);
    // try {
    ECode ec = mIInputContext->PerformPrivateCommand(action, data);
    *flag = SUCCEEDED(ec) ? TRUE : FALSE;
    return ec;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos
