// Receiver library for zpl.track
// This Software is dual licensed under the following licenses:

// Unlicense
// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>

// BSD 3-Clause
// Copyright (c) 2022 <author-name>. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
// 3. Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

const dgram = require('dgram')

module.exports = {
	init: (type='udp6', ipv6_only=false) => {
		module.sock = dgram.createSocket({type: type, ipv6Only: ipv6_only})

		module.sock.on('error', err => {
			throw err
		})

		module.sock.on('message', (msg, rinfo) => {
			if (module.cb_log) module.cb_log(msg, rinfo)

			try {
				const payload = JSON.parse(msg)

				// determine which callback to execute
				if (payload.hasOwnProperty('userId') && payload.hasOwnProperty('event') && payload.hasOwnProperty('properties')) {
					if (module.cb_event) module.cb_event(payload.userId, payload.event, payload.properties, payload)
				}
				else if (payload.hasOwnProperty('userId') && payload.hasOwnProperty('groupId') && payload.hasOwnProperty('traits')) {
					if (module.cb_group) module.cb_group(payload.userId, payload.groupId, payload.traits, payload)
				}
				else if (payload.hasOwnProperty('userId') && payload.hasOwnProperty('traits')) {
					if (module.cb_ident) module.cb_ident(payload.userId, payload.traits, payload)
				} else {
					if (module.cb_unknown) module.cb_unknown(payload)
				}
			}
			catch (e) {
				console.log(e)
				if (module.cb_unknown) module.cb_unknown(msg.toString(), true)
			}
		})
	},

	stop: () => {
		module.sock.close()
	},

	log: cb => {
		module.cb_log = cb
	},

	bind: (port, host) => {
		if (module.sock === null)
			module.init()
		module.sock.bind(port, host)
	},

	event: cb => {
		module.cb_event = cb
	},
	
	ident: cb => {
		module.cb_ident = cb
	},
	
	group: cb => {
		module.cb_group = cb
	},

	unknown: cb => {
		module.cb_unknown = cb
	}
}
