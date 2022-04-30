{
  "targets": [
    {
      "target_name": "oled128x64",
      "type": "static_library",

      "sources": [
        "oled128x64/src/oled128x64.cpp",
      ],
      "include_dirs": [
        "gpio/include",
        "i2c/include",
        "oled128x64/include",
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
