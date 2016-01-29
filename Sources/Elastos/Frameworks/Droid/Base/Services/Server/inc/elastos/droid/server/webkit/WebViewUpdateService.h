/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.server.webkit;

using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Utility::ISlog;
using Elastos::Droid::Webkit::IIWebViewUpdateService;
using Elastos::Droid::Webkit::IWebViewFactory;

using Elastos::Droid::Server::ISystemService;

/**
 * Private service to wait for the updatable WebView to be ready for use.
 * @hide
 */
public class WebViewUpdateService extends SystemService {

    private static const String TAG = "WebViewUpdateService";
    private static const Int32 WAIT_TIMEOUT_MS = 5000; // Same as KEY_DISPATCHING_TIMEOUT.

    private Boolean mRelroReady32Bit = FALSE;
    private Boolean mRelroReady64Bit = FALSE;

    private BroadcastReceiver mWebViewUpdatedReceiver;

    public WebViewUpdateService(Context context) {
        Super(context);
    }

    //@Override
    CARAPI OnStart() {
        mWebViewUpdatedReceiver = new BroadcastReceiver() {
                //@Override
                CARAPI OnReceive(Context context, Intent intent) {
                    String webviewPackage = "package:" + WebViewFactory->GetWebViewPackageName();
                    if (webviewPackage->Equals(intent->GetDataString())) {
                        OnWebViewUpdateInstalled();
                    }
                }
        };
        IntentFilter filter = new IntentFilter();
        filter->AddAction(IIntent::ACTION_PACKAGE_REPLACED);
        filter->AddDataScheme("package");
        GetContext()->RegisterReceiver(mWebViewUpdatedReceiver, filter);

        PublishBinderService("webviewupdate", new BinderService());
    }

    private void OnWebViewUpdateInstalled() {
        Slogger::D(TAG, "WebView Package updated!");

        synchronized(this) {
            mRelroReady32Bit = FALSE;
            mRelroReady64Bit = FALSE;
        }
        WebViewFactory->OnWebViewUpdateInstalled();
    }

    private class BinderService extends IWebViewUpdateService.Stub {

        /**
         * The shared relro process calls this to notify us that it's done trying to create a relro
         * file. This method gets called even if the relro creation has failed or the process
         * crashed.
         */
        //@Override // Binder call
        CARAPI NotifyRelroCreationCompleted(Boolean is64Bit, Boolean success) {
            // Verify that the caller is either the shared relro process (nominal case) or the
            // system server (only in the case the relro process crashes and we get here via the
            // crashHandler).
            if (Binder->GetCallingUid() != Process.SHARED_RELRO_UID &&
                    Binder->GetCallingUid() != Process.SYSTEM_UID) {
                return;
            }

            synchronized(WebViewUpdateService.this) {
                if (is64Bit) {
                    mRelroReady64Bit = TRUE;
                } else {
                    mRelroReady32Bit = TRUE;
                }
                WebViewUpdateService.this->NotifyAll();
            }
        }

        /**
         * WebViewFactory calls this to block WebView loading until the relro file is created.
         */
        //@Override // Binder call
        CARAPI WaitForRelroCreationCompleted(Boolean is64Bit) {
            // The WebViewUpdateService depends on the prepareWebViewInSystemServer call, which
            // happens later (during the PHASE_ACTIVITY_MANAGER_READY) in SystemServer.java. If
            // another service there tries to bring up a WebView in the between, the wait below
            // would deadlock without the check below.
            if (Binder->GetCallingPid() == Process->MyPid()) {
                throw new IllegalStateException("Cannot create a WebView from the SystemServer");
            }

            final Int64 NS_PER_MS = 1000000;
            final Int64 timeoutTimeMs = System->NanoTime() / NS_PER_MS + WAIT_TIMEOUT_MS;
            Boolean relroReady = (is64Bit ? mRelroReady64Bit : mRelroReady32Bit);
            synchronized(WebViewUpdateService.this) {
                while (!relroReady) {
                    final Int64 timeNowMs = System->NanoTime() / NS_PER_MS;
                    if (timeNowMs >= timeoutTimeMs) break;
                    try {
                        WebViewUpdateService.this->Wait(timeoutTimeMs - timeNowMs);
                    } catch (InterruptedException e) {}
                    relroReady = (is64Bit ? mRelroReady64Bit : mRelroReady32Bit);
                }
            }
            if (!relroReady) Slogger::W(TAG, "creating relro file timed out");
        }
    }

}
