project('py4godot', 'cpp',
    meson_version: '>=1.6.0',)

cc = meson.get_compiler('cpp')
python_lib = cc.find_library('python3.11',
                             dirs: ['/home/niklas/Dokumente/privat/repositories/py4godot_on_android/example/py4godot/python/lib'])


pythonscript_lib = shared_library('pythonscript',
                        'src/pythonscript.cpp',
                        include_directories:'gdextension-api', name_prefix:'lib')

main_lib = shared_library('main',
                          'src/main.cpp',
                          include_directories: ['gdextension-api', 'example/py4godot/python/include/python3.11'],
                          dependencies: python_lib,
                          name_prefix: 'lib')