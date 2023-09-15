if [[ $1 == make ]]
then
    clang -Wall -Werror -Wshadow -Wextra $2 -o $3
elif  [[ $1 == disasm ]]
then
    ndisasm $2
else
    rm -rf ans_*.* *.out
fi
