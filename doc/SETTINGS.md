# rl42 options

## bell-style

Controls what happens when rl42 would ring the terminal bell. The possible
values are none, visible and audible.

## completion-display-width

Number of columns used to display possible matches when doing completion.
Ignored if less than 0 or greater than the terminal width. A value of 0 will
cause matches to be displayed one per line.

## completion-ignore-case

Do case-insensitive completion.

## completion-map-case

If completion-ignore-case is on, treat - and _ as the same.

## completion-query-items

If a completion produces more matches than the value of this variable the user
will be queried about viewing all the possible completions. A negative value
will result in rl42 never asking.

## editing-mode

Sets the initial editing mode. Can be set to either emacs or vi.

## enable-completion

Enables autocompletion.

## history-size

Maximum number of history entries kept in the history. If set to a value less
than 0, the number of entries is unlimited. If an attempt is made to set the
value to a non-numeric value, the maximum number of entries is set to 500.

## keyseq-timeout

Duration in milliseconds that rl42 will wait for a character when reading an
ambiguous key sequence. If no input is received within the timeout, the shorter
complete key sequence will be used. If this variable is set to less than or equal
to 0 or to a non-numeric value, rl42 will wait until another key is pressed to
decide which key sequence to use.

## mark-directories

Append a slash to completed directory names.

## mark-symlinked-directories

If mark-directories is enabled append a slash to completed symlinks to directories.

## match-hidden-files

Match hidden files in addition to normal files.

## highligh-current-completion

Highlights the currently completed match.

## Default values

OPTION                              |   DEFAULT VALUE
| :---:                             |   :---:
bell-style                          |   none
completion-display-width            |   -1
completion-ignore-case              |   off
completion-map-case                 |   off
completion-query-items              |   100
editing-mode                        |   emacs
enable-completion                   |   on
history-size                        |   500
keyseq-timeout                      |   500
mark-directories                    |   on
mark-symlinked-directories          |   off
match-hidden-files                  |   on
highlight-current-completion        |   on
