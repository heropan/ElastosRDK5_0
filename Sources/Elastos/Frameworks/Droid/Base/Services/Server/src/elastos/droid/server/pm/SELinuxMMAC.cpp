
#include "elastos/droid/server/pm/SELinuxMMAC.h"
#include "elastos/droid/os/Environment.h"
#include "elastos/droid/utility/Xml.h"
#include "elastos/droid/internal/utility/XmlUtils.h"
#include <elastos/utility/logging/Slogger.h>
#include <Elastos.CoreLibrary.Security.h>
#include <Elastos.CoreLibrary.Libcore.h>

using Elastos::Droid::Content::Pm::CSignature;
using Elastos::Droid::Os::Environment;
using Elastos::Droid::Utility::Xml;
using Elastos::Droid::Internal::Utility::XmlUtils;
using Elastos::Security::IMessageDigestHelper;
using Elastos::Security::CMessageDigestHelper;
using Elastos::Security::IMessageDigest;
using Elastos::Utility::Logging::Slogger;
using Elastos::IO::IFileReader;
using Elastos::IO::CFileReader;
using Elastos::IO::ICloseable;
using Elastos::IO::CFile;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::CFileHelper;
using Elastos::IO::IFileHelper;
using Elastos::IO::IFileDescriptor;
using Libcore::IO::IIoUtils;
using Libcore::IO::CIoUtils;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

//==============================================================================
//                  SELinuxMMAC::Policy
//==============================================================================

void SELinuxMMAC::Policy::PutSeinfo(
    /* [in] */ const String& seinfoValue)
{
    mSeinfo = seinfoValue;
}

void SELinuxMMAC::Policy::PutPkg(
    /* [in] */ const String& pkg,
    /* [in] */ const String& seinfoValue)
{
    mPkgMap[pkg] = seinfoValue;
}

Boolean SELinuxMMAC::Policy::IsValid()
{
    return (!mSeinfo.IsNull() || !mPkgMap.IsEmpty());
}

String SELinuxMMAC::Policy::CheckPolicy(
    /* [in] */ const String& pkgName)
{
    // Check for package name seinfo value first.
    String seinfoValue(NULL);
    HashMap<String, String>::Iterator it = mPkgMap.Find(pkgName);
    if (it != mPkgMap.End()) {
        seinfoValue = it->mSecond;
    }
    if (!seinfoValue.IsNull()) {
        return seinfoValue;
    }

    // Return the global seinfo value.
    return mSeinfo;
}


//==============================================================================
//                  SELinuxMMAC
//==============================================================================

Boolean SELinuxMMAC::UseOverridePolicy()
{
    // try {
    AutoPtr<IIoUtils> ioUtils;
    CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
    String overrideVersion;
    if (FAILED(ioUtils->ReadFileAsString(DATA_VERSION_FILE, &overrideVersion))) {
        return FALSE;
    }
    String baseVersion;
    if (FAILED(ioUtils->ReadFileAsString(BASE_VERSION_FILE, &baseVersion))) {
        return FALSE;
    }
    if (overrideVersion.Equals(baseVersion)) {
        return TRUE;
    }
    Slogger::E(TAG, "Override policy version '%s' doesn't match base version '%s'. Skipping override policy files.",
            overrideVersion.string(), baseVersion.string());
    // } catch (FileNotFoundException fnfe) {
    //     // Override version file doesn't have to exist so silently ignore.
    // } catch (IOException ioe) {
    //     Slog.w(TAG, "Skipping override policy files.", ioe);
    // }
    return FALSE;
}

const String SELinuxMMAC::TAG("SELinuxMMAC");
const Boolean SELinuxMMAC::DEBUG_POLICY;
const Boolean SELinuxMMAC::DEBUG_POLICY_INSTALL;
HashMap<AutoPtr<ISignature>, AutoPtr<SELinuxMMAC::Policy> > SELinuxMMAC::sSigSeinfo;
String SELinuxMMAC::sDefaultSeinfo(NULL);

