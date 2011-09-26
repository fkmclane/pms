/* vi:set ts=8 sts=8 sw=8:
 *
 * Practical Music Search
 * Copyright (c) 2006-2011  Kim Tore Jensen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "command.h"
#include <string>
#include <vector>

using namespace std;

Commandlist::Commandlist()
{
	add(CONTEXT_ALL, ACT_SET, "se");
	add(CONTEXT_ALL, ACT_SET, "set");

	add(CONTEXT_ALL, ACT_QUIT, "quit");
	add(CONTEXT_ALL, ACT_QUIT, "q");
	add(CONTEXT_ALL, ACT_NEXT_WINDOW, "next-window");
	add(CONTEXT_ALL, ACT_PREVIOUS_WINDOW, "prev-window");

	add(CONTEXT_LIST, ACT_SCROLL_UP, "scroll-up");
	add(CONTEXT_LIST, ACT_SCROLL_DOWN, "scroll-down");
	add(CONTEXT_LIST, ACT_CURSOR_UP, "cursor-up");
	add(CONTEXT_LIST, ACT_CURSOR_DOWN, "cursor-down");
	add(CONTEXT_LIST, ACT_CURSOR_HOME, "cursor-home");
	add(CONTEXT_LIST, ACT_CURSOR_END, "cursor-end");
	add(CONTEXT_LIST, ACT_CURSOR_TOP, "cursor-top");
	add(CONTEXT_LIST, ACT_CURSOR_BOTTOM, "cursor-bottom");

	add(CONTEXT_ALL, ACT_CONSUME, "consume");
	add(CONTEXT_ALL, ACT_CROSSFADE, "crossfade");
	add(CONTEXT_ALL, ACT_RANDOM, "random");
	add(CONTEXT_ALL, ACT_REPEAT, "repeat");
	add(CONTEXT_ALL, ACT_SETVOL, "volume");
	add(CONTEXT_ALL, ACT_SINGLE, "single");
}

Command * Commandlist::add(int context, action_t action, string name)
{
	Command * c = new Command;
	c->context = context;
	c->action = action;
	c->name = name;
	cmds.push_back(c);
	return c;
}

vector<Command *> * Commandlist::grep(int context, string name)
{
	vector<Command *>::iterator i;

	grepcmds.clear();

	for (i = cmds.begin(); i != cmds.end(); i++)
	{
		if (name.size() > (*i)->name.size() || context != (*i)->context)
			continue;

		if (name == (*i)->name.substr(0, name.size()))
			grepcmds.push_back(*i);
	}

	return &grepcmds;
}

Command * Commandlist::find(int context, string name)
{
	vector<Command *>::iterator i;

	for (i = cmds.begin(); i != cmds.end(); i++)
	{
		if (context == (*i)->context && name == (*i)->name)
			return *i;
	}

	return NULL;
}