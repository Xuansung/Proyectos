void min_max(int a, int b, int& mn, int& mx) {
  if ( a >= b) {
    mx = a;
    mn = b;
  } else {
    mn = a;
    mx = b;
  }
}