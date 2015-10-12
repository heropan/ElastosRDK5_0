// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_AUTOFILL_AUTOFILLPOPUP_H_
#define _ELASTOS_DROID_WEBKIT_UI_AUTOFILL_AUTOFILLPOPUP_H_

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/utility/etl/List.h"
#include "elastos/droid/webkit/native/ui/DropdownPopupWindow.h"
#include "elastos/droid/webkit/native/ui/autofill/AutofillSuggestion.h"

// package org.chromium.ui.autofill;
// import android.content.Context;
// import android.view.View;
// import android.widget.AdapterView;
// import org.chromium.ui.DropdownAdapter;
// import org.chromium.ui.DropdownItem;
// import org.chromium.ui.DropdownPopupWindow;
// import org.chromium.ui.base.ViewAndroidDelegate;
// import java.util.ArrayList;
// import java.util.Arrays;
// import java.util.HashSet;
// import java.util.List;

using Elastos::Droid::Utility::List;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IView;
using Elastos::Droid::Widget::IAdapterViewOnItemClickListener;
using Elastos::Droid::Widget::IAdapterView;
using Elastos::Droid::Webkit::Ui::DropdownPopupWindow;
using Elastos::Droid::Webkit::Ui::Autofill::AutofillSuggestion;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Autofill {

class ViewAndroidDelegate;

/**
  * The Autofill suggestion popup that lists relevant suggestions.
  */
class AutofillPopup
    : public Object
    , public DropdownPopupWindow
    , public IAdapterViewOnItemClickListener
{
public:
    /**
      * An interface to handle the touch interaction with an AutofillPopup object.
      */
    class AutofillPopupDelegate
    {
    public:
        /**
          * Requests the controller to hide AutofillPopup.
          */
        virtual CARAPI RequestHide() = 0;

        /**
          * Handles the selection of an Autofill suggestion from an AutofillPopup.
          * @param listIndex The index of the selected Autofill suggestion.
          */
        virtual CARAPI SuggestionSelected(
            /* [in] */ Int32 listIndex) = 0;
    };

public:
    /**
      * Creates an AutofillWindow with specified parameters.
      * @param context Application context.
      * @param viewAndroidDelegate View delegate used to add and remove views.
      * @param autofillCallback A object that handles the calls to the native AutofillPopupView.
      */
    AutofillPopup(
        /* [in] */ IContext* context,
        /* [in] */ ViewAndroidDelegate* viewAndroidDelegate,
        /* [in] */ AutofillPopupDelegate* autofillCallback);

    /**
      * Filters the Autofill suggestions to the ones that we support and shows the popup.
      * @param suggestions Autofill suggestion data.
      */
    virtual CARAPI FilterAndShow(
        /* [in] */ ArrayOf<AutofillSuggestion*>* suggestions);

    /**
      * Overrides the default dismiss behavior to request the controller to dismiss the view.
      */
    // @Override
    CARAPI Dismiss();

    /**
      * Hides the popup.
      */
    virtual CARAPI Hide();

    // @Override
    CARAPI OnItemClick(
        /* [in] */ IAdapterView* parent,
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

private:
    /**
      * Constants defining types of Autofill suggestion entries.
      * Has to be kept in sync with enum in WebAutofillClient.h
      *
      * Not supported: MenuItemIDWarningMessage, MenuItemIDClearForm, and
      * MenuItemIDAutofillOptions.
      */
    static const Int32 ITEM_ID_AUTOCOMPLETE_ENTRY = 0;
    static const Int32 ITEM_ID_PASSWORD_ENTRY = -2;
    static const Int32 ITEM_ID_SEPARATOR_ENTRY = -3;
    static const Int32 ITEM_ID_DATA_LIST_ENTRY = -6;
    /*const*/ AutoPtr<IContext> mContext;
    /*const*/ AutoPtr<AutofillPopupDelegate> mAutofillCallback;
    AutoPtr< List< AutoPtr<AutofillSuggestion> > > mSuggestions;
};

} // namespace Autofill
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_AUTOFILL_AUTOFILLPOPUP_H_

