#/bin/sh
# Applies mods to Anarch to create a consoomer edition more suitable for a
# mainstream gaymer. Run from the root directory of the repository.
# by drummyfish

echo "making consoomer edition"

git apply mods/hd/hd.diff
git apply mods/crt.diff
git apply mods/movement_inertia.diff
git apply mods/full_zbuffer.diff
git apply mods/demo.diff
git apply mods/terminal_messages.diff

sed -i 's/\(SFG_VERSION_STRING ".*\)"/\1 consoomer"/g' texts.h
sed -i 's/SFG_PLAYER_DAMAGE_MULTIPLIER 512/SFG_PLAYER_DAMAGE_MULTIPLIER 300/g' settings.h # make the game easier

echo "DONE"
