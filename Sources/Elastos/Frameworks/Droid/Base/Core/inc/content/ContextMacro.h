
#ifndef __CONTEXTMACRO_H__
#define __CONTEXTMACRO_H__

#define ICONTEXT_METHODS_DECL()                                                                 \
    CARAPI GetAssets(                                                                           \
        /* [out] */ IAssetManager** assetManager);                                              \
                                                                                                \
    CARAPI GetResources(                                                                        \
        /* [out] */ IResources** resources);                                                    \
                                                                                                \
    CARAPI GetPackageManager(                                                                   \
        /* [out] */ IPackageManager** packageManager);                                          \
                                                                                                \
    CARAPI GetContentResolver(                                                                  \
        /* [out] */ IContentResolver** resolver);                                               \
                                                                                                \
    CARAPI GetMainLooper(                                                                       \
        /* [out] */ ILooper** looper);                                                          \
                                                                                                \
    CARAPI GetApplicationContext(                                                               \
        /* [out] */ IContext** ctx);                                                            \
                                                                                                \
    CARAPI RegisterComponentCallbacks(                                                          \
        /* [in] */ IComponentCallbacks* componentCallback);                                     \
                                                                                                \
    CARAPI UnregisterComponentCallbacks(                                                        \
        /* [in] */ IComponentCallbacks* componentCallback);                                     \
                                                                                                \
    CARAPI GetText(                                                                             \
        /* [in] */ Int32 resId,                                                                 \
        /* [out] */ ICharSequence** text);                                                      \
                                                                                                \
    CARAPI GetString(                                                                           \
        /* [in] */ Int32 resId,                                                                 \
        /* [out] */ String* str);                                                               \
                                                                                                \
    CARAPI GetStringEx(                                                                         \
        /* [in] */ Int32 resId,                                                                 \
        /* [in] */ ArrayOf<IInterface*>* formatArgs,                                            \
        /* [out] */ String* str);                                                               \
                                                                                                \
    CARAPI SetTheme(                                                                            \
        /* [in] */ Int32 resId);                                                                \
                                                                                                \
    CARAPI GetThemeResId(                                                                       \
        /* [out] */ Int32* resId);                                                              \
                                                                                                \
    CARAPI GetTheme(                                                                            \
        /* [out] */ IResourcesTheme** theme);                                                   \
                                                                                                \
    CARAPI ObtainStyledAttributes(                                                              \
        /* [in] */ ArrayOf<Int32>* attrs,                                                       \
        /* [out] */ ITypedArray** styles);                                                      \
                                                                                                \
    CARAPI ObtainStyledAttributesEx(                                                            \
        /* [in] */ Int32 resid,                                                                 \
        /* [in] */ ArrayOf<Int32>* attrs,                                                       \
        /* [out] */ ITypedArray** styles);                                                      \
                                                                                                \
    CARAPI ObtainStyledAttributesEx2(                                                           \
        /* [in] */ IAttributeSet* set,                                                          \
        /* [in] */ ArrayOf<Int32>* attrs,                                                       \
        /* [out] */ ITypedArray** styles);                                                      \
                                                                                                \
    CARAPI ObtainStyledAttributesEx3(                                                           \
        /* [in] */ IAttributeSet* set,                                                          \
        /* [in] */ ArrayOf<Int32>* attrs,                                                       \
        /* [in] */ Int32 defStyleAttr,                                                          \
        /* [in] */ Int32 defStyleRes,                                                           \
        /* [out] */ ITypedArray** styles);                                                      \
                                                                                                \
    CARAPI GetClassLoader(                                                                      \
        /* [out] */ IClassLoader** loader);                                                     \
                                                                                                \
    CARAPI GetPackageName(                                                                      \
        /* [out] */ String* packageName);                                                       \
                                                                                                \
    CARAPI GetApplicationInfo(                                                                  \
        /* [out] */ IApplicationInfo** info);                                                   \
                                                                                                \
    CARAPI GetPackageResourcePath(                                                              \
        /* [out] */ String* path);                                                              \
                                                                                                \
    CARAPI GetPackageCodePath(                                                                  \
        /* [out] */ String* codePath);                                                          \
                                                                                                \
    CARAPI GetSharedPrefsFile(                                                                  \
        /* [in] */ const String& name,                                                          \
        /* [out] */ IFile** file);                                                              \
                                                                                                \
    CARAPI GetSharedPreferences(                                                                \
        /* [in] */ const String& name,                                                          \
        /* [in] */ Int32 mode,                                                                  \
        /* [out] */ ISharedPreferences** prefs);                                                \
                                                                                                \
    CARAPI OpenFileInput(                                                                       \
        /* [in] */ const String& name,                                                          \
        /* [out] */ IFileInputStream** fileInputStream);                                        \
                                                                                                \
    CARAPI OpenFileOutput(                                                                      \
        /* [in] */ const String& name,                                                          \
        /* [in] */ Int32 mode,                                                                  \
        /* [out] */ IFileOutputStream** fileOutputStream);                                      \
                                                                                                \
    CARAPI DeleteFile(                                                                          \
        /* [in] */ const String& name,                                                          \
        /* [out] */ Boolean* succeeded);                                                        \
                                                                                                \
    CARAPI GetFileStreamPath(                                                                   \
        /* [in] */ const String& name,                                                          \
        /* [out] */ IFile** file);                                                              \
                                                                                                \
    CARAPI GetFilesDir(                                                                         \
        /* [out] */ IFile** filesDir);                                                          \
                                                                                                \
    CARAPI GetExternalFilesDir(                                                                 \
        /* [in] */ const String& type,                                                          \
        /* [out] */ IFile** filesDir);                                                          \
                                                                                                \
    CARAPI GetObbDir(                                                                           \
        /* [out] */ IFile** obbDir);                                                            \
                                                                                                \
    CARAPI GetCacheDir(                                                                         \
        /* [out] */ IFile** cacheDir);                                                          \
                                                                                                \
    CARAPI GetExternalCacheDir(                                                                 \
        /* [out] */ IFile** externalDir);                                                       \
                                                                                                \
    CARAPI GetFileList(                                                                         \
        /* [out, callee] */ ArrayOf<String>** fileList);                                        \
                                                                                                \
    CARAPI GetDir(                                                                              \
        /* [in] */ const String& name,                                                          \
        /* [in] */ Int32 mode,                                                                  \
        /* [out] */ IFile** dir);                                                               \
                                                                                                \
    CARAPI OpenOrCreateDatabase(                                                                \
        /* [in] */ const String& name,                                                          \
        /* [in] */ Int32 mode,                                                                  \
        /* [in] */ ISQLiteDatabaseCursorFactory* factory,                                       \
        /* [out] */ ISQLiteDatabase** sqliteDB);                                                \
                                                                                                \
    CARAPI OpenOrCreateDatabaseEx(                                                              \
        /* [in] */ const String& name,                                                          \
        /* [in] */ Int32 mode,                                                                  \
        /* [in] */ ISQLiteDatabaseCursorFactory* factory,                                       \
        /* [in] */ IDatabaseErrorHandler* errorHandler,                                         \
        /* [out] */ ISQLiteDatabase** sqliteDB);                                                \
                                                                                                \
    CARAPI DeleteDatabase(                                                                      \
        /* [in] */ const String& name,                                                          \
        /* [out] */ Boolean* succeeded);                                                        \
                                                                                                \
    CARAPI GetDatabasePath(                                                                     \
        /* [in] */ const String& name,                                                          \
        /* [out] */ IFile** path);                                                              \
                                                                                                \
    CARAPI GetDatabaseList(                                                                     \
        /* [out, callee] */ ArrayOf<String>** databaseList);                                    \
                                                                                                \
    CARAPI GetWallpaper(                                                                        \
        /* [out] */ IDrawable** drawable);                                                      \
                                                                                                \
    CARAPI PeekWallpaper(                                                                       \
        /* [out] */ IDrawable** drawable);                                                      \
                                                                                                \
    CARAPI GetWallpaperDesiredMinimumWidth(                                                     \
        /* [out] */ Int32* minWidth);                                                           \
                                                                                                \
    CARAPI GetWallpaperDesiredMinimumHeight(                                                    \
        /* [out] */ Int32* minHeight);                                                          \
                                                                                                \
    CARAPI SetWallpaper(                                                                        \
        /* [in] */ IBitmap* bitmap);                                                            \
                                                                                                \
    CARAPI SetWallpaperEx(                                                                      \
        /* [in] */ IInputStream* data);                                                         \
                                                                                                \
    CARAPI ClearWallpaper();                                                                    \
                                                                                                \
    CARAPI StartActivity(                                                                       \
        /* [in] */ IIntent* intent);                                                            \
                                                                                                \
    CARAPI StartActivityAsUser(                                                                 \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ IUserHandle* user);                                                          \
                                                                                                \
    CARAPI StartActivityEx(                                                                     \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ IBundle* options);                                                           \
                                                                                                \
    CARAPI StartActivityAsUserEx(                                                               \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ IBundle* options,                                                            \
        /* [in] */ IUserHandle* userId);                                                        \
                                                                                                \
    CARAPI StartActivities(                                                                     \
        /* [in] */ ArrayOf<IIntent*>* intents);                                                 \
                                                                                                \
    CARAPI StartActivitiesEx(                                                                   \
        /* [in] */ ArrayOf<IIntent*>* intents,                                                  \
        /* [in] */ IBundle* options);                                                           \
                                                                                                \
    CARAPI StartActivitiesAsUser(                                                               \
        /* [in] */ ArrayOf<IIntent*>* intents,                                                  \
        /* [in] */ IBundle* options,                                                            \
        /* [in] */ IUserHandle* userHandle);                                                    \
                                                                                                \
    CARAPI StartIntentSender(                                                                   \
        /* [in] */ IIntentSender* intent,                                                       \
        /* [in] */ IIntent* fillInIntent,                                                       \
        /* [in] */ Int32 flagsMask,                                                             \
        /* [in] */ Int32 flagsValues,                                                           \
        /* [in] */ Int32 extraFlags);                                                           \
                                                                                                \
    CARAPI StartIntentSenderEx(                                                                 \
        /* [in] */ IIntentSender* intent,                                                       \
        /* [in] */ IIntent* fillInIntent,                                                       \
        /* [in] */ Int32 flagsMask,                                                             \
        /* [in] */ Int32 flagsValues,                                                           \
        /* [in] */ Int32 extraFlags,                                                            \
        /* [in] */ IBundle* options);                                                           \
                                                                                                \
    CARAPI SendBroadcast(                                                                       \
        /* [in] */ IIntent* intent);                                                            \
                                                                                                \
    CARAPI SendBroadcastEx(                                                                     \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ const String& receiverPermission);                                           \
                                                                                                \
    CARAPI SendOrderedBroadcast(                                                                \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ const String& receiverPermission);                                           \
                                                                                                \
    CARAPI SendOrderedBroadcastEx(                                                              \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ const String& receiverPermission,                                            \
        /* [in] */ IBroadcastReceiver* resultReceiver,                                          \
        /* [in] */ IHandler* scheduler,                                                         \
        /* [in] */ Int32 initialCode,                                                           \
        /* [in] */ const String& initialData,                                                   \
        /* [in] */ IBundle* initialExtras);                                                     \
                                                                                                \
    CARAPI SendBroadcastAsUser(                                                                 \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ IUserHandle* user);                                                          \
                                                                                                \
    CARAPI SendBroadcastAsUserEx(                                                               \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ IUserHandle* user,                                                           \
        /* [in] */ const String& receiverPermission);                                           \
                                                                                                \
    CARAPI SendOrderedBroadcastAsUser(                                                          \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ IUserHandle* user,                                                           \
        /* [in] */ const String& receiverPermission,                                            \
        /* [in] */ IBroadcastReceiver* resultReceiver,                                          \
        /* [in] */ IHandler* scheduler,                                                         \
        /* [in] */ Int32 initialCode,                                                           \
        /* [in] */ const String& initialData,                                                   \
        /* [in] */ IBundle* initialExtras);                                                     \
                                                                                                \
    CARAPI SendStickyBroadcast(                                                                 \
        /* [in] */ IIntent* intent);                                                            \
                                                                                                \
    CARAPI SendStickyOrderedBroadcast(                                                          \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ IBroadcastReceiver* resultReceiver,                                          \
        /* [in] */ IHandler* scheduler,                                                         \
        /* [in] */ Int32 initialCode,                                                           \
        /* [in] */ const String& initialData,                                                   \
        /* [in] */ IBundle* initialExtras);                                                     \
                                                                                                \
    CARAPI RemoveStickyBroadcast(                                                               \
        /* [in] */ IIntent* intent);                                                            \
                                                                                                \
    CARAPI SendStickyBroadcastAsUser(                                                           \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ IUserHandle* user);                                                          \
                                                                                                \
    CARAPI SendStickyOrderedBroadcastAsUser(                                                    \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ IUserHandle* user,                                                           \
        /* [in] */ IBroadcastReceiver* resultReceiver,                                          \
        /* [in] */ IHandler* scheduler,                                                         \
        /* [in] */ Int32 initialCode,                                                           \
        /* [in] */ const String& initialData,                                                   \
        /* [in] */ IBundle* initialExtras);                                                     \
                                                                                                \
    CARAPI RemoveStickyBroadcastAsUser(                                                         \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ IUserHandle* user);                                                          \
                                                                                                \
    CARAPI RegisterReceiver(                                                                    \
        /* [in] */ IBroadcastReceiver* receiver,                                                \
        /* [in] */ IIntentFilter* filter,                                                       \
        /* [out] */ IIntent** stickyIntent);                                                    \
                                                                                                \
    CARAPI RegisterReceiverEx(                                                                  \
        /* [in] */ IBroadcastReceiver* receiver,                                                \
        /* [in] */ IIntentFilter* filter,                                                       \
        /* [in] */ const String& broadcastPermission,                                           \
        /* [in] */ IHandler* scheduler,                                                         \
        /* [out] */ IIntent** stickyIntent);                                                    \
                                                                                                \
    CARAPI RegisterReceiverAsUser(                                                              \
        /* [in] */ IBroadcastReceiver* receiver,                                                \
        /* [in] */ IUserHandle* user,                                                           \
        /* [in] */ IIntentFilter* filter,                                                       \
        /* [in] */ const String& broadcastPermission,                                           \
        /* [in] */ IHandler* scheduler,                                                         \
        /* [out] */ IIntent** stickyIntent);                                                    \
                                                                                                \
    CARAPI UnregisterReceiver(                                                                  \
        /* [in] */ IBroadcastReceiver* receiver);                                               \
                                                                                                \
    CARAPI StartService(                                                                        \
        /* [in] */ IIntent* service,                                                            \
        /* [out] */ IComponentName** name);                                                     \
                                                                                                \
    CARAPI StopService(                                                                         \
        /* [in] */ IIntent* service,                                                            \
        /* [out] */ Boolean* succeeded);                                                        \
                                                                                                \
    CARAPI StartServiceAsUser(                                                                  \
        /* [in] */ IIntent* service,                                                            \
        /* [in] */ IUserHandle* user,                                                           \
        /* [out] */ IComponentName** name);                                                     \
                                                                                                \
    CARAPI StopServiceAsUser(                                                                   \
        /* [in] */ IIntent* service,                                                            \
        /* [in] */ IUserHandle* user,                                                           \
        /* [out] */ Boolean* succeeded);                                                        \
                                                                                                \
    CARAPI BindService(                                                                         \
        /* [in] */ IIntent* service,                                                            \
        /* [in] */ IServiceConnection* conn,                                                    \
        /* [in] */ Int32 flags,                                                                 \
        /* [out] */ Boolean* succeeded);                                                        \
                                                                                                \
    CARAPI BindServiceEx(                                                                       \
        /* [in] */ IIntent* service,                                                            \
        /* [in] */ IServiceConnection* conn,                                                    \
        /* [in] */ Int32 flags,                                                                 \
        /* [in] */ Int32 userHandle,                                                            \
        /* [out] */ Boolean* succeeded);                                                        \
                                                                                                \
    CARAPI UnbindService(                                                                       \
        /* [in] */ IServiceConnection* conn);                                                   \
                                                                                                \
    CARAPI StartInstrumentation(                                                                \
        /* [in] */ IComponentName* className,                                                   \
        /* [in] */ const String& profileFile,                                                   \
        /* [in] */ IBundle* arguments,                                                          \
        /* [out] */ Boolean* succeeded);                                                        \
                                                                                                \
    CARAPI GetSystemService(                                                                    \
        /* [in] */ const String& name,                                                          \
        /* [out] */ IInterface** object);                                                       \
                                                                                                \
    CARAPI CheckPermission(                                                                     \
        /* [in] */ const String& permission,                                                    \
        /* [in] */ Int32 pid,                                                                   \
        /* [in] */ Int32 uid,                                                                   \
        /* [out] */ Int32* permissionId);                                                       \
                                                                                                \
    CARAPI CheckCallingPermission(                                                              \
        /* [in] */ const String& permission,                                                    \
        /* [out] */ Int32* permissionId);                                                       \
                                                                                                \
    CARAPI CheckCallingOrSelfPermission(                                                        \
        /* [in] */ const String& permission,                                                    \
        /* [out] */ Int32* permissionId);                                                       \
                                                                                                \
    CARAPI EnforcePermission(                                                                   \
        /* [in] */ const String& permission,                                                    \
        /* [in] */ Int32 pid,                                                                   \
        /* [in] */ Int32 uid,                                                                   \
        /* [in] */ const String& message);                                                      \
                                                                                                \
    CARAPI EnforceCallingPermission(                                                            \
        /* [in] */ const String& permission,                                                    \
        /* [in] */ const String& message);                                                      \
                                                                                                \
    CARAPI EnforceCallingOrSelfPermission(                                                      \
        /* [in] */ const String& permission,                                                    \
        /* [in] */ const String& message);                                                      \
                                                                                                \
    CARAPI GrantUriPermission(                                                                  \
        /* [in] */ const String& toPackage,                                                     \
        /* [in] */ IUri* uri,                                                                   \
        /* [in] */ Int32 modeFlags);                                                            \
                                                                                                \
    CARAPI RevokeUriPermission(                                                                 \
        /* [in] */ IUri* uri,                                                                   \
        /* [in] */ Int32 modeFlags);                                                            \
                                                                                                \
    CARAPI CheckUriPermission(                                                                  \
        /* [in] */ IUri* uri,                                                                   \
        /* [in] */ Int32 pid,                                                                   \
        /* [in] */ Int32 uid,                                                                   \
        /* [in] */ Int32 modeFlags,                                                             \
        /* [out] */ Int32* permissionId);                                                       \
                                                                                                \
    CARAPI CheckCallingUriPermission(                                                           \
        /* [in] */ IUri* uri,                                                                   \
        /* [in] */ Int32 modeFlags,                                                             \
        /* [out] */ Int32* permissionId);                                                       \
                                                                                                \
    CARAPI CheckCallingOrSelfUriPermission(                                                     \
        /* [in] */ IUri* uri,                                                                   \
        /* [in] */ Int32 modeFlags,                                                             \
        /* [out] */ Int32* permissionId);                                                       \
                                                                                                \
    CARAPI CheckUriPermissionEx(                                                                \
        /* [in] */ IUri* uri,                                                                   \
        /* [in] */ const String& readPermission,                                                \
        /* [in] */ const String& writePermission,                                               \
        /* [in] */ Int32 pid,                                                                   \
        /* [in] */ Int32 uid,                                                                   \
        /* [in] */ Int32 modeFlags,                                                             \
        /* [out] */ Int32* permissionId);                                                       \
                                                                                                \
    CARAPI EnforceUriPermission(                                                                \
        /* [in] */ IUri* uri,                                                                   \
        /* [in] */ Int32 pid,                                                                   \
        /* [in] */ Int32 uid,                                                                   \
        /* [in] */ Int32 modeFlags,                                                             \
        /* [in] */ const String& message);                                                      \
                                                                                                \
    CARAPI EnforceCallingUriPermission(                                                         \
        /* [in] */ IUri* uri,                                                                   \
        /* [in] */ Int32 modeFlags,                                                             \
        /* [in] */ const String& message);                                                      \
                                                                                                \
    CARAPI EnforceCallingOrSelfUriPermission(                                                   \
        /* [in] */ IUri* uri,                                                                   \
        /* [in] */ Int32 modeFlags,                                                             \
        /* [in] */ const String& message);                                                      \
                                                                                                \
    CARAPI EnforceUriPermissionEx(                                                              \
        /* [in] */ IUri* uri,                                                                   \
        /* [in] */ const String& readPermission,                                                \
        /* [in] */ const String& writePermission,                                               \
        /* [in] */ Int32 pid,                                                                   \
        /* [in] */ Int32 uid,                                                                   \
        /* [in] */ Int32 modeFlags,                                                             \
        /* [in] */ const String& message);                                                      \
                                                                                                \
    CARAPI CreatePackageContext(                                                                \
        /* [in] */ const String& packageName,                                                   \
        /* [in] */ Int32 flags,                                                                 \
        /* [out] */ IContext** ctx);                                                            \
                                                                                                \
    CARAPI CreatePackageContextAsUser(                                                          \
        /* [in] */ const String& packageName,                                                   \
        /* [in] */ Int32 flags,                                                                 \
        /* [in] */ IUserHandle* user,                                                           \
        /* [out] */ IContext** ctx);                                                            \
                                                                                                \
    CARAPI CreateConfigurationContext(                                                          \
        /* [in] */ IConfiguration* overrideConfiguration,                                       \
        /* [out] */ IContext** ctx);                                                            \
                                                                                                \
    CARAPI CreateDisplayContext(                                                                \
        /* [in] */ IDisplay* display,                                                           \
        /* [out] */ IContext** ctx);                                                            \
                                                                                                \
    CARAPI GetCompatibilityInfo(                                                                \
        /* [in] */ Int32 displayId,                                                             \
        /* [out] */ ICompatibilityInfoHolder** infoHolder);                                     \
                                                                                                \
    CARAPI IsRestricted(                                                                        \
        /* [out] */ Boolean* isRestricted);                                                     \

