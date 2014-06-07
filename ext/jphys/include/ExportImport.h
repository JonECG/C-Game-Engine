#ifndef ENGINE_EXPORT_IMPORT_H
#define ENGINE_EXPORT_IMPORT_H

#ifdef COMPILING_ENGINE
	#define ME_DLL_SHARED __declspec(dllexport)
#else
	#define ME_DLL_SHARED __declspec(dllimport)
#endif

#ifdef COMPILING_DEBUG_TOOLS
	#define ME_DEBUG_DLL_SHARED __declspec(dllexport)
#else
	#define ME_DEBUG_DLL_SHARED __declspec(dllimport)
#endif

#ifdef COMPILING_PHYSICS_GUI
	#define ME_PHYSICS_DLL_SHARED __declspec(dllexport)
#else
	#define ME_PHYSICS_DLL_SHARED __declspec(dllimport)
#endif

#endif