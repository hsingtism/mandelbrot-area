analyzeCheckpoint('TIME1654794494;ID1654195255;MEM247622694248;NOTMEM250594560308;UNDECI356;TESTED498217254912;GIM475136;GRE1048576') // sample

function analyzeCheckpoint(str) {
    let a = str.split(';')
    const data = {
        timestamp: a[0].substring(4,a[0].length),
        timestart: a[1].substring(2,a[1].length),
        tested: a[5].substring(6,a[5].length),
        gim: a[6].substring(3,a[6].length),
        gre: a[7].substring(3,a[7].length),
    }
    const res = {}
    res.timeElapsed = data.timestamp - data.timestart,
    res.completion = data.gim / data.gre
    res.estimatedTotalTime = data.gre / data.gim * res.timeElapsed / 86400
    res.rate = data.tested / res.timeElapsed
    res.timeElapsed /= 86400 // careful
    res.estimatedTimeToCompletion = res.estimatedTotalTime - res.timeElapsed 
    console.log('time in days, rate in points per second')
    console.table(res)
    return
}