static String InitStaticAttr(
    /* [in] */ Int32 mode,
    /* [in] */ const char* str)
{
    AutoPtr<IFile> file;
    switch (mode) {
        case 1:
            file = Environment::GetDataDirectory();
        case 2:
            file = Environment::GetRootDirectory();
    }
    String fileStr;
    IObject::Probe(file)->ToString(&fileStr);
    return fileStr + str;

}
const String SELinuxMMAC::DATA_VERSION_FILE = InitStaticAttr(1, "/security/current/selinux_version");
const String SELinuxMMAC::BASE_VERSION_FILE("/selinux_version");
const Boolean SELinuxMMAC::USE_OVERRIDE_POLICY = SELinuxMMAC::UseOverridePolicy();
const String SELinuxMMAC::DATA_MAC_PERMISSIONS = InitStaticAttr(1, "/security/current/mac_permissions.xml");
const String SELinuxMMAC::BASE_MAC_PERMISSIONS = InitStaticAttr(2, "/etc/security/mac_permissions.xml");
const String SELinuxMMAC::MAC_PERMISSIONS = USE_OVERRIDE_POLICY ? DATA_MAC_PERMISSIONS : BASE_MAC_PERMISSIONS;
const String SELinuxMMAC::DATA_SEAPP_CONTEXTS = InitStaticAttr(1, "/security/current/seapp_contexts");
const String SELinuxMMAC::BASE_SEAPP_CONTEXTS("/seapp_contexts");
const String SELinuxMMAC::SEAPP_CONTEXTS = USE_OVERRIDE_POLICY ? DATA_SEAPP_CONTEXTS : BASE_SEAPP_CONTEXTS;
const String SELinuxMMAC::SEAPP_HASH_FILE = InitStaticAttr(1, "/system/seapp_hash");

void SELinuxMMAC::FlushInstallPolicy()
{
    sSigSeinfo.Clear();
    sDefaultSeinfo = String(NULL);
}

Boolean SELinuxMMAC::ReadInstallPolicy()
{
    // Temp structures to hold the rules while we parse the xml file.
    // We add all the rules together once we know there's no structural problems.
    HashMap<AutoPtr<ISignature>, AutoPtr<Policy> > sigSeinfo;
    String defaultSeinfo(NULL);

    AutoPtr<IReader> policyFile;
    // try {
    CFileReader::New(MAC_PERMISSIONS, (IReader**)&policyFile);
    Slogger::D(TAG, "Using policy file %s", MAC_PERMISSIONS.string());

    AutoPtr<IXmlPullParser> parser;
    Xml::NewPullParser((IXmlPullParser**)&parser);
    ECode ec = NOERROR;
    String nullStr;
    while (TRUE) {
        ec = parser->SetInput(policyFile);
        if (FAILED(ec)) {
            Slogger::D(TAG, " >> SetInput");
            break;
        }

        ec = XmlUtils::BeginDocument(parser, String("policy"));
        if (FAILED(ec)) break;

        ec = XmlUtils::NextElement(parser);
        if (FAILED(ec)) break;
        Int32 type;
        if (parser->GetEventType(&type), type == IXmlPullParser::END_DOCUMENT) {
            break;
        }

        String tagName;
        parser->GetName(&tagName);
        if (tagName.Equals("signer")) {
            String cert;
            ec = parser->GetAttributeValue(nullStr, String("signature"), &cert);
            if (FAILED(ec)) {
                Slogger::E(TAG, "failed to GetAttributeValue signature");
                break;
            }
            if (cert.IsNull()) {
                String desc;
                parser->GetPositionDescription(&desc);
                Slogger::W(TAG, "<signer> without signature at %s", desc.string());
                XmlUtils::SkipCurrentTag(parser);
                continue;
            }

            AutoPtr<ISignature> signature;
            ec = CSignature::New(cert, (ISignature**)&signature);
            if (ec == (ECode)E_ILLEGAL_ARGUMENT_EXCEPTION) {
                String desc;
                parser->GetPositionDescription(&desc);
                Slogger::W(TAG, "<signer> with bad signature at %s, E_ILLEGAL_ARGUMENT_EXCEPTION", desc.string());
                XmlUtils::SkipCurrentTag(parser);
                continue;
            }
            else if (FAILED(ec)) {
                String desc;
                parser->GetPositionDescription(&desc);
                Slogger::W(TAG, "<signer> with bad signature at %s, ec=%08x", desc.string(), ec);
                break;
            }

            AutoPtr<Policy> policy;
            ec = ReadPolicyTags(parser, (Policy**)&policy);
            if (FAILED(ec)) break;
            if (policy->IsValid()) {
                sSigSeinfo[signature] = policy;
            }
        }
        else if (tagName.Equals("default")) {
            // Value is null if default tag is absent or seinfo tag is malformed.
            ec = ReadSeinfoTag(parser, &defaultSeinfo);
            if (FAILED(ec)) break;
            if (DEBUG_POLICY_INSTALL)
                Slogger::I(TAG, "<default> tag assigned seinfo=%s", defaultSeinfo.string());

        }
        else {
            XmlUtils::SkipCurrentTag(parser);
        }
    }

    AutoPtr<IIoUtils> ioUtils;
    CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
    ioUtils->CloseQuietly(ICloseable::Probe(policyFile));

    if (ec == (ECode)E_XML_PULL_PARSER_EXCEPTION) {
        Slogger::W(TAG, "Got exception parsing %s, E_XML_PULL_PARSER_EXCEPTION", MAC_PERMISSIONS.string());
        return FALSE;
    }
    else if (ec == (ECode)E_IO_EXCEPTION) {
        Slogger::W(TAG, "Got exception parsing %s, E_IO_EXCEPTION", MAC_PERMISSIONS.string());
        return FALSE;
    }
    else if (FAILED(ec)) {
        Slogger::W(TAG, "Got exception parsing %s, ec: 0x%08x", MAC_PERMISSIONS.string(), ec);
        return FALSE;
    }

    FlushInstallPolicy();
    sSigSeinfo = sigSeinfo;
    sDefaultSeinfo = defaultSeinfo;

    return TRUE;
}

