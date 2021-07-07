#ifndef IGRAPH_CONFIG_H
#define IGRAPH_CONFIG_H

/* #undef HAVE_EXPM1 */
/* #undef HAVE_FMIN */
#define HAVE_FINITE 1
#define HAVE_ISFINITE 1
/* #undef HAVE_LOG2 */
/* #undef HAVE_LOG1P */
/* #undef HAVE_RINT */
/* #undef HAVE_RINTF */
/* #undef HAVE_ROUND */
#define HAVE_STPCPY 1
#define HAVE_STRCASECMP 1
/* #undef HAVE__STRICMP */
#define HAVE_STRDUP 1

#define HAVE_GLPK 1
#define HAVE_LIBXML 1

/* #undef INTERNAL_BLAS */
/* #undef INTERNAL_LAPACK */
#define INTERNAL_ARPACK 1
/* #undef INTERNAL_GMP */

#define IGRAPH_F77_SAVE static 
#define IGRAPH_THREAD_LOCAL 

#define PACKAGE_VERSION "0.9.4"
#define PACKAGE_VERSION_MAJOR 0
#define PACKAGE_VERSION_MINOR 9
#define PACKAGE_VERSION_PATCH 4
#define PACKAGE_VERSION_PRERELEASE "cmake-experimental"

#endif
