# Horth

I was originally gonna be all like "oh it's in a high-level language" but some other guy already decided to make Porth, so I guess it just sort-of inherits my name.
I want to understand this language inside and out, so I'm rolling my own. It's like LISP but actually viable on baremetal :)

Keeping in the spirit of Forth's flexibility and live-fixing, I want any stage of this compiler to be callable through the associated library. The executable should be nothing
but a wrapper that does the whole process, making it effortless to build syntax highlighting, language servers, and other useful tools that improve developer experience.
