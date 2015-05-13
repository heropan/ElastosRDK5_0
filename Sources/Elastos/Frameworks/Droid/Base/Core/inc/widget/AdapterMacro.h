#ifndef __ADAPTERMACRO_H__
#define __ADAPTERMACRO_H__

//================== BaseAdapter ===========
#define IBASEADAPTER_METHODS_DECL()                                           \
CARAPI NotifyDataSetChanged();                                                \
                                                                              \
CARAPI NotifyDataSetInvalidated();

#define IBASEADAPTER_METHODS_IMPL(className, superClass)                      \
ECode className::NotifyDataSetChanged()                                       \
{                                                                             \
    return superClass::NotifyDataSetChanged();                                \
}                                                                             \
                                                                              \
ECode className::NotifyDataSetInvalidated()                                   \
{                                                                             \
    return superClass::NotifyDataSetInvalidated();                            \
}

//================== ListAdapter ===========
#define ILISTADAPTER_METHODS_DECL()                                           \
CARAPI AreAllItemsEnabled(                                                    \
    /* [out] */ Boolean* enabled);                                            \
                                                                              \
CARAPI IsEnabled(                                                             \
    /* [in] */ Int32 position,                                                \
    /* [out] */ Boolean* enabled);

#define ILISTADAPTER_METHODS_IMPL(className, superClass)                      \
ECode className::AreAllItemsEnabled(                                          \
    /* [out] */ Boolean* enabled)                                             \
{                                                                             \
    VALIDATE_NOT_NULL(enabled);                                               \
                                                                              \
    *enabled = superClass::AreAllItemsEnabled();                              \
    return NOERROR;                                                           \
}                                                                             \
                                                                              \
ECode className::IsEnabled(                                                   \
    /* [in] */ Int32 position,                                                \
    /* [out] */ Boolean* enabled)                                             \
{                                                                             \
    VALIDATE_NOT_NULL(enabled);                                               \
                                                                              \
    *enabled = superClass::IsEnabled(position);                               \
    return NOERROR;                                                           \
}

//================== Adapter ===========
#define IADAPTER_METHODS_DECL()                                               \
CARAPI RegisterDataSetObserver(                                               \
    /* [in] */ IDataSetObserver* observer);                                   \
                                                                              \
CARAPI UnregisterDataSetObserver(                                             \
    /* [in] */ IDataSetObserver* observer);                                   \
                                                                              \
CARAPI GetCount(                                                              \
    /* [out] */ Int32* count);                                                \
                                                                              \
CARAPI GetItem(                                                               \
    /* [in] */ Int32 position,                                                \
    /* [out] */ IInterface** item);                                           \
                                                                              \
CARAPI GetItemId(                                                             \
    /* [in] */ Int32 position,                                                \
    /* [out] */ Int64* itemId);                                               \
                                                                              \
CARAPI HasStableIds(                                                          \
    /* [out] */ Boolean* hasStableIds);                                       \
                                                                              \
CARAPI GetView(                                                               \
    /* [in] */ Int32 position,                                                \
    /* [in] */ IView* convertView,                                            \
    /* [in] */ IViewGroup* parent,                                            \
    /* [out] */ IView** view);                                                \
                                                                              \
CARAPI GetItemViewType(                                                       \
    /* [in] */ Int32 position,                                                \
    /* [out] */ Int32* viewType);                                             \
                                                                              \
CARAPI GetViewTypeCount(                                                      \
    /* [out] */ Int32* count);                                                \
                                                                              \
CARAPI IsEmpty(                                                               \
    /* [out] */ Boolean* isEmpty);

#define IADAPTER_METHODS_IMPL(className, superClass)                          \
ECode className::RegisterDataSetObserver(                                     \
    /* [in] */ IDataSetObserver* observer)                                    \
{                                                                             \
    return superClass::RegisterDataSetObserver(observer);                     \
}                                                                             \
                                                                              \
ECode className::UnregisterDataSetObserver(                                   \
    /* [in] */ IDataSetObserver* observer)                                    \
{                                                                             \
    return superClass::UnregisterDataSetObserver(observer);                   \
}                                                                             \
                                                                              \
ECode className::GetCount(                                                    \
    /* [out] */ Int32* count)                                                 \
{                                                                             \
    VALIDATE_NOT_NULL(count);                                                 \
                                                                              \
    *count = superClass::GetCount();                                          \
    return NOERROR;                                                           \
}                                                                             \
                                                                              \
