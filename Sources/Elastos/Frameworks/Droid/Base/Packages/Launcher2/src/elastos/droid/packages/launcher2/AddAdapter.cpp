
#include "elastos/droid/packages/launcher2/AddAdapter.h"
#include "Elastos.Droid.Service.h"
#include "R.h"

namespace Elastos {
namespace Droid {
namespace Packages {
namespace Launcher2 {

AddAdapter::BaseAdapter::ListItem(
    /* [in] */ IResources* res,
    /* [in] */ Int32 textResourceId,
    /* [in] */ Int32 imageResourceId,
    /* [in] */ Int32 actionTag)
{
    res->GetString(textResourceId, &mText);
    if (imageResourceId != -1) {
        res->GetDrawable(imageResourceId, (IDrawable**)&mImage);
    } else {
        mImage = null;
    }
    mActionTag = actionTag;
}

const Int32 AddAdapter::ITEM_SHORTCUT = 0;
const Int32 AddAdapter::ITEM_APPWIDGET = 1;
const Int32 AddAdapter::ITEM_APPLICATION = 2;
const Int32 AddAdapter::ITEM_WALLPAPER = 3;

CAR_INTERFACE_IMPL(AddAdapter, BaseAdapter, IAddAdapter);

AddAdapter::AddAdapter()
{
    CArrayList::New((IArrayList**)&mInflater);
}

ECode AddAdapter::constructor(
    /* [in] */ ILauncher* launcher)
{
    BaseAdapter::constructor();

    AutoPtr<IInterface> obj;
    launcher->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&obj);
    mInflater = ILayoutInflater::Probe(obj);

    // Create default actions
    AutoPtr<IResources> res;
    launcher->GetResources((IResources**)&res);

    AutoPtr<ListItem> item = new ListItem(res,
            Elastos::Droid::Packages::Launcher2::R::string::group_wallpapers,
            Elastos::Droid::Packages::Launcher2::R::mipmap::ic_launcher_wallpaper,
            ITEM_WALLPAPER);
    return mItems->Add(TO_IINTERFACE(item));
}

ECode AddAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);

    AutoPtr<IInterface> obj;
    GetItem(position, (IInterface**)&obj);
    AutoPtr<ListItem> item = (ListItem*)IObject::Probe(obj);

    if (convertView == NULL) {
        mInflater->Inflate(Elastos::Droid::Packages::Launcher2::R::layout::add_list_item,
                parent, FALSE, (IView**)&convertView);
    }

    AutoPtr<ITextView> textView = ITextView::Probe(convertView);
    textView->SetTag(TO_IINTERFACE(item));
    textView->SetText(item->mText);
    textView->SetCompoundDrawablesWithIntrinsicBounds(item->mImage,
            NULL, NULL, NULL);

    *view = convertView;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode AddAdapter::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    return mItems->GetSize(count);
}

ECode AddAdapter::GetItem(
    /* [in] */ Int32 position,
    /* [out] */ IInterface** item)
{
    VALIDATE_NOT_NULL(item);

    return mItems->Get(position, item);
}

ECode AddAdapter::GetItemId(
    /* [in] */ Int32 position,
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);

    *id = mPosition;
    return NOERROR;
}

} // namespace Launcher2
} // namespace Packages
} // namespace Droid
} // namespace Elastos