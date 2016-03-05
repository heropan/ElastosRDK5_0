
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>

#include "elastos/droid/defaultcontainerservice/MeasurementUtils.h"

namespace Elastos {
namespace Droid {
namespace DefaultContainerService {

Int64 stat_size(struct stat *s)
{
    Int64 blksize = s->st_blksize;
    // count actual blocks used instead of nominal file size
    Int64 size = s->st_blocks * 512;

    if (blksize) {
        /* round up to filesystem block size */
        size = (size + blksize - 1) & (~(blksize - 1));
    }

    return size;
}

Int64 calculate_dir_size(Int32 dfd)
{
    Int64 size = 0;
    struct stat s;
    DIR *d;
    struct dirent *de;

    d = fdopendir(dfd);
    if (d == NULL) {
        close(dfd);
        return 0;
    }

    while ((de = readdir(d))) {
        const char *name = de->d_name;
        if (fstatat(dfd, name, &s, AT_SYMLINK_NOFOLLOW) == 0) {
            size += stat_size(&s);
        }
        if (de->d_type == DT_DIR) {
            int subfd;

            /* always skip "." and ".." */
            if (name[0] == '.') {
                if (name[1] == 0)
                    continue;
                if ((name[1] == '.') && (name[2] == 0))
                    continue;
            }

            subfd = openat(dfd, name, O_RDONLY | O_DIRECTORY);
            if (subfd >= 0) {
                size += calculate_dir_size(subfd);
            }
        }
    }
    closedir(d);
    return size;
}

Int64 MeasurementUtils::MeasureDirectory(
     /* [in] */ const String& path)
{
    Int64 ret = 0L;

    if (path.IsNull()) {
        return ret;
    }

    Int32 dirfd = open(path.string(), O_DIRECTORY, O_RDONLY);
    if (dirfd < 0) {
        //ALOGI("error opening: %s: %s", path, strerror(errno));
    } else {
        ret = calculate_dir_size(dirfd);
        close(dirfd);
    }

    return ret;
}

} //namespace DefaultContainerService
} //namespace Droid
} //namespace Elastos