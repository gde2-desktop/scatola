include $(top_srcdir)/Makefile.shared

noinst_LTLIBRARIES=libscatola-private.la

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_builddir) \
	-I$(top_srcdir)/cut-n-paste-code \
	$(CORE_CFLAGS) \
	$(WARNING_CFLAGS) \
	$(DISABLE_DEPRECATED_CFLAGS) \
	$(TRACKER_CFLAGS) \
	$(BEAGLE_CFLAGS) \
	-DDATADIR=\""$(datadir)"\" \
	-DSYSCONFDIR=\""$(sysconfdir)"\" \
	-DSCATOLA_DATADIR=\""$(datadir)/scatola"\" \
	-DSCATOLA_EXTENSIONDIR=\""$(libdir)/scatola/extensions-2.0"\" \
	$(UNIQUE_CFLAGS) \
	$(NULL)

BUILT_SOURCES = \
	scatola-marshal.c \
	scatola-marshal.h \
	$(NULL)

dependency_static_libs = \
	$(top_builddir)/cut-n-paste-code/libegg/libegg.la \
	$(NULL)

libscatola_private_la_LDFLAGS = \
	-no-undefined \
	$(NULL)

libscatola_private_la_LIBADD = \
	$(dependency_static_libs) \
	$(SELINUX_LIBS) \
	$(BEAGLE_LIBS) \
	$(TRACKER_LIBS) \
	$(top_builddir)/eel/libeel-2.la \
	$(top_builddir)/libscatola-extension/libscatola-extension.la \
	$(CORE_LIBS) \
	$(NULL)

libscatola_private_la_SOURCES = \
	scatola-autorun.c \
	scatola-autorun.h \
	scatola-bookmark.c \
	scatola-bookmark.h \
	scatola-cell-renderer-text-ellipsized.c \
	scatola-cell-renderer-text-ellipsized.h \
	scatola-clipboard-monitor.c \
	scatola-clipboard-monitor.h \
	scatola-clipboard.c \
	scatola-clipboard.h \
	scatola-column-chooser.c \
	scatola-column-chooser.h \
	scatola-column-utilities.c \
	scatola-column-utilities.h \
	scatola-customization-data.c \
	scatola-customization-data.h \
	scatola-debug-log.c \
	scatola-debug-log.h \
	scatola-default-file-icon.c \
	scatola-default-file-icon.h \
	scatola-desktop-directory-file.c \
	scatola-desktop-directory-file.h \
	scatola-desktop-directory.c \
	scatola-desktop-directory.h \
	scatola-desktop-icon-file.c \
	scatola-desktop-icon-file.h \
	scatola-desktop-link-monitor.c \
	scatola-desktop-link-monitor.h \
	scatola-desktop-link.c \
	scatola-desktop-link.h \
	scatola-desktop-metadata.c \
	scatola-desktop-metadata.h \
	scatola-directory-async.c \
	scatola-directory-background.c \
	scatola-directory-background.h \
	scatola-directory-notify.h \
	scatola-directory-private.h \
	scatola-directory.c \
	scatola-directory.h \
	scatola-dnd.c \
	scatola-dnd.h \
	scatola-emblem-utils.c \
	scatola-emblem-utils.h \
	scatola-extensions.c \
	scatola-extensions.h \
	scatola-entry.c \
	scatola-entry.h \
	scatola-file-attributes.h \
	scatola-file-changes-queue.c \
	scatola-file-changes-queue.h \
	scatola-file-conflict-dialog.c \
	scatola-file-conflict-dialog.h \
	scatola-file-dnd.c \
	scatola-file-dnd.h \
	scatola-file-operations.c \
	scatola-file-operations.h \
	scatola-file-private.h \
	scatola-file-queue.c \
	scatola-file-queue.h \
	scatola-file-utilities.c \
	scatola-file-utilities.h \
	scatola-file.c \
	scatola-file.h \
	scatola-global-preferences.c \
	scatola-global-preferences.h \
	scatola-icon-canvas-item.c \
	scatola-icon-canvas-item.h \
	scatola-icon-container.c \
	scatola-icon-container.h \
	scatola-icon-dnd.c \
	scatola-icon-dnd.h \
	scatola-icon-private.h \
	scatola-icon-info.c \
	scatola-icon-info.h \
	scatola-icon-names.h \
	scatola-keep-last-vertical-box.c \
	scatola-keep-last-vertical-box.h \
	scatola-lib-self-check-functions.c \
	scatola-lib-self-check-functions.h \
	scatola-link.c \
	scatola-link.h \
	scatola-metadata.h \
	scatola-metadata.c \
	scatola-mime-actions.c \
	scatola-mime-actions.h \
	scatola-mime-application-chooser.c \
	scatola-mime-application-chooser.h \
	scatola-module.c \
	scatola-module.h \
	scatola-monitor.c \
	scatola-monitor.h \
	scatola-open-with-dialog.c \
	scatola-open-with-dialog.h \
	scatola-progress-info.c \
	scatola-progress-info.h \
	scatola-program-choosing.c \
	scatola-program-choosing.h \
	scatola-recent.c \
	scatola-recent.h \
	scatola-saved-search-file.c \
	scatola-saved-search-file.h \
	scatola-search-directory.c \
	scatola-search-directory.h \
	scatola-search-directory-file.c \
	scatola-search-directory-file.h \
	scatola-search-engine.c \
	scatola-search-engine.h \
	scatola-search-engine-simple.c \
	scatola-search-engine-simple.h \
	scatola-search-engine-beagle.c \
	scatola-search-engine-beagle.h \
	scatola-search-engine-tracker.c \
	scatola-search-engine-tracker.h \
	scatola-sidebar-provider.c \
	scatola-sidebar-provider.h \
	scatola-sidebar.c \
	scatola-sidebar.h \
	scatola-signaller.h \
	scatola-signaller.c \
	scatola-query.c \
	scatola-query.h \
	scatola-thumbnails.c \
	scatola-thumbnails.h \
	scatola-trash-monitor.c \
	scatola-trash-monitor.h \
	scatola-tree-view-drag-dest.c \
	scatola-tree-view-drag-dest.h \
	scatola-ui-utilities.c \
	scatola-ui-utilities.h \
	scatola-vfs-directory.c \
	scatola-vfs-directory.h \
	scatola-vfs-file.c \
	scatola-vfs-file.h \
	scatola-view-factory.c \
	scatola-view-factory.h \
	scatola-view.c \
	scatola-view.h \
	scatola-window-info.c \
	scatola-window-info.h \
	scatola-window-slot-info.c \
	scatola-window-slot-info.h \
	scatola-undostack-manager.c \
	scatola-undostack-manager.h \
	$(NULL)

