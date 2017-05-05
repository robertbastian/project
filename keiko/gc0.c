/*
 * gc0.c
 * 
 * This file is part of the Oxford Oberon-2 compiler
 * Copyright (c) 2006 J. M. Spivey
 * All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id: gc.c 1557 2010-01-24 20:59:31Z mike $
 */

#include "obx.h"

void *scratch_alloc(unsigned size, boolean atomic) {
     void *p = NULL;

     if (size % 4096 != 0)
          p = malloc(size);
     else {
          if (posix_memalign(&p, 4096, size) < 0) p = NULL;
     }

     if (p == NULL)
          panic("Out of memory");

     return p;
}

/* gc_init -- initialise everything */
void gc_init(void) {
}

// BEGIN HACK
void* alloc(unsigned size) {
  return malloc(size);
}

void dealloc(void* p, unsigned size) {
  free(p);
}
// END HACK

value* make_env(int size, int ref_map) {
  value* env = (value*) alloc(4*(4+size));
  env[0].i = 1;
  env[1].i = size;
  env[2].i = ref_map;
  return env;
}

void inc_ref_count(value* env) {
  if (env != 0) env[0].i++;
}

void dec_all_ref_counts(value* env) {
  dec_ref_count(env[3].p);
  for (int i = 0; i < env[1].i; i++) {
     if ((1 << i) & env[2].i) {
         value* p = (value *) getenvt(env[4+i].i);
         dec_ref_count(p);
     }
  } 
}

void dec_ref_count(value* env) {
  if (env != 0) {
    if (--env[0].i == 0) {
      dec_all_ref_counts(env);
      dealloc(env, 4*(4+env[1].i));
    }
  }
}
