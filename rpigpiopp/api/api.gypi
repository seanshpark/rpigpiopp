{
  "targets": [
    {
      "target_name": "rpigpiopp",

      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],

      "dependencies": [
        "base",
        "gpio",
      ],
      "sources": [
        "src/rpigpiopp.cpp",
        "src/Wrapper.cpp",
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "../lib/base/include",
        "../lib/gpio/include",
      ],
      'defines': [ 'NAPI_CPP_EXCEPTIONS' ],
    }
  ]
}
