# binclock

A simple ncurses program for displaying the time in binary numbers

## Usage

The program obviously requires basically no user interaction. Quit
with a simple sigint or sigterm (Ctrl-C on *NIX systems)

```
Flags:
    -h          Help information. Points here or to the manpage
    -t #        Set theme number (0-2)
                (0 is default)
```

## Themes

Theme 0:
```
[ ] [ ] [ ] [*] [*] [ ] [*] [*] 
```

Theme 1:
```
<0> <0> <0> <1> <1> <0> <1> <1> 
```

Theme 2:
```
( ) ( ) ( ) (+) (+) ( ) (+) (+) 
```

## Colour Sets

Set 0:
```
H = Red, M = Green, S = Yellow
```

Set 1:
```
H = Blue, M = Magenta, S = Cyan
```

Set 2:
```
H = Red, M = WHite, S = Blue
```

## License
(Copied from LICENSE file)

Copyright (C) 2011, 2012 John Anthony

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
