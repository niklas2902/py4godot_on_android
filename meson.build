project('py4godot', 'cpp',
    meson_version: '>=1.6.0',)

cc = meson.get_compiler('cpp')

pythonscript_lib = shared_library('pythonscript',
                        'src/pythonscript.cpp',
                        include_directories:'gdextension-api', name_prefix:'lib')