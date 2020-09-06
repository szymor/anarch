# WIP, NOT RELEASED YET!

**If you really want, feel free to test this, but this isn't finished, so I highly suggest you wait for the release.**

# Anarch

*suckless FPS, the best game ever made, for the benefit of all living beings*

## Why this game is special

- **Completely public domain (CC0) free softare, free culture, libre game** for the benefit of all living beings in the Universe, **no conoditions on use whatsoever**. **All art is original** work and licensed CC0 (as well as code).
- **100% non-commercial**, free of any ads, spyware, microtransactions, corporate logos, planned obsolescence etc.
- **Extemely low HW demands** (much less than Doom, no GPU, no FPU, just kilobytes of RAM and storage).
- **Suckless, KISS, minimal, simple**, short code (< 10000 LOC TODO).
- **Extremely portable** (much more than Doom). So far tested on GNU/Linux PC, Pokitto, TODO.
- Has **completely NO external dependencies**, not even rendering or IO, that is left to each platform's frontend, but each frontend is very simple.
- Can fit into **less than 256 kb** (including all content, textures etc.).
- Uses **no build system**, can typically be compiled with a single run of compiler.
- **Works without any file IO**, i.e. can work without config files, save files, **all content and configs are part of the source code**.
- **Doesn't use any floating point**, everything is integer math (good for platforms without FPU).
- Written in **pure C99**, also a **subset of C++** (i.e. runs as C++ as well, good for systems that are hard C++ based).
- Made to **last for centuries** without maintenance.
- Goes beyond technical design and also **attempts to avoid possible cultural dependencies and barriers** (enemies are only robots, no violence on living beings).
- **Created with only free software** (GNU/Linux, GIMP, Audacity, gcc, Vim, ...).
- **Single compilation unit** (only one .c file to compile, very fast and simple). No build systems.
- Uses a **custom-made 256 color palette** (but can run on platforms with fever colors, even just two).
- **Well documented and commented code**, written with tinkering and remixing in mind.
- Feels like good old **Doom**.

early previews:

Pokitto:

