/* perhtm.c */

/* read from input docin_lines string array
* format and write an html output file
*/


/* #define GBL_HTML_HAS_NEWLINES 1 */
#define GBL_HTML_HAS_NEWLINES 0

int gbl_we_are_in_PRE_block_content; /* 1 = yes, 0 = no */
char gbl_person_name[64]; 
char gbl_pfnameHTML[256];

#include "rkdebug_externs.h"

#include "rk.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "perhtm.h"

/* #define APP_NAME "Astrology for Me" */
/* #define APP_NAME "Astrology by Measurement" */
/* #define APP_NAME "Me & My BFFs" */
/* #define APP_NAME "\"Me & my BFFs\"" */
/* #define APP_NAME "\"My BFFs and I\"" */
#define APP_NAME "\"Me and my BFFs\""

/* file extension for group sharing will be ".mamb" */

#define MAX_STARS 82  /* in perdoc.h also */
#define GRH_CHAR '*'  /* in perdoc.h also */

/* int rkdb = 0; */ /* 0=no, 1=yes */



void add_all_benchmark_lines(void);
void write_html_for_trait_table(void);

struct trait_table_line {
  char    trait[16];
  char    score[6];
  char    influence[16];
};
struct trait_table_line trait_lines[12];  /* need 11 */

int Func_compare_trait_line_scores(
  struct trait_table_line *line1,
  struct trait_table_line *line2
);
typedef int (*compareFunc_trait) (const void *, const void *);



void p_fn_prtlin(char *lin);
void p_fn_prtlin_aspect(char *lin);  /* no \n at end */
void p_fn_prtlin_stars(char *starline);

char  swork33[4048];


/* in mambutil.o */
extern int  sfind(char s[], char c);
extern int scharcnt(char *s, int c);
extern int sall(char *s, char *set);
extern char *mkstr(char *s, char *begarg, char *end);
extern void scharout(char *s, int c);
extern void put_br_every_n(char *instr, int line_not_longer_than_this);
extern void fn_right_pad_with_hidden(char *s_to_pad, int num_to_pad);
extern int binsearch_asp(char *asp_code, struct aspect tab[], int num_elements);
/* in mambutil.o */


extern void p_docin_free(void);   /* in grpdoc.o */


void p_docin_get(char *in_line);   

char doclin[512];

/* -------------------------- */
int    global_max_docin_idx;
char **global_docin_lines;
int    global_read_idx;
int    global_n;
char  *global_p = &swork33[0];
/* -------------------------- */


FILE *Fp_p_HTML_file;

/* char s1[512]; */
/* char s2[512]; */
/* char s3[512]; */
/* char s4[512]; */
/* char s5[512]; */
/* char s6[512]; */
char writebuf[4048];
char workbuf[4048];



#define MAX_WK 10
struct {
  char wk[133]; 
} wks[MAX_WK];
#define arr(nn) (wks[nn].wk)  /* arr arr arr arr arr  HERE */
/* wks is array of struct size 133 chars
* 
*  (wks[k].wk)  <==>   arr(k)
*   
*  this expression:  (wks[k].wk)  
*  gives you the kth 133-char buffer in array wks.
*
*  With the *define* below, you can say this: 
*  arr(k) for the same buffer.
*/



void do_orig_trait_graph(void);
void do_benchmark_trait_graph(void);
void p_fn_output_top_of_html_file(void);

char aspect_code[10];
void p_fn_aspect_text(char *aspect_code);

int is_first_p_docin_get;  /* 1=yes, 0=no */





