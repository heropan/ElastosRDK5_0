
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

//===============================================================
//          MediaDrmBridge::PendingCreateSessionData
//===============================================================

MediaDrmBridge::PendingCreateSessionData::PendingCreateSessionData(int sessionId, byte[] initData, String mimeType) {
    mSessionId = sessionId;
    mInitData = initData;
    mMimeType = mimeType;
}

Int32 MediaDrmBridge::PendingCreateSessionData::SessionId()
{
    return mSessionId;
}

AutoPtr< ArrayOf<Byte> > MediaDrmBridge::PendingCreateSessionData::InitData()
{
    return mInitData;
}

String MediaDrmBridge::PendingCreateSessionData::MimeType()
{
    return mMimeType;
}

//===============================================================
//               MediaDrmBridge::MediaDrmListener
//===============================================================

MediaDrmBridge::MediaDrmListener::MediaDrmListener(
    /* [in] */ MediaDrmBridge* owner)
    : mOwner(owner)
{
}

//@Override
ECode MediaDrmBridge::MediaDrmListener::OnEvent(
    /* [in] */ IMediaDrm* mediaDrm,
    /* [in] */ ArrayOf<Byte>* session_array,
    /* [in] */ Int32 event,
    /* [in] */ Int32 extra,
    /* [in] */ ArrayOf<Byte>* data)
{
    if (session_array == NULL) {
//        Log.e(TAG, "MediaDrmListener: Null session.");
        return;
    }

    AutoPtr<IByteBuffer> session = ByteBuffer::Wrap(session_array);
    if (!SessionExists(session)) {
//        Log.e(TAG, "MediaDrmListener: Invalid session.");
        return;
    }

    Int32 sessionId = mSessionIds.get(session);
    if (sessionId == NULL || sessionId == INVALID_SESSION_ID) {
//        Log.e(TAG, "MediaDrmListener: Invalid session ID.");
        return;
    }

    switch(event) {
        case IMediaDrm::EVENT_PROVISION_REQUIRED:
//            Log.d(TAG, "MediaDrm.EVENT_PROVISION_REQUIRED");
            break;
        case IMediaDrm::EVENT_KEY_REQUIRED:
//            Log.d(TAG, "MediaDrm.EVENT_KEY_REQUIRED");
            if (mProvisioningPending) {
                return NOERROR;
            }
            String mime = mSessionMimeTypes.get(session);
            AutoPtr<IMediaDrmKeyRequest> request;
            // try {
                request = GetKeyRequest(session, data, mime);
            // } catch (android.media.NotProvisionedException e) {
            //     Log.e(TAG, "Device not provisioned", e);
            //     startProvisioning();
            //     return;
            // }

            if (request != NULL) {
                OnSessionMessage(sessionId, request);
            }
            else {
                OnSessionError(sessionId);
            }
            break;
        case IMediaDrm::EVENT_KEY_EXPIRED:
//            Log.d(TAG, "MediaDrm.EVENT_KEY_EXPIRED");
            OnSessionError(sessionId);
            break;
        case IMediaDrm::EVENT_VENDOR_DEFINED:
//            Log.d(TAG, "MediaDrm.EVENT_VENDOR_DEFINED");
            assert(FALSE);  // Should never happen.
            break;
        default:
//            Log.e(TAG, "Invalid DRM event " + event);
            return NOERROR;
    }

    return NOERROR;
}

//===============================================================
//              MediaDrmBridge::PostRequestTask
//===============================================================

const String MediaDrmBridge::PostRequestTask::TAG("PostRequestTask");

MediaDrmBridge::PostRequestTask::PostRequestTask(
    /* [in] */ MediaDrmBridge* owner,
    /* [in] */ ArrayOf<Byte>* drmRequest)
    : mOwner(owner)
    , mDrmRequest(drmRequest)
{
}

//@Override
Void MediaDrmBridge::PostRequestTask::DoInBackground(String... urls)
{
    mResponseBody = PostRequest(urls[0], mDrmRequest);
    if (mResponseBody != NULL) {
//        Log.d(TAG, "response length=" + mResponseBody.length);
    }
    return NULL;
}

