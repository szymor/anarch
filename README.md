# WIP, NOT RELEASED YET!

**If you really want, feel free to test this, but this isn't finished, so I highly suggest you wait for the release.**

# Anarch

![](media/logo_big.png)

[TODO website link]()
TODO trailer link

*extremely small, completely public domain, no-dependency, no-file,
portable suckless anarcho-pacifist from-scratch 90s-style Doom clone that runs
everywhere, made for the benefit of all living beings*

- [why this game is special](#why-this-game-is-special)
- [stats and comparisons (for version 1.0)](#stats-and-comparisons-for-version-10)
- [manifesto](#manifesto)
- [FAQ](#faq)
- [code guide](#code-guide)
- [contributing](#contributing)
- [usage rights](#usage-rights)

## Why this game is special

- **Completely public domain (CC0) free softare, free culture, libre game** for the benefit of all living beings in the Universe, **no conoditions on use whatsoever**. **All art is original** work and licensed CC0 (as well as code).
- **100% non-commercial**, free of any ads, spyware, microtransactions, corporate logos, planned obsolescence etc.
- **Extemely low HW demands** (much less than Doom, no GPU, no FPU, just kilobytes of RAM and storage).
- **Suckless, KISS, minimal, simple**, short code (< 10000 LOC TODO).
- **Extremely portable** (much more than Doom). So far officially ported to and tested on:
  - GNU/Linux PC, SDL and csfml
  - GNU/Linux PC, terminal
  - Browser
  - Pokitto (220 x 116, 48 MHz ARM, 36 KB RAM, 256 KB flash)
  - Gamebino Meta (80 x 64, 48 MHz, 32 KB RAM, 256 KB flash)
  - TODO
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

## Stats and comparisons (for version 1.0)

source code stats (80 column wrapping, a lot of empty lines):

| file                                  | LOC     | file size |
| --------------------------------------| ------- | --------- |
| game.h (main game logic)              | TODO    | TODO      |
| raycastlib.h (ray casting library)    | TODO    | TODO      |
| smallinput.h (for terminal frontend)  | TODO    | TODO      |
| images.h, levels.h, sounds.h (assets) | TODO    | TODO      |
| settings.h, constants.h               | TODO    | TODO      |
| **total**                             | TODO    | TODO      |
| Doom source code                      | 38000   | 1.6 MB    |
| Wolf 3D source code                   | 25000   | 916 KB    |

compiled:

| binary               | size                                 | target FPS  | resolution | RAM usage  |
| -------------------- | ------------------------------------ | ----------- | ---------- | ---------- |
| GNU/Linux, SDL       | TODO (gcc), TODO (clang), TODO (tcc) | TODO        | TODO       | TODO       |
| GNU/Linux, terminal  | TODO (gcc), TODO (clang), TODO (tcc) | TODO        | TODO       | TODO       |
| Pokitto              | TODO                                 | TODO        | TODO       | TODO       |
| Gamebuino Meta       | TODO                                 | TODO        | TODO       | TODO       |
| browser              | TODO (JS script)                     | TODO        | TODO       | TODO       |

system requirements:

| type               | Anarch                           | Wolf 3D                     | Doom                       |
| ------------------ | -------------------------------- | --------------------------- | -------------------------- |
| CPU                | ~40 MHz, 32 bit                  | Intel 286, ~10 MHz 16 bit   | Intel 386, ~40 MHz, 32 bit |
| RAM                | 32 KB                            | 528 KB                      | 4 MB                       |
| storage            | 200 KB                           | 8 MB                        | 12 MB                      |
| display            | 60 x 32, 8 colors, double buffer | ?                           | ?                          |
| additional         | 7 buttons, frame buffer          | HDD, frame buffer           | HDD, frame buffer          |

features:

| type                          | Anarch                           | Wolf 3D           | Doom              |
| ----------------------------- | -------------------------------- | ----------------- | ----------------- |
| levels                        | 10                               | 60 (10 shareware) | 27 (9 shareware)  |
| variable floor/ceiling height | yes                              | no                | yes               |
| engine                        | ray casting                      | ray casting       | BSP               |
| movement inertia              | no                               | no                | yes               |
| head/weapon bobbing           | yes                              | no                | yes               |
| shearing (up/down look)       | yes                              | no                | no                |
| non-square based levels       | no                               | no                | yes               |
| jumping                       | yes                              | no                | no                |
| weapons                       | 6                                | 4                 | 8                 |
| enemy types                   | 7                                | 9                 | 10                |
| ammo types                    | 3                                | 1                 | 4                 |
| armor                         | no                               | no                | yes               |
| difficulty levels             | no                               | 4                 | 5                 |

## manifesto

In today's world of capitalism and fascism no one thinks anymore about doing something without personal benefit, without expecting something in return. Complete selflessness and aim for the pure indiscriminatory long-term benefit of everyone is no longer even considered and if it appears by chance, it is laughed at and portrayed as stupidity. Technology that we are using every day is infected by this poison more than anything else.

From engineering point of view our technology is the worst in history – software is unbelievable bloated, ugly, unelegant, buggy, slow and inefficient, purposefully designed to break, to refuse to work and to enslave people, invade their privacy, to be consumed, to resist tinkering and improvement, by law, force, obscurity, brainwashing and other means. Technology doesn't serve people, it serves corporations on the detriment of people. Principles of good, efficient design and focus on long-term values have vanished in the capitalist selfish short-sighted mentality of greed. The poison has already infected the brains of the masses who are no longer even able to see their own abuse.

This game is a result of my grief about this deeply sad and frightening state of our world. It is my desperate attempt at showing that something, or rather everything, is wrong. I have put great effort in creating this completely from the ground up so that I could waive all my rights and give this away to everyone. I think it is a great shame of mankind that until now there has been no such a game completely, truly and genuinely in the public domain for all people to freely play, for students and teachers to freely study and teach, for artists and programmers to freely hack, improve and remix.

This game is also designed to be truly good technology, with as few burdens to anyone as possible, for users, programmers, distributors etc. It is designed to last for centuries without maintenance – by only relying on a C compiler, a relatively simple and perhaps the most essential piece of software, and by avoiding all unnecessary dependencies, both HW and SW, the game is almost certainly guaranteed to be easily compilable long time in the future, probably even shortly after we will have partlialy recovered from the inevitable technological collapse. The game is free and simple, modifiable by anyone with basic programming skills, not just legally but also practically (i.e. it is suckless). The design follows the essential rules of minimalism, simplicity, efficiency, elegance, hackability, openness.

As any disturbance of the status quo, my project too gets a lot of hate, being called backwards, accused of utilizing wrong programming practice, or just being a bad game in general. Nevertheless, it has only been made with love and sincere rational belief in its philosophies. I do not see it as a step backwards, but rather a step in a different, better direction. I believe that if you judge it with an open mind, you may find the hidden truth and revise your views of technology, its philosophy and its future, as I have. This game has been made for you and I hope you will find it useful.

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

### Why doesn't this have feature X? Even Doom had it.

Though inspired by Doom, this is **NOT** Doom. Keep in mind that this isn't primarily a PC game, this is a small platform independent game that happens to also natively run on PC. Also this game's goals are different than those of commercial games, it is still to a big degree just an experimental game. It has been written by a single person who had to do the design, engine programming, game programming, tool programming, graphics, sound recording, testing, media creation, all in spare time.
Please don't hate me.

### Shouldn't games simply be fun? You're complicating everything with ideological bullshit when it's really just about entertainment.

Games should definitely be fun to play, but they are still technology and engineering art. We have more than enough games that are trying to be just fun before everything else, but practically none putting a little more emphasis also on other aspects -- projects that don't spend all the effort on the shallow shell, but care equally about the insides and the wider context of the world they exist in. I think we need many more games like this one.

### Why aren't you using "modern" programming (C++17, Rust, OOP etc.) or "advanced" engines?

Because this "modern" technology is an extremely bad choice for building long-lasting, accessible programs. New languages are a product of capitalism, evolved by the markets to serve corporations to make quick profit, not fulfilling the values that are good for the people.

This game is suppost to be accessible, i.e. require only as many resources as necessarily needed, in order to run and compile even on "weak" and minimal computers, and to run long in the future, which is ensured by dropping dependencies and only relying on a C compiler, which will probably always be the highest priority piece of SW. After the technological collapse a C compiler will be the first SW we'll have to write, and with it this game will basically immediately be compilable.

### But you're using python scripts, Javascript for the web port, the PC port depends on SDL etc. Don't you contradict yourself?

No, all of these are optional. The core doesn't have any dependencies other than a C99 compiler. Frontends do depend on external libraries, but I've designed the frontend interface so that it's extremely easy to write one, so if let's say SDL dies, it's no problem to write a new frontend using another library.

Python scripts are only simple helpers for converting resources, which aren't required during compilation or code modification. In case python ceases to exist, the scripts can easily be rewritten to another languages, they're fairly simple.

### Why aren't you writing in assembly then?

Because assembly isn't portable and even a "portable assembly" (bytecode) would make it too difficult to write a game of this complexity, and still probably wouldn't be as portable as C. C is about the minimum required abstraction.

### I can make this in "Unity" in a week.

Firstly that's not a question and secondly you misunderstand the essence of this project. Your game will merely *look* the same, it will be an insult to good programming, efficient technology, users' freedom, it won't offer the same independence, portability, performance, beauty, it will probably die along with your "Unity", it will be encoumbered by licenses of your asset store, it won't carry the important messages.

### How long did this take you to make?

Depends from where you count. From my [first experiments with raycasting on Pokitto](https://talk.pokitto.com/t/pokitto-is-on-the-way-what-games-should-i-make/1266/64?u=drummyfish) it's some two years of relaxed evening programming, with taking quite long breaks.

### What tools did you use to make this game?

Only free software, preferably suckless tools: Vim text editor on Devuan GNU/Linux, mostly on a librebooted Lenovo X200 laptop. GIMP was used to make images and maps, python for small scripts and data conversions. Audacity and Blender and other programs were also used for making the trailer etc.

### So I can do anything with this for free? Even like sell it and stuff?

Basically yes, since I have given up all my IP rights, legally you can do anything with this **which is not otherwise illegal** (see the next paragraph), without needing any permission from me, which means you can play this, modify this, sell this, or do anything else without even crediting me. That is all legal, however that doesn't mean it's automatically moral or that I endorse anything you do. As an anarchist I don't want laws preventing your freedom to do anything with a copy of information. It is your responsibility to decide what is moral behavior.

Note that you may still be interested in that **legally there are still things you cannot do with a public domain work like this**. These are not my conditions, just general laws that I am informing you about. For example you cannot claim copyright for an unmodified version of the game, e.g. you **cannot** prevent others from using it in any way they want, e.g. you can sell this but you **cannot** stop others from also selling it or even redistributing it for free (notice how enforcing copyright is no longer a freedom to use one's own copy of information in any way, which is what I am giving you). Even if crediting me isn't required, you probably **cannot** claim the lie that you are the author of the unmodified version because that, again, is a false claim of copyright. Of course, you **can** add your own creations and modifictions to this work (e.g. levels, code, ...) and then claim copyright, but **only** to those specific parts you yourself created.

I don't enforce anything and don't even judge your actions, but I would still be happy if you voluntarily share with me some money that you make on this, if you credit me, if you share your derivative works as public domain or start promoting some of my philosophies. I believe these things are moral, but morals cannot and mustn't be forced. This is why I don't license this as copyleft.

Am I so stupid as to trust complete strangers to not abuse this? No, I know people will "abuse" my work and I am predict some Chinese company will soon be distributing this somehow with a false claim copyright (in which case please don't believe them, there exists evidence of me creating this and releasing this first, with all rights waived). The good that will come from this will be greater than the bad. Perhaps this alone will make you think.

### What values is this work trying to present?

The game itself can't communicate many values directly – it is just a simple game with very little text – but it tries to **bring attention** to the values of its author. Some of these are:

- [Anarchism](https://theanarchistlibrary.org/library/the-anarchist-faq-editorial-collective-an-anarchist-faq-02-17), which [automatically means](https://theanarchistlibrary.org/library/the-anarchist-faq-editorial-collective-an-anarchist-faq-12-17#toc16) anti-capitalism and caring about all other living beings.
- [Pacifism](https://en.wikipedia.org/wiki/Anarcho-pacifism), because true effort for equality can't be pursued by violence and other forms of oppression.
- [Minimalism](https://suckless.org/philosophy/) and simplicity, in technology and elsewhere, as a means of freedom and beauty.
- [Software](https://en.wikipedia.org/wiki/Free_software), [cultural](https://en.wikipedia.org/wiki/Free-culture_movement) and information freedom, opposition to intellectual property.
- Future proof technology.
- Vegetarianism, veganism, equality of life forms.
- Selflessness, sharing, collaboration, caring about others.

### Why ray casting and not e.g. BSP?

This all started with me just creating a very simple ray casting library while playing around with Pokitto, since ray casting is pretty simple. It e.g. allows easy creation of levels and doesn't require precomputation of accelerating structures. I kept improving the library and ended up with raycastlib, a more advanced ray casting library. The idea of creating a Doom clone wasn't planned from the beginning so when it came, I simply used what I had. Sure, BSP would work too, but raycasting makes Anarch kind of unique, there is not many similar games. Lately I've been thinking about creating a BSP library too, so maybe there will once be Anarch 2, who knows?

### Why CC0? Why not MIT or copyleft?

Because I strongly reject the concept of intellectual property and laws in general and because I simply don't want any bullshit burdening my creation. MIT is **not** public domain dedication, it still acknowledges copyright and has a burdening requirement of having to include a copyright notice, and it is also worded kind of vaguely. Copyleft puts even heavier legal burden on users, is by nature unclear and makes us "marry the lawyers", i.e. approve of the IP bullshit and show willingness to enforce only "good" use of our art, as if we're some kind of dictator authority.

It is shame that so little software is truly in the public domain for anyone to simply use without restriction and distraction. Yes, I am aware of CC0's shortcoming, but it's the best we have and I try to address the issues with an extra waiver.

### I've found a bug. What do?

Firstly don't panic and please don't hate me, I didn't put that bug there intentionally. Send me a mail or open an issue on GL to let me know and I'll probably try to fix it. If I for some reason can't, ask some other programmer, or fix it yourself, it most likely isn't as difficult as fixing a bug in 1M LOC program wrapped in layers of "design patterns" and written in 10 languages. 

### Who are you?

I am an anarcho-pacifist programmer. You can read more about me at [my website](https://www.tastyfish.cz). You can read my political manifesto here: [Non-Competitive Society](https://gitlab.com/drummyfish/my_writings/-/blob/master/non-competitive%20society.pdf).

### You sound like an insane person, are you crazy?

I have mental issues as most people nowadays and I go crazy from living in this world, but these don't affect my reasoning. I challenge you to critically evaluate the ideas I present.

### Can I support you?

Yes. This wasn't made for any profit but if you just want to share with a fellow human being, I'll be glad. You can find my support info at [my website](https://www.tastyfish.cz#support). You can also pay what you want on itch.io.

### Fuck you.

Peace.

## code guide

Most things should be obvious and are documented in the source code itself. This
is just an extra helper.

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
raycastlib.h     ray casting library
settings.h       game settings that users can change (FPS, resolution, ...)
smallinput.h     helper for some frontends and tests, not needed for the game
sounds.h         sounds from assets folder converted to C
texts.h          game texts
make.sh          compiling script constaining compiler settings
HTMLshell.html   HTML shell for emscripten (browser) version
index.html       game website
README.md        this readme
```

Main **game logic** is implemented in `game.h` file. All global identifiers of the game code start with the prefix `SFG_` (meaning *suckless FPS game*). Many identifiers also start with `RCL_` – these belong to the ray casting library.

The game core only implements the back end independent of any platforms and libraries. This back end has an API – a few functions that the front end has to implement, such as `SFG_setPixel` which should write a pixel to the platform's screen. Therefore if one wants to **port** the game to a new platform, it should be enough to implement these few simple functions and adjust game settings.

A deterministic **game loop** is used, meaning every main loop/simulation iteration has a constant time step, independently of actually achieved FPS. The FPS that is set determines both the target rendering FPS as well as the length of the simulation step: rendering at higher FPS than that of the simulation would bring no visual benefit as the draw function renders the game state as-is, without interpolating towards the next frame etc. Note that the game will behave slightly differently with different FPS values due to rounding errors, but should be okay as long as the FPS isn't extremely low (e.g. 2) or high (e.g. 1000). If the set  FPS can't be reached, the game will appear slowed down, so set the FPS to a value that your platform can handle.

The **rendering engine** -- raycastlib -- works on the principle of **ray casting** on a square grid and handles the rendering of the 3D environment (minus sprites). This library also handles player's **collisions** with the level. There is a copy of raycastlib in this repository but I maintain raycastlib as a separate project in a different repository, which you can see for more details about it. For us, the important functions interfacing with the engine are e.g. `SFG_floorHeightAt`, `SFG_ceilingHeightAt` (functions the engine uses to retirieve floor and ceiling height) and `SFG_pixelFunc` (function the engine uses to write pixels to the screen during rendering, which in turn uses each platform's specific `SFG_setPixel`). It is documented in its own source code.

Only **integer arithmetic** is used, no floating point is needed. Integers are effectively used as fixed point numbers, having `RCL_UNITS_PER_SQUARE` (1024) fractions in a unit. I.e. what we would write as 1.0 in floating point we write as 1024, 0.5 becomes 512 etc.

**Mods** of the vanilla version are recommended to be made as patches, so that they can easily be combined etc. If you want your mod to remain free, please don't forget a license/waiver, even for a small mod. You can never go wrong by including it.

**Sprite** (monster, items, ...) rendering is intentionally kept simple and doesn't always give completely correct result, but is good enough. Sprite scaling due to perspective should properly be done with respect to both horizontal and vertical FOV, but for simplicity the game scales them uniformly, which is mostly good enough. Visibility is also imperfect and achieved in two ways simultaneously. Firstly a 3D visibility ray is cast towards each active sprite from player's position to check if it is visible or not (ignoring the possibility of partial occlusion). Secondly a horizontal 1D z-buffer is used solely for sprites, to not overwrite closer sprites with further ones.

**Monster sprites** only have one facing direction: to the player. To keep things small, each monster has only a few frames of animations: usually 1 idle frame, 1 waling frame, 1 attacking frame. For dying animation a universal "dying" frame exists for all mosnters.

All ranged attacks in the game use **projectiles**, there is no hit scan.

The game uses a custom general purpose HSV-based 256 color **palette** which I again maintain as a separate project in a different repository as well (see it for more details). The advantage of the palette is the arrangement of colors that allows increasing/decreasing color value (brightness) by incrementing/decrementing the color index, which is used for dimming environment and sprites in the distance into shadow/fog without needing color mapping tables (which is what e.g. Doom did), saving memory and CPU cycles for memory access.

All **assets** are embedded directly in the source code and will be part of the compiled binary. This way we don't have to use file IO at all, and the game can run on platforms without a file system. Some assets use very simple compression to reduce the binary size. Provided python scripts can be used to convert assets from common formats to the game format.

All **images** in the game such as textures, sprites and backgrounds (with an exception of the font) are 32 x 32 pixels in 16 colors, i.e. 4 bits per pixel. The 16 color palette is specific to each image and is a subpalette of the main 256 color palette. The palette is stored before the image data, so each image takes 16 + (32 * 32) / 2 = 528 bytes. This makes images relatively small, working with them is easy and the code is faster than would be for arbitrary size images. One color (red) is used to indicate transparency.

The game uses a custom very simple 4x4 **font** consisting of only upper case letters and a few symbols, to reduce its size.

For **RNG** a very simple 8 bit congruent generator is used, with the period 256, yielding each possible 8 bit value exactly once in a period.

**AI** is very simple. Monsters update their state in fixed time ticks and move on a grid that has 4 x 4 points in one game square. Monsters don't have any direction, just 2D position (no height). Close range monsters move directly towards player and attack when close enough. Ranged monsters randomly choose to either shoot a projectile (depending on the aggressivity value of the monster's type) or move in random direction.

User/platform **settings** are also part of the source code, meaning that change of settings requires recompiling the game. To change settings, take a look at the default values in `settings.h` and override the ones you want by defining them before including `game.h` in your platform's front end source code.

To increase **performance**, you can adjust some settings, see settings.h and search for "performance". Many small performance tweaks exist. If you need a drastic improvement, you can set ray casting subsampling to 2 or 3, which will decrease the horizontal resolution of raycasting rendering by given factor, reducing the number of rays cast, while keeping the resolution of everything else (vertical, GUI, sprites, ...) the same. You can also divide the whole game resolution by setting the resolution scaledown, or even turn texturing completely off. You can gain or lose a huge amount of performance in your implementation of the `SFG_setPixel` function which is evaluated for every single pixel in every frame, so try to optimize here as much as possible. Also don't forget to use optimization flags of your compiler, they make the biggest difference. You can also try to disable music, set the horizontal resolution to power of 2 and similat things.

**Levels** are stored in levels.h as structs that are manually editable, but also use a little bit of compression principles to not take up too much space, as each level is 64 x 64 squares, with each square having a floor heigh, ceiling heigh, floor texture, ceiling texture plus special properties (door, elevator etc.). There is a python script that allows to create levels in image editors such as GIMP. A level consists of a tile dictionary, recordind up to 64 tile types, the map, being a 2D array of values that combine an index pointing to the tile dictionary plus the special properties (doors, elevator, ...), a list of up to 128 level elements (monsters, items, door locks, ...), and other special records (floor/ceiling color, wall textures used in the level, player start position, ...).

**Saving/loading** is an optional feature. If it is not present (frontend doesn't implement the API save/load functions), all levels are unlocked from the start and no state survives the game restart. If the feature is implemented, progress, settings and a saved position is preserved in permanent storage. What the permanent storage actually is depends on the front end implementation – it can be a file, EEPROM, a cookie etc., the game doesn't care. Only a few bytes are required to be saved. Saving of game position is primitive: position can only be saved at the start of a level, allowing to store only a few values such as health and ammo.

Performance and small size are achieved by multiple **optimization** techniques. Macros are used a lot to move computation from run time to compile time and also reduce the binary size. E.g. the game resolution is a constant and can't change during gameplay, allowing the compiler to precompute many expression with resolution values in them. Powers of 2 are used whenever possible. Approximations such as taxicab distances are used. Some "accelerating" structures are also used, e.g. a 2D bit array for item collisions. Don't forget to compile the game with -O3.

## contributing

**I am not acception any contributions to this repository**, however you can, and are encouraged to, **fork** the repo and do whatever you want with it. This is because I want to keep the original repository completely my own work to prevent the risk of "intellectual property" complications with multiple authors (e.g. a contributor starting to say he agreed to the waiver by mistake) and to make it as clear as possible that this is truly public domain. Yes, **I am afraid of free contribution from volunteers – this is what copyright laws have achieved**. If you dislike this situation, stop supporting copyright and IP laws. Outside this repo of course we can collaborate without borders.

I welcome feedback, such as bug reports, which you can create here on GL as an issue, report to me via email etc.

## usage rights

**tl;dr: everything in this repository is CC0 + a waiver of all rights, completely public domain as much as humanly possible, do absolutely anything you want**

I, Miloslav Číž (drummyfish), have created everything in this repository, including but not limited to code, graphics, sprites, palettes, fonts, sounds, music, storyline and texts, even the font in the video trailer and drum sound samples for the soundtrack, completely myself from scratch, using completely and exclusive free as in freedom software, without accepting any contributions, with the goal of creating a completely original art which is not a derivative work of anyone else's existing work, so that I could assure that by waiving my intellectual property rights the work will become completely public domain with as little doubt as posible.

This work's goal is to never be encumbered by any exclusive intellectual property rights, it is intended to always stay completely and forever in the public domain, available for any use whatsoever.

I therefore release everything in this repository under CC0 1.0 (public domain, https://creativecommons.org/publicdomain/zero/1.0/) + a waiver of all other IP rights (including patents), which is as follows:

*Each contributor to this work agrees that they waive any exclusive rights, including but not limited to copyright, patents, trademark, trade dress, industrial design, plant varieties and trade secrets, to any and all ideas, concepts, processes, discoveries, improvements and inventions conceived, discovered, made, designed, researched or developed by the contributor either solely or jointly with others, which relate to this work or result from this work. Should any waiver of such right be judged legally invalid or ineffective under applicable law, the contributor hereby grants to each affected person a royalty-free, non transferable, non sublicensable, non exclusive, irrevocable and unconditional license to this right.*

I would like to ask you, without it being any requirement at all, to please support free software and free culture by sharing at least some of your own work in a similar way I do with this project.

If you'd like to support me or just read something about me and my projects, visit my site: [www.tastyfish.cz](http://www.tastyfish.cz/).

