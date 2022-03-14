//
// Test application: blink LED at pin 23, 24
//

// TODO fix load path
const rpigpiopp = require(process.cwd() + '/build/Debug/rpigpiopp');
const DEF = rpigpiopp.DEF;

const gpiobj = new rpigpiopp.Gpio();
gpiobj.init();

function exitHandler(options, exitCode) {
  if (options.exit) {
    gpiobj.set(23, true);  // reset LED as ON
    gpiobj.set(24, true);  // reset LED as OFF

    gpiobj.release();
    process.exit(exitCode);
  }
}

process.stdin.resume();
process.on('SIGINT', exitHandler.bind(null, {exit: true}));   // ctrl+c
process.on('exit', exitHandler.bind(null, {cleanup: true}));  // app closing

console.log('press ctrl+c to quit.')

ret = rpigpiopp.hello();
console.log('check hello = ', ret);

console.log('DEF.OUT = ', DEF.OUT);

gpiobj.pin(23, DEF.OUT);  // set p23 as OUT
gpiobj.pin(24, DEF.OUT);  // set p24 as OUT
p23 = true;               // LED as ON
p24 = false;              // LED as OFF

function timerInterval() {
  gpiobj.set(23, p23);
  gpiobj.set(24, p24);

  p23 = !p23;
  p24 = !p24;
}

setInterval(timerInterval, 500);