AutoPtr< ArrayOf<Byte> > MediaDrmBridge::PostRequestTask::PostRequest(
    /* [in] */ String url,
    /* [in] */ ArrayOf<Byte>* drmRequest)
{
    AutoPtr<IHttpClient> httpClient;
    CDefaultHttpClient((IHttpClient**)&httpClient);
    String str(url);
    str += "&signedRequest=";
    str += drmRequest->ToString();
    AutoPtr<IHttpPost> httpPost;
    CHttpPost::New(str, (IHttpPost**)&httpPost);

//    Log.d(TAG, "PostRequest:" + httpPost.getRequestLine());
    // try {
        // Add data
        httpPost->SetHeader("Accept", "*/*");
        httpPost->SetHeader("User-Agent", "Widevine CDM v1.0");
        httpPost->SetHeader("Content-Type", "application/json");

        // Execute HTTP Post Request
        AutoPtr<IHttpResponse> response;
        httpClient->Execute(httpPost, (IHttpResponse**)&response);

        AutoPtr< ArrayOf<Byte> > responseBody;
        AutoPtr<IStatusLine> line;
        response->GetStatusLine((IStatusLine**)&line);
        Int32 responseCode;
        line->GetStatusCode(&responseCode);
        if (responseCode == 200) {
            responseBody = EntityUtils.toByteArray(response.getEntity());
        }
        else {
//            Log.d(TAG, "Server returned HTTP error code " + responseCode);
            return NULL;
        }

        return responseBody;
    // } catch (ClientProtocolException e) {
    //     e.printStackTrace();
    // } catch (IOException e) {
    //     e.printStackTrace();
    // }

    return NULL;
}

//@Override
void MediaDrmBridge::PostRequestTask::OnPostExecute(Void v)
{
    OnProvisionResponse(mResponseBody);
}

//===============================================================
//       MediaDrmBridge::ResumePendingOperationsRunnable
//===============================================================

MediaDrmBridge::ResumePendingOperationsRunnable::ResumePendingOperationsRunnable(
    /* [in] */ MediaDrmBridge* owner)
    : mOwner(owner)
{
}

ECode MediaDrmBridge::ResumePendingOperationsRunnable::Run()
{
    mOwner->ProcessPendingCreateSessionData();
    return NOERROR;
}

//===============================================================
//           MediaDrmBridge::OnSessionCreatedRunnable
//===============================================================

MediaDrmBridge::OnSessionCreatedRunnable::OnSessionCreatedRunnable(
    /* [in] */ MediaDrmBridge* owner,
    /* [in] */ Int32 sessionId,
    /* [in] */ const String& webSessionId)
    : mOwner(owner)
    , mSessionId(sessionId)
    , mWebSessionId(webSessionId)
{
}

ECode MediaDrmBridge::OnSessionCreatedRunnable::Run()
{
    mOwner->NativeOnSessionCreated(mOwner->mNativeMediaDrmBridge, mSessionId, mWebSessionId);
    return NOERROR;
}

//===============================================================
//          MediaDrmBridge::OnSessionMessageRunnable
//===============================================================

MediaDrmBridge::OnSessionMessageRunnable::OnSessionMessageRunnable(
    /* [in] */ MediaDrmBridge* owner,
    /* [in] */ Int32 sessionId,
    /* [in] */ IMediaDrmKeyRequest* request)
    : mOwner(owner)
    , mSessionId(sessionId)
    , mRequest(request)
{
}

ECode MediaDrmBridge::OnSessionMessageRunnable::Run()
{
    AutoPtr< ArrayOf<Byte> > data;
    request->GetData((ArrayOf<Byte>**)&data);
    String url;
    request->GetDefaultUrl(&url);
    mOwner->NativeOnSessionMessage(mOwner->mNativeMediaDrmBridge, mSessionId,
                    data, url);
    return NOERROR;
}

//===============================================================
//           MediaDrmBridge::OnSessionReadyRunnable
//===============================================================

MediaDrmBridge::OnSessionReadyRunnable::OnSessionReadyRunnable(
    /* [in] */ MediaDrmBridge* owner,
    /* [in] */ Int32 sessionId)
    : mOwner(owner)
    , mSessionId(sessionId)
{
}

ECode MediaDrmBridge::OnSessionReadyRunnable::Run()
{
    mOwner->NativeOnSessionReady(mOwner->mNativeMediaDrmBridge, mSessionId);
    return NOERROR;
}

//===============================================================
//            MediaDrmBridge::OnSessionClosedRunnable
//===============================================================

MediaDrmBridge::OnSessionClosedRunnable::OnSessionClosedRunnable(
    /* [in] */ MediaDrmBridge* owner,
    /* [in] */ Int32 sessionId)
    : mOwner(owner)
    , mSessionId(sessionId)
{
}

ECode MediaDrmBridge::OnSessionClosedRunnable::Run()
{
    mOwner->NativeOnSessionClosed(mOwner->mNativeMediaDrmBridge, mSessionId);
    return NOERROR;
}

//===============================================================
//          MediaDrmBridge::OnSessionErrorRunnable
//===============================================================

MediaDrmBridge::OnSessionErrorRunnable::OnSessionErrorRunnable(
    /* [in] */ MediaDrmBridge* owner,
    /* [in] */ Int32 sessionId)
    : mOwner(owner)
    , mSessionId(sessionId)
{
}

