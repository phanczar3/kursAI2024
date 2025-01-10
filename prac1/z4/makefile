CPP = g++		
Flags = -std=c++17 -Wall -Wextra -O3 -Wshadow -Wformat=2 -Wfloat-equal -Wshift-overflow -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector
%:	%.cpp		 
		$(CPP) $(Flags) -o $@ $<