{
  'includes': [ 'common.gypi' ],
  'targets': [
    {
      'target_name': 'nodeuv-path',
      'product_name': 'nodeuv-path',
      'type': 'static_library',
      'sources': [
        'path.h',
        './src/path.cc'
      ]
    },
    {
      'target_name': 'test',
      'type': 'executable',
      'sources': [
        './src/path.cc',
        './test/test.cc',
      ],
      'dependencies': [
        'nodeuv-path'
      ]
    }
  ]
}
