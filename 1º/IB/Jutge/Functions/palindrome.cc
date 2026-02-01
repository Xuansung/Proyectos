#include <string>
bool is_palindrome(const std::string& s){
  for (int i = 0; i < s.length(); i++) {
    if (s[i] != s[s.length() - (i + 1)]) {
      return false;
    }
  }
  return true;
}