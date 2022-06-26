# Violent Fungus C2

I’m building a command and control (C2) program suite to learn C2 software design and implementation at a fine-grained level (using MITRE ATT&CK C2 Techniques https://attack.mitre.org/tactics/TA0011/ and existing C2 software as reference).

Additionally, I want to get proficient coding Qt C++ targeting multiple platforms. I’m aiming for a 1.0 release in 2022.

Website for this project is at https://sogonsecurity.com/violentfungus-c2-project/.

## Core Goals
* Small binary size.
* Lean on resources.
* Able to run on smaller ARM boards with 512MB of RAM and potentially embedded devices.
* Handle a large number of active agents.
* Use Qt C++ as much as possible/appropriate.
* Target GNU/Linux, NetBSD 9+, OpenBSD 6.9+, FreeBSD 13+, Microsoft Windows 10+ and Server 2016+, and Apple macOS 10.5+.
  * Architectures should be somewhat normal including x86, x86-64, ARM32, and ARM64 in particular.

## Core Features
* **Support a large number of server services**: HTTPS, DNS over UDP, FTP, SMB, ICMP, SMTP, raw TCP, and raw UDP, etc., shooting for more than anyone else.
* **Multi-user server and agents**: multiple users can use the server and communicate with the same agents at the same time.
* **Plugin support**: Unsure the best way to implement this yet but would be nice if Python plugins were possible as the very least. Perhaps using Qt's signals/slots mechanism?!
* Agent...
  * **relaying**: relay communications to the server via other agents and to agents via other agents (return path doesn’t have to be the same).
  * **time/day schedules**: only operate on certain days and times, such as work hours.
  * **queues**: queue multiple commands/requests.
  * **multiple server service communications**: optionally communicate over multiple server services if available (such as multiple DNS queries and then SMTP) within the same session.
  * **communication playbooks**: predefined communication ordering to emulate normal traffic.
* **Asymmetric and symmetric encryption**: rotating keys, key expiration, and using existing industry standards of AES and DSA, for example. Perhaps ChaCha on ARM boards and older processors that don’t have AES instructions available?
* **Decoupled transmission, data chunking, and data modification**: allowing for independence at each level, one-to-many relationships, and independent plugin support.
* **Server API**: frontends can use the remote server API, allowing for a decoupled experience where user interfaces can be remote and in whatever format is needed.
