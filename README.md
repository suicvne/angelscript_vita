# angelscript_vita
A work-in-progress re-port of AngelScript 2.32.0 from the old "SN Systems ProDG" (Official Dev Kit) to the GCC-based unofficial VITASDK

## Building
Ensure you have VITASDK installed and configured correctly. You also need VITASDK in your path.
This version of AngelScript for PS Vita compiles as a static library. 

**PLEASE** use the CMake project files for ease of configuration and building.

1. Clone repo
2. cd into repo (`cd angelscript_vita`)
3. cd into library CMake Project folder (`cd sdk/angelscript/project/cmake`)
4. mkdir for build files (`mkdir build_vita && cd build_vita`)
5. CMake Configuration: `cmake -DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake [-DCMAKE_BUILD_TYPE=Debug|Release] ../`
6. Build: `make -j4 && make install`
7. ???
8. Test out some projects! Statically link `libangelscript.a`. The `game` sample is the only one that's been modified to build Vita VPK (does NOT display anything, just a small test for loading AngelScript files & testing multiple files, references, etc.), try that one out if you have debugnet installed & know how to use it. 

## Misc. Information

Ported to the unofficial PS Vita SDK by Axiom.
Axiom does NOT own AngelScript.

Modifications are documented when & where they can be!

AngelScript License:
```
   AngelCode Scripting Library
   Copyright (c) 2003-2014 Andreas Jonsson

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any
   damages arising from the use of this software.

   Permission is granted to anyone to use this software for any
   purpose, including commercial applications, and to alter it and
   redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you
      must not claim that you wrote the original software. If you use
      this software in a product, an acknowledgment in the product
      documentation would be appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and
      must not be misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
      distribution.

   The original version of this library can be located at:
   http://www.angelcode.com/angelscript/

   Andreas Jonsson
   andreas@angelcode.com
```