ECode MediaDrmBridge::OnSessionErrorRunnable::Run()
{
    mOwner->NativeOnSessionError(mOwner->mNativeMediaDrmBridge, mSessionId);
    return NOERROR;
}

//===============================================================
//                      MediaDrmBridge
//===============================================================

const String MediaDrmBridge::TAG("MediaDrmBridge");
const String MediaDrmBridge::SECURITY_LEVEL("securityLevel");
const String MediaDrmBridge::PRIVACY_MODE("privacyMode");
const String MediaDrmBridge::SESSION_SHARING("sessionSharing");
const String MediaDrmBridge::ENABLE("enable");
const Int32 MediaDrmBridge::INVALID_SESSION_ID;

MediaDrmBridge::MediaDrmBridge(
    /* [in] */ IUUID* schemeUUID,
    /* [in] */ Int64 nativeMediaDrmBridge,
    /* [in] */ Boolean singleSessionMode)
{
    mSchemeUUID = schemeUUID;
    CMediaDrm::New(schemeUUID, (IMediaDrm**)&mMediaDrm);
    mNativeMediaDrmBridge = nativeMediaDrmBridge;
    CHandler::New((IHandler**)&mHandler);
    mSingleSessionMode = singleSessionMode;
    mSessionIds = new HashMap<ByteBuffer, Integer>();
    mSessionMimeTypes = new HashMap<ByteBuffer, String>();
    mPendingCreateSessionDataQueue = new ArrayDeque<PendingCreateSessionData>();
    mResetDeviceCredentialsPending = FALSE;
    mProvisioningPending = FALSE;

    AutoPtr<MediaDrmListener> lisener;
    mMediaDrm->SetOnEventListener(lisener);
    mMediaDrm->SetPropertyString(PRIVACY_MODE, ENABLE);
    if (!mSingleSessionMode) {
        mMediaDrm->SetPropertyString(SESSION_SHARING, ENABLE);
    }

    // We could open a MediaCrypto session here to support faster start of
    // clear lead (no need to wait for createSession()). But on
    // Android, memory and battery resources are precious and we should
    // only create a session when we are sure we'll use it.
    // TODO(xhwang): Investigate other options to support fast start.
}

AutoPtr<IUUID> MediaDrmBridge::GetUUIDFromBytes(
    /* [in] */ Byte* data)
{
    if (data->GetLength() != 16) {
        return NULL;
    }

    Int64 mostSigBits = 0;
    Int64 leastSigBits = 0;
    for (Int32 i = 0; i < 8; i++) {
        mostSigBits = (mostSigBits << 8) | ((*data)[i] & 0xff);
    }

    for (Int32 i = 8; i < 16; i++) {
        leastSigBits = (leastSigBits << 8) | ((*data)[i] & 0xff);
    }

    AutoPtr<IUUID> uuid;
    CUUID::New(mostSigBits, leastSigBits, (IUUID**)&uuid);

    return uuid;
}

/**
 *  Gets session associated with the sessionId.
 *
 *  @return session if sessionId maps a valid opened session. Returns null
 *  otherwise.
 */
AutoPtr<IByteBuffer> MediaDrmBridge::GetSession(
    /* [in] */ Int32 sessionId)
{
    for (ByteBuffer session : mSessionIds.keySet()) {
        if (mSessionIds.get(session) == sessionId) {
            return session;
        }
    }

    return NULL;
}

/**
 * Create a MediaCrypto object.
 *
 * @return whether a MediaCrypto object is successfully created.
 */
Boolean MediaDrmBridge::CreateMediaCrypto()
{
    if (mMediaDrm == NULL) {
        return FALSE;
    }
    assert(!mProvisioningPending);
    assert(mMediaCryptoSession == NULL);
    assert(mMediaCrypto == NULL);

    // Open media crypto session.
    mMediaCryptoSession = OpenSession();
    if (mMediaCryptoSession == NULL) {
//        Log.e(TAG, "Cannot create MediaCrypto Session.");
        return FALSE;
    }
//    Log.d(TAG, "MediaCrypto Session created: " + mMediaCryptoSession);

    // Create MediaCrypto object.
    // try {
        AutoPtr<IMediaCryptoHelper> helper;
        CMediaCryptoHelper::AcquireSingleton((IMediaCryptoHelper**)&helper);
        Boolean isCryptoSchemeSupported;
        helper->IsCryptoSchemeSupported(mSchemeUUID, &isCryptoSchemeSupported);
        if (isCryptoSchemeSupported) {
            final byte[] mediaCryptoSession = mMediaCryptoSession.array();
            mMediaCrypto = new MediaCrypto(mSchemeUUID, mediaCryptoSession);
            assert mMediaCrypto != null;
            Log.d(TAG, "MediaCrypto successfully created!");
            mSessionIds.put(mMediaCryptoSession, INVALID_SESSION_ID);
            // Notify the native code that MediaCrypto is ready.
            NativeOnMediaCryptoReady(mNativeMediaDrmBridge);
            return TRUE;
        }
        else {
        //    Log.e(TAG, "Cannot create MediaCrypto for unsupported scheme.");
        }
    // } catch (android.media.MediaCryptoException e) {
    //     Log.e(TAG, "Cannot create MediaCrypto", e);
    // }

    Release();

    return FALSE;
}

