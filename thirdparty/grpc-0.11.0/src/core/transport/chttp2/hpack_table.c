/*
 *
 * Copyright 2015, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "src/core/transport/chttp2/hpack_table.h"

#include <assert.h>
#include <string.h>

#include <grpc/support/log.h>
#include "src/core/support/murmur_hash.h"

static struct {
  const char *key;
  const char *value;
} static_table[] = {
    /* 0: */ {NULL, NULL},
    /* 1: */ {":authority", ""},
    /* 2: */ {":method", "GET"},
    /* 3: */ {":method", "POST"},
    /* 4: */ {":path", "/"},
    /* 5: */ {":path", "/index.html"},
    /* 6: */ {":scheme", "http"},
    /* 7: */ {":scheme", "https"},
    /* 8: */ {":status", "200"},
    /* 9: */ {":status", "204"},
    /* 10: */ {":status", "206"},
    /* 11: */ {":status", "304"},
    /* 12: */ {":status", "400"},
    /* 13: */ {":status", "404"},
    /* 14: */ {":status", "500"},
    /* 15: */ {"accept-charset", ""},
    /* 16: */ {"accept-encoding", "gzip, deflate"},
    /* 17: */ {"accept-language", ""},
    /* 18: */ {"accept-ranges", ""},
    /* 19: */ {"accept", ""},
    /* 20: */ {"access-control-allow-origin", ""},
    /* 21: */ {"age", ""},
    /* 22: */ {"allow", ""},
    /* 23: */ {"authorization", ""},
    /* 24: */ {"cache-control", ""},
    /* 25: */ {"content-disposition", ""},
    /* 26: */ {"content-encoding", ""},
    /* 27: */ {"content-language", ""},
    /* 28: */ {"content-length", ""},
    /* 29: */ {"content-location", ""},
    /* 30: */ {"content-range", ""},
    /* 31: */ {"content-type", ""},
    /* 32: */ {"cookie", ""},
    /* 33: */ {"date", ""},
    /* 34: */ {"etag", ""},
    /* 35: */ {"expect", ""},
    /* 36: */ {"expires", ""},
    /* 37: */ {"from", ""},
    /* 38: */ {"host", ""},
    /* 39: */ {"if-match", ""},
    /* 40: */ {"if-modified-since", ""},
    /* 41: */ {"if-none-match", ""},
    /* 42: */ {"if-range", ""},
    /* 43: */ {"if-unmodified-since", ""},
    /* 44: */ {"last-modified", ""},
    /* 45: */ {"link", ""},
    /* 46: */ {"location", ""},
    /* 47: */ {"max-forwards", ""},
    /* 48: */ {"proxy-authenticate", ""},
    /* 49: */ {"proxy-authorization", ""},
    /* 50: */ {"range", ""},
    /* 51: */ {"referer", ""},
    /* 52: */ {"refresh", ""},
    /* 53: */ {"retry-after", ""},
    /* 54: */ {"server", ""},
    /* 55: */ {"set-cookie", ""},
    /* 56: */ {"strict-transport-security", ""},
    /* 57: */ {"transfer-encoding", ""},
    /* 58: */ {"user-agent", ""},
    /* 59: */ {"vary", ""},
    /* 60: */ {"via", ""},
    /* 61: */ {"www-authenticate", ""},
};

void grpc_chttp2_hptbl_init(grpc_chttp2_hptbl *tbl, grpc_mdctx *mdctx) {
  size_t i;

  memset(tbl, 0, sizeof(*tbl));
  tbl->mdctx = mdctx;
  tbl->max_bytes = GRPC_CHTTP2_INITIAL_HPACK_TABLE_SIZE;
  for (i = 1; i <= GRPC_CHTTP2_LAST_STATIC_ENTRY; i++) {
    tbl->static_ents[i - 1] = grpc_mdelem_from_strings(
        mdctx, static_table[i].key, static_table[i].value);
  }
}

void grpc_chttp2_hptbl_destroy(grpc_chttp2_hptbl *tbl) {
  size_t i;
  for (i = 0; i < GRPC_CHTTP2_LAST_STATIC_ENTRY; i++) {
    GRPC_MDELEM_UNREF(tbl->static_ents[i]);
  }
  for (i = 0; i < tbl->num_ents; i++) {
    GRPC_MDELEM_UNREF(
        tbl->ents[(tbl->first_ent + i) % GRPC_CHTTP2_MAX_TABLE_COUNT]);
  }
}

