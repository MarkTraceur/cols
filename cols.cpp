/*
cols -- take a file from the command line and cut it down to a specified number of columns, preserving whole words, then put it in the target file. Default 80 columns.

This program is licensed under the WTFPL, found at http://sam.zoy.org/wtfpl. I don't mean to be vulgar, but this best specifies my wishes for the code.

Original code written by Mark Holmquist at the University of Redlands in 2010.
*/

#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

void usage() {
  cout << "Usage: cols [-c column_number] [-s sourcefile] [-t targetfile]\n";
}

void help() {
  cout << "cols [infile [outfile]]-- take STDIN and cut it down to a specified number of columns, preserving whole words, and output the result to STDOUT.\n\nOPTIONS\n  -h/-? : Display this help message.\n  -c <cols> : Specify the number of columns to preserve (default 80).\n  -s <sourcefile> : Designate a source file, rather than taking input from STDIN.\n  -t <targetfile> : Desginate a target file, rather than outputting to STDOUT.\n";
}

void parse(istream &istrm, ostream &ostrm, int cols) {
  string tmp, tmp1, endout = "";
  bool islong = false; // Needed to preserve the endlines in the original file
  int loc=0;

  while (!istrm.eof()) {
    getline(istrm, tmp);
    while (tmp.size() > cols) {
      islong = true;
      tmp1 = tmp.substr(0, cols);
      loc = tmp1.find_last_of(' '); // Find last whole word.
      if (loc != string::npos) { // Make sure we have separate words
	endout.append(tmp1.substr(0, loc));
	endout.append("\n");
	tmp = tmp.substr(loc+1);
      }
      else { // If the whole section doesn't have a space, cut it to one less than the max, and add a hyphen. TODO: Make this content-sensitive (preserve syllables, etc.)
	endout.append(tmp1.substr(0,cols-1));
	endout.append("-\n");
	tmp = tmp.substr(cols-1);
      }
    }
    endout.append(tmp); // Now that we know what to append, throw it in.
    if ((!istrm.eof() || (tmp == "\n" && istrm.eof()))) { // Make sure we aren't adding endlines we don't need
      endout.append("\n");
      islong = false;
    }
  }
  ostrm << endout;
}
  

int main(int argc, char const **argv) {
  string origfile = "";
  string targfile = "";
  int cols = 80;

  for (int i = 1; i < argc; i++) { // Parsing all arguments.
    if (argv[i][0] == '-') { // Options, denoted by a hyphen. All single-letter.
      if (argv[i][1] == 'c')
	cols = atoi(argv[++i]);
      else if (argv[i][1] == 's') {
	origfile = argv[++i];
      }
      else if (argv[i][1] == 't') {
	targfile = argv[++i];
      }
      else if (argv[i][1] == 'h' || argv[i][1] == '?') {
	help();
	return 0;
      }
      else { // Send them to boarding school if they use any unknown options.
	usage();
	return 0;
      }
    }
  }
  if (origfile != "") {
    ifstream fin(origfile.c_str());
    if (targfile != "") {
      ofstream fout(targfile.c_str());
      parse(fin, fout, cols);
      fout.close();
    }
    else
      parse(fin, cout, cols);
    fin.close();
  }
  else {
    if (targfile != "") {
      ofstream fout(targfile.c_str());
      parse(cin, fout, cols);
      fout.close();
    }
    else
      parse(cin, cout, cols);
  }

  return 0;
}
