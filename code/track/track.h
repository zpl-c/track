/**
  zpl - event tracker

define TRACK_DISABLE_VALIDATION to disable JSON data validation, good idea if the data is huge or the traffics is too noisy.

TODO:
implement event filtering

GitHub:
  https://github.com/zpl-c/track

Version History:
1.1.0 - Segment module
  1.0.0 - Initial revision

License:
  This Software is dual licensed under the following licenses:

  Unlicense
  This is free and unencumbered software released into the public domain.

  Anyone is free to copy, modify, publish, use, compile, sell, or
  distribute this software, either in source code form or as a compiled
  binary, for any purpose, commercial or non-commercial, and by any
  means.

  In jurisdictions that recognize copyright laws, the author or authors
  of this software dedicate any and all copyright interest in the
  software to the public domain. We make this dedication for the benefit
  of the public at large and to the detriment of our heirs and
  successors. We intend this dedication to be an overt act of
  relinquishment in perpetuity of all present and future rights to this
  software under copyright law.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
  OTHER DEALINGS IN THE SOFTWARE.

  For more information, please refer to <http://unlicense.org/>

  BSD 3-Clause
  Copyright (c) 2016-2021 Dominik Madarász. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
  3. Neither the name of the copyright holder nor the names of its contributors
  may be used to endorse or promote products derived from this software without
  specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
#pragma once

//~ Callbacks
#define TRACK_EVENT_PROC(name) int name(char const *event_id, char const *user_id, char const *data, void *user_data)
typedef TRACK_EVENT_PROC(track_event_proc);

#define TRACK_IDENT_PROC(name) int name(char const *user_id, char const *traits, void *user_data)
typedef TRACK_IDENT_PROC(track_ident_proc);

#define TRACK_GROUP_PROC(name) int name(char const *user_id, char const *group_id, char const *traits, void *user_data)
typedef TRACK_GROUP_PROC(track_group_proc);

#define TRACK_MODULE_UNREGISTER_PROC(name) int name(void *user_data)
typedef TRACK_MODULE_UNREGISTER_PROC(track_module_unregister_proc);

//~ Lifecycle methods
int track_init(int is_async);
int track_destroy(void);

//~ Event tracking
int track_event(char const *event_id, char const *user_id, char const *data);
int track_ident(char const *user_id, char const *traits);
int track_group(char const *user_id, char const *group_id, char const *traits);

//~ Module registration methods
int track_module_register(track_event_proc *event_proc, track_ident_proc *ident_proc, track_group_proc *group_proc, track_module_unregister_proc *unregister_proc, void *user_data);

void *track_module_get_udata(int module_id);

// TODO(zaklaus): not implemented
int track_module_filter(int module_id, char const **allowlist, int allowlist_len, char const **denylist, int denylist_len);