/* invocation
*
* retval = make_per_htm_file(html_output_filename, docin_lines, docin_idx);
*
*  output entire ".html" file
*/
int make_per_htm_file(      /* old main() */
  char *in_html_filename,
  char *in_docin_lines[],
  int   in_docin_last_idx
)
{
  is_first_p_docin_get = 1;       /* init to true */
  gbl_we_are_in_PRE_block_content = 0;  /* init to false */

  strcpy(gbl_pfnameHTML, in_html_filename);

  int i;
/* trn("in make_per_htm_file()"); */
/* tn();b(160);ksn(in_html_filename); */

  global_max_docin_idx = in_docin_last_idx;
  global_docin_lines   = in_docin_lines;

  /* open output HTML file
  */
  if ( (Fp_p_HTML_file = fopen(in_html_filename, "w")) == NULL ) {
    rkabort("Error  on   perhtm.c.  fopen().");
  }
/* tn();b(10); */

/* for test */
/* p_fn_prtlin("hey owieurw0093203");
* p_docin_get(doclin);
* p_fn_prtlin(doclin);
* p_docin_get(doclin);
* p_fn_prtlin(doclin);
* p_docin_get(doclin);
* p_fn_prtlin(doclin);
*/
/* for test */


  /* in this fn is the first p_docin_get
  */
  p_fn_output_top_of_html_file();  /* output the css, headings etc. */




/* b(11); */
  /*  read until [beg_graph]
   */
  for (i=0; ; i++) {
    p_docin_get(doclin);
    if (strstr(doclin, "[beg_graph]") != NULL) break;
  }

/*   p_fn_prtlin("  <div> Stand out character traits.</div>"); */

  /*  old graph   do_orig_trait_graph(); */
  p_fn_prtlin("<div> </div>");

  do_benchmark_trait_graph();    /* !!!!!!!!!!!!!!!!!!!  trait table printed here !!! */

/* b(12); */

  /*   p_fn_prtlin("\n<h3> </h3>"); */
  p_fn_prtlin("<div><br></div>");

  p_fn_prtlin("</table>");
  p_fn_prtlin("<pre>");
  gbl_we_are_in_PRE_block_content = 1; /* 1 = yes, 0 = no */
  p_fn_prtlin("");
/*   p_fn_prtlin("  A score measures how high or low the influence  "); */

/*   p_fn_prtlin("  A score measures how influential the trait is  "); */
/*   sprintf(writebuf, "  (1 to 99) in the personality of %s.  ", gbl_person_name); */
/*   p_fn_prtlin(writebuf); */

  p_fn_prtlin("  A score from 1 to 99 measures how influential  ");
  sprintf(writebuf, "  the trait is in the personality of %s.  ", gbl_person_name);
  p_fn_prtlin(writebuf);



  p_fn_prtlin("");
/*   p_fn_prtlin("  The score does NOT measure \"good\" or \"bad\", "); */
  p_fn_prtlin("  The score here does NOT measure \"good\" or \"bad\",  ");
  p_fn_prtlin("    which can be found in the paragraphs below: ");
  p_fn_prtlin("");
  gbl_we_are_in_PRE_block_content = 0; /* 1 = yes, 0 = no */
  p_fn_prtlin("</pre>");
  p_fn_prtlin("<div> </div>");

  /* DO PARAGRAPHS HERE */

  /* read until
  */
  for (i=0; ; i++) {
    p_docin_get(doclin);
    if (strstr(doclin, "[beg_aspects]") != NULL) break;
  }


  /* now read and print aspects until we hit [end_aspects] 
  */
  for (i=0; ; i++) {
    p_docin_get(doclin);
    if (strlen(doclin) == 0) continue;
    if (strstr(doclin, "[end_aspects]") != NULL) break;
    
    strcpy(aspect_code, doclin);
    p_fn_aspect_text(aspect_code);    /* output the aspect text */
    
  }  /* read and print aspects until we hit [end_aspects] */

/* b(14); */


  for (i=0; ; i++) {  /* read until  */
    p_docin_get(doclin);
    if (strstr(doclin, "[end_program]") != NULL) break;
  }


/*   p_fn_prtlin("<div> </div>"); */
  p_fn_prtlin("<div><br></div>");

  /* put trait descriptions
  */
  p_fn_prtlin("<pre>");
  gbl_we_are_in_PRE_block_content = 1;  /* true */
  p_fn_prtlin("    *trait");
  p_fn_prtlin("");
  p_fn_prtlin("     Assertive     - competitive, authoritative, outspoken  ");
  p_fn_prtlin("     Emotional     - protective, sensitive, possessive");
  p_fn_prtlin("     Restless      - versatile, changeable, independent");
  p_fn_prtlin("     Down-to-earth - stable, practical, ambitious");
  p_fn_prtlin("     Passionate    - intense, relentless, enthusiastic");
  p_fn_prtlin("     Ups and Downs - having very high ups ");
  p_fn_prtlin("                     and very low downs in life ");
  p_fn_prtlin("");
  p_fn_prtlin("");
  p_fn_prtlin("  Check out reports \"Most Assertive\", \"Most Emotional\" ...  ");
  p_fn_prtlin("  which use trait scores to compare with other group members  ");
  p_fn_prtlin("");
  gbl_we_are_in_PRE_block_content = 0;  /* false */
  p_fn_prtlin("</pre>");

  p_fn_prtlin("<div> </div>");

/*   p_fn_prtlin("<pre>"); */
/*   gbl_we_are_in_PRE_block_content = 1; */
/*   p_fn_prtlin(""); */
/*   p_fn_prtlin(" Check out group reports \"Most Assertive\", \"Most Emotional\" etc. "); */
/*   p_fn_prtlin("     which use these scores to compare with other group members.     "); */
/*   p_fn_prtlin("  Check out group reports \"Most Assertive\", \"Most Emotional\" etc.  "); */
/*   p_fn_prtlin("  which use these trait scores to compare with other group members.  "); */
/*   p_fn_prtlin(""); */
/*   gbl_we_are_in_PRE_block_content = 0; */
/*   p_fn_prtlin("</pre>"); */

  p_fn_prtlin("<div><br></div>");

  p_fn_prtlin("<pre> ");
  gbl_we_are_in_PRE_block_content = 1; 
  p_fn_prtlin( "                                       ");
  p_fn_prtlin( "  Your intense willpower can overcome  ");
  p_fn_prtlin( "   bad traits and magnify good ones    ");
  p_fn_prtlin( "                                       ");
  gbl_we_are_in_PRE_block_content = 0;  /* false */
  p_fn_prtlin("</pre>");


/*   sprintf(writebuf, "<h5><br><br><br>produced by iPhone/iPad app named %s</h5>", APP_NAME); */
/*   sprintf(writebuf, "<h5><br><br><br>produced by iPhone app %s</h5>", APP_NAME); */
  sprintf(writebuf, "<h5><br>produced by iPhone app %s</h5>", APP_NAME);
  p_fn_prtlin(writebuf);

  p_fn_prtlin("");

/*   p_fn_prtlin("<h4><span style=\"background-color:#FFBAC7;\">&nbsp&nbsp&nbsp&nbsp&nbsp  This report is for entertainment purposes only.&nbsp&nbsp&nbsp&nbsp&nbsp  </span></h4>"); */
  p_fn_prtlin("<h4><span style=\"background-color:#FFBAC7;\">&nbspThis report is for entertainment purposes only.&nbsp</span></h4>");


  p_fn_prtlin("\n</body>\n");
  p_fn_prtlin("</html>");


  fflush(Fp_p_HTML_file);
  /* close output HTML file
  */
  if (fclose(Fp_p_HTML_file) == EOF) {
    ;
/* trn("FCLOSE FAILED !!!   #1  "); */
  } else {
    ;
/* trn("FCLOSE SUCCESS !!!  #1  "); */
  };

/* b(15); */
  return(0);

} /* end of  make_per_htm_file() */


