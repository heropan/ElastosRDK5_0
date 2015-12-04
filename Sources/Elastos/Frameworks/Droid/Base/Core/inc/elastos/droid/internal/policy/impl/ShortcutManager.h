/*
  * Copyright (C) 2007 The Android Open Source Project
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

#ifndef __ELASTOS_DROID_INTERNAL_POLICY_IMPL_SHORTCUTMANAGER_H__
#define __ELASTOS_DROID_INTERNAL_POLICY_IMPL_SHORTCUTMANAGER_H__

#include "elastos/droid/ext/frameworkext.h"

// package com.android.internal.policy.impl;
// import android.content.Context;
// import android.content.Intent;
// import android.database.ContentObserver;
// import android.database.Cursor;
// import android.os.Handler;
// import android.provider.Settings;
// import android.util.Log;
// import android.util.SparseArray;
// import android.view.KeyCharacterMap;
// import java.net.URISyntaxException;

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::View::IKeyCharacterMap;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

/**
  * Manages quick launch shortcuts by:
  * <li> Keeping the local copy in sync with the database (this is an observer)
  * <li> Returning a shortcut-matching intent to clients
  */
class ShortcutManager
    : public Object
    , public IContentObserver
{
public:
    ShortcutManager(
        /* [in] */ IContext* context,
        /* [in] */ IHandler* handler);

    /** Observes the provider of shortcut+intents */
    virtual CARAPI Observe();

    // @Override
    CARAPI OnChange(
        /* [in] */ Boolean selfChange);

    /**
      * Gets the shortcut intent for a given keycode+modifier. Make sure you
      * strip whatever modifier is used for invoking shortcuts (for example,
      * if 'Sym+A' should invoke a shortcut on 'A', you should strip the
      * 'Sym' bit from the modifiers before calling this method.
      * <p>
      * This will first try an exact match (with modifiers), and then try a
      * match without modifiers (primary character on a key).
      * 
      * @param kcm The key character map of the device on which the key was pressed.
      * @param keyCode The key code.
      * @param metaState The meta state, omitting any modifiers that were used
      * to invoke the shortcut.
      * @return The intent that matches the shortcut, or null if not found.
      */
    virtual CARAPI GetIntent(
        /* [in] */ IKeyCharacterMap* kcm,
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 metaState,
        /* [out] */ Intent** result);

private:
    static CARAPI_(AutoPtr< ArrayOf<String> >) MiddleInitSprojection();

    CARAPI_(void) UpdateShortcuts();

private:
    static const String TAG;
    static const Int32 COLUMN_SHORTCUT = 0;
    static const Int32 COLUMN_INTENT = 1;
    static AutoPtr< ArrayOf<String> > sProjection;
    AutoPtr<IContext> mContext;
    AutoPtr<ICursor> mCursor;
    /** Map of a shortcut to its intent. */
    AutoPtr< ISparseArray<IIntent> > mShortcutIntents;
};

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_POLICY_IMPL_SHORTCUTMANAGER_H__

