{
  "targets": [
    {
      "target_name": "rpigpiopp",

      "dependencies": [
        "base",
        "gpio",
      ],
      "sources": [
        "src/rpigpiopp.cpp",
        "src/Wrapper.cpp",
      ],
      "include_dirs": [
        "../lib/base/include",
        "../lib/gpio/include",
      ],
    }
  ]
}
