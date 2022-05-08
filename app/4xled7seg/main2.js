//
// Test application: TM1637 + 7Seg LED with clock pin 23, data pin 24
//

// TODO fix load path
const rpigpiopp = require(process.cwd() + '/build/Debug/rpigpiopp');

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

const numbers = '012345678901';
var value = '00:00';

led4x7seg.show(value);

var idx_start = 0;
var idx_start2;
var colon = false;

function timerInterval() {
  value = numbers.substring(idx_start, idx_start + 2);
  if (colon) {
    value = value + ':';
  } else {
    value = value + ' ';
  }
  idx_start2 = (idx_start + 2) % 10;
  value = value + numbers.substring(idx_start2, idx_start2 + 2);
  led4x7seg.show(value);

  idx_start = (idx_start + 1) % 10;
  colon = !colon;
}

setInterval(timerInterval, 500);
