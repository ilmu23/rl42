# Default rl42 keybindings

## Notation

ESCAPE      |   KEY
| :---:     |   :---:
\<C-X>      |   Control + X
\<M-X>      |   Meta (alt) + X
\<S-X>      |   Shift + X
\<F-N>      |   Function N
\<SPC>      |   Space
\<BCK>      |   Backspace
\<UP>       |   Up arrow
\<DOWN>     |   Down arrow
\<LEFT>     |   Left arrow
\<RIGHT>    |   Right arrow
\<ESC>      |   Escape
\<TAB>      |   Tab
\<ENT>      |   Enter
\<RET>      |   Return
\<INS>      |   Insert
\<HME>      |   Home
\<PGU>      |   Page up
\<DEL>      |   Delete
\<END>      |   End
\<PGD>      |   Page down

## emacs-mode

KEY             |   FUNCTION
| :---:         |   :---:
\<SPC> - ~      |   self-insert
\<DEL>          |   remove-char
\<BCK>          |   backward-remove-char
\<C-a>          |   beginning-of-line
\<HME>          |   beginning-of-line
\<C-e>          |   end-of-line
\<END>          |   end-of-line
\<RIGHT>        |   forward-char
\<C-f>          |   forward-char
\<LEFT>         |   backward-char
\<C-b>          |   backward-char
\<S-RIGHT>      |   end-of-word
\<S-LEFT>       |   beginning-of-word
\<M-f>          |   forward-word
\<M-b>          |   backward-word
\<C-l>          |   clear-screen
\<M-,>          |   forward-find-character
\<M-;>          |   backward-find-character
\<RET>          |   accept-line
\<C-o>          |   operate-and-get-next
\<C-d>          |   end-of-file
\<DOWN>         |   next-history
\<C-n>          |   next-history
\<UP>           |   previous-history
\<C-p>          |   previous-history
\<M->>          |   end-of-history
\<M-\<>         |   beginning-of-history
\<M-s>          |   forward-search-history
\<M-r>          |   reverse-search-history
\<C-h>          |   inc-forward-search-history
\<C-r>          |   inc-reverse-search-history
\<M-C-y>        |   yank-nth-arg
\<M-.>          |   yank-last-arg
\<M-u>          |   upcase-word
\<M-l>          |   downcase-word
\<M-c>          |   capitalize-word
\<C-k>          |   forward-kill-line
\<M-k>          |   backward-kill-line
\<M-K>          |   kill-whole-line
\<C-k>w         |   forward-kill-word
\<M-k>w         |   backward-kill-word
\<C-k>r         |   kill-region
\<M-\\>         |   delete-horizontal-space
\<M-C>          |   copy-region-as-kill
\<M-F>          |   copy-forward-word
\<M-B>          |   copy-backward-word
\<C-y>          |   yank
\<M-y>          |   yank-pop
\<C-t>          |   transpose-characters
\<M-t>          |   transpose-words
\<TAB>          |   complete
\<ESC>          |   prefix-meta
\<C-c>          |   discard-line
\<M-0> - \<M-9> |   digit-argument
\<M-->          |   negative-digit-argument
\<C-g>          |   abort
\<M-x>          |   set-mark
\<M-X>          |   unset-mark
\<M-C-x>        |   exchange-point-and-mark
\<M-C-e>        |   vi-editing-mode
\<M-h>          |   set-highlight-color

## vi-insert mode

KEY             |   FUNCTION
| :---:         |   :---:
\<SPC> - ~      |   self-insert
\<C-c>          |   discard-line
\<C-d>          |   end-of-file
\<C-g>          |   abort
\<C-l>          |   clear-screen
\<C-r>          |   inc-reverse-search-history
\<C-s>          |   inc-forward-search-history
\<C-t>          |   transpose-characters
\<C-y>          |   yank-last-arg
\<UP>           |   previous-history
\<DOWN>         |   next-history
\<DEL>          |   remove-char
\<BCK>          |   backward-remove-char
\<ESC>          |   vi-command-mode
\<RET>          |   accept-line
\<TAB>          |   complete

## vi-command mode

KEY             |   FUNCTION
| :---:         |   :---:
\<C-c>          |   discard-line
\<C-d>          |   end-of-file
\<C-e>          |   emacs-editing-mode
\<C-g>          |   abort
\<C-l>          |   clear-screen
\<C-r>          |   inc-reverse-search-history
\<C-s>          |   inc-forward-search-history
\<C-t>          |   transpose-characters
\<SPC>          |   forward-char
$               |   end-of-line
,               |   exchange-point-and-mark
/               |   inc-forward-search-history
0               |   beginning-of-line
1 - 9           |   digit-argument
?               |   inc-reverse-search-history
A               |   vi-insert-mode-A
F               |   backward-find-character
I               |   vi-insert-mode-I
M               |   unset-mark
P               |   yank-pop
S               |   vi-subst-line
X               |   backward-remove-char
Z               |   discard-line
_               |   yank-last-arg
a               |   vi-insert-mode-a
b               |   backward-word
c               |   set-highlight-color
d               |   vi-delete
f               |   forward-find-character
h               |   backward-char
i               |   vi-insert-mode-i
j               |   next-history
k               |   previous-history
l               |   forward-char
m               |   set-mark
p               |   yank
r               |   vi-replace
s               |   vi-subst
w               |   forward-word
x               |   remove-char
\<RET>          |   accept-line
\<TAB>          |   complete
\<ESC>          |   abort

## hlcolor mode

KEY             |   FUNCTION
| :---:         |   :---:
b               |   hlcolor-toggle-bold
u               |   hlcolor-toggle-underline
f               |   hlcolor-toggle-fg/bg
s               |   hlcolor-set-sgr
c               |   hlcolor-set-color
r               |   hlcolor-set-rgb
\<RET>          |   hlcolor-accept
\<BCK>          |   backward-remove-char
0 - 9           |   self-insert
