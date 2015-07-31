
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//===============================================================
//         ChildProcessConnectionImpl::ConnectionParams
//===============================================================

ChildProcessConnectionImpl::ConnectionParams::ConnectionParams(
    /* [in] */ ArrayOf<String>* commandLine,
    /* [in] */ ArrayOf<FileDescriptorInfo>* filesToBeMapped,
    /* [in] */ IChildProcessCallback* callback,
    /* [in] */ IBundle* sharedRelros)
    : mCommandLine(commandLine)
    , mFilesToBeMapped(filesToBeMapped)
    , mCallback(callback)
    , mSharedRelros(sharedRelros)
{
}

//===============================================================
//      ChildProcessConnectionImpl::ChildServiceConnection
//===============================================================

ChildProcessConnectionImpl::ChildServiceConnection::ChildServiceConnection(
    /* [in] */ ChildProcessConnectionImpl* owner,
    /* [in] */ Int32 bindFlags)
    : mBound(FALSE)
    , mBindFlags(bindFlags)
{
}

AutoPtr<IIntent> ChildProcessConnectionImpl::ChildServiceConnection::CreateServiceBindIntent()
{
    AutoPtr<IIntent> intent;
    CIntent::New((IIntent**)&intent);
    String name;
    mServiceClass->GetName(&name);
    name += mServiceNumber;
    intent->SetClassName(mContext,  name);
    String pkgName;
    mContext->GetPackageName(&pkgName);
    intent->SetPackage(pkgName);
    return intent;
}

Boolean ChildProcessConnectionImpl::ChildServiceConnection::Bind(
    /* [in] */ ArrayOf<String>* commandLine)
{
    if (!mBound) {
        TraceEvent::Begin();
        const AutoPtr<IIntent> intent = CreateServiceBindIntent();
        if (commandLine != NULL) {
            intent->PutExtra(EXTRA_COMMAND_LINE, commandLine);
        }

        if (mLinkerParams != NULL) {
            mLinkerParams->AddIntentExtras(intent);
        }

        mContext->BindService(intent, this, mBindFlags, &mBound);
        TraceEvent::End();
    }

    return mBound;
}

void ChildProcessConnectionImpl::ChildServiceConnection::Unbind()
{
    if (mBound) {
        mContext->UnbindService(this);
        mBound = FALSE;
    }
}

Boolean ChildProcessConnectionImpl::ChildServiceConnection::IsBound()
{
    return mBound;
}

//@Override
void ChildProcessConnectionImpl::ChildServiceConnection::OnServiceConnected(
    /* [in] */ IComponentName* className,
    /* [in] */ IBinder* service)
{
    Mutex::Autolock lock(mLock);

    // A flag from the parent class ensures we run the post-connection logic only once
    // (instead of once per each ChildServiceConnection).
    if (mServiceConnectComplete) {
        return;
    }

    TraceEvent::Begin();
    mServiceConnectComplete = TRUE;
    mService = IChildProcessService.Stub.asInterface(service);
    // Run the setup if the connection parameters have already been provided. If not,
    // doConnectionSetupLocked() will be called from setupConnection().
    if (mConnectionParams != NULL) {
        DoConnectionSetupLocked();
    }
    TraceEvent::End();
}


// Called on the main thread to notify that the child service did not disconnect gracefully.
//@Override
void ChildProcessConnectionImpl::ChildServiceConnection::OnServiceDisconnected(
    /* [in] */ IComponentName* className)
{
    Mutex::Autolock lock(mLock);

    // Ensure that the disconnection logic runs only once (instead of once per each
    // ChildServiceConnection).
    if (mServiceDisconnected) {
        return;
    }

    mServiceDisconnected = TRUE;
    // Stash the status of the oom bindings, since stop() will release all bindings.
    mWasOomProtected = mInitialBinding->IsBound() || mStrongBinding->IsBound();
//    Log.w(TAG, "onServiceDisconnected (crash or killed by oom): pid=" + mPid);
    Stop();  // We don't want to auto-restart on crash. Let the browser do that.
    mDeathCallback->OnChildProcessDied(ChildProcessConnectionImpl.this);
    // If we have a pending connection callback, we need to communicate the failure to
    // the caller.
    if (mConnectionCallback != NULL) {
        mConnectionCallback->OnConnected(0);
    }

    mConnectionCallback = NULL;
}

//===============================================================
//                  ChildProcessConnectionImpl
//===============================================================

const String ChildProcessConnectionImpl::TAG("ChildProcessConnection");