ECode className::GetItem(                                                     \
    /* [in] */ Int32 position,                                                \
    /* [out] */ IInterface** item)                                            \
{                                                                             \
    VALIDATE_NOT_NULL(item);                                                  \
    AutoPtr<IInterface> temp = superClass::GetItem(position);                 \
    *item = temp;                                                             \
    if(*item)                                                                 \
        (*item)->AddRef();                                                    \
    return NOERROR;                                                           \
}                                                                             \
                                                                              \
ECode className::GetItemId(                                                   \
    /* [in] */ Int32 position,                                                \
    /* [out] */ Int64* itemId)                                                \
{                                                                             \
    VALIDATE_NOT_NULL(itemId);                                                \
    *itemId = superClass::GetItemId(position);                                \
    return NOERROR;                                                           \
}                                                                             \
                                                                              \
ECode className::HasStableIds(                                                \
    /* [out] */ Boolean* hasStableIds)                                        \
{                                                                             \
    VALIDATE_NOT_NULL(hasStableIds);                                          \
                                                                              \
    *hasStableIds = superClass::HasStableIds();                               \
    return NOERROR;                                                           \
}                                                                             \
                                                                              \
ECode className::GetView(                                                     \
    /* [in] */ Int32 position,                                                \
    /* [in] */ IView* convertView,                                            \
    /* [in] */ IViewGroup* parent,                                            \
    /* [out] */ IView** view)                                                 \
{                                                                             \
    AutoPtr<IView> v = superClass::GetView(position, convertView, parent);    \
    *view = v;                                                                \
    if (*view != NULL) {                                                      \
        (*view)->AddRef();                                                    \
    }                                                                         \
    return NOERROR;                                                           \
}                                                                             \
                                                                              \
ECode className::GetItemViewType(                                             \
    /* [in] */ Int32 position,                                                \
    /* [out] */ Int32* viewType)                                              \
{                                                                             \
    VALIDATE_NOT_NULL(viewType);                                              \
    *viewType = superClass::GetItemViewType(position);                        \
    return NOERROR;                                                           \
}                                                                             \
                                                                              \
ECode className::GetViewTypeCount(                                            \
    /* [out] */ Int32* count)                                                 \
{                                                                             \
    VALIDATE_NOT_NULL(count);                                                 \
                                                                              \
    *count = superClass::GetViewTypeCount();                                  \
    return NOERROR;                                                           \
}                                                                             \
                                                                              \
ECode className::IsEmpty(                                                     \
/* [out] */ Boolean* isEmpty)                                                 \
{                                                                             \
    VALIDATE_NOT_NULL(isEmpty);                                               \
                                                                              \
    *isEmpty = superClass::IsEmpty();                                         \
    return NOERROR;                                                           \
}

//================== ArrayAdapter ===========
#define IARRAYADAPTER_METHODS_DECL()                                          \
CARAPI Add(                                                                   \
    /* [in] */ IInterface* object);                                           \
                                                                              \
CARAPI AddAll(                                                                \
    /* [in] */ IObjectContainer* collection);                                 \
                                                                              \
CARAPI AddAllEx(                                                              \
    /* [in] */ ArrayOf<IInterface* >* items) ;                                \
                                                                              \
CARAPI Insert(                                                                \
    /* [in] */ IInterface* object,                                            \
    /* [in] */ Int32 index);                                                  \
                                                                              \
CARAPI Remove(                                                                \
    /* [in] */ IInterface* object);                                           \
                                                                              \
CARAPI Clear();                                                               \
                                                                              \
CARAPI Sort(                                                                  \
    /* [in] */ IComparator* comparator);                                      \
                                                                              \
CARAPI SetNotifyOnChange(                                                     \
    /* [in] */ Boolean notifyOnChange);                                       \
                                                                              \
CARAPI GetContext(                                                            \
    /* [out] */ IContext** context);                                          \
                                                                              \
CARAPI GetPosition(                                                           \
    /* [in] */ IInterface* item,                                              \
    /* [out] */ Int32* position);                                             \
                                                                              \
CARAPI SetDropDownViewResource(                                               \
    /* [in] */ Int32 resource);

