#include <cstdlib>
#include <ctime>
#include <tuple>

#ifndef EM_PORT_API
#	if defined(__EMSCRIPTEN__)
#		include <emscripten.h>
#		if defined(__cplusplus)
#			define EM_PORT_API(rettype) extern "C" rettype EMSCRIPTEN_KEEPALIVE
#		else
#			define EM_PORT_API(rettype) rettype EMSCRIPTEN_KEEPALIVE
#		endif
#	else
#		if defined(__cplusplus)
#			define EM_PORT_API(rettype) extern "C" rettype
#		else
#			define EM_PORT_API(rettype) rettype
#		endif
#	endif
#endif

EM_PORT_API(int) getNowTime() {
	return static_cast<int>(std::time(nullptr));
}

EM_PORT_API(int) modifyTime(int* tm) {
	std::tm t;
	std::tie(t.tm_year, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec) = 
		std::make_tuple(tm[0] - 1900, tm[1] - 1, tm[2], tm[3], tm[4], tm[5]);
	return std::mktime(&t);
}

EM_PORT_API(int*) diffTime(int ed, int st) {
	int* ret = (int*) std::malloc(4 * sizeof(int));
	if (ret == NULL) 
		return NULL;

	time_t diff = std::difftime(st, ed);

	diff -= (ret[0] = diff / (24 * 60 * 60)) * (24 * 60 * 60);
	diff -= (ret[1] = diff / (60 * 60)) * (60 * 60);
	diff -= (ret[2] = diff / 60) * 60;
	ret[3] = diff;

	return ret;
}

EM_PORT_API(void) freeBuf(void* buf) {
	std::free(buf);
}

#ifndef __EMSCRIPTEN__
#include <bits/stdc++.h>

int main() {
	static int a[] = { 2022, 12, 15, 14, 38, 0 };
	std::cout << modifyTime(a) << '\n';
	return 0;
}
#endif