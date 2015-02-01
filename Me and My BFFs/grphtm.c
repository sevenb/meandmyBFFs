
/* grphtm.c */

/* For just_2 rpt, read from input docin_lines string array
* format and write an html output file
* For group rpts, input is 
*    struct rank_report_line  *in_rank_lines[],
*    int   in_rank_lines_last_idx )
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "rk.h"
#include "rkdebug_externs.h"
/* #include "incocoa.h" */





void put_ios_top_of_just2_group_rpt(void); /* just_2 rpt */
char gbl_person_A_first_pair[32];  // for "only html" return
char gbl_person_B_first_pair[32];  // for "only html" return




/* these are in rkdebug.o */
extern void fopen_fpdb_for_debug(void);
extern void fclose_fpdb_for_debug(void);


/* #define APP_NAME "Astrology by Measurement" */
/* #define APP_NAME "Me & My BFFs" */
#define APP_NAME "\"Me and my BFFs\""
/* #define APP_NAME "\"My BFFs and I\"" */
/* file extension for group sharing will be ".mamb" */


/* #define GBL_HTML_HAS_NEWLINES 1 */
#define GBL_HTML_HAS_NEWLINES 0


int gbl_we_are_in_PRE_block; /* 1 = yes, 0 = no */
int gbl_avg_score_this_member; /* for report bottom */
char gbl_gfnameHTML[256];




/* rank_report_line array declarations */  /* copied from incocoa.c */
/* trait_report_line array declarations */
#define MAX_SIZE_PERSON_NAME  15
struct trait_report_line {
  int  rank_in_group;
  int  score;
  char person_name[MAX_SIZE_PERSON_NAME+1];
/*  char hex_color[8]; */  /* like "66ff33" */
};
struct rank_report_line {
  int  rank_in_group;
  int  score;
  char person_A[MAX_SIZE_PERSON_NAME+1];
  char person_B[MAX_SIZE_PERSON_NAME+1];
/*  char hex_color[8]; */  /* like "66ff33" */
};
struct avg_report_line {
  int  rank_in_group;
  int  avg_score;
  char person_name[MAX_SIZE_PERSON_NAME+1];
/*  char hex_color[8]; */  /* like "66ff33" */
};
struct grp_personality {
  int  avg_score;
  char html_line[2048];
};
struct grp_personality arr_grp_personality[16];
struct grp_personality my_grp_personality;

/* int Func_compare_grp_personality_scores(const void *line1, const void *line2); */
int Func_compare_grp_personality_scores(
  struct grp_personality *score1,
  struct grp_personality *score2  );
typedef int (*compareFunc_grp_per) (const void *, const void *);


/* Define the array of ranking report line data.
*   (Rank  Score  person_a  person_b)
*
*   assuming MAX_PERSONS_IN_GROUP = 250, num pairs max is  31,125 
*   (5 sec to run on pc/gcc , 1 sec on mac/llvm )
*/
#define MAX_PERSONS_IN_GROUP 250   /* also defined incocoa.c and grpdoc.c */
#define MAX_IN_RANK_LINE_ARRAY \
( ( (MAX_PERSONS_IN_GROUP * (MAX_PERSONS_IN_GROUP - 1) / 2) ) + 64 )
struct rank_report_line *out_rank_lines[MAX_IN_RANK_LINE_ARRAY];
int out_rank_idx;  /* pts to current line in out_rank_lines */

/* end of rank_report_line array declarations */  /* copied from incocoa.c */





int out_rank_idx;  /* pts to current line in out_rank_lines */
char *mamb_csv_arr[2];  /* only 2 persons */

/* in grpdoc.c */
extern int mamb_report_whole_group(    /* called from cocoa */
  char *html_file_name,
  char *group_name,
  char *in_csv_person_arr[],  /* fmt= "Fred,3,21,1987,11,58,1,5,80.34" */
  int  num_persons_in_grp,
  struct rank_report_line *rank_lines[],
  int  *rank_idx,
  char *instructions,
  char *string_for_table_only  /* 1024 chars max (its 9 lines formatted) */
);
extern void g_rank_line_free(
  struct rank_report_line *out_rank_lines[],  /* output param returned */
  int rank_line_last_used_idx
);
/* in grpdoc.c */
extern int  mamb_report_person_in_group(  /* in grpdoc.o */ 
  char *html_file_name,
  char *group_name,
  char *in_csv_person_arr[],
  int  num_persons_in_grp,
  char *compare_everyone_with,   /* fmt= "Fred,3,21,1987,11,58,1,5,80.34" */
  struct rank_report_line *rank_lines[],   /* array of output report data */
  int  *rank_idx           /* ptr to int having last index written */
);
/* in grpdoc.c */


/* #include "grphtm.h" */
/* int rkdb = 0; */ /* 0=no, 1=yes */

void g_fn_prtlin(char *lin);
void g_fn_prtlin_stars(char *starline);

/* in mambutil.o */
extern int mapBenchmarkNumToPctlRank(int benchmark_num);
extern int sfind(char s[], char c);
extern char *scapwords(char *s);
extern void scharswitch(char *s, char ch_old, char ch_new);
extern char *csv_get_field(char *csv_string, char *delim, int want_fieldnum);
extern char *strim(char *s, char *set);
extern void commafy_int(char *dest, long intnum, int sizeofs);
extern int sall(char *s, char *set);
extern char *mkstr(char *s, char *begarg, char *end);
extern void scharout(char *s, int c);
extern void put_br_every_n(char *instr, int line_not_longer_than_this);
extern void fn_right_pad_with_hidden(char *s_to_pad, int num_to_pad);
extern void sfill(char *s, int num, int c);

/* in mambutil.o */

void do_average_trait_score_group(char *group_name, int average_trait_score);

int make_html_file_whole_group( /* produce actual html file */
  char *group_name,
  int   num_persons_in_grp,
  char *in_html_filename,           /* in grphtm.c */
  struct rank_report_line  *in_rank_lines[],
  int   in_rank_lines_last_idx,
  char *instructions,
  char *string_for_table_only   /* 1024 chars max (its 9 lines formatted) */
);



/* NOTE: this repor is NO LONGER USED    (Jul 2014)
*  so there is no new "one-string" avg_lines format
*  like rank_lines and trait_lines arrays
*/
#ifdef PUT_BACK_COMMENTED_OUT_STUFF /****************************************/
* int make_html_file_avg_scores( /* produce actual html file */
*   char *group_name,
*   int   num_persons_in_grp,
*   char *in_html_filename,           /* in grphtm.c */
*   struct avg_report_line  *in_avg_lines[],
*   int   in_avg_lines_last_idx
* );
#endif /* ifdef PUT_BACK_COMMENTED_OUT_STUFF ********************************/


int make_html_file_trait_rank( /* in grphtm.c */
  char *group_name,
  int   num_persons_in_grp,
  char *trait_name,
  char *in_html_filename,           /* in grphtm.c */
  struct trait_report_line  *in_trait_lines[],
  int   in_trait_lines_last_idx,
  char *grp_average_trait_scores_csv
);


extern void g_docin_free(void);   /* in grpdoc.o */

void g_docin_get(char *in_line);   

char doclin[4048];
char swork33[4048];



int    g_global_max_docin_idx;
char **g_global_docin_lines;
int    g_global_read_idx;
int    g_global_n;
char  *g_global_p = &swork33[0];

/* for calling mamb_report_whole_group() from make_html_just_2_people()
*  in order to get the table to show on bottom of just_2 HTML.
*/

char global_instructions[512];
char gbl_compare_everyone_with[512];
char gbl_format_as[512];
char gbl_trait_name[512];
char gblGrpAvgTraitScoresCSV2[64];

FILE *Fp_g_HTML_file;

char s1[512];
char s2[512];
char s3[512];
char s4[512];
char s5[512];
char s6[512];
/* char writebuf[2024]; */
/* char writebuf2[2024]; */
/* char writebuf3[2024]; */
char writebuf[4048];
char writebuf2[4048];
char writebuf3[4048];
/* char workbuf[512]; */


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
*  With the *define* after, you can say this: 
*  arr(k) for the same buffer.
*/


/* void g_fn_output_top_of_html_file(void); */
void put_top_of_just2_group_rpt(void); /* just_2 rpt */  // browser version

void put_top_of_html_group_rpt(char *group_name);
void put_category_label(char *category_text, int len);

int is_first_g_docin_get;  /* 1=yes, 0=no */


/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
/* @@@@@@@@@@@@@@@@  person_in_group  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
int make_html_file_person_in_group( /* produce actual html file */
  char *group_name,
  int   num_persons_in_grp,
  char *html_file_name,                    /* in grphtm.c */
  struct rank_report_line  *in_rank_lines[],  /* array of report data */
  int   in_rank_lines_last_idx,   /* int having last index written */
  int avg_score_this_member)      /* for report bottom */
{
  strcpy(gbl_gfnameHTML, html_file_name);

  gbl_avg_score_this_member = avg_score_this_member; /* for report bottom */
  gbl_we_are_in_PRE_block = 0;  /* init to false */

  /* try just calling whole_group html creation function
  * with "format as person_in_group"  instructions
  * in make_html_file_whole_group()
  */
  char string_for_table_only[2048];

/* trn("instructions for make_html_file_whole_group() = format as person_in_group"); */

  strcpy(string_for_table_only, "abc");
  int retval;
  retval = make_html_file_whole_group( /* produce actual html file */
    group_name,
    num_persons_in_grp,
    html_file_name,                     /* in grphtm.c */
    in_rank_lines,       /* array of report data */
    in_rank_lines_last_idx,   /* int having last index written */
    "format as person_in_group",  /* maybe instructions for table-only */
    string_for_table_only /* 1024 chars max (its 9 lines formatted) */
                          /* buf to hold html for table */
  );

  if (retval != 0) {
    g_docin_free();      /* free all allocated array elements */
    rkabort("Error: html file (grphtm, one person) was not produced");
    return(1);
  }

  return(0);
} /* end of make_html_file_person_in_group() */



