#include <stdio.h>

/*
	FOR_EACH(ACTION, a, 11, b, 22, c, 33)
	(GET_MACRO(a, 11, b, 22, c, 33, FE_8, FE_7, FE_6, FE_5, FE_4, FE_3, FE_2, FE_1)) (ACTION, a, 11, b, 22, c, 33) {
		_1 = a
		_2 = 11
		_3 = b
		_4 = 22
		_5 = c
		_6 = 33
		_7 = FE_8
		_8 = FE_7
		MACRO_NAME = FE_6
	}
	FE_6(ACTION, a, 11, b, 22, c, 33)
	ACTION(a, 11)   ACTION(b, 22)   ACTION(c, 33)
*/
#define GET_MACRO(PREFIX, _1, _2, _3, _4, _5, _6, _7, _8, MACRO_NAME, ...) PREFIX##MACRO_NAME // returns the first parameter after the __VA_ARGS__
#define FOR_EACH(PREFIX, ACTION, ...) GET_MACRO(PREFIX, __VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)(ACTION, __VA_ARGS__)

// For Every 2 Parameters
#define FE2_2(ACTION, LVALUE, RVALUE     ) ACTION(LVALUE, RVALUE) 
#define FE2_4(ACTION, LVALUE, RVALUE, ...) ACTION(LVALUE, RVALUE) FE2_2(ACTION, __VA_ARGS__)
#define FE2_6(ACTION, LVALUE, RVALUE, ...) ACTION(LVALUE, RVALUE) FE2_4(ACTION, __VA_ARGS__)
#define FE2_8(ACTION, LVALUE, RVALUE, ...) ACTION(LVALUE, RVALUE) FE2_6(ACTION, __VA_ARGS__)

// For Each Numbered Parameter
#define FEN_1(ACTION, PARAM     ) ACTION(PARAM, 1) 
#define FEN_2(ACTION, PARAM, ...) ACTION(PARAM, 2) FEN_1(ACTION, __VA_ARGS__)
#define FEN_3(ACTION, PARAM, ...) ACTION(PARAM, 3) FEN_2(ACTION, __VA_ARGS__)
#define FEN_4(ACTION, PARAM, ...) ACTION(PARAM, 4) FEN_3(ACTION, __VA_ARGS__)

// For Every 2 Numbered Parameters
#define FEN2_2(ACTION, LVALUE, RVALUE     ) ACTION(LVALUE, 1) 
#define FEN2_4(ACTION, LVALUE, RVALUE, ...) ACTION(LVALUE, 2) FEN2_2(ACTION, __VA_ARGS__)
#define FEN2_6(ACTION, LVALUE, RVALUE, ...) ACTION(LVALUE, 3) FEN2_4(ACTION, __VA_ARGS__)
#define FEN2_8(ACTION, LVALUE, RVALUE, ...) ACTION(LVALUE, 4) FEN2_6(ACTION, __VA_ARGS__)


#define FE_BACKUP(LVALUE)           , backup_##LVALUE = LVALUE
#define FE_RESTORE(LVALUE)          , LVALUE = backup_##LVALUE

#define FE2_UPDATE(LVALUE, RVALUE)  , (LVALUE = RVALUE)

#define FEN2_BACKUP(LVALUE, NUMBER)  , backup_##NUMBER = LVALUE
#define FEN2_RESTORE(LVALUE, NUMBER) , LVALUE = backup_##NUMBER

// Accept all comma separated parameters (expressions) and always return true
inline bool FETrue(...) { return true; }

#define BACKUP(...) for (auto run = 1 FOR_EACH(FEN_, FEN2_BACKUP,  __VA_ARGS__);  \
                              run; \
                              run = 0 FOR_EACH(FEN_, FEN2_RESTORE, __VA_ARGS__))

#define RUN_WITH(...) for (auto run = 1         FOR_EACH(FEN2_, FEN2_BACKUP,  __VA_ARGS__);  \
	                            run && FETrue(0 FOR_EACH(FE2_,  FE2_UPDATE,   __VA_ARGS__)); \
	                            run = 0         FOR_EACH(FEN2_, FEN2_RESTORE, __VA_ARGS__))

int main() {
    int a = 1, b = 2, c = 3, d = 4;

	printf("Before:   %d, %d, %d, %d\n", a, b, c, d);

	BACKUP(a, b, c) {
		a = 123;
		b = 234;
		c = 345;
		printf("BACKUP:   %d, %d, %d, %d\n", a, b, c, d);
	}

	printf("Outside:  %d, %d, %d, %d\n", a, b, c, d);

	RUN_WITH(a, 11, b, 22, c, 33) {
		printf("RUN_WITH: %d, %d, %d, %d\n", a, b, c, d);
	}

	printf("AFTER:    %d, %d, %d, %d\n", a, b, c, d);

    return 0;
}