#define IARRAYADAPTER_METHODS_IMPL(className, superClass)                     \
ECode className::Add(                                                         \
    /* [in] */ IInterface* object)                                            \
{                                                                             \
    return superClass::Add(object);                                           \
}                                                                             \
                                                                              \
ECode className::AddAll(                                                      \
    /* [in] */ IObjectContainer* collection)                                  \
{                                                                             \
    return superClass::AddAll(collection);                                    \
}                                                                             \
                                                                              \
ECode className::AddAllEx(                                                    \
    /* [in] */ ArrayOf<IInterface* >* items)                                  \
{                                                                             \
    return superClass::AddAllEx(items);                                       \
}                                                                             \
                                                                              \
ECode className::Insert(                                                      \
    /* [in] */ IInterface* object,                                            \
    /* [in] */ Int32 index)                                                   \
{                                                                             \
    return superClass::Insert(object, index);                                 \
}                                                                             \
                                                                              \
ECode className::Remove(                                                      \
    /* [in] */ IInterface* object)                                            \
{                                                                             \
    return superClass::Remove(object);                                        \
}                                                                             \
                                                                              \
ECode className::Clear()                                                      \
{                                                                             \
    return superClass::Clear();                                               \
}                                                                             \
                                                                              \
ECode className::Sort(                                                        \
    /* [in] */ IComparator* comparator)                                       \
{                                                                             \
    return superClass::Sort(comparator);                                      \
}                                                                             \
                                                                              \
ECode className::SetNotifyOnChange(                                           \
    /* [in] */ Boolean notifyOnChange)                                        \
{                                                                             \
    return superClass::SetNotifyOnChange(notifyOnChange);                     \
}                                                                             \
                                                                              \
ECode className::GetContext(                                                  \
    /* [out] */ IContext** context)                                           \
{                                                                             \
    VALIDATE_NOT_NULL(context);                                               \
    AutoPtr<IContext> temp = superClass::GetContext();                        \
    *context = temp;                                                          \
    if(*context)                                                              \
        (*context)->AddRef();                                                 \
    return NOERROR;                                                           \
}                                                                             \
                                                                              \
ECode className::GetPosition(                                                 \
    /* [in] */ IInterface* item,                                              \
    /* [out] */ Int32* position)                                              \
{                                                                             \
    VALIDATE_NOT_NULL(position);                                              \
                                                                              \
    *position = superClass::GetPosition(item);                                \
    return NOERROR;                                                           \
}                                                                             \
                                                                              \
ECode className::SetDropDownViewResource(                                     \
    /* [in] */ Int32 resource)                                                \
{                                                                             \
    return superClass::SetDropDownViewResource(resource);                     \
}

//================== SpinnerAdapter ===========
#define ISPINNERADAPTER_METHODS_DECL()                                        \
CARAPI GetDropDownView(                                                       \
    /* [in] */ Int32 position,                                                \
    /* [in] */ IView* convertView,                                            \
    /* [in] */ IViewGroup* parent,                                            \
    /* [out] */ IView** view);

#define ISPINNERADAPTER_METHODS_IMPL(className, superClass)                   \
ECode className::GetDropDownView(                                             \
    /* [in] */ Int32 position,                                                \
    /* [in] */ IView* convertView,                                            \
    /* [in] */ IViewGroup* parent,                                            \
    /* [out] */ IView** view)                                                 \
{                                                                             \
    VALIDATE_NOT_NULL(view);                                                  \
    AutoPtr<IView> v;                                                         \
    v = superClass::GetDropDownView(position, convertView, parent);           \
    *view = v;                                                                \
    INTERFACE_ADDREF(*view);                                                  \
    return NOERROR;                                                           \
}

//================== CursorAdapter ===========
#define ICURSORADAPTER_METHODS_DECL()                                         \
CARAPI NewView(                                                               \
    /* [in] */ IContext* context,                                             \
    /* [in] */ ICursor* cursor,                                               \
    /* [in] */ IViewGroup* parent,                                            \
    /* [out] */ IView** view);                                                \
                                                                              \
CARAPI NewDropDownView(                                                       \
    /* [in] */ IContext* context,                                             \
    /* [in] */ ICursor* cursor,                                               \
    /* [in] */ IViewGroup* parent,                                            \
    /* [out] */ IView** view);                                                \
                                                                              \
CARAPI BindView(                                                              \
    /* [in] */ IView* view,                                                   \
    /* [in] */ IContext* context,                                             \
    /* [in] */ ICursor* cursor);                                              \
                                                                              \
