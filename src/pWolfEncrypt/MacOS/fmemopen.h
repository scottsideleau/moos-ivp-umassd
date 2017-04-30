/**
 * fmemopen.h
 *  by Scott R. Sideleau <scott@sideleau.com>
 *      29-APR-2017
 * 
 * Derived from fmemopen_osx:
 *  fmemopen.h
 *  fmemopen.c
 *
 * A BSD port of the fmemopen Linux method using funopen.
 *
 * Created by Maksym Kryva on 12-FEB-2017.
 * https://github.com/mkryva/fmemopen_osx
 *
 * man docs for fmemopen:
 * http://linux.die.net/man/3/fmemopen
 *
 * man docs for funopen:
 * https://developer.apple.com/library/mac/#documentation/Darwin/Reference/ManPages/man3/funopen.3.html
 *
 * This method is ported from ingenuitas' python-tesseract project.
 *
 * You must call fclose on the returned file pointer or memory will be leaked.
 *
 * @param buf The data that will be used to back the FILE* methods. Must be at least
 *            @c size bytes.
 * @param size The size of the @c buf data.
 * @param mode The permitted stream operation modes.
 * @return A pointer that can be used in the fread/fwrite/fseek/fclose family of methods.
 *         If a failure occurred NULL will be returned.
 * @ingroup NimbusMemoryMappping
 */

#ifndef FMEMOPEN_OSX_FMEMOPEN_H
#define FMEMOPEN_OSX_FMEMOPEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct fmem {
  size_t pos;
  size_t size;
  char *buffer;
};
typedef struct fmem fmem_t;

static int readfn(void *handler, char *buf, int size) {
  fmem_t *mem = (fmem_t*)handler;
  size_t available = mem->size - mem->pos;

  if (size > available) {
    size = available;
  }
  memcpy(buf, mem->buffer + mem->pos, sizeof(char) * size);
  mem->pos += size;

  return size;
}

static int writefn(void *handler, const char *buf, int size) {
  fmem_t *mem = (fmem_t*)handler;
  size_t available = mem->size - mem->pos;

  if (size > available) {
    size = available;
  }
  memcpy(mem->buffer + mem->pos, buf, sizeof(char) * size);
  mem->pos += size;

  return size;
}

static fpos_t seekfn(void *handler, fpos_t offset, int whence) {
  size_t pos;
  fmem_t *mem = (fmem_t*)handler;

  switch (whence) {
    case SEEK_SET: {
                     if (offset >= 0) {
                       pos = (size_t)offset;
                     } else {
                       pos = 0;
                     }
                     break;
                   }
    case SEEK_CUR: {
                     if (offset >= 0 || (size_t)(-offset) <= mem->pos) {
                       pos = mem->pos + (size_t)offset;
                     } else {
                       pos = 0;
                     }
                     break;
                   }
    case SEEK_END: pos = mem->size + (size_t)offset; break;
    default: return -1;
  }

  if (pos > mem->size) {
    return -1;
  }

  mem->pos = pos;
  return (fpos_t)pos;
}

static int closefn(void *handler) {
  free(handler);
  return 0;
}

FILE *fmemopen(void *buf, size_t size, const char *mode) {
  // This data is released on fclose.
  fmem_t* mem = (fmem_t *) malloc(sizeof(fmem_t));

  // Zero-out the structure.
  memset(mem, 0, sizeof(fmem_t));

  mem->size = size;
  mem->buffer = (char*)buf;

  // funopen's man page:
  // https://developer.apple.com/library/mac/#documentation/Darwin/Reference/ManPages/man3/funopen.3.html
  return funopen(mem, readfn, writefn, seekfn, closefn);
}

#endif //FMEMOPEN_OSX_FMEMOPEN_H

