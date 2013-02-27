TMProLWPlugIns - TM Productions Lightwave Plug-ins and Programs - by Joe Angell
===============================================================================

Old (1999-2002) Lightwave 3D plug-ins by Joe Angell at http://www.tmproductions.com

Due to some requests to have 64 bit or otherwise updated versions of my various plug-ins, I've decided to
make the source available here on GitHub.  I'm providing the source as-is to whoever may find it useful.
The license is not restrictive (I'm thinking of one of the Creative Commons ones), but you are not allowed
to sell the plug-ins and programs found here -- they are intended to be freely available at no charge.

These are all pretty old.  They use C++ mechanisms that have been superseded by new specs, and may prove
difficult to compile.  Some plug-ins use other SDKs, notably the older Spaceball and Lightwave SDKs, and
are included here for completeness as they may not otherwise be available.  These were all built with
Microsoft Visual C 6.0, so if you can find a copy of that you may be able to successfully build them.
The plug-ins were built for 32-bit Intel x86 hardware running Windows, and sometimes DEC Alpha hardware
running Windows.

You can find some basic documentation for building these tools in the Docs directory.  In short, you need
to make sure the Lightwave SDKs are in your include and libs paths, that the Projects dir is also in your
include path, and that the Libs dir is in your libs path.  After that, everything should build with
Visual C 6.0.

I have no idea what is involved in building with later versions of the Lightwave SDK or with later versions
of Visual C and later C++ specs.  That is left as an exercise for the reader.

Good luck!

-- Joe
