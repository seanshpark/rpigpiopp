{
  "targets": [
    {
      "target_name": "led4x7seg",
      "type": "static_library",

      "sources": [
        "led4x7seg/src/led4x7seg.cpp",
      ],
      "include_dirs": [
        "gpio/include",
        "tm1637/include",
        "led4x7seg/include",
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
