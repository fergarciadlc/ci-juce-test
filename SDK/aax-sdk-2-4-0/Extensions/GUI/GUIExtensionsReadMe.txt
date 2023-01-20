=============================================

AAX SDK GUI Extensions projects

=============================================

These projects demonstrate how to extend the AAX SDK's GUI programming
interface using a variety of popular GUI frameworks:

* Native Cocoa
* Native Win32
* VSTGUI
* JUCE

These projects do not represent core functionality of the AAX SDK, but rather
they serve as examples of how plug-in GUIs can be written to the AAX
specification using a variety of different approaches.

-------------
Project Notes
-------------

* The VST and JUCE GUI Extension library projects use a macro value to resolve 
file paths to the installed framework directory.  This macro is defined in a 
Visual Studio property sheet on Windows and as a custom project variable on Mac.  
Because this macro will not be resolved on Mac until compilation, the Xcode GUI 
will not be able to find the included files.  However, the projects should 
builds successfully once the macros are updated to point to the correct 
directory.

* The JUCE GUI Extension code in this SDK was written using version 1.51 of the
JUCE framework
