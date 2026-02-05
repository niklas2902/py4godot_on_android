#if !defined(GDN_EXPORT)
#if defined(_WIN32)
#define GDN_EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#define GDN_EXPORT __attribute__((visibility("default")))
#else
#define GDN_EXPORT
#endif
#endif

#include "gdextension_interface.h"

#if defined(__linux__) || defined(__APPLE__)
#include <iostream>
#include <dlfcn.h>  // For dlopen, dlsym, dlclose on Linux/macOS
#endif
#include "Python.h"
static GDExtensionInterfacePrintError godot_print_error = NULL;

typedef GDExtensionBool (*Py4GodotInitFunc)(GDExtensionInterfaceGetProcAddress p_get_proc_address,
                                            GDExtensionClassLibraryPtr p_library,
                                            GDExtensionInitialization *r_initialization);

extern "C" {
    GDExtensionBool GDN_EXPORT hello(GDExtensionInterfaceGetProcAddress p_get_proc_address,
                                     GDExtensionClassLibraryPtr p_library,
                                     GDExtensionInitialization *r_initialization) {

        godot_print_error = (GDExtensionInterfacePrintError)
            p_get_proc_address("print_error");

        godot_print_error("Setting up Python paths for Android...", "test", "test", 1, 1);

        // Path for your package
        const char* python_home = "/data/data/com.example.newgameproject/files/py4godot/python";

        // Convert to wide string for Python 3
        wchar_t* python_home_wide = Py_DecodeLocale(python_home, NULL);
        if (python_home_wide == NULL) {
            godot_print_error("Failed to decode Python home path", "test", "test", 1, 1);
            return 0;
        }

        // Set Python home before initialization
        Py_SetPythonHome(python_home_wide);

        godot_print_error("Python home set, initializing...", "test", "test", 1, 1);

        // Initialize Python
        Py_Initialize();

        // Check if Python initialized successfully
        if (!Py_IsInitialized()) {
            godot_print_error("Python failed to initialize!", "test", "test", 1, 1);
            PyMem_RawFree(python_home_wide);
            return 0;
        }

        godot_print_error("Python initialized successfully!", "test", "test", 1, 1);

        // Manually add Python paths to sys.path for Android
        if (PyRun_SimpleString(
            "import sys\n"
            "base = '/data/data/com.example.newgameproject/files/py4godot/python'\n"
            "sys.path.insert(0, base + '/lib')\n"
            "sys.path.insert(0, base + '/lib/python3.11')\n"
            "sys.path.insert(0, base + '/lib/python3.11/lib-dynload')\n"
            "sys.path.insert(0, base + '/lib/python3.11/site-packages')\n"
        ) != 0) {
            godot_print_error("Error setting up Python paths", "test", "test", 1, 1);
            if (PyErr_Occurred()) {
                PyErr_Print();
            }
        }

        // Print Python configuration for debugging
        if (PyRun_SimpleString(
            "import sys\n"
            "print('=== Python Configuration ===')\n"
            "print('Python version:', sys.version)\n"
            "print('Executable:', sys.executable)\n"
            "print('Prefix:', sys.prefix)\n"
            "print('Base prefix:', sys.base_prefix)\n"
            "print('\\nPython paths:')\n"
            "for p in sys.path:\n"
            "    print('  -', p)\n"
            "print('===========================')\n"
        ) != 0) {
            godot_print_error("Error printing Python config", "test", "test", 1, 1);
            if (PyErr_Occurred()) {
                PyErr_Print();
            }
        }

        // Simple test
        if (PyRun_SimpleString("print('\\nHello from Python 3.11 on Android!')") != 0) {
            godot_print_error("Error running hello test", "test", "test", 1, 1);
            if (PyErr_Occurred()) {
                PyErr_Print();
            }
        }

        // Test calculation
        if (PyRun_SimpleString("print('2 + 2 =', 2 + 2)") != 0) {
            godot_print_error("Error running calculation", "test", "test", 1, 1);
            if (PyErr_Occurred()) {
                PyErr_Print();
            }
        }

        // Test importing a standard library module
        godot_print_error("Testing standard library import...", "test", "test", 1, 1);
        if (PyRun_SimpleString(
            "try:\n"
            "    import os\n"
            "    print('os module imported successfully')\n"
            "    print('Current directory:', os.getcwd())\n"
            "except Exception as e:\n"
            "    print('Error importing os:', e)\n"
        ) != 0) {
            godot_print_error("Error in import test", "test", "test", 1, 1);
            if (PyErr_Occurred()) {
                PyErr_Print();
            }
        }

        godot_print_error("Python test completed successfully!", "test", "test", 1, 1);

        // Clean up
        PyMem_RawFree(python_home_wide);

        return 1;
    }
}