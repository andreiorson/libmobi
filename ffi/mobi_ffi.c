#include "mobi_ffi.h"

// #include <string.h>
// #include <stdbool.h>
// #include <stdlib.h>
// #include <time.h>

// #include "mobi.h"

// #include "miniz.c"

/* return codes */
#define ERROR 1
#define SUCCESS 0

#define EPUB_CONTAINER "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<container version=\"1.0\" xmlns=\"urn:oasis:names:tc:opendocument:xmlns:container\">\n\
  <rootfiles>\n\
    <rootfile full-path=\"OEBPS/content.opf\" media-type=\"application/oebps-package+xml\"/>\n\
  </rootfiles>\n\
</container>"

#define EPUB_MIMETYPE "application/epub+zip"

#ifndef min
# define min(a, b) ((a) < (b) ? (a) : (b))
#endif

const char separator = '/';
bool outdir_opt = false;
char outdir[FILENAME_MAX];

// void split_fullpath(const char *fullpath, char *dirname, char *basename, const size_t buf_len) {
//     if (buf_len == 0) {
//         return;
//     }
//     char *p = strrchr(fullpath, separator);
//     if (p) {
//         p += 1;
//         if (dirname) {
//             size_t dirlen = min(buf_len - 1, (size_t) (p - fullpath));
//             strncpy(dirname, fullpath, dirlen);
//             dirname[dirlen] = '\0';
//         }
//         if (basename) {
//             strncpy(basename, p, buf_len - 1);
//             basename[buf_len - 1] = '\0';
//         }
//     }
//     else {
//         if (dirname) {
//             dirname[0] = '\0';
//         }
//         if (basename) {
//             strncpy(basename, fullpath, buf_len - 1);
//             basename[buf_len - 1] = '\0';
//         }
//     }
//     if (basename) {
//         p = strrchr(basename, '.');
//         if (p) {
//             *p = '\0';
//         }
//     }
// }

// static int create_path(char *newpath, const size_t buf_len, const char *fullpath, const char *suffix) {
//     char dirname[FILENAME_MAX];
//     char basename[FILENAME_MAX];
//     split_fullpath(fullpath, dirname, basename, FILENAME_MAX);
//     int n;
//     if (outdir_opt) {
//         n = snprintf(newpath, buf_len, "%s%s%s", outdir, basename, suffix);
//     } else {
//         n = snprintf(newpath, buf_len, "%s%s%s", dirname, basename, suffix);
//     }
//     if (n < 0) {
//         printf("Creating file name failed\n");
//         return ERROR;
//     }
//     if ((size_t) n >= buf_len) {
//         printf("File name too long\n");
//         return ERROR;
//     }
//     return SUCCESS;
// }

// A very short-lived native function.
//
// For very short-lived functions, it is fine to call them on the main isolate.
// They will block the Dart execution while running the native function, so
// only do this for native functions which are guaranteed to be short-lived.
FFI_PLUGIN_EXPORT int sum(int a, int b) { return a + b; }

// A longer-lived native function, which occupies the thread calling it.
//
// Do not call these kind of native functions in the main isolate. They will
// block Dart execution. This will cause dropped frames in Flutter applications.
// Instead, call these native functions on a separate isolate.
FFI_PLUGIN_EXPORT int sum_long_running(int a, int b) {
  // Simulate work.
#if _WIN32
  Sleep(5000);
#else
  usleep(5000 * 1000);
#endif
  return a + b;
}

// FFI_PLUGIN_EXPORT int create_epub_from_mobi(const MOBIRawml *rawml, const char *fullpath) {
//       if (rawml == NULL) {
//         printf("Rawml structure not initialized\n");
//         return ERROR;
//     }

//     char zipfile[FILENAME_MAX];
//     if (create_path(zipfile, sizeof(zipfile), fullpath, ".epub") == ERROR) {
//         return ERROR;
//     }
//     printf("Saving EPUB to %s\n", zipfile);
    
