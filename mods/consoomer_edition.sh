#/bin/sh
# Applies mods to Anarch to create a consoomer edition more suitable for a
# mainstream gaymer. Run from the root directory of the repository (NOT the
# mod directory).
# by drummyfish

echo "making consoomer edition"

for f in "particle_effects" "quicksave" "terminal_messages" "hd/hd" "crt" "movement_inertia" "full_zbuffer" "weapon_swing"
do
  echo "=== APPLYING $f ==="
  git apply --ignore-space-change --ignore-whitespace mods/$f.diff 2>&1 >/dev/null
done

sed -i 's/\(SFG_VERSION_STRING ".*\)"/\1 consoomer"/g' texts.h
sed -i 's/SFG_PLAYER_DAMAGE_MULTIPLIER 512/SFG_PLAYER_DAMAGE_MULTIPLIER 300/g' settings.h # make the game easier for normies

echo "DONE"