/* output the css, headings etc.
*/
void p_fn_output_top_of_html_file(void)
{
  int i;
/* trn("in p_fn_output_top_of_html_file()"); */

  /* 1. read until [beg_topinfo1]  (name)  (skipping [beg_program])
  */
  for (i=0; ; i++) {
    p_docin_get(doclin);
    if (strstr(doclin, "[beg_topinfo1]") != NULL) break;
  }

/* b(20); */
  /* then save lines until graph until [end_topinfo1] 
  * then put out html 
  */
  for (i=0; ; i++) {
    p_docin_get(doclin);
    if (strstr(doclin, "[end_topinfo1]") != NULL) break;
    strcpy(arr(i), doclin);
  }
/* b(25); */

/* <.>  at end, change to STRICT  */
  p_fn_prtlin( "<!doctype html public \"-//w3c//dtd html 4.01 transitional//en\" ");
/* b(26); */
  p_fn_prtlin( "  \"http://www.w3.org/TR/html4/loose.dtd\">");

  p_fn_prtlin( "<html>");
  p_fn_prtlin( "\n<head>");

/*   sprintf(writebuf, "  <title>%s- Personality, produced by iPhone app %s.</title>",arr(0), APP_NAME); */

  /* if HTML filename, gbl_pfnameHTML, has any slashes, grab the basename
  */
  char myBaseName[256], *myptr;
  if (sfind(gbl_pfnameHTML, '/')) {
    myptr = strrchr(gbl_pfnameHTML, '/');
    strcpy(myBaseName, myptr + 1);
  } else {
    strcpy(myBaseName, gbl_pfnameHTML);
  }
  sprintf(writebuf, "  <title>%s</title>", myBaseName);
  p_fn_prtlin(writebuf);
/* b(27); */
  
  /* HEAD  META
  */
  sprintf(writebuf, "  <meta name=\"description\" content=\"Personality report produced by iPhone app %s\"> ",  APP_NAME);
  p_fn_prtlin(writebuf);


  p_fn_prtlin( "  <meta http-equiv=\"Content-Type\" content=\"text/html\"; charset=\"iso-8859-1\">"); 
  /*   p_fn_prtlin( "  <meta name=\"Author\" content=\"Author goes here\">"); */


/*   p_fn_prtlin( "  <meta name=\"keywords\" content=\"group,member,astrology,personality,future,past,year in the life,compatibility,GMCR\">  */
/*   p_fn_prtlin( "  <meta name=\"keywords\" content=\"measure,group,member,best,match,calendar,year,passionate,personality\"> "); */
/*   p_fn_prtlin( "  <meta name=\"keywords\" content=\"BFF,astrology,compatibility,group,best,match,calendar,year,stress,personality\"> "); */

/*   p_fn_prtlin( "  <meta name=\"keywords\" content=\"women,woman,female,BFF,compatibility,group,best,match,personality,stress,calendar,year\"> "); */ /* 86 chars */ 
  p_fn_prtlin( "  <meta name=\"keywords\" content=\"women,woman,female,BFF,me,compatibility,group,best,match,personality,stress,calendar,year\"> ");  /* 89 chars */


  /* get rid of CHROME translate "this page is in Galician" 
  * do you want to translate?
  */
  p_fn_prtlin("  <meta name=\"google\" content=\"notranslate\">");
  p_fn_prtlin("  <meta http-equiv=\"Content-Language\" content=\"en\" />");


  /* Using the Viewport Meta Tag  (in iOS webView)
  * https://developer.apple.com/library/safari/documentation/AppleApplications/Reference/SafariWebContent/UsingtheViewport/UsingtheViewport.html#//apple_ref/doc/uid/TP40006509-SW25
  *
  * For example, TO SET THE VIEWPORT WIDTH TO THE WIDTH OF THE DEVICE, add this to your HTML file:
  * <meta name="viewport" content="width=device-width"> 
  * To set the initial scale to 1.0, add this to your HTML file:
  * <meta name="viewport" content="initial-scale=1.0"> 
  * To set the initial scale and to turn off user scaling, add this to your HTML file:
  * <meta name="viewport" content="initial-scale=2.3, user-scalable=no">
  */
  p_fn_prtlin("  <meta name=\"viewport\" content=\"width=device-width\" />");


  /* HEAD   STYLE/CSS
  */
  p_fn_prtlin( "\n  <style type=\"text/css\">");
  p_fn_prtlin( "    BODY {");

/*  p_fn_prtlin( "      background-color: #F5 EF CF;"); */
/*  g_fn_prtlin( "      background-color: #F5EFCF;"); */
  p_fn_prtlin( "      background-color: #F7ebd1;");

/*   p_fn_prtlin( "      font-family: Trebuchet MS, Arial, Verdana, sans-serif;"); */
  p_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");

  p_fn_prtlin( "      font-size:   medium;");
  p_fn_prtlin( "      font-weight: normal;");
  p_fn_prtlin( "      text-align:  center;");
  /* example comment out */
  /*   p_fn_prtlin( "    <!-- "); */
  /*   p_fn_prtlin( "      background-image: url('mkgif1g.gif');"); */
  /*   p_fn_prtlin( "    --> "); */
  p_fn_prtlin( "    }");

  p_fn_prtlin( "    H1 { font-size: 137%; font-weight: bold;   line-height: 95%; text-align: center;}");
  p_fn_prtlin( "    H2 { font-size: 125%;                      line-height: 25%; text-align: center;}");
  p_fn_prtlin( "    H3 { font-size: 110%; font-weight: normal; line-height: 30%; text-align: center;}");

/*   p_fn_prtlin( "    H5 { font-size:  55%; font-weight: normal; line-height: 90%; text-align: center;}"); */
/*   p_fn_prtlin( "    H4 { font-size:  85%; font-weight: bold;   line-height: 30%; text-align: center;}"); */
/*   p_fn_prtlin( "    H5 { font-size:  70%; font-weight: normal; line-height: 30%; text-align: center;}"); */

  p_fn_prtlin( "    H4 { font-size:  75%; font-weight: bold;   line-height: 30%; text-align: center;}");
  p_fn_prtlin( "    H5 { font-size:  70%; font-weight: normal; line-height: 30%; text-align: center;}");

  p_fn_prtlin( "    PRE {");
  p_fn_prtlin( "      background-color: #fcfce0;");
  p_fn_prtlin( "      text-align:  left;");
/*   p_fn_prtlin( "      font-size:   90%;"); */
  p_fn_prtlin( "      display: inline-block;");
/*   p_fn_prtlin( "      border-style: solid;"); */
/*   p_fn_prtlin( "      border-color: black;"); */
/*   p_fn_prtlin( "      border-width: 2px;"); */
/*   p_fn_prtlin( "      border-color: #e4dfae; "); */
/*   p_fn_prtlin( "      border-width: 5px;"); */

  p_fn_prtlin( "      border-spacing: 0;");
  p_fn_prtlin( "      border: none;");
  p_fn_prtlin( "      border-collapse: collapse;");
  p_fn_prtlin( "      border-spacing: 0;");
/*   p_fn_prtlin( "      font-family: Andale Mono, Menlo, Monospace, Courier New;"); */
  p_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");
  p_fn_prtlin( "      font-weight: normal;");
/*   p_fn_prtlin( "      line-height: 70%;"); */
/*   p_fn_prtlin( "      line-height: 100%;"); */
/*   p_fn_prtlin( "      line-height: 108%;"); */
  p_fn_prtlin( "      font-size: 85%;");
/*   p_fn_prtlin( "      margin:0 auto;"); */
  p_fn_prtlin( "    }");


  p_fn_prtlin( "    P { ");
/*   p_fn_prtlin( "      font-family: Andale Mono, Menlo, Monospace, Courier New;"); */
  p_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");
  p_fn_prtlin( "      width: auto;");
  p_fn_prtlin( "      font-size:   93%;");
  p_fn_prtlin( "      margin-top: 0;");
  p_fn_prtlin( "      margin-bottom: 0;");
  p_fn_prtlin( "      margin-left: auto;");
  p_fn_prtlin( "      margin-right:auto;");
/*   p_fn_prtlin( "      padding-left: 5%;"); */
/*   p_fn_prtlin( "      padding-right:5%;"); */
/*   p_fn_prtlin( "      text-align: center;"); */
  p_fn_prtlin( "      text-align: left;");
  p_fn_prtlin( "    }");
/* for table: */
/*       border: 2px solid black; */
/*       cellspacing: 0; */
/*       border-top: 0; */
/*       border-bottom: 0; */
  p_fn_prtlin( "    table {");
  p_fn_prtlin( "      border: none;");
  p_fn_prtlin( "      border-collapse: collapse;");
  p_fn_prtlin( "      border-spacing: 0;");
  p_fn_prtlin( "    }");
  p_fn_prtlin( "    table.center {");
  p_fn_prtlin( "      margin-left:auto;");
  p_fn_prtlin( "      margin-right:auto;");
  p_fn_prtlin( "    }");
  p_fn_prtlin( "    TD {");
  p_fn_prtlin( "      border: none;");
  p_fn_prtlin( "      border-spacing: 0;");
  p_fn_prtlin( "      white-space: nowrap;");
  p_fn_prtlin( "      padding: 0;");
  p_fn_prtlin( "    }");

/*   p_fn_prtlin( "    .cLineGood   { background-color:#c3fdc3; }"); */
  p_fn_prtlin( "    .cLineStress { background-color:#ffbac7; }");
  p_fn_prtlin( "    .row4        { background-color:#f8f0c0; }");

/*   p_fn_prtlin( "    .cGr2        { background-color:#d0fda0; }"); */
/*   p_fn_prtlin( "    .cGr2        { background-color:#8bfd87; }"); */
/*   p_fn_prtlin( "    .cGr2        { background-color:#66ff33; }"); */

/*   p_fn_prtlin( "    .cGre        { background-color:#e1fdc3; }"); */
/*   p_fn_prtlin( "    .cGre        { background-color:#ccffb9; }"); */
/*   p_fn_prtlin( "    .cRed        { background-color:#ffd9d9; }"); */
/*   p_fn_prtlin( "    .cRed        { background-color:#ffcccc; }"); */
/*   p_fn_prtlin( "    .cRe2        { background-color:#fcb9b9; }"); */
/*   p_fn_prtlin( "    .cRe2        { background-color:#fc8888; }"); */
/*   p_fn_prtlin( "    .cRe2        { background-color:#fc6094; }"); */
/*   p_fn_prtlin( "    .cRe2        { background-color:#ff3366; }"); */

/*    GREEN   */
/*     .cGr2        { background-color:#a3f275; } */
/*     .cGre        { background-color:#bbf699; } */
/*     .cNeu        { background-color:#d3f9bd; } */
/*     .cRed        { background-color:#bbf699; } */
/*     .cRe2        { background-color:#a3f275; } */


/*   p_fn_prtlin( "    .cGr2        { background-color:#66ff33; }"); */
/*   p_fn_prtlin( "    .cGr2        { background-color:#a3f275; }"); */
/*   p_fn_prtlin( "    .cGre        { background-color:#84ff98; }"); */
/*   p_fn_prtlin( "    .cGre        { background-color:#a8ff98; }"); */
/*   p_fn_prtlin( "    .cGre        { background-color:#bbf699; }"); */
/*   p_fn_prtlin( "    .cNeu        { background-color:#e1ddc3; }"); */
/*   p_fn_prtlin( "    .cNeu        { background-color:#d3f9bd; }"); */
/*   p_fn_prtlin( "    .cNeu        { background-color:#fcfce0; }"); */
/*   p_fn_prtlin( "    .cNeu        { background-color:#e6fbda; }"); */
/*   p_fn_prtlin( "      background-color: #fcfce0;"); */
/*   p_fn_prtlin( "    .cRed        { background-color:#ff98a8; }"); */
/*   p_fn_prtlin( "    .cRed        { background-color:#bbf699; }"); */
/*   p_fn_prtlin( "    .cRe2        { background-color:#ff4477; }"); */
/*   p_fn_prtlin( "    .cRe2        { background-color:#a3f275; }"); */

/*   p_fn_prtlin( "    .cGr2        { background-color:#a3f275; }"); */
/*   p_fn_prtlin( "    .cGre        { background-color:#a3f275; }"); */
/*   p_fn_prtlin( "    .cNeu        { background-color:#a3f275; }"); */
/*   p_fn_prtlin( "    .cRed        { background-color:#a3f275; }"); */
/*   p_fn_prtlin( "    .cRe2        { background-color:#a3f275; }"); */

/*   p_fn_prtlin( "    .cGr2,.cGre,.cNeu,.cRed,.cRe2 {background-color: #a3f275;}"); */
  p_fn_prtlin( "    .cGr2,.cGre,.cNeu,.cRed,.cRe2 {background-color: #d3ffa5;}");


  p_fn_prtlin("    .cNam        { color:#3f3ffa;");
  p_fn_prtlin("                   background-color: #F7ebd1;");
  p_fn_prtlin("                   font-size: 133%;");
  p_fn_prtlin("    }");

  p_fn_prtlin( "    table.trait {");
  p_fn_prtlin( "      margin-left: auto;");
  p_fn_prtlin( "      margin-right:auto;");
/*   p_fn_prtlin( "      font-family: Andale Mono, Monospace, Courier New;"); */
  p_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");
  p_fn_prtlin( "      text-align: left;");

/*   p_fn_prtlin( "      border: 1px solid black;"); */
  p_fn_prtlin( "      border: none;");
  p_fn_prtlin( "      border-spacing: 0;");
  p_fn_prtlin( "      border-collapse: collapse;");
  p_fn_prtlin( "      border-spacing: 0;");

/*   p_fn_prtlin( "      padding-right:2%;"); */
/*   p_fn_prtlin( "      padding-left:2%;"); */
  p_fn_prtlin( "    }");
  p_fn_prtlin( "    table.trait td {");
/*   p_fn_prtlin( "      font-family: Andale Mono, Monospace, Courier New;"); */
  p_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");
  p_fn_prtlin( "      white-space: pre;");
  p_fn_prtlin( "      font-size: 90%;");
  p_fn_prtlin( "      text-align: left;");

/*   p_fn_prtlin( "      border: 1px solid black;"); */
  p_fn_prtlin( "      border: none;");
  p_fn_prtlin( "      border-spacing: 0;");
  p_fn_prtlin( "      border-collapse: collapse;");
  p_fn_prtlin( "      border-spacing: 0;");

  p_fn_prtlin( "      padding-left: 10px; ");
  p_fn_prtlin( "      padding-right: 10px; ");

/*   p_fn_prtlin( "      padding-top: 2px; "); */
/*   p_fn_prtlin( "      padding-bottom: 2px; "); */
  p_fn_prtlin( "      padding-top: 1px; ");
  p_fn_prtlin( "      padding-bottom: 1px; ");

  p_fn_prtlin( "    }");
  p_fn_prtlin( "    table.trait th{");
/*   p_fn_prtlin( "      font-family: Trebuchet MS, Arial, Verdana, sans-serif;"); */
  p_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");
  p_fn_prtlin( "      font-size: 90%;");
/*   p_fn_prtlin( "      padding-left: 10px; "); */
/*   p_fn_prtlin( "      padding-right: 10px; "); */
  p_fn_prtlin( "      padding: 10px; ");

/*   p_fn_prtlin( "      background-color: #e1fdc3 ;"); */
  p_fn_prtlin( "      background-color: #fcfce0 ;");

/*   p_fn_prtlin( "      border: 1px solid black;"); */
  p_fn_prtlin( "      border: none;");
  p_fn_prtlin( "      border-spacing: 0;");

  p_fn_prtlin( "      text-align: center;");
  p_fn_prtlin( "    }");
  p_fn_prtlin( "    table.trait       td { text-align: left; }");
  p_fn_prtlin( "    table.trait    td+td { text-align: right; }");
  p_fn_prtlin( "    table.trait td+td+td { text-align: left; }");

  p_fn_prtlin( "  </style>");

/* put in favicon */
p_fn_prtlin("<link href=\"data:image/x-icon;base64,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\" rel=\"icon\" type=\"image/x-icon\" />");


  p_fn_prtlin( "</head>");
  p_fn_prtlin( " ");
  p_fn_prtlin("\n<body>");

  /*   p_fn_prtlin( "<body background=\"file:///mkgif1g.gif\">"); */
  /*   p_fn_prtlin( "<body background=\"file:///C/USR/RK/html/mkgif1g.gif\">"); */

  /* "Personality"   output here */
/* 
*   sprintf(writebuf, "\n  <h1>Personality</h1>");
*   p_fn_prtlin(writebuf);
*   sprintf(writebuf, "\n  <h2>of %s<br></h2>", arr(0) ); * of Fred  *
*   p_fn_prtlin(writebuf);
*/

/*   sprintf(writebuf, "\n  <h1>Personality <span style=\"font-size:80%%;\">of %s</span><br></h1>", arr(0) );  */
/*   sprintf(writebuf, "\n  <h1>Personality of %s<br></h1>", arr(0) ); * of Fred  * */

  p_fn_prtlin("<div><br></div>");
  sprintf(writebuf, "\n  <h1>Personality for <span class=\"cNam\">%s</span><br></h1>", arr(0) ); /* of Fred  */
  p_fn_prtlin(writebuf);

  strcpy(gbl_person_name, arr(0));

  p_fn_prtlin(" ");

/* b(29); */
} /* end of  p_fn_output_top_of_html_file() */


