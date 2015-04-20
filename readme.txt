
Table of Contents
=================

1    Introduction
1.1    Changes in version 0.2.1
1.2    Changes in version 0.2
1.3    Requirements
1.4    Installation
1.5    Overview
2    Command reference
2.1    mkaefs
2.2    aefsck
2.3    mntaefs
2.4    umntaefs
2.5    aefsdump
2.6    aefsdmn and aefsparm
3    Known problems and limitations
3.1    General problems
3.2    Security problems
4    Building AEFS
5    Compiling and using the NFS server
6    License
7    Author


1 Introduction
==============

AEFS is a cryptographic file system for OS/2 and Unix-like operating
systems.  It allows transparent access to encrypted files stored on
any file system -- it's not necessary to dedicate disk partitions.
Several block ciphers are currently supported (Rijndael and Twofish).
Extended attributes are supported under OS/2.


1.1 Changes in version 0.2.1
----------------------------

This is a bugfix release.  The following bugs were fixed:

- A bug was fixed in the treatment of paths relative to the current
  directory (except the root directory) of the form X/filename, where
  X is a one-character file name.  Such paths would incorrectly refer
  to the current directory.  For example, touch X/bla would create a
  file called bla in the current directory instead of in X.  Thanks go
  to Max Alekseyev for discovering this bug.

- Various fixes in the NFS server, which now seems to be reasonably
  stable.

- AEFS now works properly on systems that don't support growing
  truncates (e.g., Linux on some file systems).  Because of this the
  csAllocated field has been removed from the on-disk file info
  structure.  The csAllocated field wasn't actually used by the file
  system anyway, but you may receive minor warnings (complaining about
  an inconsistency between the csAllocated field and the actual
  allocation of a file) when you run an older version of aefsck
  against a file system created or modified by the current version of
  AEFS.

Furthermore, there is a new utility aefsutil that allows access to
unmounted file systems (i.e., list directories, extract files, etc.).


1.2 Changes in version 0.2
--------------------------

There are a few improvements:

- AEFS now works under other (read: Unix-like) operating systems as
  well.  I've implemented an NFS server frontend to AEFS, so in
  principle any system that can mount NFS file systems can mount an
  AEFS file system.  It has been tested on Linux and IRIX, and it at
  least compiles on FreeBSD and Solaris.

- Autoconf configuration.

- Utilities like aefsck should produce better error messages in case
  of a low-level (corefs) error.

- Rijndael is the default cipher (instead of Twofish) now that it has
  been selected as the AES.

- Syslog support.  The OS/2 daemon now logs to syslog instead of to a
  file.

- Performance improvements in the OS/2 daemon with regard to directory
  handling.

- Rijndael and Twofish now work on big-endian systems.

- Storage files are now locked when they are being accessed.  Notably,
  the superblock is locked for the entire time that the file system is
  being used.  This prevents e.g.  aefsck from messing with a file
  system that is currently mounted.

I fixed the following bugs:

- The FSD didn't correctly deal with interrupted client processes.  If
  a process waiting for the FSD to complete a request would receive a
  signal (e.g.  break or kill), then the daemon might corrupt the next
  request or even crash.

- Seeks relative to the end of the file didn't work properly because
  the FS_WRITE operation forgot to update the size field of the
  kernel's sffsi data structure.  This caused e.g.  gnutar to fail.

- Setting file info on an open file (DosSetFileInfo) would reset the
  file position to 0.


1.3 Requirements
----------------

This software requires the EMX runtime.  It can be downloaded from:
ftp://ftp.leo.org/pub/comp/os/os2/leo/gnu/emx+gcc/emxrt.zip.


1.4 Installation
----------------

Unzip the zip-file to any place on your hard disk.  You only need the
files in the bin subdirectory; all other files are source code and may
be deleted.  Then add the following lines to CONFIG.SYS:

      IFS=<bin-path>\stubfsd.ifs AEFS
      RUN=<bin-path>\aefsdmn.exe

where <bin-path> is the path of the bin subdirectory.  The first line
loads the FSD (file system driver), which is really a stub that passes
requests to the daemon, which is started in the second line and does
the real work.  You may also want to add <bin-path> to the PATH
environment variable, but that's merely a convenience.  After making
these changes, reboot the system.