ChildProcessConnectionImpl::ChildProcessConnectionImpl(
    /* [in] */ IContext* context,
    /* [in] */ Int32 number,
    /* [in] */ Boolean inSandbox,
    /* [in] */ ChildProcessConnection::DeathCallback* deathCallback,
    /* [in] */ ChildProcessService* serviceClass,
    /* [in] */ ChromiumLinkerParams* chromiumLinkerParams)
    : mServiceConnectComplete(FALSE)
    , mServiceDisconnected(FALSE)
    , mWasOomProtected(FALSE)
    , mPid(0)
    , mStrongBindingCount(0)
    , mContext(context)
    , mServiceNumber(number)
    , mInSandbox(inSandbox)
    , mDeathCallback(deathCallback)
    , mServiceClass(serviceClass)
    , mLinkerParams(chromiumLinkerParams)
{
    mInitialBinding = new ChildServiceConnection(IContext::BIND_AUTO_CREATE);
    mStrongBinding = new ChildServiceConnection(
            IContext::BIND_AUTO_CREATE | IContext::BIND_IMPORTANT);
    mWaivedBinding = new ChildServiceConnection(
            IContext::BIND_AUTO_CREATE | IContext::BIND_WAIVE_PRIORITY);
}

//@Override
Int32 ChildProcessConnectionImpl::GetServiceNumber()
{
    return mServiceNumber;
}

//@Override
Boolean ChildProcessConnectionImpl::IsInSandbox()
{
    return mInSandbox;
}

//@Override
AutoPtr<IChildProcessService> ChildProcessConnectionImpl::GetService()
{
    Object::Autolock lock(mLock);
    return mService;
}

//@Override
Int32 ChildProcessConnectionImpl::GetPid()
{
    Object::Autolock lock(mLock);
    return mPid;
}

//@Override
void ChildProcessConnectionImpl::Start(
    /* [in] */ ArrayOf<String>* commandLine)
{
    Object::Autolock lock(mLock);

    TraceEvent::Begin();
    assert(!ThreadUtils::RunningOnUiThread());
//    assert mConnectionParams == null :
//            "setupConnection() called before start() in ChildProcessConnectionImpl.";

    if (!mInitialBinding->Bind(commandLine)) {
//        Log.e(TAG, "Failed to establish the service connection.");
        // We have to notify the caller so that they can free-up associated resources.
        // TODO(ppi): Can we hard-fail here?
        mDeathCallback->OnChildProcessDied(/*ChildProcessConnectionImpl.*/this);
    }
    else {
        mWaivedBinding->Bind(NULL);
    }
    TraceEvent::End();
}

//@Override
void ChildProcessConnectionImpl::SetupConnection(
    /* [in] */ ArrayOf<String>* commandLine,
    /* [in] */ ArrayOf<FileDescriptorInfo>* filesToBeMapped,
    /* [in] */ IChildProcessCallback* processCallback,
    /* [in] */ ConnectionCallback* connectionCallback,
    /* [in] */ IBundle* sharedRelros)
{
    Object::Autolock lock(mLock);

    assert(mConnectionParams == NULL);
    if (mServiceDisconnected) {
//        Log.w(TAG, "Tried to setup a connection that already disconnected.");
        connectionCallback->OnConnected(0);
        return;
    }

    TraceEvent::Begin();
    mConnectionCallback = connectionCallback;
    mConnectionParams = new ConnectionParams(
            commandLine, filesToBeMapped, processCallback, sharedRelros);
    // Run the setup if the service is already connected. If not, doConnectionSetupLocked()
    // will be called from onServiceConnected().
    if (mServiceConnectComplete) {
        DoConnectionSetupLocked();
    }
    TraceEvent::End();
}

//@Override
void ChildProcessConnectionImpl::Stop()
{
    Object::Autolock lock(mLock);

    mInitialBinding->Unbind();
    mStrongBinding->Unbind();
    mWaivedBinding->Unbind();
    mStrongBindingCount = 0;
    if (mService != NULL) {
        mService = NULL;
    }
    mConnectionParams = NULL;
}

/**
 * Called after the connection parameters have been set (in setupConnection()) *and* a
 * connection has been established (as signaled by onServiceConnected()). These two events can
 * happen in any order. Has to be called with mLock.
 */
