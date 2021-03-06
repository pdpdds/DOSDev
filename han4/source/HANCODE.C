/****************************************************************************/
/*   TITLE              Hangul I/O Libarary <HAN> 4.0                       */
/*   SUB-TITLE          hangul code conversion functions                    */
/*   FILENAME           hancode.c                                           */
/*   DATE & TIME        11/10/91(SUN) 20:21                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

/****************************************************************************/
/*                             Macro Constants                              */
/****************************************************************************/

#define JOHAP           	0           /* 2 byte Jo-Hap code */
#define KS5601          	1           /* KS5601 code */
#define BIT7            	2           /* 7 bit Wan-Sung code*/
#define TABLE_NO     		1326

/****************************************************************************/
/*                               Data Types                                 */
/****************************************************************************/

typedef unsigned char mini;

/****************************************************************************/
/*               Tables(to be referenced by convert_han_code()              */  
/****************************************************************************/

const char han_code_table[][TABLE_NO*2] = 
{ 
   "�a�b�e�h�i�j�q�s�t�u�v�w�x�{�|�������������������������"
   "��������A�E�I�U�V�a�b�c�e�i�q�s�v�w�{����������������������"
   "���ŉɉ׉����A�E�W�a��������������������������������A�E"
   "�I�a�b�e�i�j�q�s�u�w�������������a�b�c�e�i�q�s�w�{��������"
   "�����A�a�u�����������������������A�a������������������a"
   "�e�g�i�q�u�{���������a�b�c�e�i�j�q�s�u�v�w�x�y�{�}����������"
   "����������������������A�B�E�I�U�a�b�e�q�v�w�z��������"
   "�������������֒A�a�w����������A�Q�a�b�e�i�j�q�w�x�|��������"
   "���������a�b�c�e�h�i�j�k�p�q�s�u�w�y�}�������������������"
   "�����������A�E�I�U�W��������������������A�E�I�Q�S��������"
   "��������������A�a�b�e�h�i�q�s�u�w���������������������a�b�e"
   "�i�q�w������������������A�I�������A������������a�b�e"
   "�h�i�q�u�������a�b�e�i�q�s�u�v�w�}��������������������������"
   "��������������A�B�E�I�Q�U�a�b�e�i�q�s�u�v�w������������"
   "�������A�a�w�����������������A�B�E�I�Q�W�a�b�e�i�q�s�u�w�|��"
   "���������������a�b�e�g�h�i�j�q�s�u�w�x�{�}������������������"
   "���������A�B�E�I�Q�U�a�e�i�w�y�������������������a������"
   "�����������������������A�Q�a�i�q���������������������a�b�c�e"
   "�h�i�j�l�q�s�u�w�{�����������������������������A�E�I�W"
   "�a�b�e�i�s�u�w�{�������������������֦A�I��������������������"
   "�A�a�e�i�������������������a�b�e�i�w���������������������"
   "���A�a�����������������a�e�i�q���a�b�e�i�k�q�s�u�v�w�{������"
   "����������������������������A�B�E�I�Q�S�U�a�e�v������"
   "������������A�a�e�����������������������ɮ�����A�Q�U�a"
   "�b�e�i�j�q�s�u�w�����������������������a�b�e�i�w�}��������"
   "������A�������������a�b�e�i�q�s�����������������a�b�e�f�g�i"
   "�p�q�s�u�v�w�|����������������������������������������"
   "���������������A�B�E�I�Q�U�a�b�c�e�i�i�q�s�u�w�}����������"
   "�������������������µŵɵֵ׵�A�E�a�b�e�w������������������"
   "�����������Ŷɶ�������A�B�E�I�W�a�b�e�i�q�s�w������������"
   "�����������������a�b�e�g�i�q�s�u�w�x������������������"
   "�������A�B�E�I�U�a�e�v���������������������A�a�e������������"
   "��������A�a�b�e�i�q�s�w�������������������������a�b�e�i�l�w"
   "�������������������������a�q�������������a�b�e�g�i�q�s�u"
   "�v�w�x�����������������������������A�B�I�a�v����������������"
   "�A¡�����A�a�b�q�wáâåéñóõ÷�a�b�e�i�q�w"
   "āąĉđē�����������������A�E�I�U�a�e�i�všŢťũűŵŷ��Ɓ"
   "ƂƉƗ�����A�a�e�i�qǡǢǥǩǱǵǷ�a�b�e�i�q�s�u�wȁȂȅȉȑ"
   "ȗ���������������A�B�E�I�Qɡɢɥɩɱɳɷ�Aʁʂʉʗ���A�a�b�e"
   "�i�q�uˁˡˢ˥˩˱˳˷�a�b�c�e�i�q�s�u�v�w�{̗́̂̅��������"
   "�����A�E�I�a�e�i�ẃͥͩ͢͡ͱͷ�a΁΂΅ΉΑΕΗ�A�a�e�i�qϡ"
   "ϢϥϩϱϳϵϷ�a�b�e�i�q�s�u�wЁЂЅБЕЖЗзз������������"
   "�A�B�E�I�a�b�e�i�q�s�v�wсѡѢѥѩѱѳ���B������������������"
   "�A�B�W�aҁ҂҅҉ґҕӡҥҩ���������A�W�a�b�e�i�j�q�s�w�{ӁӅ"
   "ӡӥөӱӵӵ",

   "�������������������������������������������������ŰưǰȰɰ�"
   "�̰ͰϰѰ԰հְٰڰܰݰް߰���������������������"
   "�������������������������������������������±ñǱȱ˱ͱӱԱ�"
   "�ֱױرٱ۱ܱݱޱ߱������������������������������"
   "�������������������òĲŲǲȲɲʲ˲ѲҲزٲڲ۲ܲݲ޲����"
   "������������������������������������������������������������"
   "���³óĳɳʳ˳̳ͳγгѳճֳ׳سٳڳݳ������������"
   "�����������������������������������´ôĴƴɴʴ˴̴ϴд�"
   "�ҴԴմ״ٴڴ۴ܴݴ޴ߴ�����������������������"
   "���������������������������������������ŵƵǵȵɵʵ˵εϵе�"
   "�ҵӵյֵ׵ڵ޵��������������������������������"
   "���������������������������������Ƕȶ˶϶ѶҶն׶ٶڶܶ߶��"
   "�����������������������������������������������������"
   "���������������������������������·÷ķŷƷǷȷɷʷηϷзѷ�"
   "�ӷԷշڷ������������������������������������������"
   "�������������������������������������¸øĸŸƸǸȸɸʸ͸θ�"
   "�Ըոָظڸ۸޸߸�������������������������������"
   "�����������������������¹Źǹɹʹ̹͹ιϹйҹӹֹ׹عٹڹ۹�"
   "�޹߹��������鵵�������������������������������"
   "�����������������������������������úƺɺκϺкҺӺպ׺غٺ�"
   "������������������������������������������������������"
   "�������ǻ̻ͻѻһӻԻջڻۻܻݻ߻�����������������"
   "�����������������������������������������������¼żǼ̼ҼӼ�"
   "�ּؼڼۼܼ���������������������������������������������"
   "���������������½ýĽŽƽǽȽɽʽ˽̽ͽνϽѽҽֽ׽ؽڽ���"
   "������������������������������������þľžƾǾȾɾʾ�"
   "�ξϾоѾҾӾվ־׾ؾپھ޾߾����������������"
   "����������������������������������������������������������"
   "�ÿĿſǿȿɿʿ˿ͿοϿпԿտֿܿ޿����������������"
   "������������������������������������������������������������"
   "������������������������������������������������������������"
   "������������������������������������������������������������"
   "����������������������������������������¡¢£¤¥¦§©ª¯"
   "°·¸¼½¾¿����������������������������������������������"
   "áâãäåæèìíóôõö÷øùûüýÿ��������������������"
   "��������������������������������ġĢģĥħĨĩĪīĬĭĮįĲ"
   "ĳĵĶķĸĿ������������������������������������������������"
   "����������ťũūŬŭŰűŲųŴŶŷŸŹźŻŽžſ������������"
   "��������������������������������������������������ƢƩƮƯư"
   "ƲƴƶƷƼƽƾƿ��������������������������������������������"
   "��������������������������������ǥǪǫǬǮǰǲǳǻ����������"
   "������������������������������������������������������������"
   "��������������������������ȣȤȥȦȨȩ����ȭȮȯȰȱȲȳȴȶ"
   "ȸȹȾȿ����������������������������������������������������"
   "������������",

   "_U_V_W_X_Y_Y_Z_[_\\_]_^___`_a_b_c_d_e_f_g_h_i_j_j_k_l_m_n_o_p"
   "_q_r_s_t_u_v_w_x_y_z_{_|_}_~`!`#`$`%`&`'`(`)`*`+`,`-`.`/`0`1"
   "`2`3`4`5`6`7`8`9`:`;`<`=`>`?`@`A`B`C`D`E`F`G`H`I`J`K`L`M`N`O"
   "`P`Q`R`S`T`U`V`W`X`Y`Z`[`\\`]`^`_```a`b`c`d`e`f`g`h`i`j`k`l`m"
   "`n`o`p`q`s`s`t`u`v`w`x`y`z`{`|`}`~aAaBaCaDaEaFaGaHaIaJaKaLaM"
   "aNaOaPaQaRaSaTaUaVaWaXaYaZa[a\\a]a^a_b@bAbBbCbDbEbFbGbHbIbJbK"
   "bLbMbNbObPbQbRbSbTbUbVbWbXbYbZb[b\\b]b^b_c@cAcBcCcDcEcFcGcHcI"
   "cJcKcLcMcNcOcPcQcRcScTcUcVcWcXcYcZc[c\\c]c^c_d@dAdBdCdDdEdFdG"
   "dHdIdJdKdLdMdNdOdPdQdRdSdTdUdVdWdXdYdZd[d\\d]d^d_e@eAeBeCeDeE"
   "eFeGeHeIeJeKeLeMeNeOePeQeReSeTeUeVeWeXeYeZe[e\\e]e^e_f@fAfBfC"
   "fDfEfFfGfHfIfJfKfLfMfNfOfPfQfRfSfTfUfVfWfXfYfZf[f\\f]f^f_g@gA"
   "gBgCgDgEgFgGgHgIgJgKgLgMgNgOgPgQgRgSgTgUgVgWgXgYgZg[g\\g]g^g_"
   "h@hAhBhChDhEhFhGhHhIhJhKhLhMhNhOhPhQhRhShThUhVhWhXhYhZh[h\\h]"
   "h^h_i@iAiBiCiDiEiFiGiHiIiJiKiLiMiNiOiPiQiRiSiTiUiViWiXiYiZi["
   "i\\i]i^i_j@jAjBjCjDjEjFjGjHjIjJjKjLjMjNjOjPjQjRjSjTjUjVjWjXjY"
   "jZj[j\\j]j^j_k@kAkBkCkDkEkFkGkHkIkJkKkLkMkNkOkPkQkRkSkTkUkVkW"
   "kXkYkZk[k\\k]k^k_l@lAlBlClDlElFlGlHlIlJlKlLlMlNlOlPlQlRlSlTlU"
   "lVlWlXlYlZl[l\\l]l^l_m@mAmBmCmDmEmFmGmHmImJmKmLmMmNmOmPmQmRmS"
   "mTmUmVmWmXmYmZm[m\\m]m^eQn@nAnBnCnDnEnFnGnHnInJnKnLnMnNnOnPnQ"
   "nRnSnTnUnVnWnXnYnZn[n\\n]n^n_o@oAoBoCoDoEoFoGoHoIoJoKoLoMoNoO"
   "oPoQoRoSoToUoVoWoXoYoZo[o\\o]o^o_p@pApEpCpDpEpFpGpHpHpIpJpKpL"
   "pMpNpOpPpQpRpSpTpUpVpWpXpYpZp[p\\p]p^p_q@qAqBqCqDqEqFqGqHqIqJ"
   "qKqLqMqNqOqPqQqRqSqTqUqVqWqXqYqZq[q\\q]q^q_r@rArBrCrDrErFrGrH"
   "rIrJrKrLrMrNrOrPrQrRrSrTrUrVrWrXrYrZr[r\\r]r^r_s@s@sAsBsCsDsE"
   "sFsGsHsIsJsKsLsMsNsOsPsQsRsSsTsUsVsWsXsYsZs[s\\s]s^s_s_t@tAtB"
   "tCtDtEtFtGtHtItJtKtLtMtNtOtPtQtRtStTtUtVtWtXtYtZt[t\\t]t^t_u@"
   "uAuBuCuDuEuFuGuHuIuJuKuLuMuNuOuPuQuRuSuTuUuVuWuXuYuZu[u\\u]u^"
   "u_v@vAvBvCvDvEvFvGvHvIvJvKvLvNvOvPvQvRvRvSvTvUvWvYuVuWv\\v]v^"
   "v_w@wAwBwCwDwEwFwGwHwIwJwKwLwMwNwOwPwQwRwSwTwUwVwWwXwYwZw[w\\"
   "w]w^w_x@xAxBxCxDxExFxGxHxIxJxKxLxMxNxOxPxQxRxSxTxUxVxWxXxYxZ"
   "x[x\\x]x^x_y@yAyByCyDyEyFyGyHyIyJyKyLyMyNyOyPyQyRySyTyUyVyWyX"
   "yYyZy[y\\y]y^y_z@zAzBzCzDzEzFzGzHzIzJzKzLzMzNzOzPzQzRzSzTzUzV"
   "zWzXzYzZz[z\\z]z^z_{!{#{${%{&{'{({){*{+{,{-{.{/{0{1{2{3{4{5{6"
   "{7{8{9{:{;{<{={>{?{@{A{B{C{D{E{F{G{H{I{J{K{L{M{N{O{P{Q{R{S{T"
   "{U{V{W{X{Y{Z{[{\\{]{^{_{`{a{b{c{d{e{f{g{h{i{j{k{l{m{n{o{p{p{q"
   "{r{s{t{u{v{w{x{y{z{{{|{}{~|!|#|$|%|&|'|(|)|*|+|,|-|.|/|0|1|2"
   "|3|4|5|6|7|8|9|:|;|<|=|>|?|@|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P"
   "|Q|R|S|T|U|V|W|X|Y|Z|[|\\|]|^|_|`|a|b|c|d|e|f|g|h|i|j|k|l|m|n"
   "|o|p|q|r|s|t|u|v|w|x|y|z|{|||}|~}!}#}$}%}&}'}(})}*}+},}-}.}/"
   "}0}1}2}3}4}5}6}7}8}9}:};}<}=}>}?}@}A}B}C}D}E}F}G}H}I}J}K}L}M"
   "}N}O}P}Q}R}S}T}U}V}W}X}Y}Z}[}\\}]}^}_}`}a}b}c}d}e}f}g}h}i}j}k"
   "}l}m}n}o}p}q}r}s}t}u}v}w}x}y}z}{}|}}}~~!~#~$~%~%~&~'~(~)~*~+"
   "~,~-~.~/~0~1~2~3~4~5~6~7~8~9~:~;~<~=~>~+~-~A~B~C~D~E~F~F~F~F"
   "~G~H~I~J~K~L~M~N~O~P~d~Q~R~S~T~U~V~W~X~Y~Z~[~\\~]~^~_~`~a~b~c"
   "~d~e~f~g~h~h"
};

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/
#include <string.h>
int convert_han_code(char *string, int source_code, int dest_code);