CARAPI GetFilterQueryProvider(                                                \
    /* [out] */ IFilterQueryProvider** filterQueryProvider);                  \
                                                                              \
CARAPI SetFilterQueryProvider(                                                \
    /* [in] */ IFilterQueryProvider* filterQueryProvider);                    \
                                                                              \
CARAPI SwapCursor(                                                            \
    /* [in] */ ICursor* newCursor,                                            \
    /* [out] */ ICursor** cursor);

#define ICURSORADAPTER_METHODS_IMPL(className, superClass)                    \
ECode className::NewView(                                                     \
    /* [in] */ IContext* context,                                             \
    /* [in] */ ICursor* cursor,                                               \
    /* [in] */ IViewGroup* parent,                                            \
    /* [out] */ IView** view)                                                 \
{                                                                             \
    VALIDATE_NOT_NULL(view);                                                  \
    AutoPtr<IView> v;                                                         \
    v = superClass::NewView(context, cursor, parent);                         \
    *view = v;                                                                \
    INTERFACE_ADDREF(*view);                                                  \
    return NOERROR;                                                           \
}                                                                             \
                                                                              \
ECode className::NewDropDownView(                                             \
    /* [in] */ IContext* context,                                             \
    /* [in] */ ICursor* cursor,                                               \
    /* [in] */ IViewGroup* parent,                                            \
    /* [out] */ IView** view)                                                 \
{                                                                             \
    VALIDATE_NOT_NULL(view);                                                  \
    AutoPtr<IView> v;                                                         \
    v = superClass::NewDropDownView(context, cursor, parent);                 \
    *view = v;                                                                \
    INTERFACE_ADDREF(*view);                                                  \
    return NOERROR;                                                           \
}                                                                             \
                                                                              \
ECode className::BindView(                                                    \
    /* [in] */ IView* view,                                                   \
    /* [in] */ IContext* context,                                             \
    /* [in] */ ICursor* cursor)                                               \
{                                                                             \
    return superClass::BindView(view, context, cursor);                       \
}                                                                             \
                                                                              \
ECode className::GetFilterQueryProvider(                                      \
    /* [out] */ IFilterQueryProvider** filterQueryProvider)                   \
{                                                                             \
    VALIDATE_NOT_NULL(filterQueryProvider);                                   \
    AutoPtr<IFilterQueryProvider> v;                                          \
    v = superClass::GetFilterQueryProvider();                                 \
    *filterQueryProvider = v;                                                 \
    INTERFACE_ADDREF(*filterQueryProvider);                                   \
    return NOERROR;                                                           \
}                                                                             \
                                                                              \
ECode className::SetFilterQueryProvider(                                      \
    /* [in] */ IFilterQueryProvider* filterQueryProvider)                     \
{                                                                             \
    return superClass::SetFilterQueryProvider(filterQueryProvider);           \
}                                                                             \
                                                                              \
ECode className::SwapCursor(                                                  \
    /* [in] */ ICursor* newCursor,                                            \
    /* [out] */ ICursor** cursor)                                             \
{                                                                             \
    VALIDATE_NOT_NULL(cursor);                                                \
    AutoPtr<ICursor> v;                                                       \
    v = superClass::SwapCursor(newCursor);                                    \
    *cursor = v;                                                              \
    INTERFACE_ADDREF(*cursor);                                                \
    return NOERROR;                                                           \
}

//================== ResourceCursorAdapter ===========
#define IRESOURCECURSORADAPTER_METHODS_DECL()                               \
CARAPI SetViewResource(                                                     \
    /* [in] */ Int32 layout);                                               \
                                                                            \
CARAPI SetDropDownViewResource(                                             \
    /* [in] */ Int32 dropDownLayout);

#define IRESOURCECURSORADAPTER_METHODS_IMPL(className, superClass)          \
ECode className::SetViewResource(                                           \
    /* [in] */ Int32 layout)                                                \
{                                                                           \
    return superClass::SetViewResource(layout);                             \
}                                                                           \
                                                                            \
ECode className::SetDropDownViewResource(                                   \
    /* [in] */ Int32 dropDownLayout)                                        \
{                                                                           \
    return superClass::SetDropDownViewResource(dropDownLayout);             \
}