![](https://talk.pokitto.com/uploads/default/original/2X/2/29c0e4f44688f801013ed5b8463e97772d4b203e.gif)

SDL:

![](https://talk.pokitto.com/uploads/default/original/2X/e/e69a5e26aee3bd726494e793770911ab83345187.png)


## Stats

TODO file LOC summary

TODO platform stats and sizes summary

## manifesto

In today's world of capitalism and fascism no one thinks anymore about doing something without personal benefit, without expecting something in return. Complete selflessness and aim for the pure indiscriminatory long-term benefit of everyone is no longer even considered and if it appears by chance, it is laughed at and portrayed as stupidity. Technology that we are using every day is infected by this poison more than anything else.

From engineering point of view our technology is the worst in history – software is unbelievable bloated, ugly, unelegant, buggy, slow and inefficient, purposefully designed to break, to refuse to work and to enslave people, invade their privacy, to be consumed, to resist tinkering and improvement, by law, force, obscurity, brainwashing and other means. Technology doesn't serve people, it serves corporations on the detriment of people. Principles of good, efficient design and focus on long-term values have vanished in the capitalist selfish short-sighted mentality of greed. The poison has already infected the brains of the masses who are no longer even able to see their own abuse.

This game is a result of my grief about this deeply sad and frightening state of our world. It is my desperate attempt at showing that something, or rather everything, is wrong. I have put great effort in creating this completely from the ground up so that I could waive all my rights and give this away to everyone. I think it is a great shame of mankind that until now there has been no such a game completely, truly and genuinely in the public domain for all people to freely play, for students and teachers to freely study and teach, for artists and programmers to freely hack, improve and remix.

This game is also designed to be truly good technology as much as possible, with as few burdens to anyone as possible, for both users and programmers. It is designed to last for centuries without maintenance – by only relying on a C compiler, a relatively simple and perhaps the most essential piece of software, and by avoiding all unnecessary dependencies, both HW and SW, the game is almost certainly guaranteed to be easily compilable long time in the future, probably even shortly after the inevitable technological collapse. It is free and simple, modifiable by anyone with programming skills, not just legally but also practically (i.e. it is suckless). The design follows the essential rules of minimalism, simplicity, efficiency, elegance, hackability, openness.

As any disturbance to the status quo, my project too gets a lot of hate, being called backwards, accused of utilizing wrong programming practice, or just being a bad game in general. Nevertheless, it has only been made with love and sincere rational belief in its philosophies. I do not see it as a step backwards, but rather a step in a different, better direction. I believe that if you judge it with an open mind, you may find the hidden truth and revise your views of technology, its philosophy and its future, as I have. This game has been made for you and I hope you will find it useful.

## FAQ

### Why?

Because I find it ridiculously depressing that in a world where we have millions of computer games there isn't a single serious one made purely for the benefit of everyone on the planet, without self-interest (licenses, ads, DRM, ...)  being embedded somewhere underneath. It may be one of the saddest things about this so called "advanced" society.

### Is this a joke?

No.

I **will** be ridiculed, made fun of, bullied and dismissed as an *obvious* lunatic, as many similar people in history who have seen the truth behind the curtain of that time's propaganda. And those in front of it will say "this is different because ..." No, this is not different, I am choosing to see the truth and am exiled for it. I am offering the truth to you, dear reader, but you will probably refuse it because it is too scary, uncomfortable, unbearable, and will rather try to rationalize the lies you've been taught. You will likely dismiss this as a mere joke.

### Is this running on Doom engine or what?

No, this is my custom engine (raycastlib) based on raycasting, a technique used in Wolf3D engine, but it's improved, e.g. supporting multiple levels of floor and ceiling, so that the visual result is something between Wolf3D and Doom (which was a BSP engine, i.e. a principle completely different from raycasting). I've made the engine with the same philosophy in mind as the game itself.

### How is this different from the trillion other retro shooters?

Firstly this isn't trying to look like a 90s style shooter, this **is** a 90s style shooter. The code is written in the oldschool language and style in a custom efficient SW rendering engine, just like in the old days, but taken further and evolved in a new direction.

Secondly this is more than a game and is not made for any profit but for the benefit of all people. This is not a product of capitalism, but a manifesto and a selfless work of art. The code and assets are free as in freedom, meaning you can do anything you want with them -- you don't have to buy this, agree to any terms, nothing is hidden from you.

### Isn't this backwards? The graphics looks like shit.

I wouldn't call this backwards, but rather a different, better direction than which the mainstream technology is taking, though this involves taking some steps back to before the things started to go wrong, which is why the result looks like from early 90s, but from there I am trying to go forwards in the new  direction.

The new direction is towards minimalism, simplicity, accessibility ("openness", portability, low HW demands, low education demands, ...), low maintenance cost, being future-proof, helpful to people at large and so on. Such SW is sometimes called suckless or countercomplex.

Why go back to the 90s and not further? Early 90s is roughly right before PCs and games started becoming "too popular" and before comsumerism started deeply infecting and destroying the technology itself, i.e. before programming languages such as Java or platforms like MS Windows became wide spread. At this time games were still written in C (a language invented by science, not capitalism), used software rendering even for 3D, didn't use bloat such as multithreading etc.

What we perceive as good graphics is heavily dependant on what we've learned to perceive as good graphics, and it's more about aesthetics than things like resolution or polygon count. Doom looked amazing when it came out and it still does today to people who didn't let the industry teach them that good graphics equals super HD with realistic shaders requiring the latest and most expensive GPU.

### Shouldn't games simply be fun? You're complicating everything with ideological bullshit when it's really just about entertainment.

Games should definitely be fun to play, but they are still technology and engineering art. We have more than enough games that are trying to be just fun before everything else, but practically none putting a little more emphasis also on other aspects -- projects that don't spend all the effort on the shallow shell, but care equally about the insides and the wider context of the world they exist in. I think we need many more games like this one.

### Why aren't you using "modern" programming (C++17, Rust, OOP etc.) or "advanced" engines?

Because this "modern" technology is an extremely bad choice for building long-lasting, accessible programs. New languages are a product of capitalism, evolved by the markets to serve corporations to make quick profit, not fulfilling the values that are good for the people.

This game is suppost to be accessible, i.e. require only as many resources as necessarily needed, in order to run and compile even on "weak" and minimal computers, and to run long in the future, which is ensured by dropping dependencies and only relying on a C compiler, which will probably always be the highest priority piece of SW. After the technological collapse a C compiler will be the first SW we'll have to write, and with it this game will basically immediately be compilable.

### I can make this in "Unity" in a week.

Firstly that's not a question and secondly you misunderstand the essence of this project. Your game will merely *look* the same, it will be an insult to good programming, it won't offer the same freedom, independence, portability, performance, beauty, it won't carry the message.

### But you're using python scripts, Javascript for the web port, the PC port depends on SDL etc. Don't you contradict yourself?

No, all these are optional. The core doesn't have any dependencies other than a C99 compiler. Frontends do depend on external libraries, but I've designed the frontend interface so that it's extremely easy to write one, so if let's say SDL dies, it's no problem to write a new frontend using another library.

Python scripts are only simple helpers for converting resources, which aren't required during compilation or code modification. In case python ceases to exist, the scripts can easily be rewritten to another languages, they're fairly simple.

### Why aren't you writing in assembly then?

Because assembly isn't portable and even a portable assembly would make it too difficult to write a game of this complexity. C is about the minimum required abstraction.

### So I can do anything with this for free? Even like sell it and stuff?

Basically yes, since I have given up all my IP rights, legally you can do anything with this **which is not otherwise illegal** (see the next paragraph), without needing any permission from me, which means you can play this, modify this, sell this, or do anything else without even crediting me. That is all legal, however that doesn't mean it's automatically moral or that I endorse anything you do. As an anarchist I don't want laws preventing your freedom to do anything with a copy of information. It is your responsibility to decide what is moral behavior.

Note that you may still be interested in that **legally there are still things you cannot do with a public domain work like this**. These are not my conditions, just general laws that I am informing you about. For example you cannot claim copyright for an unmodified version of the game, e.g. you **cannot** prevent others from using it in any way they want, e.g. you can sell this but you **cannot** stop others from also selling it or even redistributing it for free (notice how enforcing copyright is no longer a freedom to use one's own copy of information in any way, which is what I am giving you). Even if crediting me isn't required, you probably **cannot** claim the lie that you are the author of the unmodified version because that, again, is a false claim of copyright. Of course, you **can** add your own creations and modifictions to this work (e.g. levels, code, ...) and then claim copyright, but **only** to those specific parts you yourself created.

I don't enforce anything and don't even judge your actions, but I would still be happy if you voluntarily share with me some money that you make on this, if you credit me, if you share your derivative works as public domain or start promoting some of my philosophies. I believe these things are moral, but morals cannot and mustn't be forced. This is why I don't license this as copyleft.

Am I so stupid as to trust complete strangers to not abuse this? No, I know people will "abuse" my work and I am predict some Chinese company will soon be distributing this somehow with a false claim copyright (in which case please don't believe them, there exists evidence of me creating this and releasing this first, with all rights waived). The good that will come from this will be greater than the bad. Perhaps this alone will make you think.

### Why raycasting and not e.g. BSP?

This all started with me just creating a very simlpe raycasting library while playing around with Pokitto, since raycasting is pretty simple. It e.g. allows easy creation of levels and doesn't require precomputation of accelerating structures. I kept improving the library and ended up with raycastlib, a more advanced raycasting library. The idea of creating a Doom clone wasn't planned from the beginning so when it came, I simply used what I had. Sure, BSP would work too, but raycasting makes Anarch kind of unique, there is not many similar games. Lately I've been thinking about creating a BSP library too, so maybe there will once be Anarch 2, who knows?

### Why CC0? Why not MIT or copyleft?

Because I strongly reject the concept of intellectual property and laws in general and because I simply don't want any bullshit burdening my creation. MIT is **not** public domain dedication, it still acknowledges copyright and has a burdening requirement of having to include a copyright notice, and it is also worded kind of vaguely. Copyleft puts even heavier legal burden on users and makes us "marry the lawyers", i.e. approve of the IP bullshit and show willingness to enforce only "good" use of our art, as if we're some kind of dictator authority.

It is shame that so little software is truly in the public domain for anyone to simply use without restriction and distraction. Yes, I am aware of CC0's shortcoming, but it's the best we have and I try to address the issues with an extra waiver.

### Who are you?

I am an anarcho-pacifist programmer. You can read more about me at [my website](https://www.tastyfish.cz). You can read my political manifesto here: [Non-Competitive Society](https://gitlab.com/drummyfish/my_writings/-/blob/master/non-competitive%20society.pdf).

### You sound like an insane person, are you crazy?

I have mental issues as most people nowadays, but they don't affect my reasoning. I challenge you to critically evaluate the ideas I present to you.

### Can I support you?

Yes. This wasn't made for any profit but if you just want to share with a fellow human being, I'll be glad. You can find my support info at [my website](https://www.tastyfish.cz).

### Fuck you.

Peace.

## code guide

The repository structure is following:

```
assets/          asset sources (textures, sprites, maps, sounds, ...)
  *.py           scripts for converting assets to C structs/arrays
media/           media presenting the game (screenshots, logo, ...)
constants.h      game constants that aren't considered settings
game.h           main game logic
images.h         images (textures, sprites) from assets folder converted to C
levels.h         levels from assets folder converted to C
main_*.*         fronted implement. for various platforms, passed to compiler
palette.h        game 256 color palette
raycastlib.h     raycasting library
settings.h       game settings that users can change (FPS, resolution, ...)
sounds.h         sounds from assets folder converted to C
texts.h          game texts
make.sh          compiling script constaining compiler settings
HTMLshell.html   HTML shell for emscripten (browser) version
index.html       game website
README.md        this readme
```

TODO

source files

portability, only backend, avoiding dependencies

how to port to new platform, frontend, configs in files

All global identifiers of the game code start with the prefix `SFG_` (meaning *suckless FPS game*).

The **engine** -- raycastlib -- works on the principle of **raycasting** on a square grid and handles the rendering of the 3D environment (minus sprites). There is a copy of raycastlib in this repository but I maintain raycastlib as a separate project in a different repository, which you can see for more details about it. For us, the important functions interfacing with the engine are e.g. `SFG_floorHeightAt`, `SFG_ceilingHeightAt` (functions the engine uses to retirieve floor and ceiling height) and `SFG_pixelFunc` (function the engine uses to write pixels to the screen during rendering, which in turn uses each platform's specific `SFG_setPixel`).

integer math

sprites 1D zbuffer

The game uses a custom general purpose HSV-based 256 color **palette** which I again maintain as a separate project in a different repository as well (see it for more details). The advantage of the palette is the arrangement of colors that allows increasing/decreasing color value (brightness) by incrementing/decrementing the color index, which is used for dimming environment and sprites in the distance into shadow/fog without needing color mapping tables (which is what e.g. Doom did), saving memory and CPU cycles for memory access.

All **images** in the game such as textures, sprites and backgrounds (with just a few exceptions such as font) are 32 x 32 pixels in 16 colors, i.e. 4 bits per pixel. The 16 color palette is specific to each image and is a subpalette of the main 256 color palette. The palette is stored before the image data, so each image takes 16 + (32 * 32) / 2 = 528 bytes. This makes images relatively small, working with them is easy and the code is faster than would be for arbitrary size images. One color (red) is used to indicate transparency.

font

rng, produces all byte values

AI

map format

main loop, constant time step

saving/loading is optional

optimizations, move computations to compile time, approximations (taxicab, ...), precomputations, -O3

## usage rights

**tl;dr: everything in this repository is CC0 + a waiver of all rights, completely public domain as much as humanly possible, do absolutely anything you want**

I, Miloslav Číž (drummyfish), have created everything in this repository, including but not limited to code, graphics, sprites, palettes, fonts, sounds, music and texts, completely myself from scratch, using completely and exclusive free as in freedom software, without accepting any contributions, with the goal of creating a completely original art which is not a derivative work of anyone else's existing work, so that I could assure that by waiving my intellectual property rights the work will become completely public domain with as little doubt as posible.

This work's goal is to never be encumbered by any exclusive intellectual property rights, it is intended to always stay completely and forever in the public domain, available for any use whatsoever.

I therefore release everything in this repository under CC0 1.0 (public domain, https://creativecommons.org/publicdomain/zero/1.0/) + a waiver of all other IP rights (including patents), which is as follows:

*Each contributor to this work agrees that they waive any exclusive rights, including but not limited to copyright, patents, trademark, trade dress, industrial design, plant varieties and trade secrets, to any and all ideas, concepts, processes, discoveries, improvements and inventions conceived, discovered, made, designed, researched or developed by the contributor either solely or jointly with others, which relate to this work or result from this work. Should any waiver of such right be judged legally invalid or ineffective under applicable law, the contributor hereby grants to each affected person a royalty-free, non transferable, non sublicensable, non exclusive, irrevocable and unconditional license to this right.*

I would like to ask you, without it being any requirement at all, to please support free software and free culture by sharing at least some of your own work in a similar way I do with this project.

If you'd like to support me or just read something about me and my projects, visit my site: [www.tastyfish.cz](http://www.tastyfish.cz/).