//     /* create zip (epub) archive */
//     mz_zip_archive zip;
//     memset(&zip, 0, sizeof(mz_zip_archive));
//     mz_bool mz_ret = mz_zip_writer_init_file(&zip, zipfile, 0);
//     if (!mz_ret) {
//         printf("Could not initialize zip archive\n");
//         return ERROR;
//     }
//     /* start adding files to archive */
//     mz_ret = mz_zip_writer_add_mem(&zip, "mimetype", EPUB_MIMETYPE, sizeof(EPUB_MIMETYPE) - 1, MZ_NO_COMPRESSION);
//     if (!mz_ret) {
//         printf("Could not add mimetype\n");
//         mz_zip_writer_end(&zip);
//         return ERROR;
//     }
//     mz_ret = mz_zip_writer_add_mem(&zip, "META-INF/container.xml", EPUB_CONTAINER, sizeof(EPUB_CONTAINER) - 1, (mz_uint)MZ_DEFAULT_COMPRESSION);
//     if (!mz_ret) {
//         printf("Could not add container.xml\n");
//         mz_zip_writer_end(&zip);
//         return ERROR;
//     }
//     char partname[FILENAME_MAX];
//     if (rawml->markup != NULL) {
//         /* Linked list of MOBIPart structures in rawml->markup holds main text files */
//         MOBIPart *curr = rawml->markup;
//         while (curr != NULL) {
//             MOBIFileMeta file_meta = mobi_get_filemeta_by_type(curr->type);
//             snprintf(partname, sizeof(partname), "OEBPS/part%05zu.%s", curr->uid, file_meta.extension);
//             mz_ret = mz_zip_writer_add_mem(&zip, partname, curr->data, curr->size, (mz_uint) MZ_DEFAULT_COMPRESSION);
//             if (!mz_ret) {
//                 printf("Could not add file to archive: %s\n", partname);
//                 mz_zip_writer_end(&zip);
//                 return ERROR;
//             }
//             curr = curr->next;
//         }
//     }
//     if (rawml->flow != NULL) {
//         /* Linked list of MOBIPart structures in rawml->flow holds supplementary text files */
//         MOBIPart *curr = rawml->flow;
//         /* skip raw html file */
//         curr = curr->next;
//         while (curr != NULL) {
//             MOBIFileMeta file_meta = mobi_get_filemeta_by_type(curr->type);
//             snprintf(partname, sizeof(partname), "OEBPS/flow%05zu.%s", curr->uid, file_meta.extension);
//             mz_ret = mz_zip_writer_add_mem(&zip, partname, curr->data, curr->size, (mz_uint) MZ_DEFAULT_COMPRESSION);
//             if (!mz_ret) {
//                 printf("Could not add file to archive: %s\n", partname);
//                 mz_zip_writer_end(&zip);
//                 return ERROR;
//             }
//             curr = curr->next;
//         }
//     }
//     if (rawml->resources != NULL) {
//         /* Linked list of MOBIPart structures in rawml->resources holds binary files, also opf files */
//         MOBIPart *curr = rawml->resources;
//         /* jpg, gif, png, bmp, font, audio, video, also opf, ncx */
//         while (curr != NULL) {
//             MOBIFileMeta file_meta = mobi_get_filemeta_by_type(curr->type);
//             if (curr->size > 0) {
//                 if (file_meta.type == T_OPF) {
//                     snprintf(partname, sizeof(partname), "OEBPS/content.opf");
//                 } else {
//                     snprintf(partname, sizeof(partname), "OEBPS/resource%05zu.%s", curr->uid, file_meta.extension);
//                 }
//                 mz_ret = mz_zip_writer_add_mem(&zip, partname, curr->data, curr->size, (mz_uint) MZ_DEFAULT_COMPRESSION);
//                 if (!mz_ret) {
//                     printf("Could not add file to archive: %s\n", partname);
//                     mz_zip_writer_end(&zip);
//                     return ERROR;
//                 }
//             }
//             curr = curr->next;
//         }
//     }
//     /* Finalize epub archive */
//     mz_ret = mz_zip_writer_finalize_archive(&zip);
//     if (!mz_ret) {
//         printf("Could not finalize zip archive\n");
//         mz_zip_writer_end(&zip);
//         return ERROR;
//     }
//     mz_ret = mz_zip_writer_end(&zip);
//     if (!mz_ret) {
//         printf("Could not finalize zip writer\n");
//         return ERROR;
//     }
//     return SUCCESS;    
// }