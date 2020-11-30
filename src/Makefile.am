include $(top_srcdir)/Makefile.shared

SUBDIRS=file-manager

bin_PROGRAMS = \
	scatola \
	scatola-file-management-properties \
	scatola-autorun-software \
	scatola-connect-server \
	$(NULL)

libexec_PROGRAMS = \
	scatola-convert-metadata \
	$(NULL)

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/cut-n-paste-code \
	-I$(top_builddir)/libscatola-private \
	$(CORE_CFLAGS) \
	$(WARNING_CFLAGS) \
	$(EXIF_CFLAGS) \
	$(EXEMPI_CFLAGS) \
	-DDATADIR=\""$(datadir)"\" \
	-DLIBDIR=\""$(libdir)"\" \
	-DLIBEXECDIR=\""$(libexecdir)"\" \
	-DSCATOLA_DATADIR=\""$(datadir)/scatola"\" \
	-DUIDIR=\""$(datadir)/scatola/ui"\" \
	-DSCATOLA_PIXMAPDIR=\""$(datadir)/pixmaps/scatola"\" \
	-DPREFIX=\""$(prefix)"\" \
	-DSYSCONFDIR=\""$(sysconfdir)"\" \
	-DVERSION="\"$(VERSION)\"" \
	$(DISABLE_DEPRECATED_CFLAGS) \
	$(UNIQUE_CFLAGS) \
	$(NULL)

LDADD = \
	$(top_builddir)/src/file-manager/libscatola-file-manager.la \
	$(top_builddir)/libscatola-private/libscatola-private.la \
	$(CORE_LIBS) \
	$(EXIF_LIBS) \
	$(EXEMPI_LIBS) \
	$(POPT_LIBS) \
	$(NULL)

dbus_freedesktop_built_sources =			\
	scatola-freedesktop-generated.c		\
	scatola-freedesktop-generated.h

$(dbus_freedesktop_built_sources) : Makefile.am $(top_srcdir)/data/freedesktop-dbus-interfaces.xml
	gdbus-codegen									\
		--interface-prefix org.freedesktop.					\
		--c-namespace ScatolaFreedesktop					\
		--c-generate-object-manager						\
		--generate-c-code scatola-freedesktop-generated			\
		$(top_srcdir)/data/freedesktop-dbus-interfaces.xml			\
		$(NULL)

@INTLTOOL_DESKTOP_RULE@

desktop_in_files=gde2-network-scheme.desktop.in
desktop_files=$(desktop_in_files:.desktop.in=.desktop)
desktopdir = $(datadir)/gde2/network/

schemedir = $(datadir)/applications
scheme_DATA = gde2-network-scheme.desktop

BUILT_SOURCES = \
	scatola-src-marshal.c \
	scatola-src-marshal.h \
	$(dbus_freedesktop_built_sources) \
	$(NULL)

