// vim: set et ts=2 sw=2 :
// Copyright 2014 Shaun Ruffell <sruffell@digium.com>

// The STL regex library is not supported on GCC until version 4.9, which isn't
// on many standard distributions. Therefore, I'll just use the boost library
// for the time being.
#include <boost/regex.hpp>

#include <cstdio>

#include <algorithm>
#include <array>
#include <set>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::unique_ptr;
using std::set;
using std::function;

using boost::regex;
using boost::regex_search;
using boost::cmatch;

// Strips the chars from the front and back of the string pointed to by str, and
// returns str.
static inline string* strip_in_place(string* str, const char *chars) {
  str->erase(str->find_last_not_of(chars) + 1);
  str->erase(0, str->find_first_not_of(chars));
  return str;
}

// Returns an ordered collection of regular expressions loaded from the
// filename.
vector<regex> load_patterns(const char *filename) {
  vector<regex> res;
  // unique_file_ptr will automatically close the FILE* when it is destructed.
  auto close_file = [](FILE* fp) { if (fp) { ::fclose(fp); } };
  using unique_file_ptr = unique_ptr<FILE, decltype(close_file) >;
  unique_file_ptr fin(fopen(filename, "r"), close_file);

  if (fin) {
    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), fin.get())) {
      if ('#' == buffer[0]) {
        continue;
      }
      string pattern(buffer);
      strip_in_place(&pattern, " \r\n\t");
      if (0 == pattern.size())
        continue;
      res.emplace_back(pattern, regex::icase);
    }
  } else {
    fprintf(stderr, "Failed to open: %s\n", filename);
  }
  return res;
}

// Functor to sort case insensitively first, then by case.
//
// This is used to enforce strict ordering of what we want to be an otherwise
// case insensitive sort operations.
class custom_string_cmp {
 public:
  bool operator()(const string& lhs, const string& rhs) {
    auto value = strcasecmp(lhs.c_str(), rhs.c_str());
    if (0 == value) {
      return strcmp(lhs.c_str(), rhs.c_str()) < 0;
    } else {
      return value < 0;
    }
  }
};

// Reads from fin and returns a std::set with all the found authors, using the
// collection of patterns to find the authors in the stream.
template<typename T> set<string, custom_string_cmp>
get_authors(FILE* fin, const T& collection) {
  set<string, custom_string_cmp> authors;
  char buffer[BUFSIZ];
  while (fgets(buffer, sizeof(buffer), fin)) {
    for (const auto& regex_ : collection) {
      cmatch match;
      if (regex_search(buffer, match, regex_)) {
        string author_name = match.str(1);
        authors.emplace(*strip_in_place(&author_name, " \n\r\t."));
        break;  // Go to next line on first match
      }
    }
  }
  return authors;
}

int main(int argc, char* argv[]) {
  auto authors = get_authors(stdin, load_patterns("patterns.txt"));
  for (const auto& author : authors) {
    fputs(author.c_str(), stdout);
    fputc('\n', stdout);
  }
  return 0;
}
