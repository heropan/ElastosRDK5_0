/*
 * Copyright (C) 2014 The Android Open Source Project
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

package com.android.settings;

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Pm::IUserInfo;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Database::IDataSetObserver;
using Elastos::Droid::Graphics::Drawable::IBitmapDrawable;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::IUserManager;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::Widget::IImageView;
using Elastos::Droid::Widget::ISpinnerAdapter;
using Elastos::Droid::Widget::ITextView;

using Elastos::Droid::Internal::Utility::IUserIcons;

using Elastos::Utility::IArrayList;

/**
 * Adapter for a spinner that shows a list of users.
 */
public class UserSpinnerAdapter implements SpinnerAdapter {
    // TODO: Update UI. See: http://b/16518801
    /** Holder for user details */
    public static class UserDetails {
        private final UserHandle mUserHandle;
        private final String name;
        private final Drawable icon;

        public UserDetails(UserHandle userHandle, UserManager um, Context context) {
            mUserHandle = userHandle;
            UserInfo userInfo = um->GetUserInfo(mUserHandle->GetIdentifier());
            if (userInfo->IsManagedProfile()) {
                name = context->GetString(R::string::managed_user_title);
                icon = Resources->GetSystem()->GetDrawable(
                    R.drawable.ic_corp_icon);
            } else {
                name = userInfo.name;
                final Int32 userId = userInfo.id;
                if (um->GetUserIcon(userId) != NULL) {
                    icon = new BitmapDrawable(context->GetResources(), um->GetUserIcon(userId));
                } else {
                    icon = UserIcons->GetDefaultUserIcon(userId, /* light= */ FALSE);
                }
            }
        }
    }
    private ArrayList<UserDetails> data;
    private final LayoutInflater mInflater;

    public UserSpinnerAdapter(Context context, ArrayList<UserDetails> users) {
        if (users == NULL) {
            throw new IllegalArgumentException("A list of user details must be provided");
        }
        this.data = users;
        mInflater = (LayoutInflater) context->GetSystemService(Context.LAYOUT_INFLATER_SERVICE);
    }

    public UserHandle GetUserHandle(Int32 position) {
        if (position < 0 || position >= data->Size()) {
            return NULL;
        }
        return data->Get(position).mUserHandle;
    }

    //@Override
    public View GetDropDownView(Int32 position, View convertView, ViewGroup parent) {
        final View row = convertView != NULL ? convertView : CreateUser(parent);

        UserDetails user = data->Get(position);
        ((ImageView) row->FindViewById(android.R.id.icon)).SetImageDrawable(user.icon);
        ((TextView) row->FindViewById(android.R.id.title)).SetText(user.name);
        return row;
    }

    private View CreateUser(ViewGroup parent) {
        return mInflater->Inflate(R.layout.user_preference, parent, FALSE);
    }

    //@Override
    CARAPI RegisterDataSetObserver(DataSetObserver observer) {
        // We don't support observers
    }

    //@Override
    CARAPI UnregisterDataSetObserver(DataSetObserver observer) {
        // We don't support observers
    }

    //@Override
    public Int32 GetCount() {
        return data->Size();
    }

    //@Override
    public UserDetails GetItem(Int32 position) {
        return data->Get(position);
    }

    //@Override
    public Int64 GetItemId(Int32 position) {
        return data->Get(position).mUserHandle->GetIdentifier();
    }

    //@Override
    public Boolean HasStableIds() {
        return FALSE;
    }

    //@Override
    public View GetView(Int32 position, View convertView, ViewGroup parent) {
        return GetDropDownView(position, convertView, parent);
    }

    //@Override
    public Int32 GetItemViewType(Int32 position) {
        return 0;
    }

    //@Override
    public Int32 GetViewTypeCount() {
        return 1;
    }

    //@Override
    public Boolean IsEmpty() {
        return data->IsEmpty();
    }
}