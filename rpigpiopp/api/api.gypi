{
  "targets": [
    {
      "target_name": "rpigpiopp",

      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],

      "dependencies": [
        "base",
        "gpio",
        "tm1637",
        "led4x7seg",
      ],
      "sources": [
        "src/rpigpiopp.cpp",
        "src/Wrapper.cpp",
        "src/WrapGpio.cpp",
        "src/WrapTM1637.cpp",
        "src/WrapLED4x7Seg.cpp",
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "../lib/base/include",
        "../lib/gpio/include",
        "../lib/tm1637/include",
        "../lib/led4x7seg/include",
      ],
      'defines': [ 'NAPI_CPP_EXCEPTIONS' ],
    }
  ]
}