/* arg in_docin_last_idx  is pointing at the last line written.
* Therefore, the current docin_lines written
* run from index = 0 to index = arg in_docin_last_idx.
*/
void p_docin_get(char *in_line)
{
  
/* tr("in p_docin_get");  ksn(in_line); */
  if (is_first_p_docin_get == 1) global_read_idx = 0;
  else                           global_read_idx++;
  
  is_first_p_docin_get = 0;  /* set to false */

  if (global_read_idx > global_max_docin_idx) {
    p_docin_free();
    rkabort("Error. perhtm.c walked off end of docin_lines array");
  }

  strcpy(in_line, global_docin_lines[global_read_idx] );

  scharout(in_line,'\n');   /* remove newlines */

} /* end of p_docin_get */


/* ************************************************************
*
* ************************************************************/
void p_fn_aspect_text(char *aspect_code) {
  int nn;

/* tr("in p_fn_aspect_text()"); */

  nn = binsearch_asp(aspect_code, p_asptab, NKEYS_ASP);

  if (nn < 0) return;  /* do not print any aspect text at all  */

  strcpy(my_aspect_text, p_asptab[nn].asp_text);


  /* wrap lines at 80 chars with <br> */
/*   put_br_every_n(my_aspect_text, 80);  */
  put_br_every_n(my_aspect_text, 65); 

  char para_beg[133];
  char para_end[133];

  strcpy(para_beg, "<table class=\"center\"><tr><td><p>");
  strcpy(para_end, "</p></td></tr><br></table>");
  sprintf(writebuf, "  %s%s%s\n", para_beg, my_aspect_text, para_end);

  p_fn_prtlin(writebuf);

}  /* end of p_fn_aspect_text(); */


