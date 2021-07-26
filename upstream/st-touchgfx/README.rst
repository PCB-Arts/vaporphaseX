
ST TouchGFX Mirror
==================

This repository contains our copy of ST's TouchGFX touchscreen user interface framework for use in the Vapor Phase One
display controller firmware. This code is factored out for three reasons:

1. To keep the main VPO repo small
2. To keep the separately updated upstream TouchGFX code out of the VPO main repo
3. To alleviate licensing issues around TouchGFX's hostile licensing terms

AS-IS, THE LEGAL STATE OF THIS REPO HAS NOT BEEN CLEARED AND THUS THIS REPO SHOULD NOT BE PUBLISHED!

Repository Structure
--------------------

The master branch of this repo only contains the repo's infrastructure files. The actual touchGFX code is on the release
branch. To keep clones small, each TouchGFX version is branched anew from master. Each version is tagged ``v-**.**.**``.

Directory structure
-------------------

``upstream/``
  contains the content of ``en.x-cube-touchgfx-****.zip`` as downloaded from st.com minus unnecessary files as removed
  by ``cleanup_download.sh``

``cleanup_download.sh``
  removes unnecessary files from a touchGFX download to make this repo marginally less bloated

``version.txt``
  contains the version of that library

``licensing/``
  contains all terms&conditions and licensing documents that popped up while dowloading the library
