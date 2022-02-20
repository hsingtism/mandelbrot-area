//prototyping in js
const seed = [-1, 0.0001]
const c = seed

function printComplex(n) {
    console.log(`${n[0]} + ${n[1]}i`)
}

const addComplex = (a, b) => [a[0] + b[0], a[1] + b[1]]
const squareComplex = (a) => [a[0] * a[0] - a[1] * a[1], 2 * a[0] * a[1]]
const absComplex = (a) => Math.sqrt(a[0] * a[0] + a[1] * a[1])

let z = [0, 0]

for (let i = 0; i < 1000; i++) {
    let zf = z
    z = addComplex(squareComplex(z), c)
    printComplex(z)
    if (absComplex(z) > 2) {
        console.log('not a member')
        break
    }
    if (zf[0] == z[0] && zf[1] == z[1]) {
        console.log('member')
        break
    }
}
