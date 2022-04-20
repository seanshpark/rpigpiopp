{
  "targets": [
    {
      "target_name": "i2c",
      "type": "static_library",

      "sources": [
        "i2c/src/i2c.cpp",
      ],
      "include_dirs": [
        "i2c/include",
        "gpio/include",
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
