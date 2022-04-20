{
  "targets": [
    {
      "target_name": "lcd1602",
      "type": "static_library",

      "sources": [
        "lcd1602/src/lcd1602.cpp",
      ],
      "include_dirs": [
        "gpio/include",
        "i2c/include",
        "pcf8574/include",
        "lcd1602/include",
      ],

      "direct_dependent_settings": {
        "include_dirs": [
        ],
        "libraries": [
        ],
        "link_settings": {
          "library_dirs": [
          ]
        },
      },
    }
  ]
}
