
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//===============================================================
//       AwWebContentsDelegateAdapter::GetDisplayNameTask
//===============================================================

AwWebContentsDelegateAdapter::GetDisplayNameTask::GetDisplayNameTask(
    /* [in] */ IContentResolver* contentResolver,
    /* [in] */ Int32 processId,
    /* [in] */ Int32 renderId,
    /* [in] */ Int32 modeFlags,
    /* [in] */ ArrayOf<String>* filePaths)
    : mProcessId(processId)
    , mRenderId(renderId)
    , mModeFlags(modeFlags)
    , mFilePaths(filePaths)
    , mContentResolver(contentResolver)
{
}

//@Override
AutoPtr< ArrayOf<String> > AwWebContentsDelegateAdapter::GetDisplayNameTask::DoInBackground(/*Void...voids*/)
{
    AutoPtr< ArrayOf<String> > displayNames = ArrayOf<String>::Alloc(mFilePaths->GetLength());
    for (Int32 i = 0; i < mFilePaths.length; i++) {
        (*displayNames)[i] = ResolveFileName(mFilePaths[i]);
    }
    return displayNames;
}

//@Override
void AwWebContentsDelegateAdapter::GetDisplayNameTask::OnPostExecute(
    /* [in] */ ArrayOf<String>* result)
{
    NativeFilesSelectedInChooser(mProcessId, mRenderId, mModeFlags, mFilePaths, result);
}

/**
 * @return the display name of a path if it is a content URI and is present in the database
 * or an empty string otherwise.
 */
String AwWebContentsDelegateAdapter::GetDisplayNameTask::ResolveFileName(
    /* [in] */ String filePath)
{
    if (mContentResolver == NULL || filePath == NULL)
    {
        return String("");
    }

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    AutoPtr<IUri> uri;
    helper->Parse(filePath, (IUri**)&helper);
    return ContentUriUtils::GetDisplayName(
            uri, mContentResolver, IMediaStoreMediaColumns::DISPLAY_NAME);
}

//===============================================================
//         AwWebContentsDelegateAdapter::InnerHandler
//===============================================================

AwWebContentsDelegateAdapter::InnerHandler::InnerHandler(
    /* [in] */ AwWebContentsDelegateAdapter* owner,
    /* [in] */ ContentViewCore* contentViewCore)
    : mOwner(owner)
{
}

ECode AwWebContentsDelegateAdapter::InnerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);
    Int32 what;
    msg->GetWhat(&what);
    switch(what) {
        case MSG_CONTINUE_PENDING_RELOAD: {
            contentViewCore->ContinuePendingReload();
            break;
        }
        case MSG_CANCEL_PENDING_RELOAD: {
            contentViewCore->CancelPendingReload();
            break;
        }
//        default:
//            throw new IllegalStateException(
//                    "WebContentsDelegateAdapter: unhandled message " + msg.what);
    }
    return NOERROR;
}

//===============================================================
//       AwWebContentsDelegateAdapter::InnerValueCallback
//===============================================================

AwWebContentsDelegateAdapter::InnerValueCallback::InnerValueCallback(
    /* [in] */ AwWebContentsDelegateAdapter* owner,
    /* [in] */ Int32 processId,
    /* [in] */ Int32 renderId,
    /* [in] */ Int32 modeFlags,
    /* [in] */ Boolean completed)
    : mOwner(owner)
    , mProcessId(processId)
    , mRenderId(renderId)
    , mModeFlags(modeFlags)
    , mCompleted(completed)
{
}

ECode AwWebContentsDelegateAdapter::InnerValueCallback::OnReceiveValue(
    /* [in] */ IInterface* results)
{
    if (mCompleted) {
        //throw new IllegalStateException("Duplicate showFileChooser result");
        assert(0);
    }

    mCompleted = TRUE;

    if (results == NULL) {
        NativeFilesSelectedInChooser(
                mProcessId, mRenderId, mModeFlags, NULL, NULL);
        return;
    }

    AutoPtr<IContentResolver> contentResolver;
    mContext->GetContentResolver((IContentResolver**)&contentResolver);
    AutoPtr<GetDisplayNameTask> task = new GetDisplayNameTask(
            contentResolver, mProcessId, mRenderId, mModeFlags, results);
    task->Execute();

    return NOERROR;
}

