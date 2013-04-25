Getting started with git
========================

.. image:: ../images/git_logo.png
   :width: 300 px
   :alt: git logo
   :align: center


Introduction and Installation
-----------------------------

The google test C++ testing framework (gtest for short) is a framework
for unit testing in C++. In previous years it has been used for unit
testing in ELEN3009 (Software Development 2). This is a guide on how
to set it up in codeblocks for APPM3021. I tested this on the latest
codeblock (12.11). The guide assumes you have a
certain file structure::

    noiziv
      ├── config
      ├── doc
      ├── lib
      ├── log
      ├── src
      └── tests

Hopefully the names of the files make sense and I don't need to
explain them. Gtest can be downloaded from their `google code page`_.
It is a zip archive `(gtest-1.6.zip)`. You unzip it, rename the
resulting folder `gtest-1.6` as `gtest` and drop it in `lib` in the
above file structure.


Configuration
-------------

The configurations can be done in two ways. Globally and per project.
I will be showing the per project method here.

Step 1 - Create an empty project
++++++++++++++++++++++++++++++++

If you have an existing project, I recommend you create a new project
anyway. This is to show you how the whole setup is done then you can
apply it to your existing project. It also makes it easier for you to
follow this guide.
The project is a console application,

.. image:: screenshots/
   :width: 300 px
   :alt: git logo
   :align: center



Gtest primer
------------


Further reading
+++++++++++++++

If you are not satified with what I presented above (I wonder why)
then you can read some more things:

#. `Github's intro <http://learn.github.com/p/intro.html>`_
#. `Git for computer scientists <http://eagain.net/articles/git-for-computer-scientists/>`_
#. `Pro git book <http://git-scm.com/book>`_
#. `Ten tutorials for beginners <http://sixrevisions.com/resources/git-tutorials-beginners>`_