//================== SimpleCursorAdapter ===========
#define ISIMPLECURSORADAPTER_METHODS_DECL()                                 \
CARAPI GetViewBinder(                                                       \
    /* [out] */ ISimpleCursorAdapterViewBinder** viewBinder);               \
                                                                            \
CARAPI SetViewBinder(                                                       \
    /* [in] */ ISimpleCursorAdapterViewBinder* viewBinder);                 \
                                                                            \
CARAPI SetViewImage(                                                        \
    /* [in] */ IImageView* v,                                               \
    /* [in] */ const String& value);                                        \
                                                                            \
CARAPI SetViewText(                                                         \
    /* [in] */ ITextView* v,                                                \
    /* [in] */ const String& text);                                         \
                                                                            \
CARAPI GetStringConversionColumn(                                           \
    /* [out] */ Int32* index);                                              \
                                                                            \
CARAPI SetStringConversionColumn(                                           \
    /* [in] */ Int32 stringConversionColumn);                               \
                                                                            \
CARAPI GetCursorToStringConverter(                                          \
    /* [out] */ ICursorToStringConverter** cursorToStringConverter);        \
                                                                            \
CARAPI SetCursorToStringConverter(                                          \
    /* [in] */ ICursorToStringConverter* cursorToStringConverter);          \
                                                                            \
CARAPI ChangeCursorAndColumns(                                              \
    /* [in] */ ICursor* c,                                                  \
    /* [in] */ ArrayOf<String>* from,                                       \
    /* [in] */ ArrayOf<Int32>* to);

#define ISIMPLECURSORADAPTER_METHODS_IMPL(className, superClass)            \
ECode className::GetViewBinder(                                             \
    /* [out] */ ISimpleCursorAdapterViewBinder** viewBinder)                \
{                                                                           \
    VALIDATE_NOT_NULL(viewBinder);                                          \
    AutoPtr<ISimpleCursorAdapterViewBinder> v;                              \
    v = superClass::GetViewBinder();                                        \
    *viewBinder = v;                                                        \
    INTERFACE_ADDREF(*viewBinder);                                          \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetViewBinder(                                             \
    /* [in] */ ISimpleCursorAdapterViewBinder* viewBinder)                  \
{                                                                           \
    return superClass::SetViewBinder(viewBinder);                           \
}                                                                           \
                                                                            \
ECode className::SetViewImage(                                              \
    /* [in] */ IImageView* v,                                               \
    /* [in] */ const String& value)                                         \
{                                                                           \
    return superClass::SetViewImage(v, value);                              \
}                                                                           \
                                                                            \
ECode className::SetViewText(                                               \
    /* [in] */ ITextView* v,                                                \
    /* [in] */ const String& text)                                          \
{                                                                           \
    return superClass::SetViewText(v,  text);                               \
}                                                                           \
                                                                            \
ECode className::GetStringConversionColumn(                                 \
    /* [out] */ Int32* index)                                               \
{                                                                           \
    VALIDATE_NOT_NULL(index);                                               \
    Int32 idx = superClass::GetStringConversionColumn();                    \
    *index = idx;                                                           \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetStringConversionColumn(                                 \
    /* [in] */ Int32 stringConversionColumn)                                \
{                                                                           \
    return superClass::SetStringConversionColumn(stringConversionColumn);   \
}                                                                           \
                                                                            \
ECode className::GetCursorToStringConverter(                                \
    /* [out] */ ICursorToStringConverter** c)                               \
{                                                                           \
    VALIDATE_NOT_NULL(c);                                                   \
    AutoPtr<ICursorToStringConverter> v;                                    \
    v = superClass::GetCursorToStringConverter();                           \
    *c = v;                                                                 \
    INTERFACE_ADDREF(*c);                                                   \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetCursorToStringConverter(                                \
    /* [in] */ ICursorToStringConverter* converter)                         \
{                                                                           \
    return superClass::SetCursorToStringConverter(converter);               \
}                                                                           \
                                                                            \
ECode className::ChangeCursorAndColumns(                                    \
    /* [in] */ ICursor* c,                                                  \
    /* [in] */ ArrayOf<String>* from,                                       \
    /* [in] */ ArrayOf<Int32>* to)                                          \
{                                                                           \
    return superClass::ChangeCursorAndColumns(c, from, to);                 \
}

#endif //__ADAPTERMACRO_H__
