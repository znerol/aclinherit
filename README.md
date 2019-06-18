ACL Inherit
===========

Inherit ACL from enclosing directory. Existing entries are replaced.

This command is designed to fix missing ACLs caused by misbehaving macOS AFP
clients when copying files within network volumes.

Requirements
------------

macOS with ACL enabled volume.

Usage
-----

```
Usage: aclinherit FILE...
```

Example
-------

Find all files with missig ACLs and restore them from the parent directory.

```
find /path/to/shared/volume -type f -not -acl -print0 | xargs -0 aclinherit
```

Compiling
---------

```
make
```

Installation
------------

```
sudo make install
```

License
-------

MIT
