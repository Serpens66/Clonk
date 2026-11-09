/*-- Forschung anpassen --*/

#strict


func Initialize()
{
SetPosition(1,1);
}

// dies ist nur ein hilfsobjekt. Der Faktor wird im Chooser gesetzt und im Forschungsgebäude abgefragt


func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}

// Kann mittels des neuen Spielzielauswählers variabel eingestellt werden
public func IsVariabelChooseable2() { return(1); }
