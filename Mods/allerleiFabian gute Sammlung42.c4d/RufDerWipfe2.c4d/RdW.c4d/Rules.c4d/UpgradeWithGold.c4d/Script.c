#strict 3

protected func Activate(caller) {
  MessageWindow(GetDesc(), caller);
  return(1);
}
