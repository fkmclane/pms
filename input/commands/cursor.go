package commands

import (
	"fmt"
	"strconv"

	"github.com/ambientsound/pms/input/lexer"
	"github.com/ambientsound/pms/widgets"
)

// Cursor moves the cursor in a songlist widget. It can take human-readable
// parameters such as 'up' and 'down', and it also accepts relative positions
// if a number is given.
type Cursor struct {
	songlistWidget *widgets.SonglistWidget
	relative       int
	absolute       int
	finished       bool
}

func NewCursor(songlistWidget *widgets.SonglistWidget) *Cursor {
	return &Cursor{songlistWidget: songlistWidget}
}

func (cmd *Cursor) Reset() {
	cmd.relative = 0
	cmd.absolute = 0
	cmd.finished = false
}

func (cmd *Cursor) Execute(t lexer.Token) error {
	s := t.String()

	if cmd.finished && t.Class != lexer.TokenEnd {
		return fmt.Errorf("Unknown input '%s', expected END", s)
	}

	switch t.Class {

	case lexer.TokenIdentifier:
		switch s {
		case "up":
			cmd.relative = -1
		case "down":
			cmd.relative = 1
		case "pgup":
			fallthrough
		case "pageup":
			_, y := cmd.songlistWidget.Size()
			cmd.relative = -y
		case "pgdn":
			fallthrough
		case "pagedn":
			fallthrough
		case "pagedown":
			_, y := cmd.songlistWidget.Size()
			cmd.relative = y
		case "home":
			cmd.absolute = 0
		case "end":
			cmd.absolute = cmd.songlistWidget.Len() - 1
		default:
			i, err := strconv.Atoi(s)
			if err != nil {
				return fmt.Errorf("Cannot move cursor: input '%s' is not recognized, and is not a number", s)
			}
			cmd.relative = i
		}
		cmd.finished = true

	case lexer.TokenEnd:
		switch {
		case !cmd.finished:
			return fmt.Errorf("Unexpected END, expected cursor offset. Try one of: up, down, [number]", s)
		case cmd.relative != 0:
			cmd.songlistWidget.MoveCursor(cmd.relative)
		default:
			cmd.songlistWidget.SetCursor(cmd.absolute)
		}

	default:
		return fmt.Errorf("Unknown input '%s', expected END", s)
	}

	return nil
}