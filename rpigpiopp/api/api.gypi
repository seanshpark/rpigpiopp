{
  "targets": [
    {
      "target_name": "rpigpiopp",

      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],

      "dependencies": [
        "base",
        "gpio",
        "i2c",
        "tm1637",
        "pcf8574",
        "led4x7seg",
        "lcd1602",
        "oled128x64",
      ],
      "sources": [
        "src/rpigpiopp.cpp",
        "src/Wrapper.cpp",
        "src/WrapGpio.cpp",
        "src/WrapI2C.cpp",
        "src/WrapTM1637.cpp",
        "src/WrapPCF8574.cpp",
        "src/WrapLED4x7Seg.cpp",
        "src/WrapLCD1602.cpp",
        "src/WrapOLED128x64.cpp",
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "../lib/base/include",
        "../lib/gpio/include",
        "../lib/i2c/include",
        "../lib/tm1637/include",
        "../lib/pcf8574/include",
        "../lib/led4x7seg/include",
        "../lib/lcd1602/include",
        "../lib/oled128x64/include",
      ],
      'defines': [ 'NAPI_CPP_EXCEPTIONS' ],
    }
  ]
}
