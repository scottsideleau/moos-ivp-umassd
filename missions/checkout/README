SAMPLE MISSION AND BEHAVIOR FILES FOR UMASSD OPERATIONS
by Scott R. Sideleau <ssideleau@umassd.edu>
   22-DEC-2015

Special thanks to MIT's Laboratory for Marine Sensing Systems (LAMSS) for
pioneering the initial nsplug-based missions directory, missions-lamss, which
allowed me to reverse engineer the automation, clean up the approach, and 
release something easily modifiable and extendable to other IVER users.

Due to the nature of the shell scripting involved, the target autonomy payload
computer should be running Linux, but a savvy end user could re-engineer the 
scripts to be compatible with a Windows-based payload.


STATIC VS. GENERATED

For those users running a non-Linux based autonomy payload computer, please
see the contents of the 'static' directory for stand-alone .moos and .bhv 
files that can be easily modified and/or extended for your own use.  For your
convenience, I have included separate simulation and runtime .moos files to
facilitate the use of the existing MOOS-IvP simulation environment before you
jump to commanding the IVER vehicle via iOceanServerComms.

For those users running Linux that are interested in having more control over
their .moos and .bhv files and who are interested in facilitating 
configuration reusability across experiments, read on.


QUICK OVERVIEW OF THE MISSION AND BEHAVIOR FILE GENERATION

The following is a quick overview of the mission and behavior file generation
process.  By running 'simulation_launch.sh' or 'runtime_launch.sh', you are 
choosing to run a simulation or control an actual IVER, respectively.  By 
default, each runtype has been customized to show or hide the xterm "pop-up" 
windows associated with the running processes.  As such, only the
'simulation_launch.sh' script will permit the xterm windows of the running 
processes to be displayed, as this is not desired functionality on a headless
autonomous vehicle.

When you run either of the two scripts, the following actions are taken...

1. Any previous version of last.moos is removed.
2. A new last.moos (simulation or runtime) is generated from 'all.def', 
   'moos.meta', and the necessary files from the 'plugs/' directory.
3. Any previous version of current.bhv is removed.
4. A new current.bhv (simulation or runtime) is generated from 'all.def',
   'bhv.meta', and the necessary files from the 'plugs/' directory.
5. The new last.moos file is launched with pAntler, piping the output to
   /dev/null so that your terminal remains usable.

The .meta files simply #include the necessary files from the 'plugs/'
directory in the proper order, wrapped in any necessary logic, to facilitate
the generation of clean files.  Each process or behavior configuration block
found in the 'static' files lies in its own file in plugs/ along with a few
convenience plugs (e.g. data_datum.plug, data_errata.plug, etc).


EXTENDING THE SAMPLE MISSION AND BEHAVIOR FILES

Should you want to make additions or modifications to the mission or behavior
file, simply...

1. Add additional .plug files to plugs/ following the existing naming
   convention (app_* for MOOS blocks, bhv_* for behavior blocks, etc).
2. Add necessary #includes to the .meta files.
3. Add any variable declarations used in your plugs to the all.def file.
4. Wrap your additions in #ifdef logic if it should only happen during
   'simulation' or 'runtime', the two runtypes supported.

Then, simply run the appropriate launch script and, barring no syntax errors, 
you should see your new mission being executed.

