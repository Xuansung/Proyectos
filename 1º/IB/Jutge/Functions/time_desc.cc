void decompose(int n, int& h, int& m, int& s) {
  h = n / 3600;
  m = n % 3600 / 60;
  s = n % 3600 % 60;
}