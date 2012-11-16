/**
���ļ�������Lex��Yacc����Ҫ�ĸ��������Ķ��塣
**/

#include "YYParser.h"
#include "YYLexer.h"
#include "Lex.h"

int yyerror(const char* err){
	return 0;
}

int yylex(YYSTYPE *lvalp, void* pLexer)
{
	((Lexer*)pLexer)->SetTokenValue(lvalp);
	return ((Lexer*)pLexer)->yylex();
}