/* show "stars" in graph as green line
*  Must change all "*" to " ";
*/
void p_fn_prtlin_stars(char *starline)
{
/* trn("in p_fn_prtlin_stars()"); */

  char *pBegStar;
  char *pEndStar;
  char beforeStars[512];
  char allStars[512];  /* assume stars start at startline[0] */
  char afterStars[512];

  if (sall(starline, " ") == 1) {
    return;
  }

  if (scharcnt(starline, GRH_CHAR) == MAX_STARS) {
    sprintf(writebuf,
      "<span style=\"background-color:#e1fdc3;\">%s</span>", starline);
    p_fn_prtlin(writebuf);   /* write star line here */
    return;
  }


  pBegStar = strchr (starline, GRH_CHAR);
  pEndStar = strrchr(starline, GRH_CHAR);
  if (starline[0] == GRH_CHAR) {
    strcpy(beforeStars, "");
    mkstr(allStars,    pBegStar, pEndStar);
    mkstr(afterStars,  pEndStar + 1, starline + strlen(starline) - 1);
  } else {
    mkstr(beforeStars, starline, pBegStar - 1);
    mkstr(allStars,    pBegStar, pEndStar);
    strcpy(afterStars, "");
  }

  sprintf(writebuf,
    "%s<span style=\"background-color:#e1fdc3;\">%s</span>%s",
    beforeStars,
    allStars,
    afterStars
  ); 

  p_fn_prtlin(writebuf);   /* write star line here */

} /* end of p_fn_prtlin_stars() */

