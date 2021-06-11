Steps to build from git checkout
================================

1. Open this folder as a project in STM32CubeIDE
2. Right-click the project in the project tree, press "Refresh"
3. Right-click the project in the project tree, press "Build Project"

This will build the project without re-generating TouchGFX sources.

Steps to re-generate TouchGFX sources
=====================================

1. Open TouchGFX/VPO-display.touchgfx in touchGFX designer
2. Press "Generate" (top right). The TouchGFX designer will re-generate all sources.
3. Check that all sources are added to the git repo EXCEPT the Middleware folder (that folder is useless, but TouchGFX
   insists on generating it anyway)
4. Open the STM32CubeIDE project and "refresh" it (right click on project name in project browser, then "Refresh")

Steps to flash
==============

1. Open STM32CubeProgrammer
2. Go to "EL" external loader tab (chip symbol on bottom left)
3. Check the "select" tick on the "N25Q128A_STM32469I-DISCO" / "STM32469I-DISCO"
4. Go back to "Erasing & Programming" tab (download symbol on top left)
5. As "File Path", select display-fw/Debug/VPO-Display.elf (for debug build, for release select .../Release/...)
6. Press green "Connect" button (top right)
7. Press "Start programming button" (middle, below "File path" field in "Download" section)