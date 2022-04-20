{
  "targets": [
    {
      "target_name": "pcf8574",
      "type": "static_library",

      "sources": [
        "pcf8574/src/pcf8574.cpp",
      ],
      "include_dirs": [
        "pcf8574/include",
        "i2c/include",
        "gpio/include"
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
