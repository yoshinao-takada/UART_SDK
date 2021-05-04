#ifndef BLFILE_H_
#define BLFILE_H_
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef  __cplusplus
extern "C" {
#endif
    /*!
    \brief replace directory delimiter character with '\0'
    */
    void BLfile_split(char* pathstr);

    /*!
    \brief return the pointer to the beginning of the filename base.
    for example, filepath = "/hello-world/123.txt", the return pointer points to
    character '1'.
    filepath = "only filename.pdf", the return pointer points to character 'o'.
    */
    const char* BLfile_findfilename(const char* filepath);

    /*!
    \brief copy the directory part of filepath.
    for example, filepath = "/usr/local/bin/sh", return pointer holds "/usr/local/bin/".
    Caller code must free() to release the returned directory path string.
    \param filepath [in] path string
    \return directory part of filepath.
    */
    char* BLfile_copydirpath(const char* filepath);

    /*!
    \brief create a new path string combining two path strings.
    The left one can be terminated or unterminated with directory delimiter character.
    \param path_left [in] left part of the path string.
    \param path_right [in] right part of the path string.
    \return path_left + path_right.
    for example, path_left = "/usr/bin", path_right = "bash", return = "/usr/bin/bash",
    another example, path_left = "/usr/bin/", path_right = "bash", return = "/usr/bin/bash"
    */
    char* BLfile_append(const char* path_left, const char* path_right);

    
    int BLfile_size(const char* path, uint32_t* cb_filesize);

    int BLfile_open(FILE* *ppf, const char* path, const char* mode);

    int BLfile_create_directories(const char* path);

    int BLfile_remove_directories(const char* path);

    bool BLfile_exists(const char* path);
#ifdef __cplusplus
}
#endif
#endif