/**
 * Open a new session..
 *
 * @return the session opened. Returns null if unexpected error happened.
 */
AutoPtr<IByteBuffer> MediaDrmBridge::OpenSession()
{
    assert(mMediaDrm != NULL);
    // try {
        AutoPtr< ArrayOf<Byte> > session;
        mMediaDrm->OpenSession((ArrayOf<Byte>**)&session);
        // ByteBuffer.wrap() is backed by the byte[]. Make a clone here in
        // case the underlying byte[] is modified.
        return ByteBuffer::Wrap(session->Clone());
    // } catch (java.lang.RuntimeException e) {  // TODO(xhwang): Drop this?
    //     Log.e(TAG, "Cannot open a new session", e);
    //     release();
    //     return null;
    // } catch (android.media.NotProvisionedException e) {
    //     // Throw NotProvisionedException so that we can startProvisioning().
    //     throw e;
    // } catch (android.media.MediaDrmException e) {
    //     // Other MediaDrmExceptions (e.g. ResourceBusyException) are not
    //     // recoverable.
    //     Log.e(TAG, "Cannot open a new session", e);
    //     release();
    //     return null;
    // }
}

/**
 * Close a session.
 *
 * @param session to be closed.
 */
void MediaDrmBridge::CloseSession(
    /* [in] */ IByteBuffer* session)
{
    assert(mMediaDrm != NULL);
    mMediaDrm->CloseSession(session.array());
}

/**
 * Check whether the crypto scheme is supported for the given container.
 * If |containerMimeType| is an empty string, we just return whether
 * the crypto scheme is supported.
 *
 * @return true if the container and the crypto scheme is supported, or
 * false otherwise.
 */
//@CalledByNative
Boolean MediaDrmBridge::IsCryptoSchemeSupported(
    /* [in] */ ArrayOf<Byte>* schemeUUID,
    /* [in] */ String containerMimeType)
{
    AutoPtr<IUUID> cryptoScheme = GetUUIDFromBytes(schemeUUID);

    AutoPtr<IMediaCryptoHelper> helper;
    CMediaCryptoHelper::AcquireSingleton((IMediaCryptoHelper**)&helper);
    Boolean result;

    if (containerMimeType.IsEmpty()) {
        helper->IsCryptoSchemeSupported(cryptoScheme, &result);
        return result;
    }

    helper->IsCryptoSchemeSupported(cryptoScheme, containerMimeType, &result);
    return result;
}

/**
 * Create a new MediaDrmBridge from the crypto scheme UUID.
 *
 * @param schemeUUID Crypto scheme UUID.
 * @param securityLevel Security level to be used.
 * @param nativeMediaDrmBridge Native object of this class.
 */
//@CalledByNative
AutoPtr<MediaDrmBridge> MediaDrmBridge::Create(
    /* [in] */ ArrayOf<Byte>* schemeUUID,
    /* [in] */ Int64 nativeMediaDrmBridge)
{
    AutoPtr<IUUID> cryptoScheme = GetUUIDFromBytes(schemeUUID);
    if (cryptoScheme == NULL || !MediaDrm.isCryptoSchemeSupported(cryptoScheme)) {
        return NULL;
    }

    Boolean singleSessionMode = FALSE;
    if (Build.VERSION.RELEASE.Equals("4.4")) {
        singleSessionMode = TRUE;
    }

//    Log.d(TAG, "MediaDrmBridge uses " +
//            (singleSessionMode ? "single" : "multiple") + "-session mode.");

    AutoPtr<MediaDrmBridge> mediaDrmBridge;
    // try {
        mediaDrmBridge = new MediaDrmBridge(
            cryptoScheme, nativeMediaDrmBridge, singleSessionMode);
//        Log.d(TAG, "MediaDrmBridge successfully created.");
    // } catch (android.media.UnsupportedSchemeException e) {
    //     Log.e(TAG, "Unsupported DRM scheme", e);
    // } catch (java.lang.IllegalArgumentException e) {
    //     Log.e(TAG, "Failed to create MediaDrmBridge", e);
    // } catch (java.lang.IllegalStateException e) {
    //     Log.e(TAG, "Failed to create MediaDrmBridge", e);
    // }

    return mediaDrmBridge;
}

