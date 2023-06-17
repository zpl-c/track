// Receiver library for zpl.track
// For license, see LICENSE file.

const dgram = require('dgram')

module.exports = {
	init: (type='udp6', ipv6_only=false) => {
		module.callbacks = {}
		module.sock = dgram.createSocket({type: type, ipv6Only: ipv6_only})

		module.sock.on('error', err => {
			if (module.cb_error) module.cb_error(err)
		})

		module.sock.on('message', (rmsg, rinfo) => {
			const msg = rmsg.toString()
			if (module.callbacks["log"]) module.callbacks["log"](msg, rinfo)

			try {
				const payload = JSON.parse(msg)

				// determine which callback to execute
				/**/if (Object.keys(payload).length == 3 
						&& payload.hasOwnProperty('userId') 
						&& payload.hasOwnProperty('event') 
						&& payload.hasOwnProperty('properties')) {
					if (module.callbacks["event"]) module.callbacks["event"](payload)
				}
				else if (Object.keys(payload).length == 3 
						&& payload.hasOwnProperty('userId') 
						&& payload.hasOwnProperty('groupId') 
						&& payload.hasOwnProperty('traits')) {
					if (module.callbacks["group"]) module.callbacks["group"](payload)
				}
				else if (Object.keys(payload).length == 2 
						&& payload.hasOwnProperty('userId') 
						&& payload.hasOwnProperty('traits')) {
					if (module.callbacks["ident"]) module.callbacks["ident"](payload)
				} else {
					if (module.callbacks["unknown"]) module.callbacks["unknown"](payload)
				}
			}
			catch (err) {
				if (module.callbacks["error"]) module.callbacks["error"](err, msg)
			}
		})
	},

	stop: () => {
		module.sock.close()
	},

	bind: (port, host) => {
		if (module.sock === null)
			throw "Make sure to call init first!"
		module.sock.bind(port, host)
	},

	on: (kind, cb) => {
		if (module.sock === null)
			throw "Make sure to call init first!"
		module.callbacks[kind] = cb
	}
}
