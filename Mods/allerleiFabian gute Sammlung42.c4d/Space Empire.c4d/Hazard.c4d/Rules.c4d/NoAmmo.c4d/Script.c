#strict 3

protected func Activate(iPlr) {
  MessageWindow(GetDesc(),iPlr);
}

global func NoAmmo() {
  return(ObjectCount(NOAM));
}

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsChooseable2() { return(1); }
public func IsChooseable() { return(1); }