//===============================================================
//                AwWebContentsDelegateAdapter
//===============================================================

AwWebContentsDelegateAdapter::AwWebContentsDelegateAdapter(
    /* [in] */ AwContentsClient* contentsClient,
    /* [in] */ IView* containerView,
    /* [in] */ IContext* context)
    : mContentsClient(contentsClient)
    , mContext(context)
{
    SetContainerView(containerView);
}

void AwWebContentsDelegateAdapter::SetContainerView(
    /* [in] */ IView* containerView)
{
    mContainerView = containerView;
}

//@Override
void AwWebContentsDelegateAdapter::OnLoadProgressChanged(
    /* [in] */ Int32 progress)
{
    mContentsClient->OnProgressChanged(progress);
}

//@Override
void AwWebContentsDelegateAdapter::handleKeyboardEvent(
    /* [in] */ IKeyEvent* event)
{
    Int32 action;
    event->GetAction(&action);
    if (action == IKeyEvent::ACTION_DOWN) {
        Int32 direction;
        Int32 keyCode;
        event->GetKeyCode(&keyCode);
        switch (keyCode) {
            case IKeyEvent::KEYCODE_DPAD_DOWN:
                direction = IView::FOCUS_DOWN;
                break;
            case IKeyEvent::KEYCODE_DPAD_UP:
                direction = IView::FOCUS_UP;
                break;
            case IKeyEvent::KEYCODE_DPAD_LEFT:
                direction = IView::FOCUS_LEFT;
                break;
            case IKeyEvent::KEYCODE_DPAD_RIGHT:
                direction = IView::FOCUS_RIGHT;
                break;
            default:
                direction = 0;
                break;
        }
        if (direction != 0 && TryToMoveFocus(direction)) return;
    }
    mContentsClient->OnUnhandledKeyEvent(event);
}

//@Override
Boolean AwWebContentsDelegateAdapter::TakeFocus(
    /* [in] */ Boolean reverse)
{
    Int32 layoutDirection;
    mContainerView->GetLayoutDirection(&layoutDirection);
    Int32 direction =
        (reverse == (layoutDirection == IView::LAYOUT_DIRECTION_RTL)) ?
        IView::FOCUS_RIGHT : IView::FOCUS_LEFT;
    if (TryToMoveFocus(direction)) return TRUE;
    direction = reverse ? IView::FOCUS_UP : IView::FOCUS_DOWN;
    return TryToMoveFocus(direction);
}

Boolean AwWebContentsDelegateAdapter::TryToMoveFocus(
    /* [in] */ Int32 direction)
{
    AutoPtr<IView> focus;
    mContainerView->FocusSearch(direction, (IView**)&focus);
    Boolean bFocus = FALSE;
    focus->RequestFocus(&bFocus);
    return focus != NULL && focus != mContainerView && bFocus;
}

//@Override
Boolean AwWebContentsDelegateAdapter::AddMessageToConsole(
    /* [in] */ Int32 level,
    /* [in] */ String message,
    /* [in] */ Int32 lineNumber,
    /* [in] */ String sourceId)
{
    ConsoleMessage.MessageLevel messageLevel = ConsoleMessage.MessageLevel.DEBUG;
    switch(level) {
        case LOG_LEVEL_TIP:
            messageLevel = ConsoleMessage.MessageLevel.TIP;
            break;
        case LOG_LEVEL_LOG:
            messageLevel = ConsoleMessage.MessageLevel.LOG;
            break;
        case LOG_LEVEL_WARNING:
            messageLevel = ConsoleMessage.MessageLevel.WARNING;
            break;
        case LOG_LEVEL_ERROR:
            messageLevel = ConsoleMessage.MessageLevel.ERROR;
            break;
        default:
//            Log.w(TAG, "Unknown message level, defaulting to DEBUG");
            break;
    }

    AutoPtr<IConsoleMessage> cm;
    CConsoleMessage::New(message, sourceId, lineNumber, messageLevel, (IConsoleMessage**)&cm);
    return mContentsClient->OnConsoleMessage(cm);
}

