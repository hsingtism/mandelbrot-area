// paste last status line of log files instence by instance here
// each entry should be a string
const data = [
    '1646287714, 1646024638, 8050334216, 77446357194, 1326',
]

console.time('total')
const l = a => Math.log10(Math.abs(a))

let dataTable = []
const sum = {
    instanceNum : 0,
    totalComputationTIme : 0,
    totalTested : 0,
    totalMember : 0,
    totalNonMember : 0,
    totalUndecided : 0,
}


for(let i = 0; i < data.length; i++) {
    let eline = data[i].split(', ').map(i => parseInt(i))
    let compTime = eline[0] - eline[1]
    dataTable[i] = {
        id : eline[1], 
        computationTime: compTime, 
        mem : eline[2], 
        notmem : eline[3], 
        undeci : eline[4]
    }
    sum.instanceNum++
    sum.totalComputationTIme += compTime 
    sum.totalMember += eline[2]
    sum.totalNonMember += eline[3]
    sum.totalUndecided += eline[4]
}

sum.totalTested = sum.totalMember + sum.totalNonMember + sum.totalUndecided

// are there better ways to do this? Yes
// am i gonna learn them and do it? Not right now
let commonLogTotal = {
    instanceNum: {value : sum.instanceNum, log10 : l(sum.instanceNum)},
    totalComputationTIme: {value : sum.totalComputationTIme, log10 : l(sum.totalComputationTIme)},
    totalTested: {value : sum.totalTested, log10 : l(sum.totalTested)},
    totalMember: {value : sum.totalMember, log10 : l(sum.totalMember)},
    totalNonMember: {value : sum.totalNonMember, log10 : l(sum.totalNonMember)},
    totalUndecided: {value : sum.totalUndecided, log10 : l(sum.totalUndecided)},
}


let estimatedArea = {
    high : 16 * (sum.totalMember + sum.totalUndecided) / sum.totalTested,
    low : 16 * sum.totalMember / sum.totalTested
}

estimatedArea.difference = estimatedArea.high - estimatedArea.low
estimatedArea.diffLog10 = l(estimatedArea.difference)
estimatedArea.a = 'THE ACTUAL AREA IS NOT WITHIN THIS RANGE'

const bestExistingEst = 1.5065918849 // 1.5065918849±0.0000000028 by Thorsten Förstemann (2012)
estimatedArea.diffToBestEstH = bestExistingEst - estimatedArea.high
estimatedArea.diffToBestEstHLog10 = l(estimatedArea.diffToBestEstH)
estimatedArea.diffToBestEstL = bestExistingEst - estimatedArea.low
estimatedArea.diffToBestEstLLog10 = l(estimatedArea.diffToBestEstL)

for(let i = 0; i < dataTable.length; i++) {
    dataTable[i].totalTested = dataTable[i].mem + dataTable[i].notmem + dataTable[i].undeci 
    dataTable[i].computationRate = dataTable[i].totalTested / dataTable[i].computationTime 
    let estH = 16 * (dataTable[i].mem + dataTable[i].undeci) / dataTable[i].totalTested
    let estL = 16 * dataTable[i].mem / dataTable[i].totalTested
    dataTable[i].diffToAvgHLog10 = l(estH - estimatedArea.high)
    dataTable[i].diffToAvgLLog10 = l(estL - estimatedArea.low)
    dataTable[i].diffToBestAcceptedHLog10 = l(estH - bestExistingEst)
    dataTable[i].diffToBestAcceptedLLog10 = l(estL - bestExistingEst)
}

console.table(dataTable)
console.table(commonLogTotal)
console.table(estimatedArea)
console.timeEnd('total')