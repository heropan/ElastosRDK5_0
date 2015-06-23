
#include "provider/CContactsPeople.h"
#include "provider/CContactsGroups.h"
#include "provider/CContactsGroupMembership.h"
#include "net/Uri.h"
#include "content/CContentUris.h"
#include "content/CContentValues.h"
#include "graphics/CBitmapFactory.h"
#include <elastos/core/StringUtils.h>

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::CArrayOf;
using Elastos::Core::CByte;
using Elastos::Core::EIID_IByte;
using Elastos::Core::CInteger64;
using Elastos::Core::StringUtils;
using Elastos::Core::CStringWrapper;
using Elastos::IO::IByteArrayInputStream;
using Elastos::IO::CByteArrayInputStream;
using Elastos::Droid::Net::Uri;
using Elastos::Droid::Content::CContentUris;
using Elastos::Droid::Content::IContentUris;
using Elastos::Droid::Content::IContentValues;
using Elastos::Droid::Content::CContentValues;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;
using Elastos::Droid::Provider::IBaseColumns;
using Elastos::Droid::Provider::IContactsGroups;
using Elastos::Droid::Provider::CContactsGroups;
using Elastos::Droid::Provider::IContactsGroupMembership;
using Elastos::Droid::Provider::CContactsGroupMembership;

namespace Elastos {
namespace Droid {
namespace Provider {

static AutoPtr<ArrayOf<String> > initGROUPSPROJECTION()
{
    AutoPtr<ArrayOf<String> > args = ArrayOf<String>::Alloc(1);
    (*args)[0] = IBaseColumns::ID;
    return args;
}

AutoPtr<ArrayOf<String> > CContactsPeople::GROUPS_PROJECTION = initGROUPSPROJECTION();

ECode CContactsPeople::constructor()
{
    return NOERROR;
}

ECode CContactsPeople::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://contacts/people"), uri);
}

ECode CContactsPeople::GetCONTENTFILTERURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://contacts/people/filter"), uri);
}

ECode CContactsPeople::GetDELETEDCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://contacts/deleted_people"), uri);
}

ECode CContactsPeople::GetWITHEMAILORIMFILTERURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://contacts/people/with_email_or_im_filter"), uri);
}

ECode CContactsPeople::GetDEFAULTSORTORDER(
    /* [out] */ String* order)
{
    VALIDATE_NOT_NULL(order);

    *order = IContactsPeopleColumns::NAME + String(" ASC");
    return NOERROR;
}

ECode CContactsPeople::MarkAsContacted(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ Int64 personId)
{
    AutoPtr<IUri> _uri, uri, contentUri;
    GetCONTENTURI((IUri**)&contentUri);
    AutoPtr<IContentUris> helper;
    CContentUris::AcquireSingleton((IContentUris**)&helper);
    helper->WithAppendedId(contentUri, personId, (IUri**)&_uri);
    Uri::WithAppendedPath(_uri, String("update_contact_time"), (IUri**)&uri);
    AutoPtr<IContentValues> values;
    CContentValues::New((IContentValues**)&values);
    // There is a trigger in place that will update TIMES_CONTACTED when
    // LAST_TIME_CONTACTED is modified.
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 now;
    system->GetCurrentTimeMillis(&now);
    AutoPtr<IInteger64> num;
    CInteger64::New(now, (IInteger64**)&num);
    values->PutInt64(IContactsPeopleColumns::LAST_TIME_CONTACTED, num);
    Int32 result;
    return resolver->Update(uri, values, String(NULL), NULL, &result);
}

ECode CContactsPeople::TryGetMyContactsGroupId(
    /* [in] */ IContentResolver* resolver,
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);

    AutoPtr<ICursor> groupsCursor;
    AutoPtr<IContactsGroups> helper;
    CContactsGroups::AcquireSingleton((IContactsGroups**)&helper);
    AutoPtr<IUri> uri;
    helper->GetCONTENTURI((IUri**)&uri);
    resolver->Query(uri, GROUPS_PROJECTION,
        IContactsGroupsColumns::SYSTEM_ID + String("='") + IContactsGroups::GROUP_MY_CONTACTS + String("'"),
        NULL, String(NULL), (ICursor**)&groupsCursor);
    if (groupsCursor != NULL) {
        //try {
        Boolean result;
        ECode ec = groupsCursor->MoveToFirst(&result);
        if (SUCCEEDED(ec) && result) {
            groupsCursor->GetInt64(0, id);
            groupsCursor->Close();
            return NOERROR;
        }
        //} finally {
        groupsCursor->Close();
        //}
    }
    *id = 0;
    return NOERROR;
}

