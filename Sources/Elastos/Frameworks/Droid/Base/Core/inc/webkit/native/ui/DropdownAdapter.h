// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_DROPDOWNADAPTER_H_
#define _ELASTOS_DROID_WEBKIT_UI_DROPDOWNADAPTER_H_

//package org.chromium.ui;

//import android.content.Context;
//import android.graphics.Color;
//import android.graphics.Typeface;
//import android.text.TextUtils;
//import android.view.LayoutInflater;
//import android.view.View;
//import android.view.ViewGroup;
//import android.widget.AbsListView.LayoutParams;
//import android.widget.ArrayAdapter;
//import android.widget.TextView;

//import org.chromium.base.ApiCompatibilityUtils;

//import java.util.List;
//import java.util.Set;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

/**
 * Dropdown item adapter for DropdownPopupWindow.
 */
class DropdownAdapter : public ArrayAdapter<DropdownItem*>
{
public:
    DropdownAdapter(
        /* in */ IContext* context,
        /* in */ IList< AutoPtr<DropdownItem> >* items,
        /* in */ ISet<IInteger32>* separators);

    DropdownAdapter(
        /* in */ IContext* context,
        /* in */ ArrayOf< AutoPtr<DropdownItem> >* items,
        /* in */ ISet<IInteger32>* separators);

    //@Override
    CARAPI_(Boolean) AreAllItemsEnabled();

    //@Override
    CARAPI_(Boolean) IsEnabled(
        /* in */ Int32 position);

private:
    CARAPI_(Boolean) CheckAreAllItemsEnabled();

private:
    AutoPtr<IContext> mContext;
    AutoPtr< ISet<IInteger32> > mSeparators;
    Boolean mAreAllItemsEnabled;
};

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_DROPDOWNADAPTER_H_

