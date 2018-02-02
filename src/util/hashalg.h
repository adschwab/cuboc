#ifndef HASHALG_H
#define HASHALG_H

typedef  unsigned long  long ub8;   /* unsigned 8-byte quantities */
typedef  unsigned long  int  ub4;   /* unsigned 4-byte quantities */
typedef  unsigned       char ub1;

namespace hashalg {

ub8 hash(ub1 *k, ub8 length, ub8 level);

ub8 hash(void *arr, int length);

} // namespace

#endif
