grammar HLSL;

hlslFile
    : element* EOF
    ;

element
    : structDeclaration
    | functionDeclaration
    ;

structDeclaration
    : 'struct' ID '{' structMember+ '}' ';'
    ;

structMember
    : type ID ':' semantic ';'
    ;

type
    : 'float' | 'float2' | 'float3' | 'float4'
    | 'int' | 'matrix'
    | ID
    ;

functionDeclaration
    : type ID '(' parameterList? ')' (':' semantic)? block
    ;

parameterList
    : parameter (',' parameter)*
    ;

parameter
    : type ID ':' semantic
    ;

block
    : '{' statement* '}'
    ;

statement
    : declaration ';'
    | assignment ';'
    | returnStatement ';'
    ;

declaration
    : type ID
    ;

assignment
    : ID '=' expression
    ;

returnStatement
    : 'return' expression
    ;

expression
    : ID
    | functionCall
    | constructorExpression
    ;

functionCall
    : ID '(' argumentList? ')'
    ;

constructorExpression
    : type '(' argumentList ')'
    ;

argumentList
    : expression (',' expression)*
    ;

semantic
    : ID (DIGIT+)?
    ;

ID  : [a-zA-Z_][a-zA-Z_0-9]*;
DIGIT: [0-9];
WS  : [ \t\r\n]+ -> skip;
COMMENT: '//' ~[\r\n]* -> skip;
