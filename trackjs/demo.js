const track = require('./index.js')

const host = process.env.HOST      || null
const port = process.env.PORT      || 8200
const prot = process.env.PROT_KIND || 'udp6'

track.init(prot)

track.on('event', ({userId, event, properties}) => {
    console.log(`Event: ${event} from [${userId}]: ${properties}`)
})

track.on('ident', ({userId, traits}) => {
    console.log(`User: ${userId} with traits: ${traits}`)
})

track.on('group', ({userId, groupId, traits}) => {
    console.log(`User: ${userId}, group: ${groupId} with traits: ${traits}`)
})

track.on('error', (err, msg) => {
    console.error(err)
    if (msg) console.error(msg)
})

track.bind(port, host)