/**
 * Set the security level that the MediaDrm object uses.
 * This function should be called right after we construct MediaDrmBridge
 * and before we make any other calls.
 */
//@CalledByNative
Boolean MediaDrmBridge::SetSecurityLevel(
    /* [in] */ String securityLevel)
{
    if (mMediaDrm == NULL || mMediaCrypto != NULL) {
        return FALSE;
    }

    String currentSecurityLevel;
    mMediaDrm->GetPropertyString(SECURITY_LEVEL, &currentSecurityLevel);
//    Log.e(TAG, "Security level: current " + currentSecurityLevel + ", new " + securityLevel);
    if (securityLevel.Equals(currentSecurityLevel)) {
        // No need to set the same security level again. This is not just
        // a shortcut! Setting the same security level actually causes an
        // exception in MediaDrm!
        return TRUE;
    }

    // try {
        mMediaDrm->SetPropertyString(SECURITY_LEVEL, securityLevel);
        return TRUE;
    // } catch (java.lang.IllegalArgumentException e) {
    //     Log.e(TAG, "Failed to set security level " + securityLevel, e);
    // } catch (java.lang.IllegalStateException e) {
    //     Log.e(TAG, "Failed to set security level " + securityLevel, e);
    // }

//    Log.e(TAG, "Security level " + securityLevel + " not supported!");

    return FALSE;
}

/**
 * Return the MediaCrypto object if available.
 */
//@CalledByNative
AutoPtr<IMediaCrypto> MediaDrmBridge::GetMediaCrypto()
{
    return mMediaCrypto;
}

/**
 * Reset the device DRM credentials.
 */
//@CalledByNative
void MediaDrmBridge::ResetDeviceCredentials()
{
    mResetDeviceCredentialsPending = TRUE;
    AutoPtr<IMediaDrmProvisionRequest> request;
    mMediaDrm->GetProvisionRequest((IMediaDrmProvisionRequest**)&request);
    AutoPtr< ArrayOf<Byte> > data;
    request->GetData((ArrayOf<Byte>**)&data);
    AutoPtr<PostRequestTask> postTask = new PostRequestTask(this, data);
    String url;
    request->GetDefaultUrl(&url);
    postTask->Execute(url);
}

/**
 * Release the MediaDrmBridge object.
 */
//@CalledByNative
void MediaDrmBridge::Release()
{
    // Do not reset mHandler and mNativeMediaDrmBridge so that we can still
    // post KeyError back to native code.

    mPendingCreateSessionDataQueue->Clear();
    mPendingCreateSessionDataQueue = NULL;

    for (ByteBuffer session : mSessionIds.keySet()) {
        CloseSession(session);
    }

    mSessionIds->Clear();
    mSessionIds = NULL;
    mSessionMimeTypes->Clear();
    mSessionMimeTypes = NULL;

    // This session was closed in the "for" loop above.
    mMediaCryptoSession = NULL;

    if (mMediaCrypto != NULL) {
        mMediaCrypto->Release();
        mMediaCrypto = NULL;
    }

    if (mMediaDrm != NULL) {
        mMediaDrm->Release();
        mMediaDrm = NULL;
    }
}

/**
 * Get a key request.
 *
 * @param session Session on which we need to get the key request.
 * @param data Data needed to get the key request.
 * @param mime Mime type to get the key request.
 *
 * @return the key request.
 */
AutoPtr<IMediaDrmKeyRequest> MediaDrmBridge::GetKeyRequest(
    /* [in] */ IByteBuffer* session,
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ String mime)
{
    assert(mMediaDrm != NULL);
    assert(mMediaCrypto != NULL);
    assert(!mProvisioningPending);

    HashMap<String, String> optionalParameters = new HashMap<String, String>();
    MediaDrm.KeyRequest request = mMediaDrm.getKeyRequest(
            session.array(), data, mime, MediaDrm.KEY_TYPE_STREAMING, optionalParameters);
    String result = (request != null) ? "successed" : "failed";
//    Log.d(TAG, "getKeyRequest " + result + "!");
    return request;
}

/**
 * Save data to |mPendingCreateSessionDataQueue| so that we can resume the
 * createSession() call later.
 */
void MediaDrmBridge::SavePendingCreateSessionData(
    /* [in] */ Int32 sessionId,
    /* [in] */ ArrayOf<Byte>* initData,
    /* [in] */ String mime)
{
//    Log.d(TAG, "savePendingCreateSessionData()");
    AutoPtr<PendingCreateSessionData> data = new PendingCreateSessionData(sessionId, initData, mime);
    mPendingCreateSessionDataQueue->Offer(data);
}