/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
/* %%%%%%%%%%%%%%%%%%%%%%%  just_2 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

int make_html_file_just_2_people(      /* old main() */
  char *in_html_filename,  // browser html or webivew html
  char *in_docin_lines[],
  int   in_docin_last_idx,
  char *person_1_csv,        /* fmt= "Fred,3,21,1987,11,58,1,5,80.34" */
  char *person_2_csv         /* used for whole grp table at bot of report */
)
{
/*  char string_for_table_only[1024], mybuf[512], category_text[128]; */
 char string_for_table_only[2048], category_text[128];
 int mylen;

  strcpy(gbl_gfnameHTML, in_html_filename);
  is_first_g_docin_get = 1;  /* set to true */
  gbl_we_are_in_PRE_block = 0;  /* init to false */

  int i;
/* trn("in  make_html_file_just_2_people() "); */

  g_global_max_docin_idx = in_docin_last_idx;
  g_global_docin_lines   = in_docin_lines;

  /* open output HTML file
  */
  if ( (Fp_g_HTML_file = fopen(in_html_filename, "w")) == NULL ) {
    rkabort("Error  on   grphtm.c.  fopen().");
  }
  /* in this fn is the first g_docin_get for just_2 rpt
  */
  if (strstr(in_html_filename, "webview") != NULL) {
    put_ios_top_of_just2_group_rpt(); /* output the css, headings etc. */
  } else {
    put_top_of_just2_group_rpt(); /* output the css, headings etc. */
  }


  /* TABLE with "Match Score"
  */

  /* Here, get into a string (1024 chars) the table-only html
  *  from the whole_group report (with colors).
  *  The group has only the two members. The table:
  *
  *        Rank  Score  Pair of Group Members
  *                373  Great
  *                213  Good
  *           1    152  barry   johnH
  *                100  Median
  *                 42  Not so good
  *                 18  OMG
  * 
  */
  /* ------------------------------------------- */


  mamb_csv_arr[0] = person_1_csv;   /* fmt= "Fred,3,21,1987,11,58,1,5,80.34" */
  mamb_csv_arr[1] = person_2_csv;   /* fmt= "Fred,3,21,1987,11,58,1,5,80.34" */


// tn();trn(" before report wholefor string"); nksn(gbl_gfnameHTML);
// trn(" before report wholefor string"); nksn(in_html_filename);

  sfill(string_for_table_only, 2000, ' ');
  out_rank_idx = 0;

  mamb_report_whole_group(    /* called from cocoa or just_2_people() in grphtm.c */
//    "",              /* *html_file_name,*/
    in_html_filename,              /* *html_file_name,*/  // HAS TO be there to act as flag for /webview/browser
    "mambTempGroup", /* *group_name,*/
    mamb_csv_arr,    /* fmt= "Fred,3,21,1987,11,58,1,5,80.34" */
    2,               /* num_persons_in_grp,*/
    out_rank_lines,  /* rank_report_line *out_rank_lines[],   output params returned (UNUSED HERE */
                     /* (the array contents are not used here) */
    &out_rank_idx,    /* UNUSED HERE */
    "return only html for table in string",  /* instructions to return string only */
/* <.> */
    string_for_table_only               /* 1024 chars max (its 9 lines formatted) */
  );

// tn();trn(" after report wholefor string"); nksn(gbl_gfnameHTML);
// trn(" after report wholefor string"); nksn(in_html_filename);

fopen_fpdb_for_debug(); /* for test  */


  /* when finished, free array elements 
  */
  g_rank_line_free(out_rank_lines, out_rank_idx);

  strcpy(global_instructions, "ok to write html now"); 


/* ksn(string_for_table_only); */
  g_fn_prtlin(string_for_table_only); /* OUTPUT THE HTML FOR THE TABLE ========  */

  /* END of   TABLE with "Match Score" */


  if (strstr(in_html_filename, "webview") != NULL) {  // webview version

    //g_fn_prtlin("<pre class=\"checkoutbestmatch\">");
    g_fn_prtlin("<div class=\"checkoutbestmatch\">");
  //  g_fn_prtlin("<pre style=\"margin-left: 10em;\">");
    gbl_we_are_in_PRE_block = 1;  /* true */

   // g_fn_prtlin("<span style=\"background-color: #fcfce0;\">                                            ");
    g_fn_prtlin("<span style=\"background-color: #fcfce0;\">                                            </span>");

    //g_fn_prtlin("  Check out the Best Match in Group report  ");
    g_fn_prtlin("<span style=\"background-color: #fcfce0;\">  Check out the Best Match in Group report  </span>");

    //g_fn_prtlin("   which uses this score to compare with    ");
    g_fn_prtlin("<span style=\"background-color: #fcfce0;\">   which uses this score to compare with    </span>");

    //g_fn_prtlin("      other pairs of group members          ");
    g_fn_prtlin("<span style=\"background-color: #fcfce0;\">      other pairs of group members          </span>");

    //g_fn_prtlin("                                            ");
    g_fn_prtlin("<span style=\"background-color: #fcfce0;\">                                            </span>");

  /*   g_fn_prtlin("                                                                     "); 
  *   g_fn_prtlin("  Check out the group reports \"Best Match\" and \"Best Match For ...\"  ");
  *   g_fn_prtlin("  which use this score to compare with other pairs of group members  ");
  *   g_fn_prtlin("                                                                     ");
  */
    gbl_we_are_in_PRE_block = 0;  /* false */

    g_fn_prtlin("</div>");   // end of checkoutbestmatch

  } else {  // browser version


    g_fn_prtlin("<pre>");
    gbl_we_are_in_PRE_block = 1;  /* true */
    g_fn_prtlin("                                            ");

  /*   g_fn_prtlin("  Check out the Best Match report    "); */
    g_fn_prtlin("  Check out the Best Match in Group report  ");
    g_fn_prtlin("   which uses this score to compare with    ");
    g_fn_prtlin("      other pairs of group members          ");

    g_fn_prtlin("                                            ");
  /*   g_fn_prtlin("                                                                     "); 
  *   g_fn_prtlin("  Check out the group reports \"Best Match\" and \"Best Match For ...\"  ");
  *   g_fn_prtlin("  which use this score to compare with other pairs of group members  ");
  *   g_fn_prtlin("                                                                     ");
  */
    gbl_we_are_in_PRE_block = 0;  /* false */
    g_fn_prtlin("<br><br><br></pre>");

  } // browser version


  /*  read until [beg_graph]
   */
  for (i=0; ; i++) {
    g_docin_get(doclin);
    if (strstr(doclin, "[beg_graph]") != NULL) break;
  }

  char spanbeg[64], spanend[64];  // used in web only

  if (strstr(in_html_filename, "webview") != NULL) {  // webview version

    gbl_we_are_in_PRE_block = 1;  /* true */

  /*   g_fn_prtlin("<span style=\"font-weight:bold;\">"); */
  /*   g_fn_prtlin("                                                "); */
    g_fn_prtlin( " <div class=\"explproportion\">");

  /* <.> */
  //  g_fn_prtlin("                                                                                  ");
  //  g_fn_prtlin("                           These show the proportion of                           ");
  //  g_fn_prtlin("                        <span class=\"cGre\">good aspects + </span> and <span class=\"cRed\">bad aspects - </span>                        ");
  //  g_fn_prtlin("                                for each category                                 ");
  //  g_fn_prtlin("                                                                                  <span>");

  //  g_fn_prtlin("                                         ");
  //  g_fn_prtlin("       These show the proportion of      ");
  //  g_fn_prtlin("    <span class=\"cGre\">good aspects + </span> and <span class=\"cRed\">bad aspects - </span>   ");
  //
  //  g_fn_prtlin("            for each category            ");
  //  g_fn_prtlin("                                         <span>");

    strcpy(spanbeg, "<span style=\"background-color: #fcfce0;\"> ");
    strcpy(spanend, "</span>");

  //    "                                                                          ",

    sprintf(writebuf, "%s%s%s", spanbeg,
      "                                                             ",
    spanend);
    g_fn_prtlin(writebuf);

    sprintf(writebuf, "%s%s%s", spanbeg,
      "   For each category below you can see the proportion of     ",
    spanend);
    g_fn_prtlin(writebuf);

    sprintf(writebuf, "%s%s%s", spanbeg,
      "        <span class=\"cGre\"> good aspects  +  </span> and <span class=\"cRed\"> bad aspects  -  </span>             ",
      spanend);
    g_fn_prtlin(writebuf);

    sprintf(writebuf, "%s%s%s", spanbeg,
      "                                                             ",
    spanend);
    g_fn_prtlin(writebuf);

  /* <.> */
  g_fn_prtlin("</div><div class=\"categories\">");  // end of explproportion


  /*   sprintf(mybuf, "%-82s","                     Low              Average                            High     "); */
  /*   g_fn_prtlin(mybuf); */
  /*   sprintf(mybuf, "%-82s","                      |                  |                                |       "); */
  /*   g_fn_prtlin(mybuf); */

  //  g_fn_prtlin("                                                                                  "); /* blanks */

  //g_fn_prtlin("                       Low              Average                            High   ");
  //g_fn_prtlin("                        |                  |                                |     ");

    g_fn_prtlin("                       Low                  Average                 High          ");
    g_fn_prtlin("                        |                      |                     |            ");



  /*   sprintf(mybuf, "%-92s", "             <span class=\"cCat\">CLOSENESS </span>"); */
  /*   sprintf(mybuf, "%-92s", "             <span class=\"cCat\">CLOSENESS </span>                          "); */

    /* put category string in field of 92 with 13 spaces at line beg
    *  (not counting <span> characters)
    */
    sprintf(category_text, "%s", "CLOSENESS ");
  /*   put_category_label(category_text);  */
    put_category_label(category_text, (int)strlen(category_text)); 

  /*   g_fn_prtlin("  <span class="cCat">CLOSENESS </span>                                                                      "); */

    /* ================================================================= */
    /*  read until [beg_persn]   */
    for (i=0; ; i++) {
      g_docin_get(doclin);
      if (strstr(doclin, "[beg_persn]") != NULL) break;
    }
    for (i=0; ; i++) {   /* print star lines */
      g_docin_get(doclin);
      if (strstr(doclin, "[end_persn]") != NULL) break;

  /*     g_fn_prtlin(doclin); */
      scharout(doclin, '|');  /* remove pipes (for old sideline)    */
  /* tn();b(21);ks(doclin); */
      g_fn_prtlin_stars(doclin);  
    }
    /* finished personal stars */


  /*   g_fn_prtlin(" Shows the completely natural ease of liking the other person in a comfortable way.              "); */
  /*   sprintf(mybuf, "%-92s", "             Shows the completely natural ease of liking the other person in a comfortable way."); */

    /* out aug2013
    */
  /*   sprintf(mybuf, "%-92s", "             Shows the natural ease of liking the other person in a comfortable way."); */
  /*   g_fn_prtlin(mybuf); */

  /*   g_fn_prtlin(" "); */

  //  g_fn_prtlin("                                                                                  "); /* blanks */
    g_fn_prtlin("                                                                                  "); /* blanks */
  /*   g_fn_prtlin("<br>"); */

    /* ================================================================= */

    /* ================================================================= */

    for (i=0; ; i++) {
      g_docin_get(doclin);
      if (strstr(doclin, "[beg_aview]") != NULL) break;
    }

  /*   sprintf(writebuf, "<span class=\"cCat\">FROM %s's POINT OF VIEW </span>                                                           ",arr(0));
  *   sprintf(mybuf, "%-92s", writebuf);
  *   g_fn_prtlin(mybuf);
  */
  /*   g_fn_prtlin(writebuf); */


  /*   g_fn_prtlin("<pre>"); */
  /*   gbl_we_are_in_PRE_block = 1;  */


  //  g_fn_prtlin("                                                                                  "); /* blanks */
  /*   sprintf(category_text, "FROM %s's POINT OF VIEW ", arr(0) ); */
  /*   sprintf(category_text, */
  /*     "FROM <span class=\"cNam2\">%s</span>'s POINT OF VIEW ", arr(0) ); */

    mylen = sprintf(category_text, "FROM %s's POINT OF VIEW ", arr(0) );
    sprintf(category_text,
      "FROM %s's POINT OF VIEW ", arr(0) );
//      "FROM <span class=\"cNam2\">%s</span>'s POINT OF VIEW ", arr(0) );

    put_category_label(category_text, mylen); 


    for (i=0; ; i++) {  /* print until [end_sensi] */
      g_docin_get(doclin);
      if (strstr(doclin, "[end_aview]") != NULL) break;
      scharout(doclin, '|');  /* remove pipes (for old sideline)    */
  /* tn();b(24);ks(doclin); */
      g_fn_prtlin_stars(doclin);  
    }
  /*   g_fn_prtlin(" "); */
    g_fn_prtlin("                                                                                  "); /* blanks */
  /*   g_fn_prtlin(""); */


    /* ================================================================= */

    for (i=0; ; i++) {
      g_docin_get(doclin);
      if (strstr(doclin, "[beg_bview]") != NULL) break;
    }

  /*   sprintf(writebuf, "<span class=\"cCat\">FROM %s's POINT OF VIEW </span>",arr(1));
  *   sprintf(mybuf, "%-92s", writebuf);
  *   g_fn_prtlin(mybuf);
  */


  //  g_fn_prtlin("<pre>");


    gbl_we_are_in_PRE_block = 1; 

  //  g_fn_prtlin("                                                                                  "); /* blanks */

  /*   sprintf(category_text, "FROM %s's POINT OF VIEW ", arr(1) ); */
    mylen = sprintf(category_text, "FROM %s's POINT OF VIEW ", arr(1) );
    sprintf(category_text,
      "FROM %s's POINT OF VIEW ", arr(1) );
//      "FROM <span class=\"cNam2\">%s</span>'s POINT OF VIEW ", arr(1) );
    put_category_label(category_text, mylen); 


    for (i=0; ; i++) { 
      g_docin_get(doclin);
      if (strstr(doclin, "[end_bview]") != NULL) break;
      scharout(doclin, '|');  /* remove pipes (for old sideline)    */
      g_fn_prtlin_stars(doclin);  
    }
    g_fn_prtlin("                                                                                  "); /* blanks */

    /* ================================================================= */

    for (i=0; ; i++) {
      g_docin_get(doclin);
      if (strstr(doclin, "[beg_love]") != NULL) break;
    }
  /*   sprintf(mybuf, "%-92s",  "<span class=\"cCat\">LOVE </span>");
  *   g_fn_prtlin(mybuf);
  */

  /*   g_fn_prtlin("<pre>"); */
  /*   gbl_we_are_in_PRE_block = 1;  */

  //  g_fn_prtlin("                                                                                  "); /* blanks */
    sprintf(category_text, "LOVE ");
  /*   put_category_label(category_text);  */
    put_category_label(category_text, (int)strlen(category_text)); 

    for (i=0; ; i++) {
      g_docin_get(doclin);
      if (strstr(doclin, "[end_love]") != NULL) break;
      scharout(doclin, '|');  /* remove pipes (for old sideline)    */
      g_fn_prtlin_stars(doclin);  
    }
  /*   g_fn_prtlin(" "); */
    g_fn_prtlin("                                                                                  "); /* blanks */


    /* ================================================================= */
  /*   g_fn_prtlin("<pre>"); */
  /*   gbl_we_are_in_PRE_block = 1;  */

    for (i=0; ; i++) {
      g_docin_get(doclin);
      if (strstr(doclin, "[beg_money]") != NULL) break;
    }
  /*   g_fn_prtlin(" MONEY AND BUSINESS                                                                              "); */
  /*   sprintf(mybuf, "%-92s", "<span class=\"cCat\">MONEY AND BUSINESS </span>");
  *   g_fn_prtlin(mybuf);
  */

  /*   sprintf(category_text, "MONEY AND BUSINESS "); */
  //  g_fn_prtlin("                                                                                  "); /* blanks */
    sprintf(category_text, "MONEY ");
  /*   put_category_label(category_text);  */
    put_category_label(category_text, (int)strlen(category_text)); 

    for (i=0; ; i++) { 
      g_docin_get(doclin);
      if (strstr(doclin, "[end_money]") != NULL) break;
      scharout(doclin, '|');  /* remove pipes (for old sideline)    */
      g_fn_prtlin_stars(doclin);  
    }
    g_fn_prtlin("                                                                                  "); /* blanks */



    /* ================================================================= */
  /*   g_fn_prtlin("<pre>"); */
  /*   gbl_we_are_in_PRE_block = 1;  */


    for (i=0; ; i++) {
      g_docin_get(doclin);
      if (strstr(doclin, "[beg_ovral]") != NULL) break;
    }
  /*   sprintf(mybuf, "%-92s", "<span class=\"cCat\">OVERALL COMPATIBILITY </span>");
  *   g_fn_prtlin(mybuf);
  */
  //  g_fn_prtlin("                                                                                  "); /* blanks */



//    sprintf(category_text, "OVERALL COMPATIBILITY ");
//    put_category_label(category_text, (int)strlen(category_text)); 

    for (i=0; ; i++) {
      g_docin_get(doclin);
      if (strstr(doclin, "[end_ovral]") != NULL) break;
  /*     g_fn_prtlin(doclin); */
      scharout(doclin, '|');  /* remove pipes (for old sideline)    */

//      g_fn_prtlin_stars(doclin);  

    }
  /*   g_fn_prtlin(" A combination of all the different factors of compatibility for this pair.                      "); */

  //  g_fn_prtlin("                                                                                  "); /* blanks */

    /* ================================================================= */


    /* read until
    */
    for (i=0; ; i++) {
      g_docin_get(doclin);
      if (strstr(doclin, "[end_graph]") != NULL) break;
    }

  /*   g_fn_prtlin("   -  CLOSENESS is the most important category.                           "); */

  /* <.> */
    g_fn_prtlin( "</div><div class=\"explpotential\">");  // end of categories

    //char spanbeg[64], spanend[64];
    strcpy(spanbeg, "<span style=\"background-color: #fcfce0;\"> ");
    strcpy(spanend, "</span>");


  //  g_fn_prtlin("                                                                ");
  //  g_fn_prtlin("    For good compatibility potential                            "); 
  //  g_fn_prtlin(" -  you want to have a \"High\" number of pluses.                 ");
  //  g_fn_prtlin(" -  you also want to see double the pluses compared to minuses. ");
  //  g_fn_prtlin("                                                                ");


    sprintf(writebuf, "%s%s%s", spanbeg,
      "                                                               ", spanend);
    g_fn_prtlin(writebuf);

    sprintf(writebuf, "%s%s%s", spanbeg,
      "  For good compatibility potential                             ", spanend);
    g_fn_prtlin(writebuf);

    sprintf(writebuf, "%s%s%s", spanbeg,
      "  you want to have a \"High\" number of pluses and               ", spanend);
    g_fn_prtlin(writebuf);

    sprintf(writebuf, "%s%s%s", spanbeg,
      "  you also want to see double the pluses compared to minuses.  ", spanend);
//      "  -  you also would like to see double the pluses compared to minuses.  ", spanend);
    g_fn_prtlin(writebuf);

    sprintf(writebuf, "%s%s%s", spanbeg,
      "                                                               ", spanend);
    g_fn_prtlin(writebuf);


  //g_fn_prtlin("</div></div>");  // end of explpotential
  g_fn_prtlin("</div>");  // end of explpotential



    for (i=0; ; i++) {  /* read until  */
      g_docin_get(doclin);
      if (strstr(doclin, "[end_program]") != NULL) break;
    }

    gbl_we_are_in_PRE_block = 0;


  } else {  // browser version


    g_fn_prtlin("<pre>");
    gbl_we_are_in_PRE_block = 1;  /* true */

  /*   g_fn_prtlin("<span style=\"font-weight:bold;\">"); */
  /*   g_fn_prtlin("                                                "); */
    g_fn_prtlin("                                                                                  ");
    g_fn_prtlin("             For each category below you can see the proportion of                ");
    g_fn_prtlin("                     <span class=\"cGre\"> good aspects  +  </span> and <span class=\"cRed\"> bad aspects  -  </span>                     ");
    g_fn_prtlin("                                                                                  ");

  /*   gbl_we_are_in_PRE_block = 0;   */
  /*   g_fn_prtlin("</pre>"); */


  /*   sprintf(mybuf, "%-92s", " "); */
  /*   g_fn_prtlin(mybuf); */
  /*   g_fn_prtlin(" "); */


  /*   sprintf(mybuf, "%-92s",  "                    less important                   important                  remarkable");
  *   g_fn_prtlin(mybuf);
  * 
  *   sprintf(mybuf, "%-92s", "                         |                           |                          |");
  *   g_fn_prtlin(mybuf);
  */

  /* 
  *   sprintf(mybuf, "%-82s",  "          less important                   important                  remarkable");
  *   g_fn_prtlin(mybuf);
  * 
  *   sprintf(mybuf, "%-82s", "                |                           |                          |");
  *   g_fn_prtlin(mybuf);
  */


  /*   g_fn_prtlin("<pre>"); */
  /*   gbl_we_are_in_PRE_block = 1;  */

  /*   sprintf(mybuf, "%-82s","                     Low              Average                            High     "); */
  /*   g_fn_prtlin(mybuf); */
  /*   sprintf(mybuf, "%-82s","                      |                  |                                |       "); */
  /*   g_fn_prtlin(mybuf); */

//    g_fn_prtlin("                                                                                  "); /* blanks */
//    g_fn_prtlin("                       Low              Average                            High   ");
//    g_fn_prtlin("                        |                  |                                |     ");

    g_fn_prtlin("                       Low                  Average                 High          ");
    g_fn_prtlin("                        |                      |                     |            ");





  /*   sprintf(mybuf, "%-92s", "             <span class=\"cCat\">CLOSENESS </span>"); */
  /*   sprintf(mybuf, "%-92s", "             <span class=\"cCat\">CLOSENESS </span>                          "); */

    /* put category string in field of 92 with 13 spaces at line beg
    *  (not counting <span> characters)
    */
    sprintf(category_text, "%s", "CLOSENESS ");
  /*   put_category_label(category_text);  */
    put_category_label(category_text, (int)strlen(category_text)); 

  /*   g_fn_prtlin("  <span class="cCat">CLOSENESS </span>                                                                      "); */

    /* ================================================================= */
    /*  read until [beg_persn]   */
    for (i=0; ; i++) {
      g_docin_get(doclin);
      if (strstr(doclin, "[beg_persn]") != NULL) break;
    }
    for (i=0; ; i++) {   /* print star lines */
      g_docin_get(doclin);
      if (strstr(doclin, "[end_persn]") != NULL) break;

  /*     g_fn_prtlin(doclin); */
      scharout(doclin, '|');  /* remove pipes (for old sideline)    */
  /* tn();b(21);ks(doclin); */
      g_fn_prtlin_stars(doclin);  
    }
    /* finished personal stars */


  /*   g_fn_prtlin(" Shows the completely natural ease of liking the other person in a comfortable way.              "); */
  /*   sprintf(mybuf, "%-92s", "             Shows the completely natural ease of liking the other person in a comfortable way."); */

    /* out aug2013
    */
  /*   sprintf(mybuf, "%-92s", "             Shows the natural ease of liking the other person in a comfortable way."); */
  /*   g_fn_prtlin(mybuf); */

  /*   g_fn_prtlin(" "); */

    g_fn_prtlin("                                                                                  "); /* blanks */
//    g_fn_prtlin("                                                                                  "); /* blanks */
  /*   g_fn_prtlin("<br>"); */

    /* ================================================================= */

    /* ================================================================= */
  /*   gbl_we_are_in_PRE_block = 0;  */
  /*   g_fn_prtlin("</pre>"); */

    for (i=0; ; i++) {
      g_docin_get(doclin);
      if (strstr(doclin, "[beg_aview]") != NULL) break;
    }

  /*   sprintf(writebuf, "<span class=\"cCat\">FROM %s's POINT OF VIEW </span>                                                           ",arr(0));
  *   sprintf(mybuf, "%-92s", writebuf);
  *   g_fn_prtlin(mybuf);
  */
  /*   g_fn_prtlin(writebuf); */


  /*   g_fn_prtlin("<pre>"); */
  /*   gbl_we_are_in_PRE_block = 1;  */


//    g_fn_prtlin("                                                                                  "); /* blanks */
  /*   sprintf(category_text, "FROM %s's POINT OF VIEW ", arr(0) ); */
  /*   sprintf(category_text, */
  /*     "FROM <span class=\"cNam2\">%s</span>'s POINT OF VIEW ", arr(0) ); */

    mylen = sprintf(category_text, "FROM %s's POINT OF VIEW ", arr(0) );
    sprintf(category_text,
      "FROM %s's POINT OF VIEW ", arr(0) );
//      "FROM <span class=\"cNam2\">%s</span>'s POINT OF VIEW ", arr(0) );

    put_category_label(category_text, mylen); 


    for (i=0; ; i++) {  /* print until [end_sensi] */
      g_docin_get(doclin);
      if (strstr(doclin, "[end_aview]") != NULL) break;
      scharout(doclin, '|');  /* remove pipes (for old sideline)    */
  /* tn();b(24);ks(doclin); */
      g_fn_prtlin_stars(doclin);  
    }
  /*   g_fn_prtlin(" "); */
    g_fn_prtlin("                                                                                  "); /* blanks */
  /*   g_fn_prtlin(""); */

    gbl_we_are_in_PRE_block = 0; 
    g_fn_prtlin("</pre>");

    /* ================================================================= */

    for (i=0; ; i++) {
      g_docin_get(doclin);
      if (strstr(doclin, "[beg_bview]") != NULL) break;
    }

  /*   sprintf(writebuf, "<span class=\"cCat\">FROM %s's POINT OF VIEW </span>",arr(1));
  *   sprintf(mybuf, "%-92s", writebuf);
  *   g_fn_prtlin(mybuf);
  */

    g_fn_prtlin("<pre>");
    gbl_we_are_in_PRE_block = 1; 

//    g_fn_prtlin("                                                                                  "); /* blanks */

  /*   sprintf(category_text, "FROM %s's POINT OF VIEW ", arr(1) ); */
    mylen = sprintf(category_text, "FROM %s's POINT OF VIEW ", arr(1) );
    sprintf(category_text,
      "FROM %s's POINT OF VIEW ", arr(1) );
//      "FROM <span class=\"cNam2\">%s</span>'s POINT OF VIEW ", arr(1) );
    put_category_label(category_text, mylen); 


    for (i=0; ; i++) { 
      g_docin_get(doclin);
      if (strstr(doclin, "[end_bview]") != NULL) break;
      scharout(doclin, '|');  /* remove pipes (for old sideline)    */
      g_fn_prtlin_stars(doclin);  
    }
    g_fn_prtlin("                                                                                  "); /* blanks */

  /*   gbl_we_are_in_PRE_block = 0;  */
  /*   g_fn_prtlin("</pre>"); */
    /* ================================================================= */

    for (i=0; ; i++) {
      g_docin_get(doclin);
      if (strstr(doclin, "[beg_love]") != NULL) break;
    }
  /*   sprintf(mybuf, "%-92s",  "<span class=\"cCat\">LOVE </span>");
  *   g_fn_prtlin(mybuf);
  */

  /*   g_fn_prtlin("<pre>"); */
  /*   gbl_we_are_in_PRE_block = 1;  */

//    g_fn_prtlin("                                                                                  "); /* blanks */
    sprintf(category_text, "LOVE ");
  /*   put_category_label(category_text);  */
    put_category_label(category_text, (int)strlen(category_text)); 

    for (i=0; ; i++) {
      g_docin_get(doclin);
      if (strstr(doclin, "[end_love]") != NULL) break;
      scharout(doclin, '|');  /* remove pipes (for old sideline)    */
      g_fn_prtlin_stars(doclin);  
    }
  /*   g_fn_prtlin(" "); */
    g_fn_prtlin("                                                                                  "); /* blanks */

  /*   gbl_we_are_in_PRE_block = 0;  */
  /*   g_fn_prtlin("</pre>"); */

    /* ================================================================= */
  /*   g_fn_prtlin("<pre>"); */
  /*   gbl_we_are_in_PRE_block = 1;  */

    for (i=0; ; i++) {
      g_docin_get(doclin);
      if (strstr(doclin, "[beg_money]") != NULL) break;
    }
  /*   g_fn_prtlin(" MONEY AND BUSINESS                                                                              "); */
  /*   sprintf(mybuf, "%-92s", "<span class=\"cCat\">MONEY AND BUSINESS </span>");
  *   g_fn_prtlin(mybuf);
  */

  /*   sprintf(category_text, "MONEY AND BUSINESS "); */
//    g_fn_prtlin("                                                                                  "); /* blanks */
    sprintf(category_text, "MONEY ");
  /*   put_category_label(category_text);  */
    put_category_label(category_text, (int)strlen(category_text)); 

    for (i=0; ; i++) { 
      g_docin_get(doclin);
      if (strstr(doclin, "[end_money]") != NULL) break;
      scharout(doclin, '|');  /* remove pipes (for old sideline)    */
      g_fn_prtlin_stars(doclin);  
    }
    g_fn_prtlin("                                                                                  "); /* blanks */

  /*   gbl_we_are_in_PRE_block = 0;  */
  /*   g_fn_prtlin("</pre>"); */


    /* ================================================================= */
  /*   g_fn_prtlin("<pre>"); */
  /*   gbl_we_are_in_PRE_block = 1;  */


    for (i=0; ; i++) {
      g_docin_get(doclin);
      if (strstr(doclin, "[beg_ovral]") != NULL) break;
    }
  /*   sprintf(mybuf, "%-92s", "<span class=\"cCat\">OVERALL COMPATIBILITY </span>");
  *   g_fn_prtlin(mybuf);
  */
//    g_fn_prtlin("                                                                                  "); /* blanks */
//    sprintf(category_text, "OVERALL COMPATIBILITY ");
//    put_category_label(category_text, (int)strlen(category_text)); 

    for (i=0; ; i++) {
      g_docin_get(doclin);
      if (strstr(doclin, "[end_ovral]") != NULL) break;
  /*     g_fn_prtlin(doclin); */
//      scharout(doclin, '|');  /* remove pipes (for old sideline)    */
//      g_fn_prtlin_stars(doclin);  
    }
  /*   g_fn_prtlin(" A combination of all the different factors of compatibility for this pair.                      "); */

  /* out aug2013
  */
  /*   sprintf(mybuf, "%-92s", "             A combination of all the different factors of compatibility for this pair."); */
  /*   g_fn_prtlin(mybuf); */

  /*   g_fn_prtlin(" "); */
//    g_fn_prtlin("                                                                                  "); /* blanks */

  /*   gbl_we_are_in_PRE_block = 0;  */
  /*   g_fn_prtlin("</pre>"); */
    /* ================================================================= */


    /* read until
    */
    for (i=0; ; i++) {
      g_docin_get(doclin);
      if (strstr(doclin, "[end_graph]") != NULL) break;
    }

  /*   g_fn_prtlin("<div>CLOSENESS is by far the most important category because it shows<br> the natural ease of liking the other person in a comfortable way.</div>"); */
  /*   g_fn_prtlin("<div>CLOSENESS is by far the most important category because it shows<br> the natural ease of liking the other person in a comfortable way.</div>"); */

    g_fn_prtlin("                                                                                  "); /* blanks */
    g_fn_prtlin("      For good compatibility potential                                            ");
    g_fn_prtlin("      you want to have a \"High\" number of pluses.                                 ");
    g_fn_prtlin("      you also would like to see double the pluses compared to minuses.           ");
    g_fn_prtlin("                                                                                  ");

  /*   gbl_we_are_in_PRE_block = 0; */
  /*   g_fn_prtlin("</pre>"); */

    for (i=0; ; i++) {  /* read until  */
      g_docin_get(doclin);
      if (strstr(doclin, "[end_program]") != NULL) break;
    }

    gbl_we_are_in_PRE_block = 0;
    g_fn_prtlin("</pre>");


  } // browser version




  if (strstr(in_html_filename, "webview") != NULL) {  // webview version

    g_fn_prtlin("<div class=\"explrelationship\">");
    gbl_we_are_in_PRE_block = 1; /* 1 = yes, 0 = no */
  /*   g_fn_prtlin( "                                                        ");
  *   g_fn_prtlin( "     Note: a GOOD RELATIONSHIP needs 2 things:          ");
  *   g_fn_prtlin( "  1. compatibility potential (the \"Match Score\" above)  ");
  *   g_fn_prtlin( "  2. willpower to show positive personality traits      "); 
  *   g_fn_prtlin( "                                                        ");
  */
  //  g_fn_prtlin( "                                                  ");
  //  g_fn_prtlin( "     Note: a GOOD RELATIONSHIP needs 2 things:    ");
  //  g_fn_prtlin( "  1. compatibility potential                      ");
  //  g_fn_prtlin( "  2. BOTH sides show positive personality traits  ");
  //  g_fn_prtlin( "                                                  ");

    strcpy(spanbeg, "<span style=\"background-color: #fcfce0;\"> ");
    strcpy(spanend, "</span>");

    sprintf(writebuf, "%s%s%s", spanbeg,
      "                                                  ",
      spanend);
    g_fn_prtlin(writebuf);

    sprintf(writebuf, "%s%s%s", spanbeg,
      "     Note: a GOOD RELATIONSHIP needs 2 things:    ",
      spanend);
    g_fn_prtlin(writebuf);

    sprintf(writebuf, "%s%s%s", spanbeg,
      "  1. compatibility potential                      ",
      spanend);
    g_fn_prtlin(writebuf);

    sprintf(writebuf, "%s%s%s", spanbeg,
       "  2. BOTH sides show positive personality traits  ",
      spanend);
    g_fn_prtlin(writebuf);

    sprintf(writebuf, "%s%s%s", spanbeg,
       "                                                  ",
      spanend);
    g_fn_prtlin(writebuf);

    gbl_we_are_in_PRE_block = 0; /* 1 = yes, 0 = no */
    g_fn_prtlin("</div>");  // end of explrelationship

    g_fn_prtlin("<div class=\"appby\">");
    sprintf(writebuf, "produced by iPhone app %s", APP_NAME);
    g_fn_prtlin(writebuf);
    g_fn_prtlin("</div>");  // end of appby

    //g_fn_prtlin("<h4><span style=\"background-color:#FFBAC7;\">&nbspThis report is for entertainment purposes only.&nbsp</span></h4>");

    g_fn_prtlin("<div class=\"entertainment\">");
    //g_fn_prtlin("<span style=\"background-color:#FFBAC7;\">&nbspThis report is for entertainment purposes only.&nbsp</span>");
    g_fn_prtlin("This report is for entertainment purposes only.");
    g_fn_prtlin("</div>");  // end of appby
    g_fn_prtlin("<div><br><br></div>");

  } else {  // browser version


    g_fn_prtlin("<div><br><br></div>");

    g_fn_prtlin("<pre>");
    gbl_we_are_in_PRE_block = 1; /* 1 = yes, 0 = no */
  /*   g_fn_prtlin( "                                                        ");
  *   g_fn_prtlin( "     Note: a GOOD RELATIONSHIP needs 2 things:          ");
  *   g_fn_prtlin( "  1. compatibility potential (the \"Match Score\" above)  ");
  *   g_fn_prtlin( "  2. willpower to show positive personality traits      "); 
  *   g_fn_prtlin( "                                                        ");
  */
    g_fn_prtlin( "                                                  ");
    g_fn_prtlin( "     Note: a GOOD RELATIONSHIP needs 2 things:    ");
    g_fn_prtlin( "  1. compatibility potential                      ");
    g_fn_prtlin( "  2. both sides show positive personality traits  ");
    g_fn_prtlin( "                                                  ");
    gbl_we_are_in_PRE_block = 0; /* 1 = yes, 0 = no */
    g_fn_prtlin("</pre>");


//    sprintf(writebuf, "<h5><br><br>produced by iPhone app %s</h5>", APP_NAME);
//    g_fn_prtlin(writebuf);
//    g_fn_prtlin("<h4><span style=\"background-color:#FFBAC7;\">&nbspThis report is for entertainment purposes only.&nbsp</span></h4>");

    g_fn_prtlin("<div> <span style=\"font-size: 1.0em\"><br>produced by iPhone app \"Me and my BFFs\"</span><br><br><span style=\"font-size: 0.9em; font-weight: bold; color:#FF0000;\">This report is for entertainment purposes only.</span></div><div><br></div>");

  } // browser version



  g_fn_prtlin("\n</body>\n");
  g_fn_prtlin("</html>");


  fflush(Fp_g_HTML_file);
  /* close output HTML file
  */
  if (fclose(Fp_g_HTML_file) == EOF) {
    ;
/* trn("FCLOSE FAILED !!!   #1  "); */
  } else {
/* trn("FCLOSE SUCCESS !!!  #1  "); */
    ;
  };
  return(0);

} /* end of make_html_file_just_2_people() */ 