void p_fn_prtlin(char *lin) {
  char myEOL[8];
/* tn();
* tn();
* kin(GBL_HTML_HAS_NEWLINES);
* ksn(lin);
* kin(strlen(lin));
*/
  strcpy(myEOL, "\n");
  if (GBL_HTML_HAS_NEWLINES == 1)             strcpy(myEOL, "\n");
  if (GBL_HTML_HAS_NEWLINES == 0) {
/* tn();b(30);trn("calling scharout"); */


/*    scharout(lin,'\n'); */ /* remove newlines */


/* tn();b(31);trn("back from calling scharout"); */
/* ks(lin); */
    if (gbl_we_are_in_PRE_block_content == 1) strcpy(myEOL, "<br>");
    else                                      strcpy(myEOL, "");
  }
/* ksn(myEOL); */
  global_n = sprintf(global_p,"%s%s", lin, myEOL);
/* ksn(global_p); */
  fput(global_p, global_n, Fp_p_HTML_file);
/* b(35); */
} 

void p_fn_prtlin_aspect(char *lin) {  /* no \n at end  ( UNUSED ) */
  global_n = sprintf(global_p,"%s", lin);
  fput(global_p, global_n, Fp_p_HTML_file);
}  



#ifdef PUT_BACK_COMMENTED_OUT_STUFF /****************************************/
* void do_orig_trait_graph(void)
* {
* int i;
* 
*   /*  read until [beg_graph]
*    */
*   for (i=0; ; i++) {
*     p_docin_get(doclin);
*     if (strstr(doclin, "[beg_graph]") != NULL) break;
*   }
* 
*   p_fn_prtlin("  <div> Stand out character traits.</div>");
*  
*   p_fn_prtlin("<pre>");  /* start of graphs */
* 
*   gbl_we_are_in_PRE_block_content = 1;  /* true */
* 
*   p_fn_prtlin("  ");
* 
*   p_fn_prtlin("          less important                   important                     remarkable ");
*   p_fn_prtlin("               |                           |                             |          ");
*   p_fn_prtlin("  ");
*   p_fn_prtlin("  <span style=\"background-color:#fdfbe1\">ASSERTIVE </span> competitive, confident, enthusiastic                                     ");
* 
* 
*   /* ================================================================= */
*   /*  read until [beg_agrsv]   */
*   for (i=0; ; i++) {
*     p_docin_get(doclin);
*     if (strstr(doclin, "[beg_agrsv]") != NULL) break;
*   }
* 
*   for (i=0; ; i++) {  /* print until [end_agrsv] */
*     p_docin_get(doclin);
*     if (strstr(doclin, "[end_agrsv]") != NULL) break;
* 
*     scharout(doclin, '|');  /* remove pipes (for old sideline)    */
*     p_fn_prtlin_stars(doclin);  
*   }
*   p_fn_prtlin("  ");
* 
*   /* ================================================================= */
* 
*   /* ================================================================= */
*   for (i=0; ; i++) {
*     p_docin_get(doclin);
*     if (strstr(doclin, "[beg_sensi]") != NULL) break;
*   }
* 
*   p_fn_prtlin("  <span style=\"background-color:#fdfbe1\">EMOTIONAL </span> protective, sensitive, intuitive,                                        ");
* 
*   for (i=0; ; i++) {  /* print until [end_sensi] */
*     p_docin_get(doclin);
*     if (strstr(doclin, "[end_sensi]") != NULL) break;
*     scharout(doclin, '|');  /* remove pipes (for old sideline)    */
*     p_fn_prtlin_stars(doclin);  
*   }
*   p_fn_prtlin("  ");
* 
*   /* ================================================================= */
*   for (i=0; ; i++) {
*     p_docin_get(doclin);
*     if (strstr(doclin, "[beg_restl]") != NULL) break;
*   }
* 
*   p_fn_prtlin("  <span style=\"background-color:#fdfbe1\">RESTLESS </span> versatile, changeable, drawn to new things                                ");
* 
*   for (i=0; ; i++) {
*     p_docin_get(doclin);
*     if (strstr(doclin, "[end_restl]") != NULL) break;
*     scharout(doclin, '|');  /* remove pipes (for old sideline)    */
*     p_fn_prtlin_stars(doclin);  
*   }
*   p_fn_prtlin("  ");
* 
*   /* ================================================================= */
*   for (i=0; ; i++) {
*     p_docin_get(doclin);
*     if (strstr(doclin, "[beg_earth]") != NULL) break;
*   }
* 
*   p_fn_prtlin("  <span style=\"background-color:#fdfbe1\">DOWN-TO-EARTH </span> stable, practical, dependable                                        ");
* 
*   for (i=0; ; i++) { 
*     p_docin_get(doclin);
*     if (strstr(doclin, "[end_earth]") != NULL) break;
*     scharout(doclin, '|');  /* remove pipes (for old sideline)    */
*     p_fn_prtlin_stars(doclin);  
*   }
*   p_fn_prtlin("  ");
* 
*   /* ================================================================= */
*   for (i=0; ; i++) {
*     p_docin_get(doclin);
*     if (strstr(doclin, "[beg_sexdr]") != NULL) break;
*   }
* 
*   p_fn_prtlin("  <span style=\"background-color:#fdfbe1\">DRIVE </span> intense, passionate, relentless                                              ");
* 
*   for (i=0; ; i++) { 
*     p_docin_get(doclin);
*     if (strstr(doclin, "[end_sexdr]") != NULL) break;
*     scharout(doclin, '|');  /* remove pipes (for old sideline)    */
*     p_fn_prtlin_stars(doclin);  
*   }
*   p_fn_prtlin("  ");
* 
*   /* ================================================================= */
* /* me and my big mouth   removed jun2013
* */
* #ifdef PUT_BACK_COMMENTED_OUT_STUFF /****************************************/
* *   for (i=0; ; i++) {
* *     p_docin_get(doclin);
* *     if (strstr(doclin, "[beg_bgmth]") != NULL) break;
* *   }
* * 
* * /*   p_fn_prtlin(" ME AND MY BIG MOUTH                                                                             ");
* * */
* *   p_fn_prtlin("  <span style=\"background-color:#fdfbe1\">ME AND MY BIG MOUTH </span>  talky, chat-loving, forthcoming                               ");
* * 
* *   for (i=0; ; i++) {
* *     p_docin_get(doclin);
* *     if (strstr(doclin, "[end_bgmth]") != NULL) break;
* * /*     p_fn_prtlin(doclin); */
* *     scharout(doclin, '|');  /* remove pipes (for old sideline)    */
* *     p_fn_prtlin_stars(doclin);  
* *   }
* *   p_fn_prtlin("  ");
* * /*   p_fn_prtlin(" A high score shows a talky person who loves to chat and might talk first and think later.       ");
* * *   p_fn_prtlin("                                                                                                 ");
* * *   p_fn_prtlin("                                                                                                 ");
* * */
* #endif /* ifdef PUT_BACK_COMMENTED_OUT_STUFF ********************************/
* 
*   /* ================================================================= */
*   for (i=0; ; i++) {
*     p_docin_get(doclin);
*     if (strstr(doclin, "[beg_upndn]") != NULL) break;
*   }
* 
*   p_fn_prtlin("  <span style=\"background-color:#fdfbe1\">UPS AND DOWNS IN LIFE </span> having very high ups and very low downs                      ");
* 
*   for (i=0; ; i++) {
*     p_docin_get(doclin);
*     if (strstr(doclin, "[end_upndn]") != NULL) break;
*     scharout(doclin, '|');  /* remove pipes (for old sideline)    */
*     p_fn_prtlin_stars(doclin);  
*   }
*   p_fn_prtlin("  ");
*   p_fn_prtlin("  ");
* 
* 
*   /* read until
*   */
*   for (i=0; ; i++) {
*     p_docin_get(doclin);
*     if (strstr(doclin, "[end_graph]") != NULL) break;
*   }
* 
*   gbl_we_are_in_PRE_block_content = 0;  /* false */
*   p_fn_prtlin("</pre>\n");
* 
* } /* end of do_orig_trait_graph(void) */
#endif /* ifdef PUT_BACK_COMMENTED_OUT_STUFF ********************************/



