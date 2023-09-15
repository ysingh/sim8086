if [[ $1 == make ]]
then
    clang -Wall -Werror -Wshadow -Wextra $2 -o $1
elif  [[ $1 == disasm ]]
then
    ndisasm $2
else
    find . -type f ! -name "*.*" -delete
    rm -rf ans_*.*
fi
