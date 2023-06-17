const track = require('./index.js')

const host = process.env.HOST || null;
const port = process.env.PORT || 8200;
const prot = process.env.PROT_KIND || 'udp6';

track.init(prot)

track.event((user, event, props) => {
	console.log(`Event: ${event} from [${user}]: ${props}`)
})


track.ident((user, traits) => {
	console.log(`User: ${user} with traits: ${traits}`)
})


track.group((user, group, traits) => {
	console.log(`User: ${user}, group: ${group} with traits: ${traits}`)
})

track.error((err, msg) => {
	console.error(err)
	if (msg) console.error(msg)
})

track.bind(port, host)
