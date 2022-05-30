{
  "targets": [
    {
      "target_name": "spi",
      "type": "static_library",

      "sources": [
        "spi/src/spi.cpp",
      ],
      "include_dirs": [
        "spi/include",
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
