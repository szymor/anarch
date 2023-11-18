#!/bin/sh
# Applies mods to Anarch to create a consoomer edition more suitable for a
# mainstream gaymer. Run from the root directory of the repository (NOT the
# mod directory).
# by drummyfish

echo "making consoomer edition"

for f in "full_zbuffer" "sprite_flipper" "weapon_swing" "particle_effects" "quicksave" "terminal_messages" "hd/hd" "crt" "movement_inertia" 
do
  echo "=== APPLYING $f ==="
  patch --ignore-whitespace -p1 < mods/$f.diff
  echo $?
done

sed -i 's/\(SFG_VERSION_STRING ".*\)"/\1 consoomer"/g' texts.h
sed -i 's/SFG_PLAYER_DAMAGE_MULTIPLIER 512/SFG_PLAYER_DAMAGE_MULTIPLIER 300/g' settings.h # make the game easier for normies

echo "DONE"