#define IBROADCASTRECEIVER_METHODS_DECL()                                                       \
    CARAPI OnReceive(                                                                    \
        /* [in] */ IContext* context,                                                           \
        /* [in] */ IIntent* intent);                                                            \
                                                                                                \
    CARAPI GoAsync(                                                                             \
        /* [out] */ IBroadcastReceiverPendingResult** pendingResult);                           \
                                                                                                \
    CARAPI PeekService(                                                                         \
        /* [in] */ IContext* myContext,                                                         \
        /* [in] */ IIntent* service,                                                            \
        /* [out] */ IBinder** binder);                                                          \
                                                                                                \
    CARAPI SetResultCode(                                                                       \
        /* [in] */ Int32 code);                                                                 \
                                                                                                \
    CARAPI GetResultCode(                                                                       \
        /* [out] */ Int32* code);                                                               \
                                                                                                \
    CARAPI SetResultData(                                                                       \
        /* [in] */ const String& data);                                                         \
                                                                                                \
    CARAPI GetResultData(                                                                       \
        /* [out] */ String* data);                                                              \
                                                                                                \
    CARAPI SetResultExtras(                                                                     \
        /* [in] */ IBundle* extras);                                                            \
                                                                                                \
    CARAPI GetResultExtras(                                                                     \
        /* [in] */ Boolean makeMap,                                                             \
        /* [out] */ IBundle** extras);                                                          \
                                                                                                \
    CARAPI SetResult(                                                                           \
        /* [in] */ Int32 code,                                                                  \
        /* [in] */ const String& data,                                                          \
        /* [in] */ IBundle* extras);                                                            \
                                                                                                \
    CARAPI GetAbortBroadcast(                                                                   \
        /* [out] */ Boolean* isAborted);                                                        \
                                                                                                \
    CARAPI AbortBroadcast();                                                                    \
                                                                                                \
    CARAPI ClearAbortBroadcast();                                                               \
                                                                                                \
    CARAPI IsOrderedBroadcast(                                                                  \
        /* [out] */ Boolean* isOrdered);                                                        \
                                                                                                \
    CARAPI IsInitialStickyBroadcast(                                                            \
        /* [out] */ Boolean* isInitial);                                                        \
                                                                                                \
    CARAPI SetOrderedHint(                                                                      \
        /* [in] */ Boolean isOrdered);                                                          \
                                                                                                \
    CARAPI SetPendingResult(                                                                    \
        /* [in] */ IBroadcastReceiverPendingResult* result);                                    \
                                                                                                \
    CARAPI GetPendingResult(                                                                    \
        /* [out] */ IBroadcastReceiverPendingResult** pendingResult);                           \
                                                                                                \
    CARAPI GetSendingUserId(                                                                    \
        /* [out] */ Int32* userId);                                                             \
                                                                                                \
    CARAPI SetDebugUnregister(                                                                  \
        /* [in] */ Boolean debug);                                                              \
                                                                                                \
    CARAPI GetDebugUnregister(                                                                  \
        /* [out] */ Boolean* debugUnregister);                                                  \