ECode SELinuxMMAC::ReadPolicyTags(
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ Policy** p)
{
    VALIDATE_NOT_NULL(p)
    *p = NULL;

    Int32 type;
    Int32 outerDepth;
    parser->GetDepth(&outerDepth);
    AutoPtr<Policy> policy = new Policy();
    Int32 depth;
    while ((parser->Next(&type), type != IXmlPullParser::END_DOCUMENT)
           && (type != IXmlPullParser::END_TAG
               || (parser->GetDepth(&depth), depth > outerDepth))) {
        if (type == IXmlPullParser::END_TAG
            || type == IXmlPullParser::TEXT) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        if (tagName.Equals("seinfo")) {
            String seinfo = ParseSeinfo(parser);
            if (!seinfo.IsNull()) {
                policy->PutSeinfo(seinfo);
            }
            XmlUtils::SkipCurrentTag(parser);
        }
        else if (tagName.Equals("package")) {
            String pkg;
            parser->GetAttributeValue(String(NULL), String("name"), &pkg);
            if (!ValidatePackageName(pkg)) {
                String desc;
                parser->GetPositionDescription(&desc);
                Slogger::W(TAG, "<package> without valid name at ", desc.string());
                XmlUtils::SkipCurrentTag(parser);
                continue;
            }

            String seinfo;
            ReadSeinfoTag(parser, &seinfo);
            if (!seinfo.IsNull()) {
                policy->PutPkg(pkg, seinfo);
            }
        }
        else {
            XmlUtils::SkipCurrentTag(parser);
        }
    }
    *p = policy;
    REFCOUNT_ADD(*p)
    return NOERROR;
}

ECode SELinuxMMAC::ReadSeinfoTag(
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ String* tag)
{
    VALIDATE_NOT_NULL(tag)

    Int32 type;
    Int32 outerDepth;
    parser->GetDepth(&outerDepth);
    String seinfo(NULL);
    Int32 depth;
    while ((parser->Next(&type)), type != IXmlPullParser::END_DOCUMENT
           && (type != IXmlPullParser::END_TAG
               || (parser->GetDepth(&depth), depth > outerDepth))) {
        if (type == IXmlPullParser::END_TAG
                || type == IXmlPullParser::TEXT) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        if (tagName.Equals("seinfo")) {
            seinfo = ParseSeinfo(parser);
        }
        XmlUtils::SkipCurrentTag(parser);
    }
    *tag = seinfo;
    return NOERROR;
}

