#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifdef _MSC_VER
#ifndef BUILDING_DLL
# define DLLIMPORT __declspec (dllexport)
#else /* Not BUILDING_DLL */
# define DLLIMPORT __declspec (dllimport)
#endif /* Not BUILDING_DLL */
#else  /* _MSC_VER */
#define DLLIMPORT 
#endif

#endif