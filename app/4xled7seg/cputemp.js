//
// Test application: CPU temperature to TM1637 + LED4x7Seg with clock pin 23, data pin 24
//

// TODO fix load path
const rpigpiopp = require(process.cwd() + '/build/Debug/rpigpiopp');
var fs = require('fs');

const gpiobj = new rpigpiopp.GPIO();
gpiobj.init();

const tm1637 = new rpigpiopp.TM1637();
tm1637.init(gpiobj, 23, 24);

const led4x7seg = new rpigpiopp.LED4x7Seg();
led4x7seg.init(tm1637);

function exitHandler(options, exitCode) {
  if (options.exit) {
    led4x7seg.release();
    tm1637.release();
    gpiobj.release();
    process.exit(exitCode);
  }
}

process.stdin.resume();
process.on('SIGINT', exitHandler.bind(null, {exit: true}));   // ctrl+c
process.on('exit', exitHandler.bind(null, {cleanup: true}));  // app closing

led4x7seg.clear();
led4x7seg.bright(2);

const filename = '/sys/class/thermal/thermal_zone0/temp';

function timerInterval() {
  fs.readFile(filename, 'ascii', function(err, data) {
    if (err) return;
    let strcuts = data.split('\n');
    let temperature = parseInt(strcuts[0]) / 1000.0;
    let strTemp = temperature.toPrecision(4);
    let value = strTemp.substring(0, 2) + ':' + strTemp.substring(3, 5);
    console.log('T', value, '°C')
    led4x7seg.show(value);
  });
}

setInterval(timerInterval, 500);