/* ============================================================================ */
void do_benchmark_trait_graph(void) {
/* ============================================================================ */
  int i;
/* trn("in do_benchmark_trait_graph(void) "); */

  /* start the graph (now table  201309) 
  */

/*   p_fn_prtlin(" <div><br></div>"); */
/*   p_fn_prtlin("  <table class=\"trait\">"); */
/*   p_fn_prtlin("    <tr> <th>Trait</th> <th>Score</th> <th>Influence</th> </tr>"); */


  /* 1. add the 5 benchmark lines to array (they will sort in themselves)
  * 1b. add the 6 trait lines to array 
  *  2. sort array of structs   (by score,trait)
  *  3. print html
  */
  add_all_benchmark_lines();

  /*  read until [beg_agrsv]   */
  for (i=0; ; i++) {
    p_docin_get(doclin);
    if (strstr(doclin, "[beg_agrsv]") != NULL) break;
  }

  for (i=0; ; i++) {  /* print until [end_agrsv] */
    p_docin_get(doclin);
    if (strstr(doclin, "[end_agrsv]") != NULL) break;

    strcpy(trait_lines[5].trait, "Assertive");
    strcpy(trait_lines[5].score, doclin);
    strcpy(trait_lines[5].influence, "");
  }

  for (i=0; ; i++) {
    p_docin_get(doclin);
    if (strstr(doclin, "[beg_sensi]") != NULL) break;
  }

  for (i=0; ; i++) { 
    p_docin_get(doclin);
    if (strstr(doclin, "[end_sensi]") != NULL) break;

    strcpy(trait_lines[6].trait, "Emotional");
    strcpy(trait_lines[6].score, doclin);
    strcpy(trait_lines[6].influence, "");
  }

  for (i=0; ; i++) {
    p_docin_get(doclin);
    if (strstr(doclin, "[beg_restl]") != NULL) break;
  }

  for (i=0; ; i++) {
    p_docin_get(doclin);
    if (strstr(doclin, "[end_restl]") != NULL) break;

    strcpy(trait_lines[7].trait, "Restless");
    strcpy(trait_lines[7].score, doclin);
    strcpy(trait_lines[7].influence, "");
  }

  for (i=0; ; i++) {
    p_docin_get(doclin);
    if (strstr(doclin, "[beg_earth]") != NULL) break;
  }

  for (i=0; ; i++) { 
    p_docin_get(doclin);
    if (strstr(doclin, "[end_earth]") != NULL) break;

    strcpy(trait_lines[8].trait, "Down-to-earth");
    strcpy(trait_lines[8].score, doclin);
    strcpy(trait_lines[8].influence, "");
  }

  for (i=0; ; i++) {
    p_docin_get(doclin);
    if (strstr(doclin, "[beg_sexdr]") != NULL) break;
  }

  for (i=0; ; i++) { 
    p_docin_get(doclin);
    if (strstr(doclin, "[end_sexdr]") != NULL) break;

    strcpy(trait_lines[9].trait, "Passionate");
    strcpy(trait_lines[9].score, doclin);
    strcpy(trait_lines[9].influence, "");
  }

  for (i=0; ; i++) {
    p_docin_get(doclin);
    if (strstr(doclin, "[beg_upndn]") != NULL) break;
  }

  for (i=0; ; i++) {
    p_docin_get(doclin);
    if (strstr(doclin, "[end_upndn]") != NULL) break;

    strcpy(trait_lines[10].trait, "Ups and Downs");
    strcpy(trait_lines[10].score, doclin);
    strcpy(trait_lines[10].influence, "");
  }


  /* read until
  */
  for (i=0; ; i++) {
    p_docin_get(doclin);
    if (strstr(doclin, "[end_graph]") != NULL) break;
  }

  /* sort the trait_lines by score,trait
  */
  qsort(
    trait_lines,
    11,           /* number of elements */
    sizeof(struct trait_table_line),
    (compareFunc_trait)Func_compare_trait_line_scores
  );

  /* write the html for the table
  */
  write_html_for_trait_table();

} /* end of do_benchmark_trait_graph() */


