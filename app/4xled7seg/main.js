//
// Test application: TM1637 + 7Seg LED with clock pin 23, data pin 24
//

// TODO fix load path
const rpigpiopp = require(process.cwd() + '/build/Debug/rpigpiopp');

const gpiobj = new rpigpiopp.GPIO();
gpiobj.init();

const tm1637 = new rpigpiopp.TM1637();
tm1637.init(gpiobj, 23, 24);

function exitHandler(options, exitCode) {
  if (options.exit) {
    tm1637.release();
    gpiobj.release();
    process.exit(exitCode);
  }
}

process.stdin.resume();
process.on('SIGINT', exitHandler.bind(null, {exit: true}));   // ctrl+c
process.on('exit', exitHandler.bind(null, {cleanup: true}));  // app closing

const TM1637_CMD_ADDR = 0xc0;

const TM1637_ADDR_C0H = 0x00;
const TM1637_DBIT_COLON = 0x80;

const tm1637data = Buffer.alloc(5);

// clear display
tm1637.clear();

// set bright to second level
tm1637.bright(2);

var seg_data = [
  0b00111111,  // 0
  0b00000110,  // 1
  0b01011011,  // 2
  0b01001111,  // 3
  0b01100110,  // 4
  0b01101101,  // 5
  0b01111101,  // 6
  0b00000111,  // 7
  0b01111111,  // 8
  0b01101111   // 9
];

// set display 1234
command = TM1637_CMD_ADDR | TM1637_ADDR_C0H;
tm1637data[0] = command;
for (i = 0; i < 4; ++i) {
  tm1637data[i + 1] = seg_data[i];
}
tm1637.writes(tm1637data);

var idx_start = 0;
var colon = false;
var bright = 2;

function timerInterval() {
  for (i = 0; i < 4; ++i) {
    tm1637data[i + 1] = seg_data[(idx_start + i) % 10];
  }
  if (colon) {
    tm1637data[2] |= TM1637_DBIT_COLON;
  }

  // with command TM1637_CMD_ADDR | TM1637_ADDR_C0H
  tm1637.writes(tm1637data);

  // rotate brightness
  tm1637.bright(bright);

  idx_start = (idx_start + 1) % 10;
  colon = !colon;
  bright = (bright + 1) % 8;
}

setInterval(timerInterval, 500);