String SELinuxMMAC::ParseSeinfo(
    /* [in] */ IXmlPullParser* parser)
{
    String seinfoValue;
    parser->GetAttributeValue(String(NULL), String("value"), &seinfoValue);
    if (!ValidateValue(seinfoValue)) {
        String desc;
        parser->GetPositionDescription(&desc);
        Slogger::W(TAG, "<seinfo> without valid name at ", desc.string());
        seinfoValue = String(NULL);
    }
    return seinfoValue;
}

Boolean SELinuxMMAC::ValidatePackageName(
    /* [in] */ const String& name)
{
    if (name.IsNull())
        return FALSE;

    Int32 N = name.GetLength();
    Boolean hasSep = FALSE;
    Boolean front = TRUE;
    for (Int32 i = 0; i < N; i++) {
        char c = name.GetChar(i);
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            front = FALSE;
            continue;
        }
        if (!front) {
            if ((c >= '0' && c <= '9') || c == '_') {
                continue;
            }
        }
        if (c == '.') {
            hasSep = TRUE;
            front = TRUE;
            continue;
        }
        return FALSE;
    }
    return hasSep;
}

Boolean SELinuxMMAC::ValidateValue(
    /* [in] */ const String& name)
{
    if (name.IsNull())
        return FALSE;

    Int32 N = name.GetLength();
    if (N == 0)
        return FALSE;

    for (Int32 i = 0; i < N; i++) {
        char c = name.GetChar(i);
        if ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z') && (c != '_')) {
            return FALSE;
        }
    }
    return TRUE;
}

Boolean SELinuxMMAC::AssignSeinfoValue(
    /* [in] */ PackageParser::Package* pkg)
{
    // We just want one of the signatures to match.
    for (Int32 i = 0; i < pkg->mSignatures->GetLength(); ++i) {
        AutoPtr<ISignature> s = (*pkg->mSignatures)[i];
        if (s == NULL)
            continue;

        AutoPtr<Policy> policy;
        HashMap<AutoPtr<ISignature>, AutoPtr<Policy> >::Iterator it = sSigSeinfo.Find(s);
        if (it != sSigSeinfo.End()) {
            policy = it->mSecond;
        }
        if (policy != NULL) {
            String seinfo = policy->CheckPolicy(pkg->mPackageName);
            if (!seinfo.IsNull()) {
                pkg->mApplicationInfo->SetSeinfo(seinfo);
                if (DEBUG_POLICY_INSTALL)
                    Slogger::I(TAG, "package (%s) labeled with seinfo=%s",
                            pkg->mPackageName.string(), seinfo.string());

                return TRUE;
            }
        }
    }

    // If we have a default seinfo value then great, otherwise
    // we set a null object and that is what we started with.
    pkg->mApplicationInfo->SetSeinfo(sDefaultSeinfo);
    if (DEBUG_POLICY_INSTALL)
        Slogger::I(TAG, "package (%s) labeled with seinfo=%s",
                pkg->mPackageName.string(), (sDefaultSeinfo.IsNull() ? "null" : sDefaultSeinfo.string()));

    return (!sDefaultSeinfo.IsNull());
}

Boolean SELinuxMMAC::ShouldRestorecon()
{
    // Any error with the seapp_contexts file should be fatal
    AutoPtr<ArrayOf<Byte> > currentHash;
    // try {
    ECode ec = ReturnHash(SEAPP_CONTEXTS, (ArrayOf<Byte>**)&currentHash);
    if (ec == (ECode)E_IO_EXCEPTION) {
        Slogger::E(TAG, "Error with hashing seapp_contexts.");
        return FALSE;
    }
    // } catch (IOException ioe) {
    //     Slog.e(TAG, "Error with hashing seapp_contexts.", ioe);
    //     return false;
    // }

    // Push past any error with the stored hash file
    AutoPtr<ArrayOf<Byte> > storedHash;
    // try {
    AutoPtr<IIoUtils> ioUtils;
    CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
    ec = ioUtils->ReadFileAsByteArray(SEAPP_HASH_FILE, (ArrayOf<Byte>**)&ioUtils);
    if (ec == (ECode)E_IO_EXCEPTION) {
        Slogger::E(TAG, "Error opening %s. Assuming first boot.", SEAPP_HASH_FILE.string());
    }
    // } catch (IOException ioe) {
    //     Slog.w(TAG, "Error opening " + SEAPP_HASH_FILE + ". Assuming first boot.");
    // }

    AutoPtr<IMessageDigestHelper> helper;
    CMessageDigestHelper::AcquireSingleton((IMessageDigestHelper**)&helper);
    Boolean isEqual;
    helper->IsEqual(storedHash, currentHash, &isEqual);
    return (storedHash == NULL || !isEqual);
}

void SELinuxMMAC::SetRestoreconDone()
{
    // try {
    AutoPtr<ArrayOf<Byte> > currentHash;
    ECode ec = ReturnHash(SEAPP_CONTEXTS, (ArrayOf<Byte>**)&currentHash);
    if (ec == (ECode)E_IO_EXCEPTION) {
        Slogger::E(TAG, "Error with saving hash to %s", SEAPP_HASH_FILE.string());
    }
    AutoPtr<IFile> file;
    CFile::New(SEAPP_HASH_FILE, (IFile**)&file);
    ec = DumpHash(file, currentHash);
    if (ec == (ECode)E_IO_EXCEPTION) {
        Slogger::E(TAG, "Error with saving hash to %s", SEAPP_HASH_FILE.string());
    }
    // } catch (IOException ioe) {
    //     Slog.e(TAG, "Error with saving hash to " + SEAPP_HASH_FILE, ioe);
    // }
}

ECode SELinuxMMAC::DumpHash(
    /* [in] */ IFile* file,
    /* [in] */ ArrayOf<Byte>* content)
{
    AutoPtr<IFileOutputStream> fos;
    AutoPtr<IFile> tmp;
    // try {
    AutoPtr<IFile> parentFile;
    file->GetParentFile((IFile**)&parentFile);
    AutoPtr<IFileHelper> helper;
    CFileHelper::AcquireSingleton((IFileHelper**)&helper);
    helper->CreateTempFile(String("seapp_hash"), String(".journal"), parentFile, (IFile**)&tmp);
    Boolean result;
    tmp->SetReadable(TRUE, &result);
    CFileOutputStream::New(tmp, (IFileOutputStream**)&fos);
    IOutputStream::Probe(fos)->Write(content);
    AutoPtr<IFileDescriptor> fd;
    fos->GetFD((IFileDescriptor**)&fd);
    fd->Sync();
    ECode ec = NOERROR;
    if (tmp->RenameTo(file, &result), !result) {
        String path;
        file->GetCanonicalPath(&path);
        Slogger::E(TAG, "Failure renaming %s", path.string());
        if (tmp != NULL) {
            tmp->Delete();
        }
        ec = E_IO_EXCEPTION;
    }
    // } finally {
    //     if (tmp != null) {
    //         tmp.delete();
    //     }
    //     IoUtils.closeQuietly(fos);
    // }
    AutoPtr<IIoUtils> ioUtils;
    CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
    ioUtils->CloseQuietly(ICloseable::Probe(fos));
    return ec;
}

ECode SELinuxMMAC::ReturnHash(
    /* [in] */ const String& file,
    /* [out] */ ArrayOf<Byte>** bytes)
{
    VALIDATE_NOT_NULL(bytes)
    *bytes = NULL;
    // try {
    AutoPtr<IIoUtils> ioUtils;
    CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
    AutoPtr<ArrayOf<Byte> > contents;
    FAIL_RETURN(ioUtils->ReadFileAsByteArray(file, (ArrayOf<Byte>**)&contents))
    AutoPtr<IMessageDigestHelper> helper;
    CMessageDigestHelper::AcquireSingleton((IMessageDigestHelper**)&helper);
    AutoPtr<IMessageDigest> digest;
    helper->GetInstance(String("SHA-1"), (IMessageDigest**)&digest);
    return digest->Digest(contents, bytes);
    // } catch (NoSuchAlgorithmException nsae) {
    //     throw new RuntimeException(nsae);  // impossible
    // }
}

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos
