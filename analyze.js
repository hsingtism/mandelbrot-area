// the number of decimal places to display 
// to prevent table from getting too large
const decimalToShow = 4

// paste last status line of log files instence by instance here
// each entry should be a string
const data = [
    // SAMPLE DATA
    '1645581784, 1645502365, 2622413530, 25227764566, 464',
    '1645581811, 1645502360, 2641418777, 25410085922, 453',
    '1645581899, 1645502355, 2647735075, 25470878517, 424',
    '1645581807, 1645502350, 2641471399, 25410033338, 415',
    '1645580554, 1645502530, 2458231662, 23647116023, 411',
    '1645580498, 1645502535, 2439189207, 23464831879, 418',
    '1645580648, 1645502541, 2470721788, 23768843648, 388',
    '1645580526, 1645502546, 2451734684, 23586504138, 410',
    '1645468688, 1645421643, 1573498639, 15136608229, 268',
    '1645468699, 1645421649, 1586062281, 15258262316, 267',
    '1645468787, 1645421655, 1579814254, 15197401484, 262',
    '1645468819, 1645421663, 1592343396, 15319090087, 245',
    '1645468694, 1645422499, 1484945455, 14285637336, 249',
    '1645468758, 1645422509, 1478681167, 14224792770, 239',
    '1645468734, 1645422519, 1485039312, 14285543479, 249',
    '1645468668, 1645422531, 1478707459, 14224766480, 237',
    '1646287723, 1646024617, 8037933719, 77324539962, 1327',
    '1646287852, 1646024623, 8031359621, 77264005206, 1317',
    '1646287683, 1646024629, 8037798950, 77324674734, 1324',
    '1646287714, 1646024638, 8050334216, 77446357194, 1326',
    '1646287831, 1646022053, 8676108820, 83464360063, 1389',
    '1646287909, 1646022069, 8663522570, 83342728540, 1434',
    '1646287909, 1646022075, 8631868224, 83038838475, 1525',
    '1646287834, 1646022087, 8657062939, 83282079296, 1445',
]

console.time('took')
const l = a => Math.log10(Math.abs(a))

let dataTable = []
const sum = {
    instanceNum : 0,
    totalComputationTime : 0,
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
    sum.totalComputationTime += compTime 
    sum.totalMember += eline[2]
    sum.totalNonMember += eline[3]
    sum.totalUndecided += eline[4]
}

sum.totalTested = sum.totalMember + sum.totalNonMember + sum.totalUndecided

// are there better ways to do this? Yes
// am i gonna learn them and do it? Not right now
let commonLogTotal = {
    instanceNum: {value : sum.instanceNum, log10 : l(sum.instanceNum)},
    totalComputationTIme: {value : sum.totalComputationTime, log10 : l(sum.totalComputationTime)},
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
    dataTable[i].computationRate = (dataTable[i].totalTested / dataTable[i].computationTime).toFixed(decimalToShow) 
    let estH = 16 * (dataTable[i].mem + dataTable[i].undeci) / dataTable[i].totalTested
    let estL = 16 * dataTable[i].mem / dataTable[i].totalTested
    dataTable[i].diffToAvgHLog10 = l(estH - estimatedArea.high).toFixed(decimalToShow) + ` ${Math.sign(estH - estimatedArea.high)}`
    dataTable[i].diffToAvgLLog10 = l(estL - estimatedArea.low).toFixed(decimalToShow) + ` ${Math.sign(estL - estimatedArea.low)}`
    dataTable[i].diffToBestAcceptedHLog10 = l(estH - bestExistingEst).toFixed(decimalToShow) + ` ${Math.sign(estH - bestExistingEst)}`
    dataTable[i].diffToBestAcceptedLLog10 = l(estL - bestExistingEst).toFixed(decimalToShow) + ` ${Math.sign(estL - bestExistingEst)}`
}

console.table(dataTable)
console.table(commonLogTotal)
console.table(estimatedArea)
console.timeEnd('took')