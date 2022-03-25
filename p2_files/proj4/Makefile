.PHONY: app app_sandboxed clean

INTEGRATION_HEADER_PATH:=include/wasm_sandbox
RLBOX_HEADER_PATH:=include
CXXFLAGS:=-g -Wall
INCLUDE_FLAGS:=-I $(RLBOX_HEADER_PATH) -I $(INTEGRATION_HEADER_PATH)

app: app.cpp lib.h lib.c
	$(CXX) -std=c++17 $(CXXFLAGS) $(INCLUDE_FLAGS) lib.c app.cpp  -o $@

#Will not build until you sandbox the library
app_sandboxed: app.cpp lib.h
	$(CXX) -std=c++17 $(CXXFLAGS) $(INCLUDE_FLAGS) -pthread app.cpp -ldl -lrt -o $@

clean:
	-rm -f app
	-rm -f app_sandboxed
