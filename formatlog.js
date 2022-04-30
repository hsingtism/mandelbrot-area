let a = require("fs").readFileSync("log.txt", 'utf8').split((process.platform === "win32") ? '\r\n' : '\n')
a.pop()
console.log(JSON.stringify(a));