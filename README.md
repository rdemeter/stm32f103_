# Setting up a GCC/Eclipse toolchain for STM32

You can download the "Eclipse IDE for C/C++ Developers" release from this page:
https://www.eclipse.org/downloads/

The downloaded file is just a ZIP archive. You can expand it in the C:\STM32 folder.
When finished, you can launch the C:\STM32\eclipse\eclipse.exe executable.

The first time Eclipse starts, it will ask your for the default projects folder. I suggest to use the path C:\STM32\projects. When the IDE is started, go to Help->Install New Software....
Select the C/C++ Development Tools repository and check only the entry "CDT Main Feature".  Click on "Next" and follow the instructions.

# Install the GNU ARM plug-ins for Eclipse

To install GCC ARM plug-ins go to Help->Install New Software... and click on the "Add..." button. Fill the text fields in the following way:

- Name: GNU ARM Eclipse Plug-ins
- Location: http://gnuarmeclipse.sourceforge.net/updates

and click the "OK" button. After a while, the complete list of available plug-ins will be shown.

# Install the GCC ARM tool-chain

You can download a prebuild version for your operating system from this web site:
https://launchpad.net/gcc-arm-embedded

# Install the Build Tools

you can install the Build Tools packagefrom here:
http://sourceforge.net/projects/gnuarmeclipse/files/Miscellaneous/

# Install the ST Link drivers

