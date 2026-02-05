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
#include <iostream>
#include <dlfcn.h>

#if defined(__linux__) || defined(__APPLE__)
#include <iostream>
#include <dlfcn.h>  // For dlopen, dlsym, dlclose on Linux/macOS
#endif

static GDExtensionInterfacePrintError godot_print_error = NULL;

typedef GDExtensionBool (*Py4GodotInitFunc)(GDExtensionInterfaceGetProcAddress p_get_proc_address,
                                            GDExtensionClassLibraryPtr p_library,
                                            GDExtensionInitialization *r_initialization);

extern "C" {
    // Entry point for initializing Python script extension
    GDExtensionBool GDN_EXPORT initialize_pythonscript(GDExtensionInterfaceGetProcAddress p_get_proc_address,
                                                       GDExtensionClassLibraryPtr p_library,
                                                       GDExtensionInitialization *r_initialization) {

        godot_print_error = (GDExtensionInterfacePrintError)
        p_get_proc_address("print_error");
        godot_print_error("hello from from pythonscript", "test", "test", 1, 1);
        void *handle = dlopen("libmain.so", RTLD_NOW | RTLD_LOCAL);
        if (!handle) {
            const char* err = dlerror();
            godot_print_error(err, "test", "test", 1, 1);
            return false;
        }
        auto init_func = (Py4GodotInitFunc)dlsym(handle, "hello");
        const char *err = dlerror();
        if (err) {
            godot_print_error(err, "test", "test", 1, 1);
            dlclose(handle);
            return false;
        }

        dlerror(); // clear existing errors
        return init_func(p_get_proc_address, p_library, r_initialization);
    }
}