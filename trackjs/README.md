<div align="center">
    <a href="https://github.com/zpl-c/zpl"><img src="https://user-images.githubusercontent.com/2182108/111983468-d5593e80-8b12-11eb-9c59-8c78ecc0504e.png" alt="zpl" /></a>
</div>

<br />

<div align="center">
    <a href="https://discord.gg/2fZVEym"><img src="https://img.shields.io/discord/354670964400848898?color=7289DA&style=for-the-badge" alt="discord" /></a>
</div>

<br />
<div align="center">
  NodeJS receiver for zpl.track event tracking library.
</div>

<div align="center">
  <sub>
    Brought to you by <a href="https://github.com/zpl-zak">@zpl-zak</a>, <a href="https://github.com/inlife">@inlife</a>
    and <strong>contributors</strong>
  </sub>
</div>

# Introduction
zpl.track is a cross-platform event tracker that can send your event data to a UDP echo server. It aims to be very minimalist and easy to integrate.

You can find the C99 library [here](https://github.com/zpl-c/track).

This library serves as a receiver for zpl.track that you can use to collect telemetry data. You can then pipe the telemetry data to any destinations desired.

## How to use this library?

```js
const track = require('zpl.track')

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
```

## License

See [LICENSE](LICENSE) for the license.
