/******************************************************************************
*
*
* Notepad2
*
* Styles.c
*   Scintilla Style Management
*
* See Readme.txt for more information about this source code.
* Please send me your comments to this work.
*
* See License.txt for details about distribution and modification.
*
*                                              (c) Florian Balmer 1996-2011
*                                                  florian.balmer@gmail.com
*                                               http://www.flos-freeware.ch
*
*
******************************************************************************/
#if !defined(_WIN32_WINNT)
#define _WIN32_WINNT 0x501
#endif
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <stdio.h>
#include "scintilla.h"
#include "scilexer.h"
#include "notepad2.h"
#include "edit.h"
#include "styles.h"
#include "dialogs.h"
#include "helpers.h"
#include "resource.h"
#include "SciCall.h"

extern int iEncoding;


#define MULTI_STYLE(a,b,c,d) ((a)|(b<<8)|(c<<16)|(d<<24))


KEYWORDLIST KeyWords_NULL = {
"", "", "", "", "", "", "", "", "" };


EDITLEXER lexDefault = { SCLEX_NULL, 63000, L"Default Text", L"txt; text; wtx; log; asc; doc", L"", &KeyWords_NULL, {
                /*  0 */ { STYLE_DEFAULT, 63100, L"Default Style", L"font:Consolas; size:10", L"" },
                /*  1 */ { STYLE_LINENUMBER, 63101, L"Margins and Line Numbers", L"size:-2; fore:#FF0000", L"" },
                /*  2 */ { STYLE_BRACELIGHT, 63102, L"Matching Braces", L"size:+1; bold; fore:#FF0000", L"" },
                /*  3 */ { STYLE_BRACEBAD, 63103, L"Matching Braces Error", L"size:+1; bold; fore:#000080", L"" },
                /*  4 */ { STYLE_CONTROLCHAR, 63104, L"Control Characters (Font)", L"size:-1", L"" },
                /*  5 */ { STYLE_INDENTGUIDE, 63105, L"Indentation Guide (Color)", L"fore:#A0A0A0", L"" },
                /*  6 */ { SCI_SETSELFORE+SCI_SETSELBACK, 63106, L"Selected Text (Colors)", L"back:#0A246A; eolfilled; alpha:95", L"" },
                /*  7 */ { SCI_SETWHITESPACEFORE+SCI_SETWHITESPACEBACK+SCI_SETWHITESPACESIZE, 63107, L"Whitespace (Colors, Size 0-5)", L"fore:#FF4000", L"" },
                /*  8 */ { SCI_SETCARETLINEBACK, 63108, L"Current Line Background (Color)", L"back:#FFFF00; alpha:50", L"" },
                /*  9 */ { SCI_SETCARETFORE+SCI_SETCARETWIDTH, 63109, L"Caret (Color, Size 1-3)", L"", L"" },
                /* 10 */ { SCI_SETEDGECOLOUR, 63110, L"Long Line Marker (Colors)", L"fore:#FFC000", L"" },
                /* 11 */ { SCI_SETEXTRAASCENT+SCI_SETEXTRADESCENT, 63111, L"Extra Line Spacing (Size)", L"size:2", L"" },

                /* 12 */ { STYLE_DEFAULT, 63112, L"2nd Default Style", L"font:Courier New; size:10", L"" },
                /* 13 */ { STYLE_LINENUMBER, 63113, L"2nd Margins and Line Numbers", L"font:Tahoma; size:-2; fore:#FF0000", L"" },
                /* 14 */ { STYLE_BRACELIGHT, 63114, L"2nd Matching Braces", L"bold; fore:#FF0000", L"" },
                /* 15 */ { STYLE_BRACEBAD, 63115, L"2nd Matching Braces Error", L"bold; fore:#000080", L"" },
                /* 16 */ { STYLE_CONTROLCHAR, 63116, L"2nd Control Characters (Font)", L"size:-1", L"" },
                /* 17 */ { STYLE_INDENTGUIDE, 63117, L"2nd Indentation Guide (Color)", L"fore:#A0A0A0", L"" },
                /* 18 */ { SCI_SETSELFORE+SCI_SETSELBACK, 63118, L"2nd Selected Text (Colors)", L"eolfilled", L"" },
                /* 19 */ { SCI_SETWHITESPACEFORE+SCI_SETWHITESPACEBACK+SCI_SETWHITESPACESIZE, 63119, L"2nd Whitespace (Colors, Size 0-5)", L"fore:#FF4000", L"" },
                /* 20 */ { SCI_SETCARETLINEBACK, 63120, L"2nd Current Line Background (Color)", L"back:#FFFF00; alpha:50", L"" },
                /* 21 */ { SCI_SETCARETFORE+SCI_SETCARETWIDTH, 63121, L"2nd Caret (Color, Size 1-3)", L"", L"" },
                /* 22 */ { SCI_SETEDGECOLOUR, 63122, L"2nd Long Line Marker (Colors)", L"fore:#FFC000", L"" },
                /* 23 */ { SCI_SETEXTRAASCENT+SCI_SETEXTRADESCENT, 63123, L"2nd Extra Line Spacing (Size)", L"", L"" },
                         { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_HTML = {
"!doctype ^aria- ^data- a abbr accept accept-charset accesskey acronym action address align alink "
"alt and applet archive area article aside async audio autocomplete autofocus autoplay axis b "
"background base basefont bb bdi bdo bgcolor big blockquote body border bordercolor br button "
"canvas caption cellpadding cellspacing center challenge char charoff charset checkbox checked "
"cite class classid clear code codebase codetype col colgroup color cols colspan command compact "
"content contenteditable contextmenu controls coords data datafld dataformatas datagrid datalist "
"datapagesize datasrc datetime dd declare default defer del details dfn dialog dir dirname "
"disabled div dl draggable dropzone dt em embed enctype event eventsource face fieldset "
"figcaption figure file font footer for form formaction formenctype formmethod formnovalidate "
"formtarget frame frameborder frameset h1 h2 h3 h4 h5 h6 head header headers height hgroup hidden "
"high hr href hreflang hspace html http-equiv i icon id iframe image img input ins isindex ismap "
"kbd keygen keytype kind label lang language leftmargin legend li link list longdesc loop low "
"manifest map marginheight marginwidth mark max maxlength media menu meta meter method min "
"multiple name nav noframes nohref noresize noscript noshade novalidate nowrap object ol onabort "
"onafterprint onbeforeprint onbeforeunload onblur oncanplay oncanplaythrough onchange onclick "
"oncontextmenu oncuechange ondblclick ondrag ondragend ondragenter ondragleave ondragover "
"ondragstart ondrop ondurationchange onemptied onended onerror onfocus onformchange onforminput "
"onhashchange oninput oninvalid onkeydown onkeypress onkeyup onload onloadeddata onloadedmetadata "
"onloadstart onmessage onmousedown onmousemove onmouseout onmouseover onmouseup onmousewheel "
"onoffline ononline onpagehide onpageshow onpause onplay onplaying onpopstate onprogress "
"onratechange onreadystatechange onredo onreset onresize onscroll onseeked onseeking onselect "
"onshow onstalled onstorage onsubmit onsuspend ontimeupdate onundo onunload onvolumechange "
"onwaiting open optgroup optimum option output p param password pattern ping placeholder poster "
"pre preload profile progress prompt pubdate public q radio radiogroup readonly rel required "
"reset rev reversed rows rowspan rp rt ruby rules s samp sandbox scheme scope scoped script "
"scrolling seamless section select selected shape size sizes small source span spellcheck src "
"srcdoc srclang standby start step strike strong style sub submit summary sup tabindex table "
"target tbody td text textarea tfoot th thead time title topmargin tr track tt type u ul usemap "
"valign value valuetype var version video vlink vspace wbr width wrap xml xmlns",
"abstract boolean break byte case catch char class const continue debugger default delete do "
"double else enum export extends false final finally float for function goto if implements "
"import in instanceof int interface long native new null package private protected public "
"return short static super switch synchronized this throw throws transient true try typeof var "
"void volatile while with",
"alias and as attribute begin boolean byref byte byval call case class compare const continue "
"currency date declare dim do double each else elseif empty end enum eqv erase error event exit "
"explicit false for friend function get global gosub goto if imp implement in integer is let lib "
"load long loop lset me mid mod module new next not nothing null object on option optional or "
"preserve private property public raiseevent redim rem resume return rset select set single "
"static stop string sub then to true type unload until variant wend while with withevents xor",
"",
"__callstatic __class__ __dir__ __file__ __function__ __get __isset __line__ __method__ "
"__namespace__ __set __sleep __unset __wakeup abstract and argc argv array as break case catch "
"cfunction class clone const continue declare default define die do e_all e_error e_fatal "
"e_notice e_parse e_strict e_warning echo else elseif empty enddeclare endfor endforeach endif "
"endswitch endwhile eval exception exit extends false final for foreach function global goto "
"http_cookie_vars http_env_vars http_get_vars http_post_files http_post_vars http_server_vars if "
"implements include include_once instanceof interface isset list namespace new not null "
"old_function or parent php_self print private protected public require require_once return "
"static stdclass switch this throw true try unset use var virtual while xor",
"", "", "", "" };


EDITLEXER lexHTML = { SCLEX_HTML, 63001, L"Web Source Code", L"html; htm; asp; aspx; shtml; htd; xhtml; php; php3; phtml; htt; cfm; tpl; dtd; hta; htc", L"", &KeyWords_HTML, {
                      { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                      { MULTI_STYLE(SCE_H_TAG,SCE_H_TAGEND,0,0), 63136, L"HTML Tag", L"fore:#648000", L"" },
                      { SCE_H_TAGUNKNOWN, 63137, L"HTML Unknown Tag", L"fore:#C80000; back:#FFFF80", L"" },
                      { SCE_H_ATTRIBUTE, 63138, L"HTML Attribute", L"fore:#FF4000", L"" },
                      { SCE_H_ATTRIBUTEUNKNOWN, 63139, L"HTML Unknown Attribute", L"fore:#C80000; back:#FFFF80", L"" },
                      { SCE_H_VALUE, 63140, L"HTML Value", L"fore:#3A6EA5", L"" },
                      { MULTI_STYLE(SCE_H_DOUBLESTRING,SCE_H_SINGLESTRING,0,0), 63141, L"HTML String", L"fore:#3A6EA5", L"" },
                      { SCE_H_OTHER, 63142, L"HTML Other Inside Tag", L"fore:#3A6EA5", L"" },
                      { MULTI_STYLE(SCE_H_COMMENT,SCE_H_XCCOMMENT,0,0), 63143, L"HTML Comment", L"fore:#646464", L"" },
                      { SCE_H_ENTITY, 63144, L"HTML Entity", L"fore:#B000B0", L"" },
                      { SCE_H_DEFAULT, 63256, L"HTML Element Text", L"", L"" },
                      { MULTI_STYLE(SCE_H_XMLSTART,SCE_H_XMLEND,0,0), 63145, L"XML Identifier", L"bold; fore:#881280", L"" },
                      { SCE_H_SGML_DEFAULT, 63237, L"SGML", L"fore:#881280", L"" },
                      { SCE_H_CDATA, 63147, L"CDATA", L"fore:#646464", L"" },
                      { MULTI_STYLE(SCE_H_ASP,SCE_H_ASPAT,0,0), 63146, L"ASP Start Tag", L"bold; fore:#000080", L"" },
                      //{ SCE_H_SCRIPT, L"Script", L"", L"" },
                      { SCE_H_QUESTION, 63148, L"PHP Start Tag", L"bold; fore:#000080", L"" },
                      { SCE_HPHP_DEFAULT, 63149, L"PHP Default", L"", L"" },
                      { MULTI_STYLE(SCE_HPHP_COMMENT,SCE_HPHP_COMMENTLINE,0,0), 63157, L"PHP Comment", L"fore:#FF8000", L"" },
                      { SCE_HPHP_WORD, 63152, L"PHP Keyword", L"bold; fore:#A46000", L"" },
                      { SCE_HPHP_HSTRING, 63150, L"PHP String", L"fore:#008000", L"" },
                      { SCE_HPHP_SIMPLESTRING, 63151, L"PHP Simple String", L"fore:#008000", L"" },
                      { SCE_HPHP_NUMBER, 63153, L"PHP Number", L"fore:#FF0000", L"" },
                      { SCE_HPHP_OPERATOR, 63158, L"PHP Operator", L"fore:#B000B0", L"" },
                      { SCE_HPHP_VARIABLE, 63154, L"PHP Variable", L"italic; fore:#000080", L"" },
                      { SCE_HPHP_HSTRING_VARIABLE, 63155, L"PHP String Variable", L"italic; fore:#000080", L"" },
                      { SCE_HPHP_COMPLEX_VARIABLE, 63156, L"PHP Complex Variable", L"italic; fore:#000080", L"" },
                      { MULTI_STYLE(SCE_HJ_DEFAULT,SCE_HJ_START,0,0), 63159, L"JS Default", L"", L"" },
                      { MULTI_STYLE(SCE_HJ_COMMENT,SCE_HJ_COMMENTLINE,SCE_HJ_COMMENTDOC,0), 63160, L"JS Comment", L"fore:#646464", L"" },
                      { SCE_HJ_KEYWORD, 63163, L"JS Keyword", L"bold; fore:#A46000", L"" },
                      { SCE_HJ_WORD, 63162, L"JS Identifier", L"", L"" },
                      { MULTI_STYLE(SCE_HJ_DOUBLESTRING,SCE_HJ_SINGLESTRING,SCE_HJ_STRINGEOL,0), 63164, L"JS String", L"fore:#008000", L"" },
                      { SCE_HJ_REGEX, 63166, L"JS Regex", L"fore:#006633; back:#FFF1A8", L"" },
                      { SCE_HJ_NUMBER, 63161, L"JS Number", L"fore:#FF0000", L"" },
                      { SCE_HJ_SYMBOLS, 63165, L"JS Symbols", L"fore:#B000B0", L"" },
                      { MULTI_STYLE(SCE_HJA_DEFAULT,SCE_HJA_START,0,0), 63167, L"ASP JS Default", L"", L"" },
                      { MULTI_STYLE(SCE_HJA_COMMENT,SCE_HJA_COMMENTLINE,SCE_HJA_COMMENTDOC,0), 63168, L"ASP JS Comment", L"fore:#646464", L"" },
                      { SCE_HJA_KEYWORD, 63171, L"ASP JS Keyword", L"bold; fore:#A46000", L"" },
                      { SCE_HJA_WORD, 63170, L"ASP JS Identifier", L"", L"" },
                      { MULTI_STYLE(SCE_HJA_DOUBLESTRING,SCE_HJA_SINGLESTRING,SCE_HJA_STRINGEOL,0), 63172, L"ASP JS String", L"fore:#008000", L"" },
                      { SCE_HJA_REGEX, 63174, L"ASP JS Regex", L"fore:#006633; back:#FFF1A8", L"" },
                      { SCE_HJA_NUMBER, 63169, L"ASP JS Number", L"fore:#FF0000", L"" },
                      { SCE_HJA_SYMBOLS, 63173, L"ASP JS Symbols", L"fore:#B000B0", L"" },
                      { MULTI_STYLE(SCE_HB_DEFAULT,SCE_HB_START,0,0), 63175, L"VBS Default", L"", L"" },
                      { SCE_HB_COMMENTLINE, 63176, L"VBS Comment", L"fore:#646464", L"" },
                      { SCE_HB_WORD, 63178, L"VBS Keyword", L"bold; fore:#B000B0", L"" },
                      { SCE_HB_IDENTIFIER, 63180, L"VBS Identifier", L"", L"" },
                      { MULTI_STYLE(SCE_HB_STRING,SCE_HB_STRINGEOL,0,0), 63179, L"VBS String", L"fore:#008000", L"" },
                      { SCE_HB_NUMBER, 63177, L"VBS Number", L"fore:#FF0000", L"" },
                      { MULTI_STYLE(SCE_HBA_DEFAULT,SCE_HBA_START,0,0), 63181, L"ASP VBS Default", L"", L"" },
                      { SCE_HBA_COMMENTLINE, 63182, L"ASP VBS Comment", L"fore:#646464", L"" },
                      { SCE_HBA_WORD, 63184, L"ASP VBS Keyword", L"bold; fore:#B000B0", L"" },
                      { SCE_HBA_IDENTIFIER, 63186, L"ASP VBS Identifier", L"", L"" },
                      { MULTI_STYLE(SCE_HBA_STRING,SCE_HBA_STRINGEOL,0,0), 63185, L"ASP VBS String", L"fore:#008000", L"" },
                      { SCE_HBA_NUMBER, 63183, L"ASP VBS Number", L"fore:#FF0000", L"" },
                      //{ SCE_HP_START, L"Phyton Start", L"", L"" },
                      //{ SCE_HP_DEFAULT, L"Phyton Default", L"", L"" },
                      //{ SCE_HP_COMMENTLINE, L"Phyton Comment Line", L"", L"" },
                      //{ SCE_HP_NUMBER, L"Phyton Number", L"", L"" },
                      //{ SCE_HP_STRING, L"Phyton String", L"", L"" },
                      //{ SCE_HP_CHARACTER, L"Phyton Character", L"", L"" },
                      //{ SCE_HP_WORD, L"Phyton Keyword", L"", L"" },
                      //{ SCE_HP_TRIPLE, L"Phyton Triple", L"", L"" },
                      //{ SCE_HP_TRIPLEDOUBLE, L"Phyton Triple Double", L"", L"" },
                      //{ SCE_HP_CLASSNAME, L"Phyton Class Name", L"", L"" },
                      //{ SCE_HP_DEFNAME, L"Phyton Def Name", L"", L"" },
                      //{ SCE_HP_OPERATOR, L"Phyton Operator", L"", L"" },
                      //{ SCE_HP_IDENTIFIER, L"Phyton Identifier", L"", L"" },
                      //{ SCE_HPA_START, L"ASP Phyton Start", L"", L"" },
                      //{ SCE_HPA_DEFAULT, L"ASP Phyton Default", L"", L"" },
                      //{ SCE_HPA_COMMENTLINE, L"ASP Phyton Comment Line", L"", L"" },
                      //{ SCE_HPA_NUMBER, L"ASP Phyton Number", L"", L"" },
                      //{ SCE_HPA_STRING, L"ASP Phyton String", L"", L"" },
                      //{ SCE_HPA_CHARACTER, L"ASP Phyton Character", L"", L"" },
                      //{ SCE_HPA_WORD, L"ASP Phyton Keyword", L"", L"" },
                      //{ SCE_HPA_TRIPLE, L"ASP Phyton Triple", L"", L"" },
                      //{ SCE_HPA_TRIPLEDOUBLE, L"ASP Phyton Triple Double", L"", L"" },
                      //{ SCE_HPA_CLASSNAME, L"ASP Phyton Class Name", L"", L"" },
                      //{ SCE_HPA_DEFNAME, L"ASP Phyton Def Name", L"", L"" },
                      //{ SCE_HPA_OPERATOR, L"ASP Phyton Operator", L"", L"" },
                      //{ SCE_HPA_IDENTIFIER, L"ASP Phyton Identifier", L"", L"" },
                      { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_XML = {
"", "", "", "", "", "", "", "", "" };


EDITLEXER lexXML = { SCLEX_XML, 63002, L"XML Document", L"xml; xsl; rss; svg; xul; xsd; xslt; axl; rdf; xaml; vcproj", L"", &KeyWords_XML, {
                     { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                     { MULTI_STYLE(SCE_H_TAG,SCE_H_TAGUNKNOWN,SCE_H_TAGEND,0), 63187, L"XML Tag", L"fore:#881280", L"" },
                     { MULTI_STYLE(SCE_H_ATTRIBUTE,SCE_H_ATTRIBUTEUNKNOWN,0,0), 63188, L"XML Attribute", L"fore:#994500", L"" },
                     { SCE_H_VALUE, 63189, L"XML Value", L"fore:#1A1AA6", L"" },
                     { MULTI_STYLE(SCE_H_DOUBLESTRING,SCE_H_SINGLESTRING,0,0), 63190, L"XML String", L"fore:#1A1AA6", L"" },
                     { SCE_H_OTHER, 63191, L"XML Other Inside Tag", L"fore:#1A1AA6", L"" },
                     { MULTI_STYLE(SCE_H_COMMENT,SCE_H_XCCOMMENT,0,0), 63192, L"XML Comment", L"fore:#646464", L"" },
                     { SCE_H_ENTITY, 63193, L"XML Entity", L"fore:#B000B0", L"" },
                     { SCE_H_DEFAULT, 63257, L"XML Element Text", L"", L"" },
                     { MULTI_STYLE(SCE_H_XMLSTART,SCE_H_XMLEND,0,0), 63145, L"XML Identifier", L"bold; fore:#881280", L"" },
                     { SCE_H_SGML_DEFAULT, 63237, L"SGML", L"fore:#881280", L"" },
                     { SCE_H_CDATA, 63147, L"CDATA", L"fore:#646464", L"" },
                     { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_CSS = {
"^-moz- ^-ms- ^-o- ^-webkit- alignment-adjust alignment-baseline animation animation-delay "
"animation-direction animation-duration animation-iteration-count animation-name "
"animation-play-state animation-timing-function appearance ascent azimuth backface-visibility "
"background background-attachment background-break background-clip background-color "
"background-image background-origin background-position background-repeat background-size "
"baseline baseline-shift bbox binding bleed bookmark-label bookmark-level bookmark-state "
"bookmark-target border border-bottom border-bottom-color border-bottom-left-radius "
"border-bottom-right-radius border-bottom-style border-bottom-width border-collapse border-color "
"border-image border-image-outset border-image-repeat border-image-slice border-image-source "
"border-image-width border-left border-left-color border-left-style border-left-width "
"border-length border-radius border-right border-right-color border-right-style "
"border-right-width border-spacing border-style border-top border-top-color "
"border-top-left-radius border-top-right-radius border-top-style border-top-width border-width "
"bottom box-align box-decoration-break box-direction box-flex box-flex-group box-lines "
"box-ordinal-group box-orient box-pack box-shadow box-sizing break-after break-before "
"break-inside cap-height caption-side centerline change-bar change-bar-class change-bar-offset "
"change-bar-side clear clip color color-profile column-count column-fill column-gap column-rule "
"column-rule-color column-rule-style column-rule-width columns column-span column-width content "
"counter-increment counter-reset crop cue cue-after cue-before cursor definition-src descent "
"direction display dominant-baseline drop-initial-after-adjust drop-initial-after-align "
"drop-initial-before-adjust drop-initial-before-align drop-initial-size drop-initial-value "
"elevation empty-cells fit fit-position float float-offset font font-family font-size "
"font-size-adjust font-stretch font-style font-variant font-weight grid-columns grid-rows "
"hanging-punctuation height hyphenate-after hyphenate-before hyphenate-character hyphenate-lines "
"hyphenate-resource hyphens icon image-orientation image-resolution inline-box-align "
"insert-position left letter-spacing line-height line-stacking line-stacking-ruby "
"line-stacking-shift line-stacking-strategy list-style list-style-image list-style-position "
"list-style-type make-element margin margin-bottom margin-left margin-right margin-top mark "
"mark-after mark-before marker-offset marks marquee-direction marquee-play-count marquee-speed "
"marquee-style mathline max-height max-width min-height min-width move-to nav-down nav-index "
"nav-left nav-right nav-up opacity orphans outline outline-color outline-offset outline-style "
"outline-width overflow overflow-style overflow-x overflow-y padding padding-bottom padding-left "
"padding-right padding-top page page-break-after page-break-before page-break-inside page-policy "
"panose-1 pause pause-after pause-before perspective perspective-origin phonemes pitch "
"pitch-range play-during position presentation-level prototype prototype-insert-policy "
"prototype-insert-position punctuation-trim quotes rendering-intent resize rest rest-after "
"rest-before richness right rotation rotation-point ruby-align ruby-overhang ruby-position "
"ruby-span size slope speak speak-header speak-numeral speak-punctuation speech-rate src stemh "
"stemv stress string-set table-layout target target-name target-new target-position text-align "
"text-align-last text-decoration text-emphasis text-height text-indent text-justify text-outline "
"text-replace text-shadow text-transform text-wrap top topline transform transform-origin "
"transform-style transition transition-delay transition-duration transition-property "
"transition-timing-function unicode-bidi unicode-range units-per-em vertical-align visibility "
"voice-balance voice-duration voice-family voice-pitch voice-pitch-range voice-rate voice-stress "
"voice-volume volume white-space white-space-collapse widows width widths word-break word-spacing "
"word-wrap x-height z-index",
"active after before checked default disabled empty enabled first first-child first-letter "
"first-line first-of-type focus hover indeterminate invalid lang last-child last-of-type left "
"link not nth-child nth-last-child nth-last-of-type nth-of-type only-child only-of-type optional "
"required right root target valid visited",
"", "", "", "", "", "", "" };


EDITLEXER lexCSS = { SCLEX_CSS, 63003, L"CSS Style Sheets", L"css", L"", &KeyWords_CSS, {
                     { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                     //{ SCE_CSS_DEFAULT, L"Default", L"", L"" },
                     { SCE_CSS_COMMENT, 63127, L"Comment", L"fore:#646464", L"" },
                     { SCE_CSS_TAG, 63136, L"HTML Tag", L"bold; fore:#0A246A", L"" },
                     { SCE_CSS_CLASS, 63194, L"Tag-Class", L"fore:#648000", L"" },
                     { SCE_CSS_ID, 63195, L"Tag-Id", L"fore:#648000", L"" },
                     { SCE_CSS_ATTRIBUTE, 63196, L"Tag-Attribute", L"italic; fore:#648000", L"" },
                     { MULTI_STYLE(SCE_CSS_PSEUDOCLASS,SCE_CSS_EXTENDED_PSEUDOCLASS,SCE_CSS_PSEUDOELEMENT,SCE_CSS_EXTENDED_PSEUDOELEMENT), 63197, L"Pseudo-class/element", L"fore:#B000B0", L"" },
                     { SCE_CSS_UNKNOWN_PSEUDOCLASS, 63198, L"Unknown Pseudo-class", L"fore:#C80000; back:#FFFF80", L"" },
                     { MULTI_STYLE(SCE_CSS_IDENTIFIER,SCE_CSS_IDENTIFIER2,SCE_CSS_IDENTIFIER3,SCE_CSS_EXTENDED_IDENTIFIER), 63199, L"CSS Property", L"fore:#FF4000", L"" },
                     { SCE_CSS_UNKNOWN_IDENTIFIER, 63200, L"Unknown Property", L"fore:#C80000; back:#FFFF80", L"" },
                     { MULTI_STYLE(SCE_CSS_DOUBLESTRING,SCE_CSS_SINGLESTRING,0,0), 63131, L"String", L"fore:#008000", L"" },
                     { SCE_CSS_VALUE, 63201, L"Value", L"fore:#3A6EA5", L"" },
                     { SCE_CSS_OPERATOR, 63132, L"Operator", L"fore:#B000B0", L"" },
                     { SCE_CSS_IMPORTANT, 63202, L"Important", L"bold; fore:#C80000", L"" },
                     { SCE_CSS_DIRECTIVE, 63203, L"Directive", L"bold; fore:#000000; back:#FFF1A8", L"" },
                     { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_CPP = {
"__abstract __alignof __asm __assume __based __box __cdecl __declspec __delegate __event "
"__except __except__try __fastcall __finally __forceinline __gc __hook __identifier "
"__if_exists __if_not_exists __inline __int16 __int32 __int64 __int8 __interface __leave "
"__m128 __m128d __m128i __m64 __multiple_inheritance __nogc __noop __pin __property __raise "
"__sealed __single_inheritance __stdcall __super __try __try_cast __unhook __uuidof __value "
"__virtual_inheritance __wchar_t auto bool break case catch char class const const_cast "
"continue default defined delete do double dynamic_cast else enum explicit extern false float "
"for friend goto if inline int long mutable naked namespace new operator private protected "
"public register reinterpret_cast return short signed size_t sizeof static static_cast struct "
"switch template this throw true try typedef typeid typename union unsigned using uuid "
"virtual void volatile wchar_t while",
"",
"", "", "", "", "", "", "" };


EDITLEXER lexCPP = { SCLEX_CPP, 63004, L"C/C++ Source Code", L"c; cpp; cxx; cc; h; hpp; hxx; hh; m; mm; idl; inl; odl", L"", &KeyWords_CPP, {
                     { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                     //{ SCE_C_DEFAULT, L"Default", L"", L"" },
                     { SCE_C_COMMENT, 63127, L"Comment", L"fore:#008000", L"" },
                     { SCE_C_WORD, 63128, L"Keyword", L"bold; fore:#0A246A", L"" },
                     { SCE_C_IDENTIFIER, 63129, L"Identifier", L"", L"" },
                     { MULTI_STYLE(SCE_C_STRING,SCE_C_CHARACTER,SCE_C_STRINGEOL,SCE_C_VERBATIM), 63131, L"String", L"fore:#008000", L"" },
                     { SCE_C_NUMBER, 63130, L"Number", L"fore:#FF0000", L"" },
                     { SCE_C_OPERATOR, 63132, L"Operator", L"fore:#B000B0", L"" },
                     { SCE_C_PREPROCESSOR, 63133, L"Preprocessor", L"fore:#FF8000", L"" },
                     //{ SCE_C_UUID, L"UUID", L"", L"" },
                     //{ SCE_C_REGEX, L"Regex", L"", L"" },
                     //{ SCE_C_WORD2, L"Word 2", L"", L"" },
                     //{ SCE_C_GLOBALCLASS, L"Global Class", L"", L"" },
                     { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_CS = {
"abstract as ascending base bool break by byte case catch char checked "
"class const continue decimal default delegate descending do double else "
"enum equals event explicit extern false finally fixed float for foreach "
"from goto group if implicit in int interface internal into is join lock "
"let long namespace new null object on operator orderby out override params "
"private protected public readonly ref return sbyte sealed select short "
"sizeof stackalloc static string struct switch this throw true try typeof "
"uint ulong unchecked unsafe ushort using var virtual void where while",
"",
"", "", "", "", "", "", "" };


EDITLEXER lexCS = { SCLEX_CPP, 63005, L"C# Source Code", L"cs", L"", &KeyWords_CS, {
                    { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                    //{ SCE_C_DEFAULT, L"Default", L"", L"" },
                    { SCE_C_COMMENT, 63127, L"Comment", L"fore:#008000", L"" },
                    { SCE_C_WORD, 63128, L"Keyword", L"bold; fore:#804000", L"" },
                    { SCE_C_IDENTIFIER, 63129, L"Identifier", L"", L"" },
                    { MULTI_STYLE(SCE_C_STRING,SCE_C_CHARACTER,SCE_C_STRINGEOL,0), 63131, L"String", L"fore:#008000", L"" },
                    { SCE_C_VERBATIM, 63134, L"Verbatim String", L"fore:#008000", L"" },
                    { SCE_C_NUMBER, 63130, L"Number", L"fore:#FF0000", L"" },
                    { SCE_C_OPERATOR, 63132, L"Operator", L"fore:#B000B0", L"" },
                    { SCE_C_PREPROCESSOR, 63133, L"Preprocessor", L"fore:#FF8000", L"" },
                    //{ SCE_C_UUID, L"UUID", L"", L"" },
                    //{ SCE_C_REGEX, L"Regex", L"", L"" },
                    //{ SCE_C_WORD2, L"Word 2", L"", L"" },
                    //{ SCE_C_GLOBALCLASS, L"Global Class", L"", L"" },
                    { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_RC = {
"ACCELERATORS ALT AUTO3STATE AUTOCHECKBOX AUTORADIOBUTTON "
"BEGIN BITMAP BLOCK BUTTON CAPTION CHARACTERISTICS CHECKBOX "
"CLASS COMBOBOX CONTROL CTEXT CURSOR DEFPUSHBUTTON DIALOG "
"DIALOGEX DISCARDABLE EDITTEXT END EXSTYLE FONT GROUPBOX "
"ICON LANGUAGE LISTBOX LTEXT MENU MENUEX MENUITEM "
"MESSAGETABLE POPUP PUSHBUTTON RADIOBUTTON RCDATA RTEXT "
"SCROLLBAR SEPARATOR SHIFT STATE3 STRINGTABLE STYLE "
"TEXTINCLUDE VALUE VERSION VERSIONINFO VIRTKEY",
"", "", "", "", "", "", "", "" };


EDITLEXER lexRC = { SCLEX_CPP, 63006, L"Resource Script", L"rc; rc2; rct; rh; r; dlg", L"", &KeyWords_RC, {
                    { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                    //{ SCE_C_DEFAULT, L"Default", L"", L"" },
                    { SCE_C_COMMENT, 63127, L"Comment", L"fore:#008000", L"" },
                    { SCE_C_WORD, 63128, L"Keyword", L"bold; fore:#0A246A", L"" },
                    { SCE_C_IDENTIFIER, 63129, L"Identifier", L"", L"" },
                    { MULTI_STYLE(SCE_C_STRING,SCE_C_CHARACTER,SCE_C_STRINGEOL,SCE_C_VERBATIM), 63131, L"String", L"fore:#008000", L"" },
                    { SCE_C_NUMBER, 63130, L"Number", L"fore:#FF0000", L"" },
                    { SCE_C_OPERATOR, 63132, L"Operator", L"fore:#0A246A", L"" },
                    { SCE_C_PREPROCESSOR, 63133, L"Preprocessor", L"fore:#FF8000", L"" },
                    //{ SCE_C_UUID, L"UUID", L"", L"" },
                    //{ SCE_C_REGEX, L"Regex", L"", L"" },
                    //{ SCE_C_WORD2, L"Word 2", L"", L"" },
                    //{ SCE_C_GLOBALCLASS, L"Global Class", L"", L"" },
                    { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_MAK = {
"", "", "", "", "", "", "", "", "" };


EDITLEXER lexMAK = { SCLEX_MAKEFILE, 63007, L"Makefiles", L"mak; make; mk; dsp; msc; msvc", L"", &KeyWords_MAK, {
                     { STYLE_DEFAULT, 63126, L"Default", L"fore:#0A246A", L"" },
                     //{ SCE_MAKE_DEFAULT, L"Default", L"", L"" },
                     { SCE_MAKE_COMMENT, 63127, L"Comment", L"fore:#008000", L"" },
                     { MULTI_STYLE(SCE_MAKE_IDENTIFIER,SCE_MAKE_IDEOL,0,0), 63129, L"Identifier", L"fore:#003CE6", L"" },
                     { SCE_MAKE_OPERATOR, 63132, L"Operator", L"", L"" },
                     { SCE_MAKE_TARGET, 63204, L"Target", L"fore:#003CE6; back:#FFC000", L"" },
                     { SCE_MAKE_PREPROCESSOR, 63133, L"Preprocessor", L"fore:#FF8000", L"" },
                     { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_VBS = {
"alias and as attribute begin boolean byref byte byval call case class compare const continue "
"currency date declare dim do double each else elseif empty end enum eqv erase error event exit "
"explicit false for friend function get global gosub goto if imp implement in integer is let lib "
"load long loop lset me mid mod module new next not nothing null object on option optional or "
"preserve private property public raiseevent redim rem resume return rset select set single "
"static stop string sub then to true type unload until variant wend while with withevents xor",
"", "", "", "", "", "", "", "" };


EDITLEXER lexVBS = { SCLEX_VBSCRIPT, 63008, L"VBScript", L"vbs; dsm", L"", &KeyWords_VBS, {
                    { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                    //{ SCE_B_DEFAULT, L"Default", L"", L"" },
                    { SCE_B_COMMENT, 63127, L"Comment", L"fore:#808080", L"" },
                    { SCE_B_KEYWORD, 63128, L"Keyword", L"bold; fore:#B000B0", L"" },
                    { SCE_B_IDENTIFIER, 63129, L"Identifier", L"", L"" },
                    { MULTI_STYLE(SCE_B_STRING,SCE_B_STRINGEOL,0,0), 63131, L"String", L"fore:#008000", L"" },
                    { SCE_B_NUMBER, 63130, L"Number", L"fore:#FF0000", L"" },
                    { SCE_B_OPERATOR, 63132, L"Operator", L"", L"" },
                    //{ SCE_B_PREPROCESSOR, 63133, L"Preprocessor", L"fore:#FF9C00", L"" },
                    //{ SCE_B_CONSTANT, L"Constant", L"", L"" },
                    //{ SCE_B_DATE, L"Date", L"", L"" },
                    //{ SCE_B_KEYWORD2, L"Keyword 2", L"", L"" },
                    //{ SCE_B_KEYWORD3, L"Keyword 3", L"", L"" },
                    //{ SCE_B_KEYWORD4, L"Keyword 4", L"", L"" },
                    //{ SCE_B_ASM, L"Inline Asm", L"fore:#FF8000", L"" },
                    { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_VB = {
"addhandler addressof alias and andalso ansi any as assembly auto boolean byref byte byval call "
"case catch cbool cbyte cchar cdate cdbl cdec char cint class clng cobj compare const cshort csng "
"cstr ctype date decimal declare default delegate dim directcast do double each else elseif end "
"enum erase error event exit explicit externalsource false finally for friend function get "
"gettype gosub goto handles if implements imports in inherits integer interface is let lib like "
"long loop me mid mod module mustinherit mustoverride mybase myclass namespace new next not "
"nothing notinheritable notoverridable object on option optional or orelse overloads overridable "
"overrides paramarray preserve private property protected public raiseevent randomize readonly "
"redim rem removehandler resume return select set shadows shared short single static step stop "
"strict string structure sub synclock then throw to true try typeof unicode until variant when "
"while with withevents writeonly xor",
"", "", "", "", "", "", "", "" };


EDITLEXER lexVB = { SCLEX_VB, 63009, L"Visual Basic", L"vb; bas; frm; cls; ctl; pag; dsr; dob", L"", &KeyWords_VB, {
                    { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                    //{ SCE_B_DEFAULT, L"Default", L"", L"" },
                    { SCE_B_COMMENT, 63127, L"Comment", L"fore:#808080", L"" },
                    { SCE_B_KEYWORD, 63128, L"Keyword", L"bold; fore:#B000B0", L"" },
                    { SCE_B_IDENTIFIER, 63129, L"Identifier", L"", L"" },
                    { MULTI_STYLE(SCE_B_STRING,SCE_B_STRINGEOL,0,0), 63131, L"String", L"fore:#008000", L"" },
                    { MULTI_STYLE(SCE_B_NUMBER,SCE_B_DATE,0,0), 63130, L"Number", L"fore:#FF0000", L"" },
                    { SCE_B_OPERATOR, 63132, L"Operator", L"", L"" },
                    { SCE_B_PREPROCESSOR, 63133, L"Preprocessor", L"fore:#FF9C00", L"" },
                    //{ SCE_B_CONSTANT, L"Constant", L"", L"" },
                    //{ SCE_B_KEYWORD2, L"Keyword 2", L"", L"" },
                    //{ SCE_B_KEYWORD3, L"Keyword 3", L"", L"" },
                    //{ SCE_B_KEYWORD4, L"Keyword 4", L"", L"" },
                    //{ SCE_B_ASM, L"Inline Asm", L"fore:#FF8000", L"" },
                    { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_JS = {
"abstract boolean break byte case catch char class const continue debugger default delete do "
"double else enum export extends false final finally float for function goto if implements "
"import in instanceof int interface long native new null package private protected public "
"return short static super switch synchronized this throw throws transient true try typeof var "
"void volatile while with",
"", "", "", "", "", "", "", "" };


EDITLEXER lexJS = { SCLEX_CPP, 63010, L"JavaScript", L"js; jse; jsm; json; as", L"", &KeyWords_JS, {
                    { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                    //{ SCE_C_DEFAULT, L"Default", L"", L"" },
                    { SCE_C_COMMENT, 63127, L"Comment", L"fore:#646464", L"" },
                    { SCE_C_WORD, 63128, L"Keyword", L"bold; fore:#A46000", L"" },
                    { SCE_C_IDENTIFIER, 63129, L"Identifier", L"", L"" },
                    { MULTI_STYLE(SCE_C_STRING,SCE_C_CHARACTER,SCE_C_STRINGEOL,SCE_C_VERBATIM), 63131, L"String", L"fore:#008000", L"" },
                    { SCE_C_REGEX, 63135, L"Regex", L"fore:#006633; back:#FFF1A8", L"" },
                    { SCE_C_NUMBER, 63130, L"Number", L"fore:#FF0000", L"" },
                    { SCE_C_OPERATOR, 63132, L"Operator", L"fore:#B000B0", L"" },
                    //{ SCE_C_UUID, L"UUID", L"", L"" },
                    //{ SCE_C_PREPROCESSOR, L"Preprocessor", L"fore:#FF8000", L"" },
                    //{ SCE_C_WORD2, L"Word 2", L"", L"" },
                    //{ SCE_C_GLOBALCLASS, L"Global Class", L"", L"" },
                    { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_JAVA = {
"abstract assert boolean break byte case catch char class const "
"continue default do double else extends final finally float for future "
"generic goto if implements import inner instanceof int interface long "
"native new null outer package private protected public rest return "
"short static super switch synchronized this throw throws transient try "
"var void volatile while",
"", "", "", "", "", "", "", "" };


EDITLEXER lexJAVA = { SCLEX_CPP, 63011, L"Java Source Code", L"java", L"", &KeyWords_JAVA, {
                      { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                      //{ SCE_C_DEFAULT, L"Default", L"", L"" },
                      { SCE_C_COMMENT, 63127, L"Comment", L"fore:#646464", L"" },
                      { SCE_C_WORD, 63128, L"Keyword", L"bold; fore:#A46000", L"" },
                      { SCE_C_IDENTIFIER, 63129, L"Identifier", L"", L"" },
                      { MULTI_STYLE(SCE_C_STRING,SCE_C_CHARACTER,SCE_C_STRINGEOL,SCE_C_VERBATIM), 63131, L"String", L"fore:#008000", L"" },
                      { SCE_C_REGEX, 63135, L"Regex", L"fore:#006633; back:#FFF1A8", L"" },
                      { SCE_C_NUMBER, 63130, L"Number", L"fore:#FF0000", L"" },
                      { SCE_C_OPERATOR, 63132, L"Operator", L"fore:#B000B0", L"" },
                      //{ SCE_C_UUID, L"UUID", L"", L"" },
                      //{ SCE_C_PREPROCESSOR, L"Preprocessor", L"fore:#FF8000", L"" },
                      //{ SCE_C_WORD2, L"Word 2", L"", L"" },
                      //{ SCE_C_GLOBALCLASS, L"Global Class", L"", L"" },
                      { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_PAS = {
"absolute abstract alias and array as asm assembler begin break case cdecl class const constructor continue cppdecl default "
"destructor dispose div do downto else end end. except exit export exports external false far far16 file finalization finally for "
"forward function goto if implementation in index inherited initialization inline interface is label library local message mod "
"name near new nil nostackframe not object of oldfpccall on operator or out overload override packed pascal private procedure "
"program property protected public published raise read record register reintroduce repeat resourcestring safecall self set shl "
"shr softfloat stdcall stored string then threadvar to true try type unit until uses var virtual while with write xor",
"", "", "", "", "", "", "", "" };


EDITLEXER lexPAS = { SCLEX_PASCAL, 63012, L"Pascal Source Code", L"pas; dpr; dpk; dfm; inc; pp", L"", &KeyWords_PAS, {
                     { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                     //{ SCE_PAS_DEFAULT, L"Default", L"", L"" },
                     { MULTI_STYLE(SCE_PAS_COMMENT,SCE_PAS_COMMENT2,SCE_PAS_COMMENTLINE,0), 63127, L"Comment", L"fore:#646464", L"" },
                     { SCE_PAS_WORD, 63128, L"Keyword", L"bold; fore:#800080", L"" },
                     { SCE_PAS_IDENTIFIER, 63129, L"Identifier", L"", L"" },
                     { MULTI_STYLE(SCE_PAS_STRING,SCE_PAS_CHARACTER,SCE_PAS_STRINGEOL,0), 63131, L"String", L"fore:#008000", L"" },
                     { MULTI_STYLE(SCE_PAS_NUMBER,SCE_PAS_HEXNUMBER,0,0), 63130, L"Number", L"fore:#FF0000", L"" },
                     { SCE_PAS_OPERATOR, 63132, L"Operator", L"bold", L"" },
                     { SCE_PAS_ASM, 63205, L"Inline Asm", L"fore:#0000FF", L"" },
                     { MULTI_STYLE(SCE_PAS_PREPROCESSOR,SCE_PAS_PREPROCESSOR2,0,0), 63133, L"Preprocessor", L"fore:#FF00FF", L"" },
                     { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_ASM = {
"aaa aad aam aas adc add and arpl bound bsf bsr bswap bt btc btr bts call cbw cdq cflush clc cld "
"cli clts cmc cmova cmovae cmovb cmovbe cmovc cmove cmovg cmovge cmovl cmovle cmovna cmovnae "
"cmovnb cmovnbe cmovnc cmovne cmovng cmovnge cmovnl cmovnle cmovno cmovnp cmovns cmovnz cmovo "
"cmovp cmovpe cmovpo cmovs cmovz cmp cmps cmpsb cmpsd cmpsq cmpsw cmpxchg cmpxchg486 cmpxchg8b "
"cpuid cwd cwde daa das dec div emms enter esc femms hlt ibts icebp idiv imul in inc ins insb "
"insd insw int int01 int03 int1 int3 into invd invlpg iret iretd iretdf iretf iretw ja jae jb jbe "
"jc jcxz je jecxz jg jge jl jle jmp jna jnae jnb jnbe jnc jne jng jnge jnl jnle jno jnp jns jnz "
"jo jp jpe jpo js jz lahf lar lds lea leave les lfs lgdt lgs lidt lldt lmsw loadall loadall286 "
"lock lods lodsb lodsd lodsq lodsw loop loopd loope looped loopew loopne loopned loopnew loopnz "
"loopnzd loopnzw loopw loopz loopzd loopzw lsl lss ltr mov movs movsb movsd movsq movsw movsx "
"movsxd movzx mul neg nop not or out outs outsb outsd outsw pop popa popad popaw popf popfd popfw "
"push pusha pushad pushaw pushd pushf pushfd pushfw pushw rcl rcr rdmsr rdpmc rdshr rdtsc rep "
"repe repne repnz repz ret retf retn rol ror rsdc rsldt rsm rsts sahf sal salc sar sbb scas scasb "
"scasd scasq scasw seta setae setb setbe setc sete setg setge setl setle setna setnae setnb "
"setnbe setnc setne setng setnge setnl setnle setno setnp setns setnz seto setp setpe setpo sets "
"setz sgdt shl shld shr shrd sidt sldt smi smint smintold smsw stc std sti stos stosb stosd stosq "
"stosw str sub svdc svldt svts syscall sysenter sysexit sysret test ud0 ud1 ud2 umov verr verw "
"wait wbinvd wrmsr wrshr xadd xbts xchg xlat xlatb xor",
"f2xm1 fabs fadd faddp fbld fbstp fchs fclex fcmovb fcmovbe fcmove fcmovnb fcmovnbe fcmovne "
"fcmovnu fcmovu fcom fcomi fcomip fcomp fcompp fcos fdecstp fdisi fdiv fdivp fdivr fdivrp feni "
"ffree ffreep fiadd ficom ficomp fidiv fidivr fild fimul fincstp finit fist fistp fisub fisubr "
"fld fld1 fldcw fldenv fldenvd fldenvw fldl2e fldl2t fldlg2 fldln2 fldpi fldz fmul fmulp fnclex "
"fndisi fneni fninit fnop fnsave fnsaved fnsavew fnstcw fnstenv fnstenvd fnstenvw fnstsw fpatan "
"fprem fprem1 fptan frndint frstor frstord frstorw fsave fsaved fsavew fscale fsetpm fsin fsincos "
"fsqrt fst fstcw fstenv fstenvd fstenvw fstp fstsw fsub fsubp fsubr fsubrp ftst fucom fucomp "
"fucompp fwait fxam fxch fxtract fyl2x fyl2xp1",
"ah al ax bh bl bp bx ch cl cr0 cr2 cr3 cr4 cs cx dh di dl dr0 dr1 dr2 dr3 dr6 dr7 ds dx eax ebp "
"ebx ecx edi edx eip es esi esp fs gs mm0 mm1 mm2 mm3 mm4 mm5 mm6 mm7 r10 r10b r10d r10w r11 r11b "
"r11d r11w r12 r12b r12d r12w r13 r13b r13d r13w r14 r14b r14d r14w r15 r15b r15d r15w r8 r8b r8d "
"r8w r9 r9b r9d r9w rax rbp rbx rcx rdi rdx rip rsi rsp si sp ss st st0 st1 st2 st3 st4 st5 st6 "
"st7 tr3 tr4 tr5 tr6 tr7 xmm0 xmm1 xmm10 xmm11 xmm12 xmm13 xmm14 xmm15 xmm2 xmm3 xmm4 xmm5 xmm6 "
"xmm7 xmm8 xmm9 ymm0 ymm1 ymm10 ymm11 ymm12 ymm13 ymm14 ymm15 ymm2 ymm3 ymm4 ymm5 ymm6 ymm7 ymm8 "
"ymm9",
"%arg %assign %define %elif %elifctk %elifdef %elifid %elifidn %elifidni %elifmacro %elifnctk "
"%elifndef %elifnid %elifnidn %elifnidni %elifnmacro %elifnnum %elifnstr %elifnum %elifstr %else "
"%endif %endmacro %endrep %error %exitrep %iassign %idefine %if %ifctk %ifdef %ifid %ifidn "
"%ifidni %ifmacro %ifnctk %ifndef %ifnid %ifnidn %ifnidni %ifnmacro %ifnnum %ifnstr %ifnum %ifstr "
"%imacro %include %line %local %macro %out %pop %push %rep %repl %rotate %stacksize %strlen "
"%substr %undef %xdefine %xidefine .186 .286 .286c .286p .287 .386 .386c .386p .387 .486 .486p "
".8086 .8087 .alpha .break .code .const .continue .cref .data .data? .dosseg .else .elseif .endif "
".endw .err .err1 .err2 .errb .errdef .errdif .errdifi .erre .erridn .erridni .errnb .errndef "
".errnz .exit .fardata .fardata? .if .lall .lfcond .list .listall .listif .listmacro "
".listmacroall .model .msfloat .no87 .nocref .nolist .nolistif .nolistmacro .radix .repeat .sall "
".seq .sfcond .stack .startup .tfcond .type .until .untilcxz .while .xall .xcref .xlist absolute "
"alias align alignb assume at bits catstr comm comment common cpu db dd df dosseg dq dt dup dw "
"echo else elseif elseif1 elseif2 elseifb elseifdef elseifdif elseifdifi elseife elseifidn "
"elseifidni elseifnb elseifndef end endif endm endp ends endstruc eq equ even exitm export extern "
"externdef extrn for forc ge global goto group gt high highword iend if if1 if2 ifb ifdef ifdif "
"ifdifi ife ifidn ifidni ifnb ifndef import incbin include includelib instr invoke irp irpc "
"istruc label le length lengthof local low lowword lroffset lt macro mask mod name ne offset "
"opattr option org page popcontext proc proto ptr public purge pushcontext record repeat rept "
"resb resd resq rest resw section seg segment short size sizeof sizestr struc struct substr "
"subtitle subttl textequ this times title type typedef union use16 use32 while width",
"$ $$ %0 %1 %2 %3 %4 %5 %6 %7 %8 %9 .bss .data .text ? @b @f a16 a32 abs addr all assumes at "
"basic byte c carry? casemap common compact cpu dotname dword emulator epilogue error export "
"expr16 expr32 far far16 far32 farstack flat forceframe fortran fword huge language large listing "
"ljmp loadds m510 medium memory near near16 near32 nearstack nodotname noemulator nokeyword "
"noljmp nom510 none nonunique nooldmacros nooldstructs noreadonly noscoped nosignextend nosplit "
"nothing notpublic o16 o32 oldmacros oldstructs os_dos overflow? para parity? pascal private "
"prologue qword radix readonly real10 real4 real8 req sbyte scoped sdword seq setif2 sign? small "
"smallstack stdcall sword syscall tbyte tiny use16 use32 uses vararg word wrt zero?",
"addpd addps addsd addss andnpd andnps andpd andps blendpd blendps blendvpd blendvps cmpeqpd "
"cmpeqps cmpeqsd cmpeqss cmplepd cmpleps cmplesd cmpless cmpltpd cmpltps cmpltsd cmpltss cmpnepd "
"cmpneps cmpnesd cmpness cmpnlepd cmpnleps cmpnlesd cmpnless cmpnltpd cmpnltps cmpnltsd cmpnltss "
"cmpordpd cmpordps cmpordsd cmpordss cmpunordpd cmpunordps cmpunordsd cmpunordss comisd comiss "
"crc32 cvtdq2pd cvtdq2ps cvtpd2dq cvtpd2pi cvtpd2ps cvtpi2pd cvtpi2ps cvtps2dq cvtps2pd cvtps2pi "
"cvtsd2si cvtsd2ss cvtsi2sd cvtsi2ss cvtss2sd cvtss2si cvttpd2dq cvttpd2pi cvttps2dq cvttps2pi "
"cvttsd2si cvttss2si divpd divps divsd divss dppd dpps extractps fxrstor fxsave insertps ldmxscr "
"lfence maskmovdq maskmovdqu maxpd maxps maxss mfence minpd minps minsd minss movapd movaps movd "
"movdq2q movdqa movdqu movhlps movhpd movhps movlhps movlpd movlps movmskpd movmskps movntdq "
"movntdqa movnti movntpd movntps movntq movq movq2dq movsd movss movupd movups mpsadbw mulpd "
"mulps mulsd mulss orpd orps packssdw packsswb packusdw packuswb paddb paddd paddq paddsb paddsiw "
"paddsw paddusb paddusw paddw pand pandn pause paveb pavgb pavgusb pavgw paxsd pblendvb pblendw "
"pcmpeqb pcmpeqd pcmpeqq pcmpeqw pcmpestri pcmpestrm pcmpgtb pcmpgtd pcmpgtq pcmpgtw pcmpistri "
"pcmpistrm pdistib pextrb pextrd pextrq pextrw pf2id pf2iw pfacc pfadd pfcmpeq pfcmpge pfcmpgt "
"pfmax pfmin pfmul pfnacc pfpnacc pfrcp pfrcpit1 pfrcpit2 pfrsqit1 pfrsqrt pfsub pfsubr "
"phminposuw pi2fd pinsrb pinsrd pinsrq pinsrw pmachriw pmaddwd pmagw pmaxsb pmaxsd pmaxsw pmaxub "
"pmaxud pmaxuw pminsb pminsd pminsw pminub pminud pminuw pmovmskb pmovsxbd pmovsxbq pmovsxbw "
"pmovsxdq pmovsxwd pmovsxwq pmovzxbd pmovzxbq pmovzxbw pmovzxdq pmovzxwd pmovzxwq pmuldq pmulhriw "
"pmulhrwa pmulhrwc pmulhuw pmulhw pmulld pmullw pmuludq pmvgezb pmvlzb pmvnzb pmvzb popcnt por "
"prefetch prefetchnta prefetcht0 prefetcht1 prefetcht2 prefetchw psadbw pshufd pshufhw pshuflw "
"pshufw pslld pslldq psllq psllw psrad psraw psrld psrldq psrlq psrlw psubb psubd psubq psubsb "
"psubsiw psubsw psubusb psubusw psubw pswapd ptest punpckhbw punpckhdq punpckhqdq punpckhwd "
"punpcklbw punpckldq punpcklqdq punpcklwd pxor rcpps rcpss roundpd roundps roundsd roundss "
"rsqrtps rsqrtss sfence shufpd shufps sqrtpd sqrtps sqrtsd sqrtss stmxcsr subpd subps subsd subss "
"ucomisd ucomiss unpckhpd unpckhps unpcklpd unpcklps xorpd xorps",
"", "", "" };


EDITLEXER lexASM = { SCLEX_ASM, 63013, L"Assembly Script", L"asm", L"", &KeyWords_ASM, {
                     { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                     //{ SCE_ASM_DEFAULT, L"Default", L"", L"" },
                     { MULTI_STYLE(SCE_ASM_COMMENT,SCE_ASM_COMMENTBLOCK,0,0), 63127, L"Comment", L"fore:#008000", L"" },
                     { SCE_ASM_IDENTIFIER, 63129, L"Identifier", L"", L"" },
                     { MULTI_STYLE(SCE_ASM_STRING,SCE_ASM_CHARACTER,SCE_ASM_STRINGEOL,0), 63131, L"String", L"fore:#008000", L"" },
                     { SCE_ASM_NUMBER, 63130, L"Number", L"fore:#FF0000", L"" },
                     { SCE_ASM_OPERATOR, 63132, L"Operator", L"fore:#0A246A", L"" },
                     { SCE_ASM_CPUINSTRUCTION, 63206, L"CPU Instruction", L"fore:#0A246A", L"" },
                     { SCE_ASM_MATHINSTRUCTION, 63207, L"FPU Instruction", L"fore:#0A246A", L"" },
                     { SCE_ASM_EXTINSTRUCTION, 63210, L"Extended Instruction", L"fore:#0A246A", L"" },
                     { SCE_ASM_DIRECTIVE, 63203, L"Directive", L"fore:#0A246A", L"" },
                     { SCE_ASM_DIRECTIVEOPERAND, 63209, L"Directive Operand", L"fore:#0A246A", L"" },
                     { SCE_ASM_REGISTER, 63208, L"Register", L"fore:#FF8000", L"" },
                     { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_PL = {
"__DATA__ __END__ __FILE__ __LINE__ __PACKAGE__ abs accept alarm and atan2 AUTOLOAD BEGIN "
"bind binmode bless caller chdir CHECK chmod chomp chop chown chr chroot close closedir cmp "
"connect continue CORE cos crypt dbmclose dbmopen default defined delete DESTROY die do "
"dump each else elsif END endgrent endhostent endnetent endprotoent endpwent endservent eof "
"eq EQ eval exec exists exit exp fcntl fileno flock for foreach fork format formline ge GE "
"getc getgrent getgrgid getgrnam gethostbyaddr gethostbyname gethostent getlogin "
"getnetbyaddr getnetbyname getnetent getpeername getpgrp getppid getpriority getprotobyname "
"getprotobynumber getprotoent getpwent getpwnam getpwuid getservbyname getservbyport "
"getservent getsockname getsockopt given glob gmtime goto grep gt GT hex if index INIT int "
"ioctl join keys kill last lc lcfirst le LE length link listen local localtime lock log "
"lstat lt LT map mkdir msgctl msgget msgrcv msgsnd my ne NE next no not NULL oct open "
"opendir or ord our pack package pipe pop pos print printf prototype push qu quotemeta rand "
"read readdir readline readlink readpipe recv redo ref rename require reset return reverse "
"rewinddir rindex rmdir say scalar seek seekdir select semctl semget semop send setgrent "
"sethostent setnetent setpgrp setpriority setprotoent setpwent setservent setsockopt shift "
"shmctl shmget shmread shmwrite shutdown sin sleep socket socketpair sort splice split "
"sprintf sqrt srand stat state study sub substr symlink syscall sysopen sysread sysseek "
"system syswrite tell telldir tie tied time times truncate uc ucfirst umask undef UNITCHECK "
"unless unlink unpack unshift untie until use utime values vec wait waitpid wantarray warn "
"when while write xor",
"", "", "", "", "", "", "", "" };


EDITLEXER lexPL = { SCLEX_PERL, 63014, L"Perl Script", L"pl; pm; cgi; pod", L"", &KeyWords_PL, {
                    { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                    //{ SCE_PL_DEFAULT, L"Default", L"", L"" },
                    { SCE_PL_COMMENTLINE, 63127, L"Comment", L"fore:#646464", L"" },
                    { SCE_PL_WORD, 63128, L"Keyword", L"bold; fore:#804000", L"" },
                    { SCE_PL_IDENTIFIER, 63129, L"Identifier", L"", L"" },
                    { SCE_PL_STRING, 63211, L"String double quoted", L"fore:#008000", L"" },
                    { SCE_PL_CHARACTER, 63212, L"String single quoted", L"fore:#008000", L"" },
                    { SCE_PL_NUMBER, 63130, L"Number", L"fore:#FF0000", L"" },
                    { SCE_PL_OPERATOR, 63132, L"Operator", L"bold", L"" },
                    { SCE_PL_SCALAR, 63215, L"Scalar $var", L"fore:#0A246A", L"" },
                    { SCE_PL_ARRAY, 63216, L"Array @var", L"fore:#003CE6", L"" },
                    { SCE_PL_HASH, 63217, L"Hash %var", L"fore:#B000B0", L"" },
                    { SCE_PL_SYMBOLTABLE, 63218, L"Symbol table *var", L"fore:#3A6EA5", L"" },
                    { SCE_PL_REGEX, 63219, L"Regex /re/ or m{re}", L"fore:#006633; back:#FFF1A8", L"" },
                    { SCE_PL_REGSUBST, 63220, L"Substitution s/re/ore/", L"fore:#006633; back:#FFF1A8", L"" },
                    { SCE_PL_BACKTICKS, 63221, L"Back ticks", L"fore:#E24000; back:#FFF1A8", L"" },
                    { SCE_PL_HERE_DELIM, 63223, L"Here-doc (delimiter)", L"fore:#648000", L"" },
                    { SCE_PL_HERE_Q, 63224, L"Here-doc (single quoted, q)", L"fore:#648000", L"" },
                    { SCE_PL_HERE_QQ, 63225, L"Here-doc (double quoted, qq)", L"fore:#648000", L"" },
                    { SCE_PL_HERE_QX, 63226, L"Here-doc (back ticks, qx)", L"fore:#E24000; back:#FFF1A8", L"" },
                    { SCE_PL_STRING_Q, 63227, L"Single quoted string (generic, q)", L"fore:#008000", L"" },
                    { SCE_PL_STRING_QQ, 63228, L"Double quoted string (qq)", L"fore:#008000", L"" },
                    { SCE_PL_STRING_QX, 63229, L"Back ticks (qx)", L"fore:#E24000; back:#FFF1A8", L"" },
                    { SCE_PL_STRING_QR, 63230, L"Regex (qr)", L"fore:#006633; back:#FFF1A8", L"" },
                    { SCE_PL_STRING_QW, 63231, L"Array (qw)", L"fore:#003CE6", L"" },
                    { SCE_PL_SUB_PROTOTYPE, 63253, L"Prototype", L"fore:#800080; back:#FFE2FF", L"" },
                    { SCE_PL_FORMAT_IDENT, 63254, L"Format identifier", L"bold; fore:#648000; back:#FFF1A8", L"" },
                    { SCE_PL_FORMAT, 63255, L"Format body", L"fore:#648000; back:#FFF1A8", L"" },
                    { SCE_PL_POD, 63213, L"POD (common)", L"fore:#A46000; back:#FFFFC0; eolfilled", L"" },
                    { SCE_PL_POD_VERB, 63214, L"POD (verbatim)", L"fore:#A46000; back:#FFFFC0; eolfilled", L"" },
                    { SCE_PL_DATASECTION, 63222, L"Data section", L"fore:#A46000; back:#FFFFC0; eolfilled", L"" },
                    { SCE_PL_ERROR, 63252, L"Parsing error", L"fore:#C80000; back:#FFFF80", L"" },
                    //{ SCE_PL_PUNCTUATION, L"Symbols / punctuation (not used)", L"", L"" },
                    //{ SCE_PL_PREPROCESSOR, L"Preprocessor (not used)", L"", L"" },
                    //{ SCE_PL_LONGQUOTE, L"Long quote (qq, qr, qw, qx) (not used)", L"", L"" },
                    { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_INI = {
"", "", "", "", "", "", "", "", "" };


EDITLEXER lexINI = { SCLEX_PROPERTIES, 63015, L"Configuration Files", L"ini; inf; reg; cfg; properties; oem; sif; url; sed; theme", L"", &KeyWords_INI, {
                     { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                     //{ SCE_PROPS_DEFAULT, L"Default", L"", L"" },
                     { SCE_PROPS_COMMENT, 63127, L"Comment", L"fore:#008000", L"" },
                     { SCE_PROPS_SECTION, 63232, L"Section", L"fore:#000000; back:#FF8040; bold; eolfilled", L"" },
                     { SCE_PROPS_ASSIGNMENT, 63233, L"Assignment", L"fore:#FF0000", L"" },
                     { SCE_PROPS_DEFVAL, 63234, L"Default Value", L"fore:#FF0000", L"" },
                     { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_BAT = {
"arp assoc attrib bcdedit bootcfg break cacls call cd change chcp chdir chkdsk chkntfs choice cipher "
"cleanmgr cls cmd cmdkey color com comp compact con convert copy country ctty date defined defrag del "
"dir disableextensions diskcomp diskcopy diskpart do doskey driverquery echo echo. else enabledelayedexpansion "
"endlocal equ erase errorlevel exist exit expand fc find findstr for forfiles format fsutil ftp ftype "
"geq goto gpresult gpupdate graftabl gtr help icacls if in ipconfig kill label leq loadfix loadhigh logman "
"logoff lpt lss md mem mkdir mklink mode more move msg msiexe nbtstat neq net netstat netsh not nslookup nul "
"openfiles path pathping pause perfmon popd powercfg print prompt pushd rd recover reg regedit regsvr32 rem "
"ren rename replace rmdir robocopy route runas rundll32 sc schtasks sclist set setlocal sfc shift shutdown sort "
"start subst systeminfo taskkill tasklist time timeout title tracert tree type typeperf ver verify vol wmic xcopy",
"", "", "", "", "", "", "", "" };


EDITLEXER lexBAT = { SCLEX_BATCH, 63016, L"Batch Files", L"bat; cmd", L"", &KeyWords_BAT, {
                     { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                     //{ SCE_BAT_DEFAULT, L"Default", L"", L"" },
                     { SCE_BAT_COMMENT, 63127, L"Comment", L"fore:#008000", L"" },
                     { SCE_BAT_WORD, 63128, L"Keyword", L"bold; fore:#0A246A", L"" },
                     { SCE_BAT_IDENTIFIER, 63129, L"Identifier", L"fore:#003CE6; back:#FFF1A8", L"" },
                     { SCE_BAT_OPERATOR, 63132, L"Operator", L"fore:#B000B0", L"" },
                     { MULTI_STYLE(SCE_BAT_COMMAND,SCE_BAT_HIDE,0,0), 63236, L"Command", L"bold", L"" },
                     { SCE_BAT_LABEL, 63235, L"Label", L"fore:#C80000; back:#F4F4F4; eolfilled", L"" },
                     { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_DIFF = {
"", "", "", "", "", "", "", "", "" };


EDITLEXER lexDIFF = { SCLEX_DIFF, 63017, L"Diff Files", L"diff; patch", L"", &KeyWords_DIFF, {
                      { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                      //{ SCE_DIFF_DEFAULT, L"Default", L"", L"" },
                      { SCE_DIFF_COMMENT, 63127, L"Comment", L"fore:#008000", L"" },
                      { SCE_DIFF_COMMAND, 63236, L"Command", L"bold; fore:#0A246A", L"" },
                      { SCE_DIFF_HEADER, 63238, L"Source and Destination", L"fore:#C80000; back:#FFF1A8; eolfilled", L"" },
                      { SCE_DIFF_POSITION, 63239, L"Position Setting", L"fore:#0000FF", L"" },
                      { SCE_DIFF_ADDED, 63240, L"Line Addition", L"fore:#002000; back:#80FF80; eolfilled", L"" },
                      { SCE_DIFF_DELETED, 63241, L"Line Removal", L"fore:#200000; back:#FF8080; eolfilled", L"" },
                      { SCE_DIFF_CHANGED, 63242, L"Line Change", L"fore:#000020; back:#8080FF; eolfilled", L"" },
                      { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_SQL = {
"abort accessible action add after all alter analyze and as asc asensitive attach autoincrement "
"before begin between bigint binary bit blob both by call cascade case cast change char character "
"check collate column commit condition conflict constraint continue convert create cross current_date "
"current_time current_timestamp current_user cursor database databases date day_hour day_microsecond "
"day_minute day_second dec decimal declare default deferrable deferred delayed delete desc describe "
"detach deterministic distinct distinctrow div double drop dual each else elseif enclosed end enum "
"escape escaped except exclusive exists exit explain fail false fetch float float4 float8 for force "
"foreign from full fulltext glob grant group having high_priority hour_microsecond hour_minute "
"hour_second if ignore immediate in index infile initially inner inout insensitive insert instead int "
"int1 int2 int3 int4 int8 integer intersect interval into is isnull iterate join key keys kill "
"leading leave left like limit linear lines load localtime localtimestamp lock long longblob longtext "
"loop low_priority master_ssl_verify_server_cert match mediumblob mediumint mediumtext middleint "
"minute_microsecond minute_second mod modifies natural no no_write_to_binlog not notnull null numeric "
"of offset on optimize option optionally or order out outer outfile plan pragma precision primary "
"procedure purge query raise range read read_only read_write reads real references regexp reindex "
"release rename repeat replace require restrict return revoke right rlike rollback row rowid schema "
"schemas second_microsecond select sensitive separator set show smallint spatial specific sql "
"sql_big_result sql_calc_found_rows sql_small_result sqlexception sqlstate sqlwarning ssl starting "
"straight_join table temp temporary terminated text then time timestamp tinyblob tinyint tinytext to "
"trailing transaction trigger true undo union unique unlock unsigned update usage use using utc_date "
"utc_time utc_timestamp vacuum values varbinary varchar varcharacter varying view virtual when where "
"while with write xor year_month zerofill",
"", "", "", "", "", "", "", "" };


EDITLEXER lexSQL = { SCLEX_SQL, 63018, L"SQL Query", L"sql", L"", &KeyWords_SQL, {
                     { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                     //{ SCE_SQL_DEFAULT, L"Default", L"", L"" },
                     { SCE_SQL_COMMENT, 63127, L"Comment", L"fore:#505050", L"" },
                     { SCE_SQL_WORD, 63128, L"Keyword", L"bold; fore:#800080", L"" },
                     { MULTI_STYLE(SCE_SQL_STRING,SCE_SQL_CHARACTER,0,0), 63131, L"String", L"fore:#008000; back:#FFF1A8", L"" },
                     { SCE_SQL_IDENTIFIER, 63129, L"Identifier", L"fore:#800080", L"" },
                     { SCE_SQL_QUOTEDIDENTIFIER, 63243, L"Quoted Identifier", L"fore:#800080; back:#FFCCFF", L"" },
                     { SCE_SQL_NUMBER, 63130, L"Number", L"fore:#FF0000", L"" },
                     { SCE_SQL_OPERATOR, 63132, L"Operator", L"bold; fore:#800080", L"" },
                     { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_PY = {
"and as assert break class continue def del elif else except "
"exec False finally for from global if import in is lambda None "
"not or pass print raise return True try with while yield",
"", "", "", "", "", "", "", "" };


EDITLEXER lexPY = { SCLEX_PYTHON, 63019, L"Python Script", L"py; pyw", L"", &KeyWords_PY, {
                    { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                    //{ SCE_P_DEFAULT, L"Default", L"", L"" },
                    { MULTI_STYLE(SCE_P_COMMENTLINE,SCE_P_COMMENTBLOCK,0,0), 63127, L"Comment", L"fore:#880000", L"" },
                    { SCE_P_WORD, 63128, L"Keyword", L"fore:#000088", L"" },
                    { SCE_P_IDENTIFIER, 63129, L"Identifier", L"", L"" },
                    { MULTI_STYLE(SCE_P_STRING,SCE_P_STRINGEOL,0,0), 63211, L"String double quoted", L"fore:#008800", L"" },
                    { SCE_P_CHARACTER, 63212, L"String single quoted", L"fore:#008800", L"" },
                    { SCE_P_TRIPLEDOUBLE, 63244, L"String triple double quotes", L"fore:#008800", L"" },
                    { SCE_P_TRIPLE, 63245, L"String triple single quotes", L"fore:#008800", L"" },
                    { SCE_P_NUMBER, 63130, L"Number", L"fore:#FF4000", L"" },
                    { SCE_P_OPERATOR, 63132, L"Operator", L"bold; fore:#666600", L"" },
                    { SCE_P_DEFNAME, 63247, L"Function name", L"fore:#660066", L"" },
                    { SCE_P_CLASSNAME, 63246, L"Class name", L"fore:#660066", L"" },
                    { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_CONF = {
"acceptmutex acceptpathinfo accessconfig accessfilename action addalt addaltbyencoding addaltbytype addcharset adddefaultcharset "
"adddescription addencoding addhandler addicon addiconbyencoding addiconbytype addinputfilter addlanguage addmodule addmoduleinfo "
"addoutputfilter addoutputfilterbytype addtype agentlog alias aliasmatch all allow allowconnect allowencodedslashes allowoverride "
"anonymous anonymous_authoritative anonymous_logemail anonymous_mustgiveemail anonymous_nouserid anonymous_verifyemail "
"assignuserid authauthoritative authdbauthoritative authdbgroupfile authdbmauthoritative authdbmgroupfile authdbmtype "
"authdbmuserfile authdbuserfile authdigestalgorithm authdigestdomain authdigestfile authdigestgroupfile authdigestnccheck "
"authdigestnonceformat authdigestnoncelifetime authdigestqop authdigestshmemsize authgroupfile authldapauthoritative "
"authldapbinddn authldapbindpassword authldapcharsetconfig authldapcomparednonserver authldapdereferencealiases authldapenabled "
"authldapfrontpagehack authldapgroupattribute authldapgroupattributeisdn authldapremoteuserisdn authldapurl authname authtype "
"authuserfile bindaddress browsermatch browsermatchnocase bs2000account bufferedlogs cachedefaultexpire cachedirlength "
"cachedirlevels cachedisable cacheenable cacheexpirycheck cachefile cacheforcecompletion cachegcclean cachegcdaily "
"cachegcinterval cachegcmemusage cachegcunused cacheignorecachecontrol cacheignoreheaders cacheignorenolastmod "
"cachelastmodifiedfactor cachemaxexpire cachemaxfilesize cacheminfilesize cachenegotiateddocs cacheroot cachesize cachetimemargin "
"cgimapextension charsetdefault charsetoptions charsetsourceenc checkspelling childperuserid clearmodulelist contentdigest "
"cookiedomain cookieexpires cookielog cookiename cookiestyle cookietracking coredumpdirectory customlog dav davdepthinfinity "
"davlockdb davmintimeout defaulticon defaultlanguage defaulttype define deflatebuffersize deflatecompressionlevel "
"deflatefilternote deflatememlevel deflatewindowsize deny directory directoryindex directorymatch directoryslash documentroot "
"dumpioinput dumpiooutput enableexceptionhook enablemmap enablesendfile errordocument errorlog example expiresactive "
"expiresbytype expiresdefault extendedstatus extfilterdefine extfilteroptions fancyindexing fileetag files filesmatch "
"forcelanguagepriority forcetype forensiclog from group header headername hostnamelookups identitycheck ifdefine ifmodule "
"imapbase imapdefault imapmenu include indexignore indexoptions indexorderdefault isapiappendlogtoerrors isapiappendlogtoquery "
"isapicachefile isapifakeasync isapilognotsupported isapireadaheadbuffer keepalive keepalivetimeout languagepriority "
"ldapcacheentries ldapcachettl ldapconnectiontimeout ldapopcacheentries ldapopcachettl ldapsharedcachefile ldapsharedcachesize "
"ldaptrustedca ldaptrustedcatype limit limitexcept limitinternalrecursion limitrequestbody limitrequestfields "
"limitrequestfieldsize limitrequestline limitxmlrequestbody listen listenbacklog loadfile loadmodule location locationmatch "
"lockfile logformat loglevel maxclients maxkeepaliverequests maxmemfree maxrequestsperchild maxrequestsperthread maxspareservers "
"maxsparethreads maxthreads maxthreadsperchild mcachemaxobjectcount mcachemaxobjectsize mcachemaxstreamingbuffer "
"mcacheminobjectsize mcacheremovalalgorithm mcachesize metadir metafiles metasuffix mimemagicfile minspareservers minsparethreads "
"mmapfile modmimeusepathinfo multiviewsmatch namevirtualhost nocache noproxy numservers nwssltrustedcerts nwsslupgradeable "
"options order passenv pidfile port protocolecho proxy proxybadheader proxyblock proxydomain proxyerroroverride proxyiobuffersize "
"proxymatch proxymaxforwards proxypass proxypassreverse proxypreservehost proxyreceivebuffersize proxyremote proxyremotematch "
"proxyrequests proxytimeout proxyvia qsc readmename redirect redirectmatch redirectpermanent redirecttemp refererignore "
"refererlog removecharset removeencoding removehandler removeinputfilter removelanguage removeoutputfilter removetype "
"requestheader require resourceconfig rewritebase rewritecond rewriteengine rewritelock rewritelog rewriteloglevel rewritemap "
"rewriteoptions rewriterule rlimitcpu rlimitmem rlimitnproc satisfy scoreboardfile script scriptalias scriptaliasmatch "
"scriptinterpretersource scriptlog scriptlogbuffer scriptloglength scriptsock securelisten sendbuffersize serveradmin serveralias "
"serverlimit servername serverpath serverroot serversignature servertokens servertype setenv setenvif setenvifnocase sethandler "
"setinputfilter setoutputfilter singlelisten ssiendtag ssierrormsg ssistarttag ssitimeformat ssiundefinedecho "
"sslcacertificatefile sslcacertificatepath sslcarevocationfile sslcarevocationpath sslcertificatechainfile sslcertificatefile "
"sslcertificatekeyfile sslciphersuite sslengine sslmutex ssloptions sslpassphrasedialog sslprotocol sslproxycacertificatefile "
"sslproxycacertificatepath sslproxycarevocationfile sslproxycarevocationpath sslproxyciphersuite sslproxyengine "
"sslproxymachinecertificatefile sslproxymachinecertificatepath sslproxyprotocol sslproxyverify sslproxyverifydepth sslrandomseed "
"sslrequire sslrequiressl sslsessioncache sslsessioncachetimeout sslusername sslverifyclient sslverifydepth startservers "
"startthreads suexecusergroup threadlimit threadsperchild threadstacksize timeout transferlog typesconfig unsetenv "
"usecanonicalname user userdir virtualdocumentroot virtualdocumentrootip virtualhost virtualscriptalias virtualscriptaliasip "
"win32disableacceptex xbithack",
"", //"on off standalone inetd force-response-1.0 downgrade-1.0 nokeepalive indexes includes followsymlinks none x-compress x-gzip",
"", "", "", "", "", "", "" };


EDITLEXER lexCONF = { SCLEX_CONF, 63020, L"Apache Config Files", L"conf; htaccess", L"", &KeyWords_CONF, {
                      { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                      { SCE_CONF_COMMENT, 63127, L"Comment", L"fore:#648000", L"" },
                      { SCE_CONF_STRING, 63131, L"String", L"fore:#B000B0", L"" },
                      { SCE_CONF_NUMBER, 63130, L"Number", L"fore:#FF4000", L"" },
                      { SCE_CONF_DIRECTIVE, 63203, L"Directive", L"fore:#003CE6", L"" },
                      { SCE_CONF_IP, 63248, L"IP Address", L"bold; fore:#FF4000", L"" },
// Not used by lexer  { SCE_CONF_IDENTIFIER, L"Identifier", L"", L"" },
// Lexer is buggy     { SCE_CONF_OPERATOR, L"Operator", L"", L"" },
// Lexer is buggy     { SCE_CONF_PARAMETER, L"Runtime Directive Parameter", L"", L"" },
// Lexer is buggy     { SCE_CONF_EXTENSION, L"Extension", L"", L"" },
                      { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_PS = {
"begin break catch continue data do dynamicparam else elseif end exit filter finally for foreach "
"from function if in local param private process return switch throw trap try until where while",
"add-computer add-content add-history add-member add-pssnapin add-type checkpoint-computer "
"clear-content clear-eventlog clear-history clear-host clear-item clear-itemproperty "
"clear-variable compare-object complete-transaction connect-wsman convertfrom-csv "
"convertfrom-securestring convertfrom-stringdata convert-path convertto-csv convertto-html "
"convertto-securestring convertto-xml copy-item copy-itemproperty debug-process "
"disable-computerrestore disable-psbreakpoint disable-psremoting disable-pssessionconfiguration "
"disable-wsmancredssp disconnect-wsman enable-computerrestore enable-psbreakpoint "
"enable-psremoting enable-pssessionconfiguration enable-wsmancredssp enter-pssession "
"exit-pssession export-alias export-clixml export-console export-counter export-csv "
"export-formatdata export-modulemember export-pssession foreach-object format-custom format-list "
"format-table format-wide get-acl get-alias get-authenticodesignature get-childitem get-command "
"get-computerrestorepoint get-content get-counter get-credential get-culture get-date get-event "
"get-eventlog get-eventsubscriber get-executionpolicy get-formatdata get-help get-history "
"get-host get-hotfix get-item get-itemproperty get-job get-location get-member get-module "
"get-pfxcertificate get-process get-psbreakpoint get-pscallstack get-psdrive get-psprovider "
"get-pssession get-pssessionconfiguration get-pssnapin get-random get-service get-tracesource "
"get-transaction get-uiculture get-unique get-variable get-verb get-winevent get-wmiobject "
"get-wsmancredssp get-wsmaninstance group-object import-alias import-clixml import-counter "
"import-csv import-localizeddata import-module import-pssession invoke-command invoke-expression "
"invoke-history invoke-item invoke-wmimethod invoke-wsmanaction join-path limit-eventlog "
"measure-command measure-object move-item move-itemproperty new-alias new-event new-eventlog "
"new-item new-itemproperty new-module new-modulemanifest new-object new-psdrive new-pssession "
"new-pssessionoption new-service new-timespan new-variable new-webserviceproxy new-wsmaninstance "
"new-wsmansessionoption out-default out-file out-gridview out-host out-null out-printer "
"out-string pop-location push-location read-host receive-job register-engineevent "
"register-objectevent register-pssessionconfiguration register-wmievent remove-computer "
"remove-event remove-eventlog remove-item remove-itemproperty remove-job remove-module "
"remove-psbreakpoint remove-psdrive remove-pssession remove-pssnapin remove-variable "
"remove-wmiobject remove-wsmaninstance rename-item rename-itemproperty "
"reset-computermachinepassword resolve-path restart-computer restart-service restore-computer "
"resume-service select-object select-string select-xml send-mailmessage set-acl set-alias "
"set-authenticodesignature set-content set-date set-executionpolicy set-item set-itemproperty "
"set-location set-psbreakpoint set-psdebug set-pssessionconfiguration set-service set-strictmode "
"set-tracesource set-variable set-wmiinstance set-wsmaninstance set-wsmanquickconfig "
"show-eventlog sort-object split-path start-job start-process start-service start-sleep "
"start-transaction start-transcript stop-computer stop-job stop-process stop-service "
"stop-transcript suspend-service tee-object test-computersecurechannel test-connection "
"test-modulemanifest test-path test-wsman trace-command undo-transaction unregister-event "
"unregister-pssessionconfiguration update-formatdata update-list update-typedata use-transaction "
"wait-event wait-job wait-process where-object write-debug write-error write-eventlog write-host "
"write-output write-progress write-verbose write-warning",
"ac asnp cat cd chdir clc clear clhy cli clp cls clv compare copy cp cpi cpp cvpa dbp del diff "
"dir ebp echo epal epcsv epsn erase etsn exsn fc fl foreach ft fw gal gbp gc gci gcm gcs gdr ghy "
"gi gjb gl gm gmo gp gps group gsn gsnp gsv gu gv gwmi h help history icm iex ihy ii ipal ipcsv "
"ipmo ipsn ise iwmi kill lp ls man md measure mi mkdir more mount move mp mv nal ndr ni nmo nsn "
"nv ogv oh popd ps pushd pwd r rbp rcjb rd rdr ren ri rjb rm rmdir rmo rni rnp rp rsn rsnp rv "
"rvpa rwmi sajb sal saps sasv sbp sc select set si sl sleep sort sp spjb spps spsv start sv swmi "
"tee type where wjb write",
"importsystemmodules prompt psedit tabexpansion",
"", "", "", "", "" };


EDITLEXER lexPS = { SCLEX_POWERSHELL, 63021, L"PowerShell Script", L"ps1; psd1; psm1", L"", &KeyWords_PS, {
                    { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                    //{ SCE_POWERSHELL_DEFAULT, L"Default", L"", L"" },
                    { MULTI_STYLE(SCE_POWERSHELL_COMMENT,SCE_POWERSHELL_COMMENTSTREAM,0,0), 63127, L"Comment", L"fore:#646464", L"" },
                    { SCE_POWERSHELL_KEYWORD, 63128, L"Keyword", L"bold; fore:#804000", L"" },
                    { SCE_POWERSHELL_IDENTIFIER, 63129, L"Identifier", L"", L"" },
                    { MULTI_STYLE(SCE_POWERSHELL_STRING,SCE_POWERSHELL_CHARACTER,0,0), 63131, L"String", L"fore:#008000", L"" },
                    { SCE_POWERSHELL_NUMBER, 63130, L"Number", L"fore:#FF0000", L"" },
                    { SCE_POWERSHELL_OPERATOR, 63132, L"Operator", L"bold", L"" },
                    { SCE_POWERSHELL_VARIABLE, 63249, L"Variable", L"fore:#0A246A", L"" },
                    { MULTI_STYLE(SCE_POWERSHELL_CMDLET,SCE_POWERSHELL_FUNCTION,0,0), 63250, L"Cmdlet", L"fore:#804000; back:#FFF1A8", L"" },
                    { SCE_POWERSHELL_ALIAS, 63251, L"Alias", L"bold; fore:#0A246A", L"" },
                    { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_NSIS = {
"!addincludedir !addplugindir !appendfile !cd !define !delfile !echo !else !endif !error "
"!execute !if !ifdef !ifmacrodef !ifmacrondef !ifndef !include !insertmacro !macro !macroend "
"!packhdr !system !tempfile !undef !verbose !warning .onguiend .onguiinit .oninit .oninstfailed "
".oninstsuccess .onmouseoversection .onrebootfailed .onselchange .onuserabort .onverifyinstdir "
"un.onguiend un.onguiinit un.oninit un.onrebootfailed un.onuninstfailed un.onuninstsuccess un.onuserabort "
"abort addbrandingimage addsize allowrootdirinstall allowskipfiles autoclosewindow bannertrimpath "
"bgfont bggradient brandingtext bringtofront call callinstdll caption changeui checkbitmap clearerrors "
"completedtext componenttext copyfiles crccheck createdirectory createfont createshortcut delete "
"deleteinisec deleteinistr deleteregkey deleteregvalue detailprint detailsbuttontext dirstate dirtext "
"dirvar dirverify enablewindow enumregkey enumregvalue exch exec execshell execwait expandenvstrings "
"file filebufsize fileclose fileerrortext fileopen fileread filereadbyte fileseek filewrite filewritebyte "
"findclose findfirst findnext findwindow flushini getcurinsttype getcurrentaddress getdlgitem "
"getdllversion getdllversionlocal geterrorlevel getfiletime getfiletimelocal getfullpathname "
"getfunctionaddress getinstdirerror getlabeladdress gettempfilename goto hidewindow icon ifabort iferrors "
"iffileexists ifrebootflag ifsilent initpluginsdir installbuttontext installcolors installdir "
"installdirregkey instprogressflags insttypegettext insttypesettext intcmp intcmpu intfmt intop iswindow "
"langstring licensebkcolor licensedata licenseforceselection licenselangstring licensetext loadlanguagefile "
"lockwindow logset logtext messagebox miscbuttontext name nop outfile page pop push quit readenvstr "
"readinistr readregdword readregstr reboot regdll rename requestexecutionlevel reservefile return rmdir "
"searchpath sectiongetflags sectiongetinsttypes sectiongetsize sectiongettext sectionin sectionsetflags "
"sectionsetinsttypes sectionsetsize sectionsettext sendmessage setautoclose setbrandingimage setcompress "
"setcompressor setcompressordictsize setctlcolors setcurinsttype setdatablockoptimize setdatesave setdetailsprint "
"setdetailsview seterrorlevel seterrors setfileattributes setfont setoutpath setoverwrite setpluginunload "
"setrebootflag setregview setshellvarcontext setsilent showinstdetails showuninstdetails showwindow silentinstall "
"silentuninstall sleep spacetexts strcmp strcmps strcpy strlen subcaption uninstallbuttontext uninstallcaption "
"uninstallicon uninstallsubcaption uninstalltext uninstpage unregdll var viaddversionkey viproductversion "
"windowicon writeinistr writeregbin writeregdword writeregexpandstr writeregstr writeuninstaller xpstyle",
"${nsisdir} $0 $1 $2 $3 $4 $5 $6 $7 $8 $9 $r0 $r1 $r2 $r3 $r4 $r5 $r6 $r7 $r8 $r9 $instdir $outdir $cmdline "
"$language $programfiles $programfiles32 $programfiles64 $commonfiles $commonfiles32 $commonfiles64 "
"$desktop $exedir $exefile $exepath $windir $sysdir $temp $startmenu $smprograms $smstartup $quicklaunch "
"$documents $sendto $recent $favorites $music $pictures $videos $nethood $fonts $templates $appdata "
"$localappdata $printhood $internet_cache $cookies $history $profile $admintools $resources $resources_localized "
"$cdburn_area $hwndparent $pluginsdir ${__date__} ${__file__} ${__function__} ${__global__} ${__line__} "
"${__pageex__} ${__section__} ${__time__} ${__timestamp__} ${__uninstall__}",
"oname rebootok nonfatal ifempty nounload silent filesonly short mb_ok mb_okcancel mb_abortretryignore "
"mb_retrycancel mb_yesno mb_yesnocancel mb_iconexclamation mb_iconinformation mb_iconquestion mb_iconstop "
"mb_usericon mb_topmost mb_setforeground mb_right mb_rtlreading mb_defbutton1 mb_defbutton2 mb_defbutton3 "
"mb_defbutton4 idabort idcancel idignore idno idok idretry idyes sd current all timeout imgid resizetofit "
"listonly textonly both branding hkcr hkey_classes_root hklm hkey_local_machine hkcu hkey_current_user hku hkey_users "
"hkcc hkey_current_config hkdd hkey_dyn_data hkpd hkey_performance_data shctx shell_context silent left right "
"top bottom true false on off italic underline strike trimleft trimright trimcenter idd_license idd_dir "
"idd_selcom idd_inst idd_instfiles idd_uninst idd_verify force windows nocustom customstring componentsonlyoncustom "
"gray none user highest admin lang hide show nevershow normal silent silentlog auto solid final zlib bzip2 lzma try "
"ifnewer ifdiff lastused manual alwaysoff normal file_attribute_normal archive file_attribute_archive hidden "
"file_attribute_hidden offline file_attribute_offline readonly file_attribute_readonly system file_attribute_system "
"temporary file_attribute_temporary custom license components directory instfiles uninstconfirm 32 64 enablecancel",
"", "", "", "", "", "" };


EDITLEXER lexNSIS = { SCLEX_NSIS, 63284, L"NSIS Script", L"nsi; nsh", L"", &KeyWords_NSIS, {
                      { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                      //{ SCE_NSIS_DEFAULT, L"Default", L"", L"" },
                      { MULTI_STYLE(SCE_NSIS_COMMENT,SCE_NSIS_COMMENTBOX,0,0), 63127, L"Comment", L"fore:#008000", L"" },
                      { MULTI_STYLE(SCE_NSIS_STRINGDQ,SCE_NSIS_STRINGLQ,SCE_NSIS_STRINGRQ,0), 63131, L"String", L"fore:#666666; back:#EEEEEE", L"" },
                      { SCE_NSIS_FUNCTION, 63277, L"Function", L"fore:#0033CC", L"" },
                      { SCE_NSIS_VARIABLE, 63249, L"Variable", L"fore:#CC3300", L"" },
                      { SCE_NSIS_STRINGVAR, 63285, L"Variable within String", L"fore:#CC3300; back:#EEEEEE", L"" },
                      { SCE_NSIS_NUMBER, 63130, L"Number", L"fore:#FF0000", L"" },
                      { SCE_NSIS_LABEL, 63286, L"Constant", L"fore:#FF9900", L"" },
                      { SCE_NSIS_SECTIONDEF, 63232, L"Section", L"fore:#0033CC", L"" },
                      { SCE_NSIS_SUBSECTIONDEF, 63287, L"Sub Section", L"fore:#0033CC", L"" },
                      { SCE_NSIS_SECTIONGROUP, 63288, L"Section Group", L"fore:#0033CC", L"" },
                      { SCE_NSIS_FUNCTIONDEF, 63289, L"Function Definition", L"fore:#0033CC", L"" },
                      { SCE_NSIS_PAGEEX, 63290, L"PageEx", L"fore:#0033CC", L"" },
                      { SCE_NSIS_IFDEFINEDEF, 63291, L"If Definition", L"fore:#0033CC", L"" },
                      { SCE_NSIS_MACRODEF, 63292, L"Macro Definition", L"fore:#0033CC", L"" },
                      //{ SCE_NSIS_USERDEFINED, L"User Defined", L"", L"" },
                      { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_INNO = {
"code components custommessages dirs files icons ini installdelete langoptions languages messages "
"registry run setup types tasks uninstalldelete uninstallrun _istool",
"allowcancelduringinstall allownoicons allowrootdirectory allowuncpath alwaysrestart alwaysshowcomponentslist "
"alwaysshowdironreadypage alwaysshowgrouponreadypage alwaysusepersonalgroup appcomments appcontact appcopyright "
"appenddefaultdirname appenddefaultgroupname appid appmodifypath appmutex appname apppublisher apppublisherurl "
"appreadmefile appsupportphone appsupporturl appupdatesurl appvername appversion architecturesallowed "
"architecturesinstallin64bitmode backcolor backcolor2 backcolordirection backsolid beveledlabel changesassociations "
"changesenvironment compression compressionthreads copyrightfontname copyrightfontsize createappdir "
"createuninstallregkey defaultdirname defaultgroupname defaultuserinfoname defaultuserinfoorg defaultuserinfoserial "
"dialogfontname dialogfontsize direxistswarning disabledirpage disablefinishedpage disableprogramgrouppage disablereadymemo "
"disablereadypage disablestartupprompt diskclustersize diskslicesize diskspanning enabledirdoesntexistwarning "
"encryption extradiskspacerequired flatcomponentslist infoafterfile infobeforefile internalcompresslevel "
"languagedetectionmethod languagecodepage languageid languagename licensefile lzmaalgorithm lzmablocksize "
"lzmadictionarysize lzmamatchfinder lzmanumblockthreads lzmanumfastbytes lzmauseseparateprocess mergeduplicatefiles "
"minversion onlybelowversion outputbasefilename outputdir outputmanifestfile password privilegesrequired reservebytes "
"restartifneededbyrun righttoleft setupiconfile setuplogging showcomponentsizes showlanguagedialog showtaskstreelines "
"showundisplayablelanguages signeduninstaller signeduninstallerdir signtool slicesperdisk solidcompression sourcedir "
"strongassemblyname timestamprounding timestampsinutc titlefontname titlefontsize touchdate touchtime uninstallable "
"uninstalldisplayicon uninstalldisplayname uninstallfilesdir uninstalldisplaysize uninstalllogmode uninstallrestartcomputer "
"updateuninstalllogappname usepreviousappdir usepreviousgroup usepreviouslanguage useprevioussetuptype useprevioustasks "
"verb versioninfoproductname useprevioususerinfo userinfopage usesetupldr versioninfocompany versioninfocopyright "
"versioninfodescription versioninfoproductversion versioninfotextversion versioninfoversion versioninfoproducttextversion "
"welcomefontname welcomefontsize windowshowcaption windowstartmaximized windowresizable windowvisible wizardimagebackcolor "
"wizardimagefile wizardimagestretch wizardsmallimagefile",
"appusermodelid afterinstall attribs beforeinstall check comment components copymode description destdir destname excludes "
"extradiskspacerequired filename flags fontinstall groupdescription hotkey infoafterfile infobeforefile iconfilename "
"iconindex key languages licensefile messagesfile minversion name onlybelowversion parameters permissions root runonceid "
"section source statusmsg string subkey tasks terminalservicesaware type types valuedata valuename valuetype workingdir",
"append define dim else emit elif endif endsub error expr file for if "
"ifdef ifexist ifndef ifnexist include insert pragma sub undef",
"and begin break case const continue do downto else end except finally for function "
"if not of or procedure repeat then to try type until uses var while with",
"", "", "", "" };


EDITLEXER lexINNO = { SCLEX_INNOSETUP, 63293, L"Inno Setup Script", L"iss; isl; islu", L"", &KeyWords_INNO, {
                      { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                    //{ SCE_INNO_DEFAULT, L"Default", L"", L"" },
                      { SCE_INNO_COMMENT, 63127, L"Comment", L"fore:#008000", L"" },
                      { SCE_INNO_KEYWORD, 63128, L"Keyword", L"fore:#0000FF", L"" },
                      { SCE_INNO_PARAMETER, 63294, L"Parameter", L"fore:#0000FF", L"" },
                      { SCE_INNO_SECTION, 63232, L"Section", L"fore:#000080; bold", L"" },
                      { SCE_INNO_PREPROC, 63133, L"Preprocessor", L"fore:#CC0000", L"" },
                      { SCE_INNO_INLINE_EXPANSION, 63295, L"Inline Expansion", L"fore:#800080", L"" },
                      { SCE_INNO_COMMENT_PASCAL, 63296, L"Pascal Comment", L"fore:#008000", L"" },
                      { SCE_INNO_KEYWORD_PASCAL, 63297, L"Pascal Keyword", L"fore:#0000FF", L"" },
                      { MULTI_STYLE(SCE_INNO_STRING_DOUBLE,SCE_INNO_STRING_SINGLE,0,0), 63131, L"String", L"", L"" },
                      //{ SCE_INNO_IDENTIFIER, 63129, L"Identifier", L"", L"" },
                      //{ SCE_INNO_KEYWORD_USER, L"User Defined", L"", L"" },
                      { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_RUBY = {
"__FILE__ __LINE__ alias and begin break case class def defined? do else elsif end ensure "
"false for in if module next nil not or redo rescue retry return self super then true "
"undef unless until when while yield",
"", "", "", "", "", "", "", "" };

EDITLEXER lexRUBY = { SCLEX_RUBY, 63304, L"Ruby Script", L"rb; ruby; rbw; rake; rjs; Rakefile", L"", &KeyWords_RUBY, {
                    { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                    //{ SCE_P_DEFAULT, L"Default", L"", L"" },
                    { MULTI_STYLE(SCE_RB_COMMENTLINE,SCE_P_COMMENTBLOCK,0,0), 63127, L"Comment", L"fore:#008000", L"" },
                    { SCE_RB_WORD, 63128, L"Keyword", L"fore:#00007F", L"" },
                    { SCE_RB_IDENTIFIER, 63129, L"Identifier", L"", L"" },
                    { SCE_RB_NUMBER, 63130, L"Number", L"fore:#008080", L"" },
                    { SCE_RB_OPERATOR, 63132, L"Operator", L"", L"" },
                    { MULTI_STYLE(SCE_RB_STRING,SCE_RB_CHARACTER,SCE_P_STRINGEOL,0), 63131, L"String", L"fore:#FF8000", L"" },
                    { SCE_RB_CLASSNAME, 63246, L"Class name", L"fore:#0000FF", L"" },
                    { SCE_RB_DEFNAME, 63247, L"Function name", L"fore:#007F7F", L"" },
                    { SCE_RB_POD, 63314, L"POD", L"fore:#004000; back:#C0FFC0; eolfilled", L"" },
                    { SCE_RB_REGEX, 63315, L"Regex", L"fore:#000000; back:#A0FFA0", L"" },
                    { SCE_RB_SYMBOL, 63316, L"Symbol", L"fore:#C0A030", L"" },
                    { SCE_RB_MODULE_NAME, 63317, L"Module name", L"fore:#A000A0", L"" },
                    { SCE_RB_INSTANCE_VAR, 63318, L"Instance Var", L"fore:#B00080", L"" },
                    { SCE_RB_CLASS_VAR, 63319, L"Class Var", L"fore:#8000B0", L"" },
                    { SCE_RB_DATASECTION, 63320, L"Data Section", L"fore:#600000; back:#FFF0D8; eolfilled", L"" },
                    { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_LUA = {
"and break do else elseif end false for function if in local nil not or repeat return then "
"true until while",
// Basic Functions
"_VERSION assert collectgarbage dofile error gcinfo loadfile loadstring print rawget rawset "
"require tonumber tostring type unpack _ALERT _ERRORMESSAGE _INPUT _PROMPT _OUTPUT _STDERR "
"_STDIN _STDOUT call dostring foreach foreachi getn globals newtype sort tinsert tremove "
"_G getfenv getmetatable ipairs loadlib next pairs pcall rawequal setfenv setmetatable xpcall "
"string table math coroutine io os debug load module select",
// String Manipulation, Table Manipulation, Mathematical Functions
"abs acos asin atan atan2 ceil cos deg exp floor format frexp gsub ldexp log log10 max min "
"mod rad random randomseed sin sqrt strbyte strchar strfind strlen strlower strrep strsub strupper tan "
"string.byte string.char string.dump string.find string.len string.lower string.rep string.sub string.upper "
"string.format string.gfind string.gsub table.concat table.foreach table.foreachi table.getn table.sort "
"table.insert table.remove table.setn math.abs math.acos math.asin math.atan math.atan2 math.ceil math.cos "
"math.deg math.exp math.floor math.frexp math.ldexp math.log math.log10 math.max math.min math.mod "
"math.pi math.pow math.rad math.random math.randomseed math.sin math.sqrt math.tan string.gmatch "
"string.match string.reverse table.maxn math.cosh math.fmod math.modf math.sinh math.tanh math.huge",
// Input and Output Facilities & System Facilities Coroutine Manipulation,
//Input and Output Facilities, System Facilities (coroutine & io & os)
"openfile closefile readfrom writeto appendto remove rename flush seek tmpfile tmpname read "
"write clock date difftime execute exit getenv setlocale time coroutine.create coroutine.resume "
"coroutine.status coroutine.wrap coroutine.yield io.close io.flush io.input io.lines io.open io.output "
"io.read io.tmpfile io.type io.write io.stdin io.stdout io.stderr os.clock os.date os.difftime "
"os.execute os.exit os.getenv os.remove os.rename os.setlocale os.time os.tmpname coroutine.running "
"package.cpath package.loaded package.loadlib package.path package.preload package.seeall io.popen",
"", "", "", "", "" };


EDITLEXER lexLUA = { SCLEX_LUA, 63298, L"Lua Script", L"lua", L"", &KeyWords_LUA, {
                    { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                    //{ SCE_LUA_DEFAULT, L"Default", L"", L"" },
                    { MULTI_STYLE(SCE_LUA_COMMENT,SCE_LUA_COMMENTLINE,SCE_LUA_COMMENTDOC,0), 63127, L"Comment", L"fore:#008000", L"" },
                    { SCE_LUA_NUMBER, 63130, L"Number", L"fore:#008080", L"" },
                    { SCE_LUA_WORD, 63128, L"Keyword", L"fore:#00007F", L"" },
                    { SCE_LUA_WORD2, 63321, L"Basic Functions", L"fore:#00007F", L"" },
                    { SCE_LUA_WORD3, 63322, L"String, Table & Math Functions", L"fore:#00007F", L"" },
                    { SCE_LUA_WORD4, 63323, L"Input, Output & System Facilities", L"fore:#00007F", L"" },
                    { MULTI_STYLE(SCE_LUA_STRING,SCE_LUA_STRINGEOL,0,0), 63131, L"String", L"fore:#B000B0", L"" },
                    { SCE_LUA_LITERALSTRING, 63302, L"Literal String", L"fore:#B000B0", L"" },
                    { SCE_LUA_CHARACTER, 63301, L"Character", L"fore:#B000B0", L"" },
                    { SCE_LUA_PREPROCESSOR, 63133, L"Preprocessor", L"fore:#FF8000", L"" },
                    { SCE_LUA_OPERATOR, 63132, L"Operator", L"", L"" },
                    { SCE_LUA_IDENTIFIER, 63129, L"Identifier", L"", L"" },
                    { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_BASH = {
"alias ar asa awk banner basename bash bc bdiff break bunzip2 bzip2 cal calendar case cat "
"cc cd chmod cksum clear cmp col comm compress continue cp cpio crypt csplit ctags cut date "
"dc dd declare deroff dev df diff diff3 dircmp dirname do done du echo ed egrep elif else "
"env esac eval ex exec exit expand export expr false fc fgrep fi file find fmt fold for function "
"functions getconf getopt getopts grep gres hash head help history iconv id if in integer "
"jobs join kill local lc let line ln logname look ls m4 mail mailx make man mkdir more mt mv "
"newgrp nl nm nohup ntps od pack paste patch pathchk pax pcat perl pg pr print printf ps pwd "
"read readonly red return rev rm rmdir sed select set sh shift size sleep sort spell split "
"start stop strings strip stty sum suspend sync tail tar tee test then time times touch tr "
"trap true tsort tty type typeset ulimit umask unalias uname uncompress unexpand uniq unpack "
"unset until uudecode uuencode vi vim vpax wait wc whence which while who wpaste wstart xargs "
"zcat chgrp chown chroot dir dircolors factor groups hostid install link md5sum mkfifo mknod "
"nice pinky printenv ptx readlink seq sha1sum shred stat su tac unlink users vdir whoami yes",
"", "", "", "", "", "", "", "" };


EDITLEXER lexBASH = { SCLEX_BASH, 63259, L"Shell Script", L"sh", L"", &KeyWords_BASH, {
                      { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                      { SCE_SH_ERROR, 63260, L"Error", L"", L"" },
                      { SCE_SH_COMMENTLINE, 63127, L"Comment", L"fore:#008000", L"" },
                      { SCE_SH_NUMBER, 63130, L"Number", L"fore:#008080", L"" },
                      { SCE_SH_WORD, 63128, L"Keyword", L"fore:#0000FF", L"" },
                      { SCE_SH_STRING, 63211, L"String double quoted", L"fore:#008080", L"" },
                      { SCE_SH_CHARACTER, 63212, L"String single quoted", L"fore:#800080", L"" },
                      { SCE_SH_OPERATOR, 63132, L"Operator", L"", L"" },
                      { SCE_SH_IDENTIFIER, 63129, L"Identifier", L"", L"" },
                      { SCE_SH_SCALAR, 63268, L"Scalar", L"fore:#808000", L"" },
                      { SCE_SH_PARAM, 63269, L"Parameter expansion", L"fore:#808000;back:#FFFF99", L"" },
                      { SCE_SH_BACKTICKS, 63270, L"Back Ticks", L"fore:#FF0080", L"" },
                      { SCE_SH_HERE_DELIM, 63271, L"Here-doc (Delimiter)", L"", L"" },
                      { SCE_SH_HERE_Q, 63272, L"Here-doc (Single quoted, q)", L"fore:#008080", L"" },
                      { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_TCL = {
// TCL Keywords
"after append array auto_execok auto_import auto_load auto_load_index auto_qualify beep "
"bgerror binary break case catch cd clock close concat continue dde default echo else "
"elseif encoding eof error eval exec exit expr fblocked fconfigure fcopy file fileevent "
"flush for foreach format gets glob global history http if incr info interp join lappend "
"lindex linsert list llength load loadTk lrange lreplace lsearch lset lsort memory msgcat "
"namespace open package pid pkg::create pkg_mkIndex Platform-specific proc puts pwd "
"re_syntax read regexp registry regsub rename resource return scan seek set socket source "
"split string subst switch tclLog tclMacPkgSearch tclPkgSetup tclPkgUnknown tell time trace "
"unknown unset update uplevel upvar variable vwait while",
// TK Keywords
"bell bind bindtags bitmap button canvas checkbutton clipboard colors console cursors "
"destroy entry event focus font frame grab grid image Inter-client keysyms label labelframe "
"listbox lower menu menubutton message option options pack panedwindow photo place "
"radiobutton raise scale scrollbar selection send spinbox text tk tk_chooseColor "
"tk_chooseDirectory tk_dialog tk_focusNext tk_getOpenFile tk_messageBox tk_optionMenu "
"tk_popup tk_setPalette tkerror tkvars tkwait toplevel winfo wish wm",
// iTCL Keywords
"@scope body class code common component configbody constructor define destructor hull "
"import inherit itcl itk itk_component itk_initialize itk_interior itk_option iwidgets keep "
"method private protected public",
"", "", "", "", "", "" };


#define SCE_TCL__MULTI_COMMENT      MULTI_STYLE(SCE_TCL_COMMENT,SCE_TCL_COMMENTLINE,SCE_TCL_COMMENT_BOX,SCE_TCL_BLOCK_COMMENT)
#define SCE_TCL__MULTI_KEYWORD      MULTI_STYLE(SCE_TCL_WORD,SCE_TCL_WORD2,SCE_TCL_WORD3,SCE_TCL_WORD_IN_QUOTE)
#define SCE_TCL__MULTI_SUBSTITUTION MULTI_STYLE(SCE_TCL_SUBSTITUTION,SCE_TCL_SUB_BRACE,0,0)


EDITLEXER lexTCL = { SCLEX_TCL, 63273, L"Tcl Script", L"tcl; itcl", L"", &KeyWords_TCL, {
                     { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                     { SCE_TCL__MULTI_COMMENT, 63127, L"Comment", L"fore:#008000", L"" },
                     { SCE_TCL__MULTI_KEYWORD, 63128, L"Keyword", L"fore:#0000FF", L"" },
                     { SCE_TCL_NUMBER, 63130, L"Number", L"fore:#008080", L"" },
                     { SCE_TCL_IN_QUOTE, 63131, L"String", L"fore:#008080", L"" },
                     { SCE_TCL_OPERATOR, 63132, L"Operator", L"", L"" },
                     { SCE_TCL_IDENTIFIER, 63129, L"Identifier", L"fore:#800080", L"" },
                     { SCE_TCL__MULTI_SUBSTITUTION, 63274, L"Substitution", L"fore:#CC0000", L"" },
                     { SCE_TCL_MODIFIER, 63275, L"Modifier", L"fore:#FF00FF", L"" },
                     { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_AU3 = {
"and byref case const continuecase continueloop default dim do else elseif endfunc endif "
"endselect endswitch endwith enum exit exitloop false for func global if in local next not "
"or redim return select step switch then to true until wend while with",
"abs acos adlibdisable adlibenable asc ascw asin assign atan autoitsetoption autoitwingettitle "
"autoitwinsettitle beep binary binarylen binarymid binarytostring bitand bitnot bitor bitrotate "
"bitshift bitxor blockinput break call cdtray ceiling chr chrw clipget clipput consoleread "
"consolewrite consolewriteerror controlclick controlcommand controldisable controlenable "
"controlfocus controlgetfocus controlgethandle controlgetpos controlgettext controlhide "
"controllistview controlmove controlsend controlsettext controlshow controltreeview cos dec "
"dircopy dircreate dirgetsize dirmove dirremove dllcall dllcallbackfree dllcallbackgetptr "
"dllcallbackregister dllclose dllopen dllstructcreate dllstructgetdata dllstructgetptr "
"dllstructgetsize dllstructsetdata drivegetdrive drivegetfilesystem drivegetlabel drivegetserial "
"drivegettype drivemapadd drivemapdel drivemapget drivesetlabel drivespacefree drivespacetotal "
"drivestatus envget envset envupdate eval execute exp filechangedir fileclose filecopy "
"filecreatentfslink filecreateshortcut filedelete fileexists filefindfirstfile filefindnextfile "
"filegetattrib filegetlongname filegetshortcut filegetshortname filegetsize filegettime "
"filegetversion fileinstall filemove fileopen fileopendialog fileread filereadline filerecycle "
"filerecycleempty filesavedialog fileselectfolder filesetattrib filesettime filewrite filewriteline "
"floor ftpsetproxy guicreate guictrlcreateavi guictrlcreatebutton guictrlcreatecheckbox "
"guictrlcreatecombo guictrlcreatecontextmenu guictrlcreatedate guictrlcreatedummy guictrlcreateedit "
"guictrlcreategraphic guictrlcreategroup guictrlcreateicon guictrlcreateinput guictrlcreatelabel "
"guictrlcreatelist guictrlcreatelistview guictrlcreatelistviewitem guictrlcreatemenu "
"guictrlcreatemenuitem guictrlcreatemonthcal guictrlcreateobj guictrlcreatepic guictrlcreateprogress "
"guictrlcreateradio guictrlcreateslider guictrlcreatetab guictrlcreatetabitem guictrlcreatetreeview "
"guictrlcreatetreeviewitem guictrlcreateupdown guictrldelete guictrlgethandle guictrlgetstate "
"guictrlread guictrlrecvmsg guictrlregisterlistviewsort guictrlsendmsg guictrlsendtodummy "
"guictrlsetbkcolor guictrlsetcolor guictrlsetcursor guictrlsetdata guictrlsetdefbkcolor "
"guictrlsetdefcolor guictrlsetfont guictrlsetgraphic guictrlsetimage guictrlsetlimit guictrlsetonevent "
"guictrlsetpos guictrlsetresizing guictrlsetstate guictrlsetstyle guictrlsettip guidelete "
"guigetcursorinfo guigetmsg guigetstyle guiregistermsg guisetaccelerators guisetbkcolor guisetcoord "
"guisetcursor guisetfont guisethelp guiseticon guisetonevent guisetstate guisetstyle guistartgroup "
"guiswitch hex hotkeyset httpsetproxy hwnd inetget inetgetsize inidelete iniread inireadsection "
"inireadsectionnames inirenamesection iniwrite iniwritesection inputbox int isadmin "
"isarray isbinary isbool isdeclared isdllstruct isfloat ishwnd isint iskeyword isnumber "
"isobj isptr isstring log memgetstats mod mouseclick mouseclickdrag mousedown mousegetcursor "
"mousegetpos mousemove mouseup mousewheel msgbox number objcreate objevent objevent "
"objget objname opt ping pixelchecksum pixelgetcolor pixelsearch pluginclose pluginopen "
"processclose processexists processgetstats processlist processsetpriority processwait "
"processwaitclose progressoff progresson progressset ptr random regdelete regenumkey "
"regenumval regread regwrite round run runas runaswait runwait send sendkeepactive "
"seterror setextended shellexecute shellexecutewait shutdown sin sleep soundplay soundsetwavevolume "
"splashimageon splashoff splashtexton sqrt srandom statusbargettext stderrread stdinwrite "
"stdioclose stdoutread string stringaddcr stringcompare stringformat stringinstr stringisalnum "
"stringisalpha stringisascii stringisdigit stringisfloat stringisint stringislower "
"stringisspace stringisupper stringisxdigit stringleft stringlen stringlower stringmid "
"stringregexp stringregexpreplace stringreplace stringright stringsplit stringstripcr "
"stringstripws stringtobinary stringtrimleft stringtrimright stringupper tan tcpaccept "
"tcpclosesocket tcpconnect tcplisten tcpnametoip tcprecv tcpsend tcpshutdown tcpstartup "
"timerdiff timerinit tooltip traycreateitem traycreatemenu traygetmsg trayitemdelete "
"trayitemgethandle trayitemgetstate trayitemgettext trayitemsetonevent trayitemsetstate "
"trayitemsettext traysetclick trayseticon traysetonevent traysetpauseicon traysetstate "
"traysettooltip traytip ubound udpbind udpclosesocket udpopen udprecv udpsend udpshutdown "
"udpstartup vargettype winactivate winactive winclose winexists winflash wingetcaretpos "
"wingetclasslist wingetclientsize wingethandle wingetpos wingetprocess wingetstate "
"wingettext wingettitle winkill winlist winmenuselectitem winminimizeall winminimizeallundo "
"winmove winsetontop winsetstate winsettitle winsettrans winwait winwaitactive winwaitclose "
"winwaitnotactive",
"@appdatacommondir @appdatadir @autoitexe @autoitpid @autoitunicode @autoitversion @autoitx64 "
"@com_eventobj @commonfilesdir @compiled @computername @comspec @cr @crlf @desktopcommondir "
"@desktopdepth @desktopdir @desktopheight @desktoprefresh @desktopwidth @documentscommondir "
"@error @exitcode @exitmethod @extended @favoritescommondir @favoritesdir @gui_ctrlhandle "
"@gui_ctrlid @gui_dragfile @gui_dragid @gui_dropid @gui_winhandle @homedrive @homepath @homeshare "
"@hotkeypressed @hour @inetgetactive @inetgetbytesread @ipaddress1 @ipaddress2 @ipaddress3 "
"@ipaddress4 @kblayout @lf @logondnsdomain @logondomain @logonserver @mday @min @mon @mydocumentsdir "
"@numparams @osbuild @oslang @osservicepack @ostype @osversion @processorarch @programfilesdir "
"@programscommondir @programsdir @scriptdir @scriptfullpath @scriptlinenumber @scriptname @sec "
"@startmenucommondir @startmenudir @startupcommondir @startupdir @sw_disable @sw_enable @sw_hide @sw_lock "
"@sw_maximize @sw_minimize @sw_restore @sw_show @sw_showdefault @sw_showmaximized "
"@sw_showminimized @sw_showminnoactive @sw_showna @sw_shownoactivate @sw_shownormal "
"@sw_unlock @systemdir @tab @tempdir @tray_id @trayiconflashing @trayiconvisible @username "
"@userprofiledir @wday @windowsdir @workingdir @yday @year",
"{!} {#} {^} {{} {}} {+} {alt} {altdown} {altup} {appskey} "
"{asc} {backspace} {break} {browser_back} {browser_favorites} {browser_forward} {browser_home} "
"{browser_refresh} {browser_search} {browser_stop} {bs} {capslock} {ctrldown} {ctrlup} "
"{del} {delete} {down} {end} {enter} {esc} {escape} {f1} {f10} {f11} {f12} {f2} {f3} "
"{f4} {f5} {f6} {f7} {f8} {f9} {home} {ins} {insert} {lalt} {launch_app1} {launch_app2} "
"{launch_mail} {launch_media} {lctrl} {left} {lshift} {lwin} {lwindown} {lwinup} {media_next} "
"{media_play_pause} {media_prev} {media_stop} {numlock} {numpad0} {numpad1} {numpad2} "
"{numpad3} {numpad4} {numpad5} {numpad6} {numpad7} {numpad8} {numpad9} {numpadadd} "
"{numpaddiv} {numpaddot} {numpadenter} {numpadmult} {numpadsub} {pause} {pgdn} {pgup} "
"{printscreen} {ralt} {rctrl} {right} {rshift} {rwin} {rwindown} {rwinup} {scrolllock} "
"{shiftdown} {shiftup} {sleep} {space} {tab} {up} {volume_down} {volume_mute} {volume_up}",
"#ce #comments-end #comments-start #cs #include #include-once #notrayicon #requireadmin",
"#autoit3wrapper_au3check_parameters #autoit3wrapper_au3check_stop_onwarning "
"#autoit3wrapper_change2cui #autoit3wrapper_compression #autoit3wrapper_cvswrapper_parameters "
"#autoit3wrapper_icon #autoit3wrapper_outfile #autoit3wrapper_outfile_type #autoit3wrapper_plugin_funcs "
"#autoit3wrapper_res_comment #autoit3wrapper_res_description #autoit3wrapper_res_field "
"#autoit3wrapper_res_file_add #autoit3wrapper_res_fileversion #autoit3wrapper_res_fileversion_autoincrement "
"#autoit3wrapper_res_icon_add #autoit3wrapper_res_language #autoit3wrapper_res_legalcopyright "
"#autoit3wrapper_res_requestedexecutionlevel #autoit3wrapper_res_savesource #autoit3wrapper_run_after "
"#autoit3wrapper_run_au3check #autoit3wrapper_run_before #autoit3wrapper_run_cvswrapper "
"#autoit3wrapper_run_debug_mode #autoit3wrapper_run_obfuscator #autoit3wrapper_run_tidy "
"#autoit3wrapper_tidy_stop_onerror #autoit3wrapper_useansi #autoit3wrapper_useupx "
"#autoit3wrapper_usex64 #autoit3wrapper_version #endregion #forceref #obfuscator_ignore_funcs "
"#obfuscator_ignore_variables #obfuscator_parameters #region #tidy_parameters",
"", "", "" };


EDITLEXER lexAU3 = { SCLEX_AU3, 63276, L"AutoIt3 Script", L"au3", L"", &KeyWords_AU3, {
                     { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                     { MULTI_STYLE(SCE_AU3_COMMENT,SCE_AU3_COMMENTBLOCK,0,0), 63127, L"Comment", L"fore:#008000", L"" },
                     { SCE_AU3_NUMBER, 63130, L"Number", L"fore:#008080", L"" },
                     { SCE_AU3_FUNCTION, 63277, L"Function", L"fore:#0000FF", L"" },
                     { SCE_AU3_KEYWORD, 63128, L"Keyword", L"fore:#0000FF", L"" },
                     { SCE_AU3_MACRO, 63278, L"Macro", L"fore:#0080FF", L"" },
                     { SCE_AU3_STRING, 63131, L"String", L"fore:#008080", L"" },
                     { SCE_AU3_OPERATOR, 63132, L"Operator", L"#C000C0", L"" },
                     { SCE_AU3_VARIABLE, 63249, L"Variable", L"fore:#808000", L"" },
                     { SCE_AU3_SENT, 63279, L"Send Key", L"fore:#FF0000", L"" },
                     { SCE_AU3_PREPROCESSOR, 63133, L"Preprocessor", L"fore:#FF8000", L"" },
                     { SCE_AU3_SPECIAL, 63280, L"Special", L"fore:#FF8000", L"" },
                     { -1, 00000, L"", L"", L"" } } };


EDITLEXER lexLATEX = { SCLEX_LATEX, 63281, L"LaTeX Files", L"tex; latex; sty", L"", &KeyWords_NULL, {
                       { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                       { SCE_L_COMMAND, 63236, L"Command", L"fore:#0000FF", L"" },
                       { SCE_L_TAG, 63282, L"Tag", L"fore:#0000FF", L"" },
                       { SCE_L_MATH, 63283, L"Math", L"fore:#FF0000", L"" },
                       { SCE_L_COMMENT, 63127, L"Comment", L"fore:#008000", L"" },
                       { -1, 00000, L"", L"", L"" } } };


EDITLEXER lexANSI = { SCLEX_NULL, 63258, L"ANSI Art", L"nfo; diz", L"", &KeyWords_NULL, {
                      { STYLE_DEFAULT, 63126, L"Default", L"font:Lucida Console", L"" },
                      { STYLE_LINENUMBER, 63101, L"Margins and Line Numbers", L"font:Lucida Console; size:-2", L"" },
                      { STYLE_BRACELIGHT, 63102, L"Matching Braces", L"size:+0", L"" },
                      { STYLE_BRACEBAD, 63103, L"Matching Braces Error", L"size:+0", L"" },
                      { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_AHK = {
"break continue else exit exitapp gosub goto if ifequal ifexist ifgreater ifgreaterorequal ifinstring "
"ifless iflessorequal ifmsgbox ifnotequal ifnotexist ifnotinstring ifwinactive ifwinexist ifwinnotactive "
"ifwinnotexist loop onexit pause repeat return setbatchlines settimer sleep suspend static global local byref "
"autotrim blockinput clipwait control controlclick controlfocus controlget controlgetfocus controlgetpos "
"controlgettext controlmove controlsend controlsendraw controlsettext coordmode critical detecthiddentext "
"detecthiddenwindows drive driveget drivespacefree edit endrepeat envadd envdiv envget envmult envset envsub "
"envupdate fileappend filecopy filecopydir filecreatedir filecreateshortcut filedelete filegetattrib "
"filegetshortcut filegetsize filegettime filegetversion fileinstall filemove filemovedir fileread filereadline "
"filerecycle filerecycleempty fileremovedir fileselectfile fileselectfolder filesetattrib filesettime formattime "
"getkeystate groupactivate groupadd groupclose groupdeactivate gui guicontrol guicontrolget hideautoitwin hotkey "
"imagesearch inidelete iniread iniwrite input inputbox keyhistory keywait listhotkeys listlines listvars menu "
"mouseclick mouseclickdrag mousegetpos mousemove msgbox outputdebug pixelgetcolor pixelsearch postmessage "
"process progress random regdelete regread regwrite reload run runas runwait send sendevent sendinput "
"sendmessage sendmode sendplay sendraw setcapslockstate setcontroldelay setdefaultmousespeed setenv setformat "
"setkeydelay setmousedelay setnumlockstate setscrolllockstate setstorecapslockmode settitlematchmode setwindelay "
"setworkingdir shutdown sort soundbeep soundget soundgetwavevolume soundplay soundset soundsetwavevolume "
"splashimage splashtextoff splashtexton splitpath statusbargettext statusbarwait stringcasesense stringgetpos stringleft "
"stringlen stringlower stringmid stringreplace stringright stringsplit stringtrimleft stringtrimright stringupper "
"sysget thread tooltip transform traytip urldownloadtofile winactivate winactivatebottom winclose winget "
"wingetactivestats wingetactivetitle wingetclass wingetpos wingettext wingettitle winhide winkill winmaximize "
"winmenuselectitem winminimize winminimizeall winminimizeallundo winmove winrestore winset winsettitle "
"winshow winwait winwaitactive winwaitclose winwaitnotactive abs acos asc asin atan ceil chr cos dllcall exp "
"fileexist floor getkeystate il_add il_create il_destroy instr islabel ln log lv_add lv_delete lv_deletecol "
"lv_getcount lv_getnext lv_gettext lv_insert lv_insertcol lv_modify lv_modifycol lv_setimagelist mod onmessage round "
"regexmatch regexreplace sb_seticon sb_setparts sb_settext sin sqrt strlen substr tan tv_add tv_delete tv_getchild "
"tv_getcount tv_getnext tv_get tv_getparent tv_getprev tv_getselection tv_gettext tv_modify varsetcapacity winactive winexist "
"allowsamelinecomments clipboardtimeout commentflag errorstdout escapechar hotkeyinterval hotkeymodifiertimeout "
"hotstring ifwinactive ifwinexist include includeagain installkeybdhook installmousehook keyhistory ltrim "
"maxhotkeysperinterval maxmem maxthreads maxthreadsbuffer maxthreadsperhotkey noenv notrayicon persistent singleinstance "
"usehook winactivateforce shift lshift rshift alt lalt ralt control lcontrol rcontrol ctrl lctrl rctrl lwin rwin appskey "
"altdown altup shiftdown shiftup ctrldown ctrlup lwindown lwinup rwindown rwinup lbutton rbutton mbutton wheelup wheeldown "
"xbutton1 xbutton2 joy1 joy2 joy3 joy4 joy5 joy6 joy7 joy8 joy9 joy10 joy11 joy12 joy13 joy14 joy15 joy16 joy17 joy18 joy19 "
"joy20 joy21 joy22 joy23 joy24 joy25 joy26 joy27 joy28 joy29 joy30 joy31 joy32 joyx joyy joyz joyr joyu joyv joypov joyname "
"joybuttons joyaxes joyinfo space tab enter escape esc backspace bs delete del insert ins pgup pgdn home end up down left right "
"printscreen ctrlbreak pause scrolllock capslock numlock numpad0 numpad1 numpad2 numpad3 numpad4 numpad5 numpad6 numpad7 "
"numpad8 numpad9 numpadmult numpadadd numpadsub numpaddiv numpaddot numpaddel numpadins numpadclear numpadup numpaddown "
"numpadleft numpadright numpadhome numpadend numpadpgup numpadpgdn numpadenter f1 f2 f3 f4 f5 f6 f7 f8 f9 f10 f11 f12 "
"f13 f14 f15 f16 f17 f18 f19 f20 f21 f22 f23 f24 browser_back browser_forward browser_refresh browser_stop browser_search "
"browser_favorites browser_home volume_mute volume_down volume_up media_next media_prev media_stop media_play_pause "
"launch_mail launch_media launch_app1 launch_app2 blind click raw a_ahkpath a_ahkversion a_appdata a_appdatacommon "
"a_autotrim a_batchlines a_caretx a_carety a_computername a_controldelay a_cursor a_dd a_ddd a_dddd a_defaultmousespeed "
"a_desktop a_desktopcommon a_detecthiddentext a_detecthiddenwindows a_endchar a_eventinfo a_exitreason a_formatfloat "
"a_formatinteger a_gui a_guievent a_guicontrol a_guicontrolevent a_guiheight a_guiwidth a_guix a_guiy a_hour "
"a_iconfile a_iconhidden a_iconnumber a_icontip a_index a_ipaddress1 a_ipaddress2 a_ipaddress3 a_ipaddress4 a_isadmin a_iscompiled "
"a_issuspended a_keydelay a_language a_lasterror a_linefile a_linenumber a_loopfield a_loopfileattrib a_loopfiledir a_loopfileext "
"a_loopfilefullpath a_loopfilelongpath a_loopfilename a_loopfileshortname a_loopfileshortpath a_loopfilesize a_loopfilesizekb "
"a_loopfilesizemb a_loopfiletimeaccessed a_loopfiletimecreated a_loopfiletimemodified a_loopreadline a_loopregkey "
"a_loopregname a_loopregsubkey a_loopregtimemodified a_loopregtype a_mday a_min a_mm a_mmm a_mmmm a_mon a_mousedelay a_msec a_mydocuments "
"a_now a_nowutc a_numbatchlines a_ostype a_osversion a_priorhotkey a_programfiles a_programs a_programscommon a_screenheight a_screenwidth "
"a_scriptdir a_scriptfullpath a_scriptname a_sec a_space a_startmenu a_startmenucommon a_startup a_startupcommon a_stringcasesense a_tab a_temp "
"a_thishotkey a_thismenu a_thismenuitem a_thismenuitempos a_tickcount a_timeidle a_timeidlephysical a_timesincepriorhotkey a_timesincethishotkey "
"a_titlematchmode a_titlematchmodespeed a_username a_wday a_windelay a_windir a_workingdir a_yday a_year a_yweek a_yyyy "
"clipboard clipboardall comspec errorlevel programfiles true false ltrim rtrim join ahk_id ahk_pid ahk_class ahk_group "
"processname minmax controllist statuscd filesystem setlabel alwaysontop mainwindow nomainwindow useerrorlevel "
"altsubmit hscroll vscroll imagelist wantctrla wantf2 vis visfirst wantreturn backgroundtrans minimizebox maximizebox sysmenu toolwindow exstyle "
"check3 checkedgray readonly notab lastfound lastfoundexist alttab shiftalttab alttabmenu alttabandmenu alttabmenudismiss "
"controllisthwnd hwnd deref pow bitnot bitand bitor bitxor bitshiftleft bitshiftright sendandmouse mousemove mousemouveoff "
"hkey_local_machine hkey_users hkey_current_user hkey_classes_root hkey_current_config hklm hku hkcu hkcr hkcc "
"reg_sz reg_expand_sz reg_multi_sz reg_dword reg_qword reg_binary reg_link reg_resource_list reg_full_resource_descriptor "
"reg_resource_requirements_list reg_dword_big_endian regex pixel mouse screen relative rgb low belownormal normal abovenormal high realtime "
"between contains in is integer float number digit xdigit alpha upper lower alnum time date not or and topmost top bottom transparent "
"transcolor redraw region id idlast count list capacity eject lock unlock label serial type status seconds minutes hours days "
"read parse logoff close error single shutdown menu exit reload tray add rename check uncheck togglecheck enable disable toggleenable default "
"nodefault standard nostandard color delete deleteall icon noicon tip click show edit progress hotkey text picture pic groupbox button "
"checkbox radio dropdownlist ddl combobox statusbar treeview listbox listview datetime monthcal updown slider tab iconsmall tile report "
"sortdesc nosort nosorthdr grid hdr autosize range xm ym ys xs xp yp font resize owner submit nohide minimize maximize restore noactivate na "
"cancel destroy center margin owndialogs guiescape guiclose guisize guicontextmenu guidropfiles tabstop section wrap border top bottom buttons "
"expand first lines number uppercase lowercase limit password multi group background bold italic strike underline norm theme caption delimiter "
"flash style checked password hidden left right center section move focus hide choose choosestring text pos enabled disabled visible "
"notimers interrupt priority waitclose unicode yes no ok cancel abort retry ignore on off all send ",
"", "", "", "", "", "", "", "" };


EDITLEXER lexAHK = { SCLEX_AHK, 63305, L"AutoHotkey Script", L"ahk; ia; scriptlet", L"", &KeyWords_AHK, {
                    { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                    //{ SCE_AHK_DEFAULT, L"Default", L"", L"" },
                    { MULTI_STYLE(SCE_AHK_COMMENTLINE,SCE_AHK_COMMENTBLOCK,0,0), 63127, L"Comment", L"fore:#008000", L"" },
                    { SCE_AHK_ESCAPE, 63306, L"Escape", L"fore:#FF8000", L"" },
                    { SCE_AHK_SYNOPERATOR, 63307, L"Syntax Operator", L"fore:#7F200F", L"" },
                    { SCE_AHK_EXPOPERATOR, 63308, L"Expression operator", L"", L"" },
                    { SCE_AHK_STRING, 63131, L"String", L"fore:#FF8000", L"" },
                    { SCE_AHK_NUMBER, 63130, L"Number", L"fore:#008080", L"" },
                    { SCE_AHK_IDENTIFIER, 63129, L"Identifier", L"fore:#C80000", L"" },
                    { SCE_AHK_VARREF, 63309, L"Variable dereferencing", L"fore:#C80000; back:#F0F0FF", L"" },
                    { SCE_AHK_LABEL, 63235, L"Label", L"back:#FFFFD1", L"" },
                    { SCE_AHK_WORD_CF, 63310, L"Flow of control", L"fore:#0000C8", L"" },
                    { SCE_AHK_WORD_CMD, 63236, L"Command", L"fore:#0000A0", L"" },
                    { SCE_AHK_WORD_FN, 63277, L"Function", L"fore:#0000FF", L"" },
                    { SCE_AHK_WORD_DIR, 63203, L"Directive", L"fore:#008000", L"" },
                    { SCE_AHK_WORD_KB, 63311, L"Keys & buttons", L"fore:#FF00FF", L"" },
                    { SCE_AHK_WORD_VAR, 63312, L"Built-in Variables", L"back:#F0F0FF", L"" },
                    { SCE_AHK_WORD_SP, 63280, L"Special", L"fore:#FF00FF; italics", L"" },
                    //{ SCE_AHK_WORD_UD, 63106, L"User Defined", L"fore:#800020", L"" },
                    { SCE_AHK_VARREFKW, 63313, L"Variable keyword", L"fore:#FF00FF; bold; back:#F0F0FF", L"" },
                    { SCE_AHK_ERROR, 63260, L"Error", L"back:#FFC0C0", L"" },
                    { -1, 00000, L"", L"", L"" } } };


KEYWORDLIST KeyWords_CMAKE = {
"add_custom_command add_custom_target add_definitions add_dependencies add_executable add_library "
"add_subdirectory add_test aux_source_directory build_command build_name cmake_minimum_required "
"configure_file create_test_sourcelist else elseif enable_language enable_testing endforeach endif "
"endmacro endwhile exec_program execute_process export_library_dependencies file find_file find_library "
"find_package find_path find_program fltk_wrap_ui foreach get_cmake_property get_directory_property "
"get_filename_component get_source_file_property get_target_property get_test_property if include "
"include_directories include_external_msproject include_regular_expression install install_files "
"install_programs install_targets link_directories link_libraries list load_cache load_command "
"macro make_directory mark_as_advanced math message option output_required_files project qt_wrap_cpp "
"qt_wrap_ui remove remove_definitions separate_arguments set set_directory_properties set_source_files_properties "
"set_target_properties set_tests_properties site_name source_group string subdir_depends subdirs "
"target_link_libraries try_compile try_run use_mangled_mesa utility_source variable_requires vtk_make_instantiator "
"vtk_wrap_java vtk_wrap_python vtk_wrap_tcl while write_file",
"ABSOLUTE ABSTRACT ADDITIONAL_MAKE_CLEAN_FILES ALL AND APPEND ARGS ASCII BEFORE CACHE CACHE_VARIABLES "
"CLEAR COMMAND COMMANDS COMMAND_NAME COMMENT COMPARE COMPILE_FLAGS COPYONLY DEFINED DEFINE_SYMBOL "
"DEPENDS DOC EQUAL ESCAPE_QUOTES EXCLUDE EXCLUDE_FROM_ALL EXISTS EXPORT_MACRO EXT EXTRA_INCLUDE "
"FATAL_ERROR FILE FILES FORCE FUNCTION GENERATED GLOB GLOB_RECURSE GREATER GROUP_SIZE HEADER_FILE_ONLY "
"HEADER_LOCATION IMMEDIATE INCLUDES INCLUDE_DIRECTORIES INCLUDE_INTERNALS INCLUDE_REGULAR_EXPRESSION "
"LESS LINK_DIRECTORIES LINK_FLAGS LOCATION MACOSX_BUNDLE MACROS MAIN_DEPENDENCY MAKE_DIRECTORY MATCH "
"MATCHALL MATCHES MODULE NAME NAME_WE NOT NOTEQUAL NO_SYSTEM_PATH OBJECT_DEPENDS OPTIONAL OR OUTPUT "
"OUTPUT_VARIABLE PATH PATHS POST_BUILD POST_INSTALL_SCRIPT PREFIX PREORDER PRE_BUILD PRE_INSTALL_SCRIPT "
"PRE_LINK PROGRAM PROGRAM_ARGS PROPERTIES QUIET RANGE READ REGEX REGULAR_EXPRESSION REPLACE REQUIRED "
"RETURN_VALUE RUNTIME_DIRECTORY SEND_ERROR SHARED SOURCES STATIC STATUS STREQUAL STRGREATER STRLESS "
"SUFFIX TARGET TOLOWER TOUPPER VAR VARIABLES VERSION WIN32 WRAP_EXCLUDE WRITE APPLE MINGW MSYS CYGWIN "
"BORLAND WATCOM MSVC MSVC_IDE MSVC60 MSVC70 MSVC71 MSVC80 CMAKE_COMPILER_2005 OFF ON",
"", "", "", "", "", "", "" };


EDITLEXER lexCmake = { SCLEX_CMAKE, 63324, L"Cmake Script", L"cmake; ctest", L"", &KeyWords_CMAKE, {
                     { STYLE_DEFAULT, 63126, L"Default", L"", L"" },
                     //{ SCE_CMAKE_DEFAULT, L"Default", L"", L"" },
                     { SCE_CMAKE_COMMENT, 63127, L"Comment", L"fore:#008000", L"" },
                     { MULTI_STYLE(SCE_CMAKE_STRINGDQ,SCE_CMAKE_STRINGLQ,SCE_CMAKE_STRINGRQ,0), 63131, L"String", L"back:#EEEEEE;fore:#7F007F", L"" },
                     { SCE_CMAKE_COMMANDS, 63277, L"Function", L"fore:#00007F", L"" },
                     { SCE_CMAKE_PARAMETERS, 63294, L"Parameter", L"fore:#7F200F", L"" },
                     { SCE_CMAKE_VARIABLE, 63249, L"Variable", L"fore:#CC3300", L"" },
                     { SCE_CMAKE_WHILEDEF, 63325, L"While def", L"fore:#00007F", L"" },
                     { SCE_CMAKE_FOREACHDEF, 63326, L"For each def", L"fore:#00007F", L"" },
                     { SCE_CMAKE_IFDEFINEDEF, 63327, L"If def", L"fore:#00007F", L"" },
                     { SCE_CMAKE_MACRODEF, 63328, L"Macro def", L"fore:#00007F", L"" },
                     { SCE_CMAKE_STRINGVAR, 63329, L"Variable within string", L"back:#EEEEEE;fore:#CC3300", L"" },
                     { SCE_CMAKE_NUMBER, 63130, L"Number", L"fore:#008080", L"" },
                     //{ SCE_CMAKE_USERDEFINED, 63106, L"User Defined", L"fore:#800020", L"" },
                     { -1, 00000, L"", L"", L"" } } };


// This array holds all the lexers...
PEDITLEXER pLexArray[NUMLEXERS] =
{
  &lexDefault,
  &lexANSI,
  &lexCONF, //Apache Config Scripts
  &lexASM,
  &lexAHK,
  &lexAU3,
  &lexBAT,
  &lexINI,
  &lexCPP,
  &lexCS,
  &lexCmake,
  &lexCSS,
  &lexDIFF,
  &lexINNO,
  &lexJAVA,
  &lexJS,
  &lexMAK,
  &lexLATEX,
  &lexLUA,
  &lexNSIS,
  &lexPAS,
  &lexPL,
  &lexPS,
  &lexPY,
  &lexRC,
  &lexRUBY,
  &lexBASH,
  &lexSQL,
  &lexTCL,
  &lexVBS,
  &lexVB,
  &lexHTML,
  &lexXML
};


// Currently used lexer
PEDITLEXER pLexCurrent = &lexDefault;
COLORREF crCustom[16];
BOOL bUse2ndDefaultStyle;
BOOL fStylesModified = FALSE;
BOOL fWarnedNoIniFile = FALSE;
BOOL fIsConsolasAvailable = FALSE;
int iBaseFontSize = 10;
int iDefaultLexer;
BOOL bAutoSelect;
int cxStyleSelectDlg;
int cyStyleSelectDlg;
extern int  iDefaultCodePage;
extern int  iDefaultCharSet;
extern BOOL bHiliteCurrentLine;


//=============================================================================
//
//  Style_Load()
//
void Style_Load()
{
  int i,iLexer;
  WCHAR tch[32];
  WCHAR *pIniSection = LocalAlloc(LPTR,sizeof(WCHAR)*32*1024);
  int   cchIniSection = (int)LocalSize(pIniSection)/sizeof(WCHAR);

  // Custom colors
  crCustom [0] = RGB(0x00,0x00,0x00);
  crCustom [1] = RGB(0x0A,0x24,0x6A);
  crCustom [2] = RGB(0x3A,0x6E,0xA5);
  crCustom [3] = RGB(0x00,0x3C,0xE6);
  crCustom [4] = RGB(0x00,0x66,0x33);
  crCustom [5] = RGB(0x60,0x80,0x20);
  crCustom [6] = RGB(0x64,0x80,0x00);
  crCustom [7] = RGB(0xA4,0x60,0x00);
  crCustom [8] = RGB(0xFF,0xFF,0xFF);
  crCustom [9] = RGB(0xFF,0xFF,0xE2);
  crCustom[10] = RGB(0xFF,0xF1,0xA8);
  crCustom[11] = RGB(0xFF,0xC0,0x00);
  crCustom[12] = RGB(0xFF,0x40,0x00);
  crCustom[13] = RGB(0xC8,0x00,0x00);
  crCustom[14] = RGB(0xB0,0x00,0xB0);
  crCustom[15] = RGB(0xB2,0x8B,0x40);

  LoadIniSection(L"Custom Colors",pIniSection,cchIniSection);
  for (i = 0; i < 16; i++) {
    int itok;
    int irgb;
    WCHAR wch[32];
    wsprintf(tch,L"%02i",i+1);
    if (IniSectionGetString(pIniSection,tch,L"",wch,COUNTOF(wch))) {
      if (wch[0] == L'#') {
        itok = swscanf(CharNext(wch),L"%x",&irgb);
        if (itok == 1)
          crCustom[i] = RGB((irgb&0xFF0000) >> 16,(irgb&0xFF00) >> 8,irgb&0xFF);
      }
    }
  }

  LoadIniSection(L"Styles",pIniSection,cchIniSection);
  // 2nd default
  bUse2ndDefaultStyle = (IniSectionGetInt(pIniSection,L"Use2ndDefaultStyle",0)) ? 1 : 0;

  // default scheme
  iDefaultLexer = IniSectionGetInt(pIniSection,L"DefaultScheme",0);
  iDefaultLexer = min(max(iDefaultLexer,0),NUMLEXERS-1);

  // auto select
  bAutoSelect = (IniSectionGetInt(pIniSection,L"AutoSelect",1)) ? 1 : 0;

  // scheme select dlg dimensions
  cxStyleSelectDlg = IniSectionGetInt(pIniSection,L"SelectDlgSizeX",304);
  cxStyleSelectDlg = max(cxStyleSelectDlg,0);

  cyStyleSelectDlg = IniSectionGetInt(pIniSection,L"SelectDlgSizeY",0);
  cyStyleSelectDlg = max(cyStyleSelectDlg,324);

  for (iLexer = 0; iLexer < NUMLEXERS; iLexer++) {
    LoadIniSection(pLexArray[iLexer]->pszName,pIniSection,cchIniSection);
    if (!IniSectionGetString(pIniSection,L"FileNameExtensions",pLexArray[iLexer]->pszDefExt,
          pLexArray[iLexer]->szExtensions,COUNTOF(pLexArray[iLexer]->szExtensions)))
      lstrcpyn(pLexArray[iLexer]->szExtensions,pLexArray[iLexer]->pszDefExt,
        COUNTOF(pLexArray[iLexer]->szExtensions));
    i = 0;
    while (pLexArray[iLexer]->Styles[i].iStyle != -1) {
      IniSectionGetString(pIniSection,pLexArray[iLexer]->Styles[i].pszName,
        pLexArray[iLexer]->Styles[i].pszDefault,
        pLexArray[iLexer]->Styles[i].szValue,
        COUNTOF(pLexArray[iLexer]->Styles[i].szValue));
      i++;
    }
  }
  LocalFree(pIniSection);
}


//=============================================================================
//
//  Style_Save()
//
void Style_Save()
{
  int i,iLexer;
  WCHAR tch[32];
  WCHAR *pIniSection = LocalAlloc(LPTR,sizeof(WCHAR)*32*1024);
  int   cchIniSection = (int)LocalSize(pIniSection)/sizeof(WCHAR);

  // Custom colors
  for (i = 0; i < 16; i++) {
    WCHAR wch[32];
    wsprintf(tch,L"%02i",i+1);
    wsprintf(wch,L"#%02X%02X%02X",
      (int)GetRValue(crCustom[i]),(int)GetGValue(crCustom[i]),(int)GetBValue(crCustom[i]));
    IniSectionSetString(pIniSection,tch,wch);
  }
  SaveIniSection(L"Custom Colors",pIniSection);
  ZeroMemory(pIniSection,cchIniSection);

  // auto select
  IniSectionSetInt(pIniSection,L"Use2ndDefaultStyle",bUse2ndDefaultStyle);

  // default scheme
  IniSectionSetInt(pIniSection,L"DefaultScheme",iDefaultLexer);

  // auto select
  IniSectionSetInt(pIniSection,L"AutoSelect",bAutoSelect);

  // scheme select dlg dimensions
  IniSectionSetInt(pIniSection,L"SelectDlgSizeX",cxStyleSelectDlg);
  IniSectionSetInt(pIniSection,L"SelectDlgSizeY",cyStyleSelectDlg);

  SaveIniSection(L"Styles",pIniSection);

  if (!fStylesModified) {
    LocalFree(pIniSection);
    return;
  }

  ZeroMemory(pIniSection,cchIniSection);
  for (iLexer = 0; iLexer < NUMLEXERS; iLexer++) {
    IniSectionSetString(pIniSection,L"FileNameExtensions",pLexArray[iLexer]->szExtensions);
    i = 0;
    while (pLexArray[iLexer]->Styles[i].iStyle != -1) {
      IniSectionSetString(pIniSection,pLexArray[iLexer]->Styles[i].pszName,pLexArray[iLexer]->Styles[i].szValue);
      i++;
    }
    SaveIniSection(pLexArray[iLexer]->pszName,pIniSection);
    ZeroMemory(pIniSection,cchIniSection);
  }
  LocalFree(pIniSection);
}


//=============================================================================
//
//  Style_Import()
//
BOOL Style_Import(HWND hwnd)
{
  WCHAR szFile[MAX_PATH * 2] = L"";
  WCHAR szFilter[256];
  OPENFILENAME ofn;

  ZeroMemory(&ofn,sizeof(OPENFILENAME));
  GetString(IDS_FILTER_INI,szFilter,COUNTOF(szFilter));
  PrepareFilterStr(szFilter);

  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFilter = szFilter;
  ofn.lpstrFile = szFile;
  ofn.lpstrDefExt = L"ini";
  ofn.nMaxFile = COUNTOF(szFile);
  ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_DONTADDTORECENT
            | OFN_PATHMUSTEXIST | OFN_SHAREAWARE /*| OFN_NODEREFERENCELINKS*/;

  if (GetOpenFileName(&ofn)) {

    int i,iLexer;
    WCHAR *pIniSection = LocalAlloc(LPTR,sizeof(WCHAR)*32*1024);
    int   cchIniSection = (int)LocalSize(pIniSection)/sizeof(WCHAR);

    for (iLexer = 0; iLexer < NUMLEXERS; iLexer++) {
      if (GetPrivateProfileSection(pLexArray[iLexer]->pszName,pIniSection,cchIniSection,szFile)) {
        if (!IniSectionGetString(pIniSection,L"FileNameExtensions",pLexArray[iLexer]->pszDefExt,
              pLexArray[iLexer]->szExtensions,COUNTOF(pLexArray[iLexer]->szExtensions)))
          lstrcpyn(pLexArray[iLexer]->szExtensions,pLexArray[iLexer]->pszDefExt,
            COUNTOF(pLexArray[iLexer]->szExtensions));
        i = 0;
        while (pLexArray[iLexer]->Styles[i].iStyle != -1) {
          IniSectionGetString(pIniSection,pLexArray[iLexer]->Styles[i].pszName,
            pLexArray[iLexer]->Styles[i].pszDefault,
            pLexArray[iLexer]->Styles[i].szValue,
            COUNTOF(pLexArray[iLexer]->Styles[i].szValue));
          i++;
        }
      }
    }
    LocalFree(pIniSection);
    return(TRUE);
  }
  return(FALSE);
}

//=============================================================================
//
//  Style_Export()
//
BOOL Style_Export(HWND hwnd)
{
  WCHAR szFile[MAX_PATH * 2] = L"";
  WCHAR szFilter[256];
  OPENFILENAME ofn;
  DWORD dwError = ERROR_SUCCESS;

  ZeroMemory(&ofn,sizeof(OPENFILENAME));
  GetString(IDS_FILTER_INI,szFilter,COUNTOF(szFilter));
  PrepareFilterStr(szFilter);

  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFilter = szFilter;
  ofn.lpstrFile = szFile;
  ofn.lpstrDefExt = L"ini";
  ofn.nMaxFile = COUNTOF(szFile);
  ofn.Flags = /*OFN_FILEMUSTEXIST |*/ OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_DONTADDTORECENT
            | OFN_PATHMUSTEXIST | OFN_SHAREAWARE /*| OFN_NODEREFERENCELINKS*/ | OFN_OVERWRITEPROMPT;

  if (GetSaveFileName(&ofn)) {

    int i,iLexer;
    WCHAR *pIniSection = LocalAlloc(LPTR,sizeof(WCHAR)*32*1024);
    int   cchIniSection = (int)LocalSize(pIniSection)/sizeof(WCHAR);

    for (iLexer = 0; iLexer < NUMLEXERS; iLexer++) {
      IniSectionSetString(pIniSection,L"FileNameExtensions",pLexArray[iLexer]->szExtensions);
      i = 0;
      while (pLexArray[iLexer]->Styles[i].iStyle != -1) {
        IniSectionSetString(pIniSection,pLexArray[iLexer]->Styles[i].pszName,pLexArray[iLexer]->Styles[i].szValue);
        i++;
      }
      if (!WritePrivateProfileSection(pLexArray[iLexer]->pszName,pIniSection,szFile))
        dwError = GetLastError();
      ZeroMemory(pIniSection,cchIniSection);
    }
    LocalFree(pIniSection);

    if (dwError != ERROR_SUCCESS) {
      MsgBox(MBINFO,IDS_EXPORT_FAIL,szFile);
    }
    return(TRUE);
  }
  return(FALSE);
}


//=============================================================================
//
//  Style_SetLexer()
//
void Style_SetLexer(HWND hwnd,PEDITLEXER pLexNew)
{
  int i;
  //WCHAR *p;
  int rgb;
  int iValue;
  int iIdx;
  int iStyleBits;
  WCHAR wchCaretStyle[64] = L"";

  // Select default if NULL is specified
  if (!pLexNew)
    pLexNew = pLexArray[iDefaultLexer];

  // Lexer
  SendMessage(hwnd,SCI_SETLEXER,pLexNew->iLexer,0);

  iStyleBits = (int)SendMessage(hwnd,SCI_GETSTYLEBITSNEEDED,0,0);
  SendMessage(hwnd,SCI_SETSTYLEBITS,(WPARAM)iStyleBits,0);

  if (pLexNew->iLexer == SCLEX_XML)
    SendMessage(hwnd,SCI_SETPROPERTY,(WPARAM)"lexer.xml.allow.scripts",(LPARAM)"1");
  if (pLexNew->iLexer == SCLEX_CPP) {
    SendMessage(hwnd,SCI_SETPROPERTY,(WPARAM)"styling.within.preprocessor",(LPARAM)"1");
    SendMessage(hwnd,SCI_SETPROPERTY,(WPARAM)"lexer.cpp.track.preprocessor",(LPARAM)"0");
    SendMessage(hwnd,SCI_SETPROPERTY,(WPARAM)"lexer.cpp.update.preprocessor",(LPARAM)"0");
  }
  else if (pLexNew->iLexer == SCLEX_PASCAL)
    SendMessage(hwnd,SCI_SETPROPERTY,(WPARAM)"lexer.pascal.smart.highlighting",(LPARAM)"1");
  else if (pLexNew->iLexer == SCLEX_SQL) {
    SendMessage(hwnd,SCI_SETPROPERTY,(WPARAM)"sql.backslash.escapes",(LPARAM)"1");
    SendMessage(hwnd,SCI_SETPROPERTY,(WPARAM)"lexer.sql.backticks.identifier",(LPARAM)"1");
    SendMessage(hwnd,SCI_SETPROPERTY,(WPARAM)"lexer.sql.numbersign.comment",(LPARAM)"1");
  }
  else if (pLexNew->iLexer == SCLEX_NSIS)
    SciCall_SetProperty("nsis.ignorecase", "1");

  // Code folding
  SciCall_SetProperty("fold", "1");
  SciCall_SetProperty("fold.compact", "0");
  SciCall_SetProperty("fold.comment", "1");
  SciCall_SetProperty("fold.html", "1");
  SciCall_SetProperty("fold.preprocessor", "1");

  // Add KeyWord Lists
  for (i = 0; i < 9; i++)
    SendMessage(hwnd,SCI_SETKEYWORDS,i,(LPARAM)pLexNew->pKeyWords->pszKeyWords[i]);

  // Use 2nd default style
  iIdx = (bUse2ndDefaultStyle) ? 12 : 0;

  // Font quality setup, check availability of Consolas
  Style_SetFontQuality(hwnd,lexDefault.Styles[0+iIdx].szValue);
  fIsConsolasAvailable = IsFontAvailable(L"Consolas");

  // Clear
  SendMessage(hwnd,SCI_CLEARDOCUMENTSTYLE,0,0);

  // Default Values are always set
  SendMessage(hwnd,SCI_STYLERESETDEFAULT,0,0);
  SendMessage(hwnd,SCI_STYLESETCHARACTERSET,STYLE_DEFAULT,(LPARAM)DEFAULT_CHARSET);
  iBaseFontSize = 10;
  Style_SetStyles(hwnd,lexDefault.Styles[0+iIdx].iStyle,lexDefault.Styles[0+iIdx].szValue);  // default
  Style_StrGetSize(lexDefault.Styles[0+iIdx].szValue,&iBaseFontSize);                        // base size

  // Auto-select codepage according to charset
  //Style_SetACPfromCharSet(hwnd);

  if (!Style_StrGetColor(TRUE,lexDefault.Styles[0+iIdx].szValue,&iValue))
    SendMessage(hwnd,SCI_STYLESETFORE,STYLE_DEFAULT,(LPARAM)GetSysColor(COLOR_WINDOWTEXT));   // default text color
  if (!Style_StrGetColor(FALSE,lexDefault.Styles[0+iIdx].szValue,&iValue))
    SendMessage(hwnd,SCI_STYLESETBACK,STYLE_DEFAULT,(LPARAM)GetSysColor(COLOR_WINDOW));       // default window color

  if (pLexNew->iLexer != SCLEX_NULL || pLexNew == &lexANSI)
    Style_SetStyles(hwnd,pLexNew->Styles[0].iStyle,pLexNew->Styles[0].szValue); // lexer default
  SendMessage(hwnd,SCI_STYLECLEARALL,0,0);

  Style_SetStyles(hwnd,lexDefault.Styles[1+iIdx].iStyle,lexDefault.Styles[1+iIdx].szValue); // linenumber
  Style_SetStyles(hwnd,lexDefault.Styles[2+iIdx].iStyle,lexDefault.Styles[2+iIdx].szValue); // brace light
  Style_SetStyles(hwnd,lexDefault.Styles[3+iIdx].iStyle,lexDefault.Styles[3+iIdx].szValue); // brace bad
  if (pLexNew != &lexANSI)
    Style_SetStyles(hwnd,lexDefault.Styles[4+iIdx].iStyle,lexDefault.Styles[4+iIdx].szValue); // control char
  Style_SetStyles(hwnd,lexDefault.Styles[5+iIdx].iStyle,lexDefault.Styles[5+iIdx].szValue); // indent guide

  // More default values...
  if (Style_StrGetColor(TRUE,lexDefault.Styles[6+iIdx].szValue,&rgb)) { // selection fore
    SendMessage(hwnd,SCI_SETSELFORE,TRUE,rgb);
    SendMessage(hwnd,SCI_SETADDITIONALSELFORE,rgb,0);
  }
  else {
    SendMessage(hwnd,SCI_SETSELFORE,0,0);
    SendMessage(hwnd,SCI_SETADDITIONALSELFORE,0,0);
  }

  if (Style_StrGetColor(FALSE,lexDefault.Styles[6+iIdx].szValue,&iValue)) { // selection back
    SendMessage(hwnd,SCI_SETSELBACK,TRUE,iValue);
    SendMessage(hwnd,SCI_SETADDITIONALSELBACK,iValue,0);
  }
  else {
    SendMessage(hwnd,SCI_SETSELBACK,TRUE,RGB(0xC0,0xC0,0xC0)); // use a default value...
    SendMessage(hwnd,SCI_SETADDITIONALSELBACK,RGB(0xC0,0xC0,0xC0),0);
  }

  if (Style_StrGetAlpha(lexDefault.Styles[6+iIdx].szValue,&iValue)) { // selection alpha
    SendMessage(hwnd,SCI_SETSELALPHA,iValue,0);
    SendMessage(hwnd,SCI_SETADDITIONALSELALPHA,iValue,0);
  }
  else {
    SendMessage(hwnd,SCI_SETSELALPHA,SC_ALPHA_NOALPHA,0);
    SendMessage(hwnd,SCI_SETADDITIONALSELALPHA,SC_ALPHA_NOALPHA,0);
  }

  if (StrStrI(lexDefault.Styles[6+iIdx].szValue,L"eolfilled")) // selection eolfilled
    SendMessage(hwnd,SCI_SETSELEOLFILLED,1,0);
  else
    SendMessage(hwnd,SCI_SETSELEOLFILLED,0,0);

  if (Style_StrGetColor(TRUE,lexDefault.Styles[7+iIdx].szValue,&rgb)) // whitespace fore
    SendMessage(hwnd,SCI_SETWHITESPACEFORE,TRUE,rgb);
  else
    SendMessage(hwnd,SCI_SETWHITESPACEFORE,0,0);

  if (Style_StrGetColor(FALSE,lexDefault.Styles[7+iIdx].szValue,&rgb)) // whitespace back
    SendMessage(hwnd,SCI_SETWHITESPACEBACK,TRUE,rgb);
  else
    SendMessage(hwnd,SCI_SETWHITESPACEBACK,0,0);    // use a default value...

  // whitespace dot size
  iValue = 1;
  if (Style_StrGetSize(lexDefault.Styles[7+iIdx].szValue,&iValue)) {

    WCHAR tch[32];
    WCHAR wchStyle[COUNTOF(lexDefault.Styles[0].szValue)];
    lstrcpyn(wchStyle,lexDefault.Styles[7+iIdx].szValue,COUNTOF(lexDefault.Styles[0].szValue));

    iValue = max(min(iValue,5),0);
    wsprintf(lexDefault.Styles[7+iIdx].szValue,L"size:%i",iValue);

    if (Style_StrGetColor(TRUE,wchStyle,&rgb)) {
      wsprintf(tch,L"; fore:#%02X%02X%02X",
        (int)GetRValue(rgb),
        (int)GetGValue(rgb),
        (int)GetBValue(rgb));
      lstrcat(lexDefault.Styles[7+iIdx].szValue,tch);
    }

    if (Style_StrGetColor(FALSE,wchStyle,&rgb)) {
      wsprintf(tch,L"; back:#%02X%02X%02X",
        (int)GetRValue(rgb),
        (int)GetGValue(rgb),
        (int)GetBValue(rgb));
      lstrcat(lexDefault.Styles[7+iIdx].szValue,tch);
    }
  }
  SendMessage(hwnd,SCI_SETWHITESPACESIZE,iValue,0);

  if (bHiliteCurrentLine) {

    if (Style_StrGetColor(FALSE,lexDefault.Styles[8+iIdx].szValue,&rgb)) // caret line back
    {
      SendMessage(hwnd,SCI_SETCARETLINEVISIBLE,TRUE,0);
      SendMessage(hwnd,SCI_SETCARETLINEBACK,rgb,0);

      if (Style_StrGetAlpha(lexDefault.Styles[8+iIdx].szValue,&iValue))
        SendMessage(hwnd,SCI_SETCARETLINEBACKALPHA,iValue,0);
      else
        SendMessage(hwnd,SCI_SETCARETLINEBACKALPHA,SC_ALPHA_NOALPHA,0);
    }
    else
      SendMessage(hwnd,SCI_SETCARETLINEVISIBLE,FALSE,0);
  }
  else
    SendMessage(hwnd,SCI_SETCARETLINEVISIBLE,FALSE,0);

  // caret style and width
  if (StrStr(lexDefault.Styles[9+iIdx].szValue,L"block")) {
    SendMessage(hwnd,SCI_SETCARETSTYLE,CARETSTYLE_BLOCK,0);
    lstrcpy(wchCaretStyle,L"block");
  }
  else {
    WCHAR wch[32];
    iValue = 1;
    if (Style_StrGetSize(lexDefault.Styles[9+iIdx].szValue,&iValue)) {
      iValue = max(min(iValue,3),1);
      wsprintf(wch,L"size:%i",iValue);
      lstrcat(wchCaretStyle,wch);
    }
    SendMessage(hwnd,SCI_SETCARETSTYLE,CARETSTYLE_LINE,0);
    SendMessage(hwnd,SCI_SETCARETWIDTH,iValue,0);
  }
  if (StrStr(lexDefault.Styles[9+iIdx].szValue,L"noblink")) {
    SendMessage(hwnd,SCI_SETCARETPERIOD,(WPARAM)0,0);
    if (lstrlen(wchCaretStyle))
      lstrcat(wchCaretStyle,L"; ");
    lstrcat(wchCaretStyle,L"noblink");
  }
  else
    SendMessage(hwnd,SCI_SETCARETPERIOD,(WPARAM)GetCaretBlinkTime(),0);

  // caret fore
  if (!Style_StrGetColor(TRUE,lexDefault.Styles[9+iIdx].szValue,&rgb))
    rgb = GetSysColor(COLOR_WINDOWTEXT);
  else {
    WCHAR wch[32];
    wsprintf(wch,L"fore:#%02X%02X%02X",
      (int)GetRValue(rgb),
      (int)GetGValue(rgb),
      (int)GetBValue(rgb));
    if (lstrlen(wchCaretStyle))
      lstrcat(wchCaretStyle,L"; ");
    lstrcat(wchCaretStyle,wch);
  }
  if (!VerifyContrast(rgb,(COLORREF)SendMessage(hwnd,SCI_STYLEGETBACK,0,0)))
    rgb = (int)SendMessage(hwnd,SCI_STYLEGETFORE,0,0);
  SendMessage(hwnd,SCI_SETCARETFORE,rgb,0);
  SendMessage(hwnd,SCI_SETADDITIONALCARETFORE,rgb,0);
  lstrcpy(lexDefault.Styles[9+iIdx].szValue,wchCaretStyle);

  if (SendMessage(hwnd,SCI_GETEDGEMODE,0,0) == EDGE_LINE) {
    if (Style_StrGetColor(TRUE,lexDefault.Styles[10+iIdx].szValue,&rgb)) // edge fore
      SendMessage(hwnd,SCI_SETEDGECOLOUR,rgb,0);
    else
      SendMessage(hwnd,SCI_SETEDGECOLOUR,GetSysColor(COLOR_3DLIGHT),0);
  }
  else {
    if (Style_StrGetColor(FALSE,lexDefault.Styles[10+iIdx].szValue,&rgb)) // edge back
      SendMessage(hwnd,SCI_SETEDGECOLOUR,rgb,0);
    else
      SendMessage(hwnd,SCI_SETEDGECOLOUR,GetSysColor(COLOR_3DLIGHT),0);
  }

  // Extra Line Spacing
  if (Style_StrGetSize(lexDefault.Styles[11+iIdx].szValue,&iValue) && pLexNew != &lexANSI) {
    int iAscent = 0;
    int iDescent = 0;
    iValue = min(max(iValue,0),64);
    wsprintf(lexDefault.Styles[11+iIdx].szValue,L"size:%i",iValue);
    if (iValue % 2) {
      iAscent++;
      iValue--;
    }
    iAscent += iValue / 2;
    iDescent += iValue / 2;
    SendMessage(hwnd,SCI_SETEXTRAASCENT,(WPARAM)iAscent,0);
    SendMessage(hwnd,SCI_SETEXTRADESCENT,(WPARAM)iDescent,0);
  }
  else {
    SendMessage(hwnd,SCI_SETEXTRAASCENT,0,0);
    SendMessage(hwnd,SCI_SETEXTRADESCENT,0,0);
    //wsprintf(lexDefault.Styles[11+iIdx].szValue,L"size:0");
  }

  { // set folding style; braces are for scoping only
    static const int iMarkerIDs[] =
    {
      SC_MARKNUM_FOLDEROPEN,
      SC_MARKNUM_FOLDER,
      SC_MARKNUM_FOLDERSUB,
      SC_MARKNUM_FOLDERTAIL,
      SC_MARKNUM_FOLDEREND,
      SC_MARKNUM_FOLDEROPENMID,
      SC_MARKNUM_FOLDERMIDTAIL
    };

    int i;

    COLORREF clrFore = SciCall_StyleGetFore(STYLE_DEFAULT);
    COLORREF clrBack = SciCall_StyleGetBack(STYLE_DEFAULT);

    SciCall_SetFoldMarginColour(TRUE, clrBack);
    SciCall_SetFoldMarginHiColour(TRUE, clrBack);

    // Set marker color to the average of clrFore and clrBack
    clrFore = (((clrFore & 0xFF0000) + (clrBack & 0xFF0000)) >> 1 & 0xFF0000) |
              (((clrFore & 0x00FF00) + (clrBack & 0x00FF00)) >> 1 & 0x00FF00) |
              (((clrFore & 0x0000FF) + (clrBack & 0x0000FF)) >> 1 & 0x0000FF);

    // Rounding hack for pure white against pure black
    if (clrFore == 0x7F7F7F) clrFore = 0x808080;

    for (i = 0; i < COUNTOF(iMarkerIDs); ++i)
    {
      SciCall_MarkerSetBack(iMarkerIDs[i], clrFore);
      SciCall_MarkerSetFore(iMarkerIDs[i], clrBack);
    }
  } // end set folding style

  if (SendMessage(hwnd,SCI_GETINDENTATIONGUIDES,0,0) != SC_IV_NONE)
    Style_SetIndentGuides(hwnd,TRUE);

  if (pLexNew->iLexer != SCLEX_NULL || pLexNew == &lexANSI)
  {
    int j;
    i = 1;
    while (pLexNew->Styles[i].iStyle != -1) {

      for (j = 0; j < 4 && (pLexNew->Styles[i].iStyle8[j] != 0 || j == 0); ++j)
        Style_SetStyles(hwnd,pLexNew->Styles[i].iStyle8[j],pLexNew->Styles[i].szValue);

      if (pLexNew->iLexer == SCLEX_HTML && pLexNew->Styles[i].iStyle8[0] == SCE_HPHP_DEFAULT) {
        int iRelated[] = { SCE_HPHP_COMMENT, SCE_HPHP_COMMENTLINE, SCE_HPHP_WORD, SCE_HPHP_HSTRING, SCE_HPHP_SIMPLESTRING, SCE_HPHP_NUMBER,
                           SCE_HPHP_OPERATOR, SCE_HPHP_VARIABLE, SCE_HPHP_HSTRING_VARIABLE, SCE_HPHP_COMPLEX_VARIABLE };
        for (j = 0; j < COUNTOF(iRelated); j++)
          Style_SetStyles(hwnd,iRelated[j],pLexNew->Styles[i].szValue);
      }

      if (pLexNew->iLexer == SCLEX_HTML && pLexNew->Styles[i].iStyle8[0] == SCE_HJ_DEFAULT) {
        int iRelated[] = { SCE_HJ_COMMENT, SCE_HJ_COMMENTLINE, SCE_HJ_COMMENTDOC, SCE_HJ_KEYWORD, SCE_HJ_WORD, SCE_HJ_DOUBLESTRING,
                           SCE_HJ_SINGLESTRING, SCE_HJ_STRINGEOL, SCE_HJ_REGEX, SCE_HJ_NUMBER, SCE_HJ_SYMBOLS };
        for (j = 0; j < COUNTOF(iRelated); j++)
          Style_SetStyles(hwnd,iRelated[j],pLexNew->Styles[i].szValue);
      }

      if (pLexNew->iLexer == SCLEX_HTML && pLexNew->Styles[i].iStyle8[0] == SCE_HJA_DEFAULT) {
        int iRelated[] = { SCE_HJA_COMMENT, SCE_HJA_COMMENTLINE, SCE_HJA_COMMENTDOC, SCE_HJA_KEYWORD, SCE_HJA_WORD, SCE_HJA_DOUBLESTRING,
                           SCE_HJA_SINGLESTRING, SCE_HJA_STRINGEOL, SCE_HJA_REGEX, SCE_HJA_NUMBER, SCE_HJA_SYMBOLS };
        for (j = 0; j < COUNTOF(iRelated); j++)
          Style_SetStyles(hwnd,iRelated[j],pLexNew->Styles[i].szValue);
      }

      if (pLexNew->iLexer == SCLEX_HTML && pLexNew->Styles[i].iStyle8[0] == SCE_HB_DEFAULT) {
        int iRelated[] = { SCE_HB_COMMENTLINE, SCE_HB_WORD, SCE_HB_IDENTIFIER, SCE_HB_STRING, SCE_HB_STRINGEOL, SCE_HB_NUMBER };
        for (j = 0; j < COUNTOF(iRelated); j++)
          Style_SetStyles(hwnd,iRelated[j],pLexNew->Styles[i].szValue);
      }

      if (pLexNew->iLexer == SCLEX_HTML && pLexNew->Styles[i].iStyle8[0] == SCE_HBA_DEFAULT) {
        int iRelated[] = { SCE_HBA_COMMENTLINE, SCE_HBA_WORD, SCE_HBA_IDENTIFIER, SCE_HBA_STRING, SCE_HBA_STRINGEOL, SCE_HBA_NUMBER };
        for (j = 0; j < COUNTOF(iRelated); j++)
          Style_SetStyles(hwnd,iRelated[j],pLexNew->Styles[i].szValue);
      }

      if ((pLexNew->iLexer == SCLEX_HTML || pLexNew->iLexer == SCLEX_XML) && pLexNew->Styles[i].iStyle8[0] == SCE_H_SGML_DEFAULT) {
        int iRelated[] = { SCE_H_SGML_COMMAND, SCE_H_SGML_1ST_PARAM, SCE_H_SGML_DOUBLESTRING, SCE_H_SGML_SIMPLESTRING, SCE_H_SGML_ERROR,
                           SCE_H_SGML_SPECIAL, SCE_H_SGML_ENTITY, SCE_H_SGML_COMMENT, SCE_H_SGML_1ST_PARAM_COMMENT, SCE_H_SGML_BLOCK_DEFAULT };
        for (j = 0; j < COUNTOF(iRelated); j++)
          Style_SetStyles(hwnd,iRelated[j],pLexNew->Styles[i].szValue);
      }

      if ((pLexNew->iLexer == SCLEX_HTML || pLexNew->iLexer == SCLEX_XML) && pLexNew->Styles[i].iStyle8[0] == SCE_H_CDATA) {
        int iRelated[] = { SCE_HP_START, SCE_HP_DEFAULT, SCE_HP_COMMENTLINE, SCE_HP_NUMBER, SCE_HP_STRING,
                           SCE_HP_CHARACTER, SCE_HP_WORD, SCE_HP_TRIPLE, SCE_HP_TRIPLEDOUBLE, SCE_HP_CLASSNAME,
                           SCE_HP_DEFNAME, SCE_HP_OPERATOR, SCE_HP_IDENTIFIER, SCE_HPA_START, SCE_HPA_DEFAULT,
                           SCE_HPA_COMMENTLINE, SCE_HPA_NUMBER, SCE_HPA_STRING, SCE_HPA_CHARACTER, SCE_HPA_WORD,
                           SCE_HPA_TRIPLE, SCE_HPA_TRIPLEDOUBLE, SCE_HPA_CLASSNAME, SCE_HPA_DEFNAME, SCE_HPA_OPERATOR,
                           SCE_HPA_IDENTIFIER };
        for (j = 0; j < COUNTOF(iRelated); j++)
          Style_SetStyles(hwnd,iRelated[j],pLexNew->Styles[i].szValue);
      }

      if (pLexNew->iLexer == SCLEX_XML && pLexNew->Styles[i].iStyle8[0] == SCE_H_CDATA) {
        int iRelated[] = { SCE_H_SCRIPT, SCE_H_ASP, SCE_H_ASPAT, SCE_H_QUESTION,
                           SCE_HPHP_DEFAULT, SCE_HPHP_COMMENT, SCE_HPHP_COMMENTLINE, SCE_HPHP_WORD, SCE_HPHP_HSTRING,
                           SCE_HPHP_SIMPLESTRING, SCE_HPHP_NUMBER, SCE_HPHP_OPERATOR, SCE_HPHP_VARIABLE,
                           SCE_HPHP_HSTRING_VARIABLE, SCE_HPHP_COMPLEX_VARIABLE, SCE_HJ_START, SCE_HJ_DEFAULT,
                           SCE_HJ_COMMENT, SCE_HJ_COMMENTLINE, SCE_HJ_COMMENTDOC, SCE_HJ_KEYWORD, SCE_HJ_WORD,
                           SCE_HJ_DOUBLESTRING, SCE_HJ_SINGLESTRING, SCE_HJ_STRINGEOL, SCE_HJ_REGEX, SCE_HJ_NUMBER,
                           SCE_HJ_SYMBOLS, SCE_HJA_START, SCE_HJA_DEFAULT, SCE_HJA_COMMENT, SCE_HJA_COMMENTLINE,
                           SCE_HJA_COMMENTDOC, SCE_HJA_KEYWORD, SCE_HJA_WORD, SCE_HJA_DOUBLESTRING, SCE_HJA_SINGLESTRING,
                           SCE_HJA_STRINGEOL, SCE_HJA_REGEX, SCE_HJA_NUMBER, SCE_HJA_SYMBOLS, SCE_HB_START, SCE_HB_DEFAULT,
                           SCE_HB_COMMENTLINE, SCE_HB_WORD, SCE_HB_IDENTIFIER, SCE_HB_STRING, SCE_HB_STRINGEOL,
                           SCE_HB_NUMBER, SCE_HBA_START, SCE_HBA_DEFAULT, SCE_HBA_COMMENTLINE, SCE_HBA_WORD,
                           SCE_HBA_IDENTIFIER, SCE_HBA_STRING, SCE_HBA_STRINGEOL, SCE_HBA_NUMBER, SCE_HP_START,
                           SCE_HP_DEFAULT, SCE_HP_COMMENTLINE, SCE_HP_NUMBER, SCE_HP_STRING, SCE_HP_CHARACTER, SCE_HP_WORD,
                           SCE_HP_TRIPLE, SCE_HP_TRIPLEDOUBLE, SCE_HP_CLASSNAME, SCE_HP_DEFNAME, SCE_HP_OPERATOR,
                           SCE_HP_IDENTIFIER, SCE_HPA_START, SCE_HPA_DEFAULT, SCE_HPA_COMMENTLINE, SCE_HPA_NUMBER,
                           SCE_HPA_STRING, SCE_HPA_CHARACTER, SCE_HPA_WORD, SCE_HPA_TRIPLE, SCE_HPA_TRIPLEDOUBLE,
                           SCE_HPA_CLASSNAME, SCE_HPA_DEFNAME, SCE_HPA_OPERATOR, SCE_HPA_IDENTIFIER };
        for (j = 0; j < COUNTOF(iRelated); j++)
          Style_SetStyles(hwnd,iRelated[j],pLexNew->Styles[i].szValue);
      }

      if (pLexNew->iLexer == SCLEX_CPP && pLexNew->Styles[i].iStyle8[0] == SCE_C_COMMENT) {
        int iRelated[] = { SCE_C_COMMENTLINE, SCE_C_COMMENTDOC, SCE_C_COMMENTLINEDOC, SCE_C_COMMENTDOCKEYWORD, SCE_C_COMMENTDOCKEYWORDERROR };
        for (j = 0; j < COUNTOF(iRelated); j++)
          Style_SetStyles(hwnd,iRelated[j],pLexNew->Styles[i].szValue);
      }

      if (pLexNew -> iLexer == SCLEX_SQL && pLexNew->Styles[i].iStyle8[0] == SCE_SQL_COMMENT) {
        int iRelated[] = { SCE_SQL_COMMENTLINE, SCE_SQL_COMMENTDOC, SCE_SQL_COMMENTLINEDOC, SCE_SQL_COMMENTDOCKEYWORD, SCE_SQL_COMMENTDOCKEYWORDERROR };
        for (j = 0; j < COUNTOF(iRelated); j++)
          Style_SetStyles(hwnd,iRelated[j],pLexNew->Styles[i].szValue);
      }
      i++;
    }
  }

  SendMessage(hwnd,SCI_COLOURISE,0,(LPARAM)-1);

  // Save current lexer
  pLexCurrent = pLexNew;
}


//=============================================================================
//
//  Style_SetLongLineColors()
//
void Style_SetLongLineColors(HWND hwnd)
{
  int rgb;

  // Use 2nd default style
  int iIdx = (bUse2ndDefaultStyle) ? 12 : 0;

  if (SendMessage(hwnd,SCI_GETEDGEMODE,0,0) == EDGE_LINE) {
    if (Style_StrGetColor(TRUE,lexDefault.Styles[10+iIdx].szValue,&rgb)) // edge fore
      SendMessage(hwnd,SCI_SETEDGECOLOUR,rgb,0);
    else
      SendMessage(hwnd,SCI_SETEDGECOLOUR,GetSysColor(COLOR_3DLIGHT),0);
  }
  else {
    if (Style_StrGetColor(FALSE,lexDefault.Styles[10+iIdx].szValue,&rgb)) // edge back
      SendMessage(hwnd,SCI_SETEDGECOLOUR,rgb,0);
    else
      SendMessage(hwnd,SCI_SETEDGECOLOUR,GetSysColor(COLOR_3DLIGHT),0);
  }
}


//=============================================================================
//
//  Style_SetCurrentLineBackground()
//
void Style_SetCurrentLineBackground(HWND hwnd)
{
  int rgb, iValue;

  // Use 2nd default style
  int iIdx = (bUse2ndDefaultStyle) ? 12 : 0;

  if (bHiliteCurrentLine) {

    if (Style_StrGetColor(FALSE,lexDefault.Styles[8+iIdx].szValue,&rgb)) // caret line back
    {
      SendMessage(hwnd,SCI_SETCARETLINEVISIBLE,TRUE,0);
      SendMessage(hwnd,SCI_SETCARETLINEBACK,rgb,0);

      if (Style_StrGetAlpha(lexDefault.Styles[8+iIdx].szValue,&iValue))
        SendMessage(hwnd,SCI_SETCARETLINEBACKALPHA,iValue,0);
      else
        SendMessage(hwnd,SCI_SETCARETLINEBACKALPHA,SC_ALPHA_NOALPHA,0);
    }
    else
      SendMessage(hwnd,SCI_SETCARETLINEVISIBLE,FALSE,0);
  }
  else
    SendMessage(hwnd,SCI_SETCARETLINEVISIBLE,FALSE,0);
}


//=============================================================================
//
//  Style_SniffShebang()
//
PEDITLEXER __fastcall Style_SniffShebang(char *pchText)
{
  if (StrCmpNA(pchText,"#!",2) == 0) {
    char *pch = pchText + 2;
    while (*pch == ' ' || *pch == '\t')
      pch++;
    while (*pch && *pch != ' ' && *pch != '\t' && *pch != '\r' && *pch != '\n')
      pch++;
    if ((pch - pchText) >= 3 && StrCmpNA(pch-3,"env",3) == 0) {
      while (*pch == ' ')
        pch++;
      while (*pch && *pch != ' ' && *pch != '\t' && *pch != '\r' && *pch != '\n')
        pch++;
    }
    if ((pch - pchText) >= 3 && StrCmpNIA(pch-3,"php",3) == 0)
      return(&lexHTML);
    else if ((pch - pchText) >= 4 && StrCmpNIA(pch-4,"perl",4) == 0)
      return(&lexPL);
    else if ((pch - pchText) >= 6 && StrCmpNIA(pch-6,"python",6) == 0)
      return(&lexPY);
    else if ((pch - pchText) >= 3 && StrCmpNA(pch-3,"tcl",3) == 0)
      return(&lexTCL);
    else if ((pch - pchText) >= 4 && StrCmpNA(pch-4,"wish",4) == 0)
      return(&lexTCL);
    else if ((pch - pchText) >= 5 && StrCmpNA(pch-5,"tclsh",5) == 0)
      return(&lexTCL);
    else if ((pch - pchText) >= 2 && StrCmpNA(pch-2,"sh",2) == 0)
      return(&lexBASH);
  }

  return(NULL);
}


//=============================================================================
//
//  Style_MatchLexer()
//
PEDITLEXER __fastcall Style_MatchLexer(LPCWSTR lpszMatch,BOOL bCheckNames)
{
  int i;
  WCHAR  tch[256+16];
  WCHAR  *p1,*p2;

  if (!bCheckNames) {

    for (i = 0; i < NUMLEXERS; i++) {

      ZeroMemory(tch,sizeof(WCHAR)*COUNTOF(tch));
      lstrcpy(tch,pLexArray[i]->szExtensions);
      p1 = tch;
      while (*p1) {
        if (p2 = StrChr(p1,L';'))
          *p2 = L'\0';
        else
          p2 = StrEnd(p1);
        StrTrim(p1,L" .");
        if (lstrcmpi(p1,lpszMatch) == 0)
          return(pLexArray[i]);
        p1 = p2+1;
      }
    }
  }

  else {

    int cch = lstrlen(lpszMatch);
    if (cch >= 3) {

      for (i = 0; i < NUMLEXERS; i++) {
        if (StrCmpNI(pLexArray[i]->pszName,lpszMatch,cch) == 0)
          return(pLexArray[i]);
      }
    }
  }
  return(NULL);
}


//=============================================================================
//
//  Style_SetLexerFromFile()
//
extern int fNoHTMLGuess;
extern int fNoCGIGuess;
extern FILEVARS fvCurFile;

void Style_SetLexerFromFile(HWND hwnd,LPCWSTR lpszFile)
{
  LPWSTR lpszExt = PathFindExtension(lpszFile);
  BOOL  bFound = FALSE;
  PEDITLEXER pLexNew = pLexArray[iDefaultLexer];
  PEDITLEXER pLexSniffed;

  if ((fvCurFile.mask & FV_MODE) && fvCurFile.tchMode[0]) {

    WCHAR wchMode[32];
    PEDITLEXER pLexMode;
    UINT cp = (UINT)SendMessage(hwnd,SCI_GETCODEPAGE,0,0);
    MultiByteToWideChar(cp,0,fvCurFile.tchMode,-1,wchMode,COUNTOF(wchMode));

    if (!fNoCGIGuess && (lstrcmpi(wchMode,L"cgi") == 0 || lstrcmpi(wchMode,L"fcgi") == 0)) {
      char tchText[256];
      SendMessage(hwnd,SCI_GETTEXT,(WPARAM)COUNTOF(tchText)-1,(LPARAM)tchText);
      StrTrimA(tchText," \t\n\r");
      if (pLexSniffed = Style_SniffShebang(tchText)) {
        if (iEncoding != g_DOSEncoding || pLexSniffed != &lexDefault || (
            lstrcmpi(lpszExt,L"nfo") && lstrcmpi(lpszExt,L"diz"))) {
          // Although .nfo and .diz were removed from the default lexer's
          // default extensions list, they may still presist in the user's INI
          pLexNew = pLexSniffed;
          bFound = TRUE;
        }
      }
    }

    if (!bFound) {
      if (pLexMode = Style_MatchLexer(wchMode,FALSE)) {
        pLexNew = pLexMode;
        bFound = TRUE;
      }
      else if (pLexMode = Style_MatchLexer(wchMode,TRUE)) {
        pLexNew = pLexMode;
        bFound = TRUE;
      }
    }
  }

  if (!bFound && bAutoSelect && /* bAutoSelect == FALSE skips lexer search */
      (lpszFile && lstrlen(lpszFile) > 0 && *lpszExt)) {

    if (*lpszExt == L'.')
      lpszExt++;

    if (!fNoCGIGuess && (lstrcmpi(lpszExt,L"cgi") == 0 || lstrcmpi(lpszExt,L"fcgi") == 0)) {
      char tchText[256];
      SendMessage(hwnd,SCI_GETTEXT,(WPARAM)COUNTOF(tchText)-1,(LPARAM)tchText);
      StrTrimA(tchText," \t\n\r");
      if (pLexSniffed = Style_SniffShebang(tchText)) {
        pLexNew = pLexSniffed;
        bFound = TRUE;
      }
    }

    if (!bFound && lstrcmpi(PathFindFileName(lpszFile),L"cmakelists.txt") == 0) {
      pLexNew = &lexCmake;
      bFound = TRUE;
    }

    // check associated extensions
    if (!bFound) {
      if (pLexSniffed = Style_MatchLexer(lpszExt,FALSE)) {
        pLexNew = pLexSniffed;
        bFound = TRUE;
      }
    }
  }

  if (!bFound && bAutoSelect &&
       lstrcmpi(PathFindFileName(lpszFile),L"makefile") == 0) {
    pLexNew = &lexMAK;
    bFound = TRUE;
  }

  if (!bFound && bAutoSelect && (!fNoHTMLGuess || !fNoCGIGuess)) {
    char tchText[512];
    SendMessage(hwnd,SCI_GETTEXT,(WPARAM)COUNTOF(tchText)-1,(LPARAM)tchText);
    StrTrimA(tchText," \t\n\r");
    if (!fNoHTMLGuess && tchText[0] == '<') {
      if (StrStrIA(tchText,"<html"))
        pLexNew = &lexHTML;
      else
        pLexNew = &lexXML;
      bFound = TRUE;
    }
    else if (!fNoCGIGuess && (pLexSniffed = Style_SniffShebang(tchText))) {
      pLexNew = pLexSniffed;
      bFound = TRUE;
    }
  }

  if (!bFound && iEncoding == g_DOSEncoding)
    pLexNew = &lexANSI;

  // Apply the new lexer
  Style_SetLexer(hwnd,pLexNew);
}


//=============================================================================
//
//  Style_SetLexerFromName()
//
void Style_SetLexerFromName(HWND hwnd,LPCWSTR lpszFile,LPCWSTR lpszName)
{
  PEDITLEXER pLexNew;
  if (pLexNew = Style_MatchLexer(lpszName,FALSE))
    Style_SetLexer(hwnd,pLexNew);
  else if (pLexNew = Style_MatchLexer(lpszName,TRUE))
    Style_SetLexer(hwnd,pLexNew);
  else
    Style_SetLexerFromFile(hwnd,lpszFile);
}


//=============================================================================
//
//  Style_SetDefaultLexer()
//
void Style_SetDefaultLexer(HWND hwnd)
{
  Style_SetLexer(hwnd,pLexArray[0]);
}


//=============================================================================
//
//  Style_SetHTMLLexer()
//
void Style_SetHTMLLexer(HWND hwnd)
{
  Style_SetLexer(hwnd,Style_MatchLexer(L"Web Source Code",TRUE));
}


//=============================================================================
//
//  Style_SetXMLLexer()
//
void Style_SetXMLLexer(HWND hwnd)
{
  Style_SetLexer(hwnd,Style_MatchLexer(L"XML Document",TRUE));
}


//=============================================================================
//
//  Style_SetLexerFromID()
//
void Style_SetLexerFromID(HWND hwnd,int id)
{
  if (id >= 0 && id < NUMLEXERS) {
    Style_SetLexer(hwnd,pLexArray[id]);
  }
}


//=============================================================================
//
//  Style_ToggleUse2ndDefault()
//
void Style_ToggleUse2ndDefault(HWND hwnd)
{
  bUse2ndDefaultStyle = (bUse2ndDefaultStyle) ? 0 : 1;
  Style_SetLexer(hwnd,pLexCurrent);
}


//=============================================================================
//
//  Style_SetDefaultFont()
//
void Style_SetDefaultFont(HWND hwnd)
{
  int iIdx = (bUse2ndDefaultStyle) ? 12 : 0;
  if (Style_SelectFont(hwnd,
        lexDefault.Styles[0+iIdx].szValue,
        COUNTOF(lexDefault.Styles[0].szValue),
        TRUE)) {
    fStylesModified = TRUE;
    Style_SetLexer(hwnd,pLexCurrent);
  }
}


//=============================================================================
//
//  Style_GetUse2ndDefault()
//
BOOL Style_GetUse2ndDefault(HWND hwnd)
{
  return (bUse2ndDefaultStyle);
}


//=============================================================================
//
//  Style_SetIndentGuides()
//
extern int flagSimpleIndentGuides;

void Style_SetIndentGuides(HWND hwnd,BOOL bShow)
{
  int iIndentView = SC_IV_NONE;
  if (bShow) {
    if (!flagSimpleIndentGuides) {
      if (SendMessage(hwnd,SCI_GETLEXER,0,0) == SCLEX_PYTHON)
        iIndentView = SC_IV_LOOKFORWARD;
      else
        iIndentView = SC_IV_LOOKBOTH;
    }
    else
      iIndentView = SC_IV_REAL;
  }
  SendMessage(hwnd,SCI_SETINDENTATIONGUIDES,iIndentView,0);
}


//=============================================================================
//
//  Style_GetFileOpenDlgFilter()
//
extern WCHAR tchFileDlgFilters[5*1024];

BOOL Style_GetOpenDlgFilterStr(LPWSTR lpszFilter,int cchFilter)
{
  if (lstrlen(tchFileDlgFilters) == 0)
    GetString(IDS_FILTER_ALL,lpszFilter,cchFilter);
  else {
    lstrcpyn(lpszFilter,tchFileDlgFilters,cchFilter-2);
    lstrcat(lpszFilter,L"||");
  }
  PrepareFilterStr(lpszFilter);
  return TRUE;
}


//=============================================================================
//
//  Style_StrGetFont()
//
BOOL Style_StrGetFont(LPCWSTR lpszStyle,LPWSTR lpszFont,int cchFont)
{
  WCHAR tch[256];
  WCHAR *p;

  if (p = StrStrI(lpszStyle,L"font:"))
  {
    lstrcpy(tch,p + CSTRLEN(L"font:"));
    if (p = StrChr(tch,L';'))
      *p = L'\0';
    TrimString(tch);
    lstrcpyn(lpszFont,tch,cchFont);
    return TRUE;
  }
  return FALSE;
}


//=============================================================================
//
//  Style_StrGetFontQuality()
//
BOOL Style_StrGetFontQuality(LPCWSTR lpszStyle,LPWSTR lpszQuality,int cchQuality)
{
  WCHAR tch[256];
  WCHAR *p;

  if (p = StrStrI(lpszStyle,L"smoothing:"))
  {
    lstrcpy(tch,p + CSTRLEN(L"smoothing:"));
    if (p = StrChr(tch,L';'))
      *p = L'\0';
    TrimString(tch);
    if (lstrcmpi(tch,L"none") == 0 ||
        lstrcmpi(tch,L"standard") == 0 ||
        lstrcmpi(tch,L"cleartype") == 0 ||
        lstrcmpi(tch,L"default") == 0) {
      lstrcpyn(lpszQuality,tch,cchQuality);
      return TRUE;
    }
  }
  return FALSE;
}


//=============================================================================
//
//  Style_StrGetCharSet()
//
BOOL Style_StrGetCharSet(LPCWSTR lpszStyle,int *i)
{
  WCHAR tch[256];
  WCHAR *p;
  int  iValue;
  int  itok;

  if (p = StrStrI(lpszStyle,L"charset:"))
  {
    lstrcpy(tch,p + CSTRLEN(L"charset:"));
    if (p = StrChr(tch,L';'))
      *p = L'\0';
    TrimString(tch);
    itok = swscanf(tch,L"%i",&iValue);
    if (itok == 1)
    {
      *i = iValue;
      return TRUE;
    }
  }
  return FALSE;
}


//=============================================================================
//
//  Style_StrGetSize()
//
BOOL Style_StrGetSize(LPCWSTR lpszStyle,int *i)
{
  WCHAR tch[256];
  WCHAR *p;
  int  iValue;
  int  iSign = 0;
  int  itok;

  if (p = StrStrI(lpszStyle,L"size:"))
  {
    lstrcpy(tch,p + CSTRLEN(L"size:"));
    if (tch[0] == L'+')
    {
      iSign = 1;
      tch[0] = L' ';
    }
    else if (tch[0] == L'-')
    {
      iSign = -1;
      tch[0] = L' ';
    }
    if (p = StrChr(tch,L';'))
      *p = L'\0';
    TrimString(tch);
    itok = swscanf(tch,L"%i",&iValue);
    if (itok == 1)
    {
      if (iSign == 0)
        *i = iValue;
      else
        *i = max(0,iBaseFontSize + iValue * iSign); // size must be +
      return TRUE;
    }
  }
  return FALSE;
}


//=============================================================================
//
//  Style_StrGetSizeStr()
//
BOOL Style_StrGetSizeStr(LPCWSTR lpszStyle,LPWSTR lpszSize,int cchSize)
{
  WCHAR tch[256];
  WCHAR *p;

  if (p = StrStrI(lpszStyle,L"size:"))
  {
    lstrcpy(tch,p + CSTRLEN(L"size:"));
    if (p = StrChr(tch,L';'))
      *p = L'\0';
    TrimString(tch);
    lstrcpyn(lpszSize,tch,cchSize);
    return TRUE;
  }
  return FALSE;
}


//=============================================================================
//
//  Style_StrGetColor()
//
BOOL Style_StrGetColor(BOOL bFore,LPCWSTR lpszStyle,int *rgb)
{
  WCHAR tch[256];
  WCHAR *p;
  int  iValue;
  int  itok;
  WCHAR *pItem = (bFore) ? L"fore:" : L"back:";

  if (p = StrStrI(lpszStyle,pItem))
  {
    lstrcpy(tch,p + lstrlen(pItem));
    if (tch[0] == L'#')
      tch[0] = L' ';
    if (p = StrChr(tch,L';'))
      *p = L'\0';
    TrimString(tch);
    itok = swscanf(tch,L"%x",&iValue);
    if (itok == 1)
    {
      *rgb = RGB((iValue&0xFF0000) >> 16,(iValue&0xFF00) >> 8,iValue&0xFF);
      return TRUE;
    }
  }
  return FALSE;
}


//=============================================================================
//
//  Style_StrGetCase()
//
BOOL Style_StrGetCase(LPCWSTR lpszStyle,int *i)
{
  WCHAR tch[256];
  WCHAR *p;

  if (p = StrStrI(lpszStyle,L"case:"))
  {
    lstrcpy(tch,p + CSTRLEN(L"case:"));
    if (p = StrChr(tch,L';'))
      *p = L'\0';
    TrimString(tch);
    if (tch[0] == L'u' || tch[0] == L'U') {
      *i = SC_CASE_UPPER;
      return TRUE;
    }
    else if (tch[0] == L'l' || tch[0] == L'L') {
      *i = SC_CASE_LOWER;
      return TRUE;
    }
  }
  return FALSE;
}


//=============================================================================
//
//  Style_StrGetAlpha()
//
BOOL Style_StrGetAlpha(LPCWSTR lpszStyle,int *i)
{
  WCHAR tch[256];
  WCHAR *p;
  int  iValue;
  int  itok;

  if (p = StrStrI(lpszStyle,L"alpha:"))
  {
    lstrcpy(tch,p + CSTRLEN(L"alpha:"));
    if (p = StrChr(tch,L';'))
      *p = L'\0';
    TrimString(tch);
    itok = swscanf(tch,L"%i",&iValue);
    if (itok == 1)
    {
      *i = min(max(SC_ALPHA_TRANSPARENT,iValue),SC_ALPHA_OPAQUE);
      return TRUE;
    }
  }
  return FALSE;
}


//=============================================================================
//
//  Style_SelectFont()
//
BOOL Style_SelectFont(HWND hwnd,LPWSTR lpszStyle,int cchStyle,BOOL bDefaultStyle)
{
  CHOOSEFONT cf;
  LOGFONT lf;
  WCHAR szNewStyle[512];
  int  iValue;
  WCHAR tch[32];
  HDC hdc;

  ZeroMemory(&cf,sizeof(CHOOSEFONT));
  ZeroMemory(&lf,sizeof(LOGFONT));

  // Map lpszStyle to LOGFONT
  if (Style_StrGetFont(lpszStyle,tch,COUNTOF(tch)))
    lstrcpyn(lf.lfFaceName,tch,COUNTOF(lf.lfFaceName));
  if (Style_StrGetCharSet(lpszStyle,&iValue))
    lf.lfCharSet = iValue;
  if (Style_StrGetSize(lpszStyle,&iValue)) {
    hdc = GetDC(hwnd);
    lf.lfHeight = -MulDiv(iValue,GetDeviceCaps(hdc,LOGPIXELSY),72);
    ReleaseDC(hwnd,hdc);
  }
  lf.lfWeight = (StrStrI(lpszStyle,L"bold")) ? FW_BOLD : FW_NORMAL;
  lf.lfItalic = (StrStrI(lpszStyle,L"italic")) ? 1 : 0;

  // Init cf
  cf.lStructSize = sizeof(CHOOSEFONT);
  cf.hwndOwner = hwnd;
  cf.lpLogFont = &lf;
  cf.Flags = CF_INITTOLOGFONTSTRUCT /*| CF_NOSCRIPTSEL*/ | CF_SCREENFONTS;

  if (HIBYTE(GetKeyState(VK_SHIFT)))
    cf.Flags |= CF_FIXEDPITCHONLY;

  if (!ChooseFont(&cf) || !lstrlen(lf.lfFaceName))
    return FALSE;

  // Map back to lpszStyle
  lstrcpy(szNewStyle,L"font:");
  lstrcat(szNewStyle,lf.lfFaceName);
  if (Style_StrGetFontQuality(lpszStyle,tch,COUNTOF(tch)))
  {
    lstrcat(szNewStyle,L"; smoothing:");
    lstrcat(szNewStyle,tch);
  }
  if (bDefaultStyle &&
      lf.lfCharSet != DEFAULT_CHARSET &&
      lf.lfCharSet != ANSI_CHARSET &&
      lf.lfCharSet != iDefaultCharSet) {
    lstrcat(szNewStyle,L"; charset:");
    wsprintf(tch,L"%i",lf.lfCharSet);
    lstrcat(szNewStyle,tch);
  }
  lstrcat(szNewStyle,L"; size:");
  wsprintf(tch,L"%i",cf.iPointSize/10);
  lstrcat(szNewStyle,tch);
  if (cf.nFontType & BOLD_FONTTYPE)
    lstrcat(szNewStyle,L"; bold");
  if (cf.nFontType & ITALIC_FONTTYPE)
    lstrcat(szNewStyle,L"; italic");

  if (StrStrI(lpszStyle,L"underline"))
    lstrcat(szNewStyle,L"; underline");

  // save colors
  if (Style_StrGetColor(TRUE,lpszStyle,&iValue))
  {
    wsprintf(tch,L"; fore:#%02X%02X%02X",
      (int)GetRValue(iValue),
      (int)GetGValue(iValue),
      (int)GetBValue(iValue));
    lstrcat(szNewStyle,tch);
  }
  if (Style_StrGetColor(FALSE,lpszStyle,&iValue))
  {
    wsprintf(tch,L"; back:#%02X%02X%02X",
      (int)GetRValue(iValue),
      (int)GetGValue(iValue),
      (int)GetBValue(iValue));
    lstrcat(szNewStyle,tch);
  }

  if (StrStrI(lpszStyle,L"eolfilled"))
    lstrcat(szNewStyle,L"; eolfilled");

  if (Style_StrGetCase(lpszStyle,&iValue)) {
    lstrcat(szNewStyle,L"; case:");
    lstrcat(szNewStyle,(iValue == SC_CASE_UPPER) ? L"u" : L"");
  }

  if (Style_StrGetAlpha(lpszStyle,&iValue)) {
    lstrcat(szNewStyle,L"; alpha:");
    wsprintf(tch,L"%i",iValue);
    lstrcat(szNewStyle,tch);
  }

  lstrcpyn(lpszStyle,szNewStyle,cchStyle);
  return TRUE;
}


//=============================================================================
//
//  Style_SelectColor()
//
BOOL Style_SelectColor(HWND hwnd,BOOL bFore,LPWSTR lpszStyle,int cchStyle)
{
  CHOOSECOLOR cc;
  WCHAR szNewStyle[512];
  int  iRGBResult;
  int  iValue;
  WCHAR tch[32];

  ZeroMemory(&cc,sizeof(CHOOSECOLOR));

  iRGBResult = (bFore) ? GetSysColor(COLOR_WINDOWTEXT) : GetSysColor(COLOR_WINDOW);
  Style_StrGetColor(bFore,lpszStyle,&iRGBResult);

  cc.lStructSize = sizeof(CHOOSECOLOR);
  cc.hwndOwner = hwnd;
  cc.rgbResult = iRGBResult;
  cc.lpCustColors = crCustom;
  cc.Flags = CC_FULLOPEN | CC_RGBINIT | CC_SOLIDCOLOR;

  if (!ChooseColor(&cc))
    return FALSE;

  iRGBResult = cc.rgbResult;

  // Rebuild style string
  lstrcpy(szNewStyle,L"");
  if (Style_StrGetFont(lpszStyle,tch,COUNTOF(tch)))
  {
    lstrcat(szNewStyle,L"font:");
    lstrcat(szNewStyle,tch);
  }
  if (Style_StrGetFontQuality(lpszStyle,tch,COUNTOF(tch)))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    lstrcat(szNewStyle,L"smoothing:");
    lstrcat(szNewStyle,tch);
  }
  if (Style_StrGetCharSet(lpszStyle,&iValue))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    wsprintf(tch,L"charset:%i",iValue);
    lstrcat(szNewStyle,tch);
  }
  if (Style_StrGetSizeStr(lpszStyle,tch,COUNTOF(tch)))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    lstrcat(szNewStyle,L"size:");
    lstrcat(szNewStyle,tch);
  }

  if (StrStrI(lpszStyle,L"bold"))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    lstrcat(szNewStyle,L"bold");
  }
  if (StrStrI(lpszStyle,L"italic"))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    lstrcat(szNewStyle,L"italic");
  }
  if (StrStrI(lpszStyle,L"underline"))
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    lstrcat(szNewStyle,L"underline");
  }

  if (bFore)
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    wsprintf(tch,L"fore:#%02X%02X%02X",
      (int)GetRValue(iRGBResult),
      (int)GetGValue(iRGBResult),
      (int)GetBValue(iRGBResult));
    lstrcat(szNewStyle,tch);
    if (Style_StrGetColor(FALSE,lpszStyle,&iValue))
    {
      wsprintf(tch,L"; back:#%02X%02X%02X",
        (int)GetRValue(iValue),
        (int)GetGValue(iValue),
        (int)GetBValue(iValue));
      lstrcat(szNewStyle,tch);
    }
  }
  else
  {
    if (lstrlen(szNewStyle))
      lstrcat(szNewStyle,L"; ");
    if (Style_StrGetColor(TRUE,lpszStyle,&iValue))
    {
      wsprintf(tch,L"fore:#%02X%02X%02X; ",
        (int)GetRValue(iValue),
        (int)GetGValue(iValue),
        (int)GetBValue(iValue));
      lstrcat(szNewStyle,tch);
    }
    wsprintf(tch,L"back:#%02X%02X%02X",
      (int)GetRValue(iRGBResult),
      (int)GetGValue(iRGBResult),
      (int)GetBValue(iRGBResult));
    lstrcat(szNewStyle,tch);
  }

  if (StrStrI(lpszStyle,L"eolfilled"))
    lstrcat(szNewStyle,L"; eolfilled");

  if (Style_StrGetCase(lpszStyle,&iValue)) {
    lstrcat(szNewStyle,L"; case:");
    lstrcat(szNewStyle,(iValue == SC_CASE_UPPER) ? L"u" : L"");
  }

  if (Style_StrGetAlpha(lpszStyle,&iValue)) {
    lstrcat(szNewStyle,L"; alpha:");
    wsprintf(tch,L"%i",iValue);
    lstrcat(szNewStyle,tch);
  }

  if (StrStrI(lpszStyle,L"block"))
    lstrcat(szNewStyle,L"; block");

  if (StrStrI(lpszStyle,L"noblink"))
    lstrcat(szNewStyle,L"; noblink");

  lstrcpyn(lpszStyle,szNewStyle,cchStyle);
  return TRUE;
}


//=============================================================================
//
//  Style_SetStyles()
//
void Style_SetStyles(HWND hwnd,int iStyle,LPCWSTR lpszStyle)
{

  WCHAR tch[256];
  WCHAR *p;
  int  iValue;

  // Font
  if (Style_StrGetFont(lpszStyle,tch,COUNTOF(tch))) {
    char mch[256] = "Lucida Console";
    if (fIsConsolasAvailable || lstrcmpi(tch,L"Consolas"))
      WideCharToMultiByte(CP_ACP,0,tch,-1,mch,COUNTOF(mch),NULL,NULL);
    SendMessage(hwnd,SCI_STYLESETFONT,iStyle,(LPARAM)mch);
  }

  // Size
  if (Style_StrGetSize(lpszStyle,&iValue))
    SendMessage(hwnd,SCI_STYLESETSIZE,iStyle,(LPARAM)iValue);

  // Fore
  if (Style_StrGetColor(TRUE,lpszStyle,&iValue))
    SendMessage(hwnd,SCI_STYLESETFORE,iStyle,(LPARAM)iValue);

  // Back
  if (Style_StrGetColor(FALSE,lpszStyle,&iValue))
    SendMessage(hwnd,SCI_STYLESETBACK,iStyle,(LPARAM)iValue);

  // Bold
  if (p = StrStrI(lpszStyle,L"bold"))
    SendMessage(hwnd,SCI_STYLESETBOLD,iStyle,(LPARAM)TRUE);
  else
    SendMessage(hwnd,SCI_STYLESETBOLD,iStyle,(LPARAM)FALSE);

  // Italic
  if (p = StrStrI(lpszStyle,L"italic"))
    SendMessage(hwnd,SCI_STYLESETITALIC,iStyle,(LPARAM)TRUE);
  else
    SendMessage(hwnd,SCI_STYLESETITALIC,iStyle,(LPARAM)FALSE);

  // Underline
  if (p = StrStrI(lpszStyle,L"underline"))
    SendMessage(hwnd,SCI_STYLESETUNDERLINE,iStyle,(LPARAM)TRUE);
  else
    SendMessage(hwnd,SCI_STYLESETUNDERLINE,iStyle,(LPARAM)FALSE);

  // EOL Filled
  if (p = StrStrI(lpszStyle,L"eolfilled"))
    SendMessage(hwnd,SCI_STYLESETEOLFILLED,iStyle,(LPARAM)TRUE);
  else
    SendMessage(hwnd,SCI_STYLESETEOLFILLED,iStyle,(LPARAM)FALSE);

  // Case
  if (Style_StrGetCase(lpszStyle,&iValue))
    SendMessage(hwnd,SCI_STYLESETCASE,iStyle,(LPARAM)iValue);

  // Character Set
  if (Style_StrGetCharSet(lpszStyle,&iValue))
    SendMessage(hwnd,SCI_STYLESETCHARACTERSET,iStyle,(LPARAM)iValue);

}


//=============================================================================
//
//  Style_SetFontQuality()
//
void Style_SetFontQuality(HWND hwnd,LPCWSTR lpszStyle) {

  WPARAM wQuality = SC_EFF_QUALITY_DEFAULT;
  WCHAR tch[32];

  if (Style_StrGetFontQuality(lpszStyle,tch,COUNTOF(tch))) {
    if (lstrcmpi(tch,L"none") == 0)
      wQuality = SC_EFF_QUALITY_NON_ANTIALIASED;
    else if (lstrcmpi(tch,L"standard") == 0)
      wQuality = SC_EFF_QUALITY_ANTIALIASED;
    else if (lstrcmpi(tch,L"cleartype") == 0)
      wQuality = SC_EFF_QUALITY_LCD_OPTIMIZED;
    else
      wQuality = SC_EFF_QUALITY_DEFAULT;
  }
  else {
    WCHAR tch[32];
    if (Style_StrGetFont(lpszStyle,tch,COUNTOF(tch))) {
      if (lstrcmpi(tch,L"Calibri") == 0 ||
          lstrcmpi(tch,L"Cambria") == 0 ||
          lstrcmpi(tch,L"Candara") == 0 ||
          lstrcmpi(tch,L"Consolas") == 0 ||
          lstrcmpi(tch,L"Constantia") == 0 ||
          lstrcmpi(tch,L"Corbel") == 0 ||
          lstrcmpi(tch,L"Segoe UI") == 0)
        wQuality = SC_EFF_QUALITY_LCD_OPTIMIZED;
    }
    else
      wQuality = SC_EFF_QUALITY_DEFAULT;
  }
  SendMessage(hwnd,SCI_SETFONTQUALITY,wQuality,0);
}


//=============================================================================
//
//  Style_GetCurrentLexerName()
//
void Style_GetCurrentLexerName(LPWSTR lpszName,int cchName)
{
  if (!GetString(pLexCurrent->rid,lpszName,cchName))
    lstrcpyn(lpszName,pLexCurrent->pszName,cchName);
}


//=============================================================================
//
//  Style_GetLexerIconId()
//
int Style_GetLexerIconId(PEDITLEXER plex)
{
  WCHAR *p;
  WCHAR *pszExtensions;
  WCHAR *pszFile;

  SHFILEINFO shfi;

  if (lstrlen(plex->szExtensions))
    pszExtensions = plex->szExtensions;
  else
    pszExtensions = plex->pszDefExt;

  pszFile = GlobalAlloc(GPTR,sizeof(WCHAR)*(lstrlen(pszExtensions) + CSTRLEN(L"*.txt") + 16));
  lstrcpy(pszFile,L"*.");
  lstrcat(pszFile,pszExtensions);
  if (p = StrChr(pszFile,L';'))
    *p = L'\0';

  // check for ; at beginning
  if (lstrlen(pszFile) < 3)
    lstrcat(pszFile,L"txt");

  SHGetFileInfo(pszFile,FILE_ATTRIBUTE_NORMAL,&shfi,sizeof(SHFILEINFO),
    SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES);

  GlobalFree(pszFile);

  return (shfi.iIcon);
}


//=============================================================================
//
//  Style_AddLexerToTreeView()
//
HTREEITEM Style_AddLexerToTreeView(HWND hwnd,PEDITLEXER plex)
{
  int i = 0;
  WCHAR tch[128];

  HTREEITEM hTreeNode;

  TVINSERTSTRUCT tvis;
  ZeroMemory(&tvis,sizeof(TVINSERTSTRUCT));

  tvis.hInsertAfter = TVI_LAST;

  tvis.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
  if (GetString(plex->rid,tch,COUNTOF(tch)))
    tvis.item.pszText = tch;
  else
    tvis.item.pszText = plex->pszName;
  tvis.item.iImage = Style_GetLexerIconId(plex);
  tvis.item.iSelectedImage = tvis.item.iImage;
  tvis.item.lParam = (LPARAM)plex;

  hTreeNode = (HTREEITEM)TreeView_InsertItem(hwnd,&tvis);

  tvis.hParent = hTreeNode;

  tvis.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
  //tvis.item.iImage = -1;
  //tvis.item.iSelectedImage = -1;

  while (plex->Styles[i].iStyle != -1) {

    if (GetString(plex->Styles[i].rid,tch,COUNTOF(tch)))
      tvis.item.pszText = tch;
    else
      tvis.item.pszText = plex->Styles[i].pszName;
    tvis.item.lParam = (LPARAM)(&plex->Styles[i]);
    TreeView_InsertItem(hwnd,&tvis);
    i++;
  }

  return hTreeNode;
}


//=============================================================================
//
//  Style_AddLexerToListView()
//
void Style_AddLexerToListView(HWND hwnd,PEDITLEXER plex)
{
  WCHAR tch[128];
  LVITEM lvi;
  ZeroMemory(&lvi,sizeof(LVITEM));

  lvi.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT;
  lvi.iItem = ListView_GetItemCount(hwnd);
  if (GetString(plex->rid,tch,COUNTOF(tch)))
    lvi.pszText = tch;
  else
    lvi.pszText = plex->pszName;
  lvi.iImage = Style_GetLexerIconId(plex);
  lvi.lParam = (LPARAM)plex;

  ListView_InsertItem(hwnd,&lvi);
}


//=============================================================================
//
//  Style_ConfigDlgProc()
//
INT_PTR CALLBACK Style_ConfigDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{

  static HWND hwndTV;
  static BOOL fDragging;
  static PEDITLEXER pCurrentLexer;
  static PEDITSTYLE pCurrentStyle;
  static HFONT hFontTitle;
  static HBRUSH hbrFore;
  static HBRUSH hbrBack;

  switch(umsg)
  {

    case WM_INITDIALOG:
      {
        int i;
        SHFILEINFO shfi;
        LOGFONT lf;
        HTREEITEM currentLex;
        int found = 0;

        hwndTV = GetDlgItem(hwnd,IDC_STYLELIST);
        fDragging = FALSE;

        TreeView_SetImageList(hwndTV,
          (HIMAGELIST)SHGetFileInfo(L"C:\\",0,&shfi,sizeof(SHFILEINFO),
          SHGFI_SMALLICON | SHGFI_SYSICONINDEX),TVSIL_NORMAL);

        // Add lexers
        for (i = 0; i < NUMLEXERS; i++)
        {
          if (!found && lstrcmp(pLexArray[i]->pszName,pLexCurrent->pszName) == 0)
          {
              found = 1;
              currentLex = Style_AddLexerToTreeView(hwndTV,pLexArray[i]);
          }
          else
              Style_AddLexerToTreeView(hwndTV,pLexArray[i]);
        }

        pCurrentStyle = 0;

        //SetExplorerTheme(hwndTV);
        //TreeView_Expand(hwndTV,TreeView_GetRoot(hwndTV),TVE_EXPAND);
        TreeView_Select(hwndTV,currentLex,TVGN_CARET);

        SendDlgItemMessage(hwnd,IDC_STYLEEDIT,EM_LIMITTEXT,COUNTOF(lexDefault.Styles[0].szValue)-1,0);

        MakeBitmapButton(hwnd,IDC_PREVSTYLE,g_hInstance,IDB_PREV);
        MakeBitmapButton(hwnd,IDC_NEXTSTYLE,g_hInstance,IDB_NEXT);

        // Setup title font
        if (hFontTitle)
          DeleteObject(hFontTitle);
        if (NULL == (hFontTitle = (HFONT)SendDlgItemMessage(hwnd,IDC_TITLE,WM_GETFONT,0,0)))
          hFontTitle = GetStockObject(DEFAULT_GUI_FONT);
        GetObject(hFontTitle,sizeof(LOGFONT),&lf);
        lf.lfHeight += lf.lfHeight / 5;
        lf.lfWeight = FW_BOLD;
        hFontTitle = CreateFontIndirect(&lf);
        SendDlgItemMessage(hwnd,IDC_TITLE,WM_SETFONT,(WPARAM)hFontTitle,TRUE);

        CenterDlgInParent(hwnd);
      }
      return TRUE;


    case WM_DESTROY:
      DeleteBitmapButton(hwnd,IDC_STYLEFORE);
      DeleteBitmapButton(hwnd,IDC_STYLEBACK);
      DeleteBitmapButton(hwnd,IDC_PREVSTYLE);
      DeleteBitmapButton(hwnd,IDC_NEXTSTYLE);
      return FALSE;


    case WM_NOTIFY:

      if (((LPNMHDR)(lParam))->idFrom == IDC_STYLELIST)
      {
        LPNMTREEVIEW lpnmtv = (LPNMTREEVIEW)lParam;

        switch (lpnmtv->hdr.code)
        {

          case TVN_SELCHANGED:
            {
              if (pCurrentStyle)
                GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue,COUNTOF(pCurrentStyle->szValue));

              else if (pCurrentLexer) {
                if (!GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentLexer->szExtensions,COUNTOF(pCurrentLexer->szExtensions)))
                  lstrcpy(pCurrentLexer->szExtensions,pCurrentLexer->pszDefExt);
              }

              // a lexer has been selected
              if (!TreeView_GetParent(hwndTV,lpnmtv->itemNew.hItem))
              {
                WCHAR wch[128];

                GetDlgItemText(hwnd,IDC_STYLELABELS,wch,COUNTOF(wch));
                if (StrChr(wch,L'|')) *StrChr(wch,L'|') = 0;

                pCurrentStyle = 0;
                if (pCurrentLexer = (PEDITLEXER)lpnmtv->itemNew.lParam)
                {
                  SetDlgItemText(hwnd,IDC_STYLELABEL,wch);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEEDIT),TRUE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEFONT),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEFORE),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEBACK),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEDEFAULT),TRUE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEBOLD),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEITALIC),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEUNDERLINE),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEEOLFILLED),FALSE);
                  //CheckDlgButton(hwnd,IDC_STYLEBOLD,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEITALIC,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,BST_UNCHECKED);
                  SetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentLexer->szExtensions);
                }
                else
                {
                  SetDlgItemText(hwnd,IDC_STYLELABEL,L"");
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEEDIT),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEFONT),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEFORE),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEBACK),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEDEFAULT),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEBOLD),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEITALIC),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEUNDERLINE),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEEOLFILLED),FALSE);
                  //CheckDlgButton(hwnd,IDC_STYLEBOLD,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEITALIC,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,BST_UNCHECKED);
                  SetDlgItemText(hwnd,IDC_STYLEEDIT,L"");
                }
              }

              // a style has been selected
              else
              {
                WCHAR wch[128];

                GetDlgItemText(hwnd,IDC_STYLELABELS,wch,COUNTOF(wch));
                if (StrChr(wch,L'|')) *StrChr(wch,L'|') = 0;


                pCurrentLexer = 0;
                if (pCurrentStyle = (PEDITSTYLE)lpnmtv->itemNew.lParam)
                {
                  SetDlgItemText(hwnd,IDC_STYLELABEL,StrEnd(wch)+1);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEEDIT),TRUE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEFONT),TRUE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEFORE),TRUE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEBACK),TRUE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEDEFAULT),TRUE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEBOLD),TRUE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEITALIC),TRUE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEUNDERLINE),TRUE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEEOLFILLED),TRUE);
                  //CheckDlgButton(hwnd,IDC_STYLEBOLD,(Style_StrGetAttribute(pCurrentStyle->szValue,L"bold") ? BST_CHECKED : BST_UNCHECKED));
                  //CheckDlgButton(hwnd,IDC_STYLEITALIC,(Style_StrGetAttribute(pCurrentStyle->szValue,L"italic") ? BST_CHECKED : BST_UNCHECKED));
                  //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,(Style_StrGetAttribute(pCurrentStyle->szValue,L"underline") ? BST_CHECKED : BST_UNCHECKED));
                  //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,(Style_StrGetAttribute(pCurrentStyle->szValue,L"eolfilled") ? BST_CHECKED : BST_UNCHECKED));
                  SetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue);
                }
                else
                {
                  SetDlgItemText(hwnd,IDC_STYLELABEL,L"");
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEEDIT),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEFONT),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEFORE),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEBACK),FALSE);
                  EnableWindow(GetDlgItem(hwnd,IDC_STYLEDEFAULT),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEBOLD),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEITALIC),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEUNDERLINE),FALSE);
                  //EnableWindow(GetDlgItem(hwnd,IDC_STYLEEOLFILLED),FALSE);
                  //CheckDlgButton(hwnd,IDC_STYLEBOLD,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEITALIC,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,BST_UNCHECKED);
                  //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,BST_UNCHECKED);
                  SetDlgItemText(hwnd,IDC_STYLEEDIT,L"");
                }
              }
            }
            break;

          case TVN_BEGINDRAG:
            {
              //HIMAGELIST himl;

              //if (pCurrentStyle)
              //  GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue,COUNTOF(pCurrentStyle->szValue));
              TreeView_Select(hwndTV,lpnmtv->itemNew.hItem,TVGN_CARET);

              //himl = TreeView_CreateDragImage(hwndTV,lpnmtv->itemNew.hItem);
              //ImageList_BeginDrag(himl,0,0,0);
              //ImageList_DragEnter(hwndTV,lpnmtv->ptDrag.x,lpnmtv->ptDrag.y);
              if (pCurrentStyle)
                DestroyCursor(SetCursor(LoadCursor(g_hInstance,MAKEINTRESOURCE(IDC_COPY))));
              else
                DestroyCursor(SetCursor(LoadCursor(NULL,IDC_NO)));

              SetCapture(hwnd);
              fDragging = TRUE;
            }

        }
      }

      break;


    case WM_MOUSEMOVE:
      {
        HTREEITEM htiTarget;
        TVHITTESTINFO tvht;

        if (fDragging && pCurrentStyle)
        {
          LONG xCur = LOWORD(lParam);
          LONG yCur = HIWORD(lParam);

          //ImageList_DragMove(xCur,yCur);
          //ImageList_DragShowNolock(FALSE);

          tvht.pt.x = xCur;
          tvht.pt.y = yCur;

          //ClientToScreen(hwnd,&tvht.pt);
          //ScreenToClient(hwndTV,&tvht.pt);
          MapWindowPoints(hwnd,hwndTV,&tvht.pt,1);

          if ((htiTarget = TreeView_HitTest(hwndTV,&tvht)) != NULL &&
               TreeView_GetParent(hwndTV,htiTarget) != NULL)
          {
            TreeView_SelectDropTarget(hwndTV,htiTarget);
            //TreeView_Expand(hwndTV,htiTarget,TVE_EXPAND);
            TreeView_EnsureVisible(hwndTV,htiTarget);
          }
          else
            TreeView_SelectDropTarget(hwndTV,NULL);

          //ImageList_DragShowNolock(TRUE);
        }
      }
      break;


    case WM_LBUTTONUP:
      {
        if (fDragging)
        {
          HTREEITEM htiTarget;

          //ImageList_EndDrag();

          if (htiTarget = TreeView_GetDropHilight(hwndTV))
          {
            WCHAR tchCopy[256];
            TreeView_SelectDropTarget(hwndTV,NULL);
            GetDlgItemText(hwnd,IDC_STYLEEDIT,tchCopy,COUNTOF(tchCopy));
            TreeView_Select(hwndTV,htiTarget,TVGN_CARET);

            // after select, this is new current item
            if (pCurrentStyle)
            {
              lstrcpy(pCurrentStyle->szValue,tchCopy);
              SetDlgItemText(hwnd,IDC_STYLEEDIT,tchCopy);
              //CheckDlgButton(hwnd,IDC_STYLEBOLD,(Style_StrGetAttribute(tchCopy,L"bold") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEITALIC,(Style_StrGetAttribute(tchCopy,L"italic") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,(Style_StrGetAttribute(tchCopy,L"underline") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,(Style_StrGetAttribute(tchCopy,L"eolfilled") ? BST_CHECKED : BST_UNCHECKED));
            }
          }
          ReleaseCapture();
          DestroyCursor(SetCursor(LoadCursor(NULL,IDC_ARROW)));
          fDragging = FALSE;
        }
      }
      break;


    case WM_CANCELMODE:
      {
        if (fDragging)
        {
          //ImageList_EndDrag();
          TreeView_SelectDropTarget(hwndTV,NULL);
          ReleaseCapture();
          DestroyCursor(SetCursor(LoadCursor(NULL,IDC_ARROW)));
          fDragging = FALSE;
        }
      }
      break;


    case WM_COMMAND:

      switch(LOWORD(wParam))
      {

        case IDC_PREVSTYLE:
          if (TreeView_GetSelection(hwndTV))
            TreeView_Select(hwndTV,TreeView_GetPrevVisible(hwndTV,
              TreeView_GetSelection(hwndTV)),TVGN_CARET);
          PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,IDC_STYLEEDIT)),1);
          break;

        case IDC_NEXTSTYLE:
          if (TreeView_GetSelection(hwndTV))
            TreeView_Select(hwndTV,TreeView_GetNextVisible(hwndTV,
              TreeView_GetSelection(hwndTV)),TVGN_CARET);
          PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,IDC_STYLEEDIT)),1);
          break;

        case IDC_STYLEFONT:
          if (pCurrentStyle)
          {
            WCHAR tch[256];
            GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
            if (Style_SelectFont(hwnd,tch,COUNTOF(tch),
                  lstrcmpi(pCurrentStyle->pszName,L"Default Style") == 0 ||
                  lstrcmpi(pCurrentStyle->pszName,L"2nd Default Style") == 0)) {
              SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
              //CheckDlgButton(hwnd,IDC_STYLEBOLD,(Style_StrGetAttribute(tch,L"bold") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEITALIC,(Style_StrGetAttribute(tch,L"italic") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,(Style_StrGetAttribute(tch,L"underline") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,(Style_StrGetAttribute(tch,L"eolfilled") ? BST_CHECKED : BST_UNCHECKED));
            }
          }
          PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,IDC_STYLEEDIT)),1);
          break;

        case IDC_STYLEFORE:
          if (pCurrentStyle)
          {
            WCHAR tch[256];
            GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
            if (Style_SelectColor(hwnd,TRUE,tch,COUNTOF(tch))) {
              SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
              //CheckDlgButton(hwnd,IDC_STYLEBOLD,(Style_StrGetAttribute(tch,L"bold") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEITALIC,(Style_StrGetAttribute(tch,L"italic") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,(Style_StrGetAttribute(tch,L"underline") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,(Style_StrGetAttribute(tch,L"eolfilled") ? BST_CHECKED : BST_UNCHECKED));
            }
          }
          PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,IDC_STYLEEDIT)),1);
          break;

        case IDC_STYLEBACK:
          if (pCurrentStyle)
          {
            WCHAR tch[256];
            GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
            if (Style_SelectColor(hwnd,FALSE,tch,COUNTOF(tch))) {
              SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
              //CheckDlgButton(hwnd,IDC_STYLEBOLD,(Style_StrGetAttribute(tch,L"bold") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEITALIC,(Style_StrGetAttribute(tch,L"italic") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,(Style_StrGetAttribute(tch,L"underline") ? BST_CHECKED : BST_UNCHECKED));
              //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,(Style_StrGetAttribute(tch,L"eolfilled") ? BST_CHECKED : BST_UNCHECKED));
            }
          }
          PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,IDC_STYLEEDIT)),1);
          break;

        case IDC_STYLEDEFAULT:
          if (pCurrentStyle)
          {
            lstrcpy(pCurrentStyle->szValue,pCurrentStyle->pszDefault);
            SetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue);
            //CheckDlgButton(hwnd,IDC_STYLEBOLD,(Style_StrGetAttribute(pCurrentStyle->szValue,L"bold") ? BST_CHECKED : BST_UNCHECKED));
            //CheckDlgButton(hwnd,IDC_STYLEITALIC,(Style_StrGetAttribute(pCurrentStyle->szValue,L"italic") ? BST_CHECKED : BST_UNCHECKED));
            //CheckDlgButton(hwnd,IDC_STYLEUNDERLINE,(Style_StrGetAttribute(pCurrentStyle->szValue,L"underline") ? BST_CHECKED : BST_UNCHECKED));
            //CheckDlgButton(hwnd,IDC_STYLEEOLFILLED,(Style_StrGetAttribute(pCurrentStyle->szValue,L"eolfilled") ? BST_CHECKED : BST_UNCHECKED));
          }
          else if (pCurrentLexer) {
            lstrcpy(pCurrentLexer->szExtensions,pCurrentLexer->pszDefExt);
            SetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentLexer->szExtensions);
          }
          PostMessage(hwnd,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hwnd,IDC_STYLEEDIT)),1);
          break;

        //case IDC_STYLEBOLD:
        //  if (pCurrentStyle)
        //  {
        //    WCHAR tch[256];
        //    GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
        //    Style_StrSetAttribute(tch,COUNTOF(tch),L"bold",IsDlgButtonChecked(hwnd,IDC_STYLEBOLD));
        //    SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
        //  }
        //  break;

        //case IDC_STYLEITALIC:
        //  if (pCurrentStyle)
        //  {
        //    WCHAR tch[256];
        //    GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
        //    Style_StrSetAttribute(tch,COUNTOF(tch),L"italic",IsDlgButtonChecked(hwnd,IDC_STYLEITALIC));
        //    SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
        //  }
        //  break;

        //case IDC_STYLEUNDERLINE:
        //  if (pCurrentStyle)
        //  {
        //    WCHAR tch[256];
        //    GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
        //    Style_StrSetAttribute(tch,COUNTOF(tch),L"underline",IsDlgButtonChecked(hwnd,IDC_STYLEUNDERLINE));
        //    SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
        //  }
        //  break;

        //case IDC_STYLEEOLFILLED:
        //  if (pCurrentStyle)
        //  {
        //    WCHAR tch[256];
        //    GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));
        //    Style_StrSetAttribute(tch,COUNTOF(tch),L"eolfilled",IsDlgButtonChecked(hwnd,IDC_STYLEEOLFILLED));
        //    SetDlgItemText(hwnd,IDC_STYLEEDIT,tch);
        //  }
        //  break;

        case IDC_STYLEEDIT:
          {
            if (HIWORD(wParam) == EN_CHANGE) {
              int cr;
              WCHAR tch[256];

              GetDlgItemText(hwnd,IDC_STYLEEDIT,tch,COUNTOF(tch));

              cr = -1;
              Style_StrGetColor(TRUE,tch,&cr);
              MakeColorPickButton(hwnd,IDC_STYLEFORE,g_hInstance,cr);

              cr = -1;
              Style_StrGetColor(FALSE,tch,&cr);
              MakeColorPickButton(hwnd,IDC_STYLEBACK,g_hInstance,cr);
            }
          }
          break;

        case IDC_IMPORT:
          {
            HWND hwndTV = GetDlgItem(hwnd,IDC_STYLELIST);

            if (pCurrentStyle)
              GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue,COUNTOF(pCurrentStyle->szValue));
            else if (pCurrentLexer) {
              if (!GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentLexer->szExtensions,COUNTOF(pCurrentLexer->szExtensions)))
                lstrcpy(pCurrentLexer->szExtensions,pCurrentLexer->pszDefExt);
            }

            if (Style_Import(hwnd)) {

              if (pCurrentStyle)
                SetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue);
              else if (pCurrentLexer)
                SetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentLexer->szExtensions);

              TreeView_Select(hwndTV,TreeView_GetRoot(hwndTV),TVGN_CARET);
            }
          }
          break;

        case IDC_EXPORT:
          {
            if (pCurrentStyle)
              GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue,COUNTOF(pCurrentStyle->szValue));
            else if (pCurrentLexer) {
              if (!GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentLexer->szExtensions,COUNTOF(pCurrentLexer->szExtensions)))
                lstrcpy(pCurrentLexer->szExtensions,pCurrentLexer->pszDefExt);
            }

            Style_Export(hwnd);
          }
          break;

        case IDC_PREVIEW:
          {
            // Hack from outside
            extern HWND hwndEdit;
            extern void UpdateLineNumerWidth();

            if (pCurrentStyle)
              GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue,COUNTOF(pCurrentStyle->szValue));
            else if (pCurrentLexer) {
              if (!GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentLexer->szExtensions,COUNTOF(pCurrentLexer->szExtensions)))
                lstrcpy(pCurrentLexer->szExtensions,pCurrentLexer->pszDefExt);
            }

            Style_SetLexer(hwndEdit,pLexCurrent);
            UpdateLineNumberWidth();
          }
          break;

        case IDOK:
          if (pCurrentStyle)
            GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentStyle->szValue,COUNTOF(pCurrentStyle->szValue));
          else if (pCurrentLexer) {
            if (!GetDlgItemText(hwnd,IDC_STYLEEDIT,pCurrentLexer->szExtensions,COUNTOF(pCurrentLexer->szExtensions)))
              lstrcpy(pCurrentLexer->szExtensions,pCurrentLexer->pszDefExt);
          }
          EndDialog(hwnd,IDOK);
          break;

        case IDCANCEL:
          if (fDragging)
            SendMessage(hwnd,WM_CANCELMODE,0,0);
          else
            EndDialog(hwnd,IDCANCEL);
          break;

      }

      return TRUE;

  }

  return FALSE;

}


//=============================================================================
//
//  Style_ConfigDlg()
//
void Style_ConfigDlg(HWND hwnd)
{

  WCHAR *StyleBackup[1024];
  int c,cItems,i,iLexer;

  // Backup Styles
  c = 0;
  for (iLexer = 0; iLexer < NUMLEXERS; iLexer++) {
    StyleBackup[c++] = StrDup(pLexArray[iLexer]->szExtensions);
    i = 0;
    while (pLexArray[iLexer]->Styles[i].iStyle != -1) {
      StyleBackup[c++] = StrDup(pLexArray[iLexer]->Styles[i].szValue);
      i++;
    }
  }
  cItems = c;

  if (IDCANCEL == ThemedDialogBoxParam(g_hInstance,
                    MAKEINTRESOURCE(IDD_STYLECONFIG),
                    GetParent(hwnd),Style_ConfigDlgProc,
                    (LPARAM)&StyleBackup))
  {
    // Restore Styles
    c = 0;
    for (iLexer = 0; iLexer < NUMLEXERS; iLexer++) {
      lstrcpy(pLexArray[iLexer]->szExtensions,StyleBackup[c++]);
      i = 0;
      while (pLexArray[iLexer]->Styles[i].iStyle != -1) {
        lstrcpy(pLexArray[iLexer]->Styles[i].szValue,StyleBackup[c++]);
        i++;
      }
    }
  }

  else {
    fStylesModified = TRUE;
    if (lstrlen(szIniFile) == 0 && !fWarnedNoIniFile) {
      MsgBox(MBWARN,IDS_SETTINGSNOTSAVED);
      fWarnedNoIniFile = TRUE;
    }
  }

  for (c = 0; c < cItems; c++)
    LocalFree(StyleBackup[c]);

  // Apply new (or previous) Styles
  Style_SetLexer(hwnd,pLexCurrent);
}


//=============================================================================
//
//  Style_SelectLexerDlgProc()
//
INT_PTR CALLBACK Style_SelectLexerDlgProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{

  static int cxClient;
  static int cyClient;
  static int mmiPtMaxY;
  static int mmiPtMinX;

  static HWND hwndLV;
  static int  iInternalDefault;

  switch(umsg)
  {

    case WM_INITDIALOG:
      {
        int i;
        int lvItems;
        LVITEM lvi;
        SHFILEINFO shfi;
        LVCOLUMN lvc = { LVCF_FMT|LVCF_TEXT, LVCFMT_LEFT, 0, L"", -1, 0, 0, 0 };

        RECT rc;
        WCHAR tch[MAX_PATH];
        int cGrip;

        GetClientRect(hwnd,&rc);
        cxClient = rc.right - rc.left;
        cyClient = rc.bottom - rc.top;

        AdjustWindowRectEx(&rc,GetWindowLong(hwnd,GWL_STYLE)|WS_THICKFRAME,FALSE,0);
        mmiPtMinX = rc.right-rc.left;
        mmiPtMaxY = rc.bottom-rc.top;

        if (cxStyleSelectDlg < (rc.right-rc.left))
          cxStyleSelectDlg = rc.right-rc.left;
        if (cyStyleSelectDlg < (rc.bottom-rc.top))
          cyStyleSelectDlg = rc.bottom-rc.top;
        SetWindowPos(hwnd,NULL,rc.left,rc.top,cxStyleSelectDlg,cyStyleSelectDlg,SWP_NOZORDER);

        SetWindowLongPtr(hwnd,GWL_STYLE,GetWindowLongPtr(hwnd,GWL_STYLE)|WS_THICKFRAME);
        SetWindowPos(hwnd,NULL,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);

        GetMenuString(GetSystemMenu(GetParent(hwnd),FALSE),SC_SIZE,tch,COUNTOF(tch),MF_BYCOMMAND);
        InsertMenu(GetSystemMenu(hwnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_STRING|MF_ENABLED,SC_SIZE,tch);
        InsertMenu(GetSystemMenu(hwnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_SEPARATOR,0,NULL);

        SetWindowLongPtr(GetDlgItem(hwnd,IDC_RESIZEGRIP3),GWL_STYLE,
          GetWindowLongPtr(GetDlgItem(hwnd,IDC_RESIZEGRIP3),GWL_STYLE)|SBS_SIZEGRIP|WS_CLIPSIBLINGS);

        cGrip = GetSystemMetrics(SM_CXHTHUMB);
        SetWindowPos(GetDlgItem(hwnd,IDC_RESIZEGRIP3),NULL,cxClient-cGrip,
                     cyClient-cGrip,cGrip,cGrip,SWP_NOZORDER);

        hwndLV = GetDlgItem(hwnd,IDC_STYLELIST);

        ListView_SetImageList(hwndLV,
          (HIMAGELIST)SHGetFileInfo(L"C:\\",0,&shfi,sizeof(SHFILEINFO),SHGFI_SMALLICON | SHGFI_SYSICONINDEX),
          LVSIL_SMALL);

        ListView_SetImageList(hwndLV,
          (HIMAGELIST)SHGetFileInfo(L"C:\\",0,&shfi,sizeof(SHFILEINFO),SHGFI_LARGEICON | SHGFI_SYSICONINDEX),
          LVSIL_NORMAL);

        //SetExplorerTheme(hwndLV);
        ListView_SetExtendedListViewStyle(hwndLV,/*LVS_EX_FULLROWSELECT|*/LVS_EX_DOUBLEBUFFER|LVS_EX_LABELTIP);
        ListView_InsertColumn(hwndLV,0,&lvc);

        // Add lexers
        for (i = 0; i < NUMLEXERS; i++)
          Style_AddLexerToListView(hwndLV,pLexArray[i]);

        ListView_SetColumnWidth(hwndLV,0,LVSCW_AUTOSIZE_USEHEADER);

        // Select current lexer
        lvItems = ListView_GetItemCount(hwndLV);
        lvi.mask = LVIF_PARAM;
        for (i = 0; i < lvItems; i++) {
          lvi.iItem = i;
          ListView_GetItem(hwndLV,&lvi);;
          if (lstrcmp(((PEDITLEXER)lvi.lParam)->pszName,pLexCurrent->pszName) == 0) {
            ListView_SetItemState(hwndLV,i,LVIS_FOCUSED|LVIS_SELECTED,LVIS_FOCUSED|LVIS_SELECTED);
            ListView_EnsureVisible(hwndLV,i,FALSE);
            if (iDefaultLexer == i) {
              CheckDlgButton(hwnd,IDC_DEFAULTSCHEME,BST_CHECKED);
            }
          }
        }

        iInternalDefault = iDefaultLexer;

        if (bAutoSelect)
          CheckDlgButton(hwnd,IDC_AUTOSELECT,BST_CHECKED);

        CenterDlgInParent(hwnd);
      }
      return TRUE;


    case WM_DESTROY:
      {
        RECT rc;

        GetWindowRect(hwnd,&rc);
        cxStyleSelectDlg = rc.right-rc.left;
        cyStyleSelectDlg = rc.bottom-rc.top;
      }
      return FALSE;


    case WM_SIZE:
      {
        RECT rc;

        int dxClient = LOWORD(lParam) - cxClient;
        int dyClient = HIWORD(lParam) - cyClient;
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);

        GetWindowRect(GetDlgItem(hwnd,IDC_RESIZEGRIP3),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDC_RESIZEGRIP3),NULL,rc.left+dxClient,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        InvalidateRect(GetDlgItem(hwnd,IDC_RESIZEGRIP3),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDOK),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDOK),NULL,rc.left+dxClient,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        InvalidateRect(GetDlgItem(hwnd,IDOK),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDCANCEL),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDCANCEL),NULL,rc.left+dxClient,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        InvalidateRect(GetDlgItem(hwnd,IDCANCEL),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDC_STYLELIST),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDC_STYLELIST),NULL,0,0,rc.right-rc.left+dxClient,rc.bottom-rc.top+dyClient,SWP_NOZORDER|SWP_NOMOVE);
        ListView_SetColumnWidth(GetDlgItem(hwnd,IDC_STYLELIST),0,LVSCW_AUTOSIZE_USEHEADER);
        InvalidateRect(GetDlgItem(hwnd,IDC_STYLELIST),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDC_AUTOSELECT),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDC_AUTOSELECT),NULL,rc.left,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        InvalidateRect(GetDlgItem(hwnd,IDC_AUTOSELECT),NULL,TRUE);

        GetWindowRect(GetDlgItem(hwnd,IDC_DEFAULTSCHEME),&rc);
        MapWindowPoints(NULL,hwnd,(LPPOINT)&rc,2);
        SetWindowPos(GetDlgItem(hwnd,IDC_DEFAULTSCHEME),NULL,rc.left,rc.top+dyClient,0,0,SWP_NOZORDER|SWP_NOSIZE);
        InvalidateRect(GetDlgItem(hwnd,IDC_DEFAULTSCHEME),NULL,TRUE);
      }
      return TRUE;


    case WM_GETMINMAXINFO:
      {
        LPMINMAXINFO lpmmi = (LPMINMAXINFO)lParam;
        lpmmi->ptMinTrackSize.x = mmiPtMinX;
        lpmmi->ptMinTrackSize.y = mmiPtMaxY;
        //lpmmi->ptMaxTrackSize.y = mmiPtMaxY;
      }
      return TRUE;


    case WM_NOTIFY: {
      if (((LPNMHDR)(lParam))->idFrom == IDC_STYLELIST) {

      switch (((LPNMHDR)(lParam))->code) {

        case NM_DBLCLK:
          SendMessage(hwnd,WM_COMMAND,MAKELONG(IDOK,1),0);
          break;

        case LVN_ITEMCHANGED:
        case LVN_DELETEITEM: {
          int i = ListView_GetNextItem(hwndLV,-1,LVNI_ALL | LVNI_SELECTED);
          if (iInternalDefault == i)
            CheckDlgButton(hwnd,IDC_DEFAULTSCHEME,BST_CHECKED);
          else
            CheckDlgButton(hwnd,IDC_DEFAULTSCHEME,BST_UNCHECKED);
          EnableWindow(GetDlgItem(hwnd,IDC_DEFAULTSCHEME),i != -1);
          EnableWindow(GetDlgItem(hwnd,IDOK),i != -1);
          }
          break;
        }
      }
    }

      return TRUE;


    case WM_COMMAND:

      switch(LOWORD(wParam))
      {

        case IDC_DEFAULTSCHEME:
          if (IsDlgButtonChecked(hwnd,IDC_DEFAULTSCHEME) == BST_CHECKED)
            iInternalDefault = ListView_GetNextItem(hwndLV,-1,LVNI_ALL | LVNI_SELECTED);
          else
            iInternalDefault = 0;
          break;


        case IDOK:
          {
            LVITEM lvi;

            lvi.mask = LVIF_PARAM;
            lvi.iItem = ListView_GetNextItem(hwndLV,-1,LVNI_ALL | LVNI_SELECTED);
            if (ListView_GetItem(hwndLV,&lvi)) {
              pLexCurrent = (PEDITLEXER)lvi.lParam;
              iDefaultLexer = iInternalDefault;
              bAutoSelect = (IsDlgButtonChecked(hwnd,IDC_AUTOSELECT) == BST_CHECKED) ? 1 : 0;
              EndDialog(hwnd,IDOK);
            }
          }
          break;


        case IDCANCEL:
          EndDialog(hwnd,IDCANCEL);
          break;

      }

      return TRUE;

  }

  return FALSE;

}


//=============================================================================
//
//  Style_SelectLexerDlg()
//
void Style_SelectLexerDlg(HWND hwnd)
{
  if (IDOK == ThemedDialogBoxParam(g_hInstance,
                MAKEINTRESOURCE(IDD_STYLESELECT),
                GetParent(hwnd),Style_SelectLexerDlgProc,0))

    Style_SetLexer(hwnd,pLexCurrent);
}


// End of Styles.c
