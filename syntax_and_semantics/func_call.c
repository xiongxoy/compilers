int bar( int a, int b) {
    return a+b;
}
int foo( int a, int b) {
   return bar(a,b);
}
int main() {
    return foo(1,2);
}
