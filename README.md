# WIP

# Anarch

*suckless FPS, the best game ever made, for the benefit of all living beings*

## This is going to likely be by far the best game in the universe.

Why?

- **Like Doom** but much better because of the following.
- **Extemely low HW demands** (much less than Doom, no GPU, no FPU, just kilobytes of RAM and storage).
- **Extremely portable** (much more than Doom). So far tested on GNU/Linux PC, Pokitto, TODO.
- Has **completely NO external dependencies**, not even rendering or IO, that is left to each platform's frontend, but each frontend is very simple.
- Fits into **less than 256 kb** (including all content, textures etc.).
- Uses **no build system**, can typically be compiled with a single run of compiler.
- **Works without any file IO**, i.e. can work without config files, save files, **all content and configs are part of the source code**.
- **Doesn't use ANY floating point**, everything is integer math (good for platforms without FPU).
- **Pure C99**, also a **subset of C++** (i.e. runs as C++ as well, good for systems that are hard C++ based).
- **Completely public domain (CC0) free softare, free culture, libre game** for the benefit of all living beings in the Universe, **no conoditions on use whatsoever**. **All art is original** work and licensed CC0 (as well as code).
- **Suckless, KISS, minimal, simple**, short code (< 10000 LOC).
- Made to **last for centuries** without maintenance.
- Goes beyond technical design and also **attempts to avoid possible cultural dependencies and barriers** (enemies are only robots, no violence on living beings).
- **Creted with only completely free software** (Debian, GIMP, gcc, Vim, ...).
- **Single compilation unit** (only one .c file to compile, very fast and simple).
- Uses a **custom-made 256 color palette** (but can run on platforms with fever colors, even just two).
- **Well documented and commented code**, written with tinkering and remixing in mind.

early previews:

Pokitto:

![](https://talk.pokitto.com/uploads/default/original/2X/2/29c0e4f44688f801013ed5b8463e97772d4b203e.gif)

SDL:

![](https://talk.pokitto.com/uploads/default/original/2X/e/e69a5e26aee3bd726494e793770911ab83345187.png)

## manifesto

In today's world of capitalism and fascism no one even thinks about doing something without personal benefit, without expecting something in return. Complete selflessness is no longer considered an option, and if it appears by chance, it is ridiculed. Technology that we are using every day reflects the aggressive, hostile thinking.

From engineering point of view our technology is the worst in history. Computer technology is unbelieveable bloated, ugly, buggy, slow and inefficient, purposfuly designed to enslave people, to invade their lives, to be consumed, to resist tinkering and improvement, by law, forcre, obscurity or other means. Technology doesn't serve people, it serves corporations on the detriment of people. Principles of good, efficient design and focus on long term values have vanished in the capitalist selfish short sighted mentality of greed which has already infected the mentality of the masses who are no longer even able to see their own abuse and enslavement.

This game is a result of my grief about this deeply sad and frightening state of the world, it is my desperate attempt at showing that something, or rather everything, is wrong. I have put great effort in creating it completely from the ground up so that I could waive my rights of it, give it away to everyone. I think it is a great shame of mankind that since now there has been no such a game completely and truly in the public domain for all people to freely play, for students and techers to freely study and teach, for artists and programmers to freely improve and remix.

this game is also designed to be truly good technology to the greatest extent possible, with as few burdens as possible to all people, both users and developers. It is designed to last for ceturies without maintenance -- by only relying on a C compiler, which is relatively simple and perhaps the most essential piece of software, and by avoiding all unnecessary dependencies, both HW and SW, the game is almost certainly guaranteed to be trivially compileable any time in the future, probably even shortly after the technological collapse of our civilization, which is on its way. It is free and simple, making it modifyable by anyone with programming skills, not just legally but also practically. The game tries to practice the values of good design -- minimalism, simplicity, effectiveness, efficiency, ellegance, hackability, openness.

Besides support my project has also gotten a lot of hate, being called backwards, utilizing wrong programming practice, being a bad game to play. Nevertheless it has only been made with love and sincere rational belief in its philosophies. I do not see it as a step backwards, but rather a step in a different, better direction. I believe in my work and that if you judge it with an open mind, you may find something new and revise your view of technology, its philosophy and its future. This game has been made for you and I hope you will find it useful.

## FAQ

### Why aren't you using "modern" programming (C++17, Rust, OOP etc.)?

Because this "modern" technology is an extremely bad choice for building long-lasting, accessible programs. New languages are a product of capitalism, evolved by the markets to serve corporations to make quick profit, not fulfilling the values that are good for the people.

This game is suppost to be accessible, i.e. require only as many resources as necessarily needed, in order to run and compile even on "weak" and minimal computers, and to run long in the future, which is ensured by dropping dependencies and only relying on a C compiler, which will probably always be the highest priority piece of SW. After the technological collapse a C compiler will be the first SW we'll have to write, and with it this game will basically immediately be compilable.

### Why aren't you writing in assembly?

Because assembly isn't portable and even a portable assembly would make it too difficult to write a game of this complexity. C is about the minimum required abstraction that is needed.

### You sound like an insane persone, are you crazy?

I do have mental issues, but they don't affect my reasoning. If you are interested in truth, you should always critically judge ideas in themselves without dismissign them based on their originator.

## code guide

TODO

source files

portability, only backend, avoiding dependencies

how to port to new platform, frontend, configs in files

all images are 32x32, image compression, transparency

integer math

raycastlib, 1D zbuffer

palette

rng, produces all byte values

AI

map format

main loop, constant time step

optimizations, move computations to compile time, approximations (taxicab, ...), precomputations, -O3

## usage rights

**tl;dr: everything in this repository is CC0 + a waiver of all rights, completely public domain as much as humanly possible, do absolutely anything you want**

I, Miloslav Číž (drummyfish), have created everything in this repository, including but not limited to code, graphics, sprites, palettes, fonts, sounds, music and texts, completely myself from scratch, using completely and exclusive free as in freedom software, without accepting any contributions, with the goal of creating a completely original art which is not a derivative work of anyone else's existing work, so that I could assure that by waiving my intellectual property rights the work will become completely public domain with as little doubt as posible.

This work's goal is to never be encumbered by any exclusive intellectual property rights, it is intended to always stay completely and forever in the public domain, available for any use whatsoever.

I therefore release everything in this repository under CC0 1.0 (public domain, https://creativecommons.org/publicdomain/zero/1.0/) + a waiver of all other IP rights (including patents), which is as follows:

*Each contributor to this work agrees that they waive any exclusive rights, including but not limited to copyright, patents, trademark, trade dress, industrial design, plant varieties and trade secrets, to any and all ideas, concepts, processes, discoveries, improvements and inventions conceived, discovered, made, designed, researched or developed by the contributor either solely or jointly with others, which relate to this work or result from this work. Should any waiver of such right be judged legally invalid or ineffective under applicable law, the contributor hereby grants to each affected person a royalty-free, non transferable, non sublicensable, non exclusive, irrevocable and unconditional license to this right.*

I would like to ask you, without it being any requirement at all, to please support free software and free culture by sharing at least some of your own work in a similar way I do with this project.

If you'd like to support me or just read something about me and my projects, visit my site: [www.tastyfish.cz](http://www.tastyfish.cz/).
