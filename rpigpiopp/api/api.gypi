{
  "targets": [
    {
      "target_name": "rpigpiopp",

      "dependencies": [
        "base",
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
