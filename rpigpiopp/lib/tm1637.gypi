{
  "targets": [
    {
      "target_name": "tm1637",
      "type": "static_library",

      "sources": [
        "tm1637/src/tm1637.cpp",
      ],
      "include_dirs": [
        "tm1637/include",
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