void ChildProcessConnectionImpl::DoConnectionSetupLocked()
{
    TraceEvent::Begin();
    assert(mServiceConnectComplete && mService != NULL);
    assert(mConnectionParams != NULL);

    AutoPtr<IBundle> bundle;
    CBundle::New((IBundle**)&bundle);
    bundle->PutStringArray(EXTRA_COMMAND_LINE, mConnectionParams->mCommandLine);

    AutoPtr< ArrayOf<FileDescriptorInfo> > fileInfos = mConnectionParams->mFilesToBeMapped;
    AutoPtr< ArrayOf<ParcelFileDescriptor> > parcelFiles = ArrayOf<ParcelFileDescriptor>::Alloc(fileInfos->GetLength());
    for (Int32 i = 0; i < fileInfos->GetLength(); i++) {
        if ((*fileInfos)[i]->mFd == -1) {
            // If someone provided an invalid FD, they are doing something wrong.
//            Log.e(TAG, "Invalid FD (id=" + fileInfos[i].mId + ") for process connection, "
//                  + "aborting connection.");
            return;
        }
        String idName = EXTRA_FILES_PREFIX + i + EXTRA_FILES_ID_SUFFIX;
        String fdName = EXTRA_FILES_PREFIX + i + EXTRA_FILES_FD_SUFFIX;
        if ((*fileInfos)[i]->mAutoClose) {
            // Adopt the FD, it will be closed when we close the ParcelFileDescriptor.
            (*parcelFiles)[i] = ParcelFileDescriptor::AdoptFd((*fileInfos)[i]->mFd);
        } else {
            // try {
                (*parcelFiles)[i] = ParcelFileDescriptor::FromFd((*fileInfos)[i]->mFd);
            // } catch (IOException e) {
            //     Log.e(TAG,
            //           "Invalid FD provided for process connection, aborting connection.",
            //           e);
            //     return;
            // }
        }
        bundle->PutParcelable(fdName, (*parcelFiles)[i]);
        bundle->PutInt(idName, (*fileInfos)[i]->mId);
    }
    // Add the CPU properties now.
    bundle->PutInt32(EXTRA_CPU_COUNT, CpuFeatures::GetCount());
    bundle->PutInt64(EXTRA_CPU_FEATURES, CpuFeatures::GetMask());

    bundle->PutBundle(Linker::EXTRA_LINKER_SHARED_RELROS,
                     mConnectionParams->mSharedRelros);

    // try {
        mService->SetupConnection(bundle, mConnectionParams->mCallback, &mPid);
//        assert mPid != 0 : "Child service claims to be run by a process of pid=0.";
    // } catch (android.os.RemoteException re) {
    //     Log.e(TAG, "Failed to setup connection.", re);
    // }

    // We proactively close the FDs rather than wait for GC & finalizer.
    // try {
        for (ParcelFileDescriptor parcelFile : parcelFiles) {
            if (parcelFile != NULL) parcelFile->Close();
        }
    // } catch (IOException ioe) {
    //     Log.w(TAG, "Failed to close FD.", ioe);
    // }
    mConnectionParams = NULL;

    if (mConnectionCallback != NULL) {
        mConnectionCallback->OnConnected(mPid);
    }

    mConnectionCallback = NULL;
    TraceEvent::End();
}

//@Override
Boolean ChildProcessConnectionImpl::IsInitialBindingBound()
{
    Object::Autolock lock(mLock);
    return mInitialBinding.isBound();
}

//@Override
Boolean ChildProcessConnectionImpl::IsStrongBindingBound()
{
    Object::Autolock lock(mLock);
    return mStrongBinding->IsBound();
}

//@Override
void ChildProcessConnectionImpl::RemoveInitialBinding()
{
    Object::Autolock lock(mLock);
    mInitialBinding->Unbind();
}

//@Override
Boolean ChildProcessConnectionImpl::IsOomProtectedOrWasWhenDied()
{
    Object::Autolock lock(mLock);
    if (mServiceDisconnected) {
        return mWasOomProtected;
    }
    else {
        return mInitialBinding->IsBound() || mStrongBinding->IsBound();
    }
}

//@Override
void ChildProcessConnectionImpl::DropOomBindings()
{
    Object::Autolock lock(mLock);

    mInitialBinding->Unbind();

    mStrongBindingCount = 0;
    mStrongBinding->Unbind();
}

//@Override
void ChildProcessConnectionImpl::AddStrongBinding()
{
    Object::Autolock lock(mLock);
    if (mService == NULL) {
//        Log.w(TAG, "The connection is not bound for " + mPid);
        return;
    }

    if (mStrongBindingCount == 0) {
        mStrongBinding->Bind(NULL);
    }

    mStrongBindingCount++;
}

//@Override
void ChildProcessConnectionImpl::RemoveStrongBinding()
{
    Object::Autolock lock(mLock);

    if (mService == NULL) {
//        Log.w(TAG, "The connection is not bound for " + mPid);
        return;
    }

    assert(mStrongBindingCount > 0);
    mStrongBindingCount--;
    if (mStrongBindingCount == 0) {
        mStrongBinding->Unbind();
    }
}

//@VisibleForTesting
Boolean ChildProcessConnectionImpl::CrashServiceForTesting()
{
    // try {
        mService->CrashIntentionallyForTesting();
    // } catch (DeadObjectException e) {
    //     return true;
    // }

    return FALSE;
}

//@VisibleForTesting
Boolean ChildProcessConnectionImpl::IsConnected()
{
    return mService != NULL;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
