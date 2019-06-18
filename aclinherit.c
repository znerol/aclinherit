/**
 * Copyright 2019 Lorenz Schori
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <sys/types.h>
#include <sys/param.h>
#include <sys/acl.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>

static int 
aclinherit(const char *dst_path)
{
    char src_path[MAXPATHLEN];

    // Determine dirname.
    bzero((void*)src_path, MAXPATHLEN);
    if (dirname_r(dst_path, src_path) == NULL) {
        fprintf(stderr, "Failed to determine parent dir of %s: ", dst_path);
        perror(NULL);
        return -1;
    }

    // Read ACL from parent dir.
    acl_t src_acl = acl_get_link_np(src_path, ACL_TYPE_EXTENDED);
    if (src_acl == NULL) {
        fprintf(stderr, "Failed to read ACL on %s: ", src_path);
        perror(NULL);
        return -1;
    }

    // Duplicate ACL.
    acl_t dst_acl = acl_dup(src_acl);
    if (dst_acl == NULL) {
        acl_free((void *)src_acl);

        fprintf(stderr, "Failed to copy ACL from %s: ", src_path);
        perror(NULL);
        return -1;
    }

    // Set inherit flag on duplicated ACEs.
    acl_entry_t entry = NULL;
    while(acl_get_entry(dst_acl, entry == NULL ? ACL_FIRST_ENTRY : ACL_NEXT_ENTRY, &entry) == 0) {
        if (entry) {
            acl_flagset_t mflags;

            acl_get_flagset_np(entry, &mflags);
            acl_add_flag_np(mflags, ACL_ENTRY_INHERITED);
            acl_set_flagset_np(entry, mflags);
        }
    }

    int result = acl_set_link_np(dst_path, ACL_TYPE_EXTENDED, dst_acl);

    acl_free((void *)src_acl);
    acl_free((void *)dst_acl);

    if (result == -1) {
        fprintf(stderr, "Failed to write ACL to %s: ", dst_path);
        perror(NULL);
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s FILE...\n", argv[0]);
        return 1;
    }

    for (int i=1; i<argc; i++) {
        if (aclinherit(argv[i]) == -1) {
            return 2;
        }
    }

    return 0;
}