/* put label in field of 82, 2sp at end and beg
*/
void put_category_label(char *category_text, int inlen) 
{
/* tn();trn("CATEGORY OUTPUT"); ks(category_text); */
/*   int len_label, num_spaces_at_end; */
  int            num_spaces_at_end;
  char sformat[32], category_with_span[256];
 
/*   num_spaces_at_end = 92 - 13 - len_label + 1; */
/*   len_label = strlen(category_text); */
/*   num_spaces_at_end = 82 - 2 - len_label; */

  num_spaces_at_end = 82 - 2 - inlen;
  sprintf(category_with_span,"<span class=\"cCat\">%s</span>", category_text);

  /* sformat is like "%13s%s%37s" where 37 is num_spaces_at_end
  */
/*   sprintf(sformat, "%%13s%%s%%%ds",  num_spaces_at_end); */
  sprintf(sformat, "%%2s%%s%%%ds",  num_spaces_at_end);

  sprintf(writebuf, sformat, " ", category_with_span, " ");

  g_fn_prtlin(writebuf);

} /* end of  put_category_label(category_text)  */



/* WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW */
/* WWWWWWWWWWWWWWWWWWWWWW  trait rank   WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW */
/* WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW */

int make_html_file_trait_rank( /* in grphtm.c */
  char *group_name,
  int   num_persons_in_grp,
  char *trait_name,         /* could be  "Best Calendar Year nnnn" */
  char *in_html_filename,           /* in grphtm.c */
  struct trait_report_line  *in_trait_lines[],
  int   in_trait_lines_last_idx,
  char *grp_average_trait_scores_csv
)
{
  char rowcolor[16];
  int i;
/*   char myyear[8], c; */
  char c;
/*   int i, top_10, top_25, median, bot_25, bot_10; */
/*   int is_top_10_done; */
/*   int is_top_25_done; */
/*   int is_median_done;   */
/*   int is_bot_25_done;  */
/*   int is_bot_10_done;  */
/*   int is_print_good_milestone_at_end; * catch case- all scores are over good * */
/*   int num_pairs_to_rank; */
/*   int k,FORBIDDEN[64], ichk, rank_number, is_highlight_FORBIDDEN; */
  int  rank_number;

  strcpy(gbl_gfnameHTML, in_html_filename);

/* trn("in make_html_file_trait_rank() 1"); */
  /* ksn(group_name); ki(num_persons_in_grp); ks(trait_name);
  * ksn(in_html_filename); 
  * ki(in_trait_lines_last_idx);
  * ksn(grp_average_trait_scores_csv); 
  */

  strcpy(gbl_format_as, "trait rank");
/* b(30);ksn(trait_name); */
  strcpy(gbl_trait_name, trait_name);
/* ksn(grp_average_trait_scores_csv); */
/* strcpy(gblGrpAvgTraitScoresCSV2, "abcxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"); */
/* ksn(gblGrpAvgTraitScoresCSV2); */
  strcpy(gblGrpAvgTraitScoresCSV2, grp_average_trait_scores_csv);

/*   num_pairs_to_rank =  num_persons_in_grp * (num_persons_in_grp - 1) / 2; */
  gbl_we_are_in_PRE_block = 0;  /* init to false */


  /* open output HTML file
  */
  if ( (Fp_g_HTML_file = fopen(in_html_filename, "w")) == NULL ) {
    rkabort("Error  on open html for trait rank grphtm.c.  fopen().");
  }

  put_top_of_html_group_rpt(group_name); 


  /* '_' was for html filename */
/*   scharswitch(trait_name, '_', ' '); */

  if (    strstr(trait_name, "Best Calendar Year") != NULL) {

  g_fn_prtlin("  <div><br></div>");
    sprintf(writebuf, "  <h1>%s</h1>", trait_name);
    g_fn_prtlin(writebuf);
    sprintf(writebuf, "\n  <h2>in Group \"%s\"</h2>", group_name);
    g_fn_prtlin(writebuf);

/*     g_fn_prtlin("<pre>");
*     gbl_we_are_in_PRE_block = 1;
*     g_fn_prtlin("");
*     strcpy(myyear, &trait_name[strlen(trait_name) - 4]);
*     sprintf(writebuf, " Who has the best and worst %s ? ", myyear);
*     g_fn_prtlin(writebuf);
*     g_fn_prtlin("");
*     gbl_we_are_in_PRE_block = 0;
*     g_fn_prtlin("</pre>");
*/

  } else if (strstr(trait_name, "Best Day on") != NULL) {


/*     sprintf(writebuf, "  <h1>%s</h1>", trait_name);
*     g_fn_prtlin(writebuf);
*     sprintf(writebuf, "\n  <h2>in Group \"%s\"</h2>", group_name);
*     g_fn_prtlin(writebuf);
*/
    /* sprintf(my_trait_name, "Best Day on |%s&nbsp %s %d %d", */
    /*   <h1>Best Day on</h1> */
    /*   <h2>Wed&nbsp May 21 2014</h2> */
    /*   <h2>in Group "Ulli group"</h2> */

    /* char *csv_get_field(char *csv_string, char *delim, int want_fieldnum); */
  g_fn_prtlin("  <div><br></div>");
    sprintf(writebuf, "  <h1>%s</h1>", csv_get_field(trait_name, "|", 1));
    g_fn_prtlin(writebuf);
    sprintf(writebuf, "  <h2>%s</h2>", csv_get_field(trait_name, "|", 2));
    g_fn_prtlin(writebuf);

    sprintf(writebuf, "\n  <h2>in Group \"%s\"</h2>", group_name);
    g_fn_prtlin(writebuf);



/*     g_fn_prtlin("<pre>");
*     gbl_we_are_in_PRE_block = 1;
*     g_fn_prtlin("");
*     g_fn_prtlin(" Who has the best and worst day? ");
*     g_fn_prtlin("");
*     gbl_we_are_in_PRE_block = 0;
*     g_fn_prtlin("</pre>");
*/

  } else if (   strstr(trait_name, "ups and downs") != NULL
             || strstr(trait_name, "Ups and downs") != NULL) {
/*     sprintf(writebuf, "  <h1>Person with Most Ups and Downs</h1>"); */
  g_fn_prtlin("  <div><br></div>");
    sprintf(writebuf, "  <h1><span style=\"line-height:125%%;\">Person with Biggest<br>Ups and Downs in Life</span></h1>");
    g_fn_prtlin(writebuf);
    sprintf(writebuf, "\n  <h2>in Group \"%s\"</h2>", group_name);
    g_fn_prtlin(writebuf);
     
  } else if (   strstr(trait_name, "down to earth") != NULL
             || strstr(trait_name, "Down to earth") != NULL) {

  g_fn_prtlin("  <div><br></div>");
    sprintf(writebuf, "  <h1><span style=\"line-height:125%%;\">Most Down-to-earth Person</span></h1>");
    g_fn_prtlin(writebuf);

    sprintf(writebuf, "\n  <h2>in Group \"%s\"</h2>", group_name);
    g_fn_prtlin(writebuf);
  } else {
    c = trait_name[0];
    trait_name[0] = toupper(c); /* capitalize 1st ch */
  g_fn_prtlin("  <div><br></div>");
    sprintf(writebuf, "  <h1>Most %s Person</h1>", trait_name); 
    g_fn_prtlin(writebuf);
    sprintf(writebuf, "\n  <h2>in Group \"%s\"</h2>", group_name);
    g_fn_prtlin(writebuf);
  }


  /* start of table of ranking data  ----------------------------------
  */


  /* here we output ranking data lines in the table
  */
    
    
  g_fn_prtlin(" ");
  g_fn_prtlin( "<table>");
/*   g_fn_prtlin( "  <tr> <th>Rank</th> <th>Score</th> <th colspan=\"2\">Pair of Group Members</th> </tr>"); */
/*   g_fn_prtlin( "  <tr> <th>Rank in<br>Group</th> <th>Group Member</th> <th>Score</th> <th>Benchmark</th></tr>"); */
/*   g_fn_prtlin( "  <tr> <th>Rank</th> <th>Group Member</th> <th>Score</th> <th>Benchmark</th></tr>"); */
  g_fn_prtlin( "  <tr> <th></th> <th>Group Member</th> <th>Score</th> <th></th></tr>");



#ifdef PUT_BACK_COMMENTED_OUT_STUFF /****************************************/
*   /* Get FORBIDDEN rank numbers not to put highlight on every 4 because
*   *  they are within 3 of a colored milestone line (avoid color clutter)
*   */
* 
*   /* nums are percents of world-wide scores */
* /*   top_10 = 203;
* *   top_25 = 180;
* *   median = 154;
* *   bot_25 = 135;
* *   bot_10 = 116;
* */
* /*   top_10 = 373;
* *   top_25 = 213;
* *   median = 100;
* *   bot_25 =  42;
* *   bot_10 =  18;
* */
* 
*   top_10 = 90;
*   top_25 = 75;
*   median = 50;
*   bot_25 = 25;
*   bot_10 = 10;
* 
*   is_top_10_done = 0;  /* 0=no, 1=yes */
*   is_top_25_done = 0;  /* 0=no, 1=yes */
*   is_median_done = 0;  
*   is_bot_25_done = 0; 
*   is_bot_10_done = 0; 
* 
* 
* 
*   /* read thru all the avg_lines to get FORBIDDEN
*   */
*   int last_rank_number;
*   for (ichk=0; ichk <=63; ichk++) FORBIDDEN[ichk] = 0;  /* init */
*   k = 0;  /* INIT FORBIDDEN IDX */
*   for (i=0; i <= in_trait_lines_last_idx; i++) {
*     rank_number = in_trait_lines[i]->rank_in_group;
* 
*     if (rank_number != 0) {
*       last_rank_number = rank_number;
*       continue;
*     }
* /* trn("forwards");ki(last_rank_number); */
*     /* here we are on a benchmark line reading forwards
*     *  FORBID highlighting on rank_number and the two lines before that (3 lines tot)
*     *  *and* the 3 lines after
*     */
*     FORBIDDEN[++k] = last_rank_number;
*     if (last_rank_number-1 > 0) FORBIDDEN[++k] = last_rank_number-1;
*     if (last_rank_number-2 > 0) FORBIDDEN[++k] = last_rank_number-2;
*     if (last_rank_number+1 <= in_trait_lines_last_idx) FORBIDDEN[++k] = last_rank_number+1;
*     if (last_rank_number+2 <= in_trait_lines_last_idx) FORBIDDEN[++k] = last_rank_number+2;
*     if (last_rank_number+3 <= in_trait_lines_last_idx) FORBIDDEN[++k] = last_rank_number+3;
*   }
#endif /* ifdef PUT_BACK_COMMENTED_OUT_STUFF ********************************/


  /* for each rank line
  */
  int total_trait_score, average_trait_score;
  total_trait_score = 0;  /* init */
  for (i=0; i <= in_trait_lines_last_idx; i++)  {  /* NOTE: this for() is 130 lines */

    rank_number = in_trait_lines[i]->rank_in_group;

    /* intercept milestone lines (already sorted),
    *  and color appropriately
    */

    if (strcmp(in_trait_lines[i]->person_name, "zzzhilite-top10") == 0) {
/*       g_fn_prtlin( "<tr class=\"cGr2\"><td></td><td></td><td> 373 </td><td>Very High</td></tr>"); */
/*       g_fn_prtlin( "<tr class=\"cGr2\"><td></td><td></td><td> 90 </td><td>Very High</td></tr>"); */
/*       g_fn_prtlin( "<tr class=\"cGr2\"><td></td><td></td><td> 90 </td><td>Great</td></tr>"); */

      /* if this is best cal year, use benchmark labels  great and OMG
      */
      if (    strstr(trait_name, "Best Calendar Year") != NULL
           || strstr(trait_name, "Best Day on")        != NULL) {
        g_fn_prtlin( "<tr class=\"cGr2\"><td></td><td></td><td> 90 </td><td>Great</td></tr>");
        continue;
      } else {
        g_fn_prtlin( "<tr class=\"cGr2\"><td></td><td></td><td> 90 </td><td>Very High</td></tr>");
        continue;
      }
    }

    if (strcmp(in_trait_lines[i]->person_name, "zzzhilite-good") == 0) {
/*       g_fn_prtlin( "<tr class=\"cGre\"><td></td><td></td><td>213 </td><td>High</td></tr>"); */
/*       g_fn_prtlin( "<tr class=\"cGre\"><td></td><td></td><td> 75 </td><td>High</td></tr>"); */
/*       g_fn_prtlin( "<tr class=\"cGre\"><td></td><td></td><td> 75 </td><td>Good</td></tr>"); */
      if (    strstr(trait_name, "Best Calendar Year") != NULL
           || strstr(trait_name, "Best Day on")        != NULL) {
        g_fn_prtlin( "<tr class=\"cGre\"><td></td><td></td><td> 75 </td><td>Good</td></tr>");
        continue;
      } else {
        g_fn_prtlin( "<tr class=\"cGre\"><td></td><td></td><td> 75 </td><td>High</td></tr>");
        continue;
      }
    }

    if (strcmp(in_trait_lines[i]->person_name, "zzzhilite-trait") == 0) {
/*       g_fn_prtlin( "<tr class=\"cNeu\"><td></td><td></td><td> 100 </td><td>Median</td></tr>"); */
      g_fn_prtlin( "<tr class=\"cNeu\"><td></td><td></td><td> 50 </td><td>Average</td></tr>");
      continue;
    }

    if (strcmp(in_trait_lines[i]->person_name, "   hilite-bad") == 0) {
/*       g_fn_prtlin( "<tr class=\"cRed\"><td></td><td></td><td> 42 </td><td>Low</td></tr>"); */
/*       g_fn_prtlin( "<tr class=\"cRed\"><td></td><td></td><td> 25 </td><td>Low</td></tr>"); */
/*       g_fn_prtlin( "<tr class=\"cRed\"><td></td><td></td><td> 25 </td><td>Low</td></tr>"); */
/*       g_fn_prtlin( "<tr class=\"cRed\"><td></td><td></td><td> 25 </td><td>Stress</td></tr>"); */
      if (    strstr(trait_name, "Best Calendar Year") != NULL
           || strstr(trait_name, "Best Day on")        != NULL) {
        g_fn_prtlin( "<tr class=\"cRed\"><td></td><td></td><td> 25 </td><td>Stress</td></tr>");
        continue;
      } else {
        g_fn_prtlin( "<tr class=\"cRed\"><td></td><td></td><td> 25 </td><td>Low</td></tr>");
        continue;
      }
    }

    if (strcmp(in_trait_lines[i]->person_name, "   hilite-bot10") == 0) {
/*       g_fn_prtlin( "<tr class=\"cRe2\"><td></td><td></td><td> 18 </td><td>Very Low</td></tr>"); */
/*       g_fn_prtlin( "<tr class=\"cRe2\"><td></td><td></td><td> 10 </td><td>Very Low</td></tr>"); */
/*       g_fn_prtlin( "<tr class=\"cRe2\"><td></td><td></td><td> 10 </td><td>OMG</td></tr>"); */
      if (    strstr(trait_name, "Best Calendar Year") != NULL
           || strstr(trait_name, "Best Day on")        != NULL) {
        g_fn_prtlin( "<tr class=\"cRe2\"><td></td><td></td><td> 10 </td><td>OMG</td></tr>");
        continue;
      } else {
        g_fn_prtlin( "<tr class=\"cRe2\"><td></td><td></td><td> 10 </td><td>Very Low</td></tr>");
        continue;
      }
    }



      /* 201404  this is now distracting, with better colors up
      */
#ifdef PUT_BACK_COMMENTED_OUT_STUFF /****************************************/
 *     /* HIGHLIGHT every 5 rows, unless this rank_number is in FORBIDDEN list[]
 *     */
 *     strcpy(rowcolor, "");  /* default no highlight */
 * 
 *     if (in_trait_lines_last_idx + 1 > 36 &&  /* small rpt= no hilite */
 *         i != in_trait_lines_last_idx)        /* no highlight last line */
 *     {
 *       is_highlight_FORBIDDEN = 0;
 *       for (ichk=0; ichk <=63; ichk++) {
 *         if (rank_number == FORBIDDEN[ichk]) {
 *           is_highlight_FORBIDDEN = 1; /* no highlight, FORBIDDEN */
 *           break;
 *         }
 *       }
 * 
 *       /* change from every 4 to every 5
 *       */
 *       /* if (rank_number % 4 == 0  && is_highlight_FORBIDDEN == 0 )  */
 *       if (rank_number % 5 == 0  && is_highlight_FORBIDDEN == 0 ) {
 *         strcpy(rowcolor, " class=\"row4\"");
 *       } else {
 *         strcpy(rowcolor, "");
 *       }
 *     }
#endif /* ifdef PUT_BACK_COMMENTED_OUT_STUFF ********************************/


    /* output ranking line
    */

/* tn();ks(trait_name);
* int iiscore;
* iiscore = in_trait_lines[i]->score ;tn();b(123);ki(iiscore);
*/

/*  in_trait_lines[i]->score = mapBenchmarkNumToPctlRank(iiscore); */
/* iiscore = in_trait_lines[i]->score ;tn();b(124);ki(iiscore); */

    /* put default ROWCOLOR
    */
/* tn();trn("SETTING ROWCOLOR 11111");tn(); */
    if (in_trait_lines[i]->score >= 90) strcpy(rowcolor, " class=\"cGr2\"");
    if (in_trait_lines[i]->score <  90 &&
        in_trait_lines[i]->score >= 75) strcpy(rowcolor, " class=\"cGre\"");
    if (in_trait_lines[i]->score <  75 &&
        in_trait_lines[i]->score >  25) strcpy(rowcolor, " class=\"cNeu\"");
    if (in_trait_lines[i]->score <= 25 &&
        in_trait_lines[i]->score >  10) strcpy(rowcolor, " class=\"cRed\"");
    if (in_trait_lines[i]->score <= 10) strcpy(rowcolor, " class=\"cRe2\"");

/*     sprintf(writebuf, "<tr%s><td>%d</td><td> %s</td><td>%d</td><td></td></tr>", */
/*     sprintf(writebuf, "<tr%s><td>%d</td><td>%s</td><td>%d</td><td></td></tr>", */
   
    char score_as_char[4];
    if (in_trait_lines[i]->score == -1) {   /* exclude */
/*       strcpy(score_as_char, "xx"); */
      strcpy(score_as_char, "na");
    } else {
      sprintf(score_as_char, "%d", in_trait_lines[i]->score);
    }
/* tn();ksn(score_as_char); */

/*     sprintf(writebuf, "<tr%s><td>%d </td><td> %s</td><td>%d </td><td></td></tr>", */
    sprintf(writebuf, "<tr%s><td>%d </td><td> %s</td><td>%s </td><td></td></tr>",
      rowcolor,       
      in_trait_lines[i]->rank_in_group,
      in_trait_lines[i]->person_name,
/*       in_trait_lines[i]->score */
      score_as_char
    );
    g_fn_prtlin(writebuf);

    total_trait_score = total_trait_score + in_trait_lines[i]->score;
/* int sco; sco = in_trait_lines[i]->score; kin(sco); ki(total_trait_score); */

  } /* for each rank_line */

  average_trait_score = (int) floor (
    ( (double)total_trait_score / (double)num_persons_in_grp )
    + 0.5 );


  g_fn_prtlin( "</table>");
  g_fn_prtlin(" ");


  if (    strstr(trait_name, "Best Calendar Year") != NULL) {
    g_fn_prtlin("<pre>");
    gbl_we_are_in_PRE_block = 1;  /* true */
    g_fn_prtlin("");
    g_fn_prtlin("  Check out the report \"Calendar Year\".  ");
    g_fn_prtlin("");
    gbl_we_are_in_PRE_block = 0;  /* false */
    g_fn_prtlin("</pre>");

    g_fn_prtlin("<div></div>");
    g_fn_prtlin("<pre>");
    gbl_we_are_in_PRE_block = 1;  
    g_fn_prtlin("");
    g_fn_prtlin( "  Your intense willpower can         ");
    g_fn_prtlin( "  overcome and control your destiny  ");
    g_fn_prtlin("");
    gbl_we_are_in_PRE_block = 0; 
    g_fn_prtlin("</pre>");

  } else if ( strstr(trait_name, "Best Day on") != NULL) {


    g_fn_prtlin("  <pre>");         
    gbl_we_are_in_PRE_block = 1; 
    g_fn_prtlin("");
    g_fn_prtlin("  This measures short-term influences  ");
    g_fn_prtlin("  lasting a few hours.  ");
    g_fn_prtlin("");
    g_fn_prtlin("  More important long term influences are  ");
    g_fn_prtlin("  in the graphical report \"Calendar Year\"  ");
    g_fn_prtlin("  and in the group report \"Best Year\"  ");
    g_fn_prtlin("");
    gbl_we_are_in_PRE_block = 0; 
    g_fn_prtlin("</pre>");


  } else {

    g_fn_prtlin( "<pre>");
    gbl_we_are_in_PRE_block = 1;  /* true */
    g_fn_prtlin("");
    sprintf( writebuf,"  Check out the report \"Personality\".  ");
    g_fn_prtlin(writebuf);
    g_fn_prtlin("");
    gbl_we_are_in_PRE_block = 0;  /* false */
    g_fn_prtlin( "</pre>");
    
  }



/*   sprintf(writebuf, "<h5>produced by iPhone/iPad app named %s</h5>", APP_NAME); */
  sprintf(writebuf, "<h5>produced by iPhone app %s</h5>", APP_NAME);
  g_fn_prtlin(writebuf);

/*   g_fn_prtlin("<h4><span style=\"background-color:#FFBAC7;\">&nbsp&nbsp&nbsp&nbsp&nbsp  This report is for entertainment purposes only.&nbsp&nbsp&nbsp&nbsp&nbsp  </span></h4>"); */
  g_fn_prtlin("<h4><span style=\"background-color:#FFBAC7;\">&nbspThis report is for entertainment purposes only.&nbsp</span></h4>");



  g_fn_prtlin( "</body>");
  g_fn_prtlin( "</html>");


  fflush(Fp_g_HTML_file);
  /* close output HTML file
  */
  if (fclose(Fp_g_HTML_file) == EOF) {
    ;
/* trn("FCLOSE FAILED !!!   #2  "); */
  } else {
    ;
/* trn("FCLOSE SUCCESS !!!  #2  "); */
  };

  return(0);

} /* end of make_html_file_trait_rank() */




/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */
/* &&&&&&&&&&&&&&&&&&&&&&  whole_group  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */
/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */



int make_html_file_whole_group( /* produce actual html file */
  char *group_name,
  int   num_persons_in_grp,
  char *in_html_filename,           /* in grphtm.c */
  struct rank_report_line  *in_rank_lines[],
  int   in_rank_lines_last_idx,
  char *instructions,      /* like "format as person_in_group" */
                           /* like "return only html for table in string" */
  char *string_for_table_only)  /* 1024 chars max (its 9 lines formatted) */
                                /* holds  html only  OR  compat score only */
{
/*   char group_report_type[32], */
  char rowcolor[16];
  int i;
/*   int top_10, top_25, median, bot_25, bot_10, s; */
/*   int is_top_10_done; */
/*   int is_top_25_done; */
/*   int is_median_done;   */
/*   int is_bot_25_done;  */
/*   int is_bot_10_done;  */
/*   int is_print_good_milestone_at_end; * catch case- all scores are over good * */
/*   int h,k,FORBIDDEN[64], rank_number; */
  int h, rank_number;
  int i_top_this_many, i_bot_this_many, num_pairs_to_rank;
  int len_longest_name, lenA, lenB;
  char sformat3[20];

  strcpy(gbl_gfnameHTML, in_html_filename);
/* tn();tr("make_html_file_whole_group");ks(instructions); */

  if (strstr(instructions, "return only") == NULL) {
    trn("in make_html_file_whole_group()");
  }  /* avoid dbmsg on non-rpt call */

  i_top_this_many = 99999;  /* init to no top/bot restrictions */
  i_bot_this_many = 99999;  /* init to no top/bot restrictions */
  num_pairs_to_rank =  num_persons_in_grp * (num_persons_in_grp - 1) / 2;

  gbl_we_are_in_PRE_block = 0;  /* init to false */
  strcpy(global_instructions, instructions);
  strcpy(gbl_format_as, instructions);

/* ksn(instructions); */



  /* read thru all the rank_lines to get length of Longest Name for
  *  formatting name pairs in one field
  */
  len_longest_name = 1;

  for (i=0; i <= in_rank_lines_last_idx; i++) {

    if (in_rank_lines[i]->rank_in_group == 0) continue;

    // grab names for "html only" situation  (there is only 1 pair)
    strcpy(gbl_person_A_first_pair, in_rank_lines[i]->person_A); // for "only html" return
    strcpy(gbl_person_B_first_pair, in_rank_lines[i]->person_B); // for "only html" return

    lenA = (int)strlen(in_rank_lines[i]->person_A);
    lenB = (int)strlen(in_rank_lines[i]->person_B);
    if(lenA > len_longest_name) len_longest_name = lenA;
    if(lenB > len_longest_name) len_longest_name = lenB;
  }

/* kin(len_longest_name);tn(); */

  if (strcmp(global_instructions, "format as person_in_group") == 0) {
    /* get name of compare_everyone_with
    *  It will be person_A of first element where rank != 0
    */
    for(h=0; h <= in_rank_lines_last_idx; h++) {
      if (in_rank_lines[h]->rank_in_group != 0) {
        strcpy(gbl_compare_everyone_with,
          in_rank_lines[h]->person_A);
        /* for formatting name pairs
         * (+1 avoids jammed appearance for person_in_group rpt)
         */
        len_longest_name = (int)strlen(gbl_compare_everyone_with) + 1;
        break;
      }
    }
  }

  /* sprintf(instructions_for_top_bot,
  *   "top_this_many=|%d|bot_this_many=|%d|", top_this_many, bot_this_many);
  */
  if (strstr(global_instructions, "top_this_many") != NULL) {
    i_top_this_many = atoi(csv_get_field(global_instructions, "|", 2));
    i_bot_this_many = atoi(csv_get_field(global_instructions, "|", 4));
  }

  /* Note:  incoming struct rank_report_line  *in_rank_lines[],
  *  is sorted by score field
  */

  /* check for instructions to return string only
  */
  if (  strcmp(global_instructions, "return only html for table in string") == 0
     || strcmp(global_instructions, "return only compatibility score"     ) == 0 ) {

    ; /* DO NOT OPEN html file for write */

  } else {
    /* open output HTML file
    */
    if ( (Fp_g_HTML_file = fopen(in_html_filename, "w")) == NULL ) {
      rkabort("Error  on   grphtm.c.  fopen().");
    }
  } /* check for instructions to return string only */

  put_top_of_html_group_rpt(group_name); 


  if (strcmp(global_instructions, "format as person_in_group") == 0) {
    /* get name of compare_everyone_with
    *  It will be person_A of first element where rank != 0
    */
    for(h=0; h <= in_rank_lines_last_idx; h++) {
      if (in_rank_lines[h]->rank_in_group != 0) {
        strcpy(gbl_compare_everyone_with, in_rank_lines[h]->person_A);
        break;
      }
    }
/*     sprintf(writebuf, "  <h1>Best Match for %s</h1>", gbl_compare_everyone_with); */

/*     sprintf(writebuf, "  <h1>Best Match for <span class=\"cNam\">%s</span> </h1>", gbl_compare_everyone_with); */
/*     sprintf(writebuf, "  <h1>Best Match for %s </h1>", gbl_compare_everyone_with); */


    g_fn_prtlin("<div><br></div>");
    sprintf(writebuf, "  <h1>Best Match for %s </h1>", gbl_compare_everyone_with);
//    sprintf(writebuf, "  <h1>Best Match for <span class=\"cNam\">%s</span> </h1>", gbl_compare_everyone_with);

    g_fn_prtlin(writebuf);

    sprintf(writebuf, "\n  <h2>in Group \"%s\"</h2>", group_name);
    g_fn_prtlin(writebuf);
  } else {
    g_fn_prtlin("  <div><br></div>");
    g_fn_prtlin("  <h1>Best Match</h1>");
    sprintf(writebuf, "\n  <h2>in Group \"%s\"</h2>", group_name);
    g_fn_prtlin(writebuf);

    if (strstr(global_instructions, "top_this_many") != NULL) {
      g_fn_prtlin("  <h4><br><br><br>This report has over 300 lines, so<br><br><br><br><br>");

      sprintf(writebuf, "we show only the Top %d and Bottom %d.</h4>",
        i_top_this_many, i_bot_this_many);
      g_fn_prtlin(writebuf);
    }
  }


  /* start of table of ranking data  ----------------------------------
  */

  /* here we output ranking data lines in the table
  */
    
  g_fn_prtlin(" ");
  g_fn_prtlin( "<table>");
/*   g_fn_prtlin( "  <tr> <th>Rank</th> <th>Score</th> <th colspan=\"2\">Pair of Group Members</th> </tr>"); */
/*   g_fn_prtlin( "  <tr> <th>Rank in<br>Group</th>"); */
/*   g_fn_prtlin( "  <tr> <th>Rank</th>"); */
/*   g_fn_prtlin( "       <th>Compatibility<br>Score</th>"); */
/*   g_fn_prtlin( "       <th>Benchmark</th>  </tr>"); */

/*   g_fn_prtlin( "  <tr> <th></th>");
*   g_fn_prtlin( "       <th>Pair of <br>Group Members</th>");
*   g_fn_prtlin( "       <th>Score</th>");
*   g_fn_prtlin( "       <th></th>  </tr>");
*/
  g_fn_prtlin("  <tr> <th></th> <th>Pair of <br>Group Members</th>");
  g_fn_prtlin("       <th colspan=\"2\">Compatibility <br>Potential&nbsp</th> </tr>");

  /* check for instructions to return string only
  */
// nksn(instructions);
// nksn(in_html_filename);
// ksn(gbl_gfnameHTML);
  if (strcmp(instructions, "return only html for table in string") == 0) {

    strcpy(string_for_table_only,"");  /* init table string */

    if (strstr(in_html_filename, "webview") != NULL) {  // webview version
      // get size of margin-left
      int sizefld;
      char myleftmargin[32];
      do {

        sizefld = (int)strlen(gbl_person_A_first_pair) + 2 + (int)strlen(gbl_person_B_first_pair);

             if (sizefld ==  3) { strcpy(myleftmargin, "7.1em;"); }
        else if (sizefld ==  4) { strcpy(myleftmargin, "6.9em;"); }
        else if (sizefld ==  5) { strcpy(myleftmargin, "6.7em;"); }
        else if (sizefld ==  6) { strcpy(myleftmargin, "6.5em;"); }
        else if (sizefld ==  7) { strcpy(myleftmargin, "6.3em;"); }
        else if (sizefld ==  8) { strcpy(myleftmargin, "6.1em;"); }
        else if (sizefld ==  9) { strcpy(myleftmargin, "7.0em;"); }
        else if (sizefld == 10) { strcpy(myleftmargin, "6.75em;"); }  // bigger was 5.7
        else if (sizefld == 11) { strcpy(myleftmargin, "6.5em;"); }
        else if (sizefld == 12) { strcpy(myleftmargin, "6.25em;"); }
        else if (sizefld == 13) { strcpy(myleftmargin, "6.0em;"); }
        else if (sizefld == 14) { strcpy(myleftmargin, "5.75em;"); }
        else if (sizefld == 15) { strcpy(myleftmargin, "5.5em;"); }
        else if (sizefld == 16) { strcpy(myleftmargin, "5.25em;"); }
        else if (sizefld == 17) { strcpy(myleftmargin, "5.0em;"); }
        else if (sizefld == 18) { strcpy(myleftmargin, "4.75em;"); }
        else if (sizefld == 19) { strcpy(myleftmargin, "4.5em;"); }
        else if (sizefld == 20) { strcpy(myleftmargin, "4.25em;"); }
        else if (sizefld == 21) { strcpy(myleftmargin, "4.0em;"); }
        else if (sizefld == 22) { strcpy(myleftmargin, "3.75em;"); }
        else if (sizefld == 23) { strcpy(myleftmargin, "3.5em;"); }
        else if (sizefld == 24) { strcpy(myleftmargin, "3.25em;"); }
        else if (sizefld == 25) { strcpy(myleftmargin, "3.0em;"); }
        else if (sizefld == 26) { strcpy(myleftmargin, "2.75em;"); }
        else if (sizefld == 27) { strcpy(myleftmargin, "2.5em;"); }
        else if (sizefld == 28) { strcpy(myleftmargin, "2.25em;"); }
        else if (sizefld == 29) { strcpy(myleftmargin, "2.0em;"); }
        else if (sizefld == 30) { strcpy(myleftmargin, "1.75em;"); }
        else if (sizefld == 31) { strcpy(myleftmargin, "1.5em;"); }
        else if (sizefld == 32) { strcpy(myleftmargin, "1.25em;"); }
        else                    { strcpy(myleftmargin, "1.25em;"); }

      } while (0);  // get size of margin-left
        sprintf(writebuf, "<table style=\"margin-left: %s\">  <tr><th></th><th>Pair</th><th colspan=\"2\">Compatibility <br>Potential&nbsp</th></tr>", myleftmargin);

        strcat(string_for_table_only, writebuf);

    } else {  // browser version

      strcat(string_for_table_only,
        "<table>  <tr><th></th><th>Pair</th><th colspan=\"2\">Compatibility <br>Potential&nbsp</th></tr>");

    } // browser version

  }  /* if  "return only html for table in string" */




  
/*   is_top_25_done = 0;  * 0=no, 1=yes *
*   is_median_done = 0;  
*   is_bot_25_done = 0; 
*   is_print_good_milestone_at_end = 0;  * if=1, all scores are over good *
*/
  /* for each rank line
  */
  for (i=0; i <= in_rank_lines_last_idx; i++)  {
/* kin(in_rank_lines_last_idx); */

    rank_number = in_rank_lines[i]->rank_in_group;

/* tn();trn("COLOR milestone"); */
/* kin(i);ks(in_rank_lines[i]->person_B);  */



    /* intercept milestone lines (already sorted),
    * but remove person_B field and color appropriately 
    */
    if (strcmp(in_rank_lines[i]->person_B, "qhilite - top10") == 0) {
/*       g_fn_prtlin( "<tr class=\"cGr2\"><td></td><td></td><td>90   </td><td>Great</td></tr>"); */

      //g_fn_prtlin( "<tr class=\"cGr2\"><td></td><td></td><td>90 </td><td>Great</td></tr>");
      g_fn_prtlin( "<tr class=\"cGr2\"><td></td><td>90 </td><td>Great</td></tr>");

      if (strcmp(instructions, "return only html for table in string") == 0) {
/*         strcat(string_for_table_only, "<tr class=\"cGr2\"><td></td><td> Great</td><td>373  </td></tr>"); */
/*         strcat(string_for_table_only, */
/* "<tr class=\"cGr2\"><td></td><td></td> <td>90 </td> <td>Great</td> </tr>"); */

/*         strcat(string_for_table_only, */
/*           "<tr class=\"cGr2\"><td></td> <td>90 </td> <td>Great</td> </tr>"); */
        strcat(string_for_table_only,
          "<tr class=\"cGr2\"><td></td><td></td> <td>90 </td> <td>Great</td> </tr>");
      }
      continue;
    }
    if (strcmp(in_rank_lines[i]->person_B, "qhilite - good") == 0) {

      //g_fn_prtlin( "<tr class=\"cGre\"><td></td><td></td><td>75 </td><td>Good</td></tr>");
      g_fn_prtlin( "<tr class=\"cGre\"><td></td><td>75 </td><td>Good</td></tr>");

      if (strcmp(instructions, "return only html for table in string") == 0) {
/*         strcat(string_for_table_only, "<tr class=\"cGre\"><td></td><td> Good</td><td>213  </td></tr>"); */
/*         strcat(string_for_table_only, "<tr class=\"cGre\"><td></td><td></td> <td>75 </td> <td>Good</td> </tr>"); */
/*         strcat(string_for_table_only, */
/*           "<tr class=\"cGre\"><td></td> <td>75 </td> <td>Good</td> </tr>"); */
        strcat(string_for_table_only,
          "<tr class=\"cGre\"><td></td><td></td> <td>75 </td> <td>Good</td> </tr>");
      }
      continue;
    }
    if (strcmp(in_rank_lines[i]->person_B, "qhilite - avg") == 0) {

      //g_fn_prtlin( "<tr class=\"cNeu\"><td></td><td></td><td>50 </td><td>Average</td></tr>");
      g_fn_prtlin( "<tr class=\"cNeu\"><td></td><td>50 </td><td>Average</td></tr>");

      if (strcmp(instructions, "return only html for table in string") == 0) {
/*         strcat(string_for_table_only, "<tr class=\"cNeu\"><td></td><td> Median</td><td>100  </td></tr>"); */
/*         strcat(string_for_table_only, "<tr class=\"cNeu\"><td></td><td></td> <td>50 </td> <td>Average</td> </tr>"); */
/*         strcat(string_for_table_only, */
/*           "<tr class=\"cNeu\"><td></td> <td>50 </td> <td>Average</td> </tr>"); */
        strcat(string_for_table_only,
          "<tr class=\"cNeu\"><td></td><td></td> <td>50 </td> <td>Average</td> </tr>");
      }
      continue;
    }
    if (strcmp(in_rank_lines[i]->person_B, "qhilite - bad") == 0) {
/*       g_fn_prtlin( "<tr class=\"cRed\"><td></td><td></td><td>25   </td><td>Not So Good</td></tr>"); */

      //g_fn_prtlin( "<tr class=\"cRed\"><td></td><td></td><td>25 </td><td>Not Good </td></tr>");
      g_fn_prtlin( "<tr class=\"cRed\"><td></td><td>25 </td><td>Not Good </td></tr>");

      if (strcmp(instructions, "return only html for table in string") == 0) {
/*         strcat(string_for_table_only, "<tr class=\"cRed\"><td></td><td> Not So Good</td><td>42  </td></tr>"); */
/*         strcat(string_for_table_only, "<tr class=\"cRed\"><td></td><td></td> <td>25 </td> <td>Not Good </td> </tr>"); */
/*         strcat(string_for_table_only, */
/*           "<tr class=\"cRed\"><td></td> <td>25 </td> <td>Not Good </td> </tr>"); */
        strcat(string_for_table_only,
          "<tr class=\"cRed\"><td></td><td></td> <td>25 </td> <td>Not Good </td> </tr>");
      }
      continue;
    }
    if (strcmp(in_rank_lines[i]->person_B, "qhilite - bot10") == 0) {

      //g_fn_prtlin( "<tr class=\"cRe2\"><td></td><td></td><td>10 </td><td>OMG</td></tr>");
      g_fn_prtlin( "<tr class=\"cRe2\"><td></td><td>10 </td><td>OMG</td></tr>");

      if (strcmp(instructions, "return only html for table in string") == 0) {
/*         strcat(string_for_table_only, "<tr class=\"cRe2\"><td></td><td> OMG</td><td>18  </td></tr>"); */
/*         strcat(string_for_table_only, "<tr class=\"cRe2\"><td></td><td></td> <td>18   </td> <td>OMG</td> </tr></table>"); */
/*         strcat(string_for_table_only, "<tr class=\"cRe2\"><td></td><td></td> <td>10 </td> <td>OMG</td> </tr>"); */
/*         strcat(string_for_table_only, */
/*           "<tr class=\"cRe2\"><td></td> <td>10 </td> <td>OMG</td> </tr>"); */
/*         strcat(string_for_table_only, */
/*           "<tr class=\"cRe2\"><td></td><td></td> <td>10 </td> <td>OMG</td> </tr></table>"); */
        strcat(string_for_table_only,
          "<tr class=\"cRe2\"><td></td><td></td> <td>10 </td> <td>OMG</td> </tr>");
      }
      continue;
    }
/* trn("after milestone");tn(); */

    /* put default ROWCOLOR
    */
/* trn("SETTING ROWCOLOR 33333"); */
    if (in_rank_lines[i]->score >= 90) strcpy(rowcolor, " class=\"cGr2\"");
    if (in_rank_lines[i]->score <  90 &&
        in_rank_lines[i]->score >= 75) strcpy(rowcolor, " class=\"cGre\"");
    if (in_rank_lines[i]->score <  75 &&
        in_rank_lines[i]->score >  25) strcpy(rowcolor, " class=\"cNeu\"");
    if (in_rank_lines[i]->score <= 25 &&
        in_rank_lines[i]->score >  10) strcpy(rowcolor, " class=\"cRed\"");
    if (in_rank_lines[i]->score <= 10) strcpy(rowcolor, " class=\"cRe2\"");


    /* Here we print the ranking table lines  UNLESS
    *  we have  top200/bot100-like instructions that apply 
    *  and result in not printing the middle lines.
    *
    *  Check current rank against top200/bot100.
    */

/*     if (in_rank_lines[i]->rank_in_group ==
*         num_pairs_to_rank - i_bot_this_many + 1) {
*     }
*/
    if (in_rank_lines[i]->rank_in_group > i_top_this_many  &&
        in_rank_lines[i]->rank_in_group < num_pairs_to_rank - i_bot_this_many + 1)
    {
      continue;
    }

    /*  put out line for boundary of bottom 100 before outputting that line
    */
    if (in_rank_lines[i]->rank_in_group ==
      num_pairs_to_rank - i_bot_this_many + 1) {

/*       sprintf(writebuf, "<tr><td></td><td></td><td></td></tr>"); */
      sprintf(writebuf, "<tr><td></td><td></td><td></td><td></td></tr>");
      g_fn_prtlin(writebuf);
      sprintf(writebuf,
        "<tr class=\"cHed\"><td></td><td> Bottom %d</td><td></td><td></td></tr>",
        i_bot_this_many);
      g_fn_prtlin(writebuf);
    }



    /* output ranking line
    */

    /* if report is "Best Match for ...", color kingpin person
    */
//     char color_of_person_A_beg[128];
//     char color_of_person_A_end[128];
//     if (strcmp(global_instructions, "format as person_in_group") == 0) {
// /*       strcpy(color_of_person_A_beg, "<span class=\"cNam2\">"); */
// /*       strcpy(color_of_person_A_end, "</span>"); */
//       strcpy(color_of_person_A_beg, "");
//       strcpy(color_of_person_A_end, "");
//     } else {
//       strcpy(color_of_person_A_beg, "");
//       strcpy(color_of_person_A_end, "");
//     }

/*     sprintf(sformat3, "%%-%ds %%s", len_longest_name); */

    if (  strcmp(global_instructions, "return only html for table in string") == 0) {
      strcpy(sformat3, "%s  %s");
    } else {
      sprintf(sformat3, "%%-%ds %%s", len_longest_name);
    }
    sprintf(writebuf2,  sformat3,
      in_rank_lines[i]->person_A,
      in_rank_lines[i]->person_B
    );

/*     sprintf(writebuf, "<tr%s><td>%d </td><td> %s</td><td>%d  </td></tr>", */
/*     sprintf(writebuf, "<tr%s><td>%d </td><td> %s</td><td>%d   </td><td></td></tr>", */
/*     sprintf(writebuf, "<tr%s><td>%d</td><td> %s</td><td>%d</td><td></td></tr>", */
    sprintf(writebuf, "<tr%s><td>%d </td><td> %s</td><td>  %d </td><td></td></tr>",
      rowcolor,       
      in_rank_lines[i]->rank_in_group,
      writebuf2,
      in_rank_lines[i]->score
    );
    g_fn_prtlin(writebuf);

    if (strcmp(instructions, "return only html for table in string") == 0) {
/*       strcat(string_for_table_only, writebuf); */

      sprintf(writebuf3, "<tr%s><td></td><td> %s</td><td> %d </td><td></td></tr>",
        rowcolor,       
        writebuf2,  /* names */
        in_rank_lines[i]->score
      );
      strcat(string_for_table_only, writebuf3);


    }

    if (strcmp(global_instructions, "return only compatibility score") == 0 ) {

      sprintf(string_for_table_only, "%d", in_rank_lines[i]->score);
/* ksn(string_for_table_only); */
      /* this actually serves as the string for compatibility score 
      */

    }



    /*  put out line for boundary of top 200 after outputting that line
    */
    if (in_rank_lines[i]->rank_in_group == i_top_this_many) {
      sprintf(writebuf,
        "<tr class=\"cHed\"><td></td><td> Top %d</td><td></td><td></td></tr>",
        i_top_this_many);
      g_fn_prtlin(writebuf);
      sprintf(writebuf, "<tr><td></td><td></td><td><td></td></tr>");
      g_fn_prtlin(writebuf);
    }

  } /* for each rank_line */


  g_fn_prtlin( "</table>");
  g_fn_prtlin(" ");

  if (strcmp(instructions, "return only html for table in string") == 0) {
    strcat(string_for_table_only, "</table>");
  }


/* wall of numbers
*/
/*   g_fn_prtlin( "<h3> </h3>");
*   g_fn_prtlin( "<pre>");
*   g_fn_prtlin( "");
*   g_fn_prtlin( "      top score is higher than 1,000 ");
*   g_fn_prtlin( " 10% of scores are higher than   373 ");
*   g_fn_prtlin( " 20% of scores are higher than   250 ");
*   g_fn_prtlin( " 30% of scores are higher than   180 ");
*   g_fn_prtlin( " 40% of scores are higher than   137 ");
*   g_fn_prtlin( " 50% of scores are higher than   100 ");
*   g_fn_prtlin( " 60% of scores are higher than    73 ");
*   g_fn_prtlin( " 70% of scores are higher than    51 ");
*   g_fn_prtlin( " 80% of scores are higher than    34 ");
*   g_fn_prtlin( " 90% of scores are higher than    18 ");
*   g_fn_prtlin( "               bottom score is     1 ");
*   g_fn_prtlin( "");
*   g_fn_prtlin( "</pre>");
*/


/*   sprintf(writebuf, "<pre> there are %d members in the group ", 
*     num_persons_in_grp);
*   g_fn_prtlin(writebuf);
*/

  if (strcmp(global_instructions, "format as person_in_group") != 0) {


    g_fn_prtlin( "<pre>");
    gbl_we_are_in_PRE_block = 1;  /* true */

    g_fn_prtlin(" ");
    g_fn_prtlin("  To see all the scores for only one person,  ");
/*     g_fn_prtlin(" check out the report \"Best Match for ... \" "); */
    g_fn_prtlin("  check out the report \"Best Match For ...\"  ");
/*     g_fn_prtlin("  To see all the scores for only one person,  "); */
/*     g_fn_prtlin(" check out the report "); */
/*     g_fn_prtlin(" Best Match for ... in Group ... "); */
    g_fn_prtlin(" ");
    gbl_we_are_in_PRE_block = 0;  /* false */
    g_fn_prtlin( "</pre>");
  }

    g_fn_prtlin( "<div><br></div>");


  g_fn_prtlin("<pre>");
  gbl_we_are_in_PRE_block = 1; /* 1 = yes, 0 = no */
/*   g_fn_prtlin( "                                                     "); */
/*   g_fn_prtlin( "     Note: a GOOD RELATIONSHIP needs 2 things:       "); */
/*   g_fn_prtlin( "  1. compatibility potential (the score above)       "); */
/*   g_fn_prtlin( "  2. willpower to show positive personality traits   "); */
/*   g_fn_prtlin( "                                                     "); */

  g_fn_prtlin( "                                                  ");
  g_fn_prtlin( "     Note: a GOOD RELATIONSHIP needs 2 things:    ");
  g_fn_prtlin( "  1. compatibility potential                      ");
  g_fn_prtlin( "  2. BOTH sides show positive personality traits  ");
  g_fn_prtlin( "                                                  ");

  gbl_we_are_in_PRE_block = 0; /* 1 = yes, 0 = no */
  g_fn_prtlin("</pre>");



//    sprintf(writebuf, "<h5><br><br>produced by iPhone app %s</h5>", APP_NAME);
//    g_fn_prtlin(writebuf);
//  g_fn_prtlin("<h4><span style=\"background-color:#FFBAC7;\">&nbspThis report is for entertainment purposes only.&nbsp</span></h4>");

  g_fn_prtlin("<div> <span style=\"font-size: 1.0em\"><br>produced by iPhone app \"Me and my BFFs\"</span><br><br><span style=\"font-size: 0.9em; font-weight: bold; color:#FF0000;\">This report is for entertainment purposes only.</span></div><div><br></div>");


  g_fn_prtlin( "</body>");
  g_fn_prtlin( "</html>");


  /* check for instructions to return string only
  */
  if (  strcmp(global_instructions, "return only html for table in string") == 0
     || strcmp(global_instructions, "return only compatibility score"     ) == 0 ) {

    ; /* DO NOT OPEN html file for write */

  } else {
    fflush(Fp_g_HTML_file);
    /* close output HTML file
    */
    if (fclose(Fp_g_HTML_file) == EOF) {
      ;
/* trn("FCLOSE FAILED !!!   #4  "); */
    } else {
      ;
/* trn("FCLOSE SUCCESS !!!  #4  "); */
    };
  }

  if (strcmp(global_instructions, "return only compatibility score") == 0 ) {
    strcpy(global_instructions, "");
  }

  return(0);

} /* end of make_html_file_whole_group() */




void put_top_of_html_group_rpt(char *group_name) {

  /* comments are    <!--  i am commented out  -->  */

  /*   at end, change to STRICT  */
  g_fn_prtlin( "<!doctype html public \"-//w3c//dtd html 4.01 transitional//en\" ");
  g_fn_prtlin( "  \"http://www.w3.org/TR/html4/loose.dtd\">");

  g_fn_prtlin( "<html>");
  g_fn_prtlin( "\n<head>");



  /* <title> shows up in the tab and tooltip when hover mouse ptr over chrome tab
  */

  /* if HTML filename, gbl_ffnameHTML, has any slashes, grab the basename
  */
  char myBaseName[256], *myptr;
  if (sfind(gbl_gfnameHTML, '/')) {
    myptr = strrchr(gbl_gfnameHTML, '/');
    strcpy(myBaseName, myptr + 1);
  } else {
    strcpy(myBaseName, gbl_gfnameHTML);
  }


  if (strcmp(gbl_format_as, "trait rank") == 0) {

    if (strstr(gbl_trait_name, "Best Day on") != NULL) {
/*       sprintf(writebuf, "  <title>%s, Best Day, produced by iPhone/iPad app named %s.</title>", group_name, APP_NAME); */
/*       sprintf(writebuf, "  <title>%s, Best Day, produced by iPhone app %s.</title>", group_name, APP_NAME); */
/*       g_fn_prtlin(writebuf); */

      sprintf(writebuf, "  <title>%s</title>", myBaseName);
      g_fn_prtlin(writebuf);

    } else if (strstr(gbl_trait_name, "Best Calendar Year") != NULL) {
/*       sprintf(writebuf, "  <title>%s, Best Calendar Year, produced by iPhone app %s.</title>", group_name, APP_NAME); */
/*       g_fn_prtlin(writebuf); */
      sprintf(writebuf, "  <title>%s</title>", myBaseName);
      g_fn_prtlin(writebuf);

    } else {
      /* TRAITS here "most assertive" etc ... */

/*       sprintf(writebuf, "  <title>%s, %s, produced by iPhone app %s.</title>", group_name, gbl_trait_name, APP_NAME); */
/*       g_fn_prtlin(writebuf); */
      sprintf(writebuf, "  <title>%s</title>", myBaseName);
      g_fn_prtlin(writebuf);
    }

  }  /* trait stuff above */

  else if (strcmp(gbl_format_as, "average scores") == 0) {
/*     sprintf(writebuf, "  <title>%s, Most Compatible Person, produced by iPhone app %s.</title>", group_name, APP_NAME); */
/*     g_fn_prtlin(writebuf); */
      sprintf(writebuf, "  <title>%s</title>", myBaseName);
      g_fn_prtlin(writebuf);

  } else {
    if (strcmp(global_instructions, "format as person_in_group") == 0) {
/*       sprintf(writebuf, "  <title>%s, %s Best Match, produced by iPhone app %s.</title>", */
/*         group_name, */
/*         gbl_compare_everyone_with, */
/*         APP_NAME */
/*       ); */
/*       g_fn_prtlin(writebuf); */
      sprintf(writebuf, "  <title>%s</title>", myBaseName);
      g_fn_prtlin(writebuf);

    } else {
      /* BEST MATCH in Group    here */

/*     sprintf(writebuf, "  <title>%s Best Match, produced by iPhone app %s.</title>", group_name, APP_NAME); */
/*       g_fn_prtlin(writebuf); */
      sprintf(writebuf, "  <title>%s</title>", myBaseName);
      g_fn_prtlin(writebuf);
    }
  }

  


  /* HEAD  META
  */
  if (strcmp(gbl_format_as, "trait rank") == 0) {

    if (strstr(gbl_trait_name, "Best Day on") != NULL) {
      sprintf(writebuf, "  <meta name=\"description\" content=\"Best Day in Group report produced by iPhone app %s\"> ", APP_NAME);
      g_fn_prtlin(writebuf);

    } else if (strstr(gbl_trait_name, "Best Calendar Year") != NULL) {
      sprintf(writebuf, "  <meta name=\"description\" content=\"Best Calendar Year in Group report produced by iPhone app %s\"> ", APP_NAME);
      g_fn_prtlin(writebuf);

    } else {
      sprintf(writebuf, "  <meta name=\"description\" content=\"Most %s in Group report produced by iPhone app %s\"> ", gbl_trait_name, APP_NAME);
      g_fn_prtlin(writebuf);
    }

  } else if (strcmp(gbl_format_as, "average scores") == 0) {
    sprintf(writebuf, "  <meta name=\"description\" content=\"Most Compatible Person in Group report produced by iPhone app %s\"> ", APP_NAME);
    g_fn_prtlin(writebuf);

  } else {
    if (strcmp(global_instructions, "format as person_in_group") == 0) {
      sprintf(writebuf, "  <meta name=\"description\" content=\"In Group, person is most compatibe with ... produced by iPhone app %s\"> ", APP_NAME);
      g_fn_prtlin(writebuf);

    } else {
      sprintf(writebuf, "  <meta name=\"description\" content=\"Most compatible Pair in Group produced by iPhone app %s\"> ", APP_NAME);
      g_fn_prtlin(writebuf);
    }
  }

/*   g_fn_prtlin( "  <meta http-equiv=\"Content-Type\" content=\"text/html\"; charset=\"iso-8859-1\">");  */
  g_fn_prtlin( "  <meta http-equiv=\"Content-Type\" content=\"text/html\"; charset=\"UTF-8\">"); 
/*   g_fn_prtlin( "  <meta name=\"Author\" content=\"Author goes here\">"); */


/*   g_fn_prtlin( "  <meta name=\"keywords\" content=\"measure,group,member,best,match,calendar,year,passionate,personality\"> "); */
/*   g_fn_prtlin( "  <meta name=\"keywords\" content=\"BFF,astrology,compatibility,group,best,match,calendar,year,stress,personality\"> "); */

/*   g_fn_prtlin( "  <meta name=\"keywords\" content=\"women,woman,female,BFF,compatibility,group,best,match,personality,stress,calendar,year\"> "); */ /* 86 chars */ 
  g_fn_prtlin( "  <meta name=\"keywords\" content=\"women,woman,female,BFF,me,compatibility,group,best,match,personality,stress,calendar,year\"> ");  /* 89 chars */


  /* get rid of CHROME translate "this page is in Galician" 
  * do you want to translate?
  */
  g_fn_prtlin("  <meta name=\"google\" content=\"notranslate\">");
  g_fn_prtlin("  <meta http-equiv=\"Content-Language\" content=\"en\" />");
  g_fn_prtlin("  <meta name = \"format-detection\" content = \"telephone=no\">");

  /* HEAD   STYLE/CSS
  */
  g_fn_prtlin( "\n  <style type=\"text/css\">");

/*   g_fn_prtlin( "  @media print { BODY {font-size: 60% } }"); */
  g_fn_prtlin( "    @media print { TABLE {font-size: 75%; } }");

  g_fn_prtlin( "    BODY {");
/*  g_fn_prtlin( "      background-color: #F5EFCF;"); */
  g_fn_prtlin( "      background-color: #f7ebd1;");
/*   g_fn_prtlin( "      font-family: Trebuchet MS, Arial, Verdana, sans-serif;"); */
  g_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");
  g_fn_prtlin( "      font-size:   medium;");
  g_fn_prtlin( "      font-weight: normal;");
  g_fn_prtlin( "      text-align:  center;");
/*   g_fn_prtlin( "    <!-- "); */
/*   g_fn_prtlin( "      background-image: url('mkgif1g.gif');"); */
/*   g_fn_prtlin( "    --> "); */
  g_fn_prtlin( "    }");

/*   g_fn_prtlin( "    H1 { font-size: 137%; font-weight: bold;   line-height: 95%; text-align: center;}"); */
/*   g_fn_prtlin( "    H2 { font-size: 137%; font-weight: bold;   line-height: 25%; text-align: center;}"); */
/*   g_fn_prtlin( "    H3 { font-size: 110%; font-weight: normal; line-height: 30%; text-align: center;}"); */
  g_fn_prtlin( "    H1 { font-size: 137%; font-weight: bold;   line-height: 15%; text-align: center;}");
  g_fn_prtlin( "    H2 { font-size: 120%; font-weight: bold;   line-height: 95%; text-align: center;}");
  g_fn_prtlin( "    H3 { font-size: 110%; font-weight: normal; line-height: 95%; text-align: center;}");


/*   g_fn_prtlin( "    H5 { font-size:  55%; font-weight: normal; line-height: 90%; text-align: center;}"); */
/*   g_fn_prtlin( "    H5 { font-size:  70%; font-weight: normal; line-height: 30%; text-align: center;}"); */
/*   g_fn_prtlin( "    H4 { font-size:  85%; font-weight: bold;   line-height: 30%; text-align: center;}"); */

  g_fn_prtlin( "    H4 { font-size:  75%; font-weight: bold;   line-height: 30%; text-align: center;}");
  g_fn_prtlin( "    H5 { font-size:  70%; font-weight: normal; line-height: 30%; text-align: center;}");


  g_fn_prtlin( "    PRE {");
/*   g_fn_prtlin( "      padding: 1%;"); */
  g_fn_prtlin( "      display: inline-table;");

/*   g_fn_prtlin( "      border-style: solid;"); */
/*   g_fn_prtlin( "      border-color: #e4dfae; "); */
/*   g_fn_prtlin( "      border-width: 5px;"); */

  g_fn_prtlin( "      display: inline-block;");
  g_fn_prtlin( "      background-color: #fcfce0;");
  g_fn_prtlin( "      border: none;");
  g_fn_prtlin( "      border-collapse: collapse;");
  g_fn_prtlin( "      border-spacing: 0;");

/*   g_fn_prtlin( "      padding-left 10px;"); */
/*   g_fn_prtlin( "      padding-right 10px;"); */

/*   g_fn_prtlin( "      font-family: Andale Mono, Menlo, Monospace, Courier New;"); */
  g_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");
  g_fn_prtlin( "      font-weight: normal;");
  g_fn_prtlin( "      font-size:   75%;");
/*   g_fn_prtlin( "      line-height: 110%;"); */
  g_fn_prtlin( "      line-height: 131%;");
  g_fn_prtlin( "    }");
  g_fn_prtlin( "    P { ");
/*   g_fn_prtlin( "      font-family: Andale Mono, Menlo, Monospace, Courier New;"); */
  g_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");
  g_fn_prtlin( "      width: auto;");
  g_fn_prtlin( "      font-size:   80%;");
  g_fn_prtlin( "      margin-top: 0;");
  g_fn_prtlin( "      margin-bottom: 0;");
  g_fn_prtlin( "      margin-left: auto;");
  g_fn_prtlin( "      margin-right:auto;");
/*   g_fn_prtlin( "      padding-left: 5%;"); */
/*   g_fn_prtlin( "      padding-right:5%;"); */
/*   g_fn_prtlin( "      text-align: center;"); */
  g_fn_prtlin( "      text-align: left;");
  g_fn_prtlin( "    }");
/* for table: */
/*       border: 2px solid black; */
/*       cellspacing: 0; */
/*       border-top: 0; */
/*       border-bottom: 0; */
/*   g_fn_prtlin( "    table.center {"); */
/*   g_fn_prtlin( "      margin-left:auto;"); */
/*   g_fn_prtlin( "      margin-right:auto;"); */
/*   g_fn_prtlin( "    }"); */
/*   g_fn_prtlin( "    table, th, td{"); */
/*   g_fn_prtlin( "      border: 1px solid black;"); */
/*   g_fn_prtlin( "      border-collapse: collapse;"); */
/*   g_fn_prtlin( "      border-spacing: 0;"); */
/*   g_fn_prtlin( "    }"); */
/*   g_fn_prtlin( "    TD {"); */
/*   g_fn_prtlin( "      white-space: nowrap;"); */
/*   g_fn_prtlin( "      padding: 0;"); */
/*   g_fn_prtlin( "    }"); */

  g_fn_prtlin( "    table {");
/*   g_fn_prtlin( "      font-family: Andale Mono, Menlo, Monospace, Courier New;"); */
  g_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");
  g_fn_prtlin( "      text-align: left;");

/*   g_fn_prtlin( "      border: 1px solid black;"); */
/*   g_fn_prtlin( "      border-collapse: collapse;"); */
/*   g_fn_prtlin( "      border-spacing: 0;"); */
     g_fn_prtlin( "      border: none;");
  g_fn_prtlin( "      border-spacing: 0;");


/*   g_fn_prtlin( "      padding-right:2%;"); */
/*   g_fn_prtlin( "      padding-left:2%;"); */
  g_fn_prtlin( "      margin-left: auto;");
  g_fn_prtlin( "      margin-right:auto;");
  g_fn_prtlin( "    }");
  g_fn_prtlin( "    td {");
/*   g_fn_prtlin( "      font-family: Andale Mono, Monospace, Courier New;"); */

  g_fn_prtlin( "      white-space: pre;");
/*   g_fn_prtlin( "      font-size: 80%;"); */
  g_fn_prtlin( "      font-size: 90%;");

  g_fn_prtlin( "      text-align: left;");

/*   g_fn_prtlin( "      border: 1px solid black;"); */
/*   g_fn_prtlin( "      border-collapse: collapse;"); */
/*   g_fn_prtlin( "      border-spacing: 0;"); */
     g_fn_prtlin( "      border: none;");
  g_fn_prtlin( "      border-spacing: 0;");

/*   g_fn_prtlin( "      padding-left: 5px; "); */
/*   g_fn_prtlin( "      padding-right: 5px; "); */
  g_fn_prtlin( "      padding-left: 5px; ");
  g_fn_prtlin( "      padding-right: 5px; ");

  g_fn_prtlin( "    }");
  g_fn_prtlin( "    table.center {");
  g_fn_prtlin( "      margin-left:auto;");
  g_fn_prtlin( "      margin-right:auto;");
  g_fn_prtlin( "    }");
  g_fn_prtlin( "    th {");

  g_fn_prtlin( "      padding: 5px; ");
  g_fn_prtlin( "      vertical-align:bottom;");

/*   g_fn_prtlin( "      font-family: Trebuchet MS, Arial, Verdana, sans-serif;"); */
  g_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");

/*   g_fn_prtlin( "      font-size: 85%;"); */
  g_fn_prtlin( "      font-size: 90%;");

/*   g_fn_prtlin( "      padding-left: 10px; "); */
/*   g_fn_prtlin( "      padding-right: 10px; "); */

/*   g_fn_prtlin( "      background-color: #e1fdc3 ;"); */
     g_fn_prtlin( "      background-color: #fcfce0 ;");

/*   g_fn_prtlin( "      border: 1px solid black;"); */
  g_fn_prtlin( "      border: none;");
  g_fn_prtlin( "      border-spacing: 0;");

  g_fn_prtlin( "      text-align: center;");
  g_fn_prtlin( "    }");

  g_fn_prtlin( "    table td  { text-align: right; ");
/*   g_fn_prtlin( "      font-family: Andale Mono, Menlo, Monospace, Courier New;"); */
  g_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");
  g_fn_prtlin( "    }");

  g_fn_prtlin( "    table td+td { text-align: left; ");
/*   g_fn_prtlin( "      font-family: Andale Mono, Menlo, Monospace, Courier New;"); */
  g_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");
  g_fn_prtlin( "    }");

  g_fn_prtlin( "    table td+td+td { text-align: right; ");
/*   g_fn_prtlin( "      padding-right:3%;"); */
/*   g_fn_prtlin( "      font-family: Andale Mono, Menlo, Monospace, Courier New;"); */
  g_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");
  g_fn_prtlin( "    }");

  g_fn_prtlin( "    table td+td+td+td { text-align: left; ");
/*   g_fn_prtlin( "      font-family: Andale Mono, Menlo, Monospace, Courier New;"); */
  g_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");
  g_fn_prtlin( "    }");

/*   g_fn_prtlin( "    .row4        { background-color:#fdfddc; }"); */
/*   g_fn_prtlin( "    .cLineGood   { background-color:#c3fdc3; }"); */
/*   g_fn_prtlin( "    .cLineStress { background-color:#ffbac7; }"); */
  g_fn_prtlin( "    .row4        { background-color:#f8f0c0; }");

/*   p_fn_prtlin( "    .cGr2        { background-color:#d0fda0; }"); */
/*   g_fn_prtlin( "    .cGr2        { background-color:#8bfd87; }"); */
/*   g_fn_prtlin( "    .cGr2        { background-color:#66ff33; }"); */

/*   p_fn_prtlin( "    .cGre        { background-color:#e1fdc3; }"); */
/*   g_fn_prtlin( "    .cGre        { background-color:#ccffb9; }"); */
/*   p_fn_prtlin( "    .cRed        { background-color:#ffd9d9; }"); */
/*   g_fn_prtlin( "    .cRed        { background-color:#ffcccc; }"); */
/*   p_fn_prtlin( "    .cRe2        { background-color:#fcb9b9; }"); */
/*   g_fn_prtlin( "    .cRe2        { background-color:#fc8888; }"); */
/*   g_fn_prtlin( "    .cRe2        { background-color:#fc6094; }"); */
/*   g_fn_prtlin( "    .cRe2        { background-color:#ff3366; }"); */


  /* put GREEN highlight for trait, green+red for other
  */
  if (strcmp(gbl_format_as, "trait rank") == 0 
    && strstr(gbl_trait_name, "Best Calendar Year") == NULL 
    && strstr(gbl_trait_name, "Best Day on") == NULL )
  {

    /*     g_fn_prtlin( "    .cGr2        { background-color:#a3f275; }"); */
    /*     g_fn_prtlin( "    .cGre        { background-color:#bbf699; }"); */
    /*     g_fn_prtlin( "    .cNeu        { background-color:#d3f9bd; }"); */
    /*     g_fn_prtlin( "    .cRed        { background-color:#bbf699; }"); */
    /*     g_fn_prtlin( "    .cRe2        { background-color:#a3f275; }"); */

/*     g_fn_prtlin( "    .cGr2        { background-color:#a3f275; }");  */
/*     g_fn_prtlin( "    .cGre        { background-color:#a3f275; }"); */
/*     g_fn_prtlin( "    .cNeu        { background-color:#a3f275; }"); */
/*     g_fn_prtlin( "    .cRed        { background-color:#a3f275; }"); */
/*     g_fn_prtlin( "    .cRe2        { background-color:#a3f275; }"); */

    /* all same green (all good) */

/*     g_fn_prtlin( "    .cGr2,.cGre,.cNeu,.cRed,.cRe2 {background-color: #a3f275;}"); */
    g_fn_prtlin( "    .cGr2,.cGre,.cNeu,.cRed,.cRe2 {background-color: #d3ffa5;}");


  } else {

    g_fn_prtlin( "    .cGr2        { background-color:#66ff33; }");
  /*   g_fn_prtlin( "    .cGre        { background-color:#84ff98; }"); */
    g_fn_prtlin( "    .cGre        { background-color:#a8ff98; }");
/*   g_fn_prtlin("    .cNeu        { background-color:#e1ddc3; }"); */
    g_fn_prtlin("    .cNeu        { background-color:#e5e2c7; }");
    g_fn_prtlin( "    .cRed        { background-color:#ff98a8; }");
    g_fn_prtlin( "    .cRe2        { background-color:#ff4477; }");
  }


    g_fn_prtlin( "    .cHed        { background-color:#fcfce0; }");
    g_fn_prtlin( "    .cNam        { color:#3f3ffa;");
    g_fn_prtlin( "                   background-color: #F7ebd1;");
    g_fn_prtlin( "                   font-size: 133%;");
    g_fn_prtlin( "    }");
    g_fn_prtlin( "    .cNam2       { color:#3f3ffa; }");

    g_fn_prtlin( "  </style>");

    /* put in favicon */
    g_fn_prtlin("<link href=\"data:image/x-icon;base64,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\" rel=\"icon\" type=\"image/x-icon\" />");

    
    
    // old favicon     g_fn_prtlin("<link href=\"data:image/x-icon;base64,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\" rel=\"icon\" type=\"image/x-icon\" />");

    g_fn_prtlin( "</head>");
    g_fn_prtlin( " ");
    g_fn_prtlin("\n<body>");

}  /* end of  put_top_of_html_group_rpt() */


/* output the css, headings etc.
*/
void put_top_of_just2_group_rpt(void)  /* just_2 rpt */
{
  int i;
/* tn();trn("in put_top_of_just2_group_rpt()"); */

  /* 1. read until [beg_topinfo1]  (name)  (skipping [beg_program])
  */
  for (i=0; ; i++) {
    g_docin_get(doclin);
    if (strstr(doclin, "[beg_topinfo1]") != NULL) break;
  }
  /* then save lines until graph until [end_topinfo1] 
  * then put out html 
  */
  for (i=0; ; i++) {
    g_docin_get(doclin);
    if (strstr(doclin, "[end_topinfo1]") != NULL) break;
    strcpy(arr(i), doclin);
  }

/*   at end, change to STRICT  */
  g_fn_prtlin( "<!doctype html public \"-//w3c//dtd html 4.01 transitional//en\" ");
  g_fn_prtlin( "  \"http://www.w3.org/TR/html4/loose.dtd\">");

  g_fn_prtlin( "<html>");
  g_fn_prtlin( "\n<head>");




  /* HTML HEAD <TITLE>  this appears in browser tab and tooltip when hover
  */


/*   sprintf(writebuf, "  <title>%s+%s Compatibility, produced by iPhone app %s.</title>",arr(0),arr(1), APP_NAME); */
/*   g_fn_prtlin(writebuf); */


  /* if HTML filename, gbl_ffnameHTML, has any slashes, grab the basename
  */
  char myBaseName[256], *myptr;
  if (sfind(gbl_gfnameHTML, '/')) {
    myptr = strrchr(gbl_gfnameHTML, '/');
    strcpy(myBaseName, myptr + 1);
  } else {
    strcpy(myBaseName, gbl_gfnameHTML);
  }
  sprintf(writebuf, "  <title>%s</title>", myBaseName);
  g_fn_prtlin(writebuf);
  


  /* HEAD  META
  */
  sprintf(writebuf, "  <meta name=\"description\" content=\"Report of compatibility of 2 people produced by iPhone app %s\"> ", APP_NAME);
  g_fn_prtlin(writebuf);


/*   g_fn_prtlin( "  <meta http-equiv=\"Content-Type\" content=\"text/html\"; charset=\"iso-8859-1\">");  */
  g_fn_prtlin( "  <meta http-equiv=\"Content-Type\" content=\"text/html\"; charset=\"UTF-8\">"); 
/*   g_fn_prtlin( "  <meta name=\"Author\" content=\"Author goes here\">"); */


/*   g_fn_prtlin( "  <meta name=\"keywords\" content=\"group,group member,compatibility,year in the life,astrology,future,personality,GMCR\"> "); */
/*   g_fn_prtlin( "  <meta name=\"keywords\" content=\"BFF,astrology,compatibility,group,best,match,calendar,year,stress,personality\"> "); */
/*   g_fn_prtlin( "  <meta name=\"keywords\" content=\"measure,group,member,best,match,calendar,year,passionate,personality\"> "); */
/*   g_fn_prtlin( "  <meta name=\"keywords\" content=\"women,woman,female,BFF,compatibility,group,best,match,personality,stress,calendar,year\"> "); */ /* 86 chars */ 
  g_fn_prtlin( "  <meta name=\"keywords\" content=\"women,woman,female,BFF,me,compatibility,group,best,match,personality,stress,calendar,year\"> ");  /* 89 chars */

  /* get rid of CHROME translate "this page is in Galician" 
  * do you want to translate?
  */
  g_fn_prtlin("  <meta name=\"google\" content=\"notranslate\">");
  g_fn_prtlin("  <meta http-equiv=\"Content-Language\" content=\"en\" />");


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
  g_fn_prtlin("  <meta name=\"viewport\" content=\"width=device-width\" />");
  g_fn_prtlin("  <meta name = \"format-detection\" content = \"telephone=no\">");


  /* HEAD   STYLE/CSS
  */
  g_fn_prtlin( "\n  <style type=\"text/css\">");
  g_fn_prtlin( "    @media print { TABLE { font-size: 50%; } }");

  g_fn_prtlin( "    BODY {");
/*  g_fn_prtlin( "      background-color: #F5EFCF;"); */
  g_fn_prtlin( "      background-color: #f7ebd1;");

/*   g_fn_prtlin( "      font-family: Trebuchet MS, Arial, Verdana, sans-serif;"); */
  g_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");

  g_fn_prtlin( "      font-size:   medium;");
  g_fn_prtlin( "      font-weight: normal;");
  g_fn_prtlin( "      text-align:  center;");
/*   g_fn_prtlin( "    <!-- "); */
/*   g_fn_prtlin( "      background-image: url('mkgif1g.gif');"); */
/*   g_fn_prtlin( "    --> "); */
  g_fn_prtlin( "    }");

/*   g_fn_prtlin( "    H1 { font-size: 137%; font-weight: bold;   line-height: 95%; text-align: center;}"); */
/*   g_fn_prtlin( "    H2 { font-size: 137%; font-weight: bold;   line-height: 25%; text-align: center;}"); */
/*   g_fn_prtlin( "    H3 { font-size: 110%; font-weight: normal; line-height: 30%; text-align: center;}"); */
/*   g_fn_prtlin( "    H1 { font-size: 137%; font-weight: bold;   line-height: 15%; text-align: center;}"); */
  g_fn_prtlin( "    H1 { font-size: 137%; font-weight: bold;   line-height: 100%; text-align: center;}");
  g_fn_prtlin( "    H2 { font-size: 120%; font-weight: bold;   line-height: 95%; text-align: center;}");
  g_fn_prtlin( "    H3 { font-size: 110%; font-weight: normal; line-height: 95%; text-align: center;}");

  g_fn_prtlin( "    H4 { font-size:  75%; font-weight: bold;   line-height: 30%; text-align: center;}");
  g_fn_prtlin( "    H5 { font-size:  70%; font-weight: normal; line-height: 30%; text-align: center;}");


/*   g_fn_prtlin( "    H4 { font-size:  85%; font-weight: bold;   line-height: 30%; text-align: center;}"); */
/*   g_fn_prtlin( "    H5 { font-size:  55%; font-weight: normal; line-height: 90%; text-align: center;}"); */
/*   g_fn_prtlin( "    H5 { font-size:  70%; font-weight: normal; line-height: 30%; text-align: center;}"); */

  g_fn_prtlin( "    PRE {");
/*   g_fn_prtlin( "      padding: 1%;"); */
  g_fn_prtlin( "      display: inline-block;");
/*   g_fn_prtlin( "      border-style: solid;"); */

/*   g_fn_prtlin( "      border-color: black;"); */
/*   g_fn_prtlin( "      border-width: 2px;"); */
/*   g_fn_prtlin( "      border-color: #e4dfae;"); */
/*   g_fn_prtlin( "      border-width: 5px;"); */

/*   g_fn_prtlin( "      display: inline-block;"); */
  g_fn_prtlin( "      display: inline;");

  g_fn_prtlin( "      background-color: #fcfce0;");
/*   g_fn_prtlin( "      border: none;"); */
/*   g_fn_prtlin( "      border-collapse: collapse;"); */
/*   g_fn_prtlin( "      border-spacing: 0;"); */
/*       border-collapse: collapse; */
/*   g_fn_prtlin( "      font-family: Andale Mono, Monospace, Courier New;"); */
/*   g_fn_prtlin( "      font-weight: normal;"); */
/*   g_fn_prtlin( "      font-size:   65%;"); */
/*   g_fn_prtlin( "      font-size:   75%;"); */
/*   g_fn_prtlin( "      line-height: 70%;"); */
/*   g_fn_prtlin( "      line-height: 100%;"); */
  g_fn_prtlin( "      margin:0 auto;");
  g_fn_prtlin( "    }");

/*   g_fn_prtlin( "      padding-left: 5%;"); */
/*   g_fn_prtlin( "      padding-right:5%;"); */
/*   g_fn_prtlin( "      text-align: center;"); */
/*   g_fn_prtlin( "    P { "); */
/*   g_fn_prtlin( "      font-family: Andale Mono, Menlo, Monospace, Courier New;"); */
/*   g_fn_prtlin( "      width: auto;"); */
/*   g_fn_prtlin( "      font-size:   80%;"); */
/*   g_fn_prtlin( "      margin-top: 0;"); */
/*   g_fn_prtlin( "      margin-bottom: 0;"); */
/*   g_fn_prtlin( "      margin-left: auto;"); */
/*   g_fn_prtlin( "      margin-right:auto;"); */
/*   g_fn_prtlin( "      text-align: left;"); */
/*   g_fn_prtlin( "    }"); */

  g_fn_prtlin( "    P { ");
  g_fn_prtlin( "      display: inline;");
  g_fn_prtlin( "      margin:0 auto;");
  g_fn_prtlin( "      background-color: #f7ebd1;");
  g_fn_prtlin( "    }");


/* for table: */
/*       border: 2px solid black; */
/*       cellspacing: 0; */
/*       border-top: 0; */
/*       border-bottom: 0; */

/* TABLE TABLE TABLE TABLE TABLE TABLE TABLE TABLE */
/*   g_fn_prtlin( "    table {");
*   g_fn_prtlin( "      border-collapse: collapse;");
*   g_fn_prtlin( "      border-spacing: 0;");
*   g_fn_prtlin( "    }");
*   g_fn_prtlin( "    table.center {");
*   g_fn_prtlin( "      margin-left:auto;");
*   g_fn_prtlin( "      margin-right:auto;");
*   g_fn_prtlin( "    }");
*   g_fn_prtlin( "    TD {");
*   g_fn_prtlin( "      white-space: nowrap;");
*   g_fn_prtlin( "      padding: 0;");
*   g_fn_prtlin( "    }");
*/
                               /* new stuff for bottom TABLE */
  g_fn_prtlin( "    table {");
/*   g_fn_prtlin( "      font-family: Andale Mono, Menlo, Monospace, Courier New;"); */
  g_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");
  g_fn_prtlin( "      text-align: left;");

/*   g_fn_prtlin( "      border: 1px solid black;"); */
  g_fn_prtlin( "      border-collapse: collapse;");
  g_fn_prtlin( "      border-spacing: 0;");

/*   g_fn_prtlin( "      padding-right:2%;"); */
/*   g_fn_prtlin( "      padding-left:2%;"); */
  g_fn_prtlin( "      margin-left: auto;");
  g_fn_prtlin( "      margin-right:auto;");
  g_fn_prtlin( "    }");
  g_fn_prtlin( "    td {");
/*   g_fn_prtlin( "      font-family: Andale Mono, Menlo, Monospace, Courier New;"); */
  g_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");
  g_fn_prtlin( "      white-space: pre;");
  g_fn_prtlin( "      font-size: 90%;");
  g_fn_prtlin( "      text-align: left;");

/*   g_fn_prtlin( "      border: 1px solid black;"); */
  g_fn_prtlin( "      border-collapse: collapse;");
  g_fn_prtlin( "      border-spacing: 0;");

/*   g_fn_prtlin( "      padding-left: 10px; "); */
/*   g_fn_prtlin( "      padding-right: 10px; "); */
  g_fn_prtlin( "      padding-left: 5px; ");
  g_fn_prtlin( "      padding-right: 5px; ");

  g_fn_prtlin( "    }");
  g_fn_prtlin( "    table.center {");
  g_fn_prtlin( "      margin-left:auto;");
  g_fn_prtlin( "      margin-right:auto;");
  g_fn_prtlin( "    }");
  g_fn_prtlin( "    th {");

  g_fn_prtlin( "      padding: 5px; ");
  g_fn_prtlin( "      vertical-align:bottom;");

/*   g_fn_prtlin( "      font-family: Trebuchet MS, Arial, Verdana, sans-serif;"); */
  g_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");
  g_fn_prtlin( "      font-size: 90%;");
/*   g_fn_prtlin( "      padding-left: 10px; "); */
/*   g_fn_prtlin( "      padding-right: 10px; "); */

/*   g_fn_prtlin( "      background-color: #e1fdc3 ;"); */
     g_fn_prtlin( "      background-color: #fcfce0 ;");

/*   g_fn_prtlin( "      border: 1px solid black;"); */
  g_fn_prtlin( "      border-spacing: 0;");

  g_fn_prtlin( "      text-align: center;");
  g_fn_prtlin( "    }");
  g_fn_prtlin( "    table td  { text-align: right; }");
  g_fn_prtlin( "    table td+td { text-align: left; }");

/*   g_fn_prtlin( "    table td+td+td { text-align: right; }"); */
  g_fn_prtlin( "    table td+td+td { text-align: right; ");
/*   g_fn_prtlin( "      padding-right:3%;"); */
  g_fn_prtlin( "    }");

  g_fn_prtlin( "    table td+td+td+td { text-align: left; }");

  g_fn_prtlin( "    .row4        { background-color:#f8f0c0; }");

/*   p_fn_prtlin( "    .cGr2        { background-color:#d0fda0; }"); */
/*   g_fn_prtlin( "    .cGr2        { background-color:#8bfd87; }"); */
/*   g_fn_prtlin( "    .cGr2        { background-color:#66ff33; }"); */

/*   p_fn_prtlin( "    .cGre        { background-color:#e1fdc3; }"); */
/*   g_fn_prtlin( "    .cGre        { background-color:#ccffb9; }"); */
/*   p_fn_prtlin( "    .cRed        { background-color:#ffd9d9; }"); */
/*   g_fn_prtlin( "    .cRed        { background-color:#ffcccc; }"); */
/*   p_fn_prtlin( "    .cRe2        { background-color:#fcb9b9; }"); */
/*   g_fn_prtlin( "    .cRe2        { background-color:#fc8888; }"); */
/*   g_fn_prtlin( "    .cRe2        { background-color:#fc6094; }"); */
/*   g_fn_prtlin( "    .cRe2        { background-color:#ff3366; }"); */


  g_fn_prtlin( "    .cGr2        { background-color:#66ff33; }");
/*   g_fn_prtlin( "    .cGre        { background-color:#84ff98; }"); */
  g_fn_prtlin( "    .cGre        { background-color:#a8ff98; }");
  g_fn_prtlin( "    .cRed        { background-color:#ff98a8; }");
  g_fn_prtlin( "    .cRe2        { background-color:#ff4477; }");


/*   g_fn_prtlin("    .cNeu        { background-color:#e1ddc3; }"); */
  g_fn_prtlin("    .cNeu        { background-color:#e5e2c7; }");

  g_fn_prtlin( "    .cHed        { background-color:#fcfce0; }");
  g_fn_prtlin( "    .cNam        { color:#3f3ffa;");
  g_fn_prtlin( "                   background-color: #F7ebd1;");
  g_fn_prtlin( "                   font-size: 133%;");
  g_fn_prtlin( "    }");
  g_fn_prtlin( "    .cNam2       { color:#3f3ffa; }");
  g_fn_prtlin( "    .cCat        { background-color:#fdfbe1; }");
/* TABLE TABLE TABLE TABLE TABLE TABLE TABLE TABLE */

  g_fn_prtlin( "  </style>");

/*   g_fn_prtlin( "    <!-- "); */
/*   g_fn_prtlin( "    PRE {line-height: 68%}; "); */
/*   g_p_fn_prtlin( "    P {margin-left:10%; margin-right:10%}"); */
/*   g_p_fn_prtlin( "    --> "); */

    
    /* put in favicon */
    g_fn_prtlin("<link href=\"data:image/x-icon;base64,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\" rel=\"icon\" type=\"image/x-icon\" />");

    
    
    
/* put in favicon */
    // old favicon    g_fn_prtlin("<link href=\"data:image/x-icon;base64,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\" rel=\"icon\" type=\"image/x-icon\" />");

  g_fn_prtlin( "</head>");
  g_fn_prtlin( " ");
  g_fn_prtlin("\n<body>");

  g_fn_prtlin("<h1>Compatibility Potential of </h1>");
  sprintf(writebuf,
    "<h1>%s and %s</h1>",
//    "<h1><span class=\"cNam\">%s</span> and <span class=\"cNam\">%s</span></h1>",
    arr(0), arr(1)
  );
  g_fn_prtlin(writebuf);
 
  g_fn_prtlin(" ");

} /* end of put_top_of_just2_group_rpt() */


/* arg in_docin_last_idx  is pointing at the last line written.
* Therefore, the current docin_lines written
* run from index = 0 to index = arg in_docin_last_idx.
*/
void g_docin_get(char *in_line)
{
  
/* if (rkdb == 1) {
*   tr("in g_docin_get");  ksn(in_line);
* }
*/
  if (is_first_g_docin_get == 1) g_global_read_idx = 0;
  else                           g_global_read_idx++;
  
  is_first_g_docin_get = 0;  /* set to false */

  if (g_global_read_idx > g_global_max_docin_idx) {
    g_docin_free();
    rkabort("Error. grphtm.c walked off end of docin_lines array");
  }

  strcpy(in_line, g_global_docin_lines[g_global_read_idx] );

  scharout(in_line,'\n');   /* remove newlines */

} /* end of g_docin_get */



void g_fn_prtlin(char *lin) {
  char myEOL[8];


  if (  strcmp(global_instructions, "return only html for table in string") == 0
     || strcmp(global_instructions, "return only compatibility score"     ) == 0 ) {

    return;  /* do not write anything */

  } else {


// int dbsw;
// if (strstr(lin, "xx    ") != NULL) {dbsw = 1; tn();tn();}
 //if (dbsw == 1) {ksn(lin);}

    strcpy(myEOL, "\n");
    if (GBL_HTML_HAS_NEWLINES == 1)     strcpy(myEOL, "\n");
    if (GBL_HTML_HAS_NEWLINES == 0) {
      /* scharout(lin,'\n'); */  /* remove newlines */
      if (gbl_we_are_in_PRE_block == 1) strcpy(myEOL, "<br>");
      else                              strcpy(myEOL, "");
    }


    /* g_global_n = sprintf(g_global_p,"%s\n", lin); */
    g_global_n = sprintf(g_global_p,"%s%s", lin, myEOL);
    fput(g_global_p, g_global_n, Fp_g_HTML_file);
  }
} 



/* show good      "stars" in graph as green line
*  show difficult "stars" in graph as red   line
*  Must change all "*" to " ";
*/
void g_fn_prtlin_stars(char *starline)
{
  char *pBegStar;
  char *pEndStar;
  char beforeStars[512], mycolor[8], allStars[512], afterStars[512];
  /* remember these incase the stars wrap to line 2,3, etc
  */
  static char current_star_type[16];  /* "good" or "difficult" */

//ksn(starline);

  if (sall(starline, " ") == 1) {
    return;
  }

  if (strstr(starline, "asy")      != NULL) {
    strcpy(current_star_type, "easy");
  }
  if (strstr(starline, "ifficult") != NULL) {
    strcpy(current_star_type, "difficult");
  }
  if (strcmp(current_star_type, "easy") == 0) {
/*     strcpy(mycolor, "cGr2");  */
    strcpy(mycolor, "cGre"); /* easy */
  }
  if (strcmp(current_star_type, "difficult") == 0) {
/*     strcpy(mycolor, "cRe2"); */
    strcpy(mycolor, "cRed");
  }
  /*    .cGre        { background-color:#e1fdc3; }
  *     .cRed        { background-color:#ffd9d9; }
  *     .cLineGood   { background-color:#c3fdc3; }
  *     .cLineStress { background-color:#ffbac7; }
  */

  /* write star line here
  */

  /*   pBegStar = strchr (starline, '*'); */
  /*   pEndStar = strrchr(starline, '*'); */

  /* NOTICE: "stars" are now '+'  or  '-' */

  /* find out if line has +  or  -  */
  /* returns 1 if it finds a char c in string s else, returns 0 */
  /* int sfind(char s[], char c)  */

  if (sfind(starline, '+')) {
    pBegStar = strchr (starline, '+');
    pEndStar = strrchr(starline, '+');
  } else if (sfind(starline, '-')) {
    pBegStar = strchr (starline, '-');
    pEndStar = strrchr(starline, '-');
  } else {
    return;  /* no "stars" */
  }

  mkstr(beforeStars, starline, pBegStar - 1);
  mkstr(allStars,    pBegStar, pEndStar);
  mkstr(afterStars,  pEndStar + 1, starline + strlen(starline) - 1);

  sprintf(writebuf,
    " %s<span class=\"%s\">%s</span>%s",
    beforeStars + 12,
    mycolor,
    allStars,
    afterStars
  ); 
  g_fn_prtlin(writebuf);  

  
/*   sprintf(writebuf,
*     "  <span class=\"%s\">%s</span>",
*     mycolor,
*     starline + 13   * skip easy/difficult *
*   ); 
*   g_fn_prtlin(writebuf);  
*/

} /* end of g_fn_prtlin_stars() */



// iiiiiiiiiiiiiiiiiiiiiiiiiiiiiii   webview / ios  html code  iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii
// iiiiiiiiiiiiiiiiiiiiiiiiiiiiiii   webview / ios  html code  iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii
// iiiiiiiiiiiiiiiiiiiiiiiiiiiiiii   webview / ios  html code  iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii


/* output the css, headings etc.
*/
void put_ios_top_of_just2_group_rpt(void)  /* just_2 rpt */
{
  int i;
/* tn();trn("in put_top_of_just2_group_rpt()"); */

  /* 1. read until [beg_topinfo1]  (name)  (skipping [beg_program])
  */
  for (i=0; ; i++) {
    g_docin_get(doclin);
    if (strstr(doclin, "[beg_topinfo1]") != NULL) break;
  }
  /* then save lines until graph until [end_topinfo1] 
  * then put out html 
  */
  for (i=0; ; i++) {
    g_docin_get(doclin);
    if (strstr(doclin, "[end_topinfo1]") != NULL) break;
    strcpy(arr(i), doclin);
  }

/*   at end, change to STRICT  */
  g_fn_prtlin( "<!doctype html public \"-//w3c//dtd html 4.01 transitional//en\" ");
  g_fn_prtlin( "  \"http://www.w3.org/TR/html4/loose.dtd\">");

  g_fn_prtlin( "<html>");
  g_fn_prtlin( "\n<head>");




  /* HTML HEAD <TITLE>  this appears in browser tab and tooltip when hover
  */


/*   sprintf(writebuf, "  <title>%s+%s Compatibility, produced by iPhone app %s.</title>",arr(0),arr(1), APP_NAME); */
/*   g_fn_prtlin(writebuf); */


  /* if HTML filename, gbl_ffnameHTML, has any slashes, grab the basename
  */
  char myBaseName[256], *myptr;
  if (sfind(gbl_gfnameHTML, '/')) {
    myptr = strrchr(gbl_gfnameHTML, '/');
    strcpy(myBaseName, myptr + 1);
  } else {
    strcpy(myBaseName, gbl_gfnameHTML);
  }
  sprintf(writebuf, "  <title>%s</title>", myBaseName);
  g_fn_prtlin(writebuf);
  


  /* HEAD  META
  */
  sprintf(writebuf, "  <meta name=\"description\" content=\"Report of compatibility of 2 people produced by iPhone app %s\"> ", APP_NAME);
  g_fn_prtlin(writebuf);


/*   g_fn_prtlin( "  <meta http-equiv=\"Content-Type\" content=\"text/html\"; charset=\"iso-8859-1\">");  */
  g_fn_prtlin( "  <meta http-equiv=\"Content-Type\" content=\"text/html\"; charset=\"UTF-8\">"); 
/*   g_fn_prtlin( "  <meta name=\"Author\" content=\"Author goes here\">"); */


/*   g_fn_prtlin( "  <meta name=\"keywords\" content=\"group,group member,compatibility,year in the life,astrology,future,personality,GMCR\"> "); */
/*   g_fn_prtlin( "  <meta name=\"keywords\" content=\"BFF,astrology,compatibility,group,best,match,calendar,year,stress,personality\"> "); */
/*   g_fn_prtlin( "  <meta name=\"keywords\" content=\"measure,group,member,best,match,calendar,year,passionate,personality\"> "); */
/*   g_fn_prtlin( "  <meta name=\"keywords\" content=\"women,woman,female,BFF,compatibility,group,best,match,personality,stress,calendar,year\"> "); */ /* 86 chars */ 
  g_fn_prtlin( "  <meta name=\"keywords\" content=\"women,woman,female,BFF,me,compatibility,group,best,match,personality,stress,calendar,year\"> ");  /* 89 chars */

  /* get rid of CHROME translate "this page is in Galician" 
  * do you want to translate?
  */
  g_fn_prtlin("  <meta name=\"google\" content=\"notranslate\">");
  g_fn_prtlin("  <meta http-equiv=\"Content-Language\" content=\"en\" />");


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
  g_fn_prtlin("  <meta name=\"viewport\" content=\"width=device-width\" />");
  g_fn_prtlin("  <meta name = \"format-detection\" content = \"telephone=no\">");


  /* HEAD   STYLE/CSS
  */
  g_fn_prtlin( "\n  <style type=\"text/css\">");
/* <.> */
  g_fn_prtlin( "    @media print { TABLE { font-size: 50%; } }");

  g_fn_prtlin( "    BODY {");
/*  g_fn_prtlin( "      background-color: #F5EFCF;"); */
  g_fn_prtlin( "      background-color: #f7ebd1;");

/*   g_fn_prtlin( "      font-family: Trebuchet MS, Arial, Verdana, sans-serif;"); */
  g_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");

  g_fn_prtlin( "      font-size:   medium;");
  g_fn_prtlin( "      font-weight: normal;");
  g_fn_prtlin( "      text-align:  center;");
/*   g_fn_prtlin( "    <!-- "); */
/*   g_fn_prtlin( "      background-image: url('mkgif1g.gif');"); */
/*   g_fn_prtlin( "    --> "); */
  g_fn_prtlin( "    }");

/*   g_fn_prtlin( "    H1 { font-size: 137%; font-weight: bold;   line-height: 95%; text-align: center;}"); */
/*   g_fn_prtlin( "    H2 { font-size: 137%; font-weight: bold;   line-height: 25%; text-align: center;}"); */
/*   g_fn_prtlin( "    H3 { font-size: 110%; font-weight: normal; line-height: 30%; text-align: center;}"); */
/*   g_fn_prtlin( "    H1 { font-size: 137%; font-weight: bold;   line-height: 15%; text-align: center;}"); */
  g_fn_prtlin( "    H1 { font-size: 137%; font-weight: bold;   line-height: 100%; text-align: center;}");
  g_fn_prtlin( "    H2 { font-size: 120%; font-weight: bold;   line-height: 95%; text-align: center;}");
  g_fn_prtlin( "    H3 { font-size: 110%; font-weight: normal; line-height: 95%; text-align: center;}");

  g_fn_prtlin( "    H4 { font-size:  75%; font-weight: bold;   line-height: 30%; text-align: center;}");
  g_fn_prtlin( "    H5 { font-size:  70%; font-weight: normal; line-height: 30%; text-align: center;}");


/*   g_fn_prtlin( "    H4 { font-size:  85%; font-weight: bold;   line-height: 30%; text-align: center;}"); */
/*   g_fn_prtlin( "    H5 { font-size:  55%; font-weight: normal; line-height: 90%; text-align: center;}"); */
/*   g_fn_prtlin( "    H5 { font-size:  70%; font-weight: normal; line-height: 30%; text-align: center;}"); */

  g_fn_prtlin( "    PRE {");
  g_fn_prtlin( "      display: inline;");   // NEEDED ?
  g_fn_prtlin( "      background-color: #fcfce0;");
//  g_fn_prtlin( "      font-size: 0.6em;");         // GOLD order #3
  g_fn_prtlin( "    }");
/*   g_fn_prtlin( "      padding: 1%;"); */
//  g_fn_prtlin( "      display: inline-block;");
/*   g_fn_prtlin( "      border-style: solid;"); */

/*   g_fn_prtlin( "      border-color: black;"); */
/*   g_fn_prtlin( "      border-width: 2px;"); */
/*   g_fn_prtlin( "      border-color: #e4dfae;"); */
/*   g_fn_prtlin( "      border-width: 5px;"); */

/*   g_fn_prtlin( "      display: inline-block;"); */

/*   g_fn_prtlin( "      border: none;"); */
/*   g_fn_prtlin( "      border-collapse: collapse;"); */
/*   g_fn_prtlin( "      border-spacing: 0;"); */
/*       border-collapse: collapse; */
/*   g_fn_prtlin( "      font-family: Andale Mono, Monospace, Courier New;"); */
/*   g_fn_prtlin( "      font-weight: normal;"); */
/*   g_fn_prtlin( "      font-size:   65%;"); */
/*   g_fn_prtlin( "      font-size:   75%;"); */
/*   g_fn_prtlin( "      line-height: 70%;"); */
/*   g_fn_prtlin( "      line-height: 100%;"); */
//  g_fn_prtlin( "      margin:0 auto;");

/*   g_fn_prtlin( "      padding-left: 5%;"); */
/*   g_fn_prtlin( "      padding-right:5%;"); */
/*   g_fn_prtlin( "      text-align: center;"); */
/*   g_fn_prtlin( "    P { "); */
/*   g_fn_prtlin( "      font-family: Andale Mono, Menlo, Monospace, Courier New;"); */
/*   g_fn_prtlin( "      width: auto;"); */
/*   g_fn_prtlin( "      font-size:   80%;"); */
/*   g_fn_prtlin( "      margin-top: 0;"); */
/*   g_fn_prtlin( "      margin-bottom: 0;"); */
/*   g_fn_prtlin( "      margin-left: auto;"); */
/*   g_fn_prtlin( "      margin-right:auto;"); */
/*   g_fn_prtlin( "      text-align: left;"); */
/*   g_fn_prtlin( "    }"); */

  g_fn_prtlin( "    .myTitle {");
  g_fn_prtlin( "      margin-top: 0.2em;");
//  g_fn_prtlin( "      margin-bottom: 1.2em;");
  g_fn_prtlin( "      margin-bottom: 0.2em;");
//  g_fn_prtlin( "      margin-left: 2em;");
//  f_fn_prtlin( "      text-align: center;");      // GOLD order #1
  g_fn_prtlin( "      text-align: left;");      // GOLD order #1
     // are putting spaces in code to center
//  g_fn_prtlin( "      width: 300%m;");             // GOLD order #2
  g_fn_prtlin( "      font-size: 3.0em;");         // GOLD order #3
  g_fn_prtlin( "      font-weight: bold;"); 
  g_fn_prtlin( "      background-color: #F7ebd1;");
  g_fn_prtlin( "      white-space: pre ; display: block; unicode-bidi: embed");
  g_fn_prtlin( "    }");

  g_fn_prtlin( "    .checkoutbestmatch {");
  //g_fn_prtlin( "      background-color: #fcfce0;");
  g_fn_prtlin( "      margin-left: 5.5em;");
  g_fn_prtlin( "      text-align: left;");      // GOLD order #1
//  g_fn_prtlin( "      width: 240%;");             // GOLD order #2  *BUT* width  AFFECTS OTHER BLOCKS
  g_fn_prtlin( "      font-size: 1.0em;");  /* gold order #3 */
  //g_fn_prtlin( "      line-height: 130%;");  /* <.> */
  g_fn_prtlin( "      margin-bottom: 3em;");  // MUST BE ABOVE "white-space: pre;"
  g_fn_prtlin( "      white-space: pre ; display: block; unicode-bidi: embed");
  g_fn_prtlin( "    }");

  g_fn_prtlin( "    .explproportion{");
//  g_fn_prtlin( "      background-color: #fcfce0;");

  g_fn_prtlin( "      padding-top: 0;");
  g_fn_prtlin( "      padding-bottom: 0;");
  g_fn_prtlin( "      margin-top: 0.1em;");
  g_fn_prtlin( "      margin-left: 1em;");
  g_fn_prtlin( "      text-align: left;");      // GOLD order #1
//  g_fn_prtlin( "      width: 240%;");             // GOLD order #2  *BUT* width  AFFECTS OTHER BLOCKS
  g_fn_prtlin( "      font-size: 1.0em;");  /* gold order #3 */
  //g_fn_prtlin( "      line-height: 130%;");  /* <.> */
  g_fn_prtlin( "      margin-bottom: 0.1em;");  // MUST BE ABOVE "white-space: pre;"
  g_fn_prtlin( "      white-space: pre ; display: block; unicode-bidi: embed");
  g_fn_prtlin( "    }");


  g_fn_prtlin( "    .categories{");
//  g_fn_prtlin( "      background-color: #fcfce0;");
  g_fn_prtlin( "      padding-top: 0;");
  g_fn_prtlin( "      margin-top: -1.2em;");
  g_fn_prtlin( "      margin-left: 0.5em;");
  g_fn_prtlin( "      text-align: left;");      // GOLD order #1
//  g_fn_prtlin( "      width: 240%;");             // GOLD order #2  *BUT* width  AFFECTS OTHER BLOCKS

  g_fn_prtlin( "      font-size: 0.8em;");  /* gold order #3 */
//  g_fn_prtlin( "      font-size: 0.8em;");  /* gold order #3 */
  //g_fn_prtlin( "      line-height: 130%;");  /* <.> */
  g_fn_prtlin( "      margin-bottom: 3em;");  // MUST BE ABOVE "white-space: pre;"
  g_fn_prtlin( "      white-space: pre ; display: block; unicode-bidi: embed");
  g_fn_prtlin( "    }");


  g_fn_prtlin( "    P { ");
  g_fn_prtlin( "      display: inline;");
  g_fn_prtlin( "      margin:0 auto;");
  g_fn_prtlin( "      background-color: #f7ebd1;");
  g_fn_prtlin( "    }");

  g_fn_prtlin( "    .explpotential{");
//  g_fn_prtlin( "      background-color: #fcfce0;");
  g_fn_prtlin( "      padding-top: 0;");
  g_fn_prtlin( "      padding-bottom: 0;");
  g_fn_prtlin( "      margin-top: -4.3em;");
  g_fn_prtlin( "      margin-left: 0.3em;");
  g_fn_prtlin( "      text-align: left;");      // GOLD order #1
//  g_fn_prtlin( "      width: 240%;");             // GOLD order #2  *BUT* width  AFFECTS OTHER BLOCKS

  g_fn_prtlin( "      font-size: 1.0em;");  /* gold order #3 */
//  g_fn_prtlin( "      font-size: 1.0em;");  /* gold order #3 */
  //g_fn_prtlin( "      line-height: 130%;");  /* <.> */
  g_fn_prtlin( "      margin-bottom: 0.1em;");  // MUST BE ABOVE "white-space: pre;"
  g_fn_prtlin( "      white-space: pre ; display: block; unicode-bidi: embed");
  g_fn_prtlin( "    }");


  g_fn_prtlin( "    .explrelationship{");
//  g_fn_prtlin( "      background-color: #fcfce0;");
  g_fn_prtlin( "      padding-top: 0;");
  g_fn_prtlin( "      padding-bottom: 0;");
  g_fn_prtlin( "      margin-top: 2em;");
  g_fn_prtlin( "      margin-left: 3.8em;");
  g_fn_prtlin( "      text-align: left;");      // GOLD order #1
//  g_fn_prtlin( "      width: 240%;");             // GOLD order #2  *BUT* width  AFFECTS OTHER BLOCKS
  g_fn_prtlin( "      font-size: 1.0em;");  /* gold order #3 */
  //g_fn_prtlin( "      line-height: 130%;");  /* <.> */
  g_fn_prtlin( "      margin-bottom: 0.1em;");  // MUST BE ABOVE "white-space: pre;"
  g_fn_prtlin( "      white-space: pre ; display: block; unicode-bidi: embed");
  g_fn_prtlin( "    }");

  g_fn_prtlin( "    .appby{");
//  g_fn_prtlin( "      background-color: #fcfce0;");
  g_fn_prtlin( "      padding-top: 0;");
  g_fn_prtlin( "      padding-bottom: 0;");
  g_fn_prtlin( "      margin-top: 1.5em;");
  g_fn_prtlin( "      margin-left: 12em;");
  g_fn_prtlin( "      text-align: left;");      // GOLD order #1
//  g_fn_prtlin( "      width: 240%;");             // GOLD order #2  *BUT* width  AFFECTS OTHER BLOCKS
  g_fn_prtlin( "      font-size: 0.8em;");  /* gold order #3 */
  //g_fn_prtlin( "      line-height: 130%;");  /* <.> */
  g_fn_prtlin( "      margin-bottom: 0.1em;");  // MUST BE ABOVE "white-space: pre;"
  g_fn_prtlin( "      white-space: pre ; display: block; unicode-bidi: embed");
  g_fn_prtlin( "    }");

  g_fn_prtlin( "    .entertainment {");
  g_fn_prtlin( "      margin-left: 9em;");
  g_fn_prtlin( "      text-align: left;");
//  g_fn_prtlin( "      width: 150%;");             // GOLD order #2
//  g_fn_prtlin( "      background-color: #F7ebd1;");
  g_fn_prtlin( "      font-size: 0.8em;");
  g_fn_prtlin( "      color:#FF0000;");  // RED print
  g_fn_prtlin( "      white-space: pre ; display: block; unicode-bidi: embed");
/*   p_fn_prtlin( "      font-size: 130%;");  */
  g_fn_prtlin( "    }");


/* for table: */
/*       border: 2px solid black; */
/*       cellspacing: 0; */
/*       border-top: 0; */
/*       border-bottom: 0; */

/* TABLE TABLE TABLE TABLE TABLE TABLE TABLE TABLE */
/*   g_fn_prtlin( "    table {");
*   g_fn_prtlin( "      border-collapse: collapse;");
*   g_fn_prtlin( "      border-spacing: 0;");
*   g_fn_prtlin( "    }");
*   g_fn_prtlin( "    table.center {");
*   g_fn_prtlin( "      margin-left:auto;");
*   g_fn_prtlin( "      margin-right:auto;");
*   g_fn_prtlin( "    }");
*   g_fn_prtlin( "    TD {");
*   g_fn_prtlin( "      white-space: nowrap;");
*   g_fn_prtlin( "      padding: 0;");
*   g_fn_prtlin( "    }");
*/
                               /* new stuff for bottom TABLE */
//   g_fn_prtlin( "    table {");
// /*   g_fn_prtlin( "      font-family: Andale Mono, Menlo, Monospace, Courier New;"); */
//   g_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");
//   g_fn_prtlin( "      text-align: left;");
// /*   g_fn_prtlin( "      border: 1px solid black;"); */
//   g_fn_prtlin( "      border-collapse: collapse;");
//   g_fn_prtlin( "      border-spacing: 0;");
// /*   g_fn_prtlin( "      padding-right:2%;"); */
// /*   g_fn_prtlin( "      padding-left:2%;"); */
//   g_fn_prtlin( "      margin-left: auto;");
//   g_fn_prtlin( "      margin-right:auto;");
//   g_fn_prtlin( "    }");


  g_fn_prtlin( "    table {");

//  g_fn_prtlin( "      margin-left: 2em;");
/* <.> */
  g_fn_prtlin( "      margin-top: 0.1em;");
  g_fn_prtlin( "      margin-bottom: 0.1em;");
  g_fn_prtlin( "      border-spacing: 0;");
  g_fn_prtlin( "      font-size: 1.3em;");
  g_fn_prtlin( "    }");




  g_fn_prtlin( "    td {");
/*   g_fn_prtlin( "      font-family: Andale Mono, Menlo, Monospace, Courier New;"); */
  g_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");
  g_fn_prtlin( "      white-space: pre;");
  g_fn_prtlin( "      font-size: 90%;");
  g_fn_prtlin( "      text-align: left;");

/*   g_fn_prtlin( "      border: 1px solid black;"); */
  g_fn_prtlin( "      border-collapse: collapse;");
  g_fn_prtlin( "      border-spacing: 0;");

/*   g_fn_prtlin( "      padding-left: 10px; "); */
/*   g_fn_prtlin( "      padding-right: 10px; "); */
  g_fn_prtlin( "      padding-left: 5px; ");
  g_fn_prtlin( "      padding-right: 5px; ");

  g_fn_prtlin( "    }");
  g_fn_prtlin( "    table.center {");
  g_fn_prtlin( "      margin-left:auto;");
  g_fn_prtlin( "      margin-right:auto;");
  g_fn_prtlin( "    }");
  g_fn_prtlin( "    th {");

  g_fn_prtlin( "      padding: 5px; ");
  g_fn_prtlin( "      vertical-align:bottom;");

/*   g_fn_prtlin( "      font-family: Trebuchet MS, Arial, Verdana, sans-serif;"); */
  g_fn_prtlin( "      font-family: Menlo, Andale Mono, Monospace, Courier New;");
  g_fn_prtlin( "      font-size: 90%;");
/*   g_fn_prtlin( "      padding-left: 10px; "); */
/*   g_fn_prtlin( "      padding-right: 10px; "); */

/*   g_fn_prtlin( "      background-color: #e1fdc3 ;"); */
     g_fn_prtlin( "      background-color: #fcfce0 ;");

/*   g_fn_prtlin( "      border: 1px solid black;"); */
  g_fn_prtlin( "      border-spacing: 0;");

  g_fn_prtlin( "      text-align: center;");
  g_fn_prtlin( "    }");
  g_fn_prtlin( "    table td  { text-align: right; }");
  g_fn_prtlin( "    table td+td { text-align: left; }");

/*   g_fn_prtlin( "    table td+td+td { text-align: right; }"); */
  g_fn_prtlin( "    table td+td+td { text-align: right; ");
/*   g_fn_prtlin( "      padding-right:3%;"); */
  g_fn_prtlin( "    }");

  g_fn_prtlin( "    table td+td+td+td { text-align: left; }");

  g_fn_prtlin( "    .row4        { background-color:#f8f0c0; }");

/*   p_fn_prtlin( "    .cGr2        { background-color:#d0fda0; }"); */
/*   g_fn_prtlin( "    .cGr2        { background-color:#8bfd87; }"); */
/*   g_fn_prtlin( "    .cGr2        { background-color:#66ff33; }"); */

/*   p_fn_prtlin( "    .cGre        { background-color:#e1fdc3; }"); */
/*   g_fn_prtlin( "    .cGre        { background-color:#ccffb9; }"); */
/*   p_fn_prtlin( "    .cRed        { background-color:#ffd9d9; }"); */
/*   g_fn_prtlin( "    .cRed        { background-color:#ffcccc; }"); */
/*   p_fn_prtlin( "    .cRe2        { background-color:#fcb9b9; }"); */
/*   g_fn_prtlin( "    .cRe2        { background-color:#fc8888; }"); */
/*   g_fn_prtlin( "    .cRe2        { background-color:#fc6094; }"); */
/*   g_fn_prtlin( "    .cRe2        { background-color:#ff3366; }"); */


  g_fn_prtlin( "    .cGr2        { background-color:#66ff33; }");
/*   g_fn_prtlin( "    .cGre        { background-color:#84ff98; }"); */
  g_fn_prtlin( "    .cGre        { background-color:#a8ff98; }");
  g_fn_prtlin( "    .cRed        { background-color:#ff98a8; }");
  g_fn_prtlin( "    .cRe2        { background-color:#ff4477; }");


/*   g_fn_prtlin("    .cNeu        { background-color:#e1ddc3; }"); */
  g_fn_prtlin("    .cNeu        { background-color:#e5e2c7; }");

  g_fn_prtlin( "    .cHed        { background-color:#fcfce0; }");
  g_fn_prtlin( "    .cNam        { color:#3f3ffa;");
  g_fn_prtlin( "                   background-color: #F7ebd1;");
  g_fn_prtlin( "                   font-size: 133%;");
  g_fn_prtlin( "    }");
  g_fn_prtlin( "    .cNam2       { color:#3f3ffa; }");
  g_fn_prtlin( "    .cCat        { background-color:#fdfbe1; }");
/* TABLE TABLE TABLE TABLE TABLE TABLE TABLE TABLE */

  g_fn_prtlin( "  </style>");

/*   g_fn_prtlin( "    <!-- "); */
/*   g_fn_prtlin( "    PRE {line-height: 68%}; "); */
/*   g_p_fn_prtlin( "    P {margin-left:10%; margin-right:10%}"); */
/*   g_p_fn_prtlin( "    --> "); */

    
    /* put in favicon */
    g_fn_prtlin("<link href=\"data:image/x-icon;base64,iVBORw0KGgoAAAANSUhEUgAAAB0AAAAdCAYAAABWk2cPAAAD8GlDQ1BJQ0MgUHJvZmlsZQAAOI2NVd1v21QUP4lvXKQWP6Cxjg4Vi69VU1u5GxqtxgZJk6XpQhq5zdgqpMl1bhpT1za2021Vn/YCbwz4A4CyBx6QeEIaDMT2su0BtElTQRXVJKQ9dNpAaJP2gqpwrq9Tu13GuJGvfznndz7v0TVAx1ea45hJGWDe8l01n5GPn5iWO1YhCc9BJ/RAp6Z7TrpcLgIuxoVH1sNfIcHeNwfa6/9zdVappwMknkJsVz19HvFpgJSpO64PIN5G+fAp30Hc8TziHS4miFhheJbjLMMzHB8POFPqKGKWi6TXtSriJcT9MzH5bAzzHIK1I08t6hq6zHpRdu2aYdJYuk9Q/881bzZa8Xrx6fLmJo/iu4/VXnfH1BB/rmu5ScQvI77m+BkmfxXxvcZcJY14L0DymZp7pML5yTcW61PvIN6JuGr4halQvmjNlCa4bXJ5zj6qhpxrujeKPYMXEd+q00KR5yNAlWZzrF+Ie+uNsdC/MO4tTOZafhbroyXuR3Df08bLiHsQf+ja6gTPWVimZl7l/oUrjl8OcxDWLbNU5D6JRL2gxkDu16fGuC054OMhclsyXTOOFEL+kmMGs4i5kfNuQ62EnBuam8tzP+Q+tSqhz9SuqpZlvR1EfBiOJTSgYMMM7jpYsAEyqJCHDL4dcFFTAwNMlFDUUpQYiadhDmXteeWAw3HEmA2s15k1RmnP4RHuhBybdBOF7MfnICmSQ2SYjIBM3iRvkcMki9IRcnDTthyLz2Ld2fTzPjTQK+Mdg8y5nkZfFO+se9LQr3/09xZr+5GcaSufeAfAww60mAPx+q8u/bAr8rFCLrx7s+vqEkw8qb+p26n11Aruq6m1iJH6PbWGv1VIY25mkNE8PkaQhxfLIF7DZXx80HD/A3l2jLclYs061xNpWCfoB6WHJTjbH0mV35Q/lRXlC+W8cndbl9t2SfhU+Fb4UfhO+F74GWThknBZ+Em4InwjXIyd1ePnY/Psg3pb1TJNu15TMKWMtFt6ScpKL0ivSMXIn9QtDUlj0h7U7N48t3i8eC0GnMC91dX2sTivgloDTgUVeEGHLTizbf5Da9JLhkhh29QOs1luMcScmBXTIIt7xRFxSBxnuJWfuAd1I7jntkyd/pgKaIwVr3MgmDo2q8x6IdB5QH162mcX7ajtnHGN2bov71OU1+U0fqqoXLD0wX5ZM005UHmySz3qLtDqILDvIL+iH6jB9y2x83ok898GOPQX3lk3Itl0A+BrD6D7tUjWh3fis58BXDigN9yF8M5PJH4B8Gr79/F/XRm8m241mw/wvur4BGDj42bzn+Vmc+NL9L8GcMn8F1kAcXgSteGGAAAACXBIWXMAAAsTAAALEwEAmpwYAAABWWlUWHRYTUw6Y29tLmFkb2JlLnhtcAAAAAAAPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iWE1QIENvcmUgNS40LjAiPgogICA8cmRmOlJERiB4bWxuczpyZGY9Imh0dHA6Ly93d3cudzMub3JnLzE5OTkvMDIvMjItcmRmLXN5bnRheC1ucyMiPgogICAgICA8cmRmOkRlc2NyaXB0aW9uIHJkZjphYm91dD0iIgogICAgICAgICAgICB4bWxuczp0aWZmPSJodHRwOi8vbnMuYWRvYmUuY29tL3RpZmYvMS4wLyI+CiAgICAgICAgIDx0aWZmOk9yaWVudGF0aW9uPjE8L3RpZmY6T3JpZW50YXRpb24+CiAgICAgIDwvcmRmOkRlc2NyaXB0aW9uPgogICA8L3JkZjpSREY+CjwveDp4bXBtZXRhPgpMwidZAAAICElEQVRIDY1XXWxUxxX+5t679+5617v4Z+3FmJ+UopJQeADLqtOIpo5KayFVbSoa0RaqpsipVKEiWomSl6ipVEyjqkril4inIkWgpKhSQ2IoP6IK5YG0FiSAq+IqpsYUY8cYsNm/uzP9zlzfxVXVqiPNztyZM+eb75wzZ2bV2bNnNVh838f09DQuXryIc+fO4cKFCwiCANVqFVpbERH7r0WpaMoYcF0S5XIJ69evR2/vF9HT04POzuVWl6GAEtBEIqGuXr1qBgcHceXKFbs6mUyqUqkkfaqBqKy3C/plzhblQHFfdj4IlEmnDdLpFMbHi3a+oyOn9uzZb7q7P6cIavCn8+f1oUOHhIqthUJBZ7NZ21dKaanxnEMZqfE3t1LvJ5PQ+bzSLS0iH69pYL9Qlzl4cEC///55jePHj2vStxPLli2rCziOU+8LyGIwn0ozjtJptgnO5duVbiVgvJmeHle/8kqgT59O6UuXEnpoqEN3d8sGG/TRo0c0BgYGrHA+n7etgC0GXAzWRNYFAuUWsW1KPWK7ZYur3347pSfvZLQxjQs1yzalT57ssPp37/6h9mIfiqnjEgcOAa3GJKNkCfu3YxmJGpcDpHm3aNC1xcWPvpvA1i97aGqJPC6iUms1g0TCQ2vrnFU/NHQG3ujoqHyocrlsUWNwAlp7NSpluFrdpo4VrquedRyzhs5sDIDKnMEkF2/+dkI99S1P1hMx2pnsS3qeJ+qhRkZEf1LNzPzVeLOzs3Y0DEPbyk/MkIB4ICsdhR8nEtjGdqkYiUPVikEpoVBKsn0pxNW/aNzb5OB3oyFWtit8j8zTmYj17KzBm2/WuDBEMpmCR/8JjmGUSmuL6G0gwzDaNF73ffMdzvs1Y+QQlFlDoyBGUEWF6pgx/3wtRABlHnJuN6r42le9Oug774RmaChEoeCYW7eKiMiT/oJZqcoSQZomnWL/Z4Fvvl9SyudwlTBpth4/HAKKNTXbhlal5h2YoAHq+TFlvrAnQK5NWCjz4Yc17NxZsmpLJau+DioabBGWLZSf4i66Axc7yw5STQT8vEKCijFM0ZtkmmQcUZgWhjfNjTDSHj6AyVDkK9tc+twx01Ma+18Uu8AUCgq3GYliWGtbDspOokKdQfSlnmPgdHC0miPF1Vz5GOWWKLvIpQxdDIfykpHcLAOraFT2gIvskw7mGTcvv1zGe++G6OhQioCCIpnrP5mmeBSmGObIOWYdGYuT1H36/BJBXdJgUHA2ilEqoM3hroKpjBks+aZjVu3hOWK5cLCK1werSBWAeVpCxsjQiCVjn0ZUuXOPQVZsAtJ3WSlQzhi4MwrBH+06SAxW6FPeAwgpb1ZRhoCpHgfrB334NPv0L0M4L2msbHNx434NyQoXxYjs1kEtKvVmW3lMpg3WfEohKX4aJhMeixoFJDFaUB6XKs1Z43yRgN6TCp855iOVVxgd4G7215BqceDJfUFLOZJIuJB7tJRjn9qdt7QCE9NiAODV3wRY+qyDu8xJJQLMM0rnCDZP05dosnKC3//Q8Lc7+Oy7BGSgjOyvYoaAyTb6lCB/v08WRKjJTlkiWy1imqYtS9FNhN++lcLmpzzc+aQGGVIMCo97cWgFQ/NVbxKYKnI/d/HYiwkJIFzqr6DIyypLcJfsbgigq9FCwHtyvGJE6qubN51WmJgADhwI8I1tMmwwytAUC3kSmfSzGTPMKdzApxU6Xkugrc/FvY8Nrv2ggtofNDKdDhoYC8V54JTPXdKXCQajJBkxaWRD6pOkQ5ZqYsKYrVtdvPBCFH1krV49VsVKxunXx6HIzPBKU22/cM2KXR6CvIOJUyFG+6pw6fD0csck7kBlQ2XeCwzeqoT2kpiJKMZcrVs9l06en49m9u3z0dQk4zQlE0GegXKUU5ufc83azQ5av+SYJWscVHnerx2sYPKnzKUMj1SnMqlxoNlXZphG+klIm5KMpNZKxNIeFaqNjNzV9YSw1rt2pXW5HN2DtVp0F96bzeiPx9L6/gMZl3sxq+f/nNbX+nz9AfPRlUZfX2/39Q34eiwI9BHWTpeOpD5mRtuSk21lLK6e3Hcs6plnHMO3WT3SQibabE4hyyQh87dua/P7w6FS+2r2ast1OKjQd/d5t90k3VM1bQ6FPLlMn5JGpxf8uBC4iheL4T2t+PYyXnMzzwlpJ5MSPCHE3LZI9mG5c0fjzOnQ/PpXFXwwrDmm8ETBwdIpuoFHcpzX6N/KHCcIA9ukCPgJ+/GZFB0LgEilUqZY5Hno73/e0t6+Pa9HR1M0caOemcnojz5K6zfeSOqnn47MxbV6eafS2cZHZpIxqfJ8aac5kwsPNUZq3ZTxwy6TydixTZs2ae/xxzdwHdSRIzUzPFxBV1cFMzMOhoaqUURx9228pkjAjN80KsHrqonWkBtGilz9fIioe0JVQodyMiWL5UMKLa4Iaubm5lRvb6/ByZMndV9fn92F7y+3LeVsm8+TQTv4Lo6+XecRg1gmbqn+39bG49LGr8zVq1frY8eOaS/NQ7pjxw7IW+n69etYsaKFcg5KPKhzc1U8ZHbhQ5/PDNlxlE1iE1DQFuv8+GOhdRkc9CFjxGXSYdZh2bt3L9rJQp05c0bzL4UaHx83hw8fxokTJxaWWQtJX3QKzuJWxheX/zm/ceNG1d/fb9atW6f4N8XYvxXyVJH/LfPMEvx7gcuXL2NkZASTk5MSeVa5yPw/RfwoT9hcLoe1a9diw4YNtjY3N6NSkTsO+BcbeuPABIyNOwAAAABJRU5ErkJggg==\" rel=\"icon\" type=\"image/x-icon\" />");

    
    
    
/* put in favicon */
    // old favicon    g_fn_prtlin("<link href=\"data:image/x-icon;base64,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\" rel=\"icon\" type=\"image/x-icon\" />");

  g_fn_prtlin( "</head>");
  g_fn_prtlin( " ");
  g_fn_prtlin("\n<body>");

/*   g_fn_prtlin( "<body background=\"file:///mkgif1g.gif\">"); */
/*   g_fn_prtlin( "<body background=\"file:///C/USR/RK/html/mkgif1g.gif\">"); */

 
  int sizeLongestFld;
  char myleftmargin[32], name1_and[32];

  sizeLongestFld = sprintf(name1_and, "%s  and", arr(0)); // name1 always has "  and"
  if ((int)strlen(arr(1)) > sizeLongestFld) sizeLongestFld = (int)strlen(arr(1));

  // then max 15 char name, then 4 char " and"     both centered in 19 char field
  // extra sp on left

  // centre "name1  and" in a fld of max 19 (15 + 4)
  //
       if (sizeLongestFld ==  5) { strcpy(myleftmargin, "5.3em;"); }
  else if (sizeLongestFld ==  6) { strcpy(myleftmargin, "5.0em;"); }
  else if (sizeLongestFld ==  7) { strcpy(myleftmargin, "4.8em;"); }
  else if (sizeLongestFld ==  8) { strcpy(myleftmargin, "4.5em;"); }
  else if (sizeLongestFld ==  9) { strcpy(myleftmargin, "4.3em;"); }
  else if (sizeLongestFld == 10) { strcpy(myleftmargin, "4.0em;"); }
  else if (sizeLongestFld == 11) { strcpy(myleftmargin, "3.8em;"); }
  else if (sizeLongestFld == 12) { strcpy(myleftmargin, "3.5em;"); }
  else if (sizeLongestFld == 13) { strcpy(myleftmargin, "3.3em;"); }
  else if (sizeLongestFld == 14) { strcpy(myleftmargin, "3.0em;"); }
  else if (sizeLongestFld == 15) { strcpy(myleftmargin, "2.8em;"); }
  else if (sizeLongestFld == 16) { strcpy(myleftmargin, "2.5em;"); }
  else if (sizeLongestFld == 17) { strcpy(myleftmargin, "2.3em;"); }
  else if (sizeLongestFld == 18) { strcpy(myleftmargin, "2.0em;"); }
  else if (sizeLongestFld == 19) { strcpy(myleftmargin, "1.8em;"); }
  else                           { strcpy(myleftmargin, "1.5em;"); }

  sprintf(writebuf,  "<pre style=\"margin-left: %s;\" class=\"myTitle\" >", myleftmargin); 
  g_fn_prtlin(writebuf);
  gbl_we_are_in_PRE_block = 1;

  g_fn_prtlin(name1_and);   // name1
  g_fn_prtlin(arr(1));      // name2

  gbl_we_are_in_PRE_block = 0;
  g_fn_prtlin("</pre>");



  // g_fn_prtlin(" ");

}  // end of  put_ios_top_of_just2_group_rpt(void)  /* just_2 rpt */


// iiiiiiiiiiiiiiiiiiiiiiiiiiiiiii   webview / ios  html code  iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii
// iiiiiiiiiiiiiiiiiiiiiiiiiiiiiii   webview / ios  html code  iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii
// iiiiiiiiiiiiiiiiiiiiiiiiiiiiiii   webview / ios  html code  iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii




/* end of grphtm.c */
