
# Mostly borrowed from:
# http://chris-miceli.blogspot.com.es/2011/01/integrating-doxygen-with-autotools.html
#
if HAVE_DOXYGEN

directory = $(top_srcdir)/docs/man/man3/

dist_man_MANS = $(directory)/man_page_1.3 $(directory)/man_page_2.3

$(directory)/man_page_1.3: doxyfile.stamp

$(directory)/man_page_2.3: doxyfile.stamp

doxyfile.stamp:
	$(DOXYGEN) Doxyfile
	echo Timestamp > doxyfile.stamp

CLEANFILES = doxyfile.stamp

all-local: doxyfile.stamp

clean-local:
	rm -rf man/ html/ latex/

endif
