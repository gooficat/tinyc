#ifndef __DICT__H__
#define __DICT__H__

enum keyword {
  KwInt,
  KwSigned,
  KwUnsigned,
  KwShort,
  KwChar,
  KwLong,
  KwFloat,
  KwDouble,
  KwVoid,
  KwStruct,
  KwUnion,
  KwEnum,
  KwVolatile,
  KwRegister,
  KwConst,
  KwAuto,
  KwStatic,
  KwExtern,
  KwInline,
  KwTypedef,

  KwReturn,
  KwBreak,
  KwContinue,
  KwGoto,

  KwIf,
  KwElse,
  KwWhile,
  KwDo,
  KwFor,
  KwSwitch,
  KwCase,
  KwDefault,

  KwSizeof
};
enum punctuator {
  PnBraceL,
  PnBraceR,
  PnBrackL,
  PnBrackR,
  PnParenL,
  PnParenR,
  PnComma,
  PnColon,
  PnSemi
};

enum operator{
  OpDot,
  OpArrow,
  OpDec,
  OpInc,
  OpShRAss,
  OpShLAss,
  OpAddAss,
  OpSubAss,
  OpMulAss,
  OpDivAss,
  OpModAss,
  OpShR,
  OpShL,
  OpAdd,
  OpSub,
  OpMul,
  OpDiv,
  OpMod,
  OpLogAnd,
  OpLogOr,
  OpLogEqu,
  OpLogGEqu,
  OpLogLEqu,
  OpBinAndEqu,
  OpBinOrEqu,
  OpBinXOrEqu,
  OpLogGrtr,
  OpLogLess,
  OpBinAnd,
  OpBinOr,
  OpBinXOr,
  OpBinNot,
  OpAss
};

extern char const *const KEYWORDS[];
extern char const PUNCTUATORS[];
extern char const *const OPERATORS[];

#endif
