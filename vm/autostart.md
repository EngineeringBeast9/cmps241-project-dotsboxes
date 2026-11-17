# Sprint 1 verification in Alpine

Run this from the project folder in the installed Alpine VM. Install prerequisites as root with `apk add build-base gdb valgrind git`.

```sh
sh vm/verify-sprint1.sh
```

The script records the commit and build, board tests, a complete console match, GDB and Valgrind output in a new folder under `Testing/evidence/`. Review the logs before reporting results. Earlier sessions under `docs/` are historical evidence.

After the checks pass, use a root shell in the VM to install the game and back up the console configuration:

```sh
install -m 0755 ./dots_and_boxes /usr/local/bin/dots_and_boxes
cp -n /etc/inittab /etc/inittab.pre-dots
```

Edit `/etc/inittab`. Replace only the existing `tty1::respawn:` getty line with:

```text
tty1::respawn:/usr/local/bin/dots_and_boxes
```

Keep the login on tty2 for maintenance. Reboot, observe the game menu before typing a username or password, and capture the VMware console. Start a local game and confirm that a box capture awards a point and retains the player's turn. Record the result and the revision with the screenshot. To restore the original configuration, sign in on tty2 and copy `/etc/inittab.pre-dots` back to `/etc/inittab`, then reboot.

Completion evidence is a passing run of the script and a verified reboot into the game. The script does not change the VM configuration.
