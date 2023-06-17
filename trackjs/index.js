// Receiver library for zpl.track
// For license, see LICENSE file.

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
				if (module.cb_error) module.cb_error(msg.toString())
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
	},

	error: cb => {
		module.cb_error = cb
	}
}
