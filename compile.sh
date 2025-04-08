# #!/bin/bash

# set -e

# gcc -c -Iinclude -std=c99 -pedantic -ggdb3 -O3 -fsanitize=address -fno-omit-frame-pointer \
#   -Wall -Wextra -Werror -o src/ring_buffer.o src/ring_buffer.c

# gcc -c -Iinclude -Ilib/cmocka/include -std=c99 -pedantic -ggdb3 -O3 -fsanitize=address -fno-omit-frame-pointer \
#   -Wall -Wextra -Werror -o test/test_ring_buffer.o test/test_ring_buffer.c

# gcc -Llibexec -o bin/test_ring_buffer src/ring_buffer.o test/test_ring_buffer.o -lasan -lcmocka

# rm -rf *.o */*.o */**/*.o

#!/bin/bash

set -e

cmake --build build/

