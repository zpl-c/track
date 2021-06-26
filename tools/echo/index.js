const express = require('express')
const app = express()
const port = 8200

app.use(express.json())

app.post('/', (req, res) => {
    console.log(req.body)
    res.json({
      error: "OK",
      echo: req.body
    })
})

app.listen(port, () => {
  console.log(`Echo tool listening at http://localhost:${port}`)
})
