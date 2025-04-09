/*
 * Copyright (C) 2025 Da'Jour J. Christophe. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef TRIE_H
#define TRIE_H

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

#include <stdbool.h>
#include <stdint.h>

typedef struct trie trie_t;

trie_t *trie_new(void);

void trie_destroy(trie_t *self);

void trie_insert(trie_t *self, const uint8_t *key);

bool trie_search(trie_t *self, const uint8_t *key);

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif/*TRIE_H*/
