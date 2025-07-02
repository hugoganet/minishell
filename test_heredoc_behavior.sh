#!/bin/bash

export TEST="hello world"
export USER="testuser"

echo "=== COMPORTEMENT BASH DES HEREDOCS ==="
echo

echo "1. <<EOF (pas de quotes) - EXPANSION ACTIVÉE"
cat <<EOF
Variable TEST: $TEST
Variable USER: $USER
Escaped dollar: \$TEST
EOF

echo
echo "2. <<'EOF' (quotes simples) - EXPANSION DÉSACTIVÉE"
cat <<'EOF'
Variable TEST: $TEST
Variable USER: $USER
Escaped dollar: \$TEST
EOF

echo
echo "3. <<\"EOF\" (quotes doubles) - EXPANSION DÉSACTIVÉE"
cat <<"EOF"
Variable TEST: $TEST
Variable USER: $USER
Escaped dollar: \$TEST
EOF

echo
echo "4. Test avec backslash"
cat <<EOF
Backslash simple: \$TEST
Double backslash: \\
Backslash + n: \n
Tab: \t
EOF

echo
echo "5. Test avec caractères spéciaux"
cat <<EOF
Dollar seul: $
Question mark: $?
Accolades: ${TEST}
Paramètre positionnel: $1
EOF
