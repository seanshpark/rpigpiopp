//
// Test application: hello world
//

// TODO fix load path
const rpigpiopp = require(process.cwd() + '/build/Debug/rpigpiopp');
const DEF = rpigpiopp.DEF;

const gpiobj = new rpigpiopp.Gpio();
gpiobj.init();

function exitHandler(options, exitCode) {
  if (options.exit) {
    gpiobj.release();
    process.exit(exitCode);
  }
}

process.stdin.resume();
process.on('SIGINT', exitHandler.bind(null, {exit: true}));   // ctrl+c
process.on('exit', exitHandler.bind(null, {cleanup: true}));  // app closing

console.log('press ctrl+c to quit.')

console.log('DEF.OUT = ', DEF.OUT);
// DEF.OUT = 100;
// console.log('DEF.OUT = ', DEF.OUT);

gpiobj.pin(23, DEF.OUT);
gpiobj.pin(24, DEF.OUT);
p23 = 1;
p24 = 0;

function timerInterval() {
  gpiobj.set(23, p23);
  gpiobj.set(23, p24);

  p23 = 1 - p23;
  p24 = 1 - p24;
}

setInterval(timerInterval, 500);