scatola_SOURCES = \
	scatola-actions.h \
	scatola-application.c \
	scatola-application.h \
	scatola-bookmark-list.c \
	scatola-bookmark-list.h \
	scatola-bookmarks-window.c \
	scatola-bookmarks-window.h \
	scatola-connect-server-dialog.c \
	scatola-connect-server-dialog.h \
	scatola-connect-server-dialog-nonmain.c \
	scatola-connect-server-operation.c	\
	scatola-connect-server-operation.h	\
	scatola-desktop-window.c \
	scatola-desktop-window.h \
	scatola-emblem-sidebar.c \
	scatola-emblem-sidebar.h \
	scatola-file-management-properties.c \
	scatola-file-management-properties.h \
	scatola-freedesktop-dbus.c \
	scatola-freedesktop-dbus.h \
	scatola-history-sidebar.c \
	scatola-history-sidebar.h \
	scatola-image-properties-page.c \
	scatola-image-properties-page.h \
	scatola-information-panel.c \
	scatola-information-panel.h \
	scatola-location-bar.c \
	scatola-location-bar.h \
	scatola-location-dialog.c \
	scatola-location-dialog.h \
	scatola-location-entry.c \
	scatola-location-entry.h \
	scatola-main.c \
	scatola-main.h \
	scatola-navigation-action.c \
	scatola-navigation-action.h \
	scatola-navigation-window-menus.c \
	scatola-navigation-window.c \
	scatola-navigation-window.h \
	scatola-navigation-window-pane.c \
	scatola-navigation-window-pane.h \
	scatola-navigation-window-slot.c \
	scatola-navigation-window-slot.h \
	scatola-notebook.c \
	scatola-notebook.h \
	scatola-notes-viewer.c \
	scatola-notes-viewer.h \
	scatola-pathbar.c \
	scatola-pathbar.h \
	scatola-places-sidebar.c \
	scatola-places-sidebar.h \
	scatola-property-browser.c \
	scatola-property-browser.h \
	scatola-query-editor.c \
	scatola-query-editor.h \
	scatola-search-bar.c \
	scatola-search-bar.h \
	scatola-self-check-functions.c \
	scatola-self-check-functions.h \
	scatola-side-pane.c \
	scatola-side-pane.h \
	scatola-sidebar-title.c \
	scatola-sidebar-title.h \
	scatola-spatial-window.c \
	scatola-spatial-window.h \
	scatola-trash-bar.c \
	scatola-trash-bar.h \
	scatola-view-as-action.c \
	scatola-view-as-action.h \
	scatola-window-bookmarks.c \
	scatola-window-bookmarks.h \
	scatola-window-manage-views.c \
	scatola-window-manage-views.h \
	scatola-window-menus.c \
	scatola-window-pane.c \
	scatola-window-pane.h \
	scatola-window-private.h \
	scatola-window-slot.c \
	scatola-window-slot.h \
	scatola-window-toolbars.c \
	scatola-window.c \
	scatola-window.h \
	scatola-x-content-bar.c \
	scatola-x-content-bar.h \
	scatola-zoom-action.c \
	scatola-zoom-action.h \
	scatola-zoom-control.c \
	scatola-zoom-control.h \
	$(NULL)

nodist_scatola_SOURCES = \
	$(BUILT_SOURCES) \
	$(NULL)

scatola-src-marshal.list: $(scatola_SOURCES) Makefile.am
	$(AM_V_GEN)( cd $(srcdir) && \
	sed -n -e 's/.*scatola_src_marshal_\([[:upper:][:digit:]]*__[[:upper:][:digit:]_]*\).*/\1/p' \
	$(scatola_SOURCES) ) \
	| sed -e 's/__/:/' -e 'y/_/,/' | sort -u > $@.tmp
	@if cmp -s $@.tmp $@; then \
		rm $@.tmp; \
	else \
		mv $@.tmp $@; \
	fi

%-marshal.c: %-marshal.list Makefile
	$(AM_V_GEN)echo "#include \"scatola-src-marshal.h\"" > $@ && \
	$(GLIB_GENMARSHAL) --body --prefix=$(subst -,_,$*)_marshal $< >> $*-marshal.c

%-marshal.h: %-marshal.list Makefile
	$(AM_V_GEN)$(GLIB_GENMARSHAL) --header --prefix=$(subst -,_,$*)_marshal $< > $*-marshal.h

scatola_file_management_properties_SOURCES = \
	scatola-file-management-properties.c \
	scatola-file-management-properties.h \
	scatola-file-management-properties-main.c \
	$(NULL)

scatola_autorun_software_SOURCES = \
	scatola-autorun-software.c \
	$(NULL)

scatola_connect_server_SOURCES = \
	scatola-bookmark-list.c \
	scatola-bookmark-list.h \
	scatola-connect-server-dialog.c \
	scatola-connect-server-dialog.h \
	scatola-connect-server-dialog-main.c \
	scatola-connect-server-operation.c     \
	scatola-connect-server-operation.h     \
	$(NULL)

scatola_convert_metadata_SOURCES = \
	scatola-convert-metadata.c \
	$(NULL)

TESTS=check-scatola

@INTLTOOL_SERVER_RULE@

uidir = $(datadir)/scatola/ui
ui_DATA = \
	scatola-shell-ui.xml \
	scatola-navigation-window-ui.xml \
	scatola-spatial-window-ui.xml \
	scatola-file-management-properties.ui \
	scatola-bookmarks-window.ui \
	$(NULL)

CLEANFILES = \
	scatola-src-marshal.list \
	$(BUILT_SOURCES) \
	$(desktop_files) \
	$(server_DATA) \
	$(NULL)

EXTRA_DIST = \
	scatola-src-marshal.list \
	$(server_in_files) \
	$(ui_DATA) \
	check-scatola \
	$(desktop_in_files) \
	$(NULL)

dist-hook:
	cd $(distdir); rm -f $(CLEANFILES)
