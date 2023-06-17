const track = require('./index.js')

track.init()

track.event((user, event, props) => {
	console.log(`Event: ${event} from [${user}]: ${props}`)
})


track.ident((user, traits) => {
	console.log(`User: ${user} with traits: ${traits}`)
})


track.group((user, group, traits) => {
	console.log(`User: ${user}, group: ${group} with traits: ${traits}`)
})

track.bind(8200)
