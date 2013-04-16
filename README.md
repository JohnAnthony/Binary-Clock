# binclock

A simple ncurses program for displaying the time in binary numbers

## Usage

The program obviously requires basically no user interaction. Quit
with 'q' or a simple sigint or sigterm (Ctrl-C on *NIX systems)

```
Flags:
    -h          Help information. Points here or to the manpage
    -t #        Choose a character theme form those listed below (0-2)
                (0 is default)
    -s #        Choose a colour set from those listed below (0-2)
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

## Interactive Mode

While binclock is running, you can press the following keys to interact with binclock:
```
q        Quit binclock
t        Cycle through the themes
c        Cycle through the colour sets
```

## Charity
If you find this software useful, please consider donating to Meningitis
U.K. or your local meningitis charity.

http://www.meningitisuk.org
