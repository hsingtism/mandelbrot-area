// the number of decimal places to display 
// to prevent table from getting too large
const decimalToShow = 4

// the tested area, 16 for monte carlo, 5.727 for simple grid
const testedArea = 5.727
// const testedArea = 16

// monte-carlo: paste last status line of log files instence by instance here as array entries
// simple-grid: paste the entire log file here, should be an array entry should be a line, or just let dataP read from file and set dataI to false
// each entry should be a string
const dataI = false

// read from file if manually inputted data
// ONLY FOR SIMPLE-GRID
// ONLY FOR SIMPLE-GRID
let dataP = true
if (dataP) {
    dataP = require("fs").readFileSync("log.txt", 'utf8').split((process.platform === "win32") ? '\r\n' : '\n')
    dataP.pop()
}
const data = dataI || dataP

console.time('took')
const l = a => Math.log10(Math.abs(a))

let dataTable = []
const sum = {
    instanceNum: 0,
    totalComputationTime: 0,
    totalTested: 0,
    totalMember: 0,
    totalNonMember: 0,
    totalUndecided: 0,
}


for (let i = 0; i < data.length; i++) {
    let eline = data[i].split(', ').map(i => parseInt(i))
    let compTime = eline[0] - eline[1]
    dataTable[i] = {
        id: eline[1],
        time: compTime,
        mem: eline[2],
        notmem: eline[3],
        undeci: eline[4]
    }
    sum.instanceNum++
    sum.totalComputationTime += compTime
    sum.totalMember += eline[2]
    sum.totalNonMember += eline[3]
    sum.totalUndecided += eline[4]
}

sum.totalTested = sum.totalMember + sum.totalNonMember + sum.totalUndecided

// are there better ways to do this? Yes
// am i gonna learn them and do it? Not right now
let commonLogTotal = {
    instanceNum: { value: sum.instanceNum, log10: l(sum.instanceNum) },
    totalComputationTIme: { value: sum.totalComputationTime, log10: l(sum.totalComputationTime) },
    totalTested: { value: sum.totalTested, log10: l(sum.totalTested) },
    totalMember: { value: sum.totalMember, log10: l(sum.totalMember) },
    totalNonMember: { value: sum.totalNonMember, log10: l(sum.totalNonMember) },
    totalUndecided: { value: sum.totalUndecided, log10: l(sum.totalUndecided) },
}


let estimatedArea = {
    high: testedArea * (sum.totalMember + sum.totalUndecided) / sum.totalTested,
    low: testedArea * sum.totalMember / sum.totalTested
}

estimatedArea.difference = estimatedArea.high - estimatedArea.low
estimatedArea.diffLog10 = l(estimatedArea.difference)
estimatedArea.a = 'THE ACTUAL AREA IS NOT WITHIN THIS RANGE'

const bestExistingEst = 1.5065918849 // 1.5065918849±0.0000000028 by Thorsten Förstemann (2012)
estimatedArea.diffToBestEstH = bestExistingEst - estimatedArea.high
estimatedArea.diffToBestEstHLog10 = l(estimatedArea.diffToBestEstH)
estimatedArea.diffToBestEstL = bestExistingEst - estimatedArea.low
estimatedArea.diffToBestEstLLog10 = l(estimatedArea.diffToBestEstL)

for (let i = 0; i < dataTable.length; i++) {
    dataTable[i].totalTested = dataTable[i].mem + dataTable[i].notmem + dataTable[i].undeci
    dataTable[i].computationRate = (dataTable[i].totalTested / dataTable[i].time).toFixed(decimalToShow)
    let estH = testedArea * (dataTable[i].mem + dataTable[i].undeci) / dataTable[i].totalTested
    let estL = testedArea * dataTable[i].mem / dataTable[i].totalTested
    dataTable[i].diffToAvgHLog10 = l(estH - estimatedArea.high).toFixed(decimalToShow) + ` ${Math.sign(estH - estimatedArea.high)}`
    dataTable[i].diffToAvgLLog10 = l(estL - estimatedArea.low).toFixed(decimalToShow) + ` ${Math.sign(estL - estimatedArea.low)}`
    dataTable[i].dBAHLog10 = l(estH - bestExistingEst).toFixed(decimalToShow) + ` ${Math.sign(estH - bestExistingEst)}`
    dataTable[i].dBALLog10 = l(estL - bestExistingEst).toFixed(decimalToShow) + ` ${Math.sign(estL - bestExistingEst)}`
}

console.table(dataTable)
console.table(commonLogTotal)
