/*
Copyright (C) 2014 Lauri Kasanen

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "main.h"
#include "version.h"

#define xstr(s) str(s)
#define str(s) #s

static const char *aboutme() {

	const char str[] = "<html><head><title>about:fifth</title></head><bopdy><center>"
				"<h3>Fifth "
#if GIT
				GITVERSION
#else
				VERSION
#endif
				", built on " __DATE__
				"<p>"
				"Running on WebkitFLTK " xstr(WK_FLTK_MAJOR) "."
				xstr(WK_FLTK_MINOR) "." xstr(WK_FLTK_PATCH)
				"</h3>"
				"</center></body></html>";

	return strdup(str);
}

static const char *aboutconfig() {

	string s = "<html><head><title>about:config</title>"
			"<script type=\"text/javascript\">"
			"function filter(val) {"
				"var elem = document.getElementsByTagName('tr');"
				"for (i = 0; i < elem.length; i++) {"
					"if (val.length < 1) {"
						"elem[i].style.display = '';"
					"} else {"
						"if (elem[i].className.indexOf(val) == -1)"
							"elem[i].style.display = 'none';"
						"else "
							"elem[i].style.display = '';"
					"}"
				"}"
			"}"
			"</script>"
			"<style type=\"text/css\">"
				"table { width: 100%; border-collapse: collapse; }\n"
				"tr { border-top: 1px green solid;"
					"border-bottom: 1px green solid;"
				"}\n"
				"td {"
					"padding: 0.3em;"
				"}\n"
				"div#scroller {"
					"height: 90%;"
					"overflow: auto;"
				"}\n"
			"</style></head><body><center>";

	s.reserve(16384);


	s += "<input type=\"text\" size=\"80\" placeholder=\"Filter...\" "
		"oninput=\"filter(this.value)\">"
		"<hr>";

	s += "<div id=\"scroller\"><table>\n";
	u32 i;
	for (i = 0; i < numDefaults; i++) {
		s += "<tr class=\"";
		s += defaultSettings[i].name;
		s += "\"><td class=\"name\">";
		s += defaultSettings[i].name;
		s += "</td><td class=\"type\">";

		switch (defaultSettings[i].type) {
			case ST_CHAR:
				s += "char";
			break;
			case ST_FLOAT:
				s += "float";
			break;
			case ST_U32:
				s += "unsigned";
			break;
			case ST_COUNT:
				die("Setting corruption\n");
		}

		s += "</td><td class=\"input\">"
			"<input type=\"text\" size=\"40\" value=\"";
		const setting * const set = getSetting(defaultSettings[i].name);

		char tmp[16];
		switch (set->type) {
			case ST_CHAR:
				s += set->val.c;
			break;
			case ST_FLOAT:
				snprintf(tmp, 16, "%f", set->val.f);
				s += tmp;
			break;
			case ST_U32:
				snprintf(tmp, 16, "%u", set->val.u);
				s += tmp;
			break;
			case ST_COUNT:
				die("Setting corruption\n");
		}

		s += "\">"
			"<input type=\"button\" value=\"Save\" name=\"save\" class=\"";
		s += defaultSettings[i].name;
		s += "\">"
			"<input type=\"button\" value=\"Set default\" name=\"reset\" class=\"";
		s += defaultSettings[i].name;
		s += "\"></td></tr>\n";
	}
	s += "</table></div>";

	s += "</center></body></html>";

	return strdup(s.c_str());
}

static const char *abouthistory() {

	string s = "<html><head><title>about:history</title>"
			"<script type=\"text/javascript\">"
			"function filter(val) {"
				"var elem = document.getElementsByTagName('tr');"
				"for (i = 0; i < elem.length; i++) {"
					"if (val.length < 1) {"
						"elem[i].style.display = '';"
					"} else {"
						"if (elem[i].className.indexOf(val) == -1)"
							"elem[i].style.display = 'none';"
						"else "
							"elem[i].style.display = '';"
					"}"
				"}"
			"}"
			"</script>\n"
			"<style type=\"text/css\">\n"
				"table { width: 100%; border-collapse: collapse; }\n"
				"tr { border-top: 1px green solid;"
					"border-bottom: 1px green solid;"
				"}\n"
				"td {"
					"padding: 0.3em;"
				"}\n"
				"td.name {"
					"width: 20em;"
				"}\n"
				"div#scroller {"
					"height: 90%;"
					"overflow: auto;"
				"}\n"
			"</style></head><body><center>";

	s.reserve(16384);


	s += "<input type=\"text\" size=\"80\" placeholder=\"Filter...\" "
		"oninput=\"filter(this.value)\">"
		"<hr>";

	s += "<div id=\"scroller\"><table>\n";
	u32 i;
	const u32 max = g->history->size();
	for (i = 0; i < max; i++) {
		const time_t time = g->history->getTime(i);
		const char * const url = g->history->getURL(i);

		const struct tm * const local = localtime(&time);
		char timestr[80];
		strftime(timestr, 80, "%d %b %Y", local);

		if (!fl_utf8locale()) {
			char tmp[80];
			memcpy(tmp, timestr, 80);
			size_t inleft = strlen(tmp);
			size_t outsize = 80;

			char *inptr = tmp, *outptr = timestr;
			iconv(g->conv, &inptr, &inleft, &outptr, &outsize);
		}

		s += "<tr class=\"";
		s += timestr;
		s += " ";
		s += url;
		s += "\"><td class=\"name\">";

		s += timestr;

		strftime(timestr, 80, " %H:%M", local);

		s += timestr;
		s += "</td>";

		s += "</td><td class=\"url\"><a href=\"";
		s += url;
		s += "\">";
		s += url;
		s += "</a>";

		s += "</td></tr>\n";
	}
	s += "</table></div>";

	s += "</center></body></html>";

	return strdup(s.c_str());
}

const char *aboutpage(const char * const page) {

	#define is(a) if (!strcmp(page, a))
	is ("fifth") {
		return aboutme();
	} else is ("config") {
		return aboutconfig();
	} else is ("history") {
		return abouthistory();
	}
	#undef is

	return NULL;
}