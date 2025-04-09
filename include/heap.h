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
#ifndef HEAP_H
#define HEAP_H

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

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
typedef int (*heap_compare_t)(const void *a, const void *b);

typedef struct heap heap_t;

heap_t *heap_new(heap_compare_t cmp);

void heap_destroy(heap_t *heap);

int heap_push(heap_t *heap, const void *data);

void *heap_poll(heap_t *heap);

void *heap_pop(heap_t *heap);

heap_t *heap_merge(heap_t *h1, heap_t *h2);

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif/*HEAP_H*/
