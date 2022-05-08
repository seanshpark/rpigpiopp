{
  "targets": [
    {
      "target_name": "gpio",
      "type": "static_library",

      "sources": [
        "gpio/src/gpio.cpp",
      ],
      "include_dirs": [
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
