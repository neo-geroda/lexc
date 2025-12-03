#include <stdio.h>
#include <ctype.h>
#include "../include/lookupkeywords.h"
#include "../include/tokens.h"

const char *YYCursor;
char yych;

    int lookupKeywords(char *s) {
        YYCursor = s;
        yych = *YYCursor;
        switch (yych) {
            case 'a': goto Q12;
            case 'o': goto Q15;
            case 'n': goto Q17;
            case 'i': goto Q21;
            case 'e': goto Q23;
            case 'r': goto Q29;
            case 's': goto Q38;
            case 'c': goto Q42;
            case 'f': goto Q50;
            case 'g': goto Q58;
            case 'd': goto Q66;
            case 'b': goto Q97;
            case 't': goto Q101;
            case 'l': goto Q105;
            case 'v': goto Q132;
            default: goto Q200;
        }

        Q12:
            yych = *(++YYCursor);
            switch(yych){
                case 'n': goto Q13;
                default: goto Q200;
            }
        Q13:
            yych = *(++YYCursor);
            switch(yych){
                case 'd': goto Q14;
                default: goto Q200;
            }
        Q14:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return AND_OP;
                default: goto Q200;
            }
        Q15:
            yych = *(++YYCursor);
            switch(yych){
                case 'r': goto Q16;
                case 'n': goto Q76;
                default: goto Q200;
            }
        Q16:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return OR_OP;
                default: goto Q200;
            }
        Q17:
            yych = *(++YYCursor);
            switch(yych){
                case 'o': goto Q18;
                case 'u': goto Q81;
                default: goto Q200;
            }
        Q18:
            yych = *(++YYCursor);
            switch(yych){
                case 't': goto Q19;
                default: goto Q200;
            }   
        Q19:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return NOT_OP;
                case 'h': goto Q116;
                default: goto Q200;
            }
        Q21:
            yych = *(++YYCursor);
            switch(yych){
                case 'f': goto Q22;
                case 'n': goto Q120;
                default: goto Q200;
            }
        Q22:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return IF;
                default: goto Q200;
            }
        Q23:
            yych = *(++YYCursor);
            switch(yych){
                case 'l': goto Q24;
                case 'n': goto Q141;
                default: goto Q200;
            }
        Q24:
            yych = *(++YYCursor);
            switch(yych){
                case 's': goto Q25;
                case 'i': goto Q27;
                default: goto Q200;
            }
        Q25:
            yych = *(++YYCursor);
            switch(yych){
                case 'e': goto Q26;
                default: goto Q200;
            }
        Q26:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return ELSE;
                default: goto Q200;
            }
        Q27:
            yych = *(++YYCursor);
            switch(yych){
                case 'f': goto Q28;
                default: goto Q200;
            }
        Q28:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return ELIF;
                default: goto Q200;
            }
        Q29:
            yych = *(++YYCursor);
            switch(yych){
                case 'e': goto Q30;
                default: goto Q200;
            }
        Q30:
            yych = *(++YYCursor);
            switch(yych){
                case 'p': goto Q31;
                default: goto Q200;
            }
        Q31:
            yych = *(++YYCursor);
            switch(yych){
                case 'e': goto Q32;
                default: goto Q200;
            }
        Q32:
            yych = *(++YYCursor);
            switch(yych){
                case 'a': goto Q33;
                default: goto Q200;
            }
        Q33:
            yych = *(++YYCursor);
            switch(yych){
                case 't': goto Q34;
                default: goto Q200;
            }
        Q34:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return REPEAT;
                case 'f': goto Q35;
                default: goto Q200;
            }
        Q35:
            yych = *(++YYCursor);
            switch(yych){
                case 'o': goto Q36;
                default: goto Q200;
            }
        Q36:
            yych = *(++YYCursor);
            switch(yych){
                case 'r': goto Q37;
                default: goto Q200;
            }
        Q37:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return REPEATFOR;
                default: goto Q200;
            }
        Q38:
            yych = *(++YYCursor);
            switch(yych){
                case 't': goto Q39;
                case 'y': goto Q92;
                case 'o': goto Q143;
                default: goto Q200;
            }
        Q39:
            yych = *(++YYCursor);
            switch(yych){
                case 'o': goto Q40;
                case 'a': goto Q62;
                default: goto Q200;
            }
        Q40:
            yych = *(++YYCursor);
            switch(yych){
                case 'p': goto Q41;
                default: goto Q200;
            }
        Q41:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return STOP;
                default: goto Q200;
            }
        Q42:
            yych = *(++YYCursor);
            switch(yych){
                case 'o': goto Q43;
                default: goto Q200;
            }
        Q43:
            yych = *(++YYCursor);
            switch(yych){
                case 'n': goto Q44;
                default: goto Q200;
            }
        Q44:
            yych = *(++YYCursor);
            switch(yych){
                case 't': goto Q45;
                default: goto Q200;
            }
        Q45:
            yych = *(++YYCursor);
            switch(yych){
                case 'i': goto Q46;
                default: goto Q200;
            }
        Q46:
            yych = *(++YYCursor);
            switch(yych){
                case 'n': goto Q47;
                default: goto Q200;
            }
        Q47:
            yych = *(++YYCursor);
            switch(yych){
                case 'u': goto Q48;
                default: goto Q200;
            }
        Q48:
            yych = *(++YYCursor);
            switch(yych){
                case 'e': goto Q49;
                default: goto Q200;
            }
        Q49:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return CONTINUE;
                default: goto Q200;
            }
        Q50:
            yych = *(++YYCursor);
            switch(yych){
                case 'u': goto Q51;
                case 'a': goto Q112;
                default: goto Q200;
            }
        Q51:
            yych = *(++YYCursor);
            switch(yych){
                case 'n': goto Q52;
                default: goto Q200;
            }
        Q52:
            yych = *(++YYCursor);
            switch(yych){
                case 'c': goto Q53;
                default: goto Q200;
            }
        Q53:
            yych = *(++YYCursor);
            switch(yych){
                case 't': goto Q54;
                default: goto Q200;
            }
        Q54:
            yych = *(++YYCursor);
            switch(yych){
                case 'i': goto Q55;
                default: goto Q200;
            }
        Q55:
            yych = *(++YYCursor);
            switch(yych){
                case 'o': goto Q56;
                default: goto Q200;
            }
        Q56:
            yych = *(++YYCursor);
            switch(yych){
                case 'n': goto Q57;
                default: goto Q200;
            }
        Q57:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return FUNCTION;
                default: goto Q200;
            }
        Q58:
            yych = *(++YYCursor);
            switch(yych){
                case 'i': goto Q59;
                case 'e': goto Q73;
                default: goto Q200;
            }
        Q59:
            yych = *(++YYCursor);
            switch(yych){
                case 'v': goto Q60;
                default: goto Q200;
            }
        Q60:
            yych = *(++YYCursor);
            switch(yych){
                case 'e': goto Q61;
                default: goto Q200;
            }
        Q61:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return GIVE;
                default: goto Q200;
            }
        Q62:
            yych = *(++YYCursor);
            switch(yych){
                case 't': goto Q63;
                case 'r': goto Q139;
                default: goto Q200;
            }
        Q63:
            yych = *(++YYCursor);
            switch(yych){
                case 'i': goto Q64;
                default: goto Q200;
            }
        Q64:
            yych = *(++YYCursor);
            switch(yych){
                case 'c': goto Q65;
                default: goto Q200;
            }
        Q65:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return STATIC;
                default: goto Q200;
            }
        Q66:
            yych = *(++YYCursor);
            switch(yych){
                case 'i': goto Q67;
                case 'e': goto Q86;
                default: goto Q200;
            }
        Q67:
            yych = *(++YYCursor);
            switch(yych){
                case 's': goto Q68;
                default: goto Q200;
            }
        Q68:
            yych = *(++YYCursor);
            switch(yych){
                case 'p': goto Q69;
                default: goto Q200;
            }
        Q69:
            yych = *(++YYCursor);
            switch(yych){
                case 'l': goto Q70;
                default: goto Q200;
            }
        Q70:
            yych = *(++YYCursor);
            switch(yych){
                case 'a': goto Q71;
                default: goto Q200;
            }
        Q71:
            yych = *(++YYCursor);
            switch(yych){
                case 'y': goto Q72;
                default: goto Q200;
            }
        Q72:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return DISPLAY;
                default: goto Q200;
            }
        Q73:
            yych = *(++YYCursor);
            switch(yych){
                case 't': goto Q74;
                default: goto Q200;
            }
        Q74:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return GET;
                default: goto Q200;
            }
        Q76:
            yych = *(++YYCursor);
            switch(yych){
                case 'f': goto Q77;
                default: goto Q200;
            }
        Q77:
            yych = *(++YYCursor);
            switch(yych){
                case 'a': goto Q78;
                default: goto Q200;
            }
        Q78:
            yych = *(++YYCursor);
            switch(yych){
                case 'i': goto Q79;
                default: goto Q200;
            }
        Q79:
            yych = *(++YYCursor);
            switch(yych){
                case 'l': goto Q80;
                default: goto Q200;
            }
        Q80:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return ONFAIL;
                default: goto Q200;
            }
        Q81:
            yych = *(++YYCursor);
            switch(yych){
                case 'm': goto Q82;
                default: goto Q200;
            }
        Q82:
            yych = *(++YYCursor);
            switch(yych){
                case 'b': goto Q83;
                default: goto Q200;
            }
        Q83:
            yych = *(++YYCursor);
            switch(yych){
                case 'e': goto Q84;
                default: goto Q200;
            }
        Q84:
            yych = *(++YYCursor);
            switch(yych){
                case 'r': goto Q85;
                default: goto Q200;
            }
        Q85:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return NUMBER;
                case 'a': goto Q86;
                default: goto Q200;
            }
        Q86:
            yych = *(++YYCursor);
            switch(yych){
                case 'c': goto Q87;
                default: goto Q200;
            }
        Q87:
            yych = *(++YYCursor);
            switch(yych){
                case 'i': goto Q88;
                default: goto Q200;
            }
        Q88:
            yych = *(++YYCursor);
            switch(yych){
                case 'm': goto Q89;
                default: goto Q200;
            }
        Q89:
            yych = *(++YYCursor);
            switch(yych){
                case 'a': goto Q90;
                default: goto Q200;
            }
        Q90:
            yych = *(++YYCursor);
            switch(yych){
                case 'l': goto Q91;
                default: goto Q200;
            }
        Q91:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return DECIMAL;
                default: goto Q200;
            }
        Q92:
            yych = *(++YYCursor);
            switch(yych){
                case 'm': goto Q93;
                default: goto Q200;
            }
        Q93:
            yych = *(++YYCursor);
            switch(yych){
                case 'b': goto Q94;
                default: goto Q200;
            }
        Q94:
            yych = *(++YYCursor);
            switch(yych){
                case 'o': goto Q95;
                default: goto Q200;
            }
        Q95:
            yych = *(++YYCursor);
            switch(yych){
                case 'l': goto Q96;
                default: goto Q200;
            }
        Q96:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return SYMBOL;
                default: goto Q200;
            }
        Q97:
            yych = *(++YYCursor);
            switch(yych){
                case 'o': goto Q98;
                case 'u': goto Q127;
                default: goto Q200;
            }
        Q98:
            yych = *(++YYCursor);
            switch(yych){
                case 'o': goto Q99;
                default: goto Q200;
            }
        Q99:
            yych = *(++YYCursor);
            switch(yych){
                case 'l': goto Q100;
                default: goto Q200;
            }
        Q100:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return BOOL;
                default: goto Q200;
            }
        Q101:
            yych = *(++YYCursor);
            switch(yych){
                case 'e': goto Q102;
                case 'r': goto Q109;
                case 'h': goto Q136;
                default: goto Q200;
            }
        Q102:
            yych = *(++YYCursor);
            switch(yych){
                case 'x': goto Q103;
                default: goto Q200;
            }
        Q103:
            yych = *(++YYCursor);
            switch(yych){
                case 't': goto Q104;
                default: goto Q200;
            }
        Q104:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return TEXT;
                default: goto Q200;
            }
        Q105:
            yych = *(++YYCursor);
            switch(yych){
                case 'i': goto Q106;
                default: goto Q200;
            }
        Q106:
            yych = *(++YYCursor);
            switch(yych){
                case 's': goto Q107;
                default: goto Q200;
            }
        Q107:
            yych = *(++YYCursor);
            switch(yych){
                case 't': goto Q108;
                default: goto Q200;
            }
        Q108:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return LIST;
                default: goto Q200;
            }
        Q109:
            yych = *(++YYCursor);
            switch(yych){
                case 'u': goto Q110;
                case 'y': goto Q126;
                default: goto Q200;
            }
        Q110:
            yych = *(++YYCursor);
            switch(yych){
                case 'e': goto Q111;
                default: goto Q200;
            }
        Q111:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return TRUE;
                default: goto Q200;
            }
        Q112:
            yych = *(++YYCursor);
            switch(yych){
                case 'l': goto Q113;
                default: goto Q200;
            }
        Q113:
            yych = *(++YYCursor);
            switch(yych){
                case 's': goto Q114;
                default: goto Q200;
            }
        Q114:
            yych = *(++YYCursor);
            switch(yych){
                case 'e': goto Q115;
                default: goto Q200;
            }
        Q115:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return FALSE;
                default: goto Q200;
            }
        Q116:
            yych = *(++YYCursor);
            switch(yych){
                case 'i': goto Q117;
                default: goto Q200;
            }
        Q117:
            yych = *(++YYCursor);
            switch(yych){
                case 'n': goto Q118;
                default: goto Q200;
            }
        Q118:
            yych = *(++YYCursor);
            switch(yych){
                case 'g': goto Q119;
                default: goto Q200;
            }
        Q119:   
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return NOTHING;
                default: goto Q200;
            }
        Q120:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return IN;
                case 'c': goto Q121;
                default: goto Q200;
            }
        Q121:
            yych = *(++YYCursor);
            switch(yych){
                case 'l': goto Q122;
                default: goto Q200;
            }
        Q122:
            yych = *(++YYCursor);
            switch(yych){
                case 'u': goto Q123;
                default: goto Q200;
            }
        Q123:
            yych = *(++YYCursor);
            switch(yych){
                case 'd': goto Q124;
                default: goto Q200;
            }
        Q124:
            yych = *(++YYCursor);
            switch(yych){
                case 'e': goto Q125;
                default: goto Q200;
            }
        Q125:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return INCLUDE;
                default: goto Q200;
            }
        Q126:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return TRY;
                default: goto Q200;
            }
        Q127:
            yych = *(++YYCursor);
            switch(yych){
                case 'n': goto Q128;
                default: goto Q200;
            }
        Q128:
            yych = *(++YYCursor);
            switch(yych){
                case 'd': goto Q129;
                default: goto Q200;
            }
        Q129:
            yych = *(++YYCursor);
            switch(yych){
                case 'l': goto Q130;
                default: goto Q200;
            }
        Q130:
            yych = *(++YYCursor);
            switch(yych){
                case 'e': goto Q131;
                default: goto Q200;
            }
        Q131:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return BUNDLE;
                default: goto Q200;
            }
        Q132:
            yych = *(++YYCursor);
            switch(yych){
                case 'o': goto Q133;
                default: goto Q200;
            }
        Q133:
            yych = *(++YYCursor);
            switch(yych){
                case 'i': goto Q134;
                default: goto Q200;
            }
        Q134:
            yych = *(++YYCursor);
            switch(yych){
                case 'd': goto Q135;
                default: goto Q200;
            }
        Q135:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return VOID;
                default: goto Q200;
            }
        Q136:
            yych = *(++YYCursor);
            switch(yych){
                case 'e': goto Q137;
                default: goto Q200;
            }
        Q137:
            yych = *(++YYCursor);
            switch(yych){
                case 'n': goto Q138;
                default: goto Q200;
            }
        Q138:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return NOISE_WORD;
                default: goto Q200;
            }
        Q139:
            yych = *(++YYCursor);
            switch(yych){
                case 't': goto Q140;
                default: goto Q200;
            }
        Q140:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return NOISE_WORD;
                default: goto Q200;
            }
        Q141:
            yych = *(++YYCursor);
            switch(yych){
                case 'd': goto Q142;
                default: goto Q200;
            }
        Q142:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return NOISE_WORD;
                default: goto Q200;
            }
        Q143:
            yych = *(++YYCursor);
            switch(yych){
                case '\0': return NOISE_WORD;
                default: goto Q200;
            }
        Q200:
            return IDENT;
    }