//@Override
void AwWebContentsDelegateAdapter::OnUpdateUrl(
    /* [in] */ String url)
{
    // TODO: implement
}

//@Override
void AwWebContentsDelegateAdapter::OpenNewTab(
    /* [in] */ String url,
    /* [in] */ String extraHeaders,
    /* [in] */ ArrayOf<Byte>* postData,
    /* [in] */ Int32 disposition,
    /* [in] */ Boolean isRendererInitiated)
{
    // This is only called in chrome layers.
    assert(FALSE);
}

//@Override
void AwWebContentsDelegateAdapter::CloseContents()
{
    mContentsClient->OnCloseWindow();
}

//@Override
void AwWebContentsDelegateAdapter::ShowRepostFormWarningDialog(
    /* [in] */ ContentViewCore* contentViewCore)
{
    // TODO(mkosiba) We should be using something akin to the JsResultReceiver as the
    // callback parameter (instead of ContentViewCore) and implement a way of converting
    // that to a pair of messages.
    const Int32 MSG_CONTINUE_PENDING_RELOAD = 1;
    const Int32 MSG_CANCEL_PENDING_RELOAD = 2;

    // TODO(sgurun) Remember the URL to cancel the reload behavior
    // if it is different than the most recent NavigationController entry.
    AutoPtr<ILooper> looper = ThreadUtils::GetUiThreadLooper();
    AutoPtr<IHandler> handler = new InnerHandler(this, looper, contentViewCore);
    AutoPtr<IMessage> resend;
    handler->ObtainMessage(MSG_CONTINUE_PENDING_RELOAD, (IMessage**)&resend);
    AutoPtr<IMessage> dontResend;
    handler->ObtainMessage(MSG_CANCEL_PENDING_RELOAD, (IMessage**)&dontResend);
    mContentsClient->OnFormResubmission(dontResend, resend);
}

//@Override
void AwWebContentsDelegateAdapter::RunFileChooser(
    /* [in] */ Int32 processId,
    /* [in] */ Int32 renderId,
    /* [in] */ Int32 modeFlags,
    /* [in] */ String acceptTypes,
    /* [in] */ String title,
    /* [in] */ String defaultFilename,
    /* [in] */ Boolean capture)
{
    AutoPtr<AwContentsClient::FileChooserParams> params = new AwContentsClient::FileChooserParams();
    params->mode = modeFlags;
    params->acceptTypes = acceptTypes;
    params->title = title;
    params->defaultFilename = defaultFilename;
    params->capture = capture;

    Boolean completed = FALSE;
    AutoPtr<IValueCallback> callback = new InnerValueCallback(this, processId, renderId, modeFlags);
    mContentsClient->ShowFileChooser(callback, params);
}

//@Override
Boolean AwWebContentsDelegateAdapter::AddNewContents(
    /* [in] */ Boolean isDialog,
    /* [in] */ Boolean isUserGesture)
{
    return mContentsClient->OnCreateWindow(isDialog, isUserGesture);
}

//@Override
void AwWebContentsDelegateAdapter::ActivateContents()
{
    mContentsClient->OnRequestFocus();
}

//@Override
void AwWebContentsDelegateAdapter::ToggleFullscreenModeForTab(
    /* [in] */ Boolean enterFullscreen)
{
    if (!enterFullscreen) {
        AutoPtr<ContentVideoView> videoView = ContentVideoView::GetContentVideoView();
        if (videoView != NULL) videoView->ExitFullscreen(FALSE);
    }
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos