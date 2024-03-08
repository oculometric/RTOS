# novos

hiyya! this is my kernel development project. i use 'kernel' loosely since it mostly amounts to me building all the *bits* of a kernel without really following a specific kernel design or model.

## state of the project

as of writing this, i've written a reasonably okayish (if not very portable) *bootloader* for loading an elf binary, i've implemented a simple memory allocator, designed a *UI layou*t mechanism and a drawing system, written some *3D graphics* library stuff, and implemented *serial logging* for debugging. some other cool things i've done are re-implementing *streams* to make serial-debugging things easier (and all the other useful things you can do with streams), and implementing *dynamic arrays and strings*. in fact, a fair bit of stuff is re-creating C++ standard library stuff.

## goal of the project

i don't have much of a concrete goal with this. to be honest, given how challenging this kind of development is (for me at least) i don't want to set myself huge goals, and you shouldn't expect this to turn into a full graphical operating system capable of loading Steam games for at least 70 years or so.

mostly the point of this is just learning doing things from the ground up, having fun implementing fundamental data structures and algorithms for myself, and having fun while suffering through that.

me being a graphics programming victim/interested-person, *the only long-term goal i have for this is creating some kind of simple 3D space-related videogame, in the sense that **the kernel is the game**. i.e. i'm writing a game engine, and a game to use it, but completely freestanding and bootable on its own*.

the other thing i do want to do is try and make *some* note about anything particular i learn during this. this is unlikely to happen substantially. something i am **absolutely** going to do is write *well-documented code* because *the number one reason building something like this is so difficult is that i can find very very few well-documented, understandable, followable code examples for this kind of project*. the [osdev wiki](https://wiki.osdev.org) is an amazing resource but, understandably, its pretty opaque if you don't already know everything on it intimately. hopefully someone else will find this useful (assuming another sufficiently stupid noob comes along like me).

## progress toward that goal

- [ ] test
- [x] thing