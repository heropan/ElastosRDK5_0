// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_ONCOLORCHANGEDLISTENER_H_
#define _ELASTOS_DROID_WEBKIT_UI_ONCOLORCHANGEDLISTENER_H_

#include "elastos/droid/ext/frameworkext.h"

// package org.chromium.ui;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

/**
  * The callback used to indicate the user changed the color.
  */
class OnColorChangedListener
{
public:
    /**
      * Called upon a color change.
      *
      * @param color The color that was set.
      */
    virtual CARAPI OnColorChanged(
        /* [in] */ Int32 color) = 0;
};

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_ONCOLORCHANGEDLISTENER_H_