1.5 Overview
------------

To create an AEFS volume using the default block cipher algorithm
(Rijndael, with a 128-bit key size), type

      mkaefs <path>

which will create a volume in <path> (that is, the ciphertext will be
stored in <path>).  It will ask you to (twice) enter the encryption
key to be used for this volume.  Don't forget the key!  The key should
be sufficiently hard to guess and may be of any length.  (The key you
enter is not actually the encryption key: it is hashed using SHA-1
into a key with the length expected by the block cipher).

For example,

      mkaefs c:\crypto

will make a volume in c:\crypto.

To access the volume, you must attach (`mount') it to a drive letter.
This can be done by typing

      mntaefs <drive-letter>: <path>

The program will then ask you to enter the encryption key.

For example,

      mntaefs x: c:\crypto

will attach the volume created in the previous example to the drive
letter X.  You can now access the encrypted data in c:\crypto trough
drive X.  Any data you write to or read from drive X will be encrypted
and decrypted transparently by AEFS.

When you are done using the encrypted volume, you can type

      umntaefs <drive-letter>:

to detach it.


2 Command reference
===================

All programs use the GNU getopt syntax.  They all support the
following options:

--help  Print help and exit.

--version
        Print version information and exit

For all programs that allow you to enter a key, you may set the
environment variable AEFS_ECHO to 1 to have them echo the key you type
to the screen; otherwise, user input it is not echoed.


2.1 mkaefs
----------

The mkaefs program creates an AEFS file system.  The syntax is:

mkaefs <option>* <path>

The options are:

--force
        Force creation of the file system even if <path> already
        exists.

--key <key>, -k <key>
        Use the specified key.  If the key is not specified, you will
        be asked to type it in.

--cipher <cipher>, -c <cipher>
        Use the specified block cipher algorithm.  This is a
        specification of the form
        <algorithm>[-<keysize>[-<blocksize>]] (if you omit the sizes,
        default values will be used); for example rijndael-128-128
        (which happens to be the default).  The following algorithms
        are currently available:

        rijndael
                Rijndael is the (proposed) Advanced Encryption
                Standard of the US National Institute of Standards and
                Technology -- the successor to DES.  It has a 128 bits
                block size and supports key sizes of 128, 196, and 256
                bits.  It's slightly faster (in this implementation)
                than Twofish when a 128 bits key is used, but it's
                slower for larger key sizes (because additional rounds
                are added).  See http://csrc.nist.gov/encryption/aes/
                for more information.

        twofish
                Twofish is a very fast block cipher.  Its block size
                is 128 bits.  Its supported key sizes are 128, 196 and
                256 bits.  Twofish was a candidate for the Advanced
                Encryption Standard (AES) which made it to the final
                round.  See http://www.counterpane.com/twofish.html
                for more information.

        none    This is the identity block cipher.  That is, the
                output of the cipher is equal to the input.  This
                should only be used for debugging.

        Run mkaefs with the --help flag to see a list of allowed block
        cipher specifications.

--no-cbc
        Do not use Cipher Block Chaining (CBC) mode (i.e.  use
        Electronic Codebook (ECB) mode).  By default, CBC is used.
        This is only useful for debugging (in conjunction with
        --cipher none).


2.2 aefsck
----------

The aefsck program checks an AEFS file system for errors and
optionally repairs them.  The syntax is:

aefsck <option>* <path>

The options are:

--fix, -f
        Fix errors.  Without this flag, aefsck only checks for and
        reports errors.

--force-fix
        Fix unreadable superblocks.  Without this flag, aefsck will
        immediately stop fixing the file system if it sees that the
        superblock cannot be decrypted (that is, if it has a bad
        checksum).  This usually indicates that you specified an
        invalid key.  Proceeding with an invalid key will do great
        damage to the file system, because aefsck will read much bogus
        information and then `fix' it.  If the superblock really is
        damaged, you can specify this flag, but make sure that you
        enter the right key.  Make a backup first.  aefsck will ask
        for confirmation before proceeding.

--key <key>, -k <key>
        Use the specified key.  If the key is not specified, you will
        be asked to type it in.

--scan, -s
        Perform a `surface scan'.  This means that aefsck will test
        the decryptability of every sector of every file in the file
        system.  If --fix is specified, it will rewrite sectors with
        wrong checksums.

--quiet, -q
        Run silently; only report errors and the actions subsequently
        taken.  If this flag is not given, then aefsck will report the
        several phases that it goes through.

WARNING: AEFS has not been tested widely.  Since aefsck traverses the
entire file system, if it has a bug, it might end up trashing the
entire file system.  So you may want to make a backup first.  At the
very least you should make a backup of the file `ffffffff.enc'; that's
the file that contains info about all the file objects in the file
system (in Unix terminology, it's the `i-node' table).


2.3 mntaefs
-----------

The mntaefs program attaches an AEFS file system to a drive letter.
The syntax is:

mntaefs <option>* <drive-letter>: <path>

The error code 25 is returned as a general indicator that an I/O error
occured.

The options are:

--force, -f
        Attach the file system even if its dirty flag is set.  The
        fact that it's dirty means that it wasn't properly detached
        the last time you used it, so it may have errors.

--key <key>, -k <key>
        Use the specified key.  If the key is not specified, you will
        be asked to type it in.

--nocheck, -n
        Do not start aefsck to check the file system if it is dirty.
        Without this flag, when the file system is dirty, mntaefs will
        start aefsck to fix possible errors and clear the dirty flag
        (unless --readonly is specified).  If aefsck indicates that
        there were no errors or that all errors were fixed, mntaefs
        will again attempt to attach the file system.

--readonly, -r
        Attach the file system read-only.  Note that the file system
        is always attached read-only if the read-only flag is set on
        the superblock file SUPERBLK.2.

  

2.4 umntaefs
------------

The umntaefs program detaches an AEFS file system from a drive letter.
The syntax is:

umntaefs <option>* <drive-letter>:

The options are:

--force, -f
        Detach the file system even if there are open files, active
        searches, or current directories for this drive.  This might
        not be entirely safe.


2.5 aefsdump
------------

The aefsdump program decrypts one or more encrypted storage files of
an AEFS file system and writes the decrypted data to standard output.
The syntax is:

aefsdump <option>* <file>*

<file> may be - to specify standard input.  NOTE: in that case, you
must use --key, because otherwise the key will also be read from
standard input.

The options are:

--key <key>, -k <key>
        Use the specified key.  If the key is not specified, you will
        be asked to type it in.

--cipher <cipher>, -c <cipher>
        Use the specified block cipher algorithm.  This should of
        course be the same as the cipher specified to mkaefs when the
        file system was created.  See the documentation for mkaefs for
        more information.

--no-cbc
        Do not use CBC mode.  This should only be used if this flag
        was also specified to mkaefs when the file system was created.


2.6 aefsdmn and aefsparm
------------------------

The aefsdmn program is the file system daemon.  The syntax is:

aefsdmn <option>*

It is typically started in CONFIG.SYS by a RUN statement (NOT a CALL
statement) or from a script using the detach command.

You can change the options of a running daemon with aefsparm.  The
syntax is the same.  The parameters given to aefsparm are sent
verbatim to the daemon.  Error messages are send to the syslog service
(if you have it running).

The options are:

--debug
        Write debug messages to syslog.  This makes the daemon slow.

--cache <n>
        Set the maximum size of the cache per volume in 512-byte
        units.  The default is 4096, or 2 megabytes per volume.

--storagefiles <n>
        Set the maximum number of open storage files per volume.  To
        speed up access to the encrypted files AEFS keeps the most
        recently used ones open.  The default is 16.  This number
        should not be made too high; otherwise the daemon might run
        into the open file limit of the C runtime library (use
        emxbind -a aefsdmn.exe -h<limit> to change this).

--files <n>
        Set the maximum number of files for which AEFS may cache
        sectors.  Every file in the cache has a slight overhead; hence
        the limit.  The default is 4096 files.

--lastaccess=[active|lazy]
        Specify whether the last-accessed fields of files should be
        updated always (`active') or only if other fields have changed
        as well (`lazy').  The default is `active'.

--lazy=[on|off]
        Specify whether lazy-writing (or `write-behind') should be
        enabled.  The default is `on'.  If lazy-writing is enabled,
        AEFS will flush every volume every 5 seconds.  This interval
        cannot currently be changed.

--quit  Kill the daemon.  Useful for debugging.  You should detach all
        attached volumes first.

--info  Print statistics about the memory allocation of the daemon and
        about every attached volume.  These appear in the log file.

Note: most options do not take effect for volumes that are already
attached, only for volumes attached after aefsparm is run.


3 Known problems and limitations
================================


3.1 General problems
--------------------

- It is not possible to mount an AEFS volume stored on an AEFS volume.
  This is because the daemon can handle only one request at a time,
  and a deadlock would occur if the daemon tried to access an AEFS
  volume.  However, this deadlock condition IS detected and prevented.

- The scheme for flushing out dirty sectors is rather bad.  If the
  cache is full and space is needed, all dirty sectors are flushed to
  disk at once.  This can cause high delay (due to the
  one-request-at-a-time nature of the daemon), especially if the
  ciphertext is stored on a slow medium such as a network drive.

  In fact, until somebody implements a better write-behind scheme, you
  might be better off with lazy-writing turned off (the --lazy=off
  option of aefsdmn).

- AEFS doesn't handle system shutdown very well (actually, this an
  OS/2 problem: at the time the kernel tells AEFS to flush all data,
  it will no longer pass any read or write operations to any file
  system; so we can't write dirty data to the underlying file
  systems).  If there still is dirty data in the cache when the system
  is shut down, it may not be committed to disk, causing data loss
  (run aefsck when this happens).  The surest way to prevent this is
  to detach (umntaefs) each volume before shutting down the system.

- The file `ffffffff.enc' (the `i-node' table) grows if necessary, but
  it never shrinks.  It would be nice if aefsck had an option to do
  that.


3.2 Security problems
---------------------

- The most obvious exposure is the size of the files on the volume,
  since encryption happens on a per file basis.

- Data is encrypted in independent blocks (`sectors') of 512 bytes.
  Thus, it is possible for somebody with access to the ciphertext to
  gain information about the volume's modification patterns by
  monitoring which files and which sectors are being changed.

  Also, a last-accessed field is maintained for every file (just like
  HPFS does).  This can give an attacker information about the
  volume's access patterns (because the sector containing the field
  changed).  Start aefsdmn with the option --lastaccess=lazy to have
  it update the last-accessed field only if other fields have changed
  as well.

  On the other hand, since AEFS encrypts in Cipher Block Chaining
  (CBC) mode with a pseudo-random Initialization Vector, it is not
  possible for an attacker to see WHICH bytes in a sector were
  changed.

- Sensitive data may be swapped out to the swap file.  The daemon
  should lock all buffers containing sensitive data in physical
  memory.  OS/2 applications cannot actually do this, but it should be
  possible with a little help from the FSD.  However, the fact that
  any data may be leaked to the swap file is a fundamental limitation
  of a non-secure operating system like OS/2.  The only protection is
  to completely disable swapping (`MEMMAN=NOSWAP' in CONFIG.SYS).

- The Workplace Shell stores lots of information about the files and
  directories it encounters in OS2.INI and OS2SYS.INI.  For example,
  it tends to assign a handle to every file it sees and store it in
  the HANDLES entry of OS2SYS.INI.  You can prevent this by not using
  the WPS to access the encrypted drive.

- Other programs are similarly indiscreet with data.  For example.
  many applications maintain a history of most recently opened files,
  or write possibly sensitive data to temporary directories.

- ``It doesn't matter what the algorithms are or how large the keys
  they use; user-remembered secrets are not secure.'' (Bruce Schneier,
  CRYPTO-GRAM, October 15, 1999).  AEFS relies on user-remembered keys
  to protect the data: this places a limit on the security of the
  system.

  AEFS should have an option to generate a (cryptographically) strong
  pseudo-random key to be encrypted by a user-entered pass phrase
  (like PGP does).  That way, only the file containing the encrypted
  key is vulnerable to a key guessing attack.

- AEFS should support a slow-but-reliable cipher like 3DES; Twofish
  and Rijndael are too new for one to be really sure of their security
  (although they have had some public scrutiny as part of the AES
  selection process).


4 Building AEFS
===============

The makefiles require GNU make.

To build the FSD (stubfsd.ifs) you need Microsoft C 6.00.  This
compiler can be downloaded from the IBM Developer Connection Device
Driver Kit for OS/2 site at http://service.boulder.ibm.com/ddk.  You
also need the FSD helper library and header file, which is included in
the FSD toolkit:
ftp://ftp.leo.org/pub/comp/os/os2/leo/devtools/doc/ifsinf.zip.

To build the daemon and the support programs you need gcc or pgcc,
available from various locations.

To regenerate the RPC code for the NFS server, you need a rpcgen that
supports the -C flag to generate ANSI C header files.  I used the one
from FreeBSD, which compiles trivially under OS/2.


5 Compiling and using the NFS server
====================================

You can now use AEFS under different operating systems than OS/2.  The
program aefsnfsd acts as an NFS frontend to your AEFS file systems.
I'm too lazy to write proper documentation right now, but here is a
summary of how you can get it to work.

Compile AEFS along the usual lines (i.e.
./configure; make; make install).

Then, as root, start the server:

      $ aefsdmn 

The daemon should disappear into to background.  Error messages should
appear in your system log.  Specify -d to get debug output on stdout
in case something goes wrong.  The daemon listens to TCP and UDP
requests on the loopback interface on port 843, and not port 2049,
which is the usual port for NFS.  This is because 2049 is not a
privileged port, so any user could bind to it and intercept your keys,
and because this way the daemon doesn't interfere with your regular
NFS server.  It doesn't register itself with portmap unless you
specify the -r flag.  The daemon can be cleanly shut down by sending
it a TERM or INT signal.

Since the NFS protocol doesn't know anything about encryption keys,
another protocol is used to give those keys to the daemon.  This is
what the aefsadd program does.  For example, to tell it the key for
the AEFS file system stored in /crypted/foo, do:

      $ aefsadd /crypted/foo
      key: blah blah... 

See aefsadd --help for more details.

Now that the daemon knows the key, you can mount the file system:

      $ mount -t nfs -o port=843,mountport=843 localhost:/crypted/foo /mnt/foo

which will make the decrypted file system appear on /mnt/foo.  The
options to specify the server port are for Linux; they might be
different for other systems.  In fact, IRIX 5.3 doesn't allow this at
all; in such a case recompile with the port number set to 2049 (see
nfsd/aefsctrl.h).

File names in the same directory that differ only in case are not
allowed; must fix this.

Symbolic links are fully supported.  Hard links work in principle, but
are currently disabled in the NFS server due to bogosity issues with
the readdir call (that's the part of NFSv2 that's most broken).

Unix permissions (UID, GID, etc.) are fully supported.  You cannot
make device nodes, fifos, or sockets on an AEFS volume.

Make sure that NFS-mounted file systems are unmounted BEFORE the
daemon is killed in your shutdown sequence.  Otherwise, the shutdown
may hang (this is because the client might retry requests
indefinitely; to prevent this, do a soft mount, -o soft).


6 License
=========

AEFS is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

AEFS is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with AEFS; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.

The Twofish and Rijndael code was written by Dr Brian Gladman
(gladman@seven77.demon.co.uk) who gave it the following license:

    Copyright in this implementation is held by Dr B R Gladman but I
    hereby give permission for its free direct or derivative use
    subject to acknowledgment of its origin and compliance with any
    conditions that the originators of the algorithm place on its
    exploitation.

The use or export of this software may be restricted by law.  Before
you do either make sure that you know what is allowed in your country.


7 Author
========

This software was written by Eelco Dolstra.  Send bug reports,
patches, or other comments to <eelco@cs.uu.nl>.

$Id: readme.src,v 1.9 2001/09/23 11:46:04 eelco Exp $
