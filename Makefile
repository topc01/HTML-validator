code: main.cpp source/node.cpp source/stack.cpp
	cd build && make && cd ..
	cp build/code validadorHTML

run:
	./validadorHTML
	
mk:
	cd build && cmake .. && cd ..
