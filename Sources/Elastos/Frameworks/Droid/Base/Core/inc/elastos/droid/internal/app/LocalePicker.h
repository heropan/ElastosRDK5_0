
#ifndef __ELASTOS_DROID_INTERNAL_APP_LOCALEPICKER_H__
#define __ELASTOS_DROID_INTERNAL_APP_LOCALEPICKER_H__

#include "ext/frameworkdef.h"
#include "app/ListFragment.h"

#include "widget/ArrayAdapter.h"

using Libcore::ICU::ILocale;
using Elastos::Core::IComparable;
//using Elastos::Text::ICollator;
using Elastos::Droid::App::ListFragment;
using Elastos::Droid::Internal::App::ILocaleSelectionListener;
using Elastos::Droid::Widget::ArrayAdapter;
using Elastos::Droid::Widget::IArrayAdapter;
using Elastos::Droid::Widget::EIID_IArrayAdapter;


namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

class _LocalArrayAdapter
    : public ArrayAdapter
{
public:
    _LocalArrayAdapter(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resource,
        /* [in] */ Int32 textViewResourceId,
        /* [in] */ IObjectContainer* objects,
        /* [in] */ ILayoutInflater* inflater)
        : ArrayAdapter(context, resource, textViewResourceId, objects)
        , mInflater(inflater)
        , mLayoutId(resource)
        , mFieldId(textViewResourceId)
    {}

    CARAPI_(AutoPtr<IView>) GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

private:
    AutoPtr<ILayoutInflater> mInflater;
    Int32 mLayoutId;
    Int32 mFieldId;
};

class LocalArrayAdapter
    : public ElRefBase
    , public _LocalArrayAdapter
    , public IArrayAdapter
{
public:
    CAR_INTERFACE_DECL();

    LocalArrayAdapter(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resource,
        /* [in] */ Int32 textViewResourceId,
        /* [in] */ IObjectContainer* objects,
        /* [in] */ ILayoutInflater* inflater)
        : _LocalArrayAdapter(context, resource, textViewResourceId, objects, inflater)
    {}

    IARRAYADAPTER_METHODS_DECL();
    IBASEADAPTER_METHODS_DECL();
    ILISTADAPTER_METHODS_DECL();
    IADAPTER_METHODS_DECL();
    ISPINNERADAPTER_METHODS_DECL();
};


class LocalePicker
    : public ListFragment
{
public:
    class LocaleInfo
        : public ElRefBase
        , public IComparable
    {
    public:
        CAR_INTERFACE_DECL();

        LocaleInfo(
            /* [in] */ const String& label,
            /* [in] */ ILocale* locale);

        CARAPI GetLabel(
            /* [out] */ String* label);

        CARAPI GetLocale(
            /* [out] */ ILocale** locale);

        CARAPI ToString(
            /* [out] */ String* string);

        CARAPI CompareTo(
            /* [in] */ IInterface* another,
            /* [out] */ Int32* result);

    public:
//        static AutoPtr<ICollator> sCollator;

        String mLabel;
        AutoPtr<ILocale> mLocale;
    };

public:
    LocalePicker()
        : mListener(NULL)
    {}

    static CARAPI Init();

    static CARAPI ConstructAdapter(
        /* [in] */ IContext* context,
        /* [out] */ IArrayAdapter** adapter);

    static CARAPI ConstructAdapter(
        /* [in] */ IContext* context,
        /* [in] */ Int32 layoutId,
        /* [in] */ Int32 fieldId,
        /* [out] */ IArrayAdapter** adapter);

    CARAPI OnActivityCreated(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI SetLocaleSelectionListener(
        /* [in] */ ILocaleSelectionListener* listener);

    CARAPI OnResume();

    CARAPI OnListItemClick(
        /* [in] */ IListView* l,
        /* [in] */ IView* v,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    static CARAPI UpdateLocale(
        /* [in] */ ILocale* locale);

private:
    static CARAPI_(String) ToTitleCase(
        /* [in] */ const String& s);

    static CARAPI_(String) GetDisplayName(
        /* [in] */ ILocale* l,
        /* [in] */ ArrayOf<String>* specialLocaleCodes,
        /* [in] */ ArrayOf<String>* specialLocaleNames);

private:
    static const String TAG;
    static const Boolean DEBUG = FALSE;
    AutoPtr<ILocaleSelectionListener> mListener;
};

} //namespace App
} //namespace Internal
} //namespace Droid
} //namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_APP_LOCALEPICKER_H__