/**
 * Process all pending createSession() calls synchronously.
 */
void MediaDrmBridge::ProcessPendingCreateSessionData()
{
//    Log.d(TAG, "processPendingCreateSessionData()");
    assert(mMediaDrm != NULL);

    // Check mMediaDrm != null because error may happen in createSession().
    // Check !mProvisioningPending because NotProvisionedException may be
    // thrown in createSession().
    while (mMediaDrm != NULL && !mProvisioningPending &&
            !mPendingCreateSessionDataQueue.isEmpty()) {
        PendingCreateSessionData pendingData = mPendingCreateSessionDataQueue.poll();
        int sessionId = pendingData.sessionId();
        byte[] initData = pendingData.initData();
        String mime = pendingData.mimeType();
        createSession(sessionId, initData, mime);
    }
}

/**
 * Process pending operations asynchrnously.
 */
void MediaDrmBridge::ResumePendingOperations()
{
    AutoPtr<IRunnable> runnable = new ResumePendingOperationsRunnable(this);
    mHandler->Post(runnable);
}

/**
 * Create a session with |sessionId|, |initData| and |mime|.
 * In multiple session mode, a new session will be open. In single session
 * mode, the mMediaCryptoSession will be used.
 *
 * @param sessionId ID for the session to be created.
 * @param initData Data needed to generate the key request.
 * @param mime Mime type.
 */
