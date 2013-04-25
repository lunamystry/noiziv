Getting started with git
========================

.. image:: ../images/git_logo.png
   :width: 300 px
   :alt: git logo
   :align: center


Introduction
------------

Git is probably the most popular version control system out there. It
was created by the great Linus Torvalds (guy who gave us Linux) and is
free and open source. This document will guide you through the
installation of git on Linux and Windows. It will also give
configuration and basically help you with getting started.


installation
------------

This section will give instructions on how to git install on both
Windows and Ubuntu. If you don't run either (read you run Apple) of
these you will have to find out for yourself.


Ubuntu
++++++

1. Launch a terminal (`Ctrl+t`)
2. run the install command ``sudo apt-get install git`` (easy No?)


Windows
+++++++

1. You first have to download it from `msysgit on github
   <http://msysgit.github.com>`_
2. Double click on the installer to install.

   * When prompted about line ending conversion, don't choose the last
     option. See image below

      .. image:: ../images/choose_window_to_unix.png
         :width: 300 px
         :alt: choosing window to unix line ending
         :align: center


Configuration
-------------

The simplest way I found to use on Windows was using Git bash which is
installed with git. This seems to give Linux bash type commands on
windows. This means then you can use the same instructions as the
Ubuntu. All the commands below must be run from either a terminal
(Ubuntu) or git bash (Windows).

#. Introduce yourself to git::

     git config user.name "Firstname Lastname"
     git config user.email "your@email.co.za"
     git config core.editor "your text editor" # not tested!

#. generate ssh keys (take note of where the keys are stored)
   `why passphrases matter
   <https://help.github.com/articles/working-with-ssh-key-passphrases>`_::

     ssh-keygen -t rsa -C "your@email.co.za"

#. Register on the Social coding platform, `Github <http://github.com>`_

#. Send your username to `Mandla <leonard.mbuli@students.wits.ac.za>`_
   get an invite to the noiziv repository.

#. Wait for Mandla to add you to repo authorized list (about a day)

#. Fork the repo
   `instructions on github
   <https://help.github.com/articles/fork-a-repo>`_
   The commands adapted for noiziv (change username to your username)::

       git clone https://github.com/username/noiziv.git /directory/where/you/want/code/stored
       # Clones your fork of the repository into the current directory in terminal
       cd /directory/where/you/want/code/stored
       git remote add upstream https://github.com/APPM3021/noiziv.git
       git fetch upstream
       git merge upstream/master


That should give you the latest code in the repository. The next
section gives a quick git primer and some everyday use.

Git primer
----------

Okay git is installed, how do you use it? First, lets talk about how
it all works and what some words mean.


How it works
++++++++++++

COMING SOON!


Usage
+++++

Before you hack away::

  git pull origin master
  git log  # see commit messages


After hacking away::

  git status # see what changed
  git add <files you changed> # avoid git add .
  git commit -m "Message of what you did"
  git push origin master


Further reading
+++++++++++++++

If you are not satified with what I presented above (I wonder why)
then you can read some more things:

#. `Github's intro <http://learn.github.com/p/intro.html>`_
#. `Git for computer scientists <http://eagain.net/articles/git-for-computer-scientists/>`_
#. `Pro git book <http://git-scm.com/book>`_
#. `Ten tutorials for beginners <http://sixrevisions.com/resources/git-tutorials-beginners>`_
