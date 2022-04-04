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
      ],
      "sources": [
        "src/rpigpiopp.cpp",
        "src/Wrapper.cpp",
        "src/WrapGpio.cpp",
        "src/WrapTM1637.cpp",
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "../lib/base/include",
        "../lib/gpio/include",
        "../lib/tm1637/include",
      ],
      'defines': [ 'NAPI_CPP_EXCEPTIONS' ],
    }
  ]
}