#define ICONTEXT_METHODS_IMPL(className, superClass)                                            \
    ECode className::GetAssets(                                                                 \
        /* [out] */ IAssetManager** assetManager)                                               \
    {                                                                                           \
        return superClass::GetAssets(assetManager);                                             \
    }                                                                                           \
                                                                                                \
    ECode className::GetResources(                                                              \
        /* [out] */ IResources** resources)                                                     \
    {                                                                                           \
        return superClass::GetResources(resources);                                             \
    }                                                                                           \
                                                                                                \
    ECode className::GetPackageManager(                                                         \
        /* [out] */ IPackageManager** packageManager)                                           \
    {                                                                                           \
        return superClass::GetPackageManager(packageManager);                                   \
    }                                                                                           \
                                                                                                \
    ECode className::GetContentResolver(                                                        \
        /* [out] */ IContentResolver** resolver)                                                \
    {                                                                                           \
        return superClass::GetContentResolver(resolver);                                        \
    }                                                                                           \
                                                                                                \
    ECode className::GetMainLooper(                                                             \
        /* [out] */ ILooper** looper)                                                           \
    {                                                                                           \
        return superClass::GetMainLooper(looper);                                               \
    }                                                                                           \
                                                                                                \
    ECode className::GetApplicationContext(                                                     \
        /* [out] */ IContext** ctx)                                                             \
    {                                                                                           \
        return superClass::GetApplicationContext(ctx);                                          \
    }                                                                                           \
                                                                                                \
    ECode className::RegisterComponentCallbacks(                                                \
        /* [in] */ IComponentCallbacks* componentCallback)                                      \
    {                                                                                           \
        return Context::RegisterComponentCallbacks(componentCallback);                          \
    }                                                                                           \
                                                                                                \
    ECode className::UnregisterComponentCallbacks(                                              \
        /* [in] */ IComponentCallbacks* componentCallback)                                      \
    {                                                                                           \
        return Context::UnregisterComponentCallbacks(componentCallback);                        \
    }                                                                                           \
                                                                                                \
    ECode className::GetText(                                                                   \
        /* [in] */ Int32 resId,                                                                 \
        /* [out] */ ICharSequence** text)                                                       \
    {                                                                                           \
        return Context::GetText(resId, text);                                                   \
    }                                                                                           \
                                                                                                \
    ECode className::GetString(                                                                 \
        /* [in] */ Int32 resId,                                                                 \
        /* [out] */ String* str)                                                                \
    {                                                                                           \
        return Context::GetString(resId, str);                                                  \
    }                                                                                           \
                                                                                                \
    ECode className::GetStringEx(                                                               \
        /* [in] */ Int32 resId,                                                                 \
        /* [in] */ ArrayOf<IInterface*>* formatArgs,                                            \
        /* [out] */ String* str)                                                                \
    {                                                                                           \
        return superClass::GetStringEx(resId, formatArgs, str);                                 \
    }                                                                                           \
                                                                                                \
    ECode className::SetTheme(                                                                  \
        /* [in] */ Int32 resId)                                                                 \
    {                                                                                           \
        return superClass::SetTheme(resId);                                                     \
    }                                                                                           \
                                                                                                \
    ECode className::GetThemeResId(                                                             \
        /* [out] */ Int32* resId)                                                               \
    {                                                                                           \
        return superClass::GetThemeResId(resId);                                                \
    }                                                                                           \
                                                                                                \
    ECode className::GetTheme(                                                                  \
        /* [out] */ IResourcesTheme** theme)                                                    \
    {                                                                                           \
        return superClass::GetTheme(theme);                                                     \
    }                                                                                           \
                                                                                                \
    ECode className::ObtainStyledAttributes(                                                    \
        /* [in] */ ArrayOf<Int32>* attrs,                                                       \
        /* [out] */ ITypedArray** styles)                                                       \
    {                                                                                           \
        return Context::ObtainStyledAttributes(attrs, styles);                                  \
    }                                                                                           \
                                                                                                \
    ECode className::ObtainStyledAttributesEx(                                                  \
        /* [in] */ Int32 resid,                                                                 \
        /* [in] */ ArrayOf<Int32>* attrs,                                                       \
        /* [out] */ ITypedArray** styles)                                                       \
    {                                                                                           \
        return Context::ObtainStyledAttributesEx(resid, attrs, styles);                         \
    }                                                                                           \
                                                                                                \
    ECode className::ObtainStyledAttributesEx2(                                                 \
        /* [in] */ IAttributeSet* set,                                                          \
        /* [in] */ ArrayOf<Int32>* attrs,                                                       \
        /* [out] */ ITypedArray** styles)                                                       \
    {                                                                                           \
        return Context::ObtainStyledAttributesEx2(set, attrs, styles);                          \
    }                                                                                           \
                                                                                                \
    ECode className::ObtainStyledAttributesEx3(                                                 \
        /* [in] */ IAttributeSet* set,                                                          \
        /* [in] */ ArrayOf<Int32>* attrs,                                                       \
        /* [in] */ Int32 defStyleAttr,                                                          \
        /* [in] */ Int32 defStyleRes,                                                           \
        /* [out] */ ITypedArray** styles)                                                       \
    {                                                                                           \
        return Context::ObtainStyledAttributesEx3(set, attrs, defStyleAttr, defStyleRes,        \
            styles);                                                                            \
    }                                                                                           \
                                                                                                \
    ECode className::GetClassLoader(                                                            \
        /* [out] */ IClassLoader** loader)                                                      \
    {                                                                                           \
        return superClass::GetClassLoader(loader);                                              \
    }                                                                                           \
                                                                                                \
    ECode className::GetPackageName(                                                            \
        /* [out] */ String* packageName)                                                        \
    {                                                                                           \
        return superClass::GetPackageName(packageName);                                         \
    }                                                                                           \
                                                                                                \
    ECode className::GetApplicationInfo(                                                        \
        /* [out] */ IApplicationInfo** info)                                                    \
    {                                                                                           \
        return superClass::GetApplicationInfo(info);                                            \
    }                                                                                           \
                                                                                                \
    ECode className::GetPackageResourcePath(                                                    \
        /* [out] */ String* path)                                                               \
    {                                                                                           \
        return superClass::GetPackageResourcePath(path);                                        \
    }                                                                                           \
                                                                                                \
    ECode className::GetPackageCodePath(                                                        \
        /* [out] */ String* codePath)                                                           \
    {                                                                                           \
        return superClass::GetPackageCodePath(codePath);                                        \
    }                                                                                           \
                                                                                                \
    ECode className::GetSharedPrefsFile(                                                        \
        /* [in] */ const String& name,                                                          \
        /* [out] */IFile** file)                                                                \
    {                                                                                           \
        return superClass::GetSharedPrefsFile(name, file);                                      \
    }                                                                                           \
                                                                                                \
    ECode className::GetSharedPreferences(                                                      \
        /* [in] */ const String& name,                                                          \
        /* [in] */ Int32 mode,                                                                  \
        /* [out] */ ISharedPreferences** prefs)                                                 \
    {                                                                                           \
        return superClass::GetSharedPreferences(name, mode, prefs);                             \
    }                                                                                           \
                                                                                                \
    ECode className::OpenFileInput(                                                             \
        /* [in] */ const String& name,                                                          \
        /* [out] */ IFileInputStream** fileInputStream)                                         \
    {                                                                                           \
        return superClass::OpenFileInput(name, fileInputStream);                                \
    }                                                                                           \
                                                                                                \
    ECode className::OpenFileOutput(                                                            \
        /* [in] */ const String& name,                                                          \
        /* [in] */ Int32 mode,                                                                  \
        /* [out] */ IFileOutputStream** fileOutputStream)                                       \
    {                                                                                           \
        return superClass::OpenFileOutput(name, mode, fileOutputStream);                        \
    }                                                                                           \
                                                                                                \
    ECode className::DeleteFile(                                                                \
        /* [in] */ const String& name,                                                          \
        /* [out] */ Boolean* succeeded)                                                         \
    {                                                                                           \
        return superClass::DeleteFile(name, succeeded);                                         \
    }                                                                                           \
                                                                                                \
    ECode className::GetFileStreamPath(                                                         \
        /* [in] */ const String& name,                                                          \
        /* [out] */ IFile** file)                                                               \
    {                                                                                           \
        return superClass::GetFileStreamPath(name, file);                                       \
    }                                                                                           \
                                                                                                \
    ECode className::GetFilesDir(                                                               \
        /* [out] */ IFile** filesDir)                                                           \
    {                                                                                           \
        return superClass::GetFilesDir(filesDir);                                               \
    }                                                                                           \
                                                                                                \
    ECode className::GetExternalFilesDir(                                                       \
        /* [in] */ const String& type,                                                          \
        /* [out] */ IFile** filesDir)                                                           \
    {                                                                                           \
        return superClass::GetExternalFilesDir(type, filesDir);                                 \
    }                                                                                           \
                                                                                                \
    ECode className::GetObbDir(                                                                 \
        /* [out] */ IFile** obbDir)                                                             \
    {                                                                                           \
        return superClass::GetObbDir(obbDir);                                                   \
    }                                                                                           \
                                                                                                \
    ECode className::GetCacheDir(                                                               \
        /* [out] */ IFile** cacheDir)                                                           \
    {                                                                                           \
        return superClass::GetCacheDir(cacheDir);                                               \
    }                                                                                           \
                                                                                                \
    ECode className::GetExternalCacheDir(                                                       \
        /* [out] */ IFile** externalDir)                                                        \
    {                                                                                           \
        return superClass::GetExternalCacheDir(externalDir);                                    \
    }                                                                                           \
                                                                                                \
    ECode className::GetFileList(                                                               \
        /* [out, callee] */ ArrayOf<String>** fileList)                                         \
    {                                                                                           \
        return superClass::GetFileList(fileList);                                               \
    }                                                                                           \
                                                                                                \
    ECode className::GetDir(                                                                    \
        /* [in] */ const String& name,                                                          \
        /* [in] */ Int32 mode,                                                                  \
        /* [out] */ IFile** dir)                                                                \
    {                                                                                           \
        return superClass::GetDir(name, mode, dir);                                             \
    }                                                                                           \
                                                                                                \
    ECode className::OpenOrCreateDatabase(                                                      \
        /* [in] */ const String& name,                                                          \
        /* [in] */ Int32 mode,                                                                  \
        /* [in] */ ISQLiteDatabaseCursorFactory* factory,                                       \
        /* [out] */ ISQLiteDatabase** sqliteDB)                                                 \
    {                                                                                           \
        return superClass::OpenOrCreateDatabase(name, mode, factory, sqliteDB);                 \
    }                                                                                           \
                                                                                                \
    ECode className::OpenOrCreateDatabaseEx(                                                    \
        /* [in] */ const String& name,                                                          \
        /* [in] */ Int32 mode,                                                                  \
        /* [in] */ ISQLiteDatabaseCursorFactory* factory,                                       \
        /* [in] */ IDatabaseErrorHandler* errorHandler,                                         \
        /* [out] */ ISQLiteDatabase** sqliteDB)                                                 \
    {                                                                                           \
        return superClass::OpenOrCreateDatabaseEx(name, mode, factory, errorHandler, sqliteDB); \
    }                                                                                           \
                                                                                                \
    ECode className::DeleteDatabase(                                                            \
        /* [in] */ const String& name,                                                          \
        /* [out] */ Boolean* succeeded)                                                         \
    {                                                                                           \
        return superClass::DeleteDatabase(name, succeeded);                                     \
    }                                                                                           \
                                                                                                \
    ECode className::GetDatabasePath(                                                           \
        /* [in] */ const String& name,                                                          \
        /* [out] */ IFile** path)                                                               \
    {                                                                                           \
        return superClass::GetDatabasePath(name, path);                                         \
    }                                                                                           \
                                                                                                \
    ECode className::GetDatabaseList(                                                           \
        /* [out, callee] */ ArrayOf<String>** databaseList)                                     \
    {                                                                                           \
        return superClass::GetDatabaseList(databaseList);                                       \
    }                                                                                           \
                                                                                                \
    ECode className::GetWallpaper(                                                              \
        /* [out] */ IDrawable** drawable)                                                       \
    {                                                                                           \
        return superClass::GetWallpaper(drawable);                                              \
    }                                                                                           \
                                                                                                \
    ECode className::PeekWallpaper(                                                             \
        /* [out] */ IDrawable** drawable)                                                       \
    {                                                                                           \
        return superClass::PeekWallpaper(drawable);                                             \
    }                                                                                           \
                                                                                                \
    ECode className::GetWallpaperDesiredMinimumWidth(                                           \
        /* [out] */ Int32* minWidth)                                                            \
    {                                                                                           \
        return superClass::GetWallpaperDesiredMinimumWidth(minWidth);                           \
    }                                                                                           \
                                                                                                \
    ECode className::GetWallpaperDesiredMinimumHeight(                                          \
        /* [out] */ Int32* minHeight)                                                           \
    {                                                                                           \
        return superClass::GetWallpaperDesiredMinimumHeight(minHeight);                         \
    }                                                                                           \
                                                                                                \
    ECode className::SetWallpaper(                                                              \
        /* [in] */ IBitmap* bitmap)                                                             \
    {                                                                                           \
        return superClass::SetWallpaper(bitmap);                                                \
    }                                                                                           \
                                                                                                \
    ECode className::SetWallpaperEx(                                                            \
        /* [in] */ IInputStream* data)                                                          \
    {                                                                                           \
        return superClass::SetWallpaperEx(data);                                                \
    }                                                                                           \
                                                                                                \
    ECode className::ClearWallpaper()                                                           \
    {                                                                                           \
        return superClass::ClearWallpaper();                                                    \
    }                                                                                           \
                                                                                                \
    ECode className::StartActivity(                                                             \
        /* [in] */ IIntent* intent)                                                             \
    {                                                                                           \
        return superClass::StartActivity(intent);                                               \
    }                                                                                           \
                                                                                                \
    ECode className::StartActivityAsUser(                                                       \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ IUserHandle* user)                                                           \
    {                                                                                           \
        return superClass::StartActivityAsUser(intent, user);                                   \
    }                                                                                           \
                                                                                                \
    ECode className::StartActivityEx(                                                           \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ IBundle* options)                                                            \
    {                                                                                           \
        return superClass::StartActivityEx(intent, options);                                    \
    }                                                                                           \
                                                                                                \
    ECode className::StartActivityAsUserEx(                                                     \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ IBundle* options,                                                            \
        /* [in] */ IUserHandle* userId)                                                         \
    {                                                                                           \
        return superClass::StartActivityAsUserEx(intent, options, userId);                      \
    }                                                                                           \
                                                                                                \
    ECode className::StartActivities(                                                           \
        /* [in] */ ArrayOf<IIntent*>* intents)                                                  \
    {                                                                                           \
        return superClass::StartActivities(intents);                                            \
    }                                                                                           \
                                                                                                \
    ECode className::StartActivitiesEx(                                                         \
        /* [in] */ ArrayOf<IIntent*>* intents,                                                  \
        /* [in] */ IBundle* options)                                                            \
    {                                                                                           \
        return superClass::StartActivitiesEx(intents, options);                                 \
    }                                                                                           \
                                                                                                \
    ECode className::StartActivitiesAsUser(                                                     \
        /* [in] */ ArrayOf<IIntent*>* intents,                                                  \
        /* [in] */ IBundle* options,                                                            \
        /* [in] */ IUserHandle* userHandle)                                                     \
    {                                                                                           \
        return superClass::StartActivitiesAsUser(intents, options, userHandle);                 \
    }                                                                                           \
                                                                                                \
    ECode className::StartIntentSender(                                                         \
        /* [in] */ IIntentSender* intent,                                                       \
        /* [in] */ IIntent* fillInIntent,                                                       \
        /* [in] */ Int32 flagsMask,                                                             \
        /* [in] */ Int32 flagsValues,                                                           \
        /* [in] */ Int32 extraFlags)                                                            \
    {                                                                                           \
        return superClass::StartIntentSender(intent, fillInIntent, flagsMask, flagsValues,      \
            extraFlags);                                                                        \
    }                                                                                           \
                                                                                                \
    ECode className::StartIntentSenderEx(                                                       \
        /* [in] */ IIntentSender* intent,                                                       \
        /* [in] */ IIntent* fillInIntent,                                                       \
        /* [in] */ Int32 flagsMask,                                                             \
        /* [in] */ Int32 flagsValues,                                                           \
        /* [in] */ Int32 extraFlags,                                                            \
        /* [in] */ IBundle* options)                                                            \
    {                                                                                           \
        return superClass::StartIntentSenderEx(intent, fillInIntent, flagsMask, flagsValues,    \
            extraFlags, options);                                                               \
    }                                                                                           \
                                                                                                \
    ECode className::SendBroadcast(                                                             \
        /* [in] */ IIntent* intent)                                                             \
    {                                                                                           \
        return superClass::SendBroadcast(intent);                                               \
    }                                                                                           \
                                                                                                \
    ECode className::SendBroadcastEx(                                                           \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ const String& receiverPermission)                                            \
    {                                                                                           \
        return superClass::SendBroadcastEx(intent, receiverPermission);                         \
    }                                                                                           \
                                                                                                \
    ECode className::SendOrderedBroadcast(                                                      \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ const String& receiverPermission)                                            \
    {                                                                                           \
        return superClass::SendOrderedBroadcast(intent, receiverPermission);                    \
    }                                                                                           \
                                                                                                \
    ECode className::SendOrderedBroadcastEx(                                                    \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ const String& receiverPermission,                                            \
        /* [in] */ IBroadcastReceiver* resultReceiver,                                          \
        /* [in] */ IHandler* scheduler,                                                         \
        /* [in] */ Int32 initialCode,                                                           \
        /* [in] */ const String& initialData,                                                   \
        /* [in] */ IBundle* initialExtras)                                                      \
    {                                                                                           \
        return superClass::SendOrderedBroadcastEx(intent, receiverPermission, resultReceiver,   \
            scheduler, initialCode, initialData, initialExtras);                                \
    }                                                                                           \
                                                                                                \
    ECode className::SendBroadcastAsUser(                                                       \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ IUserHandle* user)                                                           \
    {                                                                                           \
        return superClass::SendBroadcastAsUser(intent, user);                                   \
    }                                                                                           \
                                                                                                \
    ECode className::SendBroadcastAsUserEx(                                                     \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ IUserHandle* user,                                                           \
        /* [in] */ const String& receiverPermission)                                            \
    {                                                                                           \
        return superClass::SendBroadcastAsUserEx(intent, user, receiverPermission);             \
    }                                                                                           \
                                                                                                \
    ECode className::SendOrderedBroadcastAsUser(                                                \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ IUserHandle* user,                                                           \
        /* [in] */ const String& receiverPermission,                                            \
        /* [in] */ IBroadcastReceiver* resultReceiver,                                          \
        /* [in] */ IHandler* scheduler,                                                         \
        /* [in] */ Int32 initialCode,                                                           \
        /* [in] */ const String& initialData,                                                   \
        /* [in] */ IBundle* initialExtras)                                                      \
    {                                                                                           \
        return superClass::SendOrderedBroadcastAsUser(intent, user, receiverPermission,         \
            resultReceiver, scheduler, initialCode, initialData, initialExtras);                \
    }                                                                                           \
                                                                                                \
    ECode className::SendStickyBroadcast(                                                       \
        /* [in] */ IIntent* intent)                                                             \
    {                                                                                           \
        return superClass::SendStickyBroadcast(intent);                                         \
    }                                                                                           \
                                                                                                \
    ECode className::SendStickyOrderedBroadcast(                                                \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ IBroadcastReceiver* resultReceiver,                                          \
        /* [in] */ IHandler* scheduler,                                                         \
        /* [in] */ Int32 initialCode,                                                           \
        /* [in] */ const String& initialData,                                                   \
        /* [in] */ IBundle* initialExtras)                                                      \
    {                                                                                           \
        return superClass::SendStickyOrderedBroadcast(intent, resultReceiver, scheduler,        \
            initialCode, initialData, initialExtras);                                           \
    }                                                                                           \
                                                                                                \
    ECode className::RemoveStickyBroadcast(                                                     \
        /* [in] */ IIntent* intent)                                                             \
    {                                                                                           \
        return superClass::RemoveStickyBroadcast(intent);                                       \
    }                                                                                           \
                                                                                                \
    ECode className::SendStickyBroadcastAsUser(                                                 \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ IUserHandle* user)                                                           \
    {                                                                                           \
        return superClass::SendStickyBroadcastAsUser(intent, user);                             \
    }                                                                                           \
                                                                                                \
    ECode className::SendStickyOrderedBroadcastAsUser(                                          \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ IUserHandle* user,                                                           \
        /* [in] */ IBroadcastReceiver* resultReceiver,                                          \
        /* [in] */ IHandler* scheduler,                                                         \
        /* [in] */ Int32 initialCode,                                                           \
        /* [in] */ const String& initialData,                                                   \
        /* [in] */ IBundle* initialExtras)                                                      \
    {                                                                                           \
        return superClass::SendStickyOrderedBroadcastAsUser(intent, user, resultReceiver,       \
            scheduler, initialCode, initialData, initialExtras);                                \
    }                                                                                           \
                                                                                                \
    ECode className::RemoveStickyBroadcastAsUser(                                               \
        /* [in] */ IIntent* intent,                                                             \
        /* [in] */ IUserHandle* user)                                                           \
    {                                                                                           \
        return superClass::RemoveStickyBroadcastAsUser(intent, user);                           \
    }                                                                                           \
                                                                                                \
    ECode className::RegisterReceiver(                                                          \
        /* [in] */ IBroadcastReceiver* receiver,                                                \
        /* [in] */ IIntentFilter* filter,                                                       \
        /* [out] */ IIntent** stickyIntent)                                                     \
    {                                                                                           \
        return superClass::RegisterReceiver(receiver, filter, stickyIntent);                    \
    }                                                                                           \
                                                                                                \
    ECode className::RegisterReceiverEx(                                                        \
        /* [in] */ IBroadcastReceiver* receiver,                                                \
        /* [in] */ IIntentFilter* filter,                                                       \
        /* [in] */ const String& broadcastPermission,                                           \
        /* [in] */ IHandler* scheduler,                                                         \
        /* [out] */ IIntent** stickyIntent)                                                     \
    {                                                                                           \
        return superClass::RegisterReceiverEx(receiver, filter, broadcastPermission, scheduler, \
            stickyIntent);                                                                      \
    }                                                                                           \
                                                                                                \
    ECode className::RegisterReceiverAsUser(                                                    \
        /* [in] */ IBroadcastReceiver* receiver,                                                \
        /* [in] */ IUserHandle* user,                                                           \
        /* [in] */ IIntentFilter* filter,                                                       \
        /* [in] */ const String& broadcastPermission,                                           \
        /* [in] */ IHandler* scheduler,                                                         \
        /* [out] */ IIntent** stickyIntent)                                                     \
    {                                                                                           \
        return superClass::RegisterReceiverAsUser(receiver, user, filter, broadcastPermission,  \
            scheduler, stickyIntent);                                                           \
    }                                                                                           \
                                                                                                \
    ECode className::UnregisterReceiver(                                                        \
        /* [in] */ IBroadcastReceiver* receiver)                                                \
    {                                                                                           \
        return superClass::UnregisterReceiver(receiver);                                        \
    }                                                                                           \
                                                                                                \
    ECode className::StartService(                                                              \
        /* [in] */ IIntent* service,                                                            \
        /* [out] */ IComponentName** name)                                                      \
    {                                                                                           \
        return superClass::StartService(service, name);                                         \
    }                                                                                           \
                                                                                                \
    ECode className::StopService(                                                               \
        /* [in] */ IIntent* service,                                                            \
        /* [out] */ Boolean* succeeded)                                                         \
    {                                                                                           \
        return superClass::StopService(service, succeeded);                                     \
    }                                                                                           \
                                                                                                \
    ECode className::StartServiceAsUser(                                                        \
        /* [in] */ IIntent* service,                                                            \
        /* [in] */ IUserHandle* user,                                                           \
        /* [out] */ IComponentName** name)                                                      \
    {                                                                                           \
        return superClass::StartServiceAsUser(service, user, name);                             \
    }                                                                                           \
                                                                                                \
    ECode className::StopServiceAsUser(                                                         \
        /* [in] */ IIntent* service,                                                            \
        /* [in] */ IUserHandle* user,                                                           \
        /* [out] */ Boolean* succeeded)                                                         \
    {                                                                                           \
        return superClass::StopServiceAsUser(service, user, succeeded);                         \
    }                                                                                           \
                                                                                                \
    ECode className::BindService(                                                               \
        /* [in] */ IIntent* service,                                                            \
        /* [in] */ IServiceConnection* conn,                                                    \
        /* [in] */ Int32 flags,                                                                 \
        /* [out] */ Boolean* succeeded)                                                         \
    {                                                                                           \
        return superClass::BindService(service, conn, flags, succeeded);                        \
    }                                                                                           \
                                                                                                \
    ECode className::BindServiceEx(                                                             \
        /* [in] */ IIntent* service,                                                            \
        /* [in] */ IServiceConnection* conn,                                                    \
        /* [in] */ Int32 flags,                                                                 \
        /* [in] */ Int32 userHandle,                                                            \
        /* [out] */ Boolean* succeeded)                                                         \
    {                                                                                           \
        return superClass::BindServiceEx(service, conn, flags, userHandle, succeeded);          \
    }                                                                                           \
                                                                                                \
    ECode className::UnbindService(                                                             \
        /* [in] */ IServiceConnection* conn)                                                    \
    {                                                                                           \
        return superClass::UnbindService(conn);                                                 \
    }                                                                                           \
                                                                                                \
    ECode className::StartInstrumentation(                                                      \
        /* [in] */ IComponentName* className,                                                   \
        /* [in] */ const String& profileFile,                                                   \
        /* [in] */ IBundle* arguments,                                                          \
        /* [out] */ Boolean* succeeded)                                                         \
    {                                                                                           \
        return superClass::StartInstrumentation(className, profileFile, arguments, succeeded);  \
    }                                                                                           \
                                                                                                \
    ECode className::GetSystemService(                                                          \
        /* [in] */ const String& name,                                                          \
        /* [out] */ IInterface** object)                                                        \
    {                                                                                           \
        return superClass::GetSystemService(name, object);                                      \
    }                                                                                           \
                                                                                                \
    ECode className::CheckPermission(                                                           \
        /* [in] */ const String& permission,                                                    \
        /* [in] */ Int32 pid,                                                                   \
        /* [in] */ Int32 uid,                                                                   \
        /* [out] */ Int32* permissionId)                                                        \
    {                                                                                           \
        return superClass::CheckPermission(permission, pid, uid, permissionId);                 \
    }                                                                                           \
                                                                                                \
    ECode className::CheckCallingPermission(                                                    \
        /* [in] */ const String& permission,                                                    \
        /* [out] */ Int32* permissionId)                                                        \
    {                                                                                           \
        return superClass::CheckCallingPermission(permission, permissionId);                    \
    }                                                                                           \
                                                                                                \
    ECode className::CheckCallingOrSelfPermission(                                              \
        /* [in] */ const String& permission,                                                    \
        /* [out] */ Int32* permissionId)                                                        \
    {                                                                                           \
        return superClass::CheckCallingOrSelfPermission(permission, permissionId);              \
    }                                                                                           \
                                                                                                \
    ECode className::EnforcePermission(                                                         \
        /* [in] */ const String& permission,                                                    \
        /* [in] */ Int32 pid,                                                                   \
        /* [in] */ Int32 uid,                                                                   \
        /* [in] */ const String& message)                                                       \
    {                                                                                           \
        return superClass::EnforcePermission(permission, pid, uid, message);                    \
    }                                                                                           \
                                                                                                \
    ECode className::EnforceCallingPermission(                                                  \
        /* [in] */ const String& permission,                                                    \
        /* [in] */ const String& message)                                                       \
    {                                                                                           \
        return superClass::EnforceCallingPermission(permission, message);                       \
    }                                                                                           \
                                                                                                \
    ECode className::EnforceCallingOrSelfPermission(                                            \
        /* [in] */ const String& permission,                                                    \
        /* [in] */ const String& message)                                                       \
    {                                                                                           \
        return superClass::EnforceCallingOrSelfPermission(permission, message);                 \
    }                                                                                           \
                                                                                                \
    ECode className::GrantUriPermission(                                                        \
        /* [in] */ const String& toPackage,                                                     \
        /* [in] */ IUri* uri,                                                                   \
        /* [in] */ Int32 modeFlags)                                                             \
    {                                                                                           \
        return superClass::GrantUriPermission(toPackage, uri, modeFlags);                       \
    }                                                                                           \
                                                                                                \
    ECode className::RevokeUriPermission(                                                       \
        /* [in] */ IUri* uri,                                                                   \
        /* [in] */ Int32 modeFlags)                                                             \
    {                                                                                           \
        return superClass::RevokeUriPermission(uri, modeFlags);                                 \
    }                                                                                           \
                                                                                                \
    ECode className::CheckUriPermission(                                                        \
        /* [in] */ IUri* uri,                                                                   \
        /* [in] */ Int32 pid,                                                                   \
        /* [in] */ Int32 uid,                                                                   \
        /* [in] */ Int32 modeFlags,                                                             \
        /* [out] */ Int32* permissionId)                                                        \
    {                                                                                           \
        return superClass::CheckUriPermission(uri, pid, uid, modeFlags, permissionId);          \
    }                                                                                           \
                                                                                                \
    ECode className::CheckCallingUriPermission(                                                 \
        /* [in] */ IUri* uri,                                                                   \
        /* [in] */ Int32 modeFlags,                                                             \
        /* [out] */ Int32* permissionId)                                                        \
    {                                                                                           \
        return superClass::CheckCallingUriPermission(uri, modeFlags, permissionId);             \
    }                                                                                           \
                                                                                                \
    ECode className::CheckCallingOrSelfUriPermission(                                           \
        /* [in] */ IUri* uri,                                                                   \
        /* [in] */ Int32 modeFlags,                                                             \
        /* [out] */ Int32* permissionId)                                                        \
    {                                                                                           \
        return superClass::CheckCallingOrSelfUriPermission(uri, modeFlags, permissionId);       \
    }                                                                                           \
                                                                                                \
    ECode className::CheckUriPermissionEx(                                                      \
        /* [in] */ IUri* uri,                                                                   \
        /* [in] */ const String& readPermission,                                                \
        /* [in] */ const String& writePermission,                                               \
        /* [in] */ Int32 pid,                                                                   \
        /* [in] */ Int32 uid,                                                                   \
        /* [in] */ Int32 modeFlags,                                                             \
        /* [out] */ Int32* permissionId)                                                        \
    {                                                                                           \
        return superClass::CheckUriPermissionEx(uri, readPermission, writePermission, pid, uid, \
            modeFlags, permissionId);                                                           \
    }                                                                                           \
                                                                                                \
    ECode className::EnforceUriPermission(                                                      \
        /* [in] */ IUri* uri,                                                                   \
        /* [in] */ Int32 pid,                                                                   \
        /* [in] */ Int32 uid,                                                                   \
        /* [in] */ Int32 modeFlags,                                                             \
        /* [in] */ const String& message)                                                       \
    {                                                                                           \
        return superClass::EnforceUriPermission(uri, pid, uid, modeFlags, message);             \
    }                                                                                           \
                                                                                                \
    ECode className::EnforceCallingUriPermission(                                               \
        /* [in] */ IUri* uri,                                                                   \
        /* [in] */ Int32 modeFlags,                                                             \
        /* [in] */ const String& message)                                                       \
    {                                                                                           \
        return superClass::EnforceCallingUriPermission(uri, modeFlags, message);                \
    }                                                                                           \
                                                                                                \
    ECode className::EnforceCallingOrSelfUriPermission(                                         \
        /* [in] */ IUri* uri,                                                                   \
        /* [in] */ Int32 modeFlags,                                                             \
        /* [in] */ const String& message)                                                       \
    {                                                                                           \
        return superClass::EnforceCallingOrSelfUriPermission(uri, modeFlags, message);          \
    }                                                                                           \
                                                                                                \
    ECode className::EnforceUriPermissionEx(                                                    \
        /* [in] */ IUri* uri,                                                                   \
        /* [in] */ const String& readPermission,                                                \
        /* [in] */ const String& writePermission,                                               \
        /* [in] */ Int32 pid,                                                                   \
        /* [in] */ Int32 uid,                                                                   \
        /* [in] */ Int32 modeFlags,                                                             \
        /* [in] */ const String& message)                                                       \
    {                                                                                           \
        return superClass::EnforceUriPermissionEx(uri, readPermission, writePermission, pid,    \
            uid, modeFlags, message);                                                           \
    }                                                                                           \
                                                                                                \
    ECode className::CreatePackageContext(                                                      \
        /* [in] */ const String& packageName,                                                   \
        /* [in] */ Int32 flags,                                                                 \
        /* [out] */ IContext** ctx)                                                             \
    {                                                                                           \
        return superClass::CreatePackageContext(packageName, flags, ctx);                       \
    }                                                                                           \
                                                                                                \
    ECode className::CreatePackageContextAsUser(                                                \
        /* [in] */ const String& packageName,                                                   \
        /* [in] */ Int32 flags,                                                                 \
        /* [in] */ IUserHandle* user,                                                           \
        /* [out] */ IContext** ctx)                                                             \
    {                                                                                           \
        return superClass::CreatePackageContextAsUser(packageName, flags, user, ctx);           \
    }                                                                                           \
                                                                                                \
    ECode className::CreateConfigurationContext(                                                \
        /* [in] */ IConfiguration* overrideConfiguration,                                       \
        /* [out] */ IContext** ctx)                                                             \
    {                                                                                           \
        return superClass::CreateConfigurationContext(overrideConfiguration, ctx);              \
    }                                                                                           \
                                                                                                \
    ECode className::CreateDisplayContext(                                                      \
        /* [in] */ IDisplay* display,                                                           \
        /* [out] */ IContext** ctx)                                                             \
    {                                                                                           \
        return superClass::CreateDisplayContext(display, ctx);                                  \
    }                                                                                           \
                                                                                                \
    ECode className::GetCompatibilityInfo(                                                      \
        /* [in] */ Int32 displayId,                                                             \
        /* [out] */ ICompatibilityInfoHolder** infoHolder)                                      \
    {                                                                                           \
        return superClass::GetCompatibilityInfo(displayId, infoHolder);                         \
    }                                                                                           \
                                                                                                \
    ECode className::IsRestricted(                                                              \
        /* [out] */ Boolean* isRestricted)                                                      \
    {                                                                                           \
        return superClass::IsRestricted(isRestricted);                                          \
    }                                                                                           \