//@CalledByNative
void MediaDrmBridge::CreateSession(
    /* [in] */ Int32 sessionId,
    /* [in] */ ArrayOf<Byte>* initData,
    /* [in] */ String mime)
{
//    Log.d(TAG, "createSession()");
    if (mMediaDrm == NULL) {
//        Log.e(TAG, "createSession() called when MediaDrm is null.");
        return;
    }

    if (mProvisioningPending) {
        assert(mMediaCrypto == NULL);
        SavePendingCreateSessionData(sessionId, initData, mime);
        return;
    }

    Boolean newSessionOpened = FALSE;
    AutoPtr<IByteBuffer> session;
    // try {
        // Create MediaCrypto if necessary.
        if (mMediaCrypto == NULL && !CreateMediaCrypto()) {
            OnSessionError(sessionId);
            return;
        }

        assert(mMediaCrypto != NULL;
        assert(mSessionIds.containsKey(mMediaCryptoSession));

        if (mSingleSessionMode) {
            session = mMediaCryptoSession;
            if (mSessionMimeTypes.get(session) != null &&
                    !mSessionMimeTypes.get(session).equals(mime)) {
//                Log.e(TAG, "Only one mime type is supported in single session mode.");
                OnSessionError(sessionId);
                return;
            }
        }
        else {
            session = OpenSession();
            if (session == NULL) {
//                Log.e(TAG, "Cannot open session in createSession().");
                OnSessionError(sessionId);
                return;
            }

            newSessionOpened = TRUE;
            assert(!mSessionIds->ContainsKey(session));
        }

        AutoPtr<IMediaDrmKeyRequest> request;
        request = GetKeyRequest(session, initData, mime);
        if (request == NULL) {
            if (newSessionOpened) {
                CloseSession(session);
            }
            OnSessionError(sessionId);
            return;
        }

        OnSessionCreated(sessionId, GetWebSessionId(session));
        OnSessionMessage(sessionId, request);
        if (newSessionOpened) {
            // Log.d(TAG, "createSession(): Session " + getWebSessionId(session) +
            //         " (" + sessionId + ") created.");
        }

        mSessionIds->Put(session, sessionId);
        mSessionMimeTypes->Put(session, mime);
    // } catch (android.media.NotProvisionedException e) {
    //     Log.e(TAG, "Device not provisioned", e);
    //     if (newSessionOpened) {
    //         closeSession(session);
    //     }
    //     savePendingCreateSessionData(sessionId, initData, mime);
    //     startProvisioning();
    // }
}

/**
 * Returns whether |sessionId| is a valid key session, excluding the media
 * crypto session in multi-session mode.
 *
 * @param sessionId Crypto session Id.
 */
Boolean MediaDrmBridge::SessionExists(
    /* [in] */ IByteBuffer* session)
{
    if (mMediaCryptoSession == NULL) {
        assert(mSessionIds->IsEmpty());
//        Log.e(TAG, "Session doesn't exist because media crypto session is not created.");
        return FALSE;
    }

    assert(mSessionIds.containsKey(mMediaCryptoSession));

    if (mSingleSessionMode) {
        return mMediaCryptoSession.Equals(session);
    }

    return !session.Equals(mMediaCryptoSession) && mSessionIds.containsKey(session);
}

/**
 * Cancel a key request for a session Id.
 *
 * @param sessionId Reference ID of session to be released.
 */
//@CalledByNative
void MediaDrmBridge::ReleaseSession(
    /* [in] */ Int32 sessionId)
{
//    Log.d(TAG, "releaseSession(): " + sessionId);
    if (mMediaDrm == NULL) {
//        Log.e(TAG, "releaseSession() called when MediaDrm is null.");
        return;
    }

    AutoPtr<IByteBuffer> session = GetSession(sessionId);
    if (session == NULL) {
//        Log.e(TAG, "Invalid sessionId in releaseSession.");
        OnSessionError(sessionId);
        return;
    }

    mMediaDrm.removeKeys(session.array());

    // We don't close the media crypto session in single session mode.
    if (!mSingleSessionMode) {
//        Log.d(TAG, "Session " + sessionId + "closed.");
        CloseSession(session);
        mSessionIds.remove(session);
        OnSessionClosed(sessionId);
    }
}

/**
 * Add a key for a session Id.
 *
 * @param sessionId Reference ID of session to be updated.
 * @param key Response data from the server.
 */
//@CalledByNative
void MediaDrmBridge::UpdateSession(
    /* [in] */ Int32 sessionId,
    /* [in] */ ArrayOf<Byte>* key)
{
//    Log.d(TAG, "updateSession(): " + sessionId);
    if (mMediaDrm == NULL) {
//        Log.e(TAG, "updateSession() called when MediaDrm is null.");
        return;
    }

    // TODO(xhwang): We should be able to DCHECK this when WD EME is implemented.
    AutoPtr<IByteBuffer> session = GetSession(sessionId);
    if (!sessionExists(session)) {
//        Log.e(TAG, "Invalid session in updateSession.");
        OnSessionError(sessionId);
        return;
    }

    // try {
    //     try {
            mMediaDrm->ProvideKeyResponse(session.array(), key);
        // } catch (java.lang.IllegalStateException e) {
        //     // This is not really an exception. Some error code are incorrectly
        //     // reported as an exception.
        //     // TODO(qinmin): remove this exception catch when b/10495563 is fixed.
        //     Log.e(TAG, "Exception intentionally caught when calling provideKeyResponse()", e);
        // }
        OnSessionReady(sessionId);
//        Log.d(TAG, "Key successfully added for session " + sessionId);
        return;
    // } catch (android.media.NotProvisionedException e) {
    //     // TODO(xhwang): Should we handle this?
    //     Log.e(TAG, "failed to provide key response", e);
    // } catch (android.media.DeniedByServerException e) {
    //     Log.e(TAG, "failed to provide key response", e);
    // }

    OnSessionError(sessionId);
    Release();
}

/**
 * Return the security level of this DRM object.
 */
//@CalledByNative
String MediaDrmBridge::GetSecurityLevel()
{
    if (mMediaDrm == NULL) {
//        Log.e(TAG, "getSecurityLevel() called when MediaDrm is null.");
        return NULL;
    }

    String property;
    mMediaDrm->GetPropertyString(String("securityLevel"), &property);

    return property;
}

void MediaDrmBridge::StartProvisioning()
{
//    Log.d(TAG, "startProvisioning");
    assert(mMediaDrm != NULL);
    assert(!mProvisioningPending);
    mProvisioningPending = TRUE;
    AutoPtr<IMediaDrmProvisionRequest> request;
    mMediaDrm->GetProvisionRequest((IMediaDrmProvisionRequest**)&request);
    AutoPtr<PostRequestTask> postTask = new PostRequestTask(request.getData());
    String url;
    request->GetDefaultUrl(&url);
    postTask->Execute(url);
}

/**
 * Called when the provision response is received.
 *
 * @param response Response data from the provision server.
 */
void MediaDrmBridge::OnProvisionResponse(
    /* [in] */ ArrayOf<Byte>* response)
{
//    Log.d(TAG, "onProvisionResponse()");
    assert(mProvisioningPending);
    mProvisioningPending = FALSE;

    // If |mMediaDrm| is released, there is no need to callback native.
    if (mMediaDrm == NULL) {
        return;
    }

    Boolean success = ProvideProvisionResponse(response);

    if (mResetDeviceCredentialsPending) {
        NativeOnResetDeviceCredentialsCompleted(mNativeMediaDrmBridge, success);
        mResetDeviceCredentialsPending = FALSE;
    }

    if (success) {
        ResumePendingOperations();
    }
}

/**
 * Provide the provisioning response to MediaDrm.
 * @returns false if the response is invalid or on error, true otherwise.
 */
Boolean MediaDrmBridge::ProvideProvisionResponse(
    /* [in] */ ArrayOf<Byte>* response)
{
    if (response == NULL || response->GetLength() == 0) {
//        Log.e(TAG, "Invalid provision response.");
        return FALSE;
    }

    // try {
        mMediaDrm->ProvideProvisionResponse(response);
        return TRUE;
    // } catch (android.media.DeniedByServerException e) {
    //     Log.e(TAG, "failed to provide provision response", e);
    // } catch (java.lang.IllegalStateException e) {
    //     Log.e(TAG, "failed to provide provision response", e);
    // }

    return FALSE;
}

void MediaDrmBridge::OnSessionCreated(
    /* [in] */ Int32 sessionId,
    /* [in] */ const String& webSessionId)
{
    AutoPtr<IRunnable> runnable = new OnSessionCreatedRunnable(this, sessionId, webSessionId);
    mHandler->Post(runnable);
}

void MediaDrmBridge::OnSessionMessage(
    /* [in] */ Int32 sessionId,
    /* [in] */ IMediaDrmKeyRequest* request)
{
    AutoPtr<IRunnable> runnable = new OnSessionMessageRunnable(this, sessionId, request);
    mHandler->Post(runnable);
}

void MediaDrmBridge::OnSessionReady(
    /* [in] */ Int32 sessionId)
{
    AutoPtr<IRunnable> runnable = new OnSessionReadyRunnable(this, sessionId);
    mHandler->Post(runnable);
}

void MediaDrmBridge::OnSessionClosed(
    /* [in] */ Int32 sessionId)
{
    AutoPtr<IRunnable> runnable = new OnSessionClosedRunnable(this, sessionId);
    mHandler->Post(runnable);
}

void MediaDrmBridge::OnSessionError(
    /* [in] */ Int32 sessionId)
{
    // TODO(qinmin): pass the error code to native.
    AutoPtr<IRunnable> runnable = new OnSessionErrorRunnable(this, sessionId);
    mHandler->Post(runnable);
}

String MediaDrmBridge::GetWebSessionId(
    /* [in] */ IByteBuffer* session)
{
    String webSessionId = null;
    // try {
        String webSessionId = new String(session.array(), "UTF-8");
    // } catch (java.io.UnsupportedEncodingException e) {
    //     Log.e(TAG, "getWebSessionId failed", e);
    // } catch (java.lang.NullPointerException e) {
    //     Log.e(TAG, "getWebSessionId failed", e);
    // }

    return webSessionId;
}

void MediaDrmBridge::AddKeySystemUuidMapping(
    /* [in] */ String keySystem,
    /* [in] */ IUUID* uuid)
{
    AutoPtr<IByteBuffer> uuidBuffer;
    ByteBuffer::AllocateDirect(16, (IByteBuffer**)&uuidBuffer);
    // MSB (byte) should be positioned at the first element.
    uuidBuffer->Order(IByteOrder::BIG_ENDIAN);
    Int64 mostBits;
    uuid->GetMostSignificantBits(&mostBits);
    uuidBuffer->PutInt64(mostBits);
    Int64 leastBits;
    uuid->GetLeastSignificantBits(&leastBits);
    uuidBuffer->PutInt64(leastBits);
    NativeAddKeySystemUuidMapping(keySystem, uuidBuffer);
}

void MediaDrmBridge::NativeOnMediaCryptoReady(
    /* [in] */ Int64 nativeMediaDrmBridge)
{
}

void MediaDrmBridge::NativeOnSessionCreated(
    /* [in] */ Int64 nativeMediaDrmBridge,
    /* [in] */ Int32 sessionId,
    /* [in] */ String webSessionId)
{
}

void MediaDrmBridge::NativeOnSessionMessage(
    /* [in] */ Int64 nativeMediaDrmBridge,
    /* [in] */ Int32 sessionId,
    /* [in] */ ArrayOf<Byte>* message,
    /* [in] */ String destinationUrl)
{
}

void MediaDrmBridge::NativeOnSessionReady(
    /* [in] */ Int64 nativeMediaDrmBridge,
    /* [in] */ Int32 sessionId)
{
}

void MediaDrmBridge::NativeOnSessionClosed(
    /* [in] */ Int64 nativeMediaDrmBridge,
    /* [in] */ Int32 sessionId)
{
}

void MediaDrmBridge::NativeOnSessionError(
    /* [in] */ Int64 nativeMediaDrmBridge,
    /* [in] */ Int32 sessionId)
{
}

void MediaDrmBridge::NativeOnResetDeviceCredentialsCompleted(
    /* [in] */ Int64 nativeMediaDrmBridge,
    /* [in] */ Boolean success)
{
}

void MediaDrmBridge::NativeAddKeySystemUuidMapping(
    /* [in] */ String keySystem,
    /* [in] */ IByteBuffer* uuid)
{
}

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
