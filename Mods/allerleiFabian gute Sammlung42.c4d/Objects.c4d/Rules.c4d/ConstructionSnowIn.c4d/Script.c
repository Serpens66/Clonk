#strict 3

protected func Activate(caller) {
  MessageWindow(GetDesc(), caller);
  return(1);
}

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsChooseable2() { return(1); }