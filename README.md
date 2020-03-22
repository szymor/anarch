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

### Is this running on Doom engine or what?

No, this is my custom engine (raycastlib) based on raycasting, a technique used in Wolf3D engine, but it's improved, e.g. supporting multiple levels of floor and ceiling, so that the visual result is something between Wolf3D and Doom (which was a BSP engine). I've made the engine with the same philosophy in mind as the game itself.

### How is this different from the trillion other retro shooters?

Firstly this isn't trying to look like a 90s style shooter, this **is** a 90s style shooter. The code is written in the oldschool language and style, in a custom efficient SW rendering engine, just like in the old days, meaning this will run anywhere.

Secondly this is more than a game and is not made for any profit but for the benefit of all people. This is not a product of capitalism, but a manifesto and a selfless work of art. The code and art are free as in freedom, meaning you can do anything you want with them -- you don't have to buy this, agree to any terms, nothing is hidden from you.

### Isn't this backwards? The graphics looks like shit.

I wouldn't call this backwards, but rather a different, better direction than which the mainstream technology is taking, though this involves taking some steps back to before the things started to go wrong, which is why the result looks like from early 90s, but from there I am trying to go forwards in the new  direction.

The new direction is towards minimalism, simplicity, accessibility ("openness", portability, low HW demands, low education demands, ...), low maintenance cost, being future-proof, helpful to people at large and so on. Such SW is sometimes called suckless or countercomplex.

### Shouldn't games simply be fun? You're complicating everything with ideological bullshit when it's really just about entertainment.

Games should definitely be fun to play, but they are still technology and engineering art. We have more than enough games that are trying to be just fun before everything else, but practically none putting a little more emphasis also on other aspects -- projects that don't spend all the effort on the shallow shell, but care equally about the insides and the wider context of the world they exist in. I think we need many more games like this one.

### Why aren't you using "modern" programming (C++17, Rust, OOP etc.) or "advanced" engines?

Because this "modern" technology is an extremely bad choice for building long-lasting, accessible programs. New languages are a product of capitalism, evolved by the markets to serve corporations to make quick profit, not fulfilling the values that are good for the people.

This game is suppost to be accessible, i.e. require only as many resources as necessarily needed, in order to run and compile even on "weak" and minimal computers, and to run long in the future, which is ensured by dropping dependencies and only relying on a C compiler, which will probably always be the highest priority piece of SW. After the technological collapse a C compiler will be the first SW we'll have to write, and with it this game will basically immediately be compilable.

### But you're using python scripts, Javascript for the web port, the PC port depends on SDL etc. Don't you contradict yourself?

No, all these are optional. The core doesn't have any dependencies other than a C99 compiler. Frontends do depend on external libraries, but I've designed the frontend interface so that it's extremely easy to write one, so if let's say SDL dies, it's no problem to write a new frontend using another library.

Python scripts are only simple helpers for converting resources, which aren't required during compilation or code modification. In case python ceases to exist, the scripts can easily be rewritten to another languages, they're fairly simple.

### Why aren't you writing in assembly?

Because assembly isn't portable and even a portable assembly would make it too difficult to write a game of this complexity. C is about the minimum required abstraction.

### You sound like an insane persone, are you crazy?

I do have mental issues, but they don't affect my reasoning. If you are interested in truth, you should always critically judge ideas in themselves without dismissing them based on their originator.

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