#define IBROADCASTRECEIVER_METHODS_IMPL(className, superClass)                                  \
    ECode className::OnReceive(                                                          \
        /* [in] */ IContext* context,                                                           \
        /* [in] */ IIntent* intent)                                                             \
    {                                                                                           \
        return superClass::OnReceive(context, intent);                                   \
    }                                                                                           \
                                                                                                \
    ECode className::GoAsync(                                                                   \
        /* [out] */ IBroadcastReceiverPendingResult** pendingResult)                            \
    {                                                                                           \
        return superClass::GoAsync(pendingResult);                                              \
    }                                                                                           \
                                                                                                \
    ECode className::PeekService(                                                               \
        /* [in] */ IContext* myContext,                                                         \
        /* [in] */ IIntent* service,                                                            \
        /* [out] */ IBinder** binder)                                                           \
    {                                                                                           \
        return superClass::PeekService(myContext, service, binder);                             \
    }                                                                                           \
                                                                                                \
    ECode className::SetResultCode(                                                             \
        /* [in] */ Int32 code)                                                                  \
    {                                                                                           \
        return superClass::SetResultCode(code);                                                 \
    }                                                                                           \
                                                                                                \
    ECode className::GetResultCode(                                                             \
        /* [out] */ Int32* code)                                                                \
    {                                                                                           \
        return superClass::GetResultCode(code);                                                 \
    }                                                                                           \
                                                                                                \
    ECode className::SetResultData(                                                             \
        /* [in] */ const String& data)                                                          \
    {                                                                                           \
        return superClass::SetResultData(data);                                                 \
    }                                                                                           \
                                                                                                \
    ECode className::GetResultData(                                                             \
        /* [out] */ String* data)                                                               \
    {                                                                                           \
        return superClass::GetResultData(data);                                                 \
    }                                                                                           \
                                                                                                \
    ECode className::SetResultExtras(                                                           \
        /* [in] */ IBundle* extras)                                                             \
    {                                                                                           \
        return superClass::SetResultExtras(extras);                                             \
    }                                                                                           \
                                                                                                \
    ECode className::GetResultExtras(                                                           \
        /* [in] */ Boolean makeMap,                                                             \
        /* [out] */ IBundle** extras)                                                           \
    {                                                                                           \
        return superClass::GetResultExtras(makeMap, extras);                                    \
    }                                                                                           \
                                                                                                \
    ECode className::SetResult(                                                                 \
        /* [in] */ Int32 code,                                                                  \
        /* [in] */ const String& data,                                                          \
        /* [in] */ IBundle* extras)                                                             \
    {                                                                                           \
        return superClass::SetResult(code, data, extras);                                       \
    }                                                                                           \
                                                                                                \
    ECode className::GetAbortBroadcast(                                                         \
        /* [out] */ Boolean* isAborted)                                                         \
    {                                                                                           \
        return superClass::GetAbortBroadcast(isAborted);                                        \
    }                                                                                           \
                                                                                                \
    ECode className::AbortBroadcast()                                                           \
    {                                                                                           \
        return superClass::AbortBroadcast();                                                    \
    }                                                                                           \
                                                                                                \
    ECode className::ClearAbortBroadcast()                                                      \
    {                                                                                           \
        return superClass::ClearAbortBroadcast();                                               \
    }                                                                                           \
                                                                                                \
    ECode className::IsOrderedBroadcast(                                                        \
        /* [out] */ Boolean* isOrdered)                                                         \
    {                                                                                           \
        return superClass::IsOrderedBroadcast(isOrdered);                                       \
    }                                                                                           \
                                                                                                \
    ECode className::IsInitialStickyBroadcast(                                                  \
        /* [out] */ Boolean* isInitial)                                                         \
    {                                                                                           \
        return superClass::IsInitialStickyBroadcast(isInitial);                                 \
    }                                                                                           \
                                                                                                \
    ECode className::SetOrderedHint(                                                            \
        /* [in] */ Boolean isOrdered)                                                           \
    {                                                                                           \
        return superClass::SetOrderedHint(isOrdered);                                           \
    }                                                                                           \
                                                                                                \
    ECode className::SetPendingResult(                                                          \
        /* [in] */ IBroadcastReceiverPendingResult* result)                                     \
    {                                                                                           \
        return superClass::SetPendingResult(result);                                            \
    }                                                                                           \
                                                                                                \
    ECode className::GetPendingResult(                                                          \
        /* [out] */ IBroadcastReceiverPendingResult** pendingResult)                            \
    {                                                                                           \
        return superClass::GetPendingResult(pendingResult);                                     \
    }                                                                                           \
                                                                                                \
    ECode className::GetSendingUserId(                                                          \
        /* [out] */ Int32* userId)                                                              \
    {                                                                                           \
        return superClass::GetSendingUserId(userId);                                            \
    }                                                                                           \
                                                                                                \
    ECode className::SetDebugUnregister(                                                        \
        /* [in] */ Boolean debug)                                                               \
    {                                                                                           \
        return superClass::SetDebugUnregister(debug);                                           \
    }                                                                                           \
                                                                                                \
    ECode className::GetDebugUnregister(                                                        \
        /* [out] */ Boolean* debugUnregister)                                                   \
    {                                                                                           \
        return superClass::GetDebugUnregister(debugUnregister);                                 \
    }                                                                                           \

#endif //__CONTEXTMACRO_H__
