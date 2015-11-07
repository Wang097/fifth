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
#include "textweb.h"

textweb::textweb(int x, int y, int w, int h): Inputplace_Choice(x, y, w, h) {
	input().when(FL_WHEN_ENTER_KEY|FL_WHEN_NOT_CHANGED);
	menubutton().clear_visible_focus();
}

void textweb::draw() {
	Inputplace_Choice::draw();
}

int textweb::handle(const int e) {
	if (Inputplace_Choice::handle(e)) return 1;
	// Handle other events
	if (e == FL_KEYBOARD) {
		switch (Fl::event_key()) {
			case FL_Down:
				window()->cursor(FL_CURSOR_DEFAULT);
				inpmenubutton().popup();
				return 1;
		}
	}
	return 0;
}
