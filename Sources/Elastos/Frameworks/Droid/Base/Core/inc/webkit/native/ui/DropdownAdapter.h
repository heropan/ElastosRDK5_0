// wuweizuo automatic build .h file from .java file.
// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_DROPDOWNADAPTER_H_
#define _ELASTOS_DROID_WEBKIT_UI_DROPDOWNADAPTER_H_

#include "elatypes.h"
#include "elautoptr.h"
#include "ext/frameworkext.h"
#include "content/Context.h"
#include "graphics/CColor.h"
#include "graphics/Typeface.h"
#include "text/TextUtils.h"
#include "view/LayoutInflater.h"
#include "view/View.h"
#include "view/ViewGroup.h"
#include "widget/ArrayAdapter.h"
#include "widget/TextView.h"
#include "webkit/native/base/ApiCompatibilityUtils.h"

// package org.chromium.ui;
// import android.content.Context;
// import android.graphics.Color;
// import android.graphics.Typeface;
// import android.text.TextUtils;
// import android.view.LayoutInflater;
// import android.view.View;
// import android.view.ViewGroup;
// import android.widget.AbsListView.LayoutParams;
// import android.widget.ArrayAdapter;
// import android.widget.TextView;
// import org.chromium.base.ApiCompatibilityUtils;
// import java.util.List;
// import java.util.Set;

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::IColor;
using Elastos::Droid::Graphics::ITypeface;
using Elastos::Droid::Text::ITextUtils;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::Widget::IAbsListViewLayoutParams;
using Elastos::Droid::Widget::IArrayAdapter;
using Elastos::Droid::Widget::ITextView;
using Elastos::Droid::Webkit::Base::ApiCompatibilityUtils;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

/**
  * Dropdown item adapter for DropdownPopupWindow.
  */
class DropdownAdapter
    : public Object
    , public ArrayAdapter<DropdownItem>
{
public:
    DropdownAdapter(
        /* [in] */ IContext* context,
        /* [in] */ IList<DropdownItem*>* items,
        /* [in] */ ISet<Integer*>* separators);

    DropdownAdapter(
        /* [in] */ IContext* context,
        /* [in] */ ArrayOf<DropdownItem>* items,
        /* [in] */ ISet<Integer*>* separators);

    // @Override
    CARAPI_(AutoPtr<IView>) GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

    // @Override
    CARAPI_(Boolean) AreAllItemsEnabled();

    // @Override
    CARAPI_(Boolean) IsEnabled(
        /* [in] */ Int32 position);

private:
    CARAPI_(Boolean) CheckAreAllItemsEnabled();

private:
    AutoPtr<IContext> mContext;
    AutoPtr< ISet<Integer> > mSeparators;
    Boolean mAreAllItemsEnabled;
};

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_DROPDOWNADAPTER_H_

