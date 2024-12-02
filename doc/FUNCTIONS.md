# rl42 functions

## Functions for moving

### beginning-of-line

Moves the cursor to the beginning of the input line.

### end-of-line

Moves the cursor to the end of the input line.

### beginning-of-word

Moves the cursor to the beginning of the current word.

### end-of-word

Moves the cursor to the end of the current word.

### forward-char

Moves the cursor forward by one character.

### backward-char

Moves the cursor backward by one character.

### forward-word

Moves the cursor to the end of the next word.

### backward-word

Moves the cursor to the beginning of the current or previous word.

### clear-screen

Clear the screen and redisplay the current input line.

### clear-display

Clear the screen and, if possible, the terminals scrollback buffer and redisplay the current input line.

### forward-find-character

Reads a character and moves the cursor to the next occurence of the character, if one is found.

### backward-find-character

Reads a character and moves the cursor to the previous occurence of the character, if one is found.

## Functions for history manipulation

### accept-line

Accept the current input line. If history is set to on, the input line will be added to the history.
If the input line is a modified history line, it is restored to its original state.

### operate-and-get-next

Accept the current input line and fetch the next input line from the history for editing.

### previous-history

Fetch the previous entry from the history.

### next-history

Fetch the next entry from the history.

### beginning-of-history

Move to the first entry in the history.

### end-of-history

Move to the last entry in the history.

### reverse-search-history

Search backward through the history for a string specified by the user.

### forward-search-history

Search forward through the history for a string specified by the user.

### inc-reverse-search-history

Incrementally search backward through the history for a string specified by the user.

### inc-forward-search-history

Incrementally search forward through the history for a string specified by the user.

### yank-nth-arg

Insert the first argument (second word) of the previous input line at the cursor. With an argument n,
insert the nth word from the previous input line. A negative argument inserts the nth word from the
end of the previous input line.

### yank-last-arg

Insert the last argument (last word) of the previous input line at cursor. With a numeric argument, behave exactly
like yank-nth-arg. Successive calls to yank-last-arg move back through the history list, inserting the last
word (or the word specified by the argument to the first call) of each input line in turn. A negative argument
supplied to any of these successive calls switches the direction through the history.

## Functions for text manipulation

### end-of-file

If there are no characters on the input line, returns NULL to signal EOF.

### backward-remove-char

Delete the character behind the cursor.

### remove-char

Delete the character under the cursor.

### self-insert

Insert the character typed.

### upcase-word

Uppercase the current word.

### downcase-word

Lowercase the current word.

### capitalize_word

Capitalize the current word.

### forward-kill-line

Kill the text from the cursor to the end of the input line.

### backward-kill-line

Kill the text from the cursor to the beginning of the input line.

### kill-whole-line

Kill all characters on the current input line.

### forward-kill-word

Kill the text from the cursor to the end of the current word, or to the end of the next word if between words.

### backward-kill-word

Kill the text from the cursor to the start of the current word.

### kill-region

Kill the text in the region.

### delete-horizontal-space

Delete all whitespace around the cursor.

### transpose-characters

Drag the character before the cursor forward over the character under the cursor, moving the cursor forward as well.

### transpose-words

Drag the word before the cursor forward over the word after the cursor, moving the cursor forward as well.

### copy-region-as-kill

Copy the text in the region to the kill ring.

### copy-forward-word

Copy the word before the cursor to the kill ring.

### copy-backward-word

Copy the word after the cursor to the kill ring.

### yank

Yank the top of the kill ring into the buffer at the cursor.

### yank-pop

Rotate the kill ring, and yank the new top. Only works following yank or yank-pop.

### complete

Attempt to perform completion on the text before the cursor. By default, filename completion is attempted.
The default completion function can be replaced with a custom one by the calling program.

## Miscellanious functions

### re-read-init-file

Read the contents of the configuration file, and incorporate any bindings or variable assignments found.

### discard-line

Stop editing the current input line, and start editing a new input line as if ft_readline had been called
again with the same arguments.

### digit-argument

Add this digit to the current argument, or start a new argument.

### negative-digit-argument

Start a negative argument.

### abort

Abort the current editing command and ring the terminals bell depending on the value of the bell-style variable.

### set-mark

Set the mark to the cursor. If a numeric argument is supplied, the mark is set to that position.

### unset-mark

Unset mark.

### exchange-point-and-mark

Swap the cursor with the mark.

### dump-functions

Print all of the functions and their bindings. With a numeric argument, the output is formatted for the config file.

### dump-variables

Print all of the variables and their values. With a numeric argument, the output is formatted for the config file.

### dump-macros

Print all of the macros and their output strings. With a numeric argument, the output is formatted for the config file.

### emacs-editing-mode

Switch to emacs editing mode.

### vi-command-mode

Switch to vi command mode.

### vi-insert-mode-i

Switch to vi insert mode.

### vi-insert-mode-a

Switch to vi insert mode and move the cursor forward by one character.

### vi-insert-mode-I

Switch to vi insert mode and move the cursor to the beginning of the input line.

### vi-insert-mode-A

Switch to vi insert mode and move the cursor to the end of the input line.

### set-highlight-color

Enter the interactive highlight color sequence editor.

### hlcolor-toggle-bold

Toggle bold on/off for the highlight sequence.

### hlcolor-toggle-underline

Toggle underline on/off for the highlight sequence.

### hlcolor-toggle-fg/bg

Toggle between foreground/background for the highlight sequence.

### hlcolor-set-sgr

Set the highlight sequence to the sequence set by ft_rl_sethlcolor_sgr.

### hlcolor-set-color

Set the color for the highlight sequence to a predefined terminal color.

### hlcolor-set-rgb

Set the color for the highlight sequence from rgb values.

### hlcolor-accept

Accept the currently entered value while in a hlcolor function, or exit from the editor.

## Vi command mode functions

### vi-delete

vi style deletion.

### vi-subst

vi style substitution.

#### vi-subst-line

vi style line substitution.

### vi-replace

vi style replacement.