/* for the sort of array of struct trait_table_line by score and trait
* 
* int Func_compare_trait_line_scores( const void *line1, const void *line2);
* typedef int (*compareFunc_trait) (const void *, const void *);
* 
*/
int Func_compare_trait_line_scores(
  struct trait_table_line *line1,
  struct trait_table_line *line2
)
{

/* trn(" in  Func_compare_trait_line_scores()"); */
  /* sorted high to low
  */

  /* sort is on 1. score   2. trait
  */
  if ( strcmp( line2->score , line1->score ) == 0) {
    return ( strcmp(line1->trait, line2->trait ) );
  } else {
    return ( atoi(line2->score) - atoi(line1->score));
  }
}

/* <table>
* <tr> <th>Trait</th> <th>Score</th> <th>Influence</th> </tr>
* <tr class="cGr2"><td></td><td>373 </td><td>Very High</td></tr>
* <tr><td>Emotional</td><td>477 </td><td></td></tr>
* <tr><td>Passionate</td><td>300 </td><td></td></tr>
* <tr class="cGre"><td></td><td>213 </td><td>High</td></tr>
* <tr><td>Down-to-earth</td><td>150 </td><td></td></tr>
* <tr class="cNeu"><td></td><td>100 </td><td>Median</td></tr>
* <tr class="cRed"><td></td><td>42 </td><td>Low</td></tr>
* <tr><td>Restless</td><td>47 </td><td></td></tr>
* <tr><td>Assertive</td><td>33 </td><td></td></tr>
* <tr class="cRe2"><td></td><td>18 </td><td>Very Low</td></tr>
* <tr><td>Ups and Downs</td><td>15 </td><td></td></tr>
* </table>
*/
void write_html_for_trait_table(void)
{
  int i, score_int;
  char rowcolor[32];

  p_fn_prtlin("<table class=\"trait\">");
/*   p_fn_prtlin("<tr> <th>Trait *</th> <th>Score</th> <th>Benchmark</th> </tr>"); */
  p_fn_prtlin("<tr> <th>Trait*</th> <th>Score</th> <th></th> </tr>");

  for (i=0; i <=10; i++) {

    if (strcmp(trait_lines[i].influence, "Very High") == 0) {
      sprintf(writebuf, "  <tr class=\"cGr2\"><td></td><td>90 </td><td>Very High</td></tr>");
      p_fn_prtlin(writebuf);
      continue;
    }
    if (strcmp(trait_lines[i].influence, "High") == 0) {
      sprintf(writebuf, "  <tr class=\"cGre\"><td></td><td>75 </td><td>High</td></tr>");
      p_fn_prtlin(writebuf);
      continue;
    }
/*     if (strcmp(trait_lines[i].influence, "Median") == 0) { */
    if (strcmp(trait_lines[i].influence, "Average") == 0) {
      sprintf(writebuf, "  <tr class=\"cNeu\"><td></td><td>50 </td><td>Average</td></tr>");
      p_fn_prtlin(writebuf);
      continue;
    }
    if (strcmp(trait_lines[i].influence, "Low") == 0) {
      sprintf(writebuf, "  <tr class=\"cRed\"><td></td><td>25 </td><td>Low</td></tr>");
      p_fn_prtlin(writebuf);
      continue;
    }
    if (strcmp(trait_lines[i].influence, "Very Low") == 0) {
      sprintf(writebuf, "  <tr class=\"cRe2\"><td></td><td>10 </td><td>Very Low</td></tr>");
      p_fn_prtlin(writebuf);
      continue;
    }
    

    score_int = atoi(trait_lines[i].score);

    /* put ROWCOLOR
    */
    if (score_int >= 90) strcpy(rowcolor, " class=\"cGr2\"");
    if (score_int <  90 &&
        score_int >= 75) strcpy(rowcolor, " class=\"cGre\"");
    if (score_int <  75 &&
        score_int >= 25) strcpy(rowcolor, " class=\"cNeu\"");
    if (score_int <= 25 &&
        score_int >  10) strcpy(rowcolor, " class=\"cRed\"");
    if (score_int <= 10) strcpy(rowcolor, " class=\"cRe2\"");


    sprintf(writebuf,  "  <tr %s><td>%s</td><td>%s </td><td></td></tr>",
      rowcolor,
      trait_lines[i].trait,
      trait_lines[i].score
    );
    p_fn_prtlin(writebuf);

  } /* for all 11 table data lines */


  p_fn_prtlin("</table>");

} /* end of write_html_for_trait_table(void) */


void add_all_benchmark_lines(void)
{
/* trn(" in void add_all_benchmark_lines(void)"); */
  strcpy(trait_lines[0].trait, "zzzzzzzzzzzz"); /* sort below ties with 373 */
  strcpy(trait_lines[0].score, "90");
  strcpy(trait_lines[0].influence, "Very High");

  strcpy(trait_lines[1].trait, "zzzzzzzzzzzz"); /* sort below ties with 213 */
  strcpy(trait_lines[1].score, "75");
  strcpy(trait_lines[1].influence, "High");

  strcpy(trait_lines[2].trait, "zzzzzzzzzzzz"); /* sort below ties with 100 */
  strcpy(trait_lines[2].score, "50");
  strcpy(trait_lines[2].influence, "Average");  /* not median */

  strcpy(trait_lines[3].trait, "            "); /* sort above ties with  42 */
  strcpy(trait_lines[3].score, "25");
  strcpy(trait_lines[3].influence, "Low");

  strcpy(trait_lines[4].trait, "            "); /* sort above ties with  18 */
  strcpy(trait_lines[4].score, "10");
  strcpy(trait_lines[4].influence, "Very Low");
}


/* end of perhtm.c */