/****************************************************************************/
/*                    Implementations of the C Functions                    */
/****************************************************************************/

int convert_han_code(char *string, int source_code, int dest_code)
{
   mini byte0,byte1;
   int index,ret_code,i;

   if(source_code < JOHAP || source_code > BIT7 ||
      dest_code   < JOHAP || dest_code   > BIT7) return(-1);

   ret_code = index = 0;
   while(index < strlen(string))
   {
      byte0 = string[index++];

      /* if not HANGUL, continue */      
      if(source_code == BIT7)
      {
         byte1 = string[index];
         switch(byte0)
         {
            case '_' : if(byte1 >= 0x21 && byte1 <= 0x7e) 
                          break;
                       else 
                          continue;
            case '`' : if((byte1 >= 0x00 && byte1 <= 0x9f) || (byte1 >= 0xc0 && byte1 <= 0xdf))
                          break;
                       else
                          continue;
            case '{' : 
            case '!' : 
            case '}' :
            case '|' :
            case '~' : if(byte1 >= 0x21 && byte1 <= 0x7e)
                          break; 
                       else 
                          continue;
            default  :
               if( (byte0 >= 'a' && byte0 <= 'z') &&
		   ((byte1 >= 0x00 && byte1 <= 0x1f) ||
                    (byte1 >= 0x40 && byte1 <= 0x5f) ||
                    (byte1 >= 0x80 && byte1 <= 0x9f) ||
                    (byte1 >= 0xc0 && byte1 <= 0xdf)) )
                  break;
               else
                  continue;
         }
      }
      else if(!(byte0 & 0x80)) 
         continue;

      byte1 = string[index++];

      /* get offset by string <str> */
      for(i = 0; i < TABLE_NO; i++)
         if(byte0 == han_code_table[source_code][i*2] && byte1 == han_code_table[source_code][i*2+1]) break;

      /* if founded replace it or, mask on error_code */
      if(i < TABLE_NO) 
      {
         string[index-2] = han_code_table[dest_code][i*2];
         string[index-1] = han_code_table[dest_code][i*2+1];
      }
      else
         ret_code = -2;
   }
   return(ret_code);
}
