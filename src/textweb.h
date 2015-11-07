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

#ifndef TEXTWEB_H
#define TEXTWEB_H

#include <Inputplace_Choice.H>

// A text field with autocomplete, dropdown, and favicon rendering.

class textweb: public Inputplace_Choice {
public:
	textweb(int x, int y, int w, int h);
	void draw() override;
	int handle(int) override;
};

#endif