grpc_mdelem *grpc_chttp2_hptbl_lookup(const grpc_chttp2_hptbl *tbl,
                                      gpr_uint32 index) {
  /* Static table comes first, just return an entry from it */
  if (index <= GRPC_CHTTP2_LAST_STATIC_ENTRY) {
    return tbl->static_ents[index - 1];
  }
  /* Otherwise, find the value in the list of valid entries */
  index -= (GRPC_CHTTP2_LAST_STATIC_ENTRY + 1);
  if (index < tbl->num_ents) {
    gpr_uint32 offset = (tbl->num_ents - 1u - index + tbl->first_ent) %
                        GRPC_CHTTP2_MAX_TABLE_COUNT;
    return tbl->ents[offset];
  }
  /* Invalid entry: return error */
  return NULL;
}

/* Evict one element from the table */
static void evict1(grpc_chttp2_hptbl *tbl) {
  grpc_mdelem *first_ent = tbl->ents[tbl->first_ent];
  size_t elem_bytes = GPR_SLICE_LENGTH(first_ent->key->slice) +
                      GPR_SLICE_LENGTH(first_ent->value->slice) +
                      GRPC_CHTTP2_HPACK_ENTRY_OVERHEAD;
  GPR_ASSERT(elem_bytes <= tbl->mem_used);
  tbl->mem_used = (gpr_uint16)(tbl->mem_used - elem_bytes);
  tbl->first_ent =
      (gpr_uint16)((tbl->first_ent + 1) % GRPC_CHTTP2_MAX_TABLE_COUNT);
  tbl->num_ents--;
  GRPC_MDELEM_UNREF(first_ent);
}

void grpc_chttp2_hptbl_add(grpc_chttp2_hptbl *tbl, grpc_mdelem *md) {
  /* determine how many bytes of buffer this entry represents */
  size_t elem_bytes = GPR_SLICE_LENGTH(md->key->slice) +
                      GPR_SLICE_LENGTH(md->value->slice) +
                      GRPC_CHTTP2_HPACK_ENTRY_OVERHEAD;

  /* we can't add elements bigger than the max table size */
  if (elem_bytes > tbl->max_bytes) {
    /* HPACK draft 10 section 4.4 states:
     * If the size of the new entry is less than or equal to the maximum
     * size, that entry is added to the table.  It is not an error to
     * attempt to add an entry that is larger than the maximum size; an
     * attempt to add an entry larger than the entire table causes
     * the table
     * to be emptied of all existing entries, and results in an
     * empty table.
     */
    while (tbl->num_ents) {
      evict1(tbl);
    }
    return;
  }

  /* evict entries to ensure no overflow */
  while (elem_bytes > (size_t)tbl->max_bytes - tbl->mem_used) {
    evict1(tbl);
  }

  /* copy the finalized entry in */
  tbl->ents[tbl->last_ent] = md;

  /* update accounting values */
  tbl->last_ent =
      (gpr_uint16)((tbl->last_ent + 1) % GRPC_CHTTP2_MAX_TABLE_COUNT);
  tbl->num_ents++;
  tbl->mem_used = (gpr_uint16)(tbl->mem_used + elem_bytes);
}

grpc_chttp2_hptbl_find_result grpc_chttp2_hptbl_find(
    const grpc_chttp2_hptbl *tbl, grpc_mdelem *md) {
  grpc_chttp2_hptbl_find_result r = {0, 0};
  gpr_uint16 i;

  /* See if the string is in the static table */
  for (i = 0; i < GRPC_CHTTP2_LAST_STATIC_ENTRY; i++) {
    grpc_mdelem *ent = tbl->static_ents[i];
    if (md->key != ent->key) continue;
    r.index = (gpr_uint16)(i + 1);
    r.has_value = md->value == ent->value;
    if (r.has_value) return r;
  }

  /* Scan the dynamic table */
  for (i = 0; i < tbl->num_ents; i++) {
    gpr_uint16 idx =
        (gpr_uint16)(tbl->num_ents - i + GRPC_CHTTP2_LAST_STATIC_ENTRY);
    grpc_mdelem *ent =
        tbl->ents[(tbl->first_ent + i) % GRPC_CHTTP2_MAX_TABLE_COUNT];
    if (md->key != ent->key) continue;
    r.index = idx;
    r.has_value = md->value == ent->value;
    if (r.has_value) return r;
  }

  return r;
}
