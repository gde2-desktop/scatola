NULL=

AM_CPPFLAGS =\
	-I$(top_srcdir) \
	$(CORE_CFLAGS) \
	$(WARNING_CFLAGS) \
	-DVERSION="\"$(VERSION)\"" \
	-DSCATOLA_DATADIR=\""$(datadir)/scatola"\" \
	-DGDE2LOCALEDIR=\""$(prefix)/${DATADIRNAME}/locale"\" \
	$(NULL)

LDADD =\
	$(top_builddir)/libscatola-private/libscatola-private.la \
	$(CORE_LIBS) \
	$(NULL)

noinst_PROGRAMS =\
	test-scatola-wrap-table \
	test-scatola-search-engine \
	test-scatola-directory-async \
	test-scatola-copy \
	test-eel-background \
	test-eel-editable-label \
	test-eel-image-table \
	test-eel-labeled-image \
	test-eel-pixbuf-scale \
	$(NULL)

test_scatola_copy_SOURCES = test-copy.c test.c

test_scatola_wrap_table_SOURCES = test-scatola-wrap-table.c test.c

test_scatola_search_engine_SOURCES = test-scatola-search-engine.c 

test_scatola_directory_async_SOURCES = test-scatola-directory-async.c

test_eel_background_SOURCES = test-eel-background.c
test_eel_image_table_SOURCES = test-eel-image-table.c test.c
test_eel_labeled_image_SOURCES = test-eel-labeled-image.c test.c test.h
test_eel_pixbuf_scale_SOURCES = test-eel-pixbuf-scale.c test.c test.h

EXTRA_DIST = \
	test.h \
	$(NULL)
