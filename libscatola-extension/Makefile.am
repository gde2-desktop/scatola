CLEANFILES = $(NULL)

include $(top_srcdir)/Makefile.shared

lib_LTLIBRARIES=libscatola-extension.la

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_builddir) \
	$(LIBSCATOLA_EXTENSION_CFLAGS) \
	$(WARNING_CFLAGS) \
	$(DISABLE_DEPRECATED_CFLAGS) \
	-DDATADIR=\""$(datadir)"\" \
	$(UNIQUE_CFLAGS) \
	$(NULL)

libscatola_extension_la_LDFLAGS=\
	-version-info @SCATOLA_EXTENSION_VERSION_INFO@ \
	-no-undefined \
	$(NULL)

libscatola_extension_la_LIBADD = $(LIBSCATOLA_EXTENSION_LIBS)

libscatola_extension_includedir=$(includedir)/scatola/libscatola-extension

libscatola_extension_include_HEADERS = \
	scatola-column-provider.h \
	scatola-column.h \
	scatola-extension-types.h \
	scatola-file-info.h \
	scatola-info-provider.h \
	scatola-location-widget-provider.h \
	scatola-menu-item.h \
	scatola-menu-provider.h \
	scatola-property-page-provider.h \
	scatola-property-page.h \
	scatola-menu.h \
	$(NULL)

libscatola_extension_la_SOURCES = \
	scatola-column-provider.c \
	scatola-column.c \
	scatola-extension-i18n.h \
	scatola-extension-types.c \
	scatola-extension-private.h \
	scatola-file-info.c \
	scatola-info-provider.c \
	scatola-location-widget-provider.c \
	scatola-menu-item.c \
	scatola-menu-provider.c \
	scatola-property-page-provider.c \
	scatola-property-page.c \
	scatola-menu.c \
	$(NULL)

pkgconfigdir=$(libdir)/pkgconfig
pkgconfig_DATA=libscatola-extension.pc

EXTRA_DIST = \
	libscatola-extension.pc.in \
	$(NULL)

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = --add-include-path=$(srcdir)
INTROSPECTION_COMPILER_ARGS = --includedir=$(srcdir)

if HAVE_INTROSPECTION
introspection_files = \
	$(libscatola_extension_include_HEADERS) \
	$(libscatola_extension_la_SOURCES)
Scatola-2.0.gir: libscatola-extension.la Makefile
Scatola_2_0_gir_INCLUDES = Gtk-${GTK_API_VERSION} Gio-2.0 GLib-2.0
Scatola_2_0_gir_CFLAGS = \
	-I$(top_srcdir) \
	-I$(top_builddir) \
	$(LIBSCATOLA_EXTENSION_CFLAGS)
Scatola_2_0_gir_LIBS = libscatola-extension.la
Scatola_2_0_gir_FILES = $(addprefix $(srcdir)/, $(introspection_files))
INTROSPECTION_GIRS += Scatola-2.0.gir

girdir = $(datadir)/gir-1.0/
gir_DATA = $(INTROSPECTION_GIRS)

typelibsdir = $(libdir)/girepository-1.0/
typelibs_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelibs_DATA)
endif
