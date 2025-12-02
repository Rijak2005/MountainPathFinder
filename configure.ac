AC_INIT([mountainpath], [1.0], [rijak.malhotra@st.oth-regensburg.de])
AM_INIT_AUTOMAKE([foreign])
AC_PROG_CC
PKG_CHECK_MODULES([SDL], [sdl2 SDL2_mixer SDL2_image SDL2_ttf],
  [AC_SUBST(SDL_CFLAGS)
   AC_SUBST(SDL_LIBS)],
  [AC_MSG_ERROR([SDL2 (or mixer/image/ttf) not found. Please install libsdl2-dev, libsdl2-mixer-dev, etc.])])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT