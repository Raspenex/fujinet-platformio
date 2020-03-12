#include <stdio.h>
#include <string.h>

#include <iomanip>
#include <iostream>
#include <fstream>
//using namespace std;

#define EOL 155
#define EOLS -101

// todo:
// specify: page size, left/bottom margins, line spacing
// sio simulation: read input file, make 40-char buffers, replace CRLF with EOL, pad with spaces to 40 char
// using simulated sio buffer to create text stream payload for PDF lines
// use standard C and POSIX file output (don't care about input because that's SIO simulation)
// replace page(string) with incremental line string and accumulate lengths for pdf_xref
// use NUMLINES to parameterize the routine

std::ifstream prtin; // input file

FILE *f; // standard C output file

bool BOLflag=true;
float svg_X=0.;
float svg_Y=0.;
float printWidth = 480.;
float charWidth = 12.;
float lineHeight = 17.5;
float fontSize = 17.5;

void svg_new_line()
{
  // <text x="0" y="15" fill="red">I love SVG!</text>
  // position new line and start text string array
  fprintf(f,"<text x=\"0\" y=\"%g\" font-size=\"%g\" font-family=\"ATARI 1020 VECTOR FONT APPROXIM\" fill=\"black\">", svg_Y,fontSize);
  svg_X = 0; // reinforce?

  BOLflag = false;
}

void svg_end_line()
{
  // <text x="0" y="15" fill="red">I love SVG!</text>
  fprintf(f,"</text>\n"); // close the line
  svg_Y += lineHeight;     // line feed
  svg_X = 0.;               // CR
  BOLflag = true;
}

void svg_handle_char(unsigned char c)
{
  // simple ASCII printer
  if (c > 31 && c < 127)
  {
    // if (c == BACKSLASH || c == LEFTPAREN || c == RIGHTPAREN)
    //   _file->write(BACKSLASH);
    fputc ( c , f); //_file->write(c); 
    svg_X += charWidth; // update x position
    std::cout << svg_X << " ";
  }
}



void svg_header()
{
// <!DOCTYPE html>
// <html>
// <body>
// <svg height="210" width="500">
//fprintf(f,"<!DOCTYPE html>\n");
//fprintf(f,"<html>\n");
//fprintf(f,"<body>\n\n");
fprintf(f,"<svg height=\"2000\" width=\"480\" viewBox=\"0 -1000 480 2000\">\n");
}

void svg_footer()
{
// </svg>
// </body>
// </html>
 if (!BOLflag)
    svg_end_line();
fprintf(f,"</svg>\n\n");
//fprintf(f,"</body>\n");
//fprintf(f,"</html>\n");
}


void svg_add(std::string S)
{
  // loop through string
  for (int i = 0; i < S.length(); i++)
    { 
      unsigned char c = (unsigned char)S[i];
    std::cout << "c=" << c << " ";
    if (BOLflag && c == EOL)
       svg_new_line();

    // // check for EOL or if at end of line and need automatic CR
    if (!BOLflag && ((c == EOL) || (svg_X > (printWidth - charWidth))))
      { svg_end_line();
  
      }
    // // start a new line if we need to
    if (BOLflag)
       svg_new_line();

    // disposition the current byte
    svg_handle_char(c);}
}

int main()
{

  std::string payload;

  int j;

  prtin.open("in.txt");
  if (prtin.fail())
    return -1;
  f = fopen("out.svg", "w");

  svg_header();
  //SIMULATE SIO:
      //standard Atari P: handler sends 40 bytes at a time
      //break up line into 40-byte buffers
  do {
    payload.clear();
      char c='\0';
      int i=0;
      while (c!=EOLS && i<40 && !prtin.eof())  
      {
        prtin.get(c);
        if (c=='\n')
          c=EOLS;
        payload.push_back(c);
        i++;
      }
      std::cout << payload << ", ";
    svg_add(payload);
    fflush(f);
} while (!prtin.eof());
  svg_footer();
  fclose(f);
  return 0;
}
