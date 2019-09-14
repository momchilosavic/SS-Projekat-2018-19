#ifndef _HELPER_H_
#define _HELPER_H_

#define DECIMAL_8_POSITIVE "^[[:space:]]*((0*([1-9]|[1-9][0-9]|1[0-1][0-9]|12[0-7]))|(0+))[[:space:]]*$"
#define DECIMAL_8_NEGATIVE "^[[:space:]]*-0*([1-9]|[1-9][0-9]|1[0-1][0-9]|12[0-8])[[:space:]]*$"

#define DECIMAL_16_POSITIVE "^[[:space:]]*0*(12[8-9]|1[3-9][0-9]|[2-9][0-9]{2}|[1-9][0-9]{3}|[1-2][0-9]{4}|3[0-1][0-9]{3}|32[0-6][0-9]{2}|327[0-5][0-9]|3276[0-7])[[:space:]]*$"
#define DECIMAL_16_NEGATIVE "^[[:space:]]*-0*(129|1[3-9][0-9]|[2-9][0-9]{2}|[1-9][0-9]{3}|[1-2][0-9]{4}|3[0-1][0-9]{3}|32[0-6][0-9]{2}|327[0-5][0-9]|3276[0-8])[[:space:]]*$"

#define HEXADECIMAL_8_POSITIVE "^[[:space:]]*(0x|0X)((0*(([1-9]|[a-f]|[A-F])|([1-7]([0-9]|[a-f]|[A-F]))))|(0+))[[:space:]]*$"
#define HEXADECIMAL_8_NEGATIVE "^[[:space:]]*(0x|0X)0*([8-9]|[a-f]|[A-F])([0-9]|[a-f]|[A-F])[[:space:]]*$"

#define HEXADECIMAL_16_POSITIVE "^[[:space:]]*(0x|0X)0*((([1-9]|[a-f]|[A-F])([0-9]|[a-f]|[A-F]){2})|([1-7]([0-9]|[a-f]|[A-F]){3}))[[:space:]]*$"
#define HEXADECIMAL_16_NEGATIVE "^[[:space:]]*(0x|0X)0*([8-9]|[a-f]|[A-F])([0-9]|[a-f]|[A-F]){3}[[:space:]]*$"

#define BINARY_8_POSITIVE "^[[:space:]]*(0+|0*1[0-1]{0,6})(b|B)[[:space:]]*$"
#define BINARY_8_NEGATIVE "^[[:space:]]*0*1[0-1]{7}(b|B)[[:space:]]*$"

#define BINARY_16_POSITIVE "^[[:space:]]*0*1[0-1]{8-14}(b|B)[[:space:]]*$"
#define BINARY_16_NEGATIVE "^[[:space:]]*0*1[0-1]{15}(b|B)[[:space:]]*$"

#define SYMBOL "^[[:space:]]*[_[:alpha:]][_[:alnum:]]*[[:space:]]*$"

#define LABEL "^[[:space:]]*[_[:alpha:]][_[:alnum:]]*:[[:space:]]*$"

#endif
