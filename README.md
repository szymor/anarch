# WIP -- suckless FPS game

## This is going to likely be by far the best game in the universe.

Why?

- **Like Doom** but much better because of the following.
- **Extemely low HW demands** (much less than Doom, no GPU, no FPU, just kilobytes of RAM and storage).
- **Extremely portable** (much more than Doom).
- Has **completely NO dependencies**, not even rendering or IO, that is left to each platform's frontend, but every frontend is extremely simple (~100 LOC).
- Fits into **less than 256 kb** (including all content, textures etc.).
- Uses **no build system**, can typically be compiled with a single run of compiler.
- **Works without any file IO**, i.e. can work without config files, save files, **all content and configs are part of the source code**.
- **Doesn't use ANY floating point**, everything is integer math (good for platforms without FPU).
- **Pure C99**, also a **subset of C++** (i.e. runs as C++ as well, good for systems that are hard C++ based).
- **Completely public domain (CC0) free softare, free culture libre game** for the benefit of all people on Earth, no conoditions on use whatsoever. All art is original work and licensed CC0 (as well as code).
- **Suckless, KISS, minimal, simple**.
- Made to **last for centuries** without maintenance.
- Goes beyond technical design and also **attempts to avoid possible cultural dependencies and barriers** (enemies are only robots, no violence on living beings).
- **Creted with only completely free software** (Debian, GIMP, gcc, Vim, ...).
- **Single compilation unit** (only one .c file to compile, very fast and simple).
- Uses a **custom-made 256** color palette (but can run on platforms with fever colors, even just two).
- **Well documented and commented code**, written with tinkering and remixing in mind.

early previews:

Pokitto:

![](https://talk.pokitto.com/uploads/default/original/2X/2/29c0e4f44688f801013ed5b8463e97772d4b203e.gif)

SDL:

![](https://talk.pokitto.com/uploads/default/original/2X/e/e69a5e26aee3bd726494e793770911ab83345187.png)

## usage rights

**tl;dr: CC0 + waiver of all rights, completely public domain as much as humanly possible, do absolutely anything you want**

This work's goal is to never be encumbered by any exclusive intellectual property rights, it is intended to always stay completely and forever in the public domain, available for any use whatsoever.

I therefore release everything in this repository under CC0 1.0 (public domain, https://creativecommons.org/publicdomain/zero/1.0/) + a waiver of all other IP rights (including patents), which is as follows:

*Each contributor to this work agrees that they waive any exclusive rights, including but not limited to copyright, patents, trademark, trade dress, industrial design, plant varieties and trade secrets, to any and all ideas, concepts, processes, discoveries, improvements and inventions conceived, discovered, made, designed, researched or developed by the contributor either solely or jointly with others, which relate to this work or result from this work. Should any waiver of such right be judged legally invalid or ineffective under applicable law, the contributor hereby grants to each affected person a royalty-free, non transferable, non sublicensable, non exclusive, irrevocable and unconditional license to this right.*

Everything in this repository, including the code and assets, has been made completely by myself, Miloslav Ciz aka drummyfish, from scratch. I have **not** even reused any public domain material, as IP laws change and what has once been public domain may not be so in the future, so the only safe way for me to not infect this work with a potential disease I had to create it all myself.

Please support free software and free culture by using free licenses and/or waivers.

If you'd like to support me or just read something about me and my projects, visit my site: [www.tastyfish.cz](http://www.tastyfish.cz/).
