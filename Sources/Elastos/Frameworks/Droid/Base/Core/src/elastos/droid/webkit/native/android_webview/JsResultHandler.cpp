
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//===============================================================
//               JsResultHandler::ConfirmRunnable
//===============================================================

JsResultHandler::ConfirmRunnable::ConfirmRunnable(
    /* [in] */ JsResultHandler* owner,
    /* [in] */ const String& promptResult)
    : mOwner(owner)
    , mPromptResult(promptResult)
{
}

ECode JsResultHandler::ConfirmRunnable::Run()
{
    if (mOwner->mBridge != NULL)
        mOwner->mBridge->ConfirmJsResult(mOwner->mId, mPromptResult);
    mOwner->mBridge = NULL;

    return NOERROR;
}

//===============================================================
//               JsResultHandler::CancelRunnable
//===============================================================

JsResultHandler::CancelRunnable::CancelRunnable(
    /* [in] */ JsResultHandler* owner)
    : mOwner(owner)
{
}

ECode JsResultHandler::CancelRunnable::Run()
{
    if (mBridge != NULL)
        mBridge->CancelJsResult(mId);
    mBridge = NULL;

    return NOERROR;
}

//===============================================================
//                      JsResultHandler
//===============================================================

JsResultHandler::JsResultHandler(
    /* [in] */ AwContentsClientBridge* bridge,
    /* [in] */ Int32 id)
    : mBridge(bridge)
    , mId(id)
{
}

//@Override
void JsResultHandler::Confirm()
{
    Confirm(NULL);
}

//@Override
void JsResultHandler::Confirm(
    /* [in] */ const String& promptResult)
{
    AutoPtr<IRunnable> runnable =  new ConfirmRunnable(this, promptResult);
    ThreadUtils::RunOnUiThread(runnable);
}

//@Override
void JsResultHandler::Cancel()
{
    AutoPtr<IRunnable> runnable =  new CancelRunnable(this);
    ThreadUtils::RunOnUiThread(runnable);
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
