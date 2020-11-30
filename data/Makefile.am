NULL =

xml_in_files = scatola.xml.in
xml_files = $(xml_in_files:.xml.in=.xml)
@INTLTOOL_XML_RULE@

desktopdir = $(datadir)/applications
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
desktop_in_files = \
	scatola.desktop.in \
	scatola-home.desktop.in \
	scatola-computer.desktop.in \
	scatola-folder-handler.desktop.in \
	scatola-file-management-properties.desktop.in  \
	scatola-browser.desktop.in                     \
	scatola-autorun-software.desktop.in
@INTLTOOL_DESKTOP_RULE@

mimedir = $(datadir)/mime/packages
mime_DATA = $(xml_files)

servicedir = $(datadir)/dbus-1/services
service_DATA = $(service_in_files:.service.in=.service)
service_in_files = 				\
	org.gde2.freedesktop.FileManager1.service.in

org.gde2.freedesktop.FileManager1.service: org.gde2.freedesktop.FileManager1.service.in Makefile
	$(AM_V_GEN) sed -e "s|\@bindir\@|$(bindir)|" $< > $@

scatoladatadir = $(datadir)/scatola

scatoladata_DATA = \
	browser.xml \
	scatola-extras.placeholder  \
	scatola-suggested.placeholder \
	scatola.css \
    scatola-desktop.css \
    scatola-desktop-3.14.css \
	$(NULL)

# app data file
appdatadir = $(datadir)/appdata
appdata_in_files = scatola.appdata.xml.in
appdata_DATA = $(appdata_in_files:.xml.in=.xml)

EXTRA_DIST = $(scatoladata_DATA) \
	scatola.desktop \
	scatola.desktop.in \
	scatola.css \
	freedesktop-dbus-interfaces.xml \
	$(xml_in_files) \
	$(desktop_in_files) \
	$(service_in_files) \
	$(appdata_in_files) \
	$(NULL)

CLEANFILES = $(xml_files) \
	$(desktop_DATA) \
	$(service_DATA) \
	scatola.appdata.xml \
	$(NULL)

SUBDIRS = \
	icons \
	patterns \
	$(NULL)

install-data-hook:
if ENABLE_UPDATE_MIMEDB
	$(UPDATE_MIME_DATABASE) "$(DESTDIR)$(datadir)/mime"
endif

uninstall-hook:
if ENABLE_UPDATE_MIMEDB
	$(UPDATE_MIME_DATABASE) "$(DESTDIR)$(datadir)/mime"
endif
