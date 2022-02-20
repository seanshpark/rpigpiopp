//
// Test application: hello world
//

// TODO fix load path
const rpigpiopp = require(process.cwd() + '/build/Debug/rpigpiopp');

rpigpiopp.init();

rpigpiopp.hello();

function exitHandler(options, exitCode) {
  if (options.exit) {
    rpigpiopp.release();
    process.exit(exitCode);
  }
}

process.stdin.resume();
process.on('SIGINT', exitHandler.bind(null, {exit:true})); // ctrl+c
process.on('exit', exitHandler.bind(null,{cleanup:true})); // app closing

console.log("press ctrl+c to quit.")