nodist_libscatola_private_la_SOURCES =\
	$(BUILT_SOURCES) \
	$(NULL)

$(lib_LTLIBRARIES): $(dependency_static_libs)

scatola-marshal.list: $(libscatola_private_la_SOURCES) Makefile.am
	$(AM_V_GEN)( cd $(srcdir) && \
	sed -n -e 's/.*scatola_marshal_\([[:upper:][:digit:]]*__[[:upper:][:digit:]_]*\).*/\1/p' \
	$(libscatola_private_la_SOURCES) ) \
	| sed -e 's/__/:/' -e 'y/_/,/' | sort -u > $@.tmp
	@if cmp -s $@.tmp $@; then \
		rm $@.tmp; \
	else \
		mv $@.tmp $@; \
	fi

%-marshal.h: %-marshal.list Makefile
	$(AM_V_GEN)$(GLIB_GENMARSHAL) --header --prefix=$(subst -,_,$*)_marshal $< > $*-marshal.h

%-marshal.c: %-marshal.list Makefile
	$(AM_V_GEN)echo "#include \"scatola-marshal.h\"" > $@ && \
	$(GLIB_GENMARSHAL) --body --prefix=$(subst -,_,$*)_marshal $< >> $*-marshal.c

gsettings_SCHEMAS = org.gde2.scatola.gschema.xml org.gde2.media-handling.gschema.xml

@INTLTOOL_XML_NOMERGE_RULE@

@GSETTINGS_RULES@

EXTRA_DIST = \
	scatola-marshal.list \
	$(gsettings_SCHEMAS) \
	$(BEAGLE_SOURCES) \
	$(TRACKER_SOURCES) \
	$(NULL)

CLEANFILES = \
	$(BUILT_SOURCES) \
	scatola-marshal.list \
	$(NULL)

dist-hook:
	cd $(distdir); rm -f $(CLEANFILES)