ECode CContactsPeople::AddToMyContactsGroup(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ Int64 personId,
    /* [out] */ IUri** uri)
{
    Int64 groupId;
    TryGetMyContactsGroupId(resolver, &groupId);
    if (groupId == 0) {
        //throw new IllegalStateException("Failed to find the My Contacts group");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return AddToGroup(resolver, personId, groupId, uri);
}

ECode CContactsPeople::AddToGroup(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ Int64 personId,
    /* [in] */ const String& groupName,
    /* [out] */ IUri** uri)
{
    Int64 groupId = 0;
    AutoPtr<IContactsGroups> helper;
    CContactsGroups::AcquireSingleton((IContactsGroups**)&helper);
    AutoPtr<IUri> _uri;
    helper->GetCONTENTURI((IUri**)&_uri);
    AutoPtr<ArrayOf<String> > args = ArrayOf<String>::Alloc(1);
    (*args)[0] = groupName;
    AutoPtr<ICursor> groupsCursor;
    resolver->Query(_uri, GROUPS_PROJECTION,
        IContactsGroupsColumns::NAME + String("=?"), args, String(NULL), (ICursor**)&groupsCursor);
    if (groupsCursor != NULL) {
        //try {
        Boolean result;
        if (groupsCursor->MoveToFirst(&result), result) {
            groupsCursor->GetInt64(0, &groupId);
        }
        //} finally {
        groupsCursor->Close();
        //}
    }

    if (groupId == 0) {
        //throw new IllegalStateException("Failed to find the My Contacts group");
        *uri = NULL;
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return AddToGroup(resolver, personId, groupId, uri);
}

ECode CContactsPeople::AddToGroup(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ Int64 personId,
    /* [in] */ Int64 groupId,
    /* [out] */ IUri** uri)
{
    AutoPtr<IContentValues> values;
    CContentValues::New((IContentValues**)&values);
    AutoPtr<IInteger64> person;
    CInteger64::New(personId, (IInteger64**)&person);
    values->PutInt64(IContactsGroupMembership::PERSON_ID, person);
    AutoPtr<IInteger64> group;
    CInteger64::New(groupId, (IInteger64**)&group);
    values->PutInt64(IContactsGroupMembership::GROUP_ID, group);

    AutoPtr<IUri> _uri;
    AutoPtr<IContactsGroupMembership> helper;
    CContactsGroupMembership::AcquireSingleton((IContactsGroupMembership**)&helper);
    helper->GetCONTENTURI((IUri**)&_uri);
    return resolver->Insert(_uri, values, uri);
}

ECode CContactsPeople::CreatePersonInMyContactsGroup(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IContentValues* values,
    /* [out] */ IUri** uri)
{
    AutoPtr<IUri> _uri;
    GetCONTENTURI((IUri**)&_uri);
    AutoPtr<IUri> contactUri;
    resolver->Insert(_uri, values, (IUri**)&contactUri);
    if (contactUri == NULL) {
        //Log.e(TAG, "Failed to create the contact");
        *uri = NULL;
        return NOERROR;
    }

    AutoPtr<IContentUris> helper;
    CContentUris::AcquireSingleton((IContentUris**)&helper);
    Int64 id;
    helper->ParseId(contactUri, &id);
    AutoPtr<IUri> groupUri;
    AddToMyContactsGroup(resolver, id, (IUri**)&groupUri);
    if (groupUri == NULL) {
        Int32 result;
        resolver->Delete(contactUri, String(NULL), NULL, &result);
        *uri = NULL;
        return NOERROR;
    }
    *uri =  contactUri;
    REFCOUNT_ADD(*uri);
    return NOERROR;
}

ECode CContactsPeople::QueryGroups(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ Int64 person,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor);

    AutoPtr<IContactsGroupMembership> helper;
    CContactsGroupMembership::AcquireSingleton((IContactsGroupMembership**)&helper);
    AutoPtr<IUri> uri;
    helper->GetCONTENTURI((IUri**)&uri);
    AutoPtr<ArrayOf<String> > args = ArrayOf<String>::Alloc(1);
    (*args)[0] = StringUtils::Int64ToString(person);
    return resolver->Query(uri, NULL, String("person=?"),
            args, IContactsGroups::DEFAULT_SORT_ORDER, cursor);
}

ECode CContactsPeople::SetPhotoData(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* person,
    /* [in] */ const ArrayOf<Byte>& data)
{
    AutoPtr<IUri> photoUri;
    Uri::WithAppendedPath(person, IContactsPhotos::CONTENT_DIRECTORY, (IUri**)&photoUri);

    AutoPtr<IContentValues> values;
    CContentValues::New((IContentValues**)&values);
    AutoPtr<IArrayOf> array;
    CArrayOf::New(EIID_IByte, data.GetLength(), (IArrayOf**)&array);
    for (Int32 i = 0; i < data.GetLength(); i++) {
        AutoPtr<IByte> byteObj;
        CByte::New(data[i], (IByte**)&byteObj);
        array->Put(i, byteObj);
    }
    values->PutBytes(IContactsPhotosColumns::DATA, array);
    Int32 result;
    return cr->Update(photoUri, values, String(NULL), NULL, &result);
}

ECode CContactsPeople::OpenContactPhotoInputStream(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* person,
    /* [out] */ IInputStream** stream)
{
    AutoPtr<IUri> photoUri;
    Uri::WithAppendedPath(person, IContactsPhotos::CONTENT_DIRECTORY, (IUri**)&photoUri);

    AutoPtr<ArrayOf<String> > args = ArrayOf<String>::Alloc(1);
    (*args)[0] = IContactsPhotosColumns::DATA;
    AutoPtr<ICursor> cursor;
    cr->Query(photoUri, args, String(NULL), NULL, String(NULL), (ICursor**)&cursor);
    Boolean result;
    if (cursor == NULL || (cursor->MoveToNext(&result), !result)) {
        if (cursor != NULL) cursor->Close();
        *stream = NULL;
        return NOERROR;
    }
    AutoPtr<ArrayOf<Byte> > data;
    cursor->GetBlob(0, (ArrayOf<Byte>**)&data);
    if (data == NULL) {
        if (cursor != NULL) cursor->Close();
        *stream = NULL;
        return NOERROR;
    }
    AutoPtr<IByteArrayInputStream> _stream;
    CByteArrayInputStream::New(data, (IByteArrayInputStream**)&_stream);
    *stream = IInputStream::Probe(_stream);
    REFCOUNT_ADD(*stream);
    if (cursor != NULL) cursor->Close();
    return NOERROR;
}

ECode CContactsPeople::LoadContactPhoto(
    /* [in] */ IContext* context,
    /* [in] */ IUri* person,
    /* [in] */ Int32 placeholderImageResource,
    /* [in] */ IBitmapFactoryOptions* options,
    /* [out] */ IBitmap** bitmap)
{
    if (person == NULL) {
        AutoPtr<IBitmap> bm = LoadPlaceholderPhoto(placeholderImageResource, context, options);
        *bitmap = bm;
        REFCOUNT_ADD(*bitmap);
        return NOERROR;
    }

    AutoPtr<IContentResolver> resolver;
    context->GetContentResolver((IContentResolver**)&resolver);
    AutoPtr<IInputStream> stream;
    OpenContactPhotoInputStream(resolver, person, (IInputStream**)&stream);
    AutoPtr<IBitmap> bm;
    if (stream != NULL) {
        AutoPtr<IBitmapFactory> helper;
        CBitmapFactory::AcquireSingleton((IBitmapFactory**)&helper);
        helper->DecodeStream(stream, NULL, options, (IBitmap**)&bm);
    }
    if (bm == NULL) {
        bm = LoadPlaceholderPhoto(placeholderImageResource, context, options);
    }
    *bitmap = bm;
    REFCOUNT_ADD(*bitmap);
    return NOERROR;
}

AutoPtr<IBitmap> CContactsPeople::LoadPlaceholderPhoto(
    /* [in] */ Int32 placeholderImageResource,
    /* [in] */ IContext* context,
    /* [in] */ IBitmapFactoryOptions* options)
{
    if (placeholderImageResource == 0) {
        return NULL;
    }
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IBitmapFactory> helper;
    CBitmapFactory::AcquireSingleton((IBitmapFactory**)&helper);
    AutoPtr<IBitmap> bm;
    helper->DecodeResource(res, placeholderImageResource, options, (IBitmap**)&bm);
    return bm;
}

} // Provider
} // Droid
} // Elastos
