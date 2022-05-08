//
// Test application: LCD1602 + HD44780 + PCF8574 + I2C
//

// TODO fix load path
const rpigpiopp = require(process.cwd() + '/build/Debug/rpigpiopp');

const gpiobj = new rpigpiopp.GPIO();
gpiobj.init();

const i2c = new rpigpiopp.I2C();
i2c.init(gpiobj, 1, 0x27);  // 1 is for I2C-1 as (SDA @ pin2, CLK @ pin3)

// command -> lcd1602 -> pcf8574 -> i2c (-> gpio)
// what about HD44780?

const pcf8574 = new rpigpiopp.PCF8574();
pcf8574.init(i2c);

const lcd1602 = new rpigpiopp.LCD1602();
lcd1602.init(pcf8574);

function exitHandler(options, exitCode) {
  if (options.exit) {
    lcd1602.release();
    pcf8574.release();
    i2c.release();
    gpiobj.release();
    process.exit(exitCode);
  }
}

process.stdin.resume();
process.on('SIGINT', exitHandler.bind(null, {exit: true}));   // ctrl+c
process.on('exit', exitHandler.bind(null, {cleanup: true}));  // app closing

var action = 0;

lcd1602.puts('Running...');

function timerInterval() {
  console.log('action=', action);
  switch (action) {
    case 0:
      lcd1602.clear();
      lcd1602.cursor(true);
      lcd1602.blink(true);
      lcd1602.puts('Hello');
      action = action + 1;
      break;

    case 1:
      lcd1602.cursor(true);
      lcd1602.blink(false);
      lcd1602.move(1, 0);
      lcd1602.puts('Raspberry Pi');
      action = action + 1;
      break;

    case 2:
      lcd1602.clear();
      lcd1602.cursor(false);
      lcd1602.blink(false);
      lcd1602.move(0, 0);
      lcd1602.puts('Hello World!!!');
      action = action + 1;
      break;

    case 3:
      lcd1602.move(1, 0);
      lcd1602.cursor(false);
      lcd1602.blink(true);
      lcd1602.puts('^___^');
      action = action + 1;
      break;

    case 4:
      action = 0;
      break;
  }
}

setInterval(timerInterval, 